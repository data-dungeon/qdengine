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
//
//	06-02-06:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#define QDT_MEM_INTERNAL

#include	"Root.h"
#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Mem/MemPool)
#include	<stdio.h>

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
//-----------------------------------------------------------------------------
//	Name:		SetupCorruptionMarker
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOLNEW::SetupCorruptionMarker(const UInt32	uElementSize)
{
	// this is done only once at mempool init, so we can leave it enabled for flexibility
	UInt32 i;

	char* pCurrentElement = _pPool;

	while(pCurrentElement)
	{
		char* pMarker = pCurrentElement + uElementSize;

		for(i=0;i<_iPoolCorruptionMarkerSize;i++)
		{
			pMarker[i] = _cPoolCorruptionMarkerString[i];
		}

		pCurrentElement = *(char**)pCurrentElement;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CheckCorruptionMarker
//	Object:		
//	06-02-10:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOLNEW::CheckCorruptionMarker(const UInt32	uElementSize)
{
	if(!MEM::GetInstance()->_debugConfig._bUseCorruptionProtection)
		return;

	UInt32 i;
	UInt32 j;

	char* pCurrentElement = _pPool;

	for(j=0;j<_NumElementInPool;j++)
	{
		char* pMarker = pCurrentElement + uElementSize;

		for(i=0;i<_iPoolCorruptionMarkerSize;i++)
		{
			QDT_ASSERT(pMarker[i] == _cPoolCorruptionMarkerString[i]);
		}

		pCurrentElement += uElementSize + _iPoolCorruptionMarkerSize + _iPoolExtraDebugSize;
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		InitializePool
//	Object:		
//	06-02-06:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOLNEW::InitializePool(const UInt32	uElementSize)
{
	QDT_ASSERT(uElementSize > 0);

	_NumFreeElementInPool = _NumElementInPool = _iTotalPoolSize / (uElementSize+_iPoolCorruptionMarkerSize+_iPoolExtraDebugSize);

	Char* pCurrentPool = _pPool;

	_uElementSize = uElementSize;

	UInt16 i;
	for(i=0;i<_NumElementInPool-1;i++)
	{
		QDT_ASSERT((((int)pCurrentPool)%_iBigChunkAllocationAllignement) == 0);
		*(Char**)pCurrentPool = pCurrentPool + uElementSize + _iPoolCorruptionMarkerSize + _iPoolExtraDebugSize;
		pCurrentPool += uElementSize + _iPoolCorruptionMarkerSize + _iPoolExtraDebugSize;
	}

	// initialize last entry ptr to NULL
	*(Char**)pCurrentPool = NULL;

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	SetupCorruptionMarker(uElementSize);
#endif

	_pFirstFreeChunk = _pPool;
}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
Bool	MEM_POOLNEW::IsInFreeList( char* pPtr )
{
	char* pCurrentElement = _pFirstFreeChunk;

	while(pCurrentElement)
	{
		if(pCurrentElement == pPtr)
		{
			return true;
		}
		pCurrentElement = *(char**)pCurrentElement;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		DumpUsageMap
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_POOLNEW::DumpUsageMap( IO_STREAM* pStream )
{
	char textBuffer[256];
	UInt32 i;

	pStream->PutString("\t\t\t\t<POOL>");

	sprintf(textBuffer, "\t\t\t\t\t<numElement>%d</numElement>", _NumElementInPool);
	pStream->PutString(textBuffer);

	Char* pCurrentElement = _pPool;

	for(i=0;i<_NumElementInPool;i++)
	{
		
		if(IsInFreeList(pCurrentElement) == false)
		{
			pStream->PutString("\t\t\t\t\t<POOL_ELEMENT>");
			pStream->PutString("\t\t\t\t\t\t\t<isFree>0</isFree>");

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
			{
				UInt32 j;

				for(j=0;j<_iMaxStackDepth;j++)
				{
					UInt32 pStack = *(UInt32*)(((char*)pCurrentElement) + _uElementSize + _iPoolCorruptionMarkerSize + j*4);
					sprintf(textBuffer, "\t\t\t\t\t\t\t<callStack>0x%08X</callStack>", pStack);
					pStream->PutString(textBuffer);
				}
			}
#endif
#ifdef QDT_MEM_DEBUG
			{
				sprintf(textBuffer, "\t\t\t\t\t\t\t<allocationTime>%f</allocationTime>", *(Float64*)(((char*)pCurrentElement) + _uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4));
				pStream->PutString(textBuffer);

				sprintf(textBuffer, "\t\t\t\t\t\t\t<id>0x%08X</id>", *(UInt32*)(((char*)pCurrentElement) + _uElementSize + _iPoolCorruptionMarkerSize + _iMaxStackDepth * 4 + 8));
				pStream->PutString(textBuffer);
			}
#endif
			
			pStream->PutString("\t\t\t\t\t</POOL_ELEMENT>");
		}

		pCurrentElement += _uElementSize + _iPoolCorruptionMarkerSize + _iPoolExtraDebugSize;
	}

	pStream->PutString("\t\t\t\t</POOL>");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
