#include "DuCirLinkList.h"
#include <string.h>

static DuCirLinkList* create(size_t ESize)
{
	POINTCREATE(DuCirLinkList*, ret, DuCirLinkList, sizeof(DuCirLinkList));
	POINTCREATE(EMPTYDEF, ret->tmpRet, void, ESize);
	POINTCREATE_INIT(EMPTYDEF, HEAD(ret), DuCirLink, sizeof(DuCirLink));
	BEGIN(ret) = HEAD(ret);
	LAST(ret) = HEAD(ret);
	ret->e_S = ESize;
	ret->length = 0;
	return ret;
}

static void clear(DuCirLinkList* pList)
{
	for(DuCirLink* link = BEGIN(pList); link != END(pList);){
		DuCirLink* tLink = link;
		link = link->next;
		FREE(tLink->pElem);
		FREE(tLink);
	}
	BEGIN(pList) = HEAD(pList);
	LAST(pList) = HEAD(pList);
	pList->length = 0;
}

static inline void destroy(DuCirLinkList** ppList)
{
	clear(*ppList);
	FREE(HEAD(*ppList));
	FREE((*ppList)->tmpRet);
	FREE(*ppList);
}

/*insert a new node in front of pivotNode*/
static inline void INSETRT_NODE(DuCirLink* iNode, DuCirLink* pivotNode)
{
	DuCirLink* priorNode = pivotNode->prior;
	priorNode->next = iNode;
	iNode->prior = priorNode;
	iNode->next = pivotNode;
	pivotNode->prior = iNode;
}

static inline DuCirLink* FIND_LOCNODE(DuCirLinkList* pList, size_t loc)
{
	DuCirLink* locNode = HEAD(pList);
	if (loc < pList->length / 2)
		for (size_t i = 0; i <= loc; i++)
			locNode = locNode->next;
	else
		for (size_t i = pList->length; i > loc; i--)
			locNode = locNode->prior;
	return locNode;
}

static void insert(DuCirLinkList* pList, size_t loc, const void* elem)
{
	if (loc > pList->length)
		return;
	POINTCREATE(DuCirLink*, node, DuCirLink, sizeof(DuCirLink));
	POINTCREATE(EMPTYDEF, node->pElem, void, pList->e_S);
	memcpy(node->pElem, elem, pList->e_S);
	DuCirLink* locNode = FIND_LOCNODE(pList, loc);
	INSETRT_NODE(node, locNode);
	pList->length++;
}

static inline void push_back(DuCirLinkList* pList, const void* elem)
{
	insert(pList, pList->length, elem);
}

static inline void change(DuCirLinkList* pList, size_t loc, const void* elem)
{
	if (loc >= pList->length)
		return;
	DuCirLink* locNode = FIND_LOCNODE(pList, loc);
	memcpy(locNode->pElem, elem, pList->e_S);
}

static const void* erase(DuCirLinkList* pList, size_t loc)
{
	if (loc >= pList->length)
		return NULL;
	DuCirLink* locNode = FIND_LOCNODE(pList, loc);
	DuCirLink* nextNode = locNode->next;
	DuCirLink* priorNode = locNode->prior;
	nextNode->prior = priorNode;
	priorNode->next = nextNode;
	memcpy(pList->tmpRet, locNode->pElem, pList->e_S);
	pList->length--;
	FREE(locNode->pElem);
	FREE(locNode);
	return pList->tmpRet;
}

static inline const void* at(DuCirLinkList* pList, size_t loc)
{
	if (loc >= pList->length)
		return NULL;
	DuCirLink* locNode = FIND_LOCNODE(pList, loc);
	return locNode->pElem;
}

static inline size_t length(DuCirLinkList* pList)
{
	return pList->length;
}

static inline void for_each(DuCirLinkList* pList, SequenceForEachFunc_Mutable func, void* args)
{
	if (pList->length == 0)
		return;
	DuCirLink* node = BEGIN(pList);
	for (size_t i = 0; node != END(pList); node = node->next, i++)
		func(i, node->pElem, args);
}

static inline void r_for_each(DuCirLinkList* pList, SequenceForEachFunc_Mutable func, void* args)
{
	if (pList->length == 0)
		return;
	DuCirLink* node = LAST(pList);
	for (size_t i = pList->length - 1; node != END(pList); node = node->prior, i--)
		func(i, node->pElem, args);
}

static void reverse(DuCirLinkList* pList)
{
	DuCirLink* node = HEAD(pList);
	while(true){
		DuCirLink* tmpNode;
		tmpNode = node->next;
		node->next = node->prior;
		node->prior = tmpNode;
		if (tmpNode == HEAD(pList))
			break;
		node = tmpNode;
	}
}

/*so many copy*/
static inline void CHANGE_LOCATION(DuCirLink* CNode, DuCirLink* pivotNode)
{
	CNode->prior->next = CNode->next;
	CNode->next->prior = CNode->prior;
	CNode->prior = pivotNode->prior;
	CNode->prior->next = CNode;
	CNode->next = pivotNode;
	pivotNode->prior = CNode;
}

static void quick_sort(CmnCompareFunc func, DuCirLink* lhs, DuCirLink* rhs)
{
	DuCirLink* recL = NULL;
	DuCirLink* recR = NULL;
	for(DuCirLink* pNode = rhs; pNode != lhs;){
		DuCirLink* tmp = pNode->prior;
		if(func(pNode->pElem, lhs->pElem)){
			CHANGE_LOCATION(pNode, lhs);
			if(!recL)
				recL = pNode;
		}
		else{
			if(!recR)
				recR = pNode;
		}
		pNode = tmp;
	}

	if (recL && recL != lhs->prior)
		quick_sort(func, recL, lhs->prior);
	if (recR && recR != lhs->next)
		quick_sort(func, lhs->next, recR);
}

static void swap(DuCirLinkList* pList, size_t left, size_t right)
{
	if (left == right || left >= pList->length || right >= pList->length)
		return;
	DuCirLink* ln = FIND_LOCNODE(pList, left);
	DuCirLink* rn = FIND_LOCNODE(pList, right);
	for (size_t i = 0; i < pList->e_S; i++){
		unsigned char* _lhs = ln->pElem + i;
		unsigned char* _rhs = rn->pElem + i;
		unsigned char tmp = *_lhs;
		*_lhs = *_rhs;
		*_rhs = tmp;
	}
}

static void sort(DuCirLinkList* pList, CmnCompareFunc func)
{
	if (pList->length <= 1)
		return;
	quick_sort(func, BEGIN(pList), LAST(pList));
}

inline const DucListOp* GetDucListOpStruct()
{
	static const DucListOp OpList = {
		.create = create,
		.destroy = destroy,
		.clear = clear,
		.insert = insert,
		.push_back = push_back,
		.change = change,
		.erase = erase,
		.at = at,
		.length = length,
		.for_each = for_each,
		.r_for_each = r_for_each,
		.reverse = reverse,
		.sort = sort,
		.swap = swap,
	};
	return &OpList;
}