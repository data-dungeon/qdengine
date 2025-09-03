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
//
//	06-02-06:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#define QDT_MEM_INTERNAL

#include	"Root.h"
#include	INCL_KCORE(Mem/Mem)
#include	INCL_KCORE(Mem/MemPoolManager)
#include	INCL_KCORE(Mem/MemBigManager)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#ifdef QDT_MEM_USE_MEMPOOL_CALLSTACK
#include INCL_KCORE(OS/OS)
#endif

#ifdef QDT_MEM_DEBUG
#include INCL_KCORE(TIME/TIME_ABC)
#endif

#ifdef QDT_MEM_DEBUG
#include <stdio.h>
#endif

#ifdef PLATFORM_PC
#include <malloc.h>
#endif

#undef new

#ifdef QDT_MEM_DEBUG

#define ADD_POOL(a) #a,

char* MEM_POOL_MANAGER::_pPoolNames[MP_CUSTOMID_MAX] = {
	"Default",
#include	INCL_KCORE(Mem/MemPoolCustomID)
};

#undef ADD_POOL

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		
//	06-02-06:	VHA - Created
//-----------------------------------------------------------------------------
MEM_POOL_MANAGER::MEM_POOL_MANAGER(UInt32 uNumEntryPoints, UInt32 uEntryStep, UInt32 uNumPool, UInt32 uMPIdx, Bool bIsPrimary)
{
	QDT_ASSERT(uNumEntryPoints > 0);
	QDT_ASSERT(uEntryStep > 0);
	QDT_ASSERT(uNumPool > 0);
	UInt32 i;

	// allign EntryStep on 16
	if(uEntryStep%_iBigChunkAllocationAllignement)
	{
		uEntryStep += (_iBigChunkAllocationAllignement-(uEntryStep%_iBigChunkAllocationAllignement));
	}

	_uNumEntryPoints = uNumEntryPoints;
	_uEntryStep = uEntryStep;
	_uNumPool = uNumPool;
	_uMPIdx = uMPIdx;
	_bIsPrimary = bIsPrimary;

	if(g_MemConfig._bIsActive == mem_enabled)
	{
		if(!bIsPrimary)
		{
			_Mutex = new MUTEX;

			MEM_ALLOCATION_REQUEST pRequest;

			pRequest._uSize = sizeof(MEM_POOLNEW*)*uNumEntryPoints;
			pRequest._uBoundary = 0;
#ifdef QDT_MEM_DEBUG
			pRequest._uLibId = MEM_LIBID_UNLISTED;
			pRequest._uDepth = 1;
#endif
			_pMemPoolEntryTable = (MEM_POOLNEW**)MEM::GetInstance()->Allocate(&pRequest);
		}
		else
		{
			_Mutex = NULL;
	#ifdef PLATFORM_PC
			_pMemPoolEntryTable = (MEM_POOLNEW**)_aligned_malloc(sizeof(MEM_POOLNEW*)*uNumEntryPoints,16);
	#else
			_pMemPoolEntryTable = (MEM_POOLNEW**)malloc(sizeof(MEM_POOLNEW*)*uNumEntryPoints);
	#endif
#ifdef QDT_MEM_DEBUG
			strcpy(_poolName,"default");
#endif
		}

		QDT_ASSERT((((int)_pMemPoolEntryTable)%_iBigChunkAllocationAllignement) == 0);

		for(i=0;i<uNumEntryPoints;i++)
		{
			_pMemPoolEntryTable[i] = NULL;
		}

		if(uNumEntryPoints == 1)
		{
			MEM_ALLOCATION_REQUEST pRequest;

			pRequest._uSize = sizeof(MEM_POOLNEW) * uNumPool;
			pRequest._uBoundary = 0;
#ifdef QDT_MEM_DEBUG
			pRequest._uLibId = MEM_LIBID_UNLISTED;
			pRequest._uDepth = 1;
#endif
			_pMemPoolArray = (MEM_POOLNEW*)MEM::GetInstance()->Allocate(&pRequest);
		}
		else
		{
			_pMemPoolArray = (MEM_POOLNEW*)malloc( sizeof(MEM_POOLNEW) * uNumPool);
		}

		QDT_ASSERT((((int)_pMemPoolArray)%_iBigChunkAllocationAllignement) == 0);

		// initialize the list of pool
		for(i=0;i<uNumPool;i++)
		{
			QDT_ASSERT((((int)&_pMemPoolArray[i])%_iBigChunkAllocationAllignement) == 0);
			_pMemPoolArray[i]._pNextPool = &_pMemPoolArray[i+1];
		}

		// last free pool must have his next set to NULL
		_pMemPoolArray[uNumPool-1]._pNextPool = NULL;

		_pFirstFreePool = _pMemPoolArray;

		_pEndOfMemPoolArray = _pMemPoolArray+uNumPool;

	#ifdef QDT_MEM_DEBUG
		{
			UInt32 i;

			for(i=0;i<_iNumMaxLibs;i++)
			{
				_aLibInfo[i]._uCurrentUsage = 0;
				_aLibInfo[i]._uPeakUsage = 0;
				_aLibInfo[i]._fTimeOfPeak = 0.f;
			}
		}

		_uNumAllocation = 0;
	#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL_HANDLER destructor
//	Object:		
//	06-02-06:	VHA - Created
//-----------------------------------------------------------------------------
MEM_POOL_MANAGER::~MEM_POOL_MANAGER()
{
	if(g_MemConfig._bIsActive == mem_enabled)
	{
		if(_Mutex)
		{
			delete _Mutex;
		}

		if(!_bIsPrimary)
		{
			MEM::GetInstance()->Free(_pMemPoolEntryTable);
		}
		else
		{
			free(_pMemPoolEntryTable);
		}

		if(_uNumEntryPoints == 1)
		{
			MEM::GetInstance()->Free(_pMemPoolArray);
		}
		else
		{
			delete[] _pMemPoolArray;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-07-03:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL_MANAGER::Allocate(MEM_ALLOCATION_REQUEST *	pRequest)
{
	UInt32 uSize = pRequest->_uSize;

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(uSize);
	}

	UInt32 uPoolIdx;

	uPoolIdx = uSize / _uEntryStep;

	if(uPoolIdx * _uEntryStep == uSize) // TODO special case, find better way to handle it
	{
		uPoolIdx -= 1;
	}

	if(uPoolIdx >= _uNumEntryPoints)
	{
#ifdef QDT_MEM_DEBUG
		pRequest->_uDepth++;
#endif
		return MEM::GetInstance()->_pMemBigManager->Allocate(pRequest);
	}

	// Now know the pool entry to use
	QDT_ASSERT(uPoolIdx < _uNumEntryPoints);
	MEM_POOLNEW** pCurrentPool = &_pMemPoolEntryTable[uPoolIdx];

	// check if the current pool is allocated or if it's full
	if(		((*pCurrentPool)== NULL) // never allocated
		||	((*pCurrentPool)->_NumFreeElementInPool == 0)) // is full
	{
		//QDT_ENSURE(_pFirstFreePool != NULL);
		// Allocate a new element
		if(_pFirstFreePool == NULL)
		{
#ifdef QDT_MEM_DEBUG
			QDT_Message("MemPool Overflow: %s", _poolName);
			pRequest->_uDepth++;
#endif
			return MEM::GetInstance()->_pMemBigManager->Allocate(pRequest);
		}

		QDT_ASSERT(_pFirstFreePool);

		MEM_POOLNEW* pTempMemPool = *pCurrentPool;
		*pCurrentPool = _pFirstFreePool;
		_pFirstFreePool = _pFirstFreePool->_pNextPool;
		(*pCurrentPool)->_pNextPool = pTempMemPool;

		// Initialize the new pool
		(*pCurrentPool)->InitializePool((uPoolIdx+1)*_uEntryStep);
	}

	// now we are sure that pCurrentPool reference a pool with a least one free entry
	MEM_POOLNEW* pCurrentPoolDirect = *pCurrentPool;
	QDT_ASSERT(pCurrentPoolDirect->_pFirstFreeChunk); // Assert for safety

	// assert the new link is valid.
	QDT_ASSERT(!pCurrentPoolDirect->_pFirstFreeChunk || (((MEM_POOLNEW*)pCurrentPoolDirect->_pFirstFreeChunk >=_pMemPoolArray) && ((MEM_POOLNEW*)pCurrentPoolDirect->_pFirstFreeChunk < _pEndOfMemPoolArray)));

	void* pAllocatedEntry = pCurrentPoolDirect->_pFirstFreeChunk;
	pCurrentPoolDirect->_pFirstFreeChunk = *(char**)pAllocatedEntry;
	QDT_ASSERT((((int)pCurrentPoolDirect->_pFirstFreeChunk)%_iBigChunkAllocationAllignement) == 0);
	QDT_ASSERT(!pCurrentPoolDirect->_pFirstFreeChunk || (((MEM_POOLNEW*)pCurrentPoolDirect->_pFirstFreeChunk >=_pMemPoolArray) && ((MEM_POOLNEW*)pCurrentPoolDirect->_pFirstFreeChunk < _pEndOfMemPoolArray)));

	QDT_ASSERT(pCurrentPoolDirect->_NumFreeElementInPool != 0);
	pCurrentPoolDirect->_NumFreeElementInPool--;

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	SetupExtraDebugInfo(pAllocatedEntry,pRequest->_uLibId,pRequest->_uDepth+1);
#endif

	QDT_ASSERT((((int)pAllocatedEntry) % _iBigChunkAllocationAllignement) == 0);

#ifdef QDT_MEM_DEBUG
	_uNumAllocation++;
#endif

	return pAllocatedEntry;
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::Free(const void *	pPtr)
{
#ifdef QDT_MEM_DEBUG
	MEM::GetInstance()->CheckFreeTrace(pPtr);
#endif

	QDT_ASSERT(pPtr);

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free((void*)pPtr);
		return;
	}

	if(!((pPtr >=_pMemPoolArray) && (pPtr < _pEndOfMemPoolArray)))
	{
		QDT_ASSERT(_bIsPrimary == false);
		MEM::GetInstance()->Free(pPtr);
		return;
	}

	MEM_POOLNEW* pPool = (MEM_POOLNEW*)((char*)pPtr - (((char*)(pPtr)-(char*)_pMemPoolArray)%sizeof(MEM_POOLNEW)));

	QDT_ASSERT((pPtr >= pPool) && (pPtr < pPool+1));

	QDT_ASSERT(!pPool->_pFirstFreeChunk || (((MEM_POOLNEW*)pPool->_pFirstFreeChunk >=_pMemPoolArray) && ((MEM_POOLNEW*)pPool->_pFirstFreeChunk < _pEndOfMemPoolArray)));
	*(Char**)pPtr = pPool->_pFirstFreeChunk;
	pPool->_pFirstFreeChunk = (Char*)pPtr;
	QDT_ASSERT((((int)pPool->_pFirstFreeChunk)%_iBigChunkAllocationAllignement) == 0);
	QDT_ASSERT(!pPool->_pFirstFreeChunk || (((MEM_POOLNEW*)pPool->_pFirstFreeChunk >=_pMemPoolArray) && ((MEM_POOLNEW*)pPool->_pFirstFreeChunk < _pEndOfMemPoolArray)));

	pPool->_NumFreeElementInPool ++;

	if(		pPool->_pNextPool
		&& (pPool->_pNextPool->_NumFreeElementInPool == pPool->_pNextPool->_NumElementInPool)) // if next pool is free
	{
		// and this pool is empty enough
		if(pPool->_NumFreeElementInPool <= (pPool->_NumElementInPool*_iPoolDestructionTreshold)/100)
		{
			// destruct the next pool
			MEM_POOLNEW* pTempPool = pPool->_pNextPool;
			pPool->_pNextPool = pTempPool->_pNextPool;

			pTempPool->_pNextPool = _pFirstFreePool;
			_pFirstFreePool = pTempPool;
		}
	}

#ifdef QDT_MEM_DEBUG
	// decrement the lib use counter
	{
		UInt32 uId;

		uId = *(UInt32*)(((char*)pPtr) + pPool->_uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4 + 8);

		QDT_ASSERT(uId < _iNumMaxLibs);
		//QDT_ASSERT(_aLibInfo[uId]._uCurrentUsage >= pPool->_uElementSize);
		_aLibInfo[uId]._uCurrentUsage -= pPool->_uElementSize;
	}

	_uNumAllocation--;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetDataSize
//	Object:		
//	06-03-07:	VHA - Created
//-----------------------------------------------------------------------------
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
UInt32	MEM_POOL_MANAGER::GetDataSize(	void *		pPtr,
										UInt32 *	pLibId)
#else
UInt32	MEM_POOL_MANAGER::GetDataSize(void *		pPtr)
#endif
{
	QDT_ASSERT(pPtr);

	//first, we check if the ptr is ptr is in the pool array
	QDT_ASSERT((pPtr >=_pMemPoolArray) && (pPtr < _pEndOfMemPoolArray));

	MEM_POOLNEW* pPool = (MEM_POOLNEW*)((char*)pPtr - (((char*)(pPtr)-(char*)_pMemPoolArray)%sizeof(MEM_POOLNEW)));

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	*pLibId = *(UInt32*)(((char*)pPtr) + pPool->_uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4 + 8);
#endif

	return pPool->_uElementSize;
}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		CheckConsistency
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::CheckConsistency()
{
	if(!MEM::GetInstance()->_debugConfig._bAllowCheckConsistency)
		return;

	// small consistency check
	// it just check that the number of used pools + num of free pools == total number of pools
	// it also check that within a pool, all chunk are either used or in the free list
	// (by comparing the number of entry in the free list to the number of free chunk of the pool)

	//TODO: extend this when we are using the MEM_POOLNEW specific debug parts

	UInt32 i;
	UInt32 numParsedPools = 0;

	// parse used pools
	for(i=0;i<_uNumEntryPoints;i++)
	{
		UInt32 uElementSize = (i+1)*_uEntryStep;
		MEM_POOLNEW* pCurrentPool = _pMemPoolEntryTable[i];

		while(pCurrentPool)
		{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
			pCurrentPool->CheckCorruptionMarker(uElementSize);
#endif
			UInt32 numParsedChunks = 0;

			char* pCurrentChunk = pCurrentPool->_pFirstFreeChunk;

			while(pCurrentChunk)
			{
				pCurrentChunk = *(char**)pCurrentChunk;
				numParsedChunks++;
				QDT_ASSERT(numParsedChunks <= pCurrentPool->_NumElementInPool);
			}

			QDT_ASSERT(numParsedChunks == pCurrentPool->_NumFreeElementInPool);

			pCurrentPool = pCurrentPool->_pNextPool;

			numParsedPools++;
		}
	}

	// parse free pools
	MEM_POOLNEW* pCurrentPool = _pFirstFreePool;

	while(pCurrentPool)
	{
		pCurrentPool = pCurrentPool->_pNextPool;

		numParsedPools++;
	}

	QDT_ASSERT(numParsedPools == _uNumPool);
}

//-----------------------------------------------------------------------------
//	Name:		GetNumberOfUsedMemPool
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_POOL_MANAGER::GetNumberOfUsedMemPool()
{
	UInt32 i;
	UInt32 numParsedPools = 0;

	// parse used pools
	for(i=0;i<_uNumEntryPoints;i++)
	{
		MEM_POOLNEW* pCurrentPool = _pMemPoolEntryTable[i];

		while(pCurrentPool)
		{
			pCurrentPool = pCurrentPool->_pNextPool;

			numParsedPools++;
		}
	}
	return numParsedPools;
}

//-----------------------------------------------------------------------------
//	Name:		GetNumberOfUsedMemPoolChunk
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_POOL_MANAGER::GetNumberOfUsedMemPoolChunk()
{
	UInt32 i;
	UInt32 uNumPool = _iMaxSizeMPAlloc/_uEntryStep;
	UInt32 numUsedChunk = 0;

	// parse used pools
	for(i=0;i<uNumPool;i++)
	{
		MEM_POOLNEW* pCurrentPool = _pMemPoolEntryTable[i];

		while(pCurrentPool)
		{
			numUsedChunk += pCurrentPool->_NumElementInPool - pCurrentPool->_NumFreeElementInPool;

			pCurrentPool = pCurrentPool->_pNextPool;
		}
	}
	return numUsedChunk;
}
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		SetupExtraDebugInfo
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::SetupExtraDebugInfo(void* pPtr, UInt32 uId, UInt32 uDepth)
{
	MEM_POOLNEW* pPool = (MEM_POOLNEW*)((char*)pPtr - (((char*)(pPtr)-(char*)_pMemPoolArray)%sizeof(MEM_POOLNEW)));

#ifdef QDT_MEM_USE_MEMPOOL_CALLSTACK
	{
		UInt32 i;

		for(i=0;i<_iMaxStackDepth;i++)
		{
			// the +4 will have to be changed if some part of the MEM lib are passed
			*(void**)(((char*)pPtr) + pPool->_uElementSize + _iPoolCorruptionMarkerSize + i*4) = OS::GetCallStack(uDepth+i);
		}
	}
#endif

#ifdef QDT_MEM_DEBUG
	if(TIME::GetInstance())
	{
		*(Float64*)(((char*)pPtr) + pPool->_uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4) = TIME::Instance()->GetSystemTime();
	}
	else
	{
		*(Float64*)(((char*)pPtr) + pPool->_uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4) = 0.f;
	}
	*(UInt32*)(((char*)pPtr) + pPool->_uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4 + 8) = uId;
#endif

#ifdef QDT_MEM_DEBUG
	// increment the lib use counter
	QDT_ASSERT(uId < _iNumMaxLibs);
	_aLibInfo[uId]._uCurrentUsage += pPool->_uElementSize;
	if(_aLibInfo[uId]._uPeakUsage < _aLibInfo[uId]._uCurrentUsage)
	{
		_aLibInfo[uId]._uPeakUsage = _aLibInfo[uId]._uCurrentUsage;
		if(TIME::GetInstance())
		{
			_aLibInfo[uId]._fTimeOfPeak = TIME::Instance()->GetSystemTime();
		}
		else
		{
			_aLibInfo[uId]._fTimeOfPeak = 0;
		}
	}
#endif
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		AllocateCustom
//	Object:		
//	06-07-06:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL_MANAGER::AllocateCustom(UInt32	uSize, UInt32 uLibId)
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(uSize);
	}

	MEM_ALLOCATION_REQUEST request;

	request._uSize = uSize;
	request._uBoundary = 0;

	request._uLibId = uLibId;
	request._uDepth = 1;

	Lock();
	void* ptr = Allocate(&request);
	Unlock();

	return ptr;
}

//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_MANAGER::DumpUsageMap( IO_STREAM* pStream )
{
	char textBuffer[256];
	UInt32 i;

	pStream->PutString("\t<MEM_POOL_MANAGER>");

	sprintf(textBuffer, "\t\t<NumEntryPoints>%d</NumEntryPoints>", _uNumEntryPoints);
	pStream->PutString(textBuffer);
	sprintf(textBuffer, "\t\t<EntryStep>%d</EntryStep>", _uEntryStep);
	pStream->PutString(textBuffer);
	sprintf(textBuffer, "\t\t<NumPool>%d</NumPool>", _uNumPool);
	pStream->PutString(textBuffer);

	for(i=0;i<_uNumEntryPoints;i++)
	{
		pStream->PutString("\t\t\t<EntryPoint>");

		MEM_POOLNEW* pCurrentPool =  _pMemPoolEntryTable[i];

		while(pCurrentPool)
		{
			pCurrentPool->DumpUsageMap( pStream );
			pCurrentPool = pCurrentPool->_pNextPool;
		}

		pStream->PutString("\t\t\t</EntryPoint>");
	}

	pStream->PutString("\t\t\t<FreePools>");
	
	{
		MEM_POOLNEW* pCurrentPool =  _pFirstFreePool;

		while(pCurrentPool)
		{
			pStream->PutString("\t\t\t\t<POOL></POOL>");
			pCurrentPool = pCurrentPool->_pNextPool;
		}
	}

	pStream->PutString("\t\t\t</FreePools>");
	pStream->PutString("\t</MEM_POOL_MANAGER>");
}
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		getCurrentUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_POOL_MANAGER::GetCurrentUsageForLib(	UInt32 uLibId)
{
	QDT_ASSERT(uLibId < _iNumMaxLibs);

	return _aLibInfo[uLibId]._uCurrentUsage;
}
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		getPeakUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_POOL_MANAGER::GetPeakUsageForLib(	UInt32 uLibId)
{
	QDT_ASSERT(uLibId < _iNumMaxLibs);

	return _aLibInfo[uLibId]._uPeakUsage;
}
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		getPeakUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_POOL_MANAGER::GetTotalUsage()
{
	UInt32 i;
	UInt32 uUsage = 0;

	for(i=0;i<_iNumMaxLibs;i++)
	{
		uUsage += _aLibInfo[i]._uCurrentUsage;
	}
	return uUsage;
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		GetNumAllocation
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_POOL_MANAGER::GetNumAllocation()
{
	return _uNumAllocation;
}
#endif

#ifdef _DEBUG
#include "MemPoolManager.inl"
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
