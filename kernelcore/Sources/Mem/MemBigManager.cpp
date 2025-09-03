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
//
//	06-02-08:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#define QDT_MEM_INTERNAL

#include	"Root.h"
#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Mem/MemBigManager)

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
#include INCL_KCORE(TIME/TIME_ABC)
#endif

#undef new

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MEM_BIG_MANAGER constructor
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
MEM_BIG_MANAGER::MEM_BIG_MANAGER()
{
#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	_pFirstChunk = MEM_BIG_CHUNK::CreateNewBigChunk();
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	{
		UInt32 i;

		for(i=0;i<_iNumMaxLibs;i++)
		{
			_aLibInfo[i]._uCurrentUsage = 0;
			_aLibInfo[i]._uPeakUsage = 0;
			_aLibInfo[i]._fTimeOfPeak = 0.f;
		}
	}
#endif

#ifdef QDT_MEM_DEBUG
	_uNumAllocation = 0;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		MEM_BIG_MANAGER destructor
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
MEM_BIG_MANAGER::~MEM_BIG_MANAGER()
{
#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	// could be done with recursivity...
	while(pCurrentChunk)
	{
		MEM_BIG_CHUNK* pNextChunk = pCurrentChunk->_pNextChunk;

		delete pCurrentChunk;

		pCurrentChunk = pNextChunk;
	}
#endif
}

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		IncrementLibUsage
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
void MEM_BIG_MANAGER::IncrementLibUsage(	UInt32	uSize,
											UInt32	uID)
{
	QDT_ASSERT(uID < _iNumMaxLibs);
	_aLibInfo[uID]._uCurrentUsage += uSize;

	if(_aLibInfo[uID]._uPeakUsage < _aLibInfo[uID]._uCurrentUsage)
	{
		_aLibInfo[uID]._uPeakUsage = _aLibInfo[uID]._uCurrentUsage;

		if(MEM::GetInstance()->_bIsCreatingTime)
			_aLibInfo[uID]._fTimeOfPeak = 0;
		else
			_aLibInfo[uID]._fTimeOfPeak = TIME::Instance()->GetSystemTime();
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_BIG_MANAGER::Allocate(	MEM_ALLOCATION_REQUEST* pRequest)
{
	QDT_ASSERT(g_MemConfig._bIsActive);

	if(pRequest->_uSize % _iBigChunkAllocationAllignement)
	{
		pRequest->_uSize += _iBigChunkAllocationAllignement - (pRequest->_uSize%_iBigChunkAllocationAllignement);
		QDT_ASSERT((pRequest->_uSize % _iBigChunkAllocationAllignement) == 0);
	}

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	void* pPtr = _pFirstChunk->Allocate(pRequest);

#ifdef QDT_MEM_DEBUG
	UInt32 uWasted = GetLastMallocWastedMemory();
	IncrementLibUsage(pRequest->_uSize + uWasted, pRequest->_uLibId);
#endif

#ifdef QDT_MEM_DEBUG
	_uNumAllocation++;
#endif

	return pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		AllocateAtEnd
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_BIG_MANAGER::AllocateAtEnd(	MEM_ALLOCATION_REQUEST* pRequest)
{
	QDT_ASSERT(g_MemConfig._bIsActive);

	if(pRequest->_uSize % _iBigChunkAllocationAllignement)
	{
		pRequest->_uSize += _iBigChunkAllocationAllignement - (pRequest->_uSize%_iBigChunkAllocationAllignement);
		QDT_ASSERT((pRequest->_uSize % _iBigChunkAllocationAllignement) == 0);
	}
	QDT_ASSERT(pRequest->_uSize < _iBigChunkSize - sizeof(MEM_BIG_CHUNK_ELEMENT));

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	void* pPtr = _pFirstChunk->AllocateAtEnd(pRequest);

#ifdef QDT_MEM_DEBUG
	UInt32 uWasted = GetLastMallocWastedMemory();
	IncrementLibUsage(pRequest->_uSize + uWasted, pRequest->_uLibId);
#endif

#ifdef QDT_MEM_DEBUG
	_uNumAllocation++;
#endif

	return pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		AllocateAlign
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_BIG_MANAGER::AllocateAlign(	MEM_ALLOCATION_REQUEST* pRequest)
{
	QDT_ASSERT(g_MemConfig._bIsActive);

	if(pRequest->_uSize % _iBigChunkAllocationAllignement)
	{
		pRequest->_uSize += _iBigChunkAllocationAllignement - (pRequest->_uSize%_iBigChunkAllocationAllignement);
		QDT_ASSERT((pRequest->_uSize % _iBigChunkAllocationAllignement) == 0);
	}
	QDT_ASSERT(pRequest->_uSize < _iBigChunkSize - sizeof(MEM_BIG_CHUNK_ELEMENT));

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	void* pPtr = _pFirstChunk->AllocateAlign(pRequest);

#ifdef QDT_MEM_DEBUG
	UInt32 uWasted = GetLastMallocWastedMemory();
	IncrementLibUsage(pRequest->_uSize + uWasted, pRequest->_uLibId);
#endif

#ifdef QDT_MEM_DEBUG
	_uNumAllocation++;
#endif

	return pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
Bool	MEM_BIG_MANAGER::Free(const void* pPtr)
{
#ifdef QDT_MEM_DEBUG
	MEM::GetInstance()->CheckFreeTrace(pPtr);
#endif

	QDT_ASSERT(g_MemConfig._bIsActive);

	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		// is in range ?
		if(((char*)pPtr >= pCurrentChunk->GetRawChunkPtr()) && ((char*)pPtr <= pCurrentChunk->GetRawEndChunkPtr()))
		{
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			// decrement lib usage
			{
				MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));

				QDT_ASSERT(pElement->_iId < _iNumMaxLibs);
				QDT_ASSERT(_aLibInfo[pElement->_iId]._uCurrentUsage >= pElement->_blockSize);
				_aLibInfo[pElement->_iId]._uCurrentUsage -= pElement->_blockSize;
			}
#endif
			pCurrentChunk->Free(pPtr);
#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
			if((pCurrentChunk->_pNextChunk) && (pCurrentChunk->_pNextChunk->_uAllocatedSize == 0))
			{
				if(pCurrentChunk->_uFullSize - pCurrentChunk->_uAllocatedSize<(_iBigChunkDestructionTreshold * pCurrentChunk->_uFullSize)/100)
				{
					MEM_BIG_CHUNK* pChunkToDelete = pCurrentChunk->_pNextChunk;

					pCurrentChunk->_pNextChunk = pCurrentChunk->_pNextChunk->_pNextChunk;
					delete pChunkToDelete;
				}
			}
#endif

#ifdef QDT_MEM_DEBUG
			_uNumAllocation--;
#endif

			return true;
		}

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
		pCurrentChunk = pCurrentChunk->_pNextChunk;
#else
		pCurrentChunk = NULL;
#endif
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetDataSize
//	Object:		
//	06-03-07:	VHA - Created
//-----------------------------------------------------------------------------
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
UInt32	MEM_BIG_MANAGER::GetDataSize(void *		pPtr,
									 UInt32 *	pLibId)
#else
UInt32	MEM_BIG_MANAGER::GetDataSize(void *		pPtr)
#endif
{
	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		// is in range ?
		if(((char*)pPtr >= pCurrentChunk->GetRawChunkPtr()) && ((char*)pPtr <= pCurrentChunk->GetRawEndChunkPtr()))
		{
			MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			// get lib id
			{
				QDT_ASSERT(pElement->_iId < _iNumMaxLibs);
				QDT_ASSERT(_aLibInfo[pElement->_iId]._uCurrentUsage >= pElement->_blockSize);
				*pLibId = pElement->_iId;
			}
#endif
			return pElement->_blockSize;
		}

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
		pCurrentChunk = pCurrentChunk->_pNextChunk;
#else
		pCurrentChunk = NULL;
#endif
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		Shrink
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_MANAGER::Shrink(void* pPtr, UInt32 uSize)
{
	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		// is in range ?
		if(((char*)pPtr >= pCurrentChunk->GetRawChunkPtr()) && ((char*)pPtr <= pCurrentChunk->GetRawEndChunkPtr()))
		{
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			// decrement lib usage
			{
				MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));

				QDT_ASSERT(pElement->_iId < _iNumMaxLibs);
				QDT_ASSERT(_aLibInfo[pElement->_iId]._uCurrentUsage >= pElement->_blockSize);
				_aLibInfo[pElement->_iId]._uCurrentUsage -= pElement->_blockSize - uSize;
			}
#endif

			pCurrentChunk->Shrink(pPtr, uSize);

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			{
				MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
				UInt32 uWasted = GetLastMallocWastedMemory();
				_aLibInfo[pElement->_iId]._uCurrentUsage += uWasted;
			}
#endif
			break;
		}

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
		pCurrentChunk = pCurrentChunk->_pNextChunk;
#else
		pCurrentChunk = NULL;
#endif
	}

}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		CheckConsistency
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_MANAGER::CheckConsistency()
{
	if(!MEM::GetInstance()->_debugConfig._bAllowCheckConsistency)
		return;

	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		pCurrentChunk->CheckConsistency();

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
		pCurrentChunk = pCurrentChunk->_pNextChunk;
#else
		pCurrentChunk = NULL;
#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetBigChunkAllocatedSize
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_BIG_MANAGER::GetBigChunkAllocatedSize()
{
	UInt32 uSize = 0;
	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		uSize += pCurrentChunk->GetAllocatedSize();
#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
		pCurrentChunk = pCurrentChunk->_pNextChunk;
#else
		pCurrentChunk = NULL;
#endif
	}

	return uSize;
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_MANAGER::DumpUsageMap( IO_STREAM* pStream )
{
	pStream->PutString("\t<MEM_BIG_MANAGER>");

	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		pCurrentChunk->DumpUsageMap( pStream );
		pCurrentChunk = pCurrentChunk->_pNextChunk;
	}

	pStream->PutString("\t</MEM_BIG_MANAGER>");
}
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		getCurrentUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_BIG_MANAGER::GetCurrentUsageForLib(	UInt32 uLibId)
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
UInt32	MEM_BIG_MANAGER::GetPeakUsageForLib(	UInt32 uLibId)
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
UInt32	MEM_BIG_MANAGER::GetTotalUsage()
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
//	Name:		GetLastMallocWastedMemory
//	Object:		
//	06-02-17:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_BIG_MANAGER::GetLastMallocWastedMemory()
{
	UInt32 uUsage = 0;

	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		uUsage += pCurrentChunk->_uWastedMemory;
		pCurrentChunk = pCurrentChunk->_pNextChunk;
	}

	return uUsage;
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		ResetLastMallocWastedMemory
//	Object:		
//	06-02-20:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_MANAGER::ResetLastMallocWastedMemory()
{
	MEM_BIG_CHUNK* pCurrentChunk = _pFirstChunk;

	while(pCurrentChunk)
	{
		pCurrentChunk->_uWastedMemory = 0;
		pCurrentChunk = pCurrentChunk->_pNextChunk;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNumAllocation
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_BIG_MANAGER::GetNumAllocation()
{
	return _uNumAllocation;
}

#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
