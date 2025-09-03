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
//	CLASS:	STATIC_PHYSIC_DATA
//
//	06-02-16:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetGeometricDataSize
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	STATIC_PHYSIC_DATA::GetGeometricDataSize()
{
	return	(_GeometricDataSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetGeometricData
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
const void *	STATIC_PHYSIC_DATA::GetGeometricData() const
{
	return	(_pGeometricData);
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometricDataSize
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::SetGeometricDataSize(UInt32	nSize)
{
	_GeometricDataSize = nSize;
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometricData
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::SetGeometricData(void *	pGeometricData)
{
	_pGeometricData = pGeometricData;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
