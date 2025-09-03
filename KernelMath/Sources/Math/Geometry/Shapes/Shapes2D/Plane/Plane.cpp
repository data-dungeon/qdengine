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
// CLASS: PLANE
//
//
//00-11-09 JVA Created:
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/Geometry/Distance/Distance)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
#include "plane.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		PLANE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
PLANE::PLANE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PLANE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
PLANE::PLANE(const PLANE &	Plane)
:
_vNormal(Plane._vNormal)
{
	SetD(Plane.GetD());
}

//-----------------------------------------------------------------------------
//	Name:		PLANE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
PLANE::PLANE(float	rA,
			 float	rB,
			 float	rC,
			 float	rD)
:
_vNormal( rA, rB, rC )
{
	SetD(rD);
}

//-----------------------------------------------------------------------------
//	Name:		PLANE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
PLANE::PLANE(const VECTOR &	Pt,
			 const VECTOR &		Normal)
{
	SetFromPointAndNormal( Pt , Normal );
}

//-----------------------------------------------------------------------------
//	Name:		PLANE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
PLANE::PLANE(const TRIANGLE &	Triangle)
{
	SetFromPoints(Triangle.GetPoint0(), Triangle.GetPoint1(), Triangle.GetPoint2());
}

//-----------------------------------------------------------------------------
//	Name:		PLANE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
PLANE::PLANE( const VECTOR & Pt1,
			  const VECTOR & Pt2,
			  const VECTOR & Pt3 )
{
#ifdef QDT_DEBUG
	const float rEpsilon = 0.001f;
#endif

	QDT_ASSERT( DISTANCE::DistancePointPoint( Pt1, Pt2 ) > rEpsilon );
	QDT_ASSERT( DISTANCE::DistancePointPoint( Pt1, Pt3 ) > rEpsilon );
	QDT_ASSERT( DISTANCE::DistancePointPoint( Pt2, Pt3 ) > rEpsilon );

	VECTOR	Vector1 = Pt2 - Pt1;
	VECTOR	Vector2 = Pt3 - Pt1;
	VECTOR	Normal;
	
	Normal.Cross( Vector1 , Vector2 );  

	SetFromPointAndNormal(Pt1 , Normal);
}

//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-16:	RPA - Created
//  01-02-14:	GGO - Added QDT_ASSERT.
//-----------------------------------------------------------------------------
void	PLANE::SetFromPoints(const VECTOR &	Pt1, 
							 const VECTOR &	Pt2,
							 const VECTOR &	Pt3)
{
#ifdef QDT_DEBUG
	const float rEpsilon = 0.001f;
#endif

	QDT_ASSERT( DISTANCE::DistancePointPoint( Pt1, Pt2 ) > rEpsilon );
	QDT_ASSERT( DISTANCE::DistancePointPoint( Pt1, Pt3 ) > rEpsilon );
	QDT_ASSERT( DISTANCE::DistancePointPoint( Pt2, Pt3 ) > rEpsilon );

	VECTOR	Vector1 = Pt2 - Pt1;
	VECTOR	Vector2 = Pt3 - Pt1;
	VECTOR	Normal;
	
	Normal.Cross( Vector1 , Vector2 );
	Normal.Normalize();

	SetFromPointAndNormal( Pt1 , Normal );
}

//-----------------------------------------------------------------------------
//	Name:		VerticalProjection
//	Object:		
//	03-03-20:	RMA - Created
//-----------------------------------------------------------------------------
void	PLANE::VerticalProjection(const VECTOR &	Src,
								  VECTOR &			Dst) const
{
	QDT_ASSERT(_vNormal.GetY() != 0);

	Dst = Src;
	Dst.SetY(-_vNormal.GetX() * Dst.GetX() -_vNormal.GetZ() * Dst.GetZ() - GetD());
}

