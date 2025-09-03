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
//	CLASS:	AABR2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/AABR2D/AABR2D)
#include	INCL_KCORE(stream/iostream)

#include	INCL_KMATH(Math/Geometry/Intersection/Intersection2d)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle2D/Triangle2D)	

#ifdef _DEBUG
#include	"AABR2D.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::Read(IO_STREAM &	Stream)
{
	Stream >> _rXMin >> _rYMin >> _rXMax >> _rYMax;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::Write(IO_STREAM &	Stream) const
{
	Stream << _rXMin << _rYMin << _rXMax << _rYMax;
}

//-----------------------------------------------------------------------------
//	Name:		GetSidedSegment
//	Object:		
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
SEGMENT2D	AABR2D::GetSidedSegment(const SIDE	eSide) const
{
	POINT2D	P1(false), P2(false);
	switch(eSide)
	{
	default:
		QDT_FAIL();
	case	SIDE_NORTH:
		P1.SetXY(GetXMin(), GetYMin());
		P2.SetXY(GetXMax(), GetYMin());
		break;
	case	SIDE_EAST:
		P1.SetXY(GetXMax(), GetYMin());
		P2.SetXY(GetXMax(), GetYMax());
		break;
	case	SIDE_SOUTH:
		P1.SetXY(GetXMin(), GetYMax());
		P2.SetXY(GetXMax(), GetYMax());
		break;
	case	SIDE_WEST:
		P1.SetXY(GetXMin(), GetYMin());
		P2.SetXY(GetXMin(), GetYMax());
		break;
	}

	return	(SEGMENT2D(P1, P2));
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-07-12:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::Grow(const POINT2D &	P)
{
	if (P.GetX() > _rXMax) _rXMax = P.GetX();
	if (P.GetY() > _rYMax) _rYMax = P.GetY();
	if (P.GetX() < _rXMin) _rXMin = P.GetX();
	if (P.GetY() < _rYMin) _rYMin = P.GetY();
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-07-12:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::Grow(const AABR2D &	Aabr)
{
	Grow(Aabr.GetMin());
	Grow(Aabr.GetMax());
}

//-----------------------------------------------------------------------------
//	Name:		GetIntersectedArea
//	Object:		
//	02-07-15:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetIntersectedArea(const TRIANGLE2D &T) const
{
	const int	nNbrPoints = 6;
	POINT2D		aPoints[nNbrPoints];
	int			nNbrPointsOut = nNbrPoints;
	float		rArea = 0.f;

	if (T.IsDegenerated()) return (0.0f);

	if	(INTERSECTION2D::ClipPolygon(*this, T.GetBuffer(), aPoints, 3, nNbrPointsOut))
	{
		int	nNbrTriangles = nNbrPointsOut - 2;
		int	i;

		//the intersected polygon is convex so fabsf is good
		for (i = 0 ; i < nNbrTriangles ; ++i)
		{
			rArea += MATH::Abs(TRIANGLE2D(aPoints[0], aPoints[i+1], aPoints[i+2]).GetArea());
		}
	}

	return	(rArea);
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	02-07-16:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetArea() const
{
	return ((_rXMax - _rXMin) * (_rYMax - _rYMin));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
