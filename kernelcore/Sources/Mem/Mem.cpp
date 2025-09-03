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
//
//	06-02-07:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#define QDT_MEM_INTERNAL

#include	"Root.h"

#ifdef PLATFORM_PC
#include	<malloc.h>
#endif

#ifdef PLATFORM_PS3
#undef new
	inline	void *	operator new		(unsigned int, void * ptr) { return (ptr); } // new with placement
#endif

#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Mem/MemPoolManager)
#include	INCL_KCORE(Mem/MemBigManager)
#include	INCL_KCORE(Mem/Mem)
#include	INCL_KCORE(Thread/MUTEX)
#include	INCL_KCORE(Stream/FileStream)
#include	INCL_KCORE(TimeLog/TimeLog)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#ifdef QDT_NEW_REDEFINED
#undef new
#endif


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

MEM* MEM::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		MEM constructor
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
MEM::MEM()
{
	QDT_ASSERT(_pInstance == NULL);
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);

	_pInstance = this;

	_pMemConfig = &g_MemConfig;

#ifdef QDT_MEM_DEBUG
	_debugConfig._bUseCorruptionProtection = false;
	_debugConfig._bUseCallstack = false;
	_debugConfig._bUseTimeId = false;
	_debugConfig._bUseArrayProtection = false;

	_debugConfig._bAllowCheckConsistency = false;
#endif

	_pMemPoolManagerCustom = NULL;
	_pMemPoolManager = new MEM_POOL_MANAGER(_iMaxSizeMPAlloc/_iStepSizeMPAlloc, _iStepSizeMPAlloc, (_iMaxSizeMPAlloc/_iStepSizeMPAlloc) + _iNumAdditionalPool, MP_CUSTOMID_NOTCUSTOM, true);
	_pMemBigManager = new MEM_BIG_MANAGER();

	if(g_MemConfig._bIsActive == mem_enabled)
	{
		// we can't use a "new" as the mutex doesn't exist yet.
		_Mutex = (MUTEX*)_pMemPoolManager->Allocate(sizeof(MUTEX));
		_InnerMutex = (MUTEX*)_pMemPoolManager->Allocate(sizeof(MUTEX));
		QDT_ASSERT(_Mutex);
		QDT_ASSERT(_InnerMutex);
	}
	else
	{
		_Mutex = (MUTEX*)malloc(sizeof(MUTEX));
		_InnerMutex = (MUTEX*)malloc(sizeof(MUTEX));
		QDT_ASSERT(_Mutex);
		QDT_ASSERT(_InnerMutex);
	}

	#ifndef PLATFORM_PS3
	_Mutex->MUTEX::MUTEX();
	_InnerMutex->MUTEX::MUTEX();
	#else
	new (_Mutex) MUTEX;
	new (_InnerMutex) MUTEX;
	#endif

#ifdef QDT_MEM_DEBUG
	pNextPtr=NULL;
#endif

#ifdef QDT_MEM_DEBUG
	if(g_MemConfig._bIsActive == mem_enabled)
	{
		_DebugNextPtrMutex = (MUTEX*)_pMemPoolManager->Allocate(sizeof(MUTEX));
	}
	else
	{
		_DebugNextPtrMutex = (MUTEX*)malloc(sizeof(MUTEX));
	}

#ifndef PLATFORM_PS3
		_DebugNextPtrMutex->MUTEX::MUTEX();
#else
		new (_DebugNextPtrMutex) MUTEX;
#endif

#endif

#ifdef QDT_MEM_DEBUG
	_bIsCreatingTime = true;
#endif
	TIME::Instance()->GetSystemTime(); // force time creation
#ifdef QDT_MEM_DEBUG
	_bIsCreatingTime = false;
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
	_pMemTimeLogTable = new TIME_LOG[_iNumMaxLibs];

	{
		UInt32 i;

		for(i=0;i<_iNumMaxLibs;i++)
		{
			_pMemTimeLogTable[i].SetName(_pLibIdNames[i]);
			_pMemTimeLogTable[i].Init(10.f * 1000.f, 100.f); // 10sec
		}
	}
