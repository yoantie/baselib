#ifndef _user_def_h__
#define _user_def_h__

#include <memory.h>

#define MAX_FREE_UNLIMITED	0
#define MAX_INDEX			512

#define BOUNDARY_INDEX		10 
#define BOUNDARY_SIZE		1024
#define BOUDNARY_ALIGN(size, base) \
	( (size + base - 1) & ~( base -1 ) ) \

#define DEFAULT_ALIGN(size) \
	BOUDNARY_ALIGN(size, 8)

struct MemNode;
struct Allocator;

struct PoolHead
{
	MemNode* active;
	Allocator* allocator;

	PoolHead() {
		this->active = NULL;
		this->allocator = NULL;
	}
};

struct Allocator
{
	int max_index;
	int max_free_index;			
	int current_free_index;

	MemNode* free[MAX_INDEX];

	Allocator() {
		memset(this, 0, sizeof(*this));
	}
};

struct MemNode
{
	MemNode* ref;
	MemNode* next;

	int free_index;//剩余容量
	int index;//列表大小
	char* first_avail;
	char* endp;
};

#define list_insert(node, point) \
	do {	\
	  point->ref->next = node; \
	  node->ref = point->ref; \
	  point->ref = node; \
	  node->next = point; \
	} while ( false ) 

#define list_remove(node) \
	do { \
	  node->ref->next = node->next; \
	  node->next->ref = node->ref; \
	} while ( false ) 


#endif