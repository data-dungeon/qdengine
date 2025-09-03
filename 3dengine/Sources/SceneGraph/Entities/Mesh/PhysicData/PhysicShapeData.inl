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
//	CLASS:	PHYSIC_SHAPE_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_DATA::PHYSIC_SHAPE_DATA_TYPE	PHYSIC_SHAPE_DATA::GetType() const
{
	return	(_Type);
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalMatrix
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	PHYSIC_SHAPE_DATA::GetLocalMatrix() const
{
	return	(_LocalMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		SetLocalMatrix
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_DATA::SetLocalMatrix(const MATRIX &	LocalMatrix)
{
	_LocalMatrix = LocalMatrix;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
