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
//	CLASS:	MEM_POOLNEW
//	The MEM_POOLNEW class implements ...
//
//	06-02-06:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Mem/MemConfig)

class	MEM_POOLNEW;

#include	INCL_KCORE(Mem/MemPoolManager)

#undef new

class	MEM_POOLNEW
{
public:
	MEM_POOLNEW(){};
	~MEM_POOLNEW(){};

	void	InitializePool(	const UInt32	uElementSize);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	void	SetupCorruptionMarker(const UInt32	uElementSize);
	void	CheckCorruptionMarker(const UInt32	uElementSize);
#endif

	friend class MEM_POOL_MANAGER;

#ifdef QDT_NEW_REDEFINED
#undef new
#endif

	void * operator new[]( size_t cb )
	{
		return malloc(cb);
	}
	void operator delete[](void* p)
	{
		return free(p);
	}

#ifdef QDT_NEW_REDEFINED
#define new QDT_NEW_REDEFINED
#endif

#ifdef QDT_MEM_DEBUG
	Bool	IsInFreeList( char* pPtr );
	void	DumpUsageMap( IO_STREAM* pStream );
#endif

private:

	Char*	_pFirstFreeChunk;
	MEM_POOLNEW*	_pNextPool;
	UInt16	_NumFreeElementInPool;	// should we use this in release ? For now it's used for the
									// free treshold (to avoid summing-up the number of free
									// chunks at free time), and to speed-up the test to know
									// if the current pool is full
	UInt16	_NumElementInPool;	// not absolutly needed as it can be computed,
								// but because of alignement, we might as well
								// make use of it.
	UInt32	_uElementSize;
	QDT_ALIGN(16) Char	_pPool[_iTotalPoolSize];
};

#endif //__MEM_POOL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
