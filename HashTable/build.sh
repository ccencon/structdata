#!/bin/sh

gcc -std=c11 -W -gdwarf-2 -g3 -g -DDEBUG -rdynamic ./HashTable.c ./test.c ../RBTree/RBTree.c ../SqList/SqList.c ../DuCirLinkList/DuCirLinkList.c ../SqStack/SqStack.c ../DlQueue/DlQueue.c ../common/default_func.c
