#ifndef _SQ_LIST__
#define _SQ_LIST__
#include "../common/common.h"
#include <stdbool.h>

#define INIT_SIZE 150
#define INCREASE_RATE 1.75

typedef struct SqList{
	void* pElems;
	void* tmpRet;
	size_t e_S;
	size_t length;
	size_t size;
}SqList;

typedef struct SqListOp{
	SqList* (*create)(size_t, const size_t*);
	void (*destroy)(SqList**);
	void (*clear)(SqList*);
	void (*insert)(SqList*, size_t, const void*);
	void (*push_back)(SqList*, const void*);
	void (*change)(SqList*, size_t, const void*);
	void (*change_unsafe)(SqList*, size_t, const void*);
	const void* (*erase)(SqList*, size_t);
	const void* (*at)(SqList*, size_t);
	const void* (*at_unsafe)(SqList*, size_t);
	size_t (*length)(SqList*);
	void (*for_each)(SqList*, SequenceForEachFunc_Mutable, void*);
	void (*r_for_each)(SqList*, SequenceForEachFunc_Mutable, void*);/*forward*/
	void (*reverse)(SqList*);
	void (*sort)(SqList*, CmnCompareFunc);
	void (*mem_init)(SqList*);
	void (*swap)(SqList*, size_t, size_t);
}SqListOp;

#define SQLIST_FOREACH(pList, type, logic) {\
	type* _PVALUE__ = (type*)(pList)->pElems;\
	for (size_t key = 0; key < (pList)->length; key++){\
		type value = TOCONSTANT(type, _PVALUE__ + key);\
		logic;\
	}\
}

#define SQLIST_FOREACH_REVERSE(pList, type, logic) {\
	type* _PVALUE__ = (type*)(pList)->pElems;\
	for (size_t tmpKey = (pList)->length; tmpKey >= 1; tmpKey--){\
		size_t key = tmpKey - 1;\
		type value = TOCONSTANT(type, _PVALUE__ + key);\
		logic;\
	}\
}

#define SQLIST_FOREACH_UNSAFE(pList, type, logic) {\
	type* _PVALUE__ = (type*)(pList)->pElems;\
	for (size_t key = 0; key < (pList)->size; key++){\
		type value = TOCONSTANT(type, _PVALUE__ + key);\
		logic;\
	}\
}

#define SQLIST_FOREACH_REVERSE_UNSAFE(pList, type, logic) {\
	type* _PVALUE__ = (type*)(pList)->pElems;\
	for (size_t tmpKey = (pList)->size; tmpKey >= 1; tmpKey--){\
		size_t key = tmpKey - 1;\
		type value = TOCONSTANT(type, _PVALUE__ + key);\
		logic;\
	}\
}

extern const SqListOp* GetSqListOpStruct();
#define SqList() (*(GetSqListOpStruct()))
#endif