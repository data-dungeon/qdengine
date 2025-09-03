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
//	CLASS:	MEM_POOL_HEADER
//
//	02-01-03:	ONA - Created
//*****************************************************************************

#include "Root.h"
#include "MemPoolHeader.h"

#include INCL_KCORE(Memory/MemPool)
#include INCL_KCORE(Stream/iostream)

#undef MEM_POOL_HEADER

#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_MEMORY
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

bool	MEM_POOL_HEADER::_bNoVerify = true;
int		MEM_POOL_HEADER::_nMemoryFree = 0;
int     MEM_POOL_HEADER::_nFreeListMemory = 0;

#ifdef TRACK_MEM_POOL_HEADERS
QDT_VECTOR<MEM_POOL_HEADER*>	MEM_POOL_HEADER::_MemPoolList;
#endif

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL_HEADER constructor
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
#ifndef TRACK_MEM_POOL_HEADERS
MEM_POOL_HEADER::MEM_POOL_HEADER(const int	nSize,
								 const int	nFirstCapacity,
								 const int	nNextCapacity)
:
_nSize(nSize),
_nFirstCapacity(nFirstCapacity),
_pFirstPool(NULL),
_nNbrFreeItems(0),
_nNbrAllocatedFreeItems(0),
_pFreeList(NULL),
_nNbrUsedItems(0)
{
	_nNextCapacity = nNextCapacity;
	
	AddMemPool();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL_HEADER constructor
//	Object:		
//	04-12-06:	ONA - Created
//-----------------------------------------------------------------------------
#ifdef TRACK_MEM_POOL_HEADERS
MEM_POOL_HEADER::MEM_POOL_HEADER(const int    nSize, 
								 const char * pcFile,
								 int		  nLine,
								 const int    nFirstCapacity, 
								 const int    nNextCapacity)
:
_nSize(nSize),
_nFirstCapacity(nFirstCapacity),
_pFirstPool(NULL),
_nNbrFreeItems(0),
_nNbrAllocatedFreeItems(0),
_pFreeList(NULL),
_nNbrUsedItems(0),
_nLine(nLine),
_nMaxNumberAllocated(0)
{
	_nNextCapacity = nNextCapacity;
	if	(_nNextCapacity < 0)
	{
		_nNextCapacity = nFirstCapacity;
	}

	AddMemPool();

	_MemPoolList.PushTail(this);

	strcpy(_tcFile, pcFile);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL_HEADER destructor
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
MEM_POOL_HEADER::~MEM_POOL_HEADER()
{
	SPOOL *pPool = _pFirstPool;
	SPOOL *pNextPool = NULL;

	while (pPool)
	{
		pNextPool = pPool->_pNext;
		
		if	(pPool == _pFirstPool)
		{
			_nFreeListMemory -= _nFirstCapacity * sizeof(void*);
		}
		else
		{
			_nFreeListMemory -= _nNextCapacity * sizeof(void*);
		}
		
		delete pPool->_pPool;
		pPool = pNextPool;
	}

	delete [] _pFreeList;

	#ifdef TRACK_MEM_POOL_HEADERS
		QDT_VECTOR <MEM_POOL_HEADER *>::ITERATOR It;
		It = _MemPoolList.Find(_MemPoolList.GetHead(), this);
		if (It != _MemPoolList.GetTail())
		{
			_MemPoolList.Remove(It);
		}
	#endif
}

//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL_HEADER::Allocate()
{
	SPOOL *pPool = _pFirstPool;

	if (_nNbrFreeItems == 0)
	{
		AddMemPool();
	}

	QDT_ASSERT(_nNbrFreeItems > 0);
	
	--_nNbrFreeItems;
	++_nNbrUsedItems;
	QDT_ASSERT(_nNbrUsedItems <= _nNbrAllocatedFreeItems);

#ifdef TRACK_MEM_POOL_HEADERS
	if (_nNbrUsedItems > _nMaxNumberAllocated)
	{
		_nMaxNumberAllocated = _nNbrUsedItems;
	}
#endif

	_nMemoryFree -= (pPool->_pPool->GetSize() + sizeof(void*));
	return (_pFreeList[_nNbrFreeItems]);
}

//-----------------------------------------------------------------------------
//	Name:		Deallocate
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_HEADER::Deallocate(const void * pBuffer)
{
	if (_nNbrFreeItems < _nNbrAllocatedFreeItems)
	{
		_pFreeList[_nNbrFreeItems] = const_cast<void *>(pBuffer);
		++_nNbrFreeItems;
		QDT_ASSERT(_nNbrFreeItems <= _nNbrAllocatedFreeItems);
		QDT_ASSERT(_nNbrUsedItems > 0);
		--_nNbrUsedItems;
		_nMemoryFree += (_pFirstPool->_pPool->GetSize() + sizeof(void*));
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-01-07:	ONA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetSize() const
{
	return	(_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetNextCapacity
//	Object:		
//	02-01-07:	ONA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetNextCapacity() const
{
	return	(_nNextCapacity);
}

//-----------------------------------------------------------------------------
//	Name:		AddMemPool
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL_HEADER::AddMemPool()
{
	unsigned int i;

	if (_pFirstPool == NULL)
	{
		_pFirstPool = new SPOOL;
		_pFirstPool->_pPool = new MEM_POOL(_nSize, _nFirstCapacity);
		_nNbrFreeItems += _nFirstCapacity;
		_nNbrAllocatedFreeItems += _nFirstCapacity;
		QDT_ASSERT(_pFreeList == NULL);
		_pFreeList = new void * [_nNbrAllocatedFreeItems];
		_pFirstPool->_pNext = NULL;

		for (i = 0 ; i < _nNbrFreeItems ; ++i)
		{
			_pFreeList[i] = _pFirstPool->_pPool->GetBuffer(i);
		}

		_nFreeListMemory += _nFirstCapacity * sizeof(void*);
		_nMemoryFree += _nFirstCapacity * (GetSize() + sizeof(void*));
	}
	else
	{
		// We need to reallocate, if nextCapacity is -1, we should'nt do it !! so let
		// everyone know.
		if	(_nNextCapacity == -1)
		{
			QDT_Error("MEM_POOL_HEADER::AddMemPool(): Mem pool should not reallocate but it does !! ");
		}

		SPOOL * pPool = _pFirstPool;
		while (pPool->_pNext)
		{
			pPool = pPool->_pNext;
		}

		SPOOL * pNewPool = new SPOOL;
		pNewPool->_pPool = new MEM_POOL(_nSize, _nNextCapacity);
		QDT_ASSERT(_nNbrFreeItems == 0);
		_nNbrFreeItems += _nNextCapacity;
		_nNbrAllocatedFreeItems += _nNextCapacity;
		delete [] _pFreeList;
		_pFreeList = new void * [_nNbrAllocatedFreeItems];
		pNewPool->_pNext = NULL;

		for (i = 0 ; i < _nNbrFreeItems; ++i)
		{
			_pFreeList[i] = pNewPool->_pPool->GetBuffer(i);
		}

		_nFreeListMemory += _nNextCapacity * sizeof(void*);
		_nMemoryFree += _nNextCapacity * (GetSize() + sizeof(void*));
		
		pPool->_pNext = pNewPool;

#if defined(PLATFORM_PS2) && defined(_MASTER) && !defined(_MASTER_FINAL_BUILD)
		OS::GetCallStack(0);
		QDT_STRING S;
		S.Format("%x AddMPool Size:%d Tot:%d Alloc:%d Free:%d\n", OS::GetCallStack(3), _nSize, _nNbrAllocatedFreeItems, _nNbrAllocatedFreeItems - _nNbrFreeItems, _nNbrFreeItems );
		RENDER_MANAGER::Instance()->GetRenderContext()->AddDebugString(S, 5000.f);
#endif

#if (defined _MASTER) && (!defined _MASTER_FINAL_BUILD)
		printf("MEM_POOL_HEADER::AddMemPool Size:%d Total: %d Allocated: %d Free: %d\n", _nSize, _nNbrAllocatedFreeItems, _nNbrAllocatedFreeItems - _nNbrFreeItems, _nNbrFreeItems );
#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrFree
//	Object:		
//	02-01-08:	ONA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetNbrFree() const
{
	return	(_nNbrFreeItems);
}

//-----------------------------------------------------------------------------
//	Name:		GetFreeMem
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetFreeMem() const
{
	return	(_nNbrFreeItems * GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTotal
//	Object:		
//	02-01-08:	ONA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetNbrTotal() const
{
	SPOOL *pPool = _pFirstPool;
	int nTotal = 0;

	while (pPool)
	{
		nTotal += pPool->_pPool->GetCapacity();
		pPool = pPool->_pNext;
	}

	return	(nTotal);
}

//-----------------------------------------------------------------------------
//	Name:		GetMemoryUsed
//	Object:		
//	04-04-21:	VMA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetMemoryUsed()
{
	return (MEM_POOL::GetMemoryUsed() + _nFreeListMemory);
}

//-----------------------------------------------------------------------------
//	Name:		GetMemoryFree
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
int	MEM_POOL_HEADER::GetMemoryFree()
{
	return	(_nMemoryFree);
}

//-----------------------------------------------------------------------------
//	Name:		VerifyAddress
//	Object:		if pBuffer address is not within mempoolheader address space, return false
//	04-09-21:	DAC - Created
//-----------------------------------------------------------------------------
bool	MEM_POOL_HEADER::VerifyAddress(const void * pBuffer)
{
	SPOOL *pPool = _pFirstPool;

	while (pPool)
	{
		char * pStartAddress = (char*)pPool->_pPool->GetBuffer();
		char * pEndAddress = pStartAddress + ((pPool->_pPool->GetCapacity() - 1) * pPool->_pPool->GetSize());

		if (pStartAddress <= pBuffer && pBuffer <= pEndAddress)
		{
			return(true);
		}
		pPool = pPool->_pNext;
	}

	// if you end here, you are calling a destruction on an address that dont belong to this mempool_header, so if
	// address is marked as free, it might later scratch some random memory address when you write to this address.
	// exemple: QDT_DELETE_PLACED(pAddress, TYPE) where pAddress dont belong to the pool
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Print
//	Object:		
//	04-12-03:	RMA - Created
//-----------------------------------------------------------------------------
void	 MEM_POOL_HEADER::Print(IO_STREAM * pStream)
{
	QDT_ASSERT(pStream);

	int nCapacity		= -1;
	SPOOL * pSPool = _pFirstPool;

	char str[64];
	sprintf(str, "Total: %d Allocated: %d Free: %d\n", _nNbrAllocatedFreeItems, _nNbrAllocatedFreeItems - _nNbrFreeItems, _nNbrFreeItems );
	QDT_ASSERT(strlen(str) < 64);
	pStream->PutString(str);

	while ( pSPool )
	{
		nCapacity = pSPool->_pPool->GetCapacity();
		for( int j = 0 ; j < nCapacity ; j++ )
		{
			void * pBuffer = pSPool->_pPool->GetBuffer(j);

			bool b = false;
			for( UInt32 k = 0 ; k < _nNbrFreeItems ; k++ )
			{
				if ( pBuffer == _pFreeList[k] )
				{
					b = true;
					break;
				}
			}

			if ( b )
			{
				continue;
			}

			pStream->PutString((char*)pBuffer);
		}


		pSPool = pSPool->_pNext;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PrintMemoryTracking
//	Object:		
//	04-12-06:	RMA - Created
//-----------------------------------------------------------------------------
#ifdef TRACK_MEM_POOL_HEADERS
void	MEM_POOL_HEADER::PrintMemoryTracking()
{
	QDT_VECTOR<MEM_POOL_HEADER*>::ITERATOR it = _MemPoolList.GetHead();
	QDT_VECTOR<MEM_POOL_HEADER*>::ITERATOR itTail = _MemPoolList.GetTail();

	while (it != itTail)
	{
		(*it)->PrintIndividualMemoryTracking();
		++it;
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		PrintIndividualMemoryTracking
//	Object:		
//	04-12-03:	RMA - Created
//-----------------------------------------------------------------------------
#ifdef TRACK_MEM_POOL_HEADERS
void	MEM_POOL_HEADER::PrintIndividualMemoryTracking()
{
	printf("MEM_POOL_HEADER File : %s, line : %d, count : %d\n",
				_tcFile,
				_nLine,
				_nMaxNumberAllocated);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Clone
//	Object:		
//	05-01-05:	SBE - Created
//-----------------------------------------------------------------------------
MEM_POOL_HEADER *	MEM_POOL_HEADER::Clone() const
{
#ifdef TRACK_MEM_POOL_HEADERS
	MEM_POOL_HEADER * pMPH = new MEM_POOL_HEADER(_nSize, __FILE__, __LINE__, _nFirstCapacity, _nNextCapacity);
#else
	MEM_POOL_HEADER * pMPH = new MEM_POOL_HEADER(_nSize, _nFirstCapacity, _nNextCapacity);
#endif

	//pMPH->_pFirstPool = new SPOOL;

	SPOOL * pSPool = _pFirstPool;
	SPOOL * pDstSPool = pMPH->_pFirstPool;	

	QDT_MEMCOPY(pDstSPool->_pPool->_pBuffer, pSPool->_pPool->_pBuffer, pSPool->_pPool->GetSize() * pSPool->_pPool->GetCapacity());
	pDstSPool->_pNext = new SPOOL;
	pSPool = pSPool->_pNext;		
	pDstSPool = pDstSPool->_pNext;

	// Verif mem pool header unique (le clone fonctionne mais la recherche ensuite pour trouver 
	// les leaks s'en trouvent tres complexifiee autant s'arranger pour n'avoir qu'un seul pool)
	if (pSPool != NULL)		printf("Erreur : le MEM_POOL_HEADER ne doit pas avoir deborde");

/*	while ( pSPool )
	{
		pDstSPool->_pPool = new MEM_POOL(pSPool->_pPool->GetSize(), pSPool->_pPool->GetCapacity());
		pDstSPool->_pNext = new SPOOL;

		QDT_MEMCOPY(pDstSPool->_pPool->_pBuffer, pSPool->_pPool->_pBuffer, pSPool->_pPool->GetSize() * pSPool->_pPool->GetCapacity());

		pSPool = pSPool->_pNext;		
		pDstSPool = pDstSPool->_pNext;
	}*/

	delete pDstSPool;

	return pMPH;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
