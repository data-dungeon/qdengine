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
//	CLASS:	SPHERE
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(stream/iostream)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Cone/Cone)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include	INCL_KMATH(Math/Geometry/Distance/Distance)
#include	INCL_KMATH(Math/LinearAlgebra/CovarianceMatrix)

void*		SPHERE::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		SPHERE constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
SPHERE::SPHERE()
:
BOUNDING_VOLUME(TYPE_SPHERE)
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SPHERE constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
SPHERE::SPHERE(const SPHERE & C)
:
BOUNDING_VOLUME(TYPE_SPHERE),
_Center(C._Center)
{
	SetRadius(C.GetRadius());
}

//-----------------------------------------------------------------------------
//	Name:		SPHERE constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
SPHERE::SPHERE(const VECTOR &Centre, Float32 rRadius)
:
BOUNDING_VOLUME(TYPE_SPHERE),
_Center (Centre)
{
	QDT_ASSERT(rRadius >= 0.0f);
	SetRadius(rRadius);
}

//-----------------------------------------------------------------------------
//	Name:		SPHERE constructor
//	Object:		
//	05-11-21:	SBE - Created
//-----------------------------------------------------------------------------
SPHERE::SPHERE(int		nNbrPoints,
			   VECTOR *	pPoints)
:
BOUNDING_VOLUME(TYPE_SPHERE)
{
	Compute(nNbrPoints, pPoints);
}

//-----------------------------------------------------------------------------
//	Name:		SPHERE destructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
SPHERE::~SPHERE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
SPHERE& SPHERE::operator=(const SPHERE &C)
{
	_Center  = C._Center;
	SetRadius(C.GetRadius());

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name: SPHERE::Display
//	Object: Display 3 circles representing a 3D sphere. Since tis method is
//			used for debugging, it's not as optimized as it should be...
//	01-06-11:	GGO - Created
//-----------------------------------------------------------------------------
void	SPHERE::Display(const COLOR_FLOAT &	Color,
						int					nStep) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
}

//-----------------------------------------------------------------------------
//	Name:		DisplayFilled
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void	SPHERE::DisplayFilled(const COLOR_FLOAT &	Color,
							  const int				nStep) const
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name: SPHERE::SetFromPoints
//	Object: Creates the bounding sphere of a set of points.
//	01-06-22:	GGO - Created
//-----------------------------------------------------------------------------
void	SPHERE::Compute( int		nNbrPoints,
						 VECTOR *	pPoints )
{
	int i;

	Clear();

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		_Center += pPoints[i];
	}

	_Center /= (Float32)nNbrPoints;

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		Float32 rSquareDistance = DISTANCE::DistanceSquarePointPoint(_Center, pPoints[i]);
		if (rSquareDistance > GetRadius())
		{
			SetRadius(rSquareDistance);
		}
	}

	SetRadius(MATH::Sqrt(GetRadius()));	
}

//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	02-08-19:	JLT - Created
//-----------------------------------------------------------------------------
void SPHERE::Compute(const QDT_VECTOR < VECTOR > &Points)
{
	int i, nNbrPoints = Points.GetSize();

	Clear();

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		_Center += Points[i];
	}

	_Center /= (Float32)nNbrPoints;

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		Float32 rSquareDistance = DISTANCE::DistanceSquarePointPoint(_Center, Points[i]);
		if (rSquareDistance > GetRadius())
		{
			SetRadius(rSquareDistance);
		}
	}

	SetRadius(MATH::Sqrt(GetRadius()));		
}

