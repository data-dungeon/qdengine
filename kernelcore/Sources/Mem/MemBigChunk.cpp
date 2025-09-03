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
//
//	06-02-08:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#define QDT_MEM_INTERNAL

#include	"Root.h"
#include	INCL_KCORE(Mem/MemBigManager)
#include	INCL_KCORE(Mem/MemBigChunk)

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
#include INCL_KCORE(OS/OS)
#endif

#ifdef QDT_MEM_DEBUG
#include INCL_KCORE(TIME/TIME_ABC)
#endif

#ifdef QDT_MEM_DEBUG
#include <stdio.h>
#endif

#undef new

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifdef DEBUG_BIGMEM
//-----------------------------------------------------------------------------
//	Name:		MarkBlockAsFree
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::MarkBlockAsFree(MEM_BIG_CHUNK_ELEMENT* pElement)
{
	UInt32 i;
	unsigned char* dataPtr = ((unsigned char*)pElement)+sizeof(MEM_BIG_CHUNK_ELEMENT);

	for(i=0;i<pElement->_blockSize;i++)
	{
		dataPtr[i] = 0xFF;
	}
}

//-----------------------------------------------------------------------------
//	Name:		MarkBlockAsFree
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::MarkBlockAsAllocated(MEM_BIG_CHUNK_ELEMENT* pElement)
{
	UInt32 i;
	unsigned char* dataPtr = ((unsigned char*)pElement)+sizeof(MEM_BIG_CHUNK_ELEMENT);

	for(i=0;i<pElement->_blockSize;i++)
	{
		dataPtr[i] = 0xAA;
	}
}
#endif

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
//-----------------------------------------------------------------------------
//	Name:		SetupBlockCorruptionMarker
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void MEM_BIG_CHUNK::SetupBlockCorruptionMarker(MEM_BIG_CHUNK_ELEMENT* pElement)
{
#ifdef PLATFORM_PC
	{
		// opti
		UInt32* source = (UInt32*)_cCorruptionMarkerString;
		UInt32* dest = (UInt32*)pElement->_corruptionMarker;

		dest[0] = source[0];
		dest[1] = source[1];
		dest[2] = source[2];
		dest[3] = source[3];
	}
#else
	UInt32 i;
	for(i=0;i<_iBigCorruptionMarkerSize;i++)
	{
		pElement->_corruptionMarker[i] = _cCorruptionMarkerString[i];
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CheckBlockCorruptionMarker
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void MEM_BIG_CHUNK::CheckBlockCorruptionMarker(MEM_BIG_CHUNK_ELEMENT* pElement)
{
	UInt32 i;

	for(i=0;i<_iBigCorruptionMarkerSize;i++)
	{
		QDT_ASSERT(pElement->_corruptionMarker[i] == _cCorruptionMarkerString[i]);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CreateNewBigChunk
//	Object:		
//	06-06-23:	VHA - Created
//-----------------------------------------------------------------------------
MEM_BIG_CHUNK *	MEM_BIG_CHUNK::CreateNewBigChunk(UInt32	uSize)
{
	MEM_BIG_CHUNK* pBigChunk = (MEM_BIG_CHUNK*)malloc(uSize+sizeof(MEM_BIG_CHUNK)+sizeof(MEM_BIG_CHUNK_ELEMENT));

	QDT_ASSERT(pBigChunk);

	pBigChunk->Init(uSize+sizeof(MEM_BIG_CHUNK_ELEMENT));

	return pBigChunk; 
}

//-----------------------------------------------------------------------------
//	Name:		MEM_BIG_CHUNK constructor
//	Object:		
//	06-02-08:	VHA - Created
//-----------------------------------------------------------------------------
void MEM_BIG_CHUNK::Init(UInt32 uSize)
{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	QDT_ASSERT(_iBigCorruptionMarkerSize == 0x10); // si ce n'est pas le cas, il faut mettre a jour le Mark & Check
#endif

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	_pNextChunk = NULL;
#endif
	_uFullSize = uSize;
	_uAllocatedSize = 0;
	_pLastFreeElement = _pFirstFreeElement = (MEM_BIG_CHUNK_ELEMENT*)GetRawChunkPtr();

	_pFirstFreeElement->_blockSize = uSize - sizeof(MEM_BIG_CHUNK_ELEMENT);
	_pFirstFreeElement->_isFree = true;
	_pFirstFreeElement->_pNextFreeBlock = NULL;
	_pFirstFreeElement->_pPreviousBlock = NULL;

	// Ensure that the allignement is ok at creation.
	QDT_ASSERT((((int)GetRawChunkPtr()) % _iBigChunkAllocationAllignement) == 0);

#ifdef DEBUG_BIGMEM
	MarkBlockAsFree(_pFirstFreeElement);
#endif

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	SetupBlockCorruptionMarker(_pFirstFreeElement);
//	QDT_NOT_IMPLEMENTED();
/*	{
		UInt32 i;

		for(i=0;i<_iBigCorruptionMarkerSize;i++)
		{
			_corruptionMarker[i] = _cCorruptionMarkerString[i];
		}
	} */
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SplitElementAtEnd
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::SplitElementNoNewFree(	MEM_BIG_CHUNK_ELEMENT *	pElement,
												MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement)
{
	// not enough space to create the following free block
	// allocate the whole block, and relink the free blocks list
	pElement->_isFree = false;

	if(_pLastFreeElement == pElement)
	{
		_pLastFreeElement = pPreviousFreeElement;
	}

	// update previous free linkage
	if(_pFirstFreeElement == pElement)
	{
		_pFirstFreeElement = pElement->_pNextFreeBlock;
	}
	else
	{
		pPreviousFreeElement->_pNextFreeBlock = pElement->_pNextFreeBlock;
	}

	_uAllocatedSize += pElement->_blockSize;

#ifdef DEBUG_BIGMEM
	MarkBlockAsAllocated(pElement);
#endif
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	{
		SetupBlockCorruptionMarker(pElement);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SplitElement
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::SplitElement(MEM_BIG_CHUNK_ELEMENT *	pElement,
									UInt32 uSize,
									MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement)
{
	QDT_ASSERT(pElement->_blockSize >= uSize);

	// do we have enough space to allocate a new free block at the end ?
	if(pElement->_blockSize >= sizeof(MEM_BIG_CHUNK_ELEMENT) + uSize)
	{
		MEM_BIG_CHUNK_ELEMENT* pNewFreeBlock;
		
		pNewFreeBlock = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement)+sizeof(MEM_BIG_CHUNK_ELEMENT)+uSize);

		pNewFreeBlock->_blockSize = pElement->_blockSize-uSize-sizeof(MEM_BIG_CHUNK_ELEMENT);
		pNewFreeBlock->_isFree = true;
		pNewFreeBlock->_pNextFreeBlock = pElement->_pNextFreeBlock;
		pNewFreeBlock->_pPreviousBlock = pElement;

		pElement->_isFree = false;
		pElement->_blockSize = uSize;

		// update wackward list
		MEM_BIG_CHUNK_ELEMENT* pNextBlock = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pNewFreeBlock)+pNewFreeBlock->_blockSize+sizeof(MEM_BIG_CHUNK_ELEMENT));
		if(pNextBlock < (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
		{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
			CheckBlockCorruptionMarker(pNextBlock);
#endif
			pNextBlock->_pPreviousBlock = pNewFreeBlock;
		}

		// update previous free linkage
		if(_pFirstFreeElement == pElement)
		{
			_pFirstFreeElement = pNewFreeBlock;
		}
		else
		{
			pPreviousFreeElement->_pNextFreeBlock = pNewFreeBlock;
		}

		// update last free
		if(_pLastFreeElement == pElement)
		{
			_pLastFreeElement = pNewFreeBlock;
		}

		_uAllocatedSize += uSize;

#ifdef DEBUG_BIGMEM
		{
			MarkBlockAsFree(pNewFreeBlock);
			MarkBlockAsAllocated(pElement);
		}
#endif
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
		{
			SetupBlockCorruptionMarker(pNewFreeBlock);
			SetupBlockCorruptionMarker(pElement);
		}
#endif
	}
	else
	{
#ifdef QDT_MEM_DEBUG
		_uWastedMemory = pElement->_blockSize - uSize;
#endif
		SplitElementNoNewFree(pElement, pPreviousFreeElement);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SplitElementAtEnd
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
MEM_BIG_CHUNK_ELEMENT*	MEM_BIG_CHUNK::SplitElementAtEnd(	MEM_BIG_CHUNK_ELEMENT *	pElement,
															UInt32 uSize,
															MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement)
{
	QDT_ASSERT(pElement->_blockSize >= uSize);

	// do we have enough space to allocate a new free block at the end ?
	if(pElement->_blockSize >= sizeof(MEM_BIG_CHUNK_ELEMENT) + uSize)
	{
		MEM_BIG_CHUNK_ELEMENT* pNewFreeBlock;
		
		pNewFreeBlock = pElement;
		pNewFreeBlock->_blockSize -= uSize+sizeof(MEM_BIG_CHUNK_ELEMENT);

		pElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pNewFreeBlock)+sizeof(MEM_BIG_CHUNK_ELEMENT)+pNewFreeBlock->_blockSize);
		pElement->_blockSize = uSize;
		pElement->_isFree = false;
		pElement->_pPreviousBlock = pNewFreeBlock;


		// update wackward list
		MEM_BIG_CHUNK_ELEMENT* pNextBlock = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement)+pElement->_blockSize+sizeof(MEM_BIG_CHUNK_ELEMENT));
		if(pNextBlock < (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
		{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
			CheckBlockCorruptionMarker(pNextBlock);
#endif
			pNextBlock->_pPreviousBlock = pElement;
		}

		// free link didn't change in that case

		// last free didn't change either

		_uAllocatedSize += uSize;

#ifdef DEBUG_BIGMEM
		{
			MarkBlockAsFree(pNewFreeBlock);
			MarkBlockAsAllocated(pElement);
		}
#endif
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
		{
			SetupBlockCorruptionMarker(pNewFreeBlock);
			SetupBlockCorruptionMarker(pElement);
		}
#endif
		return pElement;
	}
	else
	{
		SplitElementNoNewFree(pElement, pPreviousFreeElement);
		return pElement;
	}
}


//-----------------------------------------------------------------------------
//	Name:		Allocate
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_BIG_CHUNK::Allocate(	MEM_ALLOCATION_REQUEST* pRequest)
{
#ifdef QDT_MEM_DEBUG
	_uWastedMemory = 0;
#endif

	UInt32 uSize = pRequest->_uSize;

	// enough space in current chunk ?
	MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement = NULL;
	MEM_BIG_CHUNK_ELEMENT* pCurrentChunkElement = _pFirstFreeElement;

	while(pCurrentChunkElement)
	{
		QDT_ASSERT(pCurrentChunkElement->_isFree == true);

		if(pCurrentChunkElement->_blockSize >= uSize)
		{
			// enough space, allocate this element
			SplitElement(pCurrentChunkElement, uSize, pPreviousFreeElement);
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
			SetupExtraDebugInfo(pCurrentChunkElement, pRequest->_uLibId, pRequest->_uDepth+1);
#endif

			void* pReturnPtr = ((char*)pCurrentChunkElement)+sizeof(MEM_BIG_CHUNK_ELEMENT);

			QDT_ASSERT((((int)pReturnPtr)%_iBigChunkAllocationAllignement) == 0);

			return pReturnPtr;
		}

		pPreviousFreeElement = pCurrentChunkElement;
		pCurrentChunkElement = pCurrentChunkElement->_pNextFreeBlock;
	}

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	// if we don't have a next chunk, create it
	if(_pNextChunk == NULL)
	{
		if(uSize > QDT_BIGMEM_DEFAULT_SIZE)
		{
			_pNextChunk = CreateNewBigChunk(uSize);
		}
		else
		{
			_pNextChunk = CreateNewBigChunk();
		}
	}

	QDT_ASSERT(_pNextChunk);

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	return _pNextChunk->Allocate(pRequest);
#else
	QDT_FAIL();
	return NULL;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AllocateAtEnd
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_BIG_CHUNK::AllocateAtEnd(	MEM_ALLOCATION_REQUEST* pRequest)
{
#ifdef QDT_MEM_DEBUG
	_uWastedMemory = 0;
#endif

	UInt32 uSize = pRequest->_uSize;

	// enough space in current chunk ?
	MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement;
	MEM_BIG_CHUNK_ELEMENT* pCurrentChunkElement = _pLastFreeElement;

	while(pCurrentChunkElement)
	{
		// search the previous free element
		pPreviousFreeElement = pCurrentChunkElement->_pPreviousBlock;

		while(pPreviousFreeElement && (pPreviousFreeElement->_isFree == false))
		{
			pPreviousFreeElement = pPreviousFreeElement->_pPreviousBlock;
		}

		if(pCurrentChunkElement->_isFree)
		{
			if(pCurrentChunkElement->_blockSize >= uSize)
			{
				// enough space, allocate this element
				MEM_BIG_CHUNK_ELEMENT* pAllocatedElement = SplitElementAtEnd(pCurrentChunkElement, uSize, pPreviousFreeElement);
#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
				SetupExtraDebugInfo(pAllocatedElement, pRequest->_uLibId, pRequest->_uDepth+1);
#endif
				void* pReturnPtr = ((char*)pCurrentChunkElement)+sizeof(MEM_BIG_CHUNK_ELEMENT);

				QDT_ASSERT((((int)pReturnPtr)%_iBigChunkAllocationAllignement) == 0);

				return pReturnPtr;
			}
		}

		pCurrentChunkElement = pPreviousFreeElement;
	}

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	// if we don't have a next chunk, create it
	if(_pNextChunk == NULL)
	{
		_pNextChunk = CreateNewBigChunk();
	}

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	return _pNextChunk->AllocateAtEnd(pRequest);

#else
	QDT_FAIL();
	return NULL;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AllocateAlign
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void *	MEM_BIG_CHUNK::AllocateAlign(	MEM_ALLOCATION_REQUEST* pRequest)
{
#ifdef QDT_MEM_DEBUG
	_uWastedMemory = 0;
#endif
	UInt32 uSize = pRequest->_uSize;
	UInt32 uBoundary = pRequest->_uBoundary;

	// enough space in current chunk ?
	MEM_BIG_CHUNK_ELEMENT* pPreviousFreeElement = NULL;
	MEM_BIG_CHUNK_ELEMENT* pCurrentChunkElement = _pFirstFreeElement;

	while(pCurrentChunkElement)
	{
		QDT_ASSERT(pCurrentChunkElement->_isFree == true);

		UInt32 padSize;

		padSize = uBoundary - (((UInt32)(((IntPtr)pCurrentChunkElement)+sizeof(MEM_BIG_CHUNK_ELEMENT))) % uBoundary);

		while(padSize<sizeof(MEM_BIG_CHUNK_ELEMENT))
		{
			padSize += uBoundary;
		}

		QDT_ASSERT((((((int)pCurrentChunkElement)+sizeof(MEM_BIG_CHUNK_ELEMENT))+padSize) %  uBoundary) == 0);

		if(pCurrentChunkElement->_blockSize >= uSize + padSize)
		{
			if(padSize >= sizeof(MEM_BIG_CHUNK_ELEMENT))
			{
				// enough space to create a free block
				SplitElement(pCurrentChunkElement, padSize - sizeof(MEM_BIG_CHUNK_ELEMENT), pPreviousFreeElement);
				pCurrentChunkElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pCurrentChunkElement)+pCurrentChunkElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT));

				// create the alligned block
				QDT_ASSERT(pCurrentChunkElement->_isFree);
				QDT_ASSERT(pCurrentChunkElement->_blockSize >= uSize);
				SplitElement(pCurrentChunkElement, uSize, pPreviousFreeElement);

				// release the temporary block
				Free(((char*)pCurrentChunkElement->_pPreviousBlock)+sizeof(MEM_BIG_CHUNK_ELEMENT));

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
				SetupExtraDebugInfo(pCurrentChunkElement, pRequest->_uLibId, pRequest->_uDepth+1);
#endif
				void* pReturnPtr = ((char*)pCurrentChunkElement)+sizeof(MEM_BIG_CHUNK_ELEMENT);

				QDT_ASSERT((((int)pReturnPtr)%_iBigChunkAllocationAllignement) == 0);
				QDT_ASSERT((((int)pReturnPtr)%uBoundary) == 0);

				return pReturnPtr;
			}
/*			else
			{
			} */
/*			if(pCurrentChunkElement->_pPreviousBlock)
			{
				// A previous block exists, extend it
			}
			else
			{
				// There is no previous block
				QDT_NOT_IMPLEMENTED();
			}*/
		}

		pPreviousFreeElement = pCurrentChunkElement;
		pCurrentChunkElement = pCurrentChunkElement->_pNextFreeBlock;
	}

	if(_uAllocatedSize == 0)
	{
		QDT_FAIL();
	}

#ifdef QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS
	// if we don't have a next chunk, create it
	if(_pNextChunk == NULL)
	{
		_pNextChunk = CreateNewBigChunk();
	}

#ifdef QDT_MEM_DEBUG
	pRequest->_uDepth++;
#endif

	return _pNextChunk->AllocateAlign(pRequest);

#else
	QDT_FAIL();
	return NULL;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		FreeAndMergeBothSides
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::FreeAndMergeBothSides(	MEM_BIG_CHUNK_ELEMENT* pPreviousElement,
												MEM_BIG_CHUNK_ELEMENT* pElement,
												MEM_BIG_CHUNK_ELEMENT* pNextElement)
{
	QDT_ASSERT(pElement->_isFree == false);
	QDT_ASSERT(pPreviousElement->_isFree == true);
	QDT_ASSERT(pNextElement->_isFree == true);
	QDT_ASSERT(pNextElement->_pPreviousBlock == pElement);
	QDT_ASSERT(pElement->_pPreviousBlock == pPreviousElement);
	QDT_ASSERT(pPreviousElement->_pNextFreeBlock == pNextElement);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	CheckBlockCorruptionMarker(pElement);
	CheckBlockCorruptionMarker(pPreviousElement);
	CheckBlockCorruptionMarker(pNextElement);
#endif

	// update size
	pPreviousElement->_blockSize += pElement->_blockSize + pNextElement->_blockSize + 2*sizeof(MEM_BIG_CHUNK_ELEMENT);

	// update free linked list
	pPreviousElement->_pNextFreeBlock = pNextElement->_pNextFreeBlock;

	if(_pFirstFreeElement == NULL)
	{
		_pFirstFreeElement = pPreviousElement;
	}

	// update backward list
	MEM_BIG_CHUNK_ELEMENT* pNextNextElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pNextElement) + pNextElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT));
	if(pNextNextElement!=(MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
	{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
		CheckBlockCorruptionMarker(pNextNextElement);
#endif
		QDT_ASSERT(pNextNextElement->_pPreviousBlock == pNextElement);
		pNextNextElement->_pPreviousBlock = pPreviousElement;
	}

	// update last free
	// NOTA: again, easy case, we only need to update if pNextElement was the last free

	if(_pLastFreeElement == NULL)
	{
		_pLastFreeElement = pPreviousElement;
		_pLastFreeElement->_pNextFreeBlock = NULL;
	}
	else
	{
		if(_pLastFreeElement == pNextElement)
		{
			_pLastFreeElement = pPreviousElement;
			_pLastFreeElement->_pNextFreeBlock = NULL;
		}
	}

#ifdef DEBUG_BIGMEM
	MarkBlockAsFree(pPreviousElement);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		FreeAndMergeWithPrevious
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::FreeAndMergeWithPrevious(MEM_BIG_CHUNK_ELEMENT* pPreviousBlock,
												MEM_BIG_CHUNK_ELEMENT* pElement)
{
	QDT_ASSERT(pElement->_isFree == false);
	QDT_ASSERT(pPreviousBlock->_isFree == true);
	QDT_ASSERT(pElement->_pPreviousBlock == pPreviousBlock);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	CheckBlockCorruptionMarker(pElement);
	CheckBlockCorruptionMarker(pPreviousBlock);
#endif

	// update size
	pPreviousBlock->_blockSize += pElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT);

	// update backward list
	MEM_BIG_CHUNK_ELEMENT* pNextElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement) + pElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT));
	if(pNextElement!=(MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
	{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
		CheckBlockCorruptionMarker(pNextElement);
#endif
		QDT_ASSERT(pNextElement->_pPreviousBlock == pElement);
		pNextElement->_pPreviousBlock = pPreviousBlock;
	}

	if(_pLastFreeElement == NULL)
	{
		_pLastFreeElement = pElement;
		pElement->_pNextFreeBlock = NULL;
	}

	if(_pFirstFreeElement == NULL)
	{
		_pFirstFreeElement = pElement;
	}

#ifdef DEBUG_BIGMEM
	MarkBlockAsFree(pPreviousBlock);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		FreeAndMergeWithNext
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::FreeAndMergeWithNext(MEM_BIG_CHUNK_ELEMENT* pElement,
											MEM_BIG_CHUNK_ELEMENT* pNextElement)
{
	QDT_ASSERT(pElement->_isFree == false);
	QDT_ASSERT(pNextElement->_isFree);
	QDT_ASSERT(pNextElement->_pPreviousBlock == pElement);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	CheckBlockCorruptionMarker(pElement);
	CheckBlockCorruptionMarker(pNextElement);
#endif

	// set as free
	pElement->_isFree = true;

	// update free linked list
	pElement->_pNextFreeBlock = pNextElement->_pNextFreeBlock;

	// update size
	pElement->_blockSize += pNextElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT);

	// if nextElement was the first free
	if(_pFirstFreeElement == NULL)
	{
		_pFirstFreeElement = pElement;
	}
	else
	{
		if(_pFirstFreeElement == pNextElement)
		{
			// easy case
			_pFirstFreeElement = pElement;
		}
		else
		{
			// search the free list to place the new free block as it should
			MEM_BIG_CHUNK_ELEMENT* pCurrentElementSearch = _pFirstFreeElement;

			while(pCurrentElementSearch)
			{
				QDT_ASSERT(pCurrentElementSearch->_isFree);

				if(pCurrentElementSearch->_pNextFreeBlock == pNextElement)
				{
					pCurrentElementSearch->_pNextFreeBlock = pElement;
					break;
				}

				QDT_ASSERT(pCurrentElementSearch->_pNextFreeBlock < pNextElement);

				pCurrentElementSearch = pCurrentElementSearch->_pNextFreeBlock;
			}
			QDT_ASSERT(pCurrentElementSearch); // if this assert, the free list was somehow corrupted

			pCurrentElementSearch->_pNextFreeBlock = pElement;
		}
	}

	// update backward list
	MEM_BIG_CHUNK_ELEMENT* pNextNextElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pNextElement) + pNextElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT));
	if(pNextNextElement!=(MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
	{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
		CheckBlockCorruptionMarker(pNextNextElement);
#endif
		QDT_ASSERT(pNextNextElement->_pPreviousBlock == pNextElement);
		pNextNextElement->_pPreviousBlock = pElement;
	}

	// update last free
	if(_pLastFreeElement)
	{
		if(_pLastFreeElement == pNextElement)
		{
			_pLastFreeElement = pElement;
			pElement->_pNextFreeBlock = NULL;
		}
		else
		{
			if(_pLastFreeElement < pElement)
			{
				_pLastFreeElement = pElement;
				pElement->_pNextFreeBlock = NULL;
			}
		}
	}
	else
	{
		_pLastFreeElement = pElement;
		pElement->_pNextFreeBlock = NULL;
	}

#ifdef DEBUG_BIGMEM
	MarkBlockAsFree(pElement);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		FreeNoMerge
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::FreeNoMerge(	MEM_BIG_CHUNK_ELEMENT *	pElement)
{
	QDT_ASSERT(pElement->_isFree == false);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	CheckBlockCorruptionMarker(pElement);
#endif

	// set as free
	pElement->_isFree = true;

#ifdef DEBUG_BIGMEM
	MarkBlockAsFree(pElement);
#endif

	if(_pFirstFreeElement == NULL)
	{
		QDT_ASSERT(_pLastFreeElement == NULL);

		_pFirstFreeElement = pElement;
		_pLastFreeElement = pElement;
		pElement->_pNextFreeBlock = NULL;
	}
	else
	// update the first free
	if(_pFirstFreeElement > pElement)
	{
		// easy case
		pElement->_pNextFreeBlock = _pFirstFreeElement;
		_pFirstFreeElement = pElement;
	}
	else
	// update last free
	if(_pLastFreeElement < pElement)
	{
		_pLastFreeElement->_pNextFreeBlock = pElement;
		_pLastFreeElement = pElement;
		pElement->_pNextFreeBlock = NULL;
	}
	else
	{
		// search the free list to place the new free block as it should
		MEM_BIG_CHUNK_ELEMENT* pCurrentElementSearch = _pFirstFreeElement;

		while(pCurrentElementSearch)
		{
			QDT_ASSERT(pCurrentElementSearch->_isFree);

			if(pCurrentElementSearch->_pNextFreeBlock > pElement)
			{
				pElement->_pNextFreeBlock = pCurrentElementSearch->_pNextFreeBlock;
				pCurrentElementSearch->_pNextFreeBlock = pElement;
				break;
			}

			pCurrentElementSearch = pCurrentElementSearch->_pNextFreeBlock;
		}
		QDT_ASSERT(pCurrentElementSearch); // if this assert, the free list was somehow corrupted

		pCurrentElementSearch->_pNextFreeBlock = pElement;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::Free(const void *	pPtr)
{
	QDT_ASSERT(pPtr);

#ifdef QDT_MEM_DEBUG
	_uWastedMemory = 0;
#endif

	// First, compute the supposed chunkElement pointer
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));

	//safety check
	QDT_ASSERT((((char*)pElement) >= GetRawChunkPtr()) && (((char*)pElement) < GetRawEndChunkPtr()));

	// check it's a used element (double free protection)
	QDT_ASSERT(pElement->_isFree == false);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	CheckBlockCorruptionMarker(pElement);
#endif

	MEM_BIG_CHUNK_ELEMENT* pNextElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement) + sizeof(MEM_BIG_CHUNK_ELEMENT) + pElement->_blockSize);

	_uAllocatedSize -= pElement->_blockSize;

	//TODO: check case if we are freeing the last possible element, in that case, there is no next element
	if(pNextElement == (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
	{
		pNextElement = NULL;
	}

	if(		(pElement->_pPreviousBlock && pElement->_pPreviousBlock->_isFree)
		&&	(pNextElement && pNextElement->_isFree))
	{
		FreeAndMergeBothSides(pElement->_pPreviousBlock,pElement,pNextElement);
	}
	else
	if(pElement->_pPreviousBlock && pElement->_pPreviousBlock->_isFree)
	{
		FreeAndMergeWithPrevious(pElement->_pPreviousBlock, pElement);
	}
	else
	if(pNextElement && pNextElement->_isFree)
	{
		FreeAndMergeWithNext(pElement, pNextElement);
	}
	else
	{
		FreeNoMerge(pElement);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Resize
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::Shrink(void* pPtr, UInt32 uSize)
{
	QDT_ASSERT(pPtr);

	// First, compute the supposed chunkElement pointer
	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)((char*)pPtr - sizeof(MEM_BIG_CHUNK_ELEMENT));

	//safety check
	QDT_ASSERT((((char*)pElement) >= GetRawChunkPtr()) && (((char*)pElement) < GetRawEndChunkPtr()));

	// check it's a used element (double free protection)
	QDT_ASSERT(pElement->_isFree == false);

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	CheckBlockCorruptionMarker(pElement);
#endif

	QDT_ASSERT(pElement->_blockSize > uSize);

	UInt32 uSizeDiff = pElement->_blockSize - uSize;

	// do we have enough space to create a new block
	if(uSizeDiff>=sizeof(MEM_BIG_CHUNK_ELEMENT))
	{
		MEM_BIG_CHUNK_ELEMENT* pNextElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement) + sizeof(MEM_BIG_CHUNK_ELEMENT) + pElement->_blockSize);
		if(pNextElement == (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
		{
			pNextElement = NULL;
		}

		pElement->_blockSize = uSize;
		MEM_BIG_CHUNK_ELEMENT* pNewFreeElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement) + sizeof(MEM_BIG_CHUNK_ELEMENT) + pElement->_blockSize);

		if(pNextElement && pNextElement->_isFree)
		{
			QDT_ASSERT(_pFirstFreeElement);
			QDT_ASSERT(_pLastFreeElement);

			// next element is also free, need to merge those

			// first find the previous free
			MEM_BIG_CHUNK_ELEMENT* pCurrentElementSearch = pElement->_pPreviousBlock;

			while(pCurrentElementSearch)
			{
				if(pCurrentElementSearch->_isFree == true)
				{
					break;
				}
				pCurrentElementSearch = pCurrentElementSearch->_pPreviousBlock;
			}

			if(pCurrentElementSearch == NULL)
			{
				// no previous free, it must be the first free
				QDT_ASSERT(_pFirstFreeElement == pNextElement);
				_pFirstFreeElement = pNewFreeElement;
			}
			else
			{
				// there is a previous free, link it
				QDT_ASSERT(pCurrentElementSearch->_pNextFreeBlock == pNextElement);
				pCurrentElementSearch->_pNextFreeBlock = pNewFreeElement;
			}

			// link the next free
			pNewFreeElement->_pNextFreeBlock = pNextElement->_pNextFreeBlock;

			// update last free
			if(_pLastFreeElement == pNextElement)
			{
				_pLastFreeElement = pNewFreeElement;
			}

			// finalize the merging
			pNewFreeElement->_blockSize = pNextElement->_blockSize + uSizeDiff;
			pNewFreeElement->_isFree = true;

			// update backward list
			MEM_BIG_CHUNK_ELEMENT* pNextNextElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pNextElement) + pNextElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT));
			if(pNextNextElement!=(MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
			{
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
				CheckBlockCorruptionMarker(pNextNextElement);
#endif
				QDT_ASSERT(pNextNextElement->_pPreviousBlock == pNextElement);
				pNextNextElement->_pPreviousBlock = pNewFreeElement;
			}
			pNewFreeElement->_pPreviousBlock = pElement;

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
			SetupBlockCorruptionMarker(pNewFreeElement);
#endif
		}
		else
		{
			// next block isn't a free block
			pNewFreeElement->_blockSize = uSizeDiff - sizeof(MEM_BIG_CHUNK_ELEMENT);
			pNewFreeElement->_isFree = true;

			// update free list
			if(_pFirstFreeElement || _pLastFreeElement)
			{
				// there is at last another free in the list
				QDT_ASSERT(_pFirstFreeElement);
				QDT_ASSERT(_pLastFreeElement);

				if(_pFirstFreeElement > pNewFreeElement)
				{
					pNewFreeElement->_pNextFreeBlock = _pFirstFreeElement;
					_pFirstFreeElement = pNewFreeElement;
				}
				else
				{
					// first find the previous free
					MEM_BIG_CHUNK_ELEMENT* pCurrentElementSearch = pElement->_pPreviousBlock;

					while(pCurrentElementSearch)
					{
						if(pCurrentElementSearch->_isFree == true)
						{
							break;
						}
						pCurrentElementSearch = pCurrentElementSearch->_pPreviousBlock;
					}

					QDT_ASSERT(pCurrentElementSearch);

					pNewFreeElement->_pNextFreeBlock = pCurrentElementSearch->_pNextFreeBlock;
					pCurrentElementSearch->_pNextFreeBlock = pNewFreeElement;
				}

				// update last free
				if(_pLastFreeElement < pNewFreeElement)
				{
					QDT_ASSERT(pNewFreeElement->_pNextFreeBlock == NULL);
					_pLastFreeElement = pNewFreeElement;
				}
			}
			else
			{
				// there is no other free in the list

				QDT_ASSERT(_pFirstFreeElement == NULL);
				QDT_ASSERT(_pLastFreeElement == NULL);

				_pFirstFreeElement = pNewFreeElement;
				_pLastFreeElement = pNewFreeElement;

				pNewFreeElement->_pNextFreeBlock = NULL;
			}

			// update backward list
			if(pNextElement)
			{
				QDT_ASSERT(pNextElement->_pPreviousBlock == pElement);
				pNextElement->_pPreviousBlock = pNewFreeElement;
			}
			pNewFreeElement->_pPreviousBlock = pElement;

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
			SetupBlockCorruptionMarker(pNewFreeElement);
#endif
		}

		_uAllocatedSize -= uSizeDiff;
	}
	else
	{
#ifdef QDT_MEM_DEBUG
		_uWastedMemory = uSizeDiff;
#endif
	}
}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		GetAllocatedSize
//	Object:		
//	06-02-09:	VHA - Created
//-----------------------------------------------------------------------------
UInt32	MEM_BIG_CHUNK::GetAllocatedSize()
{
	return _uAllocatedSize;
}

//-----------------------------------------------------------------------------
//	Name:		CheckConsistency
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::CheckConsistency()
{
	UInt32 uChunkDataUsed = 0;
	UInt32 uChunkDataFree = 0;
	UInt32 uChunkHeaderUsed = 0;
	UInt32 uChunkHeaderFree = 0;

	if(_pLastFreeElement)
	{
		QDT_ASSERT(_pLastFreeElement->_pNextFreeBlock == NULL);
		QDT_ASSERT(_pLastFreeElement->_isFree == true);
	}

	MEM_BIG_CHUNK_ELEMENT* pPreviousElement = NULL;
	MEM_BIG_CHUNK_ELEMENT* pCurrentChunkElement = (MEM_BIG_CHUNK_ELEMENT*)GetRawChunkPtr();

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	//QDT_NOT_IMPLEMENTED();
/*	{
		UInt32 i;

		for(i=0;i<_iBigCorruptionMarkerSize;i++)
		{
			QDT_ASSERT(_corruptionMarker[i] == _cCorruptionMarkerString[i]);
		}
	} */
#endif

	QDT_ASSERT(((char*)pCurrentChunkElement + pCurrentChunkElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT)) <= GetRawEndChunkPtr());

	while(pCurrentChunkElement != (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr())
	{
		// check corruption header
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
		CheckBlockCorruptionMarker(pCurrentChunkElement);
#endif

		QDT_ASSERT(((char*)pCurrentChunkElement + pCurrentChunkElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT)) <= GetRawEndChunkPtr());

		// check reverse link
		QDT_ASSERT(pPreviousElement == pCurrentChunkElement->_pPreviousBlock);

		if(pPreviousElement && pPreviousElement->_isFree)
		{
			QDT_ASSERT(pCurrentChunkElement->_isFree == false);
		}

		QDT_ASSERT(		(pCurrentChunkElement->_isFree == false)
					||	(pCurrentChunkElement->_isFree == true));

		QDT_ASSERT(pCurrentChunkElement->_blockSize >= 0);

#ifdef DEBUG_BIGMEM
		{
			UInt32 i;
			unsigned char* dataPtr = ((unsigned char*)pCurrentChunkElement)+sizeof(MEM_BIG_CHUNK_ELEMENT);

			if(pCurrentChunkElement->_isFree)
			{
				for(i=0;i<pCurrentChunkElement->_blockSize;i++)
				{
					QDT_ASSERT(dataPtr[i] == 0xFF);
				}
			}
			else
			{
				// we can't really check anything here
			}
		}
#endif

		// update data count
		if(pCurrentChunkElement->_isFree)
		{
			uChunkDataFree += pCurrentChunkElement->_blockSize;
			uChunkHeaderFree += sizeof(MEM_BIG_CHUNK_ELEMENT);
		}
		else
		{
			uChunkDataUsed += pCurrentChunkElement->_blockSize;
			uChunkHeaderUsed += sizeof(MEM_BIG_CHUNK_ELEMENT);
		}

		pPreviousElement = pCurrentChunkElement;
		pCurrentChunkElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pCurrentChunkElement)+pCurrentChunkElement->_blockSize + sizeof(MEM_BIG_CHUNK_ELEMENT));
	}

	QDT_ASSERT(uChunkDataUsed == _uAllocatedSize);

	QDT_ASSERT(uChunkDataUsed+uChunkDataFree+uChunkHeaderFree+uChunkHeaderUsed == _uFullSize);
	QDT_ASSERT(pCurrentChunkElement == (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr());

	// check free list consistency
	{
		UInt32 uFreeListCount = 0;

		pCurrentChunkElement = _pFirstFreeElement;

		while(pCurrentChunkElement)
		{
		// check corruption header
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
			CheckBlockCorruptionMarker(pCurrentChunkElement);
#endif
			if(pCurrentChunkElement->_pNextFreeBlock)
			{
				QDT_ASSERT(pCurrentChunkElement->_pNextFreeBlock > pCurrentChunkElement);
			}
			else
			{
				QDT_ASSERT(pCurrentChunkElement == _pLastFreeElement);
			}

			uFreeListCount += sizeof(MEM_BIG_CHUNK_ELEMENT);
			uFreeListCount += pCurrentChunkElement->_blockSize;

			pCurrentChunkElement = pCurrentChunkElement->_pNextFreeBlock;
		}

		QDT_ASSERT(uFreeListCount == uChunkDataFree+uChunkHeaderFree);
	}
}

