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
//	CLASS:	TRANSPARENT_OBJECT
//
//	03-11-26:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/TransparentObject/TransparentObject)
#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

#ifdef _DEBUG
	#include	"TransparentObject.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TRANSPARENT_OBJECT constructor
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
TRANSPARENT_OBJECT::TRANSPARENT_OBJECT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TRANSPARENT_OBJECT constructor
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
TRANSPARENT_OBJECT::TRANSPARENT_OBJECT(Float32		rCamDistance,
									   ENTITY_ABC *	pEntity,
									   UInt32		nPrimitive,
									   UInt8		uiPriority)
:
_rCameraDistance	(rCamDistance),
_pEntity			(pEntity),
_nPrimitive			(nPrimitive),
_uiPriority			(uiPriority)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TRANSPARENT_OBJECT constructor
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
TRANSPARENT_OBJECT::TRANSPARENT_OBJECT(const TRANSPARENT_OBJECT & TransObj)
:
_rCameraDistance	(TransObj._rCameraDistance),
_pEntity			(TransObj._pEntity),
_nPrimitive			(TransObj._nPrimitive),
_uiPriority			(TransObj._uiPriority)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TRANSPARENT_OBJECT destructor
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
TRANSPARENT_OBJECT::~TRANSPARENT_OBJECT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
void	TRANSPARENT_OBJECT::Display()
{
	_pEntity->Display(_nPrimitive);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
TRANSPARENT_OBJECT &	TRANSPARENT_OBJECT::operator=(const TRANSPARENT_OBJECT & TransObj)
{
	_rCameraDistance	= TransObj._rCameraDistance;
	_pEntity			= TransObj._pEntity;
	_nPrimitive			= TransObj._nPrimitive;
	_uiPriority			= TransObj._uiPriority;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator <	
//	Object:		
//	03-11-26:	RMA - Created
//-----------------------------------------------------------------------------
bool 	TRANSPARENT_OBJECT::operator <(const TRANSPARENT_OBJECT &	TransObj) const
{
	if (_uiPriority < TransObj._uiPriority)
	{
		return (true);
	}
	else if (_uiPriority == TransObj._uiPriority)
	{
		return (GetCameraDistance() > TransObj.GetCameraDistance());
	}

	return (false);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
