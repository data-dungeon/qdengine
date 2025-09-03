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
//	CLASS:	MEM_BIG_CHUNK
//	The MEM_BIG_CHUNK class implements ...
//
//	06-02-08:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_BIG_CHUNK_H__
#define __MEM_BIG_CHUNK_H__

#include	INCL_KCORE(Mem/MemBigChunkElement)
#include	<stdlib.h>

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

class MEM_BIG_CHUNK;
class MEM_BIG_MANAGER;

class	MEM_BIG_CHUNK
{
public:

	static MEM_BIG_CHUNK* CreateNewBigChunk(UInt32 uSize = QDT_BIGMEM_DEFAULT_SIZE);

	void Init(UInt32 uSize);
/*	MEM_BIG_CHUNK();
	~MEM_BIG_CHUNK(){}; */

	void *	Allocate(	MEM_ALLOCATION_REQUEST* pRequest);
	void *	AllocateAtEnd(	MEM_ALLOCATION_REQUEST* pRequest);
	void *	AllocateAlign(	MEM_ALLOCATION_REQUEST* pRequest);

	void	Free(	const void* pPtr);

	void	Shrink(	void* pPtr,
					UInt32 uSize);

	friend class MEM_BIG_MANAGER;

#ifdef QDT_NEW_REDEFINED
#undef new
#endif

	void* operator new(size_t s)
	{
		return malloc(sizeof(MEM_BIG_CHUNK));
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
#endif

private:
	void	SplitElementNoNewFree(	MEM_BIG_CHUNK_ELEMENT *	pElement,
									MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement);
	void	SplitElement(	MEM_BIG_CHUNK_ELEMENT *	pElement,
							UInt32 uSize,
							MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement);
	MEM_BIG_CHUNK_ELEMENT*	SplitElementAtEnd(	MEM_BIG_CHUNK_ELEMENT *	pElement,
												UInt32 uSize,
												MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement);
	void	FreeAndMergeBothSides(	MEM_BIG_CHUNK_ELEMENT* pPreviousElement,
									MEM_BIG_CHUNK_ELEMENT* pElement,
									MEM_BIG_CHUNK_ELEMENT* pNextElement);
	void	FreeAndMergeWithPrevious(	MEM_BIG_CHUNK_ELEMENT* pPreviousBlock,
										MEM_BIG_CHUNK_ELEMENT* pElement);
	void	FreeAndMergeWithNext(	MEM_BIG_CHUNK_ELEMENT* pElement,
									MEM_BIG_CHUNK_ELEMENT* pNextElement);
	void	FreeNoMerge(	MEM_BIG_CHUNK_ELEMENT *	pElement);

	_INLINE_ char* GetRawChunkPtr();
	_INLINE_ char* GetRawEndChunkPtr();

#ifdef QDT_MEM_DEBUG
	UInt32	GetAllocatedSize();
	void	CheckConsistency();
	void	MarkBlockAsFree(MEM_BIG_CHUNK_ELEMENT* pElement);
	void	MarkBlockAsAllocated(MEM_BIG_CHUNK_ELEMENT* pElement);
	void	SetupBlockCorruptionMarker(MEM_BIG_CHUNK_ELEMENT* pElement);
	void	CheckBlockCorruptionMarker(MEM_BIG_CHUNK_ELEMENT* pElement);
	void	SetupCallStack(MEM_BIG_CHUNK_ELEMENT* pElement, UInt32 uDepth);
	void	SetupTimeId(MEM_BIG_CHUNK_ELEMENT* pElement, UInt32 uId);
	void	SetupExtraDebugInfo(MEM_BIG_CHUNK_ELEMENT* pElement, UInt32 uId, UInt32 uDepth);
#endif

	// 0

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	MEM_BIG_CHUNK* _pNextChunk;
#else
	UInt32 _dummy0;
#endif

	// 4

#ifdef QDT_MEM_DEBUG
	UInt32 _uWastedMemory;
#else
	UInt32 _dummy1;
#endif

	// 8

	UInt32	_uFullSize;
	UInt32	_uAllocatedSize; // C
	MEM_BIG_CHUNK_ELEMENT* _pFirstFreeElement; //10
	MEM_BIG_CHUNK_ELEMENT* _pLastFreeElement; // 14

	UInt32	dummy[2]; // for Allignement

/*	
#ifdef QDT_MEM_USE_BIGMEM_CORRUPTION_PROTECTION
	char _corruptionMarker[16];
#endif*/
};

#ifndef QDT_DEBUG
#include "MemBigChunk.inl"
#endif

#endif // __MEM_BIG_CHUNK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