#endif

#ifdef QDT_MEM_DEBUG
	{
		UInt32 i;

		for(i=0;i<_iMaxFreeTrace;i++)
		{
			_freeTraceTable[i] = 0;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		MEM destructor
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
MEM::~MEM()
{
	TIME::Suicide();

	MEM_POOL_MANAGER* pCurrentMemPoolCustom;

	pCurrentMemPoolCustom = _pMemPoolManagerCustom;

	while(pCurrentMemPoolCustom)
	{
		MEM_POOL_MANAGER* pNext = _pMemPoolManagerCustom->_pNextManager;
		delete _pMemPoolManagerCustom;

		pCurrentMemPoolCustom = pNext;
	}

#ifdef QDT_MEM_DEBUG
	delete[] _pMemTimeLogTable;
#endif

#ifdef QDT_MEM_THREAD_SAFE
//	delete _Mutex;
#endif
	delete _pMemPoolManager;
	delete _pMemBigManager;
}

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM::Allocate(UInt32	uSize)
{
	void* returnPtr;
	QDT_ASSERT(uSize);

	MEM_ALLOCATION_REQUEST request;

	request._uSize = uSize;
	request._uBoundary = 0;

#ifdef QDT_MEM_DEBUG
	request._uDepth = 1;
	request._uLibId = 0;
#endif

	returnPtr = Allocate(&request);

	return returnPtr;
}

#ifdef QDT_MEM_ENABLED
//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM::Allocate(MEM_ALLOCATION_REQUEST* pRequest)
{
	void* pPtr;

	Lock();

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	QDT_FAIL();
#endif

	if(pRequest->_uSize <= _iMaxSizeMPAlloc)
	{
		pPtr = _pMemPoolManager->Allocate(pRequest);

		if(pPtr == NULL)
		{
			pPtr = _pMemBigManager->Allocate(pRequest);
		}
	}
	else
	{
		pPtr = _pMemBigManager->Allocate(pRequest);
	}

	Unlock();

	QDT_ASSERT((((UInt32)pPtr)%_iBigChunkAllocationAllignement) == 0);

	return pPtr;
}

#if 0
//-----------------------------------------------------------------------------
//	Name:		AllocateAtEnd
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
void *	MEM::AllocateAtEnd(UInt32	uSize,
						   UInt32	uId,
						   UInt32	uDepth)
#else
void *	MEM::AllocateAtEnd(UInt32	uSize)
#endif
{
	void* pPtr;

	QDT_ASSERT(uSize);

	Lock();

#ifndef QDT_MEM_USE_ARRAY_PROTECTION
	if(uSize <= _iMaxSizeMPAlloc)
	{
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		pPtr = _pMemPoolManager->Allocate(uSize, uId, uDepth+1);
#else
		pPtr = _pMemPoolManager->Allocate(uSize);
#endif
		if(pPtr == NULL)
		{
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			pPtr = _pMemBigManager->AllocateAtEnd(uSize, uId, uDepth+1);
#else
			pPtr = _pMemBigManager->AllocateAtEnd(uSize);
#endif
		}
	}
	else
#endif
	{
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
		pPtr = _pMemBigManager->AllocateAtEnd(uSize, uId, uDepth+1);
#else
		pPtr = _pMemBigManager->AllocateAtEnd(uSize);
#endif
	}

	Unlock();

	QDT_ASSERT((((UInt32)pPtr)%_iBigChunkAllocationAllignement) == 0);

	return pPtr;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		AllocateAlign
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM::AllocateAlign(UInt32	uBoundary, UInt32	uSize)
{
	void* returnPtr;
	QDT_ASSERT(uSize);

	MEM_ALLOCATION_REQUEST request;

	request._uSize = uSize;

#ifdef QDT_MEM_DEBUG
	request._uDepth = 1;
	request._uLibId = 0;
#endif

	QDT_ASSERT((uBoundary % _iBigChunkAllocationAllignement) == 0);

	if(uBoundary > _iBigChunkAllocationAllignement)
	{
		request._uBoundary = uBoundary;
		returnPtr = AllocateAlign(&request);
	}
	else
	{
		request._uBoundary = 0;
		returnPtr = Allocate(&request);
	}

	QDT_ASSERT((((int)returnPtr)%uBoundary) == 0);

	return returnPtr;
}

//-----------------------------------------------------------------------------
//	Name:		AllocateAlign
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------

void *	MEM::AllocateAlign(	MEM_ALLOCATION_REQUEST* pRequest)
{
	void* pPtr;

	if(g_MemConfig._bIsActive == mem_disabled)
	{
#ifdef PLATFORM_PC
		void* pNewPtr = _aligned_malloc(pRequest->_uSize, pRequest->_uBoundary);
		QDT_ASSERT((((int)pNewPtr)%_iBigChunkAllocationAllignement) == 0);

		return pNewPtr;
#else
		QDT_FAIL();
#endif
	}

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	Lock();
	pPtr = _pMemBigManager->AllocateAlign(pRequest);
	Unlock();

	QDT_ASSERT((((UInt32)pPtr)%_iBigChunkAllocationAllignement) == 0);

	return pPtr;
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::Free(const void *	pPtr)
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free((void*)pPtr);
		return;
	}

	if (pPtr == NULL) return; // TODO clean...

	Lock();
	if(!_pMemBigManager->Free(pPtr))
	{
		_pMemPoolManager->Free(pPtr);
		//_pMemPoolManager->Optimize();
	}
	Unlock();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Realloc
//	Object:		
//	06-03-07:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM::Realloc(void *	pPtr,
					 UInt32	uSize)
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		void* pNewPtr = realloc(pPtr, uSize);
		return pNewPtr;
	}

	if(!pPtr)
	{
		char* pNewPtr;
		pNewPtr = (char*)Allocate(uSize);
		return pNewPtr;
	}
	else
	{
		char* pNewPtr;
		UInt32 uOldSize;
#ifdef QDT_MEM_DEBUG
		UInt32 uLibId;
#endif

#ifdef QDT_MEM_THREAD_SAFE
		_InnerMutex->Lock();
#endif

#ifdef QDT_MEM_DEBUG
		uOldSize = _pMemBigManager->GetDataSize(pPtr,&uLibId);
#else
		uOldSize = _pMemBigManager->GetDataSize(pPtr);
#endif

		if(!uOldSize)
		{
#ifdef QDT_MEM_DEBUG
			uOldSize = _pMemPoolManager->GetDataSize(pPtr,&uLibId);
#else
			uOldSize = _pMemPoolManager->GetDataSize(pPtr);
#endif
		}

		QDT_ASSERT(uOldSize);

		if(uOldSize == uSize)
		{
#ifdef QDT_MEM_THREAD_SAFE
			_InnerMutex->Unlock();
#endif
			//QDT_ASSERT((((UInt32)pPtr)%_iBigChunkAllocationAllignement) == 0);
			return pPtr;
		}

		// allocate the new buffer
		{
			MEM_ALLOCATION_REQUEST request;
			request._uSize = uSize;
			request._uBoundary = 0;
#ifdef QDT_MEM_DEBUG
			request._uDepth = 1;
			request._uLibId = uLibId;
#endif
			
			pNewPtr = (char*)Allocate(&request);
		}

		if(uSize < uOldSize)
		{
			QDT_MEMCOPY(pNewPtr, pPtr, uSize);
		}
		else
		{
			QDT_MEMCOPY(pNewPtr, pPtr, uOldSize);
		}

		Free(pPtr);

#ifdef QDT_MEM_THREAD_SAFE
		_InnerMutex->Unlock();
#endif

		//QDT_ASSERT((((UInt32)pNewPtr)%_iBigChunkAllocationAllignement) == 0);
		return pNewPtr;
	}
}



