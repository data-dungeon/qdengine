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
// CLASS: DISTANCE
//
//
//00-12-20 RMA Created: 
//*****************************************************************************

#include "Root.h"
#include INCL_KMATH(Math\Geometry\Distance\Distance)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Ray\Ray)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Line\Line)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Vector\Vector)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes2D\Plane\Plane)
#include INCL_KMATH(Math\Geometry\Vector2\Vector2)

#ifdef _DEBUG
#include "distance.inl"
#endif

//-----------------------------------------------------------------------------
// Name: DISTANCE::DistanceSquareRayPoint
// Object: This method implements the calcul of the square of the distance
//		   between a ray & a point. 
// 00-12-21 RMA Created: 
// 01-02-09 GGO Revisited and optimized.
// 02-02-21 RMA Redone because it doesn't work. Please test it before modifying
//			it.
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistanceSquareRayPoint(const RAY &		Ray,
									   const VECTOR &	Point)
{
	VECTOR	Diff;
	Float32	rT ;
	
	Diff.Sub(Point, Ray.GetOrigin());

	rT = Diff.Dot(Ray.GetDirection()) ;

	if (rT >= 0.0f)
	{
		rT /= Ray.GetDirection().SquareLength();
		Diff -= rT * Ray.GetDirection();
	}

	return (Diff.SquareLength());
}

//-----------------------------------------------------------------------------
//	Name: DISTANCE::DistancePointPlane
//	Object: Distance between a plane and a point.
//			The distance is signed: it means that the sign of the distance changes
//			whereas the point is on the 'left' or on the 'right' side of the plane.
//			This mean that the distance can be negative too.
//	01-02-09:	GGO - Created
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistancePointPlane(const PLANE &	Plane,
								   const VECTOR &	Point)
{
	Float32 rD = Plane.GetD();
	
	VECTOR vNormal;
	VECTOR vVect(Point);
	vNormal.Set(Plane.GetNormal());

	Float32 r = ((vNormal.Dot(vVect) + rD) / vNormal.Length());

	return r;
}

//-----------------------------------------------------------------------------
//	Name: DISTANCE::DistancePointNormalizedPlane
//	Object: Distance between a plane and a point.
//			The distance is signed: it means that the sign of the distance changes
//			whereas the point is on the 'left' or on the 'right' side of the plane.
//	01-02-09:	GGO - Created
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistancePointNormalizedPlane(const PLANE &		NormalizedPlane,
											 const VECTOR &	Point)
{
	QDT_ASSERT( NormalizedPlane.IsNormalNormalized() == true );

	Float32 rD = NormalizedPlane.GetD();
	
	VECTOR vNormal;
	VECTOR vVect(Point);
	vNormal.Set(NormalizedPlane.GetNormal());

	Float32 r = (vNormal.Dot(vVect) + rD);

	return r;
}

//-----------------------------------------------------------------------------
//	Name: DISTANCE::DistancePointLine
//	Object: Distance squared between a point and a line.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistanceSquarePointLine(const VECTOR &	Point,
										const LINE &	Line)
{
	QDT_ASSERT(Line.GetDirection().IsNormalized() == true);

	VECTOR vVector( Line.GetPoint().GetX() - Point.GetX(),
					Line.GetPoint().GetY() - Point.GetY(),
					Line.GetPoint().GetZ() - Point.GetZ());

	VECTOR vCross;
	vCross.Cross(Line.GetDirection(), vVector);

	return (vCross.SquareLength());
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
