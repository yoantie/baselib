#include "StdAfx.h"
#include "SimplePool.h"

CSimplePool::CSimplePool(void)
{
	m_pool = NULL;
	m_list = NULL;
}

CSimplePool::~CSimplePool(void)
{
	Destroy();
}

void CSimplePool::Create()
{
	m_pool = new CMemPool;
	if ( !m_pool ) 
		return;

	MemNode* block = (MemNode*)m_pool->AllocNode(ALLOC_AMT);
	if ( !block ) 
		return;
	
	m_list = (node_alloc*)block->first_avail;
	m_list->pool = m_pool;
	m_list->freelist = NULL;
	m_list->blocks = block;
	block->first_avail += DEFAULT_ALIGN(sizeof(node_alloc));
}

void CSimplePool::Destroy()
{
	if ( m_pool ) {
		m_pool->FreeNode( m_list->blocks );

		delete m_pool;
		m_pool = NULL;
	}
}

void* CSimplePool::Alloc(unsigned int size)
{
	assert(m_pool && m_list && "not initialized" );

	node_header* node;
	MemNode* active = m_list->blocks;
	char* endp;

	size += sizeof(node_header);
	if ( size <= SMALL_NODE_SIZE ) { //基本上不会用到
		if ( m_list->freelist ) {
			node = m_list->freelist;
			m_list->freelist = node->next;
		} else {
			endp = active->first_avail + SMALL_NODE_SIZE;
			if ( endp >= active->endp ) {
				m_list->blocks = m_pool->AllocNode( ALLOC_AMT );
				if ( !m_list->blocks ) 
					return NULL;
				
				m_list->blocks->next = active;
				active = m_list->blocks;
				endp = active->first_avail + SMALL_NODE_SIZE;
			}

			node = (node_header*)active->first_avail;
			node->alloc = m_list;
			node->memnode = active;
			node->size = SMALL_NODE_SIZE;
			active->first_avail = endp;
		}
	} else {
		MemNode* memnode = m_pool->AllocNode(size);
		if ( !memnode )
			return NULL;

		node = (node_header*)memnode->first_avail;
		node->alloc = m_list;
		node->memnode = memnode;
		node->size = size;
	}

	return ((char*)node + sizeof(node_header));
}

//自觉。。
void CSimplePool::Free(void *mem)
{
	node_header* node = (node_header*)((char*)mem - sizeof(node_header));
	node_alloc* list = node->alloc;

	if ( node->size == SMALL_NODE_SIZE ) {
		node->next = list->freelist;
		list->freelist = node;
	} else {
		m_pool->FreeNode(node->memnode);
	}
}
