//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: TRIANGLE
//
//
//00 - 11 - 10 JVA Created:
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
	#include	"Triangle.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::Read(IO_STREAM &	s)
{
	s >> _Point0 >> _Point1 >> _Point2;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::Write(IO_STREAM &	s) const
{
	s << _Point0 << _Point1 << _Point2;
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::Display(const COLOR_FLOAT &	Color) const
{
#ifndef _MASTER
	SEGMENT	Edge1(_Point0, _Point1);
	SEGMENT	Edge2(_Point1, _Point2);
	SEGMENT	Edge3(_Point2, _Point0);

	Edge1.Display(Color);
	Edge2.Display(Color);
	Edge3.Display(Color);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetRandomPoint
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::GetRandomPoint(VECTOR & Point) const
{
	float	rAlpha = MATH::Rand(1.0f);
	float	rBeta = MATH::Rand(1.0f - rAlpha);
	float	rGamma = 1.0f - rAlpha - rBeta;

	VECTOR	P0(_Point0);
	VECTOR	P1(_Point1);
	VECTOR	P2(_Point2);
	VECTOR	P4;

	P0.Mul(rAlpha);
	P1.Mul(rBeta);
	P2.Mul(rGamma);

	P4.Add(P0, P1);
	Point.Add(P2, P4);
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::Transform( const MATRIX & Matrix )
{
	Matrix.TransformPosition( _Point0 );
	Matrix.TransformPosition( _Point1 );
	Matrix.TransformPosition( _Point2 );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
