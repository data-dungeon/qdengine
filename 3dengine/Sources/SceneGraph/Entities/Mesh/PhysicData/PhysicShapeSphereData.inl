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
//	CLASS:	PHYSIC_SHAPE_SPHERE_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetRadius
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_SHAPE_SPHERE_DATA::GetRadius() const
{
	return	(_rRadius);
}

//-----------------------------------------------------------------------------
//	Name:		SetRadius
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_SPHERE_DATA::SetRadius(Float32	rRadius)
{
	_rRadius = rRadius;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