//-----------------------------------------------------------------------------
//	Name:		Shrink
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::Shrink(	void* pPtr,
						UInt32 uSize)
{
	Lock();
	_pMemBigManager->Shrink(pPtr, uSize);
	Unlock();
}

//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-06-15:	RMA - Created
//-----------------------------------------------------------------------------
#ifdef QDT_MEM_DEBUG
void	MEM::DumpUsageMap( const QDT_STRING & sFileName )
{
	FILE_STREAM File(sFileName, FILE_STREAM::AT_CREATE);
	DumpUsageMap(&File);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
#ifdef QDT_MEM_DEBUG
void	MEM::DumpUsageMap( IO_STREAM* pStream )
{
	Lock();
	pStream->PutString("<?xml version=\"1.0\"?>");
	pStream->PutString("<ROOT>");

	_pMemPoolManager->DumpUsageMap( pStream );
	_pMemBigManager->DumpUsageMap( pStream );

	MEM_POOL_MANAGER* pCurrentManager = _pMemPoolManagerCustom;

	while(pCurrentManager)
	{
		pCurrentManager->DumpUsageMap( pStream );
		pCurrentManager = pCurrentManager->_pNextManager;
	}

	pStream->PutString("</ROOT>");
	Unlock();
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		CheckConsistency
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::CheckConsistency()
{
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return;
	}

	Lock();
	_pMemPoolManager->CheckConsistency();
	_pMemBigManager->CheckConsistency();

	MEM_POOL_MANAGER* pCurrentManager = _pMemPoolManagerCustom;

	while(pCurrentManager)
	{
		pCurrentManager->CheckConsistency();
		pCurrentManager = pCurrentManager->_pNextManager;
	}
	Unlock();
}

//-----------------------------------------------------------------------------
//	Name:		GetNumberOfUsedMemPool
//	Object:		
//	06-02-07:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetNumberOfUsedMemPool()
{
	Lock();
	int result = _pMemPoolManager->GetNumberOfUsedMemPool();
	Unlock();
	return result;
}

//-----------------------------------------------------------------------------
//	Name:		GetNumberOfUsedMemPoolChunk
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetNumberOfUsedMemPoolChunk()
{
	Lock();
	int result = _pMemPoolManager->GetNumberOfUsedMemPoolChunk();
	Unlock();
	return result;
}

//-----------------------------------------------------------------------------
//	Name:		GetBigChunkAllocatedSize
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetBigChunkAllocatedSize()
{
	Lock();
	int result =  _pMemBigManager->GetBigChunkAllocatedSize();
	Unlock();
	return result;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		DeleteMpCustom
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void MEM::DeleteMpCustom(MEM_POOL_MANAGER* pMpCustom)
{
	Lock();
	MEM_POOL_MANAGER* pPrevious = NULL;
	MEM_POOL_MANAGER* pCurrentMpCustom = _pMemPoolManagerCustom;

	while(pCurrentMpCustom)
	{
		if(pCurrentMpCustom == pMpCustom)
		{
			if(pPrevious)
			{
				pPrevious->_pNextManager = pMpCustom->_pNextManager;
			}
			else
			{
				_pMemPoolManagerCustom = pMpCustom->_pNextManager;
			}

			Free(pMpCustom);
			break;
		}

		pPrevious = pCurrentMpCustom;
		pCurrentMpCustom = pCurrentMpCustom->_pNextManager;
	}

	QDT_ASSERT(pCurrentMpCustom);
	Unlock();
}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		getCurrentUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetCurrentUsageForLib(	UInt32 uLibId)
{
	Lock();
	UInt32 uUsage = 0;

	uUsage += _pMemBigManager->GetCurrentUsageForLib(uLibId);
	uUsage += _pMemPoolManager->GetCurrentUsageForLib(uLibId);

	MEM_POOL_MANAGER* pCurrentPoolManager = _pMemPoolManagerCustom;
	
	while(pCurrentPoolManager)
	{
		uUsage += pCurrentPoolManager->GetCurrentUsageForLib(uLibId);
		pCurrentPoolManager = pCurrentPoolManager->_pNextManager;
	}

	Unlock();
	return uUsage;
}

//-----------------------------------------------------------------------------
//	Name:		getPeakUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetPeakUsageForLib(	UInt32 uLibId)
{
	Lock();
	UInt32 uUsage = 0;

	uUsage += _pMemBigManager->GetPeakUsageForLib(uLibId);
	uUsage += _pMemPoolManager->GetPeakUsageForLib(uLibId);

	MEM_POOL_MANAGER* pCurrentPoolManager = _pMemPoolManagerCustom;
	
	while(pCurrentPoolManager)
	{
		uUsage += pCurrentPoolManager->GetPeakUsageForLib(uLibId);
		pCurrentPoolManager = pCurrentPoolManager->_pNextManager;
	}

	Unlock();
	return uUsage;
}

//-----------------------------------------------------------------------------
//	Name:		getPeakUsageForLib
//	Object:		
//	06-02-16:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetTotalUsage()
{
	Lock();
	UInt32 uUsage = 0;

	uUsage += _pMemBigManager->GetTotalUsage();
	uUsage += _pMemPoolManager->GetTotalUsage();

	MEM_POOL_MANAGER* pCurrentPoolManager = _pMemPoolManagerCustom;
	
	while(pCurrentPoolManager)
	{
		uUsage += pCurrentPoolManager->GetTotalUsage();
		pCurrentPoolManager = pCurrentPoolManager->_pNextManager;
	}
	Unlock();
	return uUsage;
}

//-----------------------------------------------------------------------------
//	Name:		GetLastMallocWastedMemory
//	Object:		
//	06-02-17:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetLastMallocWastedMemory()
{
	Lock();
	int result =  _pMemBigManager->GetLastMallocWastedMemory();
	Unlock();
	return result;
}

//-----------------------------------------------------------------------------
//	Name:		ResetLastMallocWastedMemory
//	Object:		
//	06-02-20:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::ResetLastMallocWastedMemory()
{
	Lock();
	_pMemBigManager->ResetLastMallocWastedMemory();
	Unlock();
}

void MEM::SetNextReturnPtr(void* pPtr)
{
	//MEM::_DebugNextPtrMutex->Lock();
	GetInstance()->pNextPtr = pPtr;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		getMemPoolCustom
//	Object:		
//	06-02-23:	VHA - Created
//-----------------------------------------------------------------------------
MEM_POOL_MANAGER *	MEM::getMemPoolCustom(	UInt32 uNumEntryPoints,
											UInt32 uEntryStep,
											UInt32 uNumPool,
											UInt32 uMPIdx,
											Bool bIsPrimary )
{
#ifdef QDT_MEM_THREAD_SAFE
	_InnerMutex->Lock();
#endif
	MEM_POOL_MANAGER* pCurrentMemPool = _pMemPoolManagerCustom;

	while(pCurrentMemPool)
	{
		if(pCurrentMemPool->_uMPIdx == uMPIdx)
		{
#ifdef QDT_MEM_THREAD_SAFE
			_InnerMutex->Unlock();
#endif
			return pCurrentMemPool;
		}
		pCurrentMemPool = pCurrentMemPool->_pNextManager;
	}

	MEM_POOL_MANAGER* pNewMemPoolManager = new MEM_POOL_MANAGER(uNumEntryPoints, uEntryStep, uNumPool, uMPIdx, bIsPrimary);

#ifdef QDT_MEM_DEBUG
	strcpy(pNewMemPoolManager->_poolName, MEM_POOL_MANAGER::_pPoolNames[uMPIdx]);
#endif

	pNewMemPoolManager->_pNextManager = _pMemPoolManagerCustom;
	_pMemPoolManagerCustom = pNewMemPoolManager;

#ifdef QDT_MEM_THREAD_SAFE
	_InnerMutex->Unlock();
#endif

	return pNewMemPoolManager;
}

//-----------------------------------------------------------------------------
//	Name:		ExitCleanup
//	Object:		
//	06-02-27:	VHA - Created
//-----------------------------------------------------------------------------
void MEM::ExitCleanup()
{
//	GetInstance()->Lock();
//	delete _pInstance;
//	GetInstance()->Unlock();
}

//-----------------------------------------------------------------------------
//	Name:		Memcpy
//	Object:		
//	06-03-08:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::Memcpy(void *	pDest,
					const void *	pSource,
					UInt32	uSize)
{
	QDT_MEM_CHECKCONSISTENCY();

	QDT_MEMCOPY(pDest, pSource, uSize);	// TODO: à réimplementer de maniere optimisé sur ps3 (dma ?).
									// Uniquement valable pour les grosses copies

	QDT_MEM_CHECKCONSISTENCY();
}

#ifdef QDT_MEM_DEBUG
void MEM::DebugNextPtrMutexLock(void)
{
	MEM::GetInstance()->_DebugNextPtrMutex->Lock();
}
#endif
///////////// LIBID stuff

#ifdef QDT_MEM_DEBUG

#define ADD_TAG(a) #a,

char* MEM::_pLibIdNames[MEM_LIBID_MAX] = {
	"Default",
	"Unlisted",
#include	INCL_KCORE(Mem/MemLibId)
};

#undef ADD_TAG

#endif

/////////////////////////

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		operator delete
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void operator delete(	void * p,
						UInt32 ID,
						UInt32 uDepth)
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free(p);
		return;
	}
	
#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)p - sizeof(MEM_BIG_CHUNK_ELEMENT));
	QDT_ASSERT(pElement->_bIsArray == false);
#endif

	MEM::GetInstance()->Free(p);
}

