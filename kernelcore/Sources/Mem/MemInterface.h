#ifndef _MEM_INTERFACE_H_
#define _MEM_INTERFACE_H_

#include	INCL_KCORE(Mem/MemLibIdInterface)
#include	INCL_KCORE(Mem/MemConfigInterface)
#include	INCL_KCORE(Mem/Mem)

#ifndef QDT_MEM_INTERNAL
#ifdef QDT_MEM_ENABLED

extern int nLibId;
/*
#include	<malloc.h>

#undef calloc
#undef free
#undef malloc
#undef realloc
#undef _aligned_free
#undef _aligned_malloc
#undef _aligned_offset_malloc
#undef _aligned_realloc
#undef _aligned_offset_realloc
#undef _resetstkoflw
*/
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
void* operator new(size_t s, UInt32 ID, UInt32 uDepth);
void* operator new(size_t s, UInt32 allign, UInt32 ID, UInt32 uDepth);
void* operator new[](size_t cb, UInt32 ID, UInt32 uDepth);
void* operator new[](size_t cb, UInt32 allign, UInt32 ID, UInt32 uDepth);
void operator delete(void* pPtr, UInt32 ID, UInt32 uDepth);
void operator delete(void* pPtr, UInt32 allign, UInt32 ID, UInt32 uDepth);
void operator delete[](void* pPtr, UInt32 ID, UInt32 uDepth);
void operator delete[](void* pPtr, UInt32 allign, UInt32 ID, UInt32 uDepth);
#define QDT_malloc(uSize) MEM::GetInstance()->Allocate(uSize,nLibId)
#define QDT_malloc_end(uSize) MEM::GetInstance()->AllocateAtEnd(uSize,nLibId)
#define QDT_memalign(uBoundary,uSize) MEM::GetInstance()->AllocateAlign(uBoundary,uSize,nLibId)
#else // QDT_MEM_USE_TIME_ID
#define QDT_malloc(uSize) MEM::GetInstance()->Allocate(uSize)
#define QDT_malloc_end(uSize) MEM::GetInstance()->AllocateAtEnd(uSize)
#define QDT_memalign(uBoundary,uSize) MEM::getInstance()->AllocateAlign(uBoundary,uSize)
#endif // QDT_MEM_USE_TIME_ID
#define QDT_free(pPtr) MEM::GetInstance()->Free(pPtr)
#endif // QDT_MEM_ENABLED

#ifdef QDT_MEM_ENABLED
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
#define MEM_POOL_ALLOCFREE(CLASSNAME)								\
static MEM_POOL_MANAGER* MP;										\
void *	operator new(size_t s, UInt32 ID)							\
{ 																	\
	QDT_ASSERT(MP)													\
	return MP->Allocate(s, ID, 0);									\
} 																	\
void	operator delete(void* p) 									\
{ 																	\
	MP->Free(p); 													\
}
#else // QDT_MEM_USE_EXTRA_DEBUG_INFO
#define MEM_POOL_ALLOCFREE(CLASSNAME)								\
static MEM_POOL_MANAGER* MP;										\
void *	operator new(size_t s)										\
{ 																	\
	QDT_ASSERT(MP)													\
	return MP->Allocate(s);											\
} 																	\
void	operator delete(void* p) 									\
{ 																	\
	MP->Free(p); 													\
}
#endif // QDT_MEM_USE_EXTRA_DEBUG_INFO
#else // QDT_MEM_ENABLED
#define MEM_POOL_ALLOCFREE(CLASSNAME)
#endif // QDT_MEM_ENABLED

#ifdef QDT_MEM_ENABLED
#define MEM_POOL_DECLARE(CLASSNAME)	MEM_POOL_MANAGER*  CLASSNAME::MP = NULL;

#define MEM_POOL_INIT(CLASSNAME,NBELEMENT) \
QDT_ASSERT(CLASSNAME::MP == NULL);	\
CLASSNAME::MP = (MEM_POOL_MANAGER*)malloc(sizeof(MEM_POOL_MANAGER)); \
CLASSNAME::MP->MEM_POOL_MANAGER::MEM_POOL_MANAGER(1,sizeof(CLASSNAME),((NBELEMENT* sizeof(CLASSNAME))/_iTotalPoolSize)+1,MP_CUSTOMID_CLASSNAME)

#define MEM_POOL_DELETE(CLASSNAME) \
QDT_ASSERT(CLASSNAME::MP); \
MEM::_pInstance->DeleteMpCustom(CLASSNAME::MP); \
CLASSNAME::MP = NULL;
#else // QDT_MEM_ENABLED
#define MEM_POOL_DECLARE(CLASSNAME)
#define MEM_POOL_INIT(CLASSNAME,NBELEMENT)
#define MEM_POOL_DELETE(CLASSNAME)
#endif // QDT_MEM_ENABLED
/*
#ifdef QDT_MEM_ENABLED
#ifdef QDT_MEM_USE_TIME_ID
#ifdef QDT_LIB_ID
__forceinline void* operator new(size_t s) { return ::operator new(s, QDT_LIB_ID, 1); }
#else
__forceinline void* operator new(size_t s) { return ::operator new(s, 0, 1); }
#endif
#endif // QDT_MEM_USE_TIME_ID
#endif // QDT_MEM_ENABLED
*/
/*
#ifdef QDT_MEM_ENABLED
#ifdef QDT_MEM_USE_TIME_ID
#ifdef QDT_LIB_ID
#define new ::new(QDT_LIB_ID,1)
#endif
#endif // QDT_MEM_USE_TIME_ID
#endif // QDT_MEM_ENABLED
*/

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
#define QDT_NEW ::new(QDT_LIB_ID,1)
#else //QDT_MEM_USE_EXTRA_DEBUG_INFO
#define QDT_NEW new
#endif //QDT_MEM_USE_EXTRA_DEBUG_INFO

#endif // QDT_MEM_INTERNAL

#endif // _MEM_INTERFACE_H_


