#ifndef _memory_pool_2013_h__
#define _memory_pool_2013_h__

#pragma once

#include "pooldef.h"
#include <assert.h>

class CMemPool
{
	PoolHead* m_pool_head;

	void Init();
	void Uninit();

public:
	CMemPool(void);
	~CMemPool(void);
	
	void* AllocBuffer(int size);
	MemNode* AllocNode(int size);
	void FreeNode(MemNode* node);
};

#endif