//-----------------------------------------------------------------------------
//	Name:		operator delete
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void operator delete[](	void * p,
						UInt32 ID,
						UInt32 uDepth)
{
	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		free(p);
		return;
	}

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)p - sizeof(MEM_BIG_CHUNK_ELEMENT));
	QDT_ASSERT(pElement->_bIsArray == true);
#endif

	MEM::GetInstance()->Free(p);
}

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new[](	size_t cb,
						UInt32 ID,
						UInt32 uDepth)
{
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(cb);
	}

	MEM_ALLOCATION_REQUEST request;

	request._uSize = cb;
	request._uBoundary = 0;
	request._uLibId = ID;
	request._uDepth = uDepth + 1;

	void* pNewPtr = MEM::GetInstance()->Allocate(&request);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = true;
#endif

	return pNewPtr;
}

// NEW[](size_t,allign)

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new[](	size_t cb,
						UInt32 allign)
{
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(cb);
	}

	QDT_FAIL();

	void* pNewPtr = MEM::GetInstance()->AllocateAlign(allign,cb);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = true;
#endif

	return pNewPtr;
}

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-05-17:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new[](	size_t cb,
						UInt32 allign,
						UInt32 ID,
						UInt32 uDepth)
{
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(cb);
	}

	QDT_FAIL();

	MEM_ALLOCATION_REQUEST request;

	request._uSize = cb;
	request._uBoundary = allign;
	request._uLibId = ID;
	request._uDepth = uDepth+1;

	void* pNewPtr = MEM::GetInstance()->AllocateAlign(&request);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = true;
