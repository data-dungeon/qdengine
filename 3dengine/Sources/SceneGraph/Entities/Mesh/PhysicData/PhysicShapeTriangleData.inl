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
//	CLASS:	PHYSIC_SHAPE_TRIANGLE_DATA
//
//	06-02-17:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	06-03-27:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_SHAPE_TRIANGLE_DATA::PHYSIC_SHAPE_TRIANGLE_TYPE	PHYSIC_SHAPE_TRIANGLE_DATA::GetType() const
{
	return	(_Type);
}

//-----------------------------------------------------------------------------
//	Name:		GetGeometricDataSize
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PHYSIC_SHAPE_TRIANGLE_DATA::GetGeometricDataSize() const
{
	return	(_GeometricDataSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetGeometricData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
const void *	PHYSIC_SHAPE_TRIANGLE_DATA::GetGeometricData() const
{
	return	(_pGeometricData);
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	06-03-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_TRIANGLE_DATA::SetType(PHYSIC_SHAPE_TRIANGLE_TYPE	Type)
{
	_Type = Type;
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometricDataSize
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_TRIANGLE_DATA::SetGeometricDataSize(UInt32	nSize)
{
	_GeometricDataSize = nSize;
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometricData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_SHAPE_TRIANGLE_DATA::SetGeometricData(void *	pGeometricData)
{
	_pGeometricData = pGeometricData;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
