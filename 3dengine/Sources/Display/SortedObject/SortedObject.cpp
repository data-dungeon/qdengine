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
//	CLASS:	SORTED_OBJECT
//
//	04-07-19:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/SortedObject/SortedObject)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SORTED_OBJECT constructor
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
SORTED_OBJECT::SORTED_OBJECT()
:
_pEntity(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SORTED_OBJECT constructor
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
SORTED_OBJECT::SORTED_OBJECT(const SORTED_OBJECT & C)
:
_pEntity(C._pEntity),
_vPrimitives(C._vPrimitives)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SORTED_OBJECT constructor
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
SORTED_OBJECT::SORTED_OBJECT(ENTITY_ABC *	pEntity,
							 UInt32			nPrimitive)
:
_pEntity(pEntity)
{
	_vPrimitives.PushTail(nPrimitive);
}


//-----------------------------------------------------------------------------
//	Name:		SORTED_OBJECT destructor
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
SORTED_OBJECT::~SORTED_OBJECT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-07-19:	RMA - Created
//-----------------------------------------------------------------------------
SORTED_OBJECT &	SORTED_OBJECT::operator=(const SORTED_OBJECT & C)
{
	_pEntity = C._pEntity;
	_vPrimitives = C._vPrimitives;

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		AddPrimitive
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_OBJECT::AddPrimitive(UInt32	nPrimitive)
{
	_vPrimitives.PushTail(nPrimitive);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
