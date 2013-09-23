#include "StdAfx.h"
#include "MemPool.h"
#include <stdio.h>
#include <stdlib.h>

CMemPool::CMemPool(void)
{
	Init();
}

CMemPool::~CMemPool(void)
{
	Uninit();
}

void CMemPool::Init()
{
	m_pool_head = new PoolHead;
	if ( m_pool_head == NULL ) return;

	m_pool_head->allocator = new Allocator;
	if ( m_pool_head->allocator == NULL ) return;
	
	m_pool_head->allocator->max_free_index = MAX_INDEX;
	m_pool_head->allocator->current_free_index = MAX_INDEX;

	//active暂未使用
	//int size = BOUNDARY_SIZE - DEFAULT_ALIGN(sizeof(MemNode));
	//MemNode* node = AllocNode(size);//预分配一个单位
	//if ( node == NULL ) return;

	//node->next = node;
	//m_pool_head->active = node;
}

void CMemPool::Uninit()
{
	assert(m_pool_head && "m_pool_head is null" );
	Allocator* allocator = m_pool_head->allocator;

	assert(allocator && "allocator is null" );

	unsigned int index;
	MemNode *node, **ref;

	for( index = 0; index < MAX_INDEX; index++ ) {
		ref = &m_pool_head->allocator->free[index];
		while ( (node = *ref) != NULL ) {
			*ref = node->next;
			free(node);
		}
	}

	free(m_pool_head->allocator);
	free(m_pool_head);
}

void* CMemPool::AllocBuffer(int size)
{
	if ( m_pool_head == NULL || m_pool_head->allocator == NULL ) 
		return NULL;

	void* pbuffer = NULL;
	size = DEFAULT_ALIGN(size);
	MemNode* node = NULL;

	//1.查找(active是按序排列的)
	MemNode* active = m_pool_head->active;
	if ( active ) {
		if ( size <= (active->endp - active->first_avail) ) {
			pbuffer = active->first_avail;
			active->first_avail += size;
			return pbuffer;
		}
	}

	node = active->next;
	if ( node && ( size <= (node->endp - node->first_avail) ) ) {
		list_remove(node);
	} else {
		node = AllocNode(size);
		if ( node == NULL ) 
			return NULL;
	}
		
	node->free_index = 0;

	pbuffer = node->first_avail;
	node->first_avail += size; 
	
	/*插入到active位置
	  以防需要简单处理
	*/
	list_insert(node, active);
	m_pool_head->active = node;

	//重新计算active->free_index,
	int free_index = (BOUDNARY_ALIGN( active->endp - active->first_avail + 1, BOUNDARY_SIZE ) - BOUNDARY_SIZE ) >> BOUNDARY_INDEX;
	active->free_index = free_index;

	node = node->next;
	if ( free_index >= node->free_index ) 
		return pbuffer;

	do {
		node = node->next;
	} while ( node && free_index < node->free_index );

	//将active从链表中删除，插入node前面：排序
	list_remove(active);
	list_insert(active, node);

	return pbuffer;
}

MemNode* CMemPool::AllocNode(int in_size)
{
	assert(m_pool_head && "m_pool_head is null" );
	Allocator* allocator = m_pool_head->allocator;

	MemNode *node, **ref;
	int i, index, max_index;
	int size;

	size = BOUDNARY_ALIGN(in_size + DEFAULT_ALIGN(sizeof(MemNode)), BOUNDARY_SIZE);
	size = size > BOUNDARY_SIZE ? size : BOUNDARY_SIZE;
		
	index = ( size >> BOUNDARY_INDEX ) - 1; 
	if ( index <= allocator->max_index ) {
		max_index = allocator->max_index;
		ref = &allocator->free[index];
		i = index;
		while ( *ref == NULL && i < max_index ) {
			ref++;
			i++;
		}

		if ( (node = *ref) != NULL ) {
			if ( (*ref = node->next) == NULL && i >= max_index ) {
				do {
					ref--;
					max_index--;
				} while ( *ref == NULL && max_index > 0 );

				allocator->max_index = max_index;
			}

			allocator->current_free_index += node->index;
			if ( allocator->current_free_index > allocator->max_free_index ) {//current_free_index max_free_index实为总数
				allocator->current_free_index = allocator->max_free_index;
			}

			node->next = NULL;
			node->first_avail = (char*)node + DEFAULT_ALIGN(sizeof(MemNode));
			return node;
		}
	} else if ( allocator->free[0] ) {
		ref = &allocator->free[0];
		while ( (node = *ref) != NULL && index > node->index ) {
			ref = &node->next;
		}

		if ( node ) {
			*ref = node->next;
			allocator->current_free_index += node->index;
			if ( allocator->current_free_index > allocator->max_free_index ) {//current_free_index max_free_index实为总数
				allocator->current_free_index = allocator->max_free_index;
			}

			node->next = NULL;
			node->first_avail = (char*)node + DEFAULT_ALIGN(sizeof(MemNode));
			return node;
		}
	}

	if ( (node = (MemNode*)malloc(size)) == NULL ) 
		return NULL;

	memset(node, 0, size);
	
	node->next = NULL;
	node->index = index;
	node->first_avail = (char*)node + DEFAULT_ALIGN(sizeof(MemNode));
	node->endp = (char*)node + size;

	return node;
}

void CMemPool::FreeNode(MemNode* node)
{
	//对node的判断
	assert(m_pool_head && node && "m_pool_head is null" );
	Allocator* allocator = m_pool_head->allocator;

	assert(allocator && "allocator is null" );

	MemNode* next, *freelist = NULL;
	unsigned int index, max_index;
	unsigned int max_free_index, current_free_index;

	max_index = allocator->max_index;
	max_free_index = allocator->max_free_index;
	current_free_index = allocator->current_free_index;

	do {
		memset(node->first_avail, 0, node->endp - node->first_avail);

		next = node->next;
		index = node->index;

		//free[0]未使用，暂保留
		if ( max_free_index != MAX_FREE_UNLIMITED && index > current_free_index ) {
			node->next = freelist;
			freelist = node;
		} else if ( index < MAX_INDEX ) {
			if ( (node->next = allocator->free[index]) == NULL
				&& index > max_index ) {
				max_index = index;
			}

			allocator->free[index] = node;
			if ( current_free_index >= index ) 
				current_free_index -= index;
			else
				current_free_index = 0;
		} else {
			node->next = allocator->free[0];
			allocator->free[0] = node;
			if ( current_free_index >= index ) 
				current_free_index -= index;
			else
				current_free_index = 0;
		} 

	} while ( (node = next) != NULL );

	allocator->max_index = max_index;
	allocator->current_free_index = current_free_index;

	while ( freelist != NULL ) {
		node = freelist;
		freelist = node->next;
		free(node);
	}
}