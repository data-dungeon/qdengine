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
//
//	06-02-15:	VHA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Root.h"
#include	INCL_KCORE(Mem/MemConfig)
#include	INCL_KCORE(Mem/MemVirtualManager)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MEM_VIRTUAL_MANAGER constuctor
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
MEM_VIRTUAL_MANAGER::MEM_VIRTUAL_MANAGER(UInt32 uVirtualMemorySize)
{
	_uVirtualMemorySize = uVirtualMemorySize;
	_pFirstElement = new MEM_VIRTUAL_ELEMENT();

	_pFirstElement->_bIsFree = true;
	_pFirstElement->_uOffsetInMemory = 0;
	_pFirstElement->_uSize = uVirtualMemorySize;
	_pFirstElement->_pNextElement = NULL;
	_pFirstElement->_pPreviousElement = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		MEM_VIRTUAL_MANAGER destructor
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
MEM_VIRTUAL_MANAGER::~MEM_VIRTUAL_MANAGER()
{
}

//-----------------------------------------------------------------------------
//	Name:		Alloc
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
MEM_VIRTUAL_ELEMENT*	MEM_VIRTUAL_MANAGER::Alloc(UInt32 uSize)
{
	MEM_VIRTUAL_ELEMENT* pCurrentElement = _pFirstElement;

	while(pCurrentElement)
	{
		if((pCurrentElement->_bIsFree) && (pCurrentElement->_uSize >= uSize))
		{
			// block is big enough
			pCurrentElement->Split(uSize);
			return pCurrentElement;
		}

		pCurrentElement = pCurrentElement->_pNextElement;
	}

	QDT_FAIL();

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_VIRTUAL_MANAGER::Free(MEM_VIRTUAL_ELEMENT* pElement)
{
	QDT_ASSERT(pElement->_bIsFree == false);

	if(		(pElement->_pPreviousElement && pElement->_pPreviousElement->_bIsFree)
		&&	(pElement->_pNextElement && pElement->_pNextElement->_bIsFree))
	{
		MEM_VIRTUAL_ELEMENT* pPreviousElement = pElement->_pPreviousElement;
		MEM_VIRTUAL_ELEMENT* pNextElement = pElement->_pNextElement;

		if(pNextElement->_pNextElement)
		{
			pNextElement->_pNextElement->_pPreviousElement = pPreviousElement;
		}

		pPreviousElement->_pNextElement = pNextElement->_pNextElement;
		pPreviousElement->_uSize += pElement->_uSize + pNextElement->_uSize;
		
		delete pElement;
		delete pNextElement;
	}
	else
	if(pElement->_pPreviousElement && pElement->_pPreviousElement->_bIsFree)
	{
		MEM_VIRTUAL_ELEMENT* pPreviousElement = pElement->_pPreviousElement;

		if(pElement->_pNextElement)
		{
			pElement->_pNextElement->_pPreviousElement = pPreviousElement;
		}

		pPreviousElement->_pNextElement = pElement->_pNextElement;
		pPreviousElement->_uSize += pElement->_uSize;

		delete pElement;
	}
	else
	if(pElement->_pNextElement && pElement->_pNextElement->_bIsFree)
	{
		MEM_VIRTUAL_ELEMENT* pNextElement = pElement->_pNextElement;

		pElement->_uSize += pNextElement->_uSize;
		pElement->_bIsFree = true;

		if(pNextElement->_pNextElement)
		{
			pNextElement->_pNextElement->_pPreviousElement = pElement;
		}

		pElement->_pNextElement = pNextElement->_pNextElement;

		delete pNextElement;
	}
	else
	{
		pElement->_bIsFree = true;
	}
}

#ifdef QDT_MEM_DEBUG
//-----------------------------------------------------------------------------
//	Name:		CheckConsistency
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void	MEM_VIRTUAL_MANAGER::CheckConsistency()
{
	UInt32 uSize = 0;
	MEM_VIRTUAL_ELEMENT* pCurrentElement = _pFirstElement;

	while(pCurrentElement)
	{
		uSize+=pCurrentElement->_uSize;

		if(pCurrentElement->_bIsFree && pCurrentElement->_pNextElement)
		{
			QDT_ASSERT(pCurrentElement->_pNextElement->_bIsFree == false);
		}

		pCurrentElement = pCurrentElement->_pNextElement;
	}

	QDT_ASSERT(uSize == _uVirtualMemorySize);
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
