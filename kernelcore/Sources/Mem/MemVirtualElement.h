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
//	The MEM_VIRTUAL_ELEMENT class implements ...
//
//	06-02-15:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_VIRTUAL_ELEMENT_H__
#define __MEM_VIRTUAL_ELEMENT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Mem/MemConfig)

class MEM_VIRTUAL_MANAGER;

class MEM_VIRTUAL_ELEMENT
{
public:
	void	Split(UInt32 uSize);
	void	Free();

	UInt32	_uOffsetInMemory;

	friend class MEM_VIRTUAL_MANAGER;

private:
	Bool					_bIsFree;
	UInt32					_uSize;
	MEM_VIRTUAL_ELEMENT*	_pNextElement;
	MEM_VIRTUAL_ELEMENT*	_pPreviousElement;
};

#endif //__MEM_VIRTUAL_ELEMENT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
