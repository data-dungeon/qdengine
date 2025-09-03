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
//	CLASS:	MEM_BIG_CHUNK_ELEMENT
//	The MEM_BIG_CHUNK_ELEMENT class implements ...
//
//	06-02-08:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	MEM_BIG_CHUNK_ELEMENT
{
public:
#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
	char _corruptionMarker[16];
#endif

	Bool	_isFree; // can be changed to char if we need some space in the structure
	UInt32	_blockSize;
	MEM_BIG_CHUNK_ELEMENT* _pPreviousBlock;
	MEM_BIG_CHUNK_ELEMENT* _pNextFreeBlock; // only valid if the block is free

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
	void* _pStack[_iMaxStackDepth]; 
#endif
#ifdef QDT_MEM_DEBUG
	Float64 _fAllocationTime;
	UInt32	_iId;
	Bool	_bIsArray;
#endif
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
