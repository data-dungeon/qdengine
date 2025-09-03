//*****************************************************************************
//
// Copyright (C) 2000-2005 Quantic Dream SA
//
// These coded instructions, statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are 
// protected by French and EEC copyright laws. They may not be 
// disclosed to third parties or copied or duplicated, in whole or in part, 
// without prior written consent of Quantic Dream SA
//
// Unpublished-rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
//	CLASS:	MEM
//	The MEM class implements ...
//
//	06-02-07:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_H__
#define __MEM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#if defined PLATFORM_PC || defined PLATFORM_X360
#include <malloc.h>
#endif

#ifdef PLATFORM_PS3
#include <stdlib.h>
#endif

//#include <malloc.h>
DECL_KCORE(QDT_STRING)
DECL_KCORE(IO_STREAM);
class MUTEX;
class TIME_LOG;

struct MEM_ALLOCATION_REQUEST
{
	UInt32 _uSize;
	UInt32 _uBoundary;
#ifdef QDT_MEM_DEBUG
	UInt32 _uLibId;
	UInt32 _uDepth;
#endif
};

#ifdef PLATFORM_PS3
	#define	QDT_DELETE_PLACED(a, b)		(a)->b::~b()
#endif

/*#ifdef QDT_MEM_THREAD_SAFE
#endif*/

class MEM_POOL_MANAGER;
class MEM_BIG_MANAGER;

#ifdef QDT_MEM_DEBUG
struct MEM_RUNTIME_DEBUG_CONFIG
{
	Bool _bUseCorruptionProtection;
	Bool _bUseCallstack;
	Bool _bUseTimeId;
	Bool _bUseArrayProtection;

	Bool _bAllowCheckConsistency;
};
#endif

class	MEM
{
friend	class MEM_POOL_MANAGER;
public:
	MEM();
	~MEM();

	void *	Allocate(	UInt32	uSize);
	void *	Allocate(	MEM_ALLOCATION_REQUEST* pRequest);

	/*
	AllocateAtEnd (deprecated pour le momment ?)
	*/

	void *	AllocateAlign(	UInt32	uBoundary,	UInt32	uSize);
	void *	AllocateAlign(	MEM_ALLOCATION_REQUEST* pRequest);

	void	Free(const void *	pPtr);

	void *	Realloc(	void*	pPtr,
						UInt32	uSize);

	void	Memcpy(	void* pDest,
					const void* pSource,
					UInt32 uSize);

	void	Shrink(	void* pPtr,
					UInt32 uSize);

	void	DeleteMpCustom(MEM_POOL_MANAGER* pMpCustom);
	MEM_POOL_MANAGER* getMemPoolCustom(	UInt32 uNumEntryPoints,
										UInt32 uEntryStep,
										UInt32 uNumPool,
										UInt32 uMPIdx,
										Bool bIsPrimary = false );

	static void ExitCleanup();

	Bool	isMemStatsAvailable(void)
	{
#if defined(QDT_MEM_ENABLED) && defined(QDT_MEM_DEBUG) && defined(QDT_MEM_USE_TIME_ID)
		return true;
#else
		return false;
#endif
	}

#ifdef QDT_NEW_REDEFINED
#undef new
#endif

	_INLINE_ static void	CreateInstance();
	_INLINE_ static void	SetInstance(MEM* pNewInstance);
	_INLINE_ static MEM*	GetInstance();
	_INLINE_ static bool	IsInstancied();

#ifdef QDT_MEM_DEBUG
	static void SetNextReturnPtr(void* pPtr);

	UInt32	GetCurrentUsageForLib(	UInt32 uLibId);
	UInt32	GetPeakUsageForLib(	UInt32 uLibId);
	UInt32	GetTotalUsage();

	void	CheckConsistency();

	UInt32	GetNumberOfUsedMemPool();
	UInt32	GetNumberOfUsedMemPoolChunk();
	UInt32	GetBigChunkAllocatedSize();

	UInt32	GetNumAllocation();
	MEM_POOL_MANAGER*	GetMemPoolManager(UInt32 poolIdx);

	void	DumpUsageMap( const QDT_STRING & sFileName );
	void	DumpUsageMap( IO_STREAM* pStream );

	UInt32	GetLastMallocWastedMemory();
	void	ResetLastMallocWastedMemory();

	TIME_LOG* GetTimeLogForLib(UInt32 iLibIdx);
	static void DebugNextPtrMutexLock(void);

	void	AddFreeTrace(const void* pPtr);
	void	RemoveFreeTrace(const void* pPtr);
	void	CheckFreeTrace(const void* pPtr);
#endif

	void	Update();

	void* operator new(size_t s)
	{
		return malloc(sizeof(MEM));
	}
	void operator delete(void* p)
	{
		return free(p);
	}

#ifdef QDT_NEW_REDEFINED
#define new QDT_NEW_REDEFINED
#endif

#ifdef QDT_MEM_DEBUG
	bool						_bIsCreatingTime;
	static char*				_pLibIdNames[_iNumMaxLibs];
	TIME_LOG*					_pMemTimeLogTable;

	void*						pNextPtr;
	MUTEX*						_DebugNextPtrMutex;
	MEM_RUNTIME_DEBUG_CONFIG	_debugConfig;
#endif

private:

	_INLINE_	void Lock();
	_INLINE_	void Unlock();

	static MEM* _pInstance;

#ifdef QDT_MEM_THREAD_SAFE
	friend class MUTEX;
	MUTEX*				_Mutex;
	MUTEX*				_InnerMutex;
#endif

	MEM_CONFIG*			_pMemConfig;

	MEM_POOL_MANAGER*	_pMemPoolManagerCustom;
	MEM_POOL_MANAGER*	_pMemPoolManager;
	MEM_BIG_MANAGER*	_pMemBigManager;

#ifdef QDT_MEM_DEBUG
	UInt32				_freeTraceTable[_iMaxFreeTrace];
#endif
};

#ifdef QDT_MEM_DEBUG
#define QDT_MEM_CHECKCONSISTENCY() MEM::GetInstance()->CheckConsistency()
#else
#define QDT_MEM_CHECKCONSISTENCY()
#endif

#ifndef _DEBUG
#include "Mem.inl"
#endif

#endif // __MEM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