#endif

	return pNewPtr;
}

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new(	size_t s,
					UInt32 allign,
					UInt32 ID,
					UInt32 uDepth)
{
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(s);
	}

	MEM_ALLOCATION_REQUEST request;

	request._uSize = s;
	request._uBoundary = allign;
	request._uLibId = ID;
	request._uDepth = uDepth+1;

	void* pNewPtr = MEM::GetInstance()->AllocateAlign(&request);

#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = false;
#endif

	return pNewPtr;
}

//-----------------------------------------------------------------------------
//	Name:		operator new
//	Object:		
//	06-02-13:	VHA - Created
//-----------------------------------------------------------------------------
void* operator new(	size_t s,
					UInt32 ID,
					UInt32 uDepth)
{
	if(MEM::GetInstance()->pNextPtr)
	{
		void* pNext = MEM::GetInstance()->pNextPtr;
		MEM::GetInstance()->pNextPtr = NULL;
		MEM::GetInstance()->_DebugNextPtrMutex->Unlock();
		return pNext;
	}

	QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
	if(g_MemConfig._bIsActive == mem_disabled)
	{
		return malloc(s);
	}

	MEM_ALLOCATION_REQUEST request;

	request._uSize = s;
	request._uBoundary = 0;
	request._uLibId = ID;
	request._uDepth = uDepth+1;

	void* pNewPtr = MEM::GetInstance()->Allocate(&request);


#ifdef QDT_MEM_USE_ARRAY_PROTECTION
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pNewPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));
	pElement->_bIsArray = false;
