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

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetCameraDistance
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	TRANSPARENT_OBJECT::SetCameraDistance(Float32	rDistance)
{
	_rCameraDistance = rDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetEntity
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	TRANSPARENT_OBJECT::SetEntity(ENTITY_ABC *	pEntity)
{
	_pEntity = pEntity;
}

//-----------------------------------------------------------------------------
//	Name:		SetPrimitive
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	TRANSPARENT_OBJECT::SetPrimitive(UInt32	nPrimitive)
{
	_nPrimitive = nPrimitive;
}
			
//-----------------------------------------------------------------------------
//	Name:		GetCameraDistance
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
Float32	TRANSPARENT_OBJECT::GetCameraDistance() const
{
	return	(_rCameraDistance);
}

//-----------------------------------------------------------------------------
//	Name:		GetEntity
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
ENTITY_ABC *	TRANSPARENT_OBJECT::GetEntity()
{
	return	(_pEntity);
}

//-----------------------------------------------------------------------------
//	Name:		GetPrimitive
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	TRANSPARENT_OBJECT::GetPrimitive() const
{
	return	(_nPrimitive);
}

//-----------------------------------------------------------------------------
//	Name:		GetPrimitive
//	Object:		
//	06-03-30:	RMA - Created
//-----------------------------------------------------------------------------
void	TRANSPARENT_OBJECT::SetPriority(UInt8 uiPriority)
{
	_uiPriority = uiPriority;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
