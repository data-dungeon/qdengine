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
//	CLASS:	MEM_VIRTUAL_MANAGER
//	The MEM_VIRTUAL_MANAGER class implements ...
//
//	06-02-15:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_VIRTUAL_MANAGER_H__
#define __MEM_VIRTUAL_MANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Mem/MemVirtualElement)

class MEM_VIRTUAL_MANAGER
{
public:
	MEM_VIRTUAL_MANAGER(UInt32 uVirtualMemorySize);
	~MEM_VIRTUAL_MANAGER();

	MEM_VIRTUAL_ELEMENT* Alloc(UInt32 uSize);
	void	Free(MEM_VIRTUAL_ELEMENT* pElement);

#ifdef QDT_MEM_DEBUG
	void	CheckConsistency();
#endif

private:
	MEM_VIRTUAL_ELEMENT* _pFirstElement;
	UInt32 _uVirtualMemorySize;
};

#endif //__MEM_VIRTUAL_MANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
