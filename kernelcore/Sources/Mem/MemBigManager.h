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
//	CLASS:	MEM_BIG_MANAGER
//	The MEM_BIG_MANAGER class implements ...
//
//	06-02-08:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_BIG_MANAGER_H__
#define __MEM_BIG_MANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

//#include	<malloc.h>

class	MEM_BIG_MANAGER;

#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Mem/MemBigChunk)
#include	INCL_KCORE(Mem/MemLibInfo)

#include	"Root.h"

class	MEM_BIG_MANAGER
{
public:
	MEM_BIG_MANAGER();
	~MEM_BIG_MANAGER();

	void *	Allocate(	MEM_ALLOCATION_REQUEST* pRequest);
	void *	AllocateAtEnd(	MEM_ALLOCATION_REQUEST* pRequest);
	void *	AllocateAlign(	MEM_ALLOCATION_REQUEST* pRequest);

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	void	IncrementLibUsage(	UInt32	uSize,
								UInt32	uID);

	UInt32	GetDataSize(	void* pPtr,
							UInt32* pLibId);

#else
	UInt32	GetDataSize(	void* pPtr);
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	UInt32	GetCurrentUsageForLib(	UInt32 uLibId);
	UInt32	GetPeakUsageForLib(	UInt32 uLibId);
	UInt32	GetTotalUsage();
#endif

#ifdef QDT_MEM_DEBUG
	UInt32	GetNumAllocation();
#endif

	Bool	Free(	const void* pPtr);

	void	Shrink(	void* pPtr,
					UInt32 uSize);

#ifdef QDT_MEM_DEBUG
	void	CheckConsistency();
	UInt32	GetBigChunkAllocatedSize();
#endif

#ifdef QDT_NEW_REDEFINED
#undef new
#endif

	void* operator new(size_t s)
	{
		return malloc(sizeof(MEM_BIG_MANAGER));
	}
	void operator delete(void* p)
	{
		return free(p);
	}

#ifdef QDT_NEW_REDEFINED
#define new QDT_NEW_REDEFINED
#endif

#ifdef QDT_MEM_DEBUG
	void	DumpUsageMap( IO_STREAM* pStream );
	UInt32	GetLastMallocWastedMemory();
	void	ResetLastMallocWastedMemory();
#endif

private:
#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	MEM_BIG_CHUNK* _pFirstChunk;
#else
	MEM_BIG_CHUNK _pFirstChunk[1]; // static, but act as a pointer (no code change)
#endif

#ifdef QDT_MEM_DEBUG
	UInt32			_uNumAllocation;
	MEM_LIB_INFO	_aLibInfo[_iNumMaxLibs];
#endif
};

#endif //__MEM_BIG_MANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