//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	06-05-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SPHERE::Compute(UInt32			nNbrPoints,
						const void *	pPoints,
						UInt32			nStride)
{
	UInt32	i;
	UInt8 * p = (UInt8 *)pPoints;

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		VECTOR *	pPoint = (VECTOR *)p;

		_Center += *pPoint;

		p += nStride;
	}

	_Center /= (Float32)nNbrPoints;

	p = (UInt8 *)pPoints;

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		VECTOR *	pPoint = (VECTOR *)p;

		Float32 rSquareDistance = DISTANCE::DistanceSquarePointPoint(_Center, *pPoint);
		if (rSquareDistance > GetRadius())
		{
			SetRadius(rSquareDistance);
		}

		p += nStride;
	}

	SetRadius(MATH::Sqrt(GetRadius()));
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-07-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	SPHERE::Read(IO_STREAM &	s)
{
	Float32 r;
	s >> _Center >> r;
	SetRadius(r);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-07-22:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	SPHERE::Write(IO_STREAM &	s) const
{
	s.DebugOutput("Center : ");
	s << _Center;
	s.DebugOutput("\t");
	
	s.DebugOutput("Radius : ");
	s << GetRadius();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	SPHERE::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(SPHERE) );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
void SPHERE::Display(const COLOR_FLOAT &Color) const
{
	Display(Color, 16);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-04-16:	JLT - Created
//-----------------------------------------------------------------------------
bool SPHERE::Intersect(const VECTOR &PIn,
					   VECTOR &POut,
					   const VECTOR &vMotion,
					   const Float32 rInertia,
					   const Float32 rE) const
{
	Float32	rLength, rD = GetRadius() + rE;
	VECTOR	V;
	
	V.Sub(PIn, _Center);

	rLength = V.Normalize();
	if	(rLength < rD)
	{
		V.Mul(rD);
		POut.Add(_Center, V);

		return	(true);
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-01-16	RPA - Created
//-----------------------------------------------------------------------------
void	SPHERE::Transform( const MATRIX & Matrix )
{
	// DAC/ELE: removed because if we need to scale a mesh for ex, we need to have scale in
	// OBB Transform too
	//QDT_ASSERT(Matrix.HasNoScale() == true);

	Matrix.TransformPosition(_Center);
}

//-----------------------------------------------------------------------------
//	Name:		GetRandomPoint
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	SPHERE::GetRandomPoint(VECTOR & Point) const
{
	Float32	x , y , z;
	Float32	rRandRadius;
	Float32	rRandAngle1;
	Float32	rRandAngle2;

	rRandAngle1 = MATH::Rand( MATH::PI() * 2.0f );
	rRandAngle2 = MATH::Rand( MATH::PI() * 2.0f );

	x = MATH::Sin( rRandAngle1 ) * MATH::Cos( rRandAngle2 );
	y = MATH::Cos( rRandAngle1 );
	z = MATH::Sin( rRandAngle1 ) * MATH::Sin( rRandAngle2 );

	rRandRadius = MATH::Rand( GetRadius() );

	VECTOR Rel(x, y, z);

	Point = _Center + Rel * rRandRadius;
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const PLANE &	Plane,
						  bool &		bSide) const
{
	if	(Plane.GetNormal().IsNull())
	{
		bSide = false;
		return (false);
	}

	QDT_ASSERT( Plane.GetNormal().IsNormalized() == true );

	bool bResult;

	Float32 radius = GetRadius();
	Float32 dist = MATH::Abs( DISTANCE::DistancePointNormalizedPlane(Plane, GetCenter()) );

	bResult = dist <= radius;

	if (bResult == false)
	{
		bSide = Plane.GetSide(GetCenter());
	}

	return (bResult);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:	
//  Reference:	http://www.magic - software.com / MgcIntersection.html	
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const RAY &	Ray,
						  VECTOR &		HitPoint) const
{
	static const Float32 rEpsilon = 0.01f;

	// We have to solve a second degree polynom ax² + bx + c
	Float32 rA = Ray.GetDirection().SquareLength();

	Float32 rB = 2.0f * ( Ray.GetDirection().GetX() * (Ray.GetOrigin().GetX() - GetCenter().GetX())
				      + Ray.GetDirection().GetY() * (Ray.GetOrigin().GetY() - GetCenter().GetY())
				      + Ray.GetDirection().GetZ() * (Ray.GetOrigin().GetZ() - GetCenter().GetZ()));

	Float32 rC = MATH::Square(Ray.GetOrigin().GetX() - GetCenter().GetX())
		     + MATH::Square(Ray.GetOrigin().GetY() - GetCenter().GetY())
			 + MATH::Square(Ray.GetOrigin().GetZ() - GetCenter().GetZ())
			 - MATH::Square(GetRadius());

	// det = b² - 4ac
	Float32 rDet = rB * rB - 4.0f * rA * rC;

	// No intersection points
	if (rDet < -rEpsilon)
	{
		return (false);
	}
	else
	// Two possible intersection points
	if (rDet > rEpsilon)
	{
		Float32 rSqrtDet = MATH::Sqrt(rDet);
		Float32 rInv    = 1.0f / (2.0f * rA);

		Float32 r1 = (-rB + rSqrtDet) * rInv;
		Float32 r2 = (-rB - rSqrtDet) * rInv;

		//Two intersection points between the ray and the sphere
		if (r2 >= 0.0f) //and therefore (r1 > 0) too since (r1 > r2)
		{
			HitPoint.SetX(Ray.GetOrigin().GetX() + r2 * Ray.GetDirection().GetX());
			HitPoint.SetY(Ray.GetOrigin().GetY() + r2 * Ray.GetDirection().GetY());
			HitPoint.SetZ(Ray.GetOrigin().GetZ() + r2 * Ray.GetDirection().GetZ());

			return ( true );
		}
		else
		//The origin of the ray is inside the sphere so there is only a single intersection point
		if (r1 >= 0)
		{
			HitPoint.SetX(Ray.GetOrigin().GetX() + r1 * Ray.GetDirection().GetX());
			HitPoint.SetY(Ray.GetOrigin().GetY() + r1 * Ray.GetDirection().GetY());
			HitPoint.SetZ(Ray.GetOrigin().GetZ() + r1 * Ray.GetDirection().GetZ());

			return ( true );
		}
		else
		//The ray doesn't intersect the sphere
		{
			return ( false );
		}
	}
	//Only a single intersection point (the ray is tangent to the sphere)
	else
	{
		Float32 r = -rB / (2.0f * rA);

		//The ray itself intersect the sphere
		if ( r >= 0.0f)
		{
			HitPoint.SetX(Ray.GetOrigin().GetX() + r * Ray.GetDirection().GetX());
			HitPoint.SetY(Ray.GetOrigin().GetY() + r * Ray.GetDirection().GetY());
			HitPoint.SetZ(Ray.GetOrigin().GetZ() + r * Ray.GetDirection().GetZ());

			return (true);
		}
		else
		{
			return (false);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const RAY &	Ray,
						  Float32 &		rDistance) const
{
	VECTOR HitPoint;

	bool bRes = Intersect(Ray, HitPoint);

	if (bRes == true)
	{
		rDistance = DISTANCE::DistancePointPoint(Ray.GetOrigin(), HitPoint);
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		IsOutsideOf
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::IsOutsideOf(const PLANE &	Plane) const
{
	if	(Plane.GetNormal().IsNull()) return (false);

	QDT_ASSERT( Plane.GetNormal().IsNormalized() == true );

	VECTOR vDiff = GetCenter() - Plane.GetPoint(true);

	return (vDiff.Dot(Plane.GetNormal()) >= GetRadius()); 
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const HEXAHEDRON &	Hexahedron) const
{
	if (IsOutsideOf(Hexahedron.GetPlaneNear())  ||
		IsOutsideOf(Hexahedron.GetPlaneFar())   ||
		IsOutsideOf(Hexahedron.GetPlaneLeft())  ||
		IsOutsideOf(Hexahedron.GetPlaneRight()) ||
		IsOutsideOf(Hexahedron.GetPlaneTop())   ||
		IsOutsideOf(Hexahedron.GetPlaneBottom()))
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-07-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const HEXAHEDRON &	Hexahedron,
						  bool &				bIntersectFront) const
{
	bool	bSide;

	// Test is the sphere are outside a plane of the hexahedron.
	if (IsOutsideOf(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		return	(false);
	}
	else
	{
		bIntersectFront = Intersect(Hexahedron.GetPlaneNear(), bSide);
	}

	if (IsOutsideOf(Hexahedron.GetPlaneFar())   ||
		IsOutsideOf(Hexahedron.GetPlaneLeft())  ||
		IsOutsideOf(Hexahedron.GetPlaneRight()) ||
		IsOutsideOf(Hexahedron.GetPlaneTop())   ||
		IsOutsideOf(Hexahedron.GetPlaneBottom()))
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const HEXAHEDRON &	Hexahedron,
						  bool &				bInside,
						  bool &				bIntersectFront) const
{
	bool	bIntersect, bSide;

	if (IsOutsideOf(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;

		bInside = false;
		return	(false);
	}
	else
	{
		bIntersect = Intersect(Hexahedron.GetPlaneNear(), bSide);
		bIntersectFront = bIntersect;
	}

	if (IsOutsideOf(Hexahedron.GetPlaneLeft()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneLeft(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneRight()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneRight(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneTop()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneTop(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneBottom()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneBottom(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneFar()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneFar(), bSide);
		}
	}

	bInside = !bIntersect;
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	SPHERE::IntersectFront(const HEXAHEDRON &	Hexahedron,
							   bool &				bIntersectFront) const
{
	bool	bSide;

	// Test is the sphere are outside a plane of the hexahedron.
	if (IsOutsideOf(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		return	(false);
	}
	else
	{
		int	nSide;

		Hexahedron.GetPlaneNear().GetSide(_Center, nSide, 1.0f);
		if	(nSide <= 0)
		{
			bIntersectFront = Intersect(Hexahedron.GetPlaneNear(), bSide);
		}
		else
		{
			bIntersectFront = false;
			return	(false);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneFar())   ||
		IsOutsideOf(Hexahedron.GetPlaneLeft())  ||
		IsOutsideOf(Hexahedron.GetPlaneRight()) ||
		IsOutsideOf(Hexahedron.GetPlaneTop())   ||
		IsOutsideOf(Hexahedron.GetPlaneBottom()))
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	SPHERE::IntersectFront(const HEXAHEDRON &	Hexahedron,
							   bool &				bInside,
							   bool &				bIntersectFront) const
{
	bool	bIntersect, bSide;

	if (IsOutsideOf(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;

		bInside = false;
		return	(false);
	}
	else
	{
		int	nSide;

		Hexahedron.GetPlaneNear().GetSide(_Center, nSide, 1.0f);
		if	(nSide <= 0)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneNear(), bSide);
			bIntersectFront = bIntersect;
		}
		else
		{
			bIntersectFront = false;
			return	(false);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneLeft()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneLeft(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneRight()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneRight(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneTop()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneTop(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneBottom()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneBottom(), bSide);
		}
	}

	if (IsOutsideOf(Hexahedron.GetPlaneFar()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneFar(), bSide);
		}
	}

	bInside = !bIntersect;
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const FRUSTRUM &	Frustrum) const
{
	QDT_ASSERT(Frustrum.GetFrontDirection().IsNormalized() == true );
	QDT_ASSERT(Frustrum.GetLeftDirection().IsNormalized() == true );
	QDT_ASSERT(Frustrum.GetTopDirection().IsNormalized() == true );

	VECTOR vDiff = GetCenter() - Frustrum.GetOrigin();
	VECTOR vTemp;
	Float32 rX_TransformedCenter; // Coordinate of the sphere center
	Float32 rY_TransformedCenter; // in the frustrum coordinate
	Float32 rZ_TransformedCenter; // system.
	Float32 rSqrRadius;
	Float32 rSqrNear;
	Float32 rDotProduct;

	Float32 rFar  = Frustrum.GetFar();
	Float32 rNear = Frustrum.GetNear();
	Float32 rLeft = Frustrum.GetLeft();
	Float32 rTop  = Frustrum.GetTop();

	// Test along the main direction of the frustrum
	rX_TransformedCenter = vDiff.Dot(Frustrum.GetFrontDirection());

	if ((rX_TransformedCenter >= rFar  + GetRadius()) ||
		(rX_TransformedCenter <= rNear - GetRadius()))
	{
		return ( false );
	}

	// Optis
	rSqrRadius = GetRadius() * GetRadius();
	rSqrNear   = rNear   * rNear;

	// Test along the left direction of the frustrum.
	rY_TransformedCenter = vDiff.Dot(Frustrum.GetLeftDirection());
	rY_TransformedCenter = MATH::Abs(rY_TransformedCenter);
	rDotProduct			 = rY_TransformedCenter * rNear - rX_TransformedCenter * rLeft;

	if ( ( rDotProduct > 0.0f ) &&
		 ( rDotProduct * rDotProduct >= rSqrRadius * ( rSqrNear + rLeft * rLeft ) ) )
	{
		return ( false );
	}

	// Test along the top direction of the frustrum.
	rZ_TransformedCenter = vDiff.Dot(Frustrum.GetTopDirection());
	rZ_TransformedCenter = MATH::Abs(rZ_TransformedCenter);
	rDotProduct          = rZ_TransformedCenter * rNear - rX_TransformedCenter * rTop;

	if ((rDotProduct > 0.0f) &&
		(rDotProduct * rDotProduct >= rSqrRadius * (rSqrNear + rTop * rTop )))
	{
		return (false);
	}
	
	return (true);

/*	// But we need to perform more accurate tests to know if the sphere really intersects
	// or not the frustrum.

	// Optis.
	rRatio =	   Frustrum.GetFar() / Frustrum.GetNear();
	rLeftByRatio = rLeft * rRatio;
	rTopByRatio  = rTop  * rRatio;


	// Distance between sphere center and far top edge.
	bool bZ_LargerTopByRatio  = rZ_TransformedCenter > rTopByRatio;
	bool bY_LargerLeftByRatio = rY_TransformedCenter > rLeftByRatio;

	if ( ( bZ_LargerTopByRatio  == true  ) &&
		 ( bY_LargerLeftByRatio == false ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rFar ) +
			          rTop  * ( rZ_TransformedCenter - rTopByRatio );

		if ( rDotProduct >= 0.0f  )
		{
			rSqrDistance = MATH::Square( rZ_TransformedCenter - rTopByRatio ) +
						   MATH::Square( rX_TransformedCenter - rFar );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}

	// Distance between sphere center and near top edge.
	bool bZ_LargerTop  = rZ_TransformedCenter > rTop;
	bool bY_LargerLeft = rY_TransformedCenter > rLeft;

	if ( ( bZ_LargerTop  == true  ) &&
		 ( bY_LargerLeft == false ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rNear ) +
			          rTop  * ( rZ_TransformedCenter - rTop );

		if ( rDotProduct <= 0.0f )
		{
			rSqrDistance = MATH::Square( rZ_TransformedCenter - rTop ) +
						   MATH::Square( rX_TransformedCenter - rNear );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}

	// Distance between sphere center and far left edge.
	if ( ( bZ_LargerTopByRatio  == false ) &&
		 ( bY_LargerLeftByRatio == true  ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rFar ) +
			          rLeft * ( rY_TransformedCenter - rLeftByRatio );

		if ( rDotProduct >= 0.0f )
		{
			rSqrDistance = MATH::Square( rY_TransformedCenter - rLeftByRatio ) +
						   MATH::Square( rX_TransformedCenter - rFar );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}

	// Distance between sphere center and near left edge.
	if ( ( bZ_LargerTop  == false ) &&
		 ( bY_LargerLeft == true  ) ) 
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rNear ) + 
			          rLeft * ( rY_TransformedCenter - rLeft );

		if ( rDotProduct <= 0.0f )
		{
			rSqrDistance = MATH::Square( rY_TransformedCenter - rLeft ) +
						   MATH::Square( rX_TransformedCenter - rNear );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}

	// Distance between sphere center and top left far vertex.
	if ( ( bZ_LargerTopByRatio  == true ) &&
		 ( bY_LargerLeftByRatio == true ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rFar ) +
			          rLeft * ( rY_TransformedCenter - rLeftByRatio ) +
				      rTop  * ( rZ_TransformedCenter - rTopByRatio );

		if ( rDotProduct >= 0.0f )
		{
			rSqrDistance = MATH::Square( rX_TransformedCenter - rFar ) + 
						   MATH::Square( rY_TransformedCenter - rLeftByRatio ) +
						   MATH::Square( rZ_TransformedCenter - rTopByRatio );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}

	// Distance between sphere center and top left near vertex.
	if ( ( bZ_LargerTop  == true ) &&
		 ( bY_LargerLeft == true ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rNear ) +
					  rLeft * ( rY_TransformedCenter - rLeft ) +
					  rTop  * ( rZ_TransformedCenter - rTop );

		if ( rDotProduct <= 0.0f )
		{
			rSqrDistance = MATH::Square( rX_TransformedCenter - rNear ) + 
						   MATH::Square( rY_TransformedCenter - rLeft ) +
						   MATH::Square( rZ_TransformedCenter - rTop );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and top left edge (the worst case...).
	if ( ( rNear * rY_TransformedCenter - rLeft * rX_TransformedCenter >= 0.0f ) &&
		 ( rNear * rZ_TransformedCenter - rTop  * rX_TransformedCenter >= 0.0f ) )
	{
		rA        = rY_TransformedCenter * rNear / rLeft - rX_TransformedCenter;
		rB        = rY_TransformedCenter - rX_TransformedCenter * rLeft / rNear;
		rSqrDist1 = MATH::Square( rA * rB ) / ( rA * rA + rB * rB ); // Heigth h of a rectangular triangle whose sides are a and b: h = ab/(sqrt(aa+bb))

		rA        = rZ_TransformedCenter * rNear / rTop - rX_TransformedCenter;
		rB        = rZ_TransformedCenter - rX_TransformedCenter * rTop / rNear;
		rSqrDist2 = MATH::Square( rA * rB ) / ( rA * rA + rB * rB ); // Heigth h of a rectangular triangle whose sides are a and b: h = ab/(sqrt(aa+bb))

		rSqrDistance = rSqrDist1 + rSqrDist2;


		if ( rSqrDistance > rSqrRadius )
		{
			return ( false );
		}
	}


	QDT_ASSERT(nSpeed == 0);

	return (true);*/
}

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR &	SPHERE::GetCenter() const
{
	return (_Center);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME &	SPHERE::operator=(const BOUNDING_VOLUME &	BoundingVolume)
{
	QDT_ASSERT(BoundingVolume.GetType() == GetType());
	return (*this = static_cast<const SPHERE &>(BoundingVolume));
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const CONE &	Cone) const
{
	// If cone has a radius
	if (Cone.GetRadius() > 0.0f)
	{
		if (DISTANCE::DistanceSquarePointPoint(Cone.GetTop(), GetCenter()) > MATH::Square(Cone.GetRadius() + GetRadius()))
		{
			return (false);
		}
	}

	// test if cone top point is in sphere
	VECTOR	vDiff = GetCenter() - Cone.GetTop();
	Float32	rRSqr = MATH::Square(GetRadius());
	Float32	rLSqr = vDiff.SquareLength();
	if (rLSqr < rRSqr)
	{
		return (true);
	}

	// Test if sphere center is in cone
	Float32 rDot = vDiff.Dot(Cone.GetAxe());
	Float32 rDotSqr = MATH::Square(rDot);
	Float32 rCosSqr = MATH::Square(Cone.GetCosAngle());
	if ((rDotSqr >= (rLSqr * rCosSqr)) && (rDot > 0.0f))
	{
		// sphere center is inside cone, so sphere and cone intersect
        return true;
	}

    // Sphere center is outside cone.  Problem now reduces to looking for
    // an intersection between circle and ray in the plane containing
    // cone vertex and spanned by cone axis and vector from vertex to
    // sphere center.

    // Ray is t*D+V (t >= 0) where |D| = 1 and dot(A,D) = cos(angle).
    // Also, D = e*A+f*(C-V).  Plugging ray equation into sphere equation
    // yields R^2 = |t*D+V-C|^2, so the quadratic for intersections is
    // t^2 - 2*dot(D,C-V)*t + |C-V|^2 - R^2 = 0.  An intersection occurs
    // if and only if the discriminant is nonnegative.  This test becomes
    //
    //     dot(D,C-V)^2 >= dot(C-V,C-V) - R^2
    //
    // Note that if the right-hand side is nonpositive, then the inequality
    // is true (the sphere contains V).  I have already ruled this out in
    // the first block of code in this function.

	Float32 rULen = MATH::Sqrt(MATH::Abs(rLSqr - rDotSqr));
	Float32 rTest = Cone.GetCosAngle() * rDot + Cone.GetSinAngle() * rULen;
	Float32 rDiscr = MATH::Square(rTest) - rLSqr + rRSqr;

    return ((rDiscr >= 0.0f) && (rTest >= 0.0f));
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
Float32	SPHERE::GetSize() const
{
	return (GetRadius() * GetRadius());
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const SPHERE &	Sphere) const
{
	VECTOR vDistance = (GetCenter() - Sphere.GetCenter());
	Float32 rSquaredLength = vDistance.SquareLength();
	Float32 rSquareRadius1 = GetRadius() * GetRadius();
	Float32 rSquareRadius2 = Sphere.GetRadius() * Sphere.GetRadius();

	return (rSquaredLength < rSquareRadius1 + rSquareRadius2);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const AABB &	Aabb) const
{
	if (GetCenter().GetX() - GetRadius() > Aabb.GetXMax()) return (false);
	if (GetCenter().GetY() - GetRadius() > Aabb.GetYMax()) return (false);
	if (GetCenter().GetZ() - GetRadius() > Aabb.GetZMax()) return (false);

	if (GetCenter().GetX() + GetRadius() < Aabb.GetXMin()) return (false);
	if (GetCenter().GetY() + GetRadius() < Aabb.GetYMin()) return (false);
	if (GetCenter().GetZ() + GetRadius() < Aabb.GetZMin()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
void	SPHERE::Clear()
{
	_Center = VECTOR::ZERO;
	SetRadius(-QDT_FLOAT32_MAX);
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-12-05:	JLT - Created
//-----------------------------------------------------------------------------
bool SPHERE::Grow(const SPHERE &S)
{
	if (GetRadius() == -QDT_FLOAT32_MAX && this != &S)
	{
		*this = S;
	}
	else
	{
		VECTOR V = S._Center - _Center;

		Float32 L = V.Normalize();

		VECTOR P = _Center + (V*(L+S.GetRadius()));

		Grow(P);
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	02-12-05:	JLT - Created
//-----------------------------------------------------------------------------
bool SPHERE::Grow(const VECTOR &P)
{
	if (GetRadius() == -QDT_FLOAT32_MAX)
	{
		_Center = P;
		SetRadius(0.0f);
	}
	else
	{
		VECTOR V = P - _Center;
		Float32 L2 = V.SquareLength();
		Float32 R2 = MATH::Square(GetRadius());

		if (L2 > R2)
		{
			Float32 L = MATH::Sqrt(L2);
			Float32 DL = (L - GetRadius()) * 0.5f;
			_Center = _Center + (V * (DL / L));
			SetRadius(GetRadius() + DL);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(VERTICAL_RAY &	VerticalRay) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	SPHERE::Intersect(const VECTOR &	Point) const
{
	return (DISTANCE::DistanceSquarePointPoint(GetCenter(), Point) <= MATH::Square(GetRadius())) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetOptimalFocalAngle
//	Object:		
//	03-04-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	SPHERE::GetOptimalFocalAngle(const VECTOR &	Pos,
									 Float32				rAngle,
									 Float32 &			rNear,
									 Float32 &			rIntersectNear,
									 const VECTOR &		FrontDir) const
{
	Float32	rDistance, rMinDistance, rDot, rOpeningAngle, rDiffAngle;
	VECTOR	V;

	V.SetFromPoints(Pos, _Center);
	rDistance = V.Normalize();

	if	(rDistance < GetRadius())
	{
		return	(0.0f);
	}

	rMinDistance = rDistance - GetRadius();

	rDot = FrontDir.Dot(V);
	rNear = rMinDistance * rDot;
	rIntersectNear = rDistance * rDot;

	rOpeningAngle = MATH::ASin(GetRadius() / rDistance);
	rDiffAngle = MATH::ACos(rDot);

	if	(rAngle == 0.0f)
	{
		return	((rOpeningAngle + rDiffAngle) * 2.0f);
	}
	else
	{
		if	(rAngle + rOpeningAngle > rDiffAngle)
		{
			return	((rOpeningAngle + rDiffAngle) * 2.0f);
		}
		else
		{
			return	(0.0f);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Extend
//	Object:		
//	04-10-04:	RMA - Created
//-----------------------------------------------------------------------------
void	SPHERE::Extend(Float32	r)
{
	_Center.SetPad(_Center.GetPad() + r);
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	06-01-30:	ELE - Created
//-----------------------------------------------------------------------------
void	SPHERE::Set(const CONE &	Cone)
{
	Float32	R = Cone.GetRadius();

	_Center.Add(Cone.GetTop(), Cone.GetAxe() * (R * 0.5f));
	_Center.SetPad(MATH::Sqrt(MATH::Square(R * 0.5f) + MATH::Square(Cone.GetSinAngle() * R)));
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	06-05-29:	ELE - Created
//-----------------------------------------------------------------------------
void	SPHERE::Set(const AABB &	BoundingAABB)
{
	_Center = BoundingAABB.GetCenter();

	VECTOR	vMin, vMax;

	vMin.Sub(BoundingAABB.GetMin(), _Center);
	vMax.Sub(BoundingAABB.GetMax(), _Center);

	Float32	rRadius1 = vMin.Length();
	Float32	rRadius2 = vMax.Length();
	Float32	rRadius = MATH::Max(rRadius1, rRadius2);

	SetRadius(rRadius);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
