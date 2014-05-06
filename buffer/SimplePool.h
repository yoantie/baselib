#ifndef _simple_pool_h_2013__
#define _simple_pool_h_2013__

#pragma once

#include "MemPool.h"

#define MEMNODE_SIZE BOUDNARY_ALIGN(sizeof(MemNode), 8)
#define ALLOC_AMT (2048 - MEMNODE_SIZE)

struct node_alloc;

struct node_header {
	unsigned int size;
	node_alloc* alloc;
	MemNode* memnode;
	node_header* next;
};

struct node_alloc {
	CMemPool* pool;
	node_header* freelist;
	MemNode* blocks;
};

#define SMALL_NODE_SIZE (sizeof(CMemPool) + sizeof(node_alloc))

class CSimplePool
{
	CMemPool* m_pool;
	node_alloc* m_list;

public:
	void Destroy();

public:
	CSimplePool(void);
	~CSimplePool(void);

	void Create();
	void* Alloc(unsigned int size);
	void Free(void* mem);
};

#endif

