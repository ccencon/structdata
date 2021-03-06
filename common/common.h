#ifndef _HEAD_H_
#define _HEAD_H_
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/*
*所有实现都不是线程安全
*折腾了一下午还是不实现深拷贝了,在c的封装中实现深拷贝需要函数和返回值的各种协调,赋值也麻烦,还不如直接在应用层手动操作来的方便
*/

enum StatusCode{
	STATUS_INVALIDINDEX = 0,
	STATUS_OVERFLOW,
	STATUS_NOELEM,
	STATUS_SIZEERROR,
	STATUS_NULLFUNC,
	STATUS_DEERROR,
	STATUS_OFFSETERROR,
	STATUS_FILEUNMATCHED,
	STATUS_MMAPFAILED,
	STATUS_INVALIDBUCKETTYPE,

	STATUS_FDERROR,
	STATUS_RDERROR,
	STATUS_WRERROR,
	STATUS_FALLOCATEERROR,
	STATUS_UNLINKFAILED,
	STATUS_RENAMEFAILED,
};

static const char* errStr[] = {
	"invalid index", "overflow", "no elem", "elem size error", "null function", "degree error", "offset error",
	"file unmatched", "mmap failed", "invalid bucket type", "file descriptor error", "read error", "write error", "fallocate error",
	"unlink failed", "rename failed"
};

#define FREE(p) do{\
	free(p);\
	(p) = NULL;\
}while(0)

/*-DDEBUG -g -rdynamic*/
#ifdef DEBUG
#define CONDCHECK(con, code, file, line) do{\
	if (!(con)){\
		printf("%s:%d\n", file, line);\
		if ((code) < STATUS_FDERROR)\
			fprintf(stderr, "%s\n", errStr[code]);\
		else{\
			perror(errStr[code]);\
		}\
		void *array[20];\
		size_t traceSize = backtrace(array, 20);\
		char **strings = backtrace_symbols(array, traceSize);\
		if (strings){\
			for (size_t i = 0; i < traceSize; i++)\
				printf("%s\n", strings[i]);\
		}\
		else{\
			fprintf(stderr, "%s\n", "backtrace_symbols error");\
		}\
		FREE(strings);\
		exit(-1);\
	}\
}while (0)
#else
#define CONDCHECK(con, code, file, line) do{\
	if (!(con)){\
		printf("%s:%d\n", file, line);\
		fprintf(stderr, "%s\n", errStr[code]);\
	}\
}while (0)
#endif

#define ROOTCHECK(root) do{\
	if (!(root))\
		return;\
}while(0)

#define RELEASENODE(node)\
	FREE((node)->pKey);\
	FREE((node)->pValue);\
	FREE(node);

#define EMPTYDEF
#define POINTCREATE(def, point, type, size)\
	def point = (type*)malloc(size);\
	CONDCHECK(point, STATUS_OVERFLOW, __FILE__, __LINE__);

#define POINTCREATE_INIT(def, point, type, size)\
	POINTCREATE(def, point, type, size);\
	memset(point, 0, size);

#define TOCONSTANT(type, point) (*((type*)(point)))

#define EPS_FLOAT32 1e-6
#define EPS_FLOAT64 1e-15

//默认比较函数
extern bool default_less_func_int8(const void*, const void*);
extern bool default_less_func_uint8(const void*, const void*);
extern bool default_less_func_int16(const void*, const void*);
extern bool default_less_func_uint16(const void*, const void*);
extern bool default_less_func_int32(const void*, const void*);
extern bool default_less_func_uint32(const void*, const void*);
extern bool default_less_func_int64(const void*, const void*);
extern bool default_less_func_uint64(const void*, const void*);
extern bool default_less_func_float32(const void*, const void*);
extern bool default_less_func_float64(const void*, const void*);
extern bool default_less_func_string(const void*, const void*);
extern bool default_equal_func_int8(const void*, const void*);
extern bool default_equal_func_uint8(const void*, const void*);
extern bool default_equal_func_int16(const void*, const void*);
extern bool default_equal_func_uint16(const void*, const void*);
extern bool default_equal_func_int32(const void*, const void*);
extern bool default_equal_func_uint32(const void*, const void*);
extern bool default_equal_func_int64(const void*, const void*);
extern bool default_equal_func_uint64(const void*, const void*);
extern bool default_equal_func_float32(const void*, const void*);
extern bool default_equal_func_float64(const void*, const void*);
extern bool default_equal_func_string(const void*, const void*);
//默认哈希函数
extern size_t HASH_SEQ(const unsigned char*, size_t);
extern size_t default_hash_func_int8(const void*);
extern size_t default_hash_func_uint8(const void*);
extern size_t default_hash_func_int16(const void*);
extern size_t default_hash_func_uint16(const void*);
extern size_t default_hash_func_int32(const void*);
extern size_t default_hash_func_uint32(const void*);
extern size_t default_hash_func_int64(const void*);
extern size_t default_hash_func_uint64(const void*);
extern size_t default_hash_func_float32(const void*);
extern size_t default_hash_func_float64(const void*);
extern size_t default_hash_func_string(const void*);

typedef size_t(*HashFuncTT)(const void*);
typedef void(*SequenceForEachFunc_Mutable)(size_t, void*, void*);
typedef void(*SequenceForEachFunc_Const)(size_t, const void*, void*);
typedef void(*SequenceForEachFunc_Set)(size_t, const void*, void*);
typedef void(*UnorderedForEachFunc_Mutable)(const void*, void*, void*);
typedef void(*UnorderedForEachFunc_Const)(const void*, const void*, void*);
typedef bool(*CmnCompareFunc)(const void*, const void*);

#endif