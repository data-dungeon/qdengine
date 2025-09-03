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
//	CLASS:	MEM_VIRTUAL_ELEMENT
//
//	06-02-15:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"
#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Mem/MemVirtualElement)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Split
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void MEM_VIRTUAL_ELEMENT::Split(UInt32 uSize)
{
	QDT_ASSERT(_bIsFree == true);
	QDT_ASSERT(_uSize >= uSize);

	MEM_VIRTUAL_ELEMENT* pNewFreeBlock = new MEM_VIRTUAL_ELEMENT();

	pNewFreeBlock->_bIsFree = true;
	pNewFreeBlock->_pNextElement = _pNextElement;
	pNewFreeBlock->_pPreviousElement = this;
	pNewFreeBlock->_uOffsetInMemory = _uOffsetInMemory + uSize;
	pNewFreeBlock->_uSize = _uSize - uSize;

	_uSize = uSize;
	_bIsFree = false;
	_pNextElement = pNewFreeBlock;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
