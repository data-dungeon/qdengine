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
// CLASS: SEGMENT
//
//
//00 - 11 - 10 JVA Created:
//*****************************************************************************

#include "Root.h"
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include INCL_KMATH(Math/Geometry/Distance/Distance)
#include INCL_KCORE(Stream/iostream)
#include INCL_KMATH(Math/Geometry/Matrix/Matrix)

#ifdef _DEBUG
#include "segment.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT::Display(const COLOR_FLOAT &	Color) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT::Read(IO_STREAM & Stream)
{
	Stream >> _Point1 >> _Point2;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT::Write(IO_STREAM &	Stream) const
{
	Stream << _Point1 << _Point2;
}

//-----------------------------------------------------------------------------
//	Name:		Length
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
float	SEGMENT::Length() const
{
	return (DISTANCE::DistancePointPoint(_Point1, _Point2));
}

//-----------------------------------------------------------------------------
//	Name:		SquareLength
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
float	SEGMENT::SquareLength() const
{
	return (DISTANCE::DistanceSquarePointPoint(_Point1, _Point2));
}

//-----------------------------------------------------------------------------
//	Name:		IsOn
//	Object:		
//	03-03-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	SEGMENT::IsOn(const VECTOR &	P) const
{
	VECTOR Edge0; 
	VECTOR Edge1;
	VECTOR Cross;
	
	Edge0.Sub(P, _Point1);
	Edge1.Sub(_Point2, _Point1);

	Cross.Cross(Edge0, Edge1);
	
	if (Cross.SquareLength() < 1.0f && Edge0.Dot(Edge1) > 0.0f && 
		Edge0.SquareLength() < Edge1.SquareLength())
	{
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		SqrDistance
//	Object:		
//	03-08-13:	RMA - Created
//	06-01-23:	BFA - Updated to return projection ratio [0...1]
//-----------------------------------------------------------------------------
float	SEGMENT::SqrDistance(const VECTOR & P, Float32* prProjectionRatio)
{
	VECTOR vDiff = P - _Point1;
	VECTOR vDir = _Point2 - _Point1;

	float rT = vDiff.Dot(vDir);

	if (rT <= 0.0f)
	{
		rT = 0.0f;
	}
	else
	{
		float rSqrLength= vDir.SquareLength();

		if (rT >= rSqrLength)
        {
			rT = 1.0f;
            vDiff -= vDir;
        }
        else
        {
            rT /= rSqrLength;
            vDiff -= rT * vDir;
        }
	}

	if ( prProjectionRatio )
		*prProjectionRatio = rT;

	return (vDiff.SquareLength());
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT::Transform(const MATRIX &	Matrix)
{
	Matrix.TransformPosition(_Point1);
	Matrix.TransformPosition(_Point1);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
