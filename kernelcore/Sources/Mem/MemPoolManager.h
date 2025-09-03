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
//	CLASS:	MEM_POOL_HANDLER
//	The MEM_POOL_HANDLER class implements ...
//
//	06-02-06:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_POOL_MANAGER_H__
#define __MEM_POOL_MANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//#include <malloc.h>
#include <stdlib.h>

class MEM_POOL_MANAGER;

#include	INCL_KCORE(Mem/Mem)
#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Mem/MemPool)
#include	INCL_KCORE(Mem/MemLibInfo)

#ifdef QDT_MEM_DEBUG
#include	INCL_KCORE(Tools/QdtString)
#endif

#include	INCL_KCORE(Thread/MUTEX)

#define ADD_POOL(a) MP_CUSTOMID_##a,

enum MEM_POOL_CUSTOM_ID
{
	MP_CUSTOMID_NOTCUSTOM = 0, // special case, used by the memory manager

	#include	INCL_KCORE(Mem/MemPoolCustomID)

	MP_CUSTOMID_MAX
};

#undef ADD_POOL

class MUTEX;

class	MEM_POOL_MANAGER
{
	friend class MEM;

private: // private, so it will only get build from MEM

	MEM_POOL_MANAGER(	UInt32 uNumEntryPoints,
						UInt32 uEntryStep,
						UInt32 uNumPool,						
						UInt32 uMPIdx,
						Bool bIsPrimary = false);
	~MEM_POOL_MANAGER();
	void	Init();

public:

	void*	Allocate(MEM_ALLOCATION_REQUEST* pRequest);

	_INLINE_	void *	Allocate(UInt32	uSize);
	_INLINE_	void *	AllocateCustom(UInt32	uSize);
#ifdef QDT_MEM_DEBUG
				void *	AllocateCustom(UInt32	uSize, UInt32 uLibId);
#endif
				void	Free(const void *	pPtr);
	_INLINE_	void	FreeCustom(const void* pPtr);

	#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		UInt32	GetDataSize(	void* pPtr, UInt32* pLibId);
	#else
		UInt32	GetDataSize(	void* pPtr);
	#endif

#ifdef QDT_MEM_DEBUG
	void	CheckConsistency();
	UInt32	GetNumberOfUsedMemPool();
	UInt32	GetNumberOfUsedMemPoolChunk();

	UInt32	GetCurrentUsageForLib(	UInt32 uLibId);
	UInt32	GetPeakUsageForLib(	UInt32 uLibId);
	UInt32	GetTotalUsage();

	UInt32	GetNumAllocation();
#endif

#ifdef QDT_NEW_REDEFINED
#undef new
#endif

	_INLINE_	void* operator new(size_t s);
	_INLINE_	void operator delete(void* p);

	_INLINE_	void Lock();
	_INLINE_	void Unlock();

#ifdef QDT_NEW_REDEFINED
#define new QDT_NEW_REDEFINED
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	void	SetupExtraDebugInfo(void* pPtr, UInt32 uId, UInt32 uDepth);
#endif

#ifdef QDT_MEM_DEBUG
	void	DumpUsageMap( IO_STREAM* pStream );
	char	_poolName[0x20];
#endif

private:
	UInt32 _uNumEntryPoints;
	UInt32 _uEntryStep;
	UInt32 _uNumPool;
	UInt32 _uMPIdx;
	MEM_POOLNEW** _pMemPoolEntryTable;

	MEM_POOLNEW* _pMemPoolArray;
	MEM_POOLNEW* _pEndOfMemPoolArray;

	MEM_POOLNEW* _pFirstFreePool;

	MEM_POOL_MANAGER* _pNextManager;

	MUTEX* _Mutex;

	Bool _bIsPrimary;

#ifdef QDT_MEM_DEBUG
	UInt32			_uNumAllocation;
	MEM_LIB_INFO	_aLibInfo[_iNumMaxLibs];
	static char*	_pPoolNames[MP_CUSTOMID_MAX];
#endif
};

#ifndef _DEBUG
#include "MemPoolManager.inl"
#endif

#endif //__MEM_POOL_MANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