//-----------------------------------------------------------------------------
//	Name:		AreOutside
//	Object:		
//	03-04-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	PLANE::AreOutside(int		nNbrPoints,
						  VECTOR *	pPoints) const
{
	int		i;
	VECTOR	vDiff;

	for	(i = 0 ; i < nNbrPoints ; ++i)
	{
		vDiff.Sub(pPoints[i], GetPoint(true));

		if (vDiff.Dot(_vNormal) < 0.0f)
		{
			return	(false);
		}
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		IsOutside
//	Object:		
//	03-09-16:	ELE - Created
//-----------------------------------------------------------------------------
bool	PLANE::IsOutside(const VECTOR &	Point) const
{
	VECTOR	vDiff;

	vDiff.Sub(Point, GetPoint(true));

	if (vDiff.Dot(_vNormal) < 0.0f)
	{
		return	(false);
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	PLANE::Intersect(const VECTOR &	PIn,
						 VECTOR	&		POut,
						 const VECTOR &	vMotion,
						 const float	rInertia,
						 const float	rE) const
{
	float	rSignedDistance = GetSignedDistance(PIn);

	if	(rSignedDistance < 0.0f)
	{
		VECTOR	V = GetNormal();

		V.Mul(-rSignedDistance + rE);
		POut.Add(PIn, V);

		return	(true);
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PLANE::Read(IO_STREAM &	s)
{
	float rD;
	s >> _vNormal >> rD;
	SetD(rD);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PLANE::Write(IO_STREAM &	s) const
{
	s << _vNormal << GetD();
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::Transform(const MATRIX &	Matrix)
{
	VECTOR	Point;
	VECTOR	Normal;

	GetPoint(Point);
	GetNormal(Normal);

	Matrix.TransformPosition(Point);
	Matrix.TransformDirection(Normal);
	
	SetFromPointAndNormal(Point , Normal);
}

//-----------------------------------------------------------------------------
//	Name:		ZProjection
//	Object:		Plane Projected on screen eq. to
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::ZProjection(const float	xcen,
						   const float	ycen,
						   const float	xfoc,
						   const float	yfoc)
{
	float	rHx , rHy;
	float	rA , rB , rC , rD;

	rHx	 =  1.0f / xfoc;
	rHy	 = -1.0f / yfoc;
	rA	 = _vNormal.GetX() * rHx;
	rB	 = _vNormal.GetY() * rHy;
	rC	 = GetD();
	rD	 = -(_vNormal.GetZ() + (_vNormal.GetX() * xcen * rHx) + (_vNormal.GetY() * ycen * rHy));

	_vNormal.SetXYZ( rA, rB, rC );
	SetD(rD);
}

//-----------------------------------------------------------------------------
//	Name:		GetT
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
float	PLANE::GetT(VECTOR &	p1,
					VECTOR &	p2) const
{
	float x1 , y1 , z1;
	float x2 , y2 , z2 , t;
	p1.GetXYZ(x1 , y1 , z1);
	p2.GetXYZ(x2 , y2 , z2);
	t = (_vNormal.GetX() * x1 + _vNormal.GetY() * y1 + _vNormal.GetZ() * z1 + GetD())
	  / (_vNormal.GetX() * (x1 - x2) + _vNormal.GetY() * (y1 - y2) + _vNormal.GetZ() * (z1 - z2));
	return ( t );
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::GetPoint(VECTOR &	Point) const
{
	if ( MATH::Abs( _vNormal.GetX() ) >= MATH::Abs( _vNormal.GetY() ) )
	{
		if ( MATH::Abs( _vNormal.GetX() ) >= MATH::Abs( _vNormal.GetZ() ) )
		{
			Point.SetXYZ( -GetD() / _vNormal.GetX() , 0.0f , 0.0f );
		}
		else
		{
			Point.SetXYZ( 0.0f , 0.0f , -GetD() / _vNormal.GetZ() );
		}
	}
	else
	{
		if ( MATH::Abs( _vNormal.GetY() ) >= MATH::Abs( _vNormal.GetZ() ) )
		{
			Point.SetXYZ( 0.0f , -GetD() / _vNormal.GetY() , 0.0f );
		}
		else
		{
			Point.SetXYZ( 0.0f , 0.0f , -GetD() / _vNormal.GetZ() );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Apply
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
float	PLANE::Apply(VECTOR &	p)
{
	float x , y , z;
	p.GetXYZ( x , y , z );
	return ( x * _vNormal.GetX() + y * _vNormal.GetY() + z * _vNormal.GetZ() + GetD() );
}

//-----------------------------------------------------------------------------
//	Name:		GetProjection
//	Object:		Project a point on a plane according to the direction dir
//				Return true if the point is on the plane, else false
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
bool PLANE::Project(const VECTOR &	Pt, 
					const VECTOR &	Dir, 
					VECTOR &		Res) const
{
	VECTOR	V;
	float	rAlpha;
	float	rConst;
	float	rDot;

	rConst = GetD() - (_vNormal.GetX() * Pt.GetX() + _vNormal.GetY() * Pt.GetY() + _vNormal.GetZ() * Pt.GetZ());

	if (MATH::Abs(rConst) < MATH::EPSILON())
	{
		Res = Pt;
		return (true);
	}
	else
	{
		rDot = _vNormal.GetX() * Dir.GetX() + _vNormal.GetY() * Dir.GetY() + _vNormal.GetZ() * Dir.GetZ();

		if (MATH::Abs(rDot) < MATH::EPSILON())
		{
			return (false);
		}
		else
		{
			QDT_ASSERT(rDot != 0.0f);
			rAlpha = rConst / rDot;

			V.Mul(Dir, rAlpha);

			Res.Add(Pt, V);

			return (true);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSide
//	Object:		
//	02-04-29:	GGO - Created
//-----------------------------------------------------------------------------
void	PLANE::GetSide(const VECTOR &	Pt,
					   int &			nSide,
					   const float		rEpsilon) const
{
	QDT_ASSERT( rEpsilon >= 0.0f );

	float rValue;

	rValue =	_vNormal.GetX() * Pt.GetX() +
				_vNormal.GetY() * Pt.GetY() +
				_vNormal.GetZ() * Pt.GetZ() +
				GetD();

	if ( MATH::Abs( rValue ) <= rEpsilon )
	{
		nSide = 0;
		return;
	}

	if ( rValue > 0.0f )
	{
		nSide = 1;
		return;
	}

	if ( rValue < 0.0f )
	{
		nSide = -1;
		return;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSide
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
bool	PLANE::GetSide(const VECTOR &	Pt) const
{
	float rValue;

	rValue =	_vNormal.GetX() * Pt.GetX() +
				_vNormal.GetY() * Pt.GetY() +
				_vNormal.GetZ() * Pt.GetZ() +
				GetD();

	return ( rValue >= 0.0f );
}

//-----------------------------------------------------------------------------
//	Name:		IsOn
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
bool	PLANE::IsOn( const VECTOR & Pt )const
{
	float rValue = _vNormal.GetX() * Pt.GetX() + 
				   _vNormal.GetY() * Pt.GetY() + 
				   _vNormal.GetZ() * Pt.GetZ() + GetD();

	return (MATH::Abs(rValue) <= 0.1);
}

//-----------------------------------------------------------------------------
//	Name:		IsOn
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
bool	PLANE::IsOn(const VECTOR &	Pt,
					bool &			bSide )	const
{
	float rValue = _vNormal.GetX() * Pt.GetX() + _vNormal.GetY() * Pt.GetY() + _vNormal.GetZ() * Pt.GetZ() + GetD();

	bSide = ( rValue >= 0 );

	return ( MATH::Abs( rValue ) <= 0.1 );
}

//-----------------------------------------------------------------------------
//	Name:		PLANE::Projection
//	Object:		Project orthogonaly a point to a plane
//	01-04-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PLANE::Project(const VECTOR &	Pt,
					   VECTOR &			Res)
{
	VECTOR Projection(_vNormal.GetX(), _vNormal.GetY(), _vNormal.GetZ());

	QDT_ASSERT(_vNormal.SquareLength() != 0.0f);
	float rCoeff = -(_vNormal.GetX() * Pt.GetX() + _vNormal.GetY() * Pt.GetY() + _vNormal.GetZ() * Pt.GetZ() + GetD()) / _vNormal.SquareLength();

	Projection *= rCoeff;	// Projection is the vector which came from the point & go to the plane, // to the normal

	Res = Pt + Projection;

	QDT_ASSERT( IsOn( Res ) == true );
}

//-----------------------------------------------------------------------------
//	Name:		GetVectors
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	PLANE::GetVectors(VECTOR &	Vector1,
						  VECTOR &	Vector2) const
{
	VECTOR	V(VECTOR::UNIT_X);

	Vector1.Cross( _vNormal , V );

	if ( Vector1.IsNull() )
	{
		V.SetXYZ( 0.0f , 1.0f , 0.0f );

		Vector1.Cross( _vNormal , V );

		if ( Vector1.IsNull() )
		{
			V.SetXYZ( 0.0f , 0.0f , 1.0f );

			Vector1.Cross( _vNormal , V );

			if ( Vector1.IsNull() )
			{
				return;
			}
		}
	}

	Vector2.Cross( _vNormal , Vector1 );

	Vector1.Normalize();
	Vector2.Normalize();
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
