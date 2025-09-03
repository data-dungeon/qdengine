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
//	CLASS:	DISTANCE2D
//
//	01-09-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DistancePointPoint
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
float	DISTANCE2D::DistancePointPoint(const POINT2D &	Pt1,
										const POINT2D &	Pt2)
{
	return (MATH::Sqrt(DistanceSquarePointPoint(Pt1, Pt2)));
}

//-----------------------------------------------------------------------------
//	Name:		DistanceSquarePointPoint
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
float	DISTANCE2D::DistanceSquarePointPoint(const POINT2D &	Pt1,
											  const POINT2D &	Pt2)
{
	return (MATH::Square(Pt2.GetX() - Pt1.GetX()) + MATH::Square(Pt2.GetY() - Pt1.GetY()));
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