#endif

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
//-----------------------------------------------------------------------------
//	Name:		SetupCallStack
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::SetupCallStack(MEM_BIG_CHUNK_ELEMENT* pElement, UInt32 uDepth)
{
	UInt32 i;

	for(i=0;i<_iMaxStackDepth;i++)
	{
		// the +4 will have to be changed if some part of the MEM lib are passed
		pElement->_pStack[i] = OS::GetCallStack(uDepth+i);
	}
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		SetupCallStack
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::SetupTimeId(MEM_BIG_CHUNK_ELEMENT* pElement, UInt32 uId)
{
	if(MEM::GetInstance()->_bIsCreatingTime)
		pElement->_fAllocationTime = 0;
	else
		pElement->_fAllocationTime = TIME::Instance()->GetSystemTime();
	pElement->_iId = uId;
}
#endif

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
//-----------------------------------------------------------------------------
//	Name:		SetupExtraDebugInfo
//	Object:		
//	06-02-14:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::SetupExtraDebugInfo(MEM_BIG_CHUNK_ELEMENT* pElement, UInt32 uId, UInt32 uDepth)
{
	QDT_ASSERT(pElement);

	//safety check
	QDT_ASSERT((((char*)pElement) >= GetRawChunkPtr()) && (((char*)pElement) < GetRawEndChunkPtr()));

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
	SetupCallStack(pElement,uDepth+1);
#endif

#ifdef QDT_MEM_DEBUG
	SetupTimeId(pElement,uId);
#endif
}
#endif

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_BIG_CHUNK::DumpUsageMap( IO_STREAM* pStream )
{
	char textBuffer[256];

	pStream->PutString("\t\t<MEM_BIG_CHUNK>");

	sprintf(textBuffer,"\t\t\t<AllocatedSize>%d</AllocatedSize>",_uAllocatedSize);
	pStream->PutString(textBuffer);

	MEM_BIG_CHUNK_ELEMENT* pElement = (MEM_BIG_CHUNK_ELEMENT*)GetRawChunkPtr();

	while(pElement && (pElement < (MEM_BIG_CHUNK_ELEMENT*)GetRawEndChunkPtr()))
	{
		if (pElement->_isFree == false)
		{
			pStream->PutString("\t\t\t<MEM_BIG_CHUNK_ELEMENT>");

			sprintf(textBuffer,"\t\t\t\t<isFree>%d</isFree>",pElement->_isFree);
			pStream->PutString(textBuffer);
			sprintf(textBuffer,"\t\t\t\t<blockSize>%d</blockSize>",pElement->_blockSize);
			pStream->PutString(textBuffer);

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
			if(!pElement->_isFree)
			{
				UInt32 i;

				for(i=0;i<_iMaxStackDepth;i++)
				{
					sprintf(textBuffer,"\t\t\t\t<callStack>0x%08X</callStack>",pElement->_pStack[i]);
					pStream->PutString(textBuffer);
				}
			}
#endif

#ifdef QDT_MEM_DEBUG
	/*	if(!pElement->_isFree)
		{
			sprintf(textBuffer,"\t\t\t\t<allocationTime>%f</allocationTime>",pElement->_fAllocationTime);
			pStream->PutString(textBuffer);
			sprintf(textBuffer,"\t\t\t\t<id>%f</id>",pElement->_iId);
			pStream->PutString(textBuffer);
		} */
#endif

			pStream->PutString("\t\t\t</MEM_BIG_CHUNK_ELEMENT>");
		}

		pElement = (MEM_BIG_CHUNK_ELEMENT*)(((char*)pElement)+pElement->_blockSize+sizeof(MEM_BIG_CHUNK_ELEMENT));
	}

	pStream->PutString("\t\t</MEM_BIG_CHUNK>");
}
#endif

#ifdef QDT_DEBUG
#include "MemBigChunk.inl"
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
