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
//	CLASS:	PHYSIC_SHAPE_BOX_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetDimensions
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	PHYSIC_SHAPE_BOX_DATA::GetDimensions() const
{
	return	(_Dimensions);
}

//-----------------------------------------------------------------------------
//	Name:		SetDimensions
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_BOX_DATA::SetDimensions(const VECTOR &	Dimensions)
{
	_Dimensions = Dimensions;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