#endif

	return pNewPtr;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::Update()
{
#ifdef QDT_MEM_DEBUG
	UInt32 i;

	Float64 fNow = TIME::Instance()->GetSystemTime();

	for(i=0;i<_iNumMaxLibs;i++)
	{
		_pMemTimeLogTable[i].AddValue(fNow, (Float32)GetCurrentUsageForLib(i));
	}
#endif
}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		AddFreeTrace
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::AddFreeTrace(const void *	pPtr)
{
	UInt32 i;

	for(i=0;i<_iMaxFreeTrace;i++)
	{
		if(_freeTraceTable[i] == 0)
		{
			_freeTraceTable[i] = (UInt32)pPtr;
			return;
		}
	}

	QDT_FAIL();
}


//-----------------------------------------------------------------------------
//	Name:		RemoveFreeTrace
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::RemoveFreeTrace(const void *	pPtr)
{
	UInt32 i;

	for(i=0;i<_iMaxFreeTrace;i++)
	{
		if(_freeTraceTable[i] == (UInt32)(pPtr))
		{
			_freeTraceTable[i] = 0;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CheckFreeTrace
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM::CheckFreeTrace(const void *	pPtr)
{
	UInt32 i;

	for(i=0;i<_iMaxFreeTrace;i++)
	{
		if(_freeTraceTable[i] == (UInt32)(pPtr))
		{
			QDT_BREAK;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNumAllocation
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM::GetNumAllocation()
{
	UInt32 numAllocation = 0;

	numAllocation += _pMemPoolManager->GetNumAllocation();
	numAllocation += _pMemBigManager->GetNumAllocation();

	MEM_POOL_MANAGER* pCurrentPoolManager = _pMemPoolManagerCustom;

	while(pCurrentPoolManager)
	{
		numAllocation += pCurrentPoolManager->GetNumAllocation();
		pCurrentPoolManager = pCurrentPoolManager->_pNextManager;
	}

	return numAllocation;
}

//-----------------------------------------------------------------------------
//	Name:		GetMemPoolManager
//	Object:		
//	06-07-07:	VHA - Created
//-----------------------------------------------------------------------------
MEM_POOL_MANAGER *	MEM::GetMemPoolManager(UInt32	poolIdx)
{
	MEM_POOL_MANAGER* pCurrentPoolManager = _pMemPoolManagerCustom;

	while(pCurrentPoolManager)
	{
		if(pCurrentPoolManager->_uMPIdx == poolIdx)
		{
			return pCurrentPoolManager;
		}
		pCurrentPoolManager = pCurrentPoolManager->_pNextManager;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	06-03-24:	VHA - Created
//-----------------------------------------------------------------------------
TIME_LOG* MEM::GetTimeLogForLib(UInt32 iLibIdx)
{
	return &_pMemTimeLogTable[iLibIdx];
}
#endif

#ifdef _DEBUG
#include "Mem.inl"
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
