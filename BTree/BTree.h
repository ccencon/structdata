#ifndef _B_TREE__
#define _B_TREE__
#include "../common.h"
#include <stdbool.h>
#include <unistd.h>

typedef struct HeaderNode{
	off_t rootPointer;
	size_t keySize;
	size_t valSize;
	size_t pageSize;
}HeaderNode;

typedef bool(*BKeyCompareFuncT)(const void*, const void*);
typedef void(*BForEachFuncT)(const void*, const void*);
typedef struct BTree{
	HeaderNode head;
	BKeyCompareFuncT equalFunc;
	BKeyCompareFuncT lessFunc;
	void* tmpRet;
	int fd;
}BTree;

typedef struct BTreeOp{
	BTree* (*create)(size_t, size_t, BKeyCompareFuncT, BKeyCompareFuncT, const char*);
	void (*destroy)(BTree**);
	void (*level_order_traverse)(BTree*, BForEachFuncT);
	void (*traverse)(BTree*, BForEachFuncT);
	void (*insert)(BTree*, const void*, const void*);
	void (*erase)(BTree*, const void*);
	const void* (*at)(BTree*, const void*);
	void (*change)(BTree*, const void*, const void*);
}BTreeOp;

extern const BTreeOp* GetBTreeOpStruct();
#define BTree() (*(GetBTreeOpStruct()))