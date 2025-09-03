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
// CLASS: INTERSECTION
//
//
//00 - 11 - 09 JVA Created:
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)	
#include	INCL_KMATH(Math/Geometry/Intersection/IntersectionFrustrum)
#include	INCL_KMATH(Math/Geometry/Intersection/IntersectionHexahedron)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Cone/Cone)	
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)

#ifdef _DEBUG
#include	"Intersection.inl"
#endif

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectRayWithTriangle
// Object:  Find the intersection between a ray and a triangle. The reference '&rDistance'
//		    receives the distance between the origin of the ray and the intersectin point.
// 00-11-10 RMA Created:
// 01-02-12 GGO Added comments and optimized.
// 01-01-04	RMA Please don't try to optimize this without tests!!!!
// 01-06-26 RMA Savage optimization after code stabilisation
//-----------------------------------------------------------------------------
bool	INTERSECTION::FindIntersectionRayTriangle( const RAY			& Ray,
												   const TRIANGLE		& Triangle,
												   Float32				& rDistance )
{
	// The main idea of the test is to express the problem in the canonic base.
	// The origin of the ray is expressed in the canonic base. There, the 
	// coeffs of the origin of the ray should meet some conditions.
	Float32   ru, rv, rw;
	VECTOR  vT;	// No need to initialize
	VECTOR	vQ;
	VECTOR	vP;
	Float32	rDet;
	Float32	rInvDet ;

	// Find vectors for two edge sharing vertex 0.
	VECTOR vEdge1 = Triangle.GetPoint1() - Triangle.GetPoint0() ;
	VECTOR vEdge2 = Triangle.GetPoint2() - Triangle.GetPoint0() ;

	// Begin to calculate determinant, also used to calculate u parameter
	vP.Cross(Ray.GetDirection() , vEdge2);
	rDet = vEdge1.Dot(vP);

	// If determinant is near zero, ray lies in plane of triangle.	
	if ( MATH::Abs( rDet ) < MATH::EPSILON() )
	{
		return (false);
	}

	// Create a vector from vertex 0 to the origin of the ray
	vT = Ray.GetOrigin() - Triangle.GetPoint0() ;

	// The projection of the vector vT along the first axis
	rInvDet = 1.0f / rDet ;
	ru = vT.Dot( vP ) * rInvDet;
	if ( ( ru < 0.0f ) || ( ru > 1.0f) )
	{
		return (false);
	}

	// The sum of the projection of the vector vT along the two first axis
	vQ.Cross( vT , vEdge1 );
	rv = Ray.GetDirection().Dot( vQ ) * rInvDet;
	if ((rv < 0.0f) || ((ru + rv) > 1.0f))
	{
		return (false);
	}

	// And finally, the projection on the last axis should be negative.
	rw = vEdge2.Dot(vQ) * rInvDet;
	if (rw < MATH::EPSILON())
	{
		return (false);
	}
	else
	{
		rDistance = rw ;
		return (true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersectionSegmentTriangle
//	Object:		
//	01-11-19:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionSegmentTriangle(const SEGMENT &	Segment,
													  const TRIANGLE &	Triangle)
{
	RAY Ray;
	Ray.SetOrigin(Segment.GetPoint1());

	VECTOR RayDir = Segment.GetPoint2() - Segment.GetPoint1();
	RayDir.Normalize();
	Ray.SetDirection(RayDir);

	Float32	rDistance;
	bool	bRes = FindIntersectionRayTriangle(Ray, Triangle, rDistance);

	if (bRes)
	{
		if (rDistance < Segment.Length())
		{
			return (true);
		}
	}
	
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionSegmentAABB
//	Object:		
//	01-11-19:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionSegmentAABB(const SEGMENT &	Segment,
												  const AABB &		Aabb)
{
	QDT_NOT_IMPLEMENTED();
	
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionPointAABB
//	Object:		
//	02-02-13:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionPointAABB(const VECTOR &	Point,
												const AABB &	Bbox)
{
	if (Point.GetX() >= Bbox.GetXMax()) return (false);
	if (Point.GetX() <= Bbox.GetXMin()) return (false);
	if (Point.GetY() >= Bbox.GetYMax()) return (false);
	if (Point.GetY() <= Bbox.GetYMin()) return (false);
	if (Point.GetZ() >= Bbox.GetZMax()) return (false);
	if (Point.GetZ() <= Bbox.GetZMin()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionPointAABB
//	Object:		
//	02-05-23:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionPointAABB(const VECTOR &	Point,
												const AABB &	Bbox,
												Float32			rEpsilon)
{
	if (Point.GetX() - rEpsilon >= Bbox.GetXMax()) return (false);
	if (Point.GetX() + rEpsilon <= Bbox.GetXMin()) return (false);
	if (Point.GetY() - rEpsilon >= Bbox.GetYMax()) return (false);
	if (Point.GetY() + rEpsilon <= Bbox.GetYMin()) return (false);
	if (Point.GetZ() - rEpsilon >= Bbox.GetZMax()) return (false);
	if (Point.GetZ() + rEpsilon <= Bbox.GetZMin()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersectionRayTriangleWithCulling
//	Object:		This a variation of the previous method, but this time it uses
//				culling so triangle not orientend in front of the ray won't
//				collide...
//	01-09-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::FindIntersectionRayTriangleWithCulling(const RAY &		Ray,
															 const TRIANGLE &	Triangle,
															 Float32 &			rDistance)
{
	Float32   ru, rv, rw;
	VECTOR  vT;
	VECTOR	vQ;
	VECTOR	vP;
	Float32	rDet;
	Float32	rInvDet ;

	// Find vectors for two edge sharing vertex 0.
	VECTOR vEdge1 = Triangle.GetPoint1() - Triangle.GetPoint0() ;
	VECTOR vEdge2 = Triangle.GetPoint2() - Triangle.GetPoint0() ;

	// Begin to calculate determinant, also used to calculate u parameter
	vP.Cross(Ray.GetDirection() , vEdge2);
	rDet = vEdge1.Dot(vP);

	if (rDet < MATH::EPSILON())
	{
		return 0;
	}

	// Create a vector from vertex 0 to the origin of the ray
	vT = Ray.GetOrigin() - Triangle.GetPoint0() ;

	// The projection of the vector vT along the first axis
	ru = vT.Dot( vP );
	if ( ( ru < 0.0f ) || ( ru > rDet) )
	{
		return (false);
	}

	vQ.Cross( vT , vEdge1 );

    // calculate rV parameter and test bounds
	rv = Ray.GetDirection().Dot(vQ);
	if (rv < 0.0 || ru + rv > rDet)
	{
		return (false);
	}

	// And finally, the projection on the last axis should be negative.
	rw = vEdge2.Dot(vQ);
	rInvDet = 1.0f / rDet;
	rw *= rInvDet;
	if (rw < MATH::EPSILON())
	{
		return (false);
	}
	else
	{
		rDistance = rw;
		return (true);
	}
}

//-----------------------------------------------------------------------------
//	Name: INTERSECTION::FindIntersectionLineSphere
//	Object: Fint the intersection (if they exit) between a line and a sphere.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::FindIntersectionLineSphere(const LINE &		Line,
												 const SPHERE &		Sphere,
												 int &				nNbrHitPoints,
												 VECTOR &			HitPoint1,
												 VECTOR &			HitPoint2)
{
	//We have to solve a second degree polynom ax² + bx + c
	Float32 rA = Line.GetDirection().SquareLength();

	Float32 rB = 2.0f * ( Line.GetDirection().GetX() * (Line.GetPoint().GetX() - Sphere.GetCenter().GetX())
				      + Line.GetDirection().GetY() * (Line.GetPoint().GetY() - Sphere.GetCenter().GetY())
				      + Line.GetDirection().GetZ() * (Line.GetPoint().GetZ() - Sphere.GetCenter().GetZ()) );

	Float32 rC = MATH::Square( Line.GetPoint().GetX() - Sphere.GetCenter().GetX() )
		     + MATH::Square( Line.GetPoint().GetY() - Sphere.GetCenter().GetY() )
			 + MATH::Square( Line.GetPoint().GetZ() - Sphere.GetCenter().GetZ() )
			 - MATH::Square( Sphere.GetRadius() );

	//det = b² - 4ac
	Float32 rDet = rB * rB - 4.0f * rA * rC;

	//No intersection points
	if ( rDet < -MATH::EPSILON() )
	{
		nNbrHitPoints = 0;
		return ( false );
	}
	else
	//Two possible intersection points
	if ( rDet > MATH::EPSILON() )
	{
		Float32 rSqrtDet = MATH::Sqrt( rDet );
		Float32 rInv    = 1.0f / (2.0f * rA);

		Float32 r1 = (-rB + rSqrtDet) * rInv;
		Float32 r2 = (-rB - rSqrtDet) * rInv;

		nNbrHitPoints = 2;

		HitPoint1.SetX( Line.GetPoint().GetX() + r1 * Line.GetDirection().GetX() );
		HitPoint1.SetY( Line.GetPoint().GetY() + r1 * Line.GetDirection().GetY() );
		HitPoint1.SetZ( Line.GetPoint().GetZ() + r1 * Line.GetDirection().GetZ() );

		HitPoint2.SetX( Line.GetPoint().GetX() + r2 * Line.GetDirection().GetX() );
		HitPoint2.SetY( Line.GetPoint().GetY() + r2 * Line.GetDirection().GetY() );
		HitPoint2.SetZ( Line.GetPoint().GetZ() + r2 * Line.GetDirection().GetZ() );

		return ( true );
	}
	//Only a single intersection point (the ray is tangent to the sphere)
	else
	{
		Float32 r = -rB / (2.0f * rA);

		nNbrHitPoints = 1;

		HitPoint1.SetX( Line.GetPoint().GetX() + r * Line.GetDirection().GetX() );
		HitPoint1.SetY( Line.GetPoint().GetY() + r * Line.GetDirection().GetY() );
		HitPoint1.SetZ( Line.GetPoint().GetZ() + r * Line.GetDirection().GetZ() );

		return true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		INTERSECTION::TestIntersectionPointTriangle
//	Object:		Check if a point is inside a triangle or not.
//				This method is not opimized because it's used only in the export
//				of scene.
//				If you use it in the engine or the kernel, please try to optimize
//				it.
//	01-04-02:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionPointTriangle(const VECTOR & Point, const TRIANGLE & Triangle)
{
	PLANE Plane(Triangle);

	if (!Plane.IsOn(Point))
	{
		return (false);
	}

	Float32 rA, rB, rC, rD;
	Plane.GetCoeff(rA, rB, rC, rD);

	rA = MATH::Abs(rA);
	rB = MATH::Abs(rB);
	rC = MATH::Abs(rC);

	REFERENCE_PLANE RefPlane = PLANE_YZ;

	// Get largest of the maxT's for final choice of intersection
	if (rB > rA)
	{
		RefPlane = PLANE_XZ;

		if (rC > rB)
		{
			RefPlane = PLANE_XY;
		}
	}
	else
	{
		if (rC > rB)
		{
			RefPlane = PLANE_XY;
		}
	}

	POINT2D Edge0;
	POINT2D Edge1;
	POINT2D Edge2;

	POINT2D Vector0;
	POINT2D Vector1;
	POINT2D Vector2;

	switch (RefPlane)
	{
		default:
			QDT_FAIL();
		
		case PLANE_YZ :
			{
				// We make abstraction of the X component

				Edge0.SetX(Triangle.GetPoint1().GetY() - Triangle.GetPoint0().GetY());
				Edge0.SetY(Triangle.GetPoint1().GetZ() - Triangle.GetPoint0().GetZ());
				Edge1.SetX(Triangle.GetPoint2().GetY() - Triangle.GetPoint1().GetY());
				Edge1.SetY(Triangle.GetPoint2().GetZ() - Triangle.GetPoint1().GetZ());
				Edge2.SetX(Triangle.GetPoint0().GetY() - Triangle.GetPoint2().GetY());
				Edge2.SetY(Triangle.GetPoint0().GetZ() - Triangle.GetPoint2().GetZ());

				Vector0.SetX(Triangle.GetPoint0().GetY() - Point.GetY());
				Vector0.SetY(Triangle.GetPoint0().GetZ() - Point.GetZ());
				Vector1.SetX(Triangle.GetPoint1().GetY() - Point.GetY());
				Vector1.SetY(Triangle.GetPoint1().GetZ() - Point.GetZ());
				Vector2.SetX(Triangle.GetPoint2().GetY() - Point.GetY());
				Vector2.SetY(Triangle.GetPoint2().GetZ() - Point.GetZ());
				
				break;
			}

		case PLANE_XZ :
			{
				// We make abstraction of the Y component

				Edge0.SetX(Triangle.GetPoint1().GetX() - Triangle.GetPoint0().GetX());
				Edge0.SetY(Triangle.GetPoint1().GetZ() - Triangle.GetPoint0().GetZ());
				Edge1.SetX(Triangle.GetPoint2().GetX() - Triangle.GetPoint1().GetX());
				Edge1.SetY(Triangle.GetPoint2().GetZ() - Triangle.GetPoint1().GetZ());
				Edge2.SetX(Triangle.GetPoint0().GetX() - Triangle.GetPoint2().GetX());
				Edge2.SetY(Triangle.GetPoint0().GetZ() - Triangle.GetPoint2().GetZ());

				Vector0.SetX(Triangle.GetPoint0().GetX() - Point.GetX());
				Vector0.SetY(Triangle.GetPoint0().GetZ() - Point.GetZ());
				Vector1.SetX(Triangle.GetPoint1().GetX() - Point.GetX());
				Vector1.SetY(Triangle.GetPoint1().GetZ() - Point.GetZ());
				Vector2.SetX(Triangle.GetPoint2().GetX() - Point.GetX());
				Vector2.SetY(Triangle.GetPoint2().GetZ() - Point.GetZ());

				break;
			}

		case PLANE_XY :
			{
				// We make abstreaction of the Z component

				Edge0.SetX(Triangle.GetPoint1().GetX() - Triangle.GetPoint0().GetX());
				Edge0.SetY(Triangle.GetPoint1().GetY() - Triangle.GetPoint0().GetY());
				Edge1.SetX(Triangle.GetPoint2().GetX() - Triangle.GetPoint1().GetX());
				Edge1.SetY(Triangle.GetPoint2().GetY() - Triangle.GetPoint1().GetY());
				Edge2.SetX(Triangle.GetPoint0().GetX() - Triangle.GetPoint2().GetX());
				Edge2.SetY(Triangle.GetPoint0().GetY() - Triangle.GetPoint2().GetY());

				Vector0.SetX(Triangle.GetPoint0().GetX() - Point.GetX());
				Vector0.SetY(Triangle.GetPoint0().GetY() - Point.GetY());
				Vector1.SetX(Triangle.GetPoint1().GetX() - Point.GetX());
				Vector1.SetY(Triangle.GetPoint1().GetY() - Point.GetY());
				Vector2.SetX(Triangle.GetPoint2().GetX() - Point.GetX());
				Vector2.SetY(Triangle.GetPoint2().GetY() - Point.GetY());

				break;
			}
	}

	Float32 Res0 = Edge0.GetX() * Vector0.GetY() - Edge0.GetY() * Vector0.GetX();
	Float32 Res1 = Edge1.GetX() * Vector1.GetY() - Edge1.GetY() * Vector1.GetX();
	Float32 Res2 = Edge2.GetX() * Vector2.GetY() - Edge2.GetY() * Vector2.GetX();

	if ((Res0 >= 0) && (Res1 >= 0)  && (Res2 >= 0))
	{
		return (true);
	}

	if ((Res0 <= 0) && (Res1 <= 0)  && (Res2 <= 0))
	{
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionHexahedronPoint
//	Object:		
//	02-04-29:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionHexahedronPoint(const HEXAHEDRON &	Hexahedron,
													  const VECTOR &		Point)
{
	return (INTERSECTION_HEXAHEDRON::TestIntersectionHexahedronPoint(Hexahedron, Point));
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionFrustrumPoint
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionFrustrumPoint(const FRUSTRUM &	Frustrum,
													const VECTOR &		Point)
{
	return (INTERSECTION_FRUSTRUM::TestIntersectionFrustrumPoint(Frustrum, Point));
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersectionSphereAABB
//	Object:		Sources & Explanations : 
//				http://www.gamasutra.com/features/19991018/Gomez_4.htm
//	01-06-06:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionSphereAABB(const SPHERE &	Sphere,
												 const AABB	  & Aabb)
{
	Float32 rS, rD = 0;

	// find the square of the distance from the sphere to the box

	if (Sphere.GetCenter().GetX() < Aabb.GetXMin())
	{
		rS = Sphere.GetCenter().GetX() - Aabb.GetXMin();
		rD += MATH::Square(rS);
	}
	else if (Sphere.GetCenter().GetX() > Aabb.GetXMax())
	{
		rS = Sphere.GetCenter().GetX() - Aabb.GetXMax();
		rD += MATH::Square(rS);
	}

	if (Sphere.GetCenter().GetY() < Aabb.GetYMin())
	{
		rS = Sphere.GetCenter().GetY() - Aabb.GetYMin();
		rD += MATH::Square(rS);
	}
	else if (Sphere.GetCenter().GetY() > Aabb.GetYMax())
	{
		rS = Sphere.GetCenter().GetY() - Aabb.GetYMax();
		rD += MATH::Square(rS);
	}

	if (Sphere.GetCenter().GetZ() < Aabb.GetZMin())
	{
		rS = Sphere.GetCenter().GetZ() - Aabb.GetZMin();
		rD += MATH::Square(rS);
	}
	else if (Sphere.GetCenter().GetZ() > Aabb.GetZMax())
	{
		rS = Sphere.GetCenter().GetZ() - Aabb.GetZMax();
		rD += MATH::Square(rS);
	}

	if (rD < MATH::Square(Sphere.GetRadius()))
	{
		return (true);
	}

	return (false);
}


//-----------------------------------------------------------------------------
//	Name:		TestIntersectionPointOBB
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionPointOBB(const OBB &		obb,
											   const VECTOR &	Point)
{
	VECTOR vDiff( Point - obb.GetCenter() );

	return ( ( MATH::Abs( vDiff.Dot( obb.GetDirection1() ) ) < obb.GetDistance1() ) &&
			 ( MATH::Abs( vDiff.Dot( obb.GetDirection2() ) ) < obb.GetDistance2() ) &&
			 ( MATH::Abs( vDiff.Dot( obb.GetDirection3() ) ) < obb.GetDistance3() ) );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectLineWithLine
// Object:
// 00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectLineWithLine(const LINE &	Line1,
											const LINE &	Line2,
											VECTOR	&		HitPoint,
											bool &			bMerged)
{
	bool	bResult = false;
	VECTOR	Dir1;
	VECTOR	Dir2;
	VECTOR	CrossProdVector;
	VECTOR	Pt1;
	VECTOR	Pt2;
	VECTOR	Vector;
	VECTOR	V1, V2;
	Float32	rDet;

	Line1.GetDirection( Dir1 );
	Line2.GetDirection( Dir2 );

	Line1.GetPoint( Pt1 );
	Line2.GetPoint( Pt2 );

	Vector.Sub(Pt2, Pt1);

	CrossProdVector.Cross( Dir1 , Dir2 );

//	les droites sont paralleles
	if ( CrossProdVector.IsNull() )
	{
		CrossProdVector.Cross( Dir1 , Vector );
	
//	les droites sont confondues
		if ( CrossProdVector.IsNull() )
		{
			bMerged = true;
			bResult = true;
			return ( bResult );
		}
//	les droites sont disjointes
		else
		{
			bMerged = false;
			bResult = false;
			return ( bResult );
		}
				
	}

	bMerged = false;

	rDet = Vector.Det( Dir1 , Dir2 );

//	les droites n'intersectent pas
	if ( MATH::Abs( rDet ) > /*MATH::EPSILON()*/0.01 )
	{
		bResult = false;
		return ( bResult );
	}

	bResult = true;

	VECTOR		x1, x2, x3, x4;

	x1 = Line1.GetPoint() + Line1.GetDirection();
	x2 = Line1.GetPoint() - Line1.GetDirection();
	x3 = Line2.GetPoint() + Line2.GetDirection();
	x4 = Line2.GetPoint() - Line2.GetDirection();

	VECTOR	U1, U2, U3, V, W;
	Float32	d1, d2;

	U1.Sub(x2, x1);
	U2.Sub(x4, x3);

	U1.Normalize();
	U2.Normalize();

	V.Sub(x1, x3);
	W.Sub(x2, x3);

	U3 = V - V.Dot( U2 ) * U2;
	U3.Normalize();

	d1 = V.Dot( U3 );
	d2 = W.Dot( U3 );

	Float32 H = -( 2.0f * d1 / d2 ) / ( 1.0f - d1 / d2 );

	HitPoint = x1 + U1 * H;
	
	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name: INTERSECTION::FastTestIntersectionOBB_OBB
//	Object: This test is much faster than the usual one, but it may return 'true' even if two
//	OBBs do NOT intersect. But if this happens, the two OBBs are close enough anyway.
//	01-02-21:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::FastTestIntersectionOBB_OBB( const OBB &	obb1,
											       const OBB &	obb2 )
{
	//Vector between the centers of the two OBBs.
	VECTOR vDiff( obb1.GetCenter().GetX() - obb2.GetCenter().GetX(),
				  obb1.GetCenter().GetY() - obb2.GetCenter().GetY(),
		          obb1.GetCenter().GetZ() - obb2.GetCenter().GetZ() );

	//Projection on the direction vectors of obb1.
	Float32 rDotV1X_V2X = MATH::Abs( obb1.GetDirection1().Dot( obb2.GetDirection1() ) );
	Float32 rDotV1X_V2Y = MATH::Abs( obb1.GetDirection1().Dot( obb2.GetDirection2() ) );
	Float32 rDotV1X_V2Z = MATH::Abs( obb1.GetDirection1().Dot( obb2.GetDirection3() ) );

	if ( MATH::Abs( vDiff.Dot( obb1.GetDirection1() ) )
		 >
		 MATH::Abs( obb1.GetDistance1() ) +
		 rDotV1X_V2X * obb2.GetDistance1() +
		 rDotV1X_V2Y * obb2.GetDistance2() +
		 rDotV1X_V2Z * obb2.GetDistance3() )
	{
		return ( false );
	}

	Float32 rDotV1Y_V2X = MATH::Abs( obb1.GetDirection2().Dot( obb2.GetDirection1() ) );
	Float32 rDotV1Y_V2Y = MATH::Abs( obb1.GetDirection2().Dot( obb2.GetDirection2() ) );
	Float32 rDotV1Y_V2Z = MATH::Abs( obb1.GetDirection2().Dot( obb2.GetDirection3() ) );

	if ( MATH::Abs( vDiff.Dot( obb1.GetDirection2() ) )
		 >
		 MATH::Abs( obb1.GetDistance2() ) +
		 rDotV1Y_V2X * obb2.GetDistance1() +
		 rDotV1Y_V2Y * obb2.GetDistance2() +
		 rDotV1Y_V2Z * obb2.GetDistance3() )
	{
		return ( false );
	}
	
	Float32 rDotV1Z_V2X = MATH::Abs( obb1.GetDirection3().Dot( obb2.GetDirection1() ) );
	Float32 rDotV1Z_V2Y = MATH::Abs( obb1.GetDirection3().Dot( obb2.GetDirection2() ) );
	Float32 rDotV1Z_V2Z = MATH::Abs( obb1.GetDirection3().Dot( obb2.GetDirection3() ) );

	if ( MATH::Abs( vDiff.Dot( obb1.GetDirection3() ) )
		 >
		 MATH::Abs( obb1.GetDistance3() ) +
		 rDotV1Z_V2X * obb2.GetDistance1() +
		 rDotV1Z_V2Y * obb2.GetDistance2() +
		 rDotV1Z_V2Z * obb2.GetDistance3() )
	{
		return ( false );
	}

	//Projection on the direction vectors of obb2.
	if ( MATH::Abs( vDiff.Dot( obb2.GetDirection1() ) )
		 >
		 MATH::Abs( obb2.GetDistance1() ) +
		 rDotV1X_V2X * obb1.GetDistance1() +
		 rDotV1Y_V2X * obb1.GetDistance2() +
		 rDotV1Z_V2X * obb1.GetDistance3() )
	{
		return ( false );
	}

	if ( MATH::Abs( vDiff.Dot( obb2.GetDirection2() ) )
		 >
		 MATH::Abs( obb2.GetDistance2() ) +
		 rDotV1X_V2Y * obb1.GetDistance1() +
		 rDotV1Y_V2Y * obb1.GetDistance2() +
		 rDotV1Z_V2Y * obb1.GetDistance3() )
	{
		return ( false );
	}
	
	if ( MATH::Abs( vDiff.Dot( obb2.GetDirection3() ) )
		 >
		 MATH::Abs( obb2.GetDistance3() ) +
		 rDotV1X_V2Z * obb1.GetDistance1() +
		 rDotV1Y_V2Z * obb1.GetDistance2() +
		 rDotV1Z_V2Z * obb1.GetDistance3() )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name: INTERSECTION::TestIntersectionOBB_OBB
//	Object: Test if two OBBs intersect. This test is quite slower than 
//			the fat version above, but it's an exact test.
//	01-02-21:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionOBB_OBB( const OBB &	obb1,
											   const OBB &	obb2 )
{
	//Following datas are the representation of obb2 in obb1 coordinates.
	VECTOR		NewDirection1;
	VECTOR		NewDirection2;
	VECTOR		NewDirection3;
	VECTOR		NewCenter;

	VECTOR vDiff( obb2.GetCenter().GetX() - obb1.GetCenter().GetX(),
				  obb2.GetCenter().GetY() - obb1.GetCenter().GetY(),
		          obb2.GetCenter().GetZ() - obb1.GetCenter().GetZ() );

	//Projection on obb1 axis
	NewDirection1.SetX( obb1.GetDirection1().Dot( obb2.GetDirection1() ) );
	NewDirection2.SetX( obb1.GetDirection1().Dot( obb2.GetDirection2() ) );
	NewDirection3.SetX( obb1.GetDirection1().Dot( obb2.GetDirection3() ) );
	NewCenter.SetX( obb1.GetDirection1().Dot( vDiff ) );

	if ( MATH::Abs( NewCenter.GetX() )
		 >
		 obb1.GetDistance1() +
		 obb2.GetDistance1() * MATH::Abs( NewDirection1.GetX() ) +
		 obb2.GetDistance2() * MATH::Abs( NewDirection2.GetX() ) +
		 obb2.GetDistance3() * MATH::Abs( NewDirection3.GetX() )  )
	{
		return ( false );
	}

	NewDirection1.SetY( obb1.GetDirection2().Dot( obb2.GetDirection1() ) );
	NewDirection2.SetY( obb1.GetDirection2().Dot( obb2.GetDirection2() ) );
	NewDirection3.SetY( obb1.GetDirection2().Dot( obb2.GetDirection3() ) );
	NewCenter.SetY( obb1.GetDirection2().Dot( vDiff ) );

	if ( MATH::Abs( NewCenter.GetY() )
		 >
		 obb1.GetDistance2() +
		 obb2.GetDistance1() * MATH::Abs( NewDirection1.GetY() ) +
		 obb2.GetDistance2() * MATH::Abs( NewDirection2.GetY() ) +
		 obb2.GetDistance3() * MATH::Abs( NewDirection3.GetY() )  )
	{
		return ( false );
	}

	NewDirection1.SetZ( obb1.GetDirection3().Dot( obb2.GetDirection1() ) );
	NewDirection2.SetZ( obb1.GetDirection3().Dot( obb2.GetDirection2() ) );
	NewDirection3.SetZ( obb1.GetDirection3().Dot( obb2.GetDirection3() ) );
	NewCenter.SetZ( obb1.GetDirection3().Dot( vDiff ) );

	if ( MATH::Abs( NewCenter.GetZ() )
		 >
		 obb1.GetDistance3() +
		 obb2.GetDistance1() * MATH::Abs( NewDirection1.GetZ() ) +
		 obb2.GetDistance2() * MATH::Abs( NewDirection2.GetZ() ) +
		 obb2.GetDistance3() * MATH::Abs( NewDirection3.GetZ() )  )
	{
		return ( false );
	}

	//Projection on obb2 axis
	if ( MATH::Abs( NewCenter.Dot( NewDirection1 ) )
		 >
		 obb2.GetDistance1() +
		 obb1.GetDistance1() * MATH::Abs( NewDirection1.GetX() ) +
		 obb1.GetDistance2() * MATH::Abs( NewDirection1.GetY() ) +
		 obb1.GetDistance3() * MATH::Abs( NewDirection1.GetZ() ) )
	{
		return ( false );
	}

	if ( MATH::Abs( NewCenter.Dot( NewDirection2 ) )
		 >
		 obb2.GetDistance2() +
		 obb1.GetDistance1() * MATH::Abs( NewDirection2.GetX() ) +
		 obb1.GetDistance2() * MATH::Abs( NewDirection2.GetY() ) +
		 obb1.GetDistance3() * MATH::Abs( NewDirection2.GetZ() ) )
	{
		return ( false );
	}

	if ( MATH::Abs( NewCenter.Dot( NewDirection3 ) )
		 >
		 obb2.GetDistance3() +
		 obb1.GetDistance1() * MATH::Abs( NewDirection3.GetX() ) +
		 obb1.GetDistance2() * MATH::Abs( NewDirection3.GetY() ) +
		 obb1.GetDistance3() * MATH::Abs( NewDirection3.GetZ() ) )
	{
		return ( false );
	}

	//Projection on the 9 orthogonal axis that are cross vector from each obb's direction.
	Float32 rProjNewCenter, rProjOBB1, rProjOBB2;

	/* vOrtho:	 0.0f
				 NewDirection1.GetZ()
				-NewDirection1.GetY() */
	rProjNewCenter = MATH::Abs( NewCenter.GetY() * NewDirection1.GetZ() -
								NewCenter.GetZ() * NewDirection1.GetY() );
	rProjOBB1      = obb1.GetDistance2() * MATH::Abs( NewDirection1.GetZ() ) +
					 obb1.GetDistance3() * MATH::Abs( NewDirection1.GetY() );
	rProjOBB2	   = obb2.GetDistance2() *
		             MATH::Abs( NewDirection2.GetY() * NewDirection1.GetZ() -
								NewDirection2.GetZ() * NewDirection1.GetY() );
	rProjOBB2	  += obb2.GetDistance3() *
		             MATH::Abs( NewDirection3.GetY() * NewDirection1.GetZ() -
								NewDirection3.GetZ() * NewDirection1.GetY() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 0.0f
				 NewDirection2.GetZ()
				-NewDirection2.GetY() */
	rProjNewCenter = MATH::Abs( NewCenter.GetY() * NewDirection2.GetZ() -
								NewCenter.GetZ() * NewDirection2.GetY() );
	rProjOBB1      = obb1.GetDistance2() * MATH::Abs( NewDirection2.GetZ() ) +
					 obb1.GetDistance3() * MATH::Abs( NewDirection2.GetY() );
	rProjOBB2	   = obb2.GetDistance1() *
		             MATH::Abs( NewDirection1.GetY() * NewDirection2.GetZ() -
								NewDirection1.GetZ() * NewDirection2.GetY() );
	rProjOBB2	  += obb2.GetDistance3() *
		             MATH::Abs( NewDirection3.GetY() * NewDirection2.GetZ() -
								NewDirection3.GetZ() * NewDirection2.GetY() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 0.0f
				 NewDirection3.GetZ()
				-NewDirection3.GetY() */
	rProjNewCenter = MATH::Abs( NewCenter.GetY() * NewDirection3.GetZ() -
								NewCenter.GetZ() * NewDirection3.GetY() );
	rProjOBB1      = obb1.GetDistance2() * MATH::Abs( NewDirection3.GetZ() ) +
					 obb1.GetDistance3() * MATH::Abs( NewDirection3.GetY() );
	rProjOBB2	   = obb2.GetDistance1() *
		             MATH::Abs( NewDirection1.GetY() * NewDirection3.GetZ() -
								NewDirection1.GetZ() * NewDirection3.GetY() );
	rProjOBB2	  += obb2.GetDistance2() *
		             MATH::Abs( NewDirection2.GetY() * NewDirection3.GetZ() -
								NewDirection2.GetZ() * NewDirection3.GetY() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 NewDirection1.GetZ()
				 0.0f
				-NewDirection1.GetX() */
	rProjNewCenter = MATH::Abs( NewCenter.GetX() * NewDirection1.GetZ() -
								NewCenter.GetZ() * NewDirection1.GetX() );
	rProjOBB1      = obb1.GetDistance1() * MATH::Abs( NewDirection1.GetZ() ) +
					 obb1.GetDistance3() * MATH::Abs( NewDirection1.GetX() );
	rProjOBB2	   = obb2.GetDistance2() *
		             MATH::Abs( NewDirection2.GetX() * NewDirection1.GetZ() -
								NewDirection2.GetZ() * NewDirection1.GetX() );
	rProjOBB2	  += obb2.GetDistance3() *
		             MATH::Abs( NewDirection3.GetX() * NewDirection1.GetZ() -
								NewDirection3.GetZ() * NewDirection1.GetX() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 NewDirection2.GetZ()
				 0.0f
				-NewDirection2.GetX() */
	rProjNewCenter = MATH::Abs( NewCenter.GetX() * NewDirection2.GetZ() -
								NewCenter.GetZ() * NewDirection2.GetX() );
	rProjOBB1      = obb1.GetDistance1() * MATH::Abs( NewDirection2.GetZ() ) +
					 obb1.GetDistance3() * MATH::Abs( NewDirection2.GetX() );
	rProjOBB2	   = obb2.GetDistance1() *
		             MATH::Abs( NewDirection1.GetX() * NewDirection2.GetZ() -
								NewDirection1.GetZ() * NewDirection2.GetX() );
	rProjOBB2	  += obb2.GetDistance3() *
		             MATH::Abs( NewDirection3.GetX() * NewDirection2.GetZ() -
								NewDirection3.GetZ() * NewDirection2.GetX() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}


	/* vOrtho:	 NewDirection3.GetZ()
				 0.0f
				-NewDirection3.GetX() */
	rProjNewCenter = MATH::Abs( NewCenter.GetX() * NewDirection3.GetZ() -
								NewCenter.GetZ() * NewDirection3.GetX() );
	rProjOBB1      = obb1.GetDistance1() * MATH::Abs( NewDirection3.GetZ() ) +
					 obb1.GetDistance3() * MATH::Abs( NewDirection3.GetX() );
	rProjOBB2	   = obb2.GetDistance1() *
		             MATH::Abs( NewDirection1.GetX() * NewDirection3.GetZ() -
								NewDirection1.GetZ() * NewDirection3.GetX() );
	rProjOBB2	  += obb2.GetDistance2() *
		             MATH::Abs( NewDirection2.GetX() * NewDirection3.GetZ() -
								NewDirection2.GetZ() * NewDirection3.GetX() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 NewDirection1.GetY()
				-NewDirection1.GetX()
				 0.0f */
	rProjNewCenter = MATH::Abs( NewCenter.GetX() * NewDirection1.GetY() -
								NewCenter.GetY() * NewDirection1.GetX() );
	rProjOBB1      = obb1.GetDistance1() * MATH::Abs( NewDirection1.GetY() ) +
					 obb1.GetDistance2() * MATH::Abs( NewDirection1.GetX() );
	rProjOBB2	   = obb2.GetDistance2() *
		             MATH::Abs( NewDirection2.GetX() * NewDirection1.GetY() -
								NewDirection2.GetY() * NewDirection1.GetX() );
	rProjOBB2	  += obb2.GetDistance3() *
		             MATH::Abs( NewDirection3.GetX() * NewDirection1.GetY() -
								NewDirection3.GetY() * NewDirection1.GetX() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 NewDirection2.GetY()
				-NewDirection2.GetX()
				 0.0f */
	rProjNewCenter = MATH::Abs( NewCenter.GetX() * NewDirection2.GetY() -
								NewCenter.GetY() * NewDirection2.GetX() );
	rProjOBB1      = obb1.GetDistance1() * MATH::Abs( NewDirection2.GetY() ) +
					 obb1.GetDistance2() * MATH::Abs( NewDirection2.GetX() );
	rProjOBB2	   = obb2.GetDistance1() *
		             MATH::Abs( NewDirection1.GetX() * NewDirection2.GetY() -
								NewDirection1.GetY() * NewDirection2.GetX() );
	rProjOBB2	  += obb2.GetDistance3() *
		             MATH::Abs( NewDirection3.GetX() * NewDirection2.GetY() -
								NewDirection3.GetY() * NewDirection2.GetX() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	/* vOrtho:	 NewDirection3.GetY()
				-NewDirection3.GetX()
				 0.0f */
	rProjNewCenter = MATH::Abs( NewCenter.GetX() * NewDirection3.GetY() -
								NewCenter.GetY() * NewDirection3.GetX() );
	rProjOBB1      = obb1.GetDistance1() * MATH::Abs( NewDirection3.GetY() ) +
					 obb1.GetDistance2() * MATH::Abs( NewDirection3.GetX() );
	rProjOBB2	   = obb2.GetDistance1() *
		             MATH::Abs( NewDirection1.GetX() * NewDirection3.GetY() -
								NewDirection1.GetY() * NewDirection3.GetX() );
	rProjOBB2	  += obb2.GetDistance2() *
		             MATH::Abs( NewDirection2.GetX() * NewDirection3.GetY() -
								NewDirection2.GetY() * NewDirection3.GetX() );
	if ( rProjNewCenter > rProjOBB1 + rProjOBB2 )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionTriangleAABB
//	Object:		
//	01-11-26:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionTriangleAABB(const TRIANGLE &	Triangle,
												   const AABB &		bbox)
{
	QDT_NOT_IMPLEMENTED();
	return (false);


	//    use separating axis theorem to test overlap between triangle and box
	//    need to test for overlap in these directions:
	//    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle
	//       we do not even need to test these)
	//    2) normal of the triangle
	//    3) crossproduct(edge from tri, {x,y,z}-directin)
	//       this gives 3 x 3 = 9 more tests

/*	VECTOR	v0(false), v1(false), v2(false);
	VECTOR	BoxHalfSize(bbox.GetXMax() - bbox.GetXMin() / 2.0f,
					    bbox.GetYMax() - bbox.GetYMin() / 2.0f,
					    bbox.GetZMax() - bbox.GetZMin() / 2.0f);
	VECTOR	e0, e1, e2;
	VECTOR	Normal;
	Float32	rD;
	Float32	rMin, rMax;
	Float32	rEx, rEy, rEz;

	VECTOR	Center = bbox.GetCenter();

	// 1) first test overlap in the {x,y,z}-directions
	//    find min, max of the triangle each direction, and test for overlap in
	//    that direction -- this is equivalent to testing a minimal AABB around
	//    the triangle against the AABB

	//    test in X
	v0.SetX(Triangle.GetPoint0().GetX() - Center.GetX());
	v1.SetX(Triangle.GetPoint1().GetX() - Center.GetX());
	v2.SetX(Triangle.GetPoint2().GetX() - Center.GetX());

	MATH::FindMinMax(v0.GetX(), v1.GetX() ,v2.GetX() , rMin, rMax);

	if (rMin > BoxHalfSize.GetX() || rMax < -BoxHalfSize.GetX()) return (false);

	//    test in Y
	v0.SetY(Triangle.GetPoint0().GetY() - Center.GetY());
	v1.SetY(Triangle.GetPoint1().GetY() - Center.GetY());
	v2.SetY(Triangle.GetPoint2().GetY() - Center.GetY());
	
	MATH::FindMinMax(v0.GetY(), v1.GetY() ,v2.GetY() , rMin, rMax);	

	if (rMin > BoxHalfSize.GetY() || rMax < -BoxHalfSize.GetY()) return (false);

	//    test in Z
	v0.SetZ(Triangle.GetPoint0().GetZ() - Center.GetZ());
	v1.SetZ(Triangle.GetPoint1().GetZ() - Center.GetZ());
	v2.SetZ(Triangle.GetPoint2().GetZ() - Center.GetZ());
	
	MATH::FindMinMax(v0.GetZ(), v1.GetZ() ,v2.GetZ() , rMin, rMax);	

	if (rMin > BoxHalfSize.GetZ() || rMax < -BoxHalfSize.GetZ()) return (false);

	//    2)
	//    test if the box intersects the plane of the triangle
	//    compute plane equation of triangle: normal * x + d = 0

	e0 = v1 - v0;
	e1 = v2 - v1;
	Normal.Cross(e0, e1);
	rD = -Normal.Dot(v0);	// plane eq: normal.x + d = 0

	bool	bPlaneOverlap;
	VECTOR	vMin, vMax;

    if(Normal.GetX() > 0.0f)
    {
		vMin.SetX(-BoxHalfSize.GetX());
		vMax.SetX( BoxHalfSize.GetX());
    }
    else
    {
		vMin.SetX( BoxHalfSize.GetX());
		vMax.SetX(-BoxHalfSize.GetX());
    }

	if(Normal.GetX() > 0.0f)
    {
		vMin.SetX(-BoxHalfSize.GetX());
		vMax.SetX( BoxHalfSize.GetX());
    }
    else
    {
		vMin.SetX( BoxHalfSize.GetX());
		vMax.SetX(-BoxHalfSize.GetX());
    }

	if(Normal.GetX() > 0.0f)
    {
		vMin.SetX(-BoxHalfSize.GetX());
		vMax.SetX( BoxHalfSize.GetX());
    }
    else
    {
		vMin.SetX( BoxHalfSize.GetX());
		vMax.SetX(-BoxHalfSize.GetX());
    }

	if (Normal.Dot(vMin) + rD > 0.0f) 
	{
		bPlaneOverlap = false;
	}
	else if (Normal.Dot(vMax) + rD >= 0.0f)
	{
		bPlaneOverlap = true;
	}
	else
	{
		bPlaneOverlap = false;
	}
  
	if (!bPlaneOverlap) return (false);

	//    compute the last triangle edge
	e2 = v0 - v2;

	//    3)
	rEx = MATH::Abs(e0.GetX());
	rEy = MATH::Abs(e0.GetY());
	rEz = MATH::Abs(e0.GetZ());
	if (!AxisTest_X(e0.GetZ(), e0.GetY(), rEz, rEy, v0, v2, BoxHalfSize))
	{
		return (false);
	}
	if (!AxisTest_Y(e0.GetZ(), e0.GetX(), rEz, rEx, v0, v2, BoxHalfSize))
	{
		return (false);
	}
	if (!AxisTest_Z(e0.GetY(), e0.GetX(), rEy, rEx, v1, v2, BoxHalfSize))
	{
		return (false);
	}

	rEx = MATH::Abs(e0.GetX());
	rEy = MATH::Abs(e0.GetY());
	rEz = MATH::Abs(e0.GetZ());
	if (!AxisTest_X(e1.GetZ(), e1.GetY(), rEz, rEy, v0, v2, BoxHalfSize))
	{
		return (false);
	}
	if (!AxisTest_Y(e1.GetZ(), e1.GetX(), rEz, rEx, v0, v2, BoxHalfSize))
	{
		return (false);
	}
	if (!AxisTest_Z(e1.GetY(), e1.GetX(), rEy, rEx, v0, v1, BoxHalfSize))
	{
		return (false);
	}

	rEx = MATH::Abs(e0.GetX());
	rEy = MATH::Abs(e0.GetY());
	rEz = MATH::Abs(e0.GetZ());
	if (!AxisTest_X(e2.GetZ(), e2.GetY(), rEz, rEy, v0, v1, BoxHalfSize))
	{
		return (false);
	}
	if (!AxisTest_Y(e2.GetZ(), e2.GetX(), rEz, rEx, v0, v1, BoxHalfSize))
	{
		return (false);
	}
	if (!AxisTest_Z(e2.GetY(), e2.GetX(), rEy, rEx, v1, v2, BoxHalfSize))
	{
		return (false);
	}

	return (true);*/
}

//-----------------------------------------------------------------------------
//	Name:		AxisTest_X01
//	Object:		
//	01-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::AxisTest_X(Float32			rA,
								 Float32			rB,
								 Float32			rFA,
								 Float32			rFB,
								 const VECTOR &	v0,
								 const VECTOR &	v1,
								 const VECTOR &	BoxHalfSize)
{
	Float32 rMin, rMax;
			   
	Float32 rP0 = rA * v0.GetY() - rB * v0.GetZ();			       	   
	Float32 rP1 = rA * v1.GetY() - rB * v1.GetZ();
	
	if (rP0 < rP1) 
	{
		rMin = rP0;
		rMax = rP1;
	}
	else 
	{
		rMin = rP1; 
		rMax = rP0;
	}
	
	Float32 rRad = rFA * BoxHalfSize.GetY() + rFB * BoxHalfSize.GetZ();
	
	if(rMin > rRad || rMax < -rRad)
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		AxisTest_Y
//	Object:		
//	01-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::AxisTest_Y(Float32			rA,
								 Float32			rB,
								 Float32			rFA,
								 Float32			rFB,
								 const VECTOR &	v0,
								 const VECTOR &	v1,
								 const VECTOR &	BoxHalfSize)
{
	Float32 rMin, rMax;

	Float32 rP0 = -rA * v0.GetX() + rB * v0.GetZ();
	Float32 rP1 = -rB * v1.GetX() + rB * v1.GetZ();

	if (rP0 < rP1) 
	{
		rMin = rP0; 
		rMax = rP1;
	} 
	else 
	{
		rMin = rP1; 
		rMax = rP0;
	}

	Float32 rRad = rFA * BoxHalfSize.GetX() + rFB * BoxHalfSize.GetZ(); 
	
	if (rMin > rRad || rMax < -rRad)
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		AxisTest_Y
//	Object:		
//	01-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::AxisTest_Z(Float32			rA,
								 Float32			rB,
								 Float32			rFA,
								 Float32			rFB,
								 const VECTOR &	v0,
								 const VECTOR &	v1,
								 const VECTOR &	BoxHalfSize)
{
	Float32 rMin, rMax;

	Float32 rP0 = rA * v0.GetX() - rB * v0.GetY();
	Float32 rP1 = rB * v1.GetX() - rB * v1.GetY();

	if (rP0 < rP1) 
	{
		rMin = rP0; 
		rMax = rP1;
	} 
	else 
	{
		rMin = rP1; 
		rMax = rP0;
	}

	Float32 rRad = rFA * BoxHalfSize.GetX() + rFB * BoxHalfSize.GetY(); 
	
	if (rMin > rRad || rMax < -rRad)
	{
		return (false);
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		SphereCylinder
//	Object:		
//	02-12-11:	JLT - Created
//-----------------------------------------------------------------------------
bool INTERSECTION::SphereCylinder(const SPHERE &S, const CYLINDER &C)
{
	VECTOR Center(S.GetCenter());
	Float32 rRadius(S.GetRadius());
	Float32 R(C.GetRadius()), H(C.GetHeight());

	Float32 DZ = C.GetAxis() * Center;
	
	if ((DZ < -rRadius) || (DZ > H + rRadius))
		return false;
	
	Float32 x = (VECTOR)Center * C.GetXDir(), y = (VECTOR)Center * C.GetYDir();
	
	Float32 DR = R + rRadius;
	
	if (x*x+y*y < DR*DR)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------
//	Name:		PlaneCylinder
//	Object:		
//	02-12-04:	JLT - Created
//-----------------------------------------------------------------------------
bool INTERSECTION::PlaneCylinder(const PLANE &P, const CYLINDER &C)
{
	Float32 HalfHeight = 0.5f * C.GetHeight();
	VECTOR N = P.GetNormal(), Axis = C.GetAxis(), Center = C.GetCenter() + Axis * HalfHeight;
	Float32 D = P.GetConstant();
	Float32 Dist, AbsNdW, Root, Term;

	QDT_ASSERT(N.IsNormalized());

    Dist = N * Center - D;

    AbsNdW = MATH::Abs(N * Axis);
    
	Root = MATH::Sqrt(MATH::Abs(1.0f - AbsNdW * AbsNdW));
    
	Term = C.GetRadius() * Root + HalfHeight * AbsNdW;
	
    return (MATH::Abs(Dist) <= Term);
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::BuildPlanesTabSegments
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
SEGMENT*	INTERSECTION::BuildPlanesTabSegments( int nNbrPlanes , PLANE *pPlaneTab , int & nNbrSegments )
{
	int		i , j;
	int		nMaxLines;
	int		nNbrLines = 0;
	LINE	*pLinesTab;
	LINE	*pLine1 , *pLine2;
	PLANE	*pPlane1 , *pPlane2;
	bool	bPlanesMerged;
	
	VECTOR	HitPoint;
	VECTOR	Point1;
	VECTOR	Point2;
	bool	bFirstPoint;
	bool	bLinesMerged;

	SEGMENT	*SegmentTab;

	nNbrSegments = 0;
	SegmentTab = NULL;

	if ( nNbrPlanes < 2 )
	{
		return ( NULL );
	}

	nMaxLines = ( ( nNbrPlanes ) * ( nNbrPlanes - 1 ) ) / 2;

	pLinesTab = new LINE[ nMaxLines ];

	for ( i = 0 ; i < nNbrPlanes - 1 ; ++i )
	{
		pPlane1 = & pPlaneTab[ i ];

		for ( j = i + 1 ; j < nNbrPlanes ; ++j )
		{
			pPlane2 = & pPlaneTab[ j ];

			pLine1 = & pLinesTab[ nNbrLines ];
			
			if ( INTERSECTION::IntersectPlaneWithPlane( *pPlane1 , *pPlane2 , *pLine1 , bPlanesMerged ) )
			{
				if ( bPlanesMerged != true )
				{
					++nNbrLines ; 					
				}
			}
		}
	}

	SegmentTab = new SEGMENT[ nNbrLines ];


	for ( i = 0 ; i < nNbrLines ; ++i )
	{
				
		pLine1 = & pLinesTab[ i ];

		bFirstPoint = true;

		for ( j = 0 ; j < nNbrLines ; ++j )
		{
			if ( j == i )
			{
				continue;
			}
			
			pLine2 = & pLinesTab[ j ];

			if ( ! INTERSECTION::IntersectLineWithLine( *pLine1 , *pLine2 , HitPoint , bLinesMerged ) )
			{
				continue;
			}
			
			if ( bLinesMerged )
			{
				continue;
			}

			if ( bFirstPoint )
			{
				Point1 = HitPoint;
				bFirstPoint = false;
			}
			else
			{
				if ( DISTANCE::DistancePointPoint( HitPoint , Point1 ) > MATH::EPSILON() )
				{
					Point2 = HitPoint;
					
					SegmentTab[nNbrSegments].SetPoint1(Point1);
					SegmentTab[nNbrSegments].SetPoint2(Point2);

					++nNbrSegments;

					break;
				}
			}
		}
	}

	if ( pLinesTab )
	{
		delete [ ] pLinesTab;
	}

	return ( SegmentTab );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPointsTabWithPlanesTab
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointsTabWithPlanesTab(int		nNbrPoints,
													  VECTOR *	PointsTab,
													  int		nNbrPlanes,
													  const PLANE *	PlanesTab)
{
	int				i;
	bool			bTest;
	bool			bSide = false;

	for ( i = 0 ; i < nNbrPlanes ; ++i )
	{
				
		bTest = INTERSECTION::IntersectPointsTabWithPlane( nNbrPoints , PointsTab , PlanesTab[ i ] , bSide );
		
		if ( bTest == false )
		{
			if ( bSide == false )
			{
				return ( false );
			}
		}
	}

	return ( true );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPointsTabWithPlane
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointsTabWithPlane(int		nNbrPoints,
												  const VECTOR *PointsTab,
												  const PLANE &	Plane,
												  bool &	bSide)
{
	bool	bResult;
	bool	bFirstSide = 0 , bCurrentSide;
	VECTOR	p;
	int		i;

	bResult = false;

	for ( i = 0 ; i < nNbrPoints ; ++i )
	{
		p = PointsTab[ i ];
		bCurrentSide = Plane.GetSide( p );

		if ( i == 0 )
		{
			bFirstSide = bCurrentSide;
		}
		else
		{
			if ( bCurrentSide != bFirstSide )
			{
				bResult = true;
				break;
			}
		}
	}

	if ( bResult == false )
	{
		bSide = bFirstSide;
	}

	return ( bResult );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectSphereWithPlanesTab
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectSphereWithPlanesTab( SPHERE & Sphere,
												    int nNbrSrcPlanes,
												    const PLANE *pSrcPlanesTab )
{
	int	i;
	bool			bTest;
	bool			bSide = false;

	for ( i = 0 ; i < nNbrSrcPlanes ; ++i )
	{
				
		bTest = INTERSECTION::IntersectPlaneWithSphere( pSrcPlanesTab[ i ] , Sphere , bSide );
		
		if ( bTest == false )
		{
			if ( bSide == false )
			{
				return ( false );
			}
		}
	}

	return ( true );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectSphereWithPlanesTab
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectSphereWithPlanesTab( SPHERE & Sphere,
												    int nNbrSrcPlanes,
												    const PLANE *pSrcPlanesTab,
												    int & nNbrResPlanes,
												    int *pnResPlanesIndexes )
{
	int	i;
	bool			bTest;
	bool			bSide = false;

	nNbrResPlanes = 0;

	for ( i = 0 ; i < nNbrSrcPlanes ; ++i )
	{
				
		bTest = INTERSECTION::IntersectPlaneWithSphere( pSrcPlanesTab[ i ] , Sphere , bSide );
		
		if ( bTest == true )
		{
			pnResPlanesIndexes[ nNbrResPlanes ] = i;
			++nNbrResPlanes;
		}
		else
		{
			if ( bSide == false )
			{
								
				return ( false );
			}
		}
	}

	return ( true );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPointWithPlanesTab
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointWithPlanesTab( VECTOR &	Point,
												   int			nNbrSrcPlanes,
												   const PLANE *pSrcPlanesTab)
{
	int				i;
	bool			bTest;
	bool			bSide;

	for ( i = 0 ; i < nNbrSrcPlanes ; ++i )
	{
				
		bTest = INTERSECTION::IntersectPointWithPlane( Point , pSrcPlanesTab[ i ] , bSide );
		
		if ( bTest == false )
		{
			if ( bSide == false )
			{
				return ( false );
			}
		}
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		INTERSECTION::TestIntersectionAABB_AABB
//	Object:		Test if two AABBs intersect.
//	01-02-20:	GGO - Created
//	02-05-22:	RMA - Added = to the comparison because of octree tests in the 
//				export.
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionAABB_AABB( const AABB &	aabb1,
												 const AABB &	aabb2 )
{
	if (aabb1.GetXMin() > aabb2.GetXMax())	return (false);
	if (aabb1.GetYMin() > aabb2.GetYMax())	return (false);
	if (aabb1.GetZMin() > aabb2.GetZMax())	return (false);

	if (aabb2.GetXMin() > aabb1.GetXMax())	return (false);
	if (aabb2.GetYMin() > aabb1.GetYMax())	return (false);
	if (aabb2.GetZMin() > aabb1.GetZMax())	return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionAABB_AABB
//	Object:		
//	02-05-22:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionAABB_AABB(const AABB &	aabb1,
												const AABB &	aabb2,
												Float32			rEpsilon)
{
	if (aabb1.GetXMin() - rEpsilon > aabb2.GetXMax())	return (false);
	if (aabb1.GetYMin() - rEpsilon > aabb2.GetYMax())	return (false);
	if (aabb1.GetZMin() - rEpsilon > aabb2.GetZMax())	return (false);

	if (aabb2.GetXMin() - rEpsilon > aabb1.GetXMax())	return (false);
	if (aabb2.GetYMin() - rEpsilon > aabb1.GetYMax())	return (false);
	if (aabb2.GetZMin() - rEpsilon > aabb1.GetZMax())	return (false);

	return (true);
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPlaneWithRay
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPlaneWithRay(PLANE &			Plane,
											const RAY &		Ray,
											Float32 &			rDistance)
{
	bool	bResult;
	Float32	rAlpha;
	Float32	rConst;
	Float32	rDot;
	VECTOR	Pt;

	Pt = Ray.GetOrigin();
	rConst = Plane.Apply( Pt );
	if ( MATH::Abs( rConst ) < MATH::EPSILON() )
	{
		bResult = true;

		rDistance = 0.0f;
	}
	else
	{
		VECTOR v1 , v2;
		Plane.GetNormal( v1 );
		v2.Copy( Ray.GetDirection() );
		rDot = v1.Dot( v2 );

		if ( MATH::Abs( rDot ) < MATH::EPSILON() )
		{
			bResult = false;
		}
		else
		{
			rAlpha = - rConst / rDot;
			if (rAlpha < 0.0f )
			{
				bResult = false ; 				
			}
			else
			{
				bResult = true;

				rDistance = rAlpha;
			}
		}
	}

	return ( bResult );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPlaneWithRay
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPlaneWithRay(PLANE &	Plane,
											const RAY &		Ray,
											VECTOR &		HitPoint)
{
	bool			bTest;
	VECTOR			V;
	Float32			rDistance;

	bTest = INTERSECTION::IntersectPlaneWithRay( Plane , Ray , rDistance );
	
	if ( bTest == true )
	{
		V.Copy( Ray.GetDirection() );
		V.Mul( rDistance );
		HitPoint = Ray.GetOrigin();
		HitPoint.Add( V );

		return ( true ) ; 	
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPlaneWithPlane
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPlaneWithPlane(const PLANE &	Plane1,
											  const PLANE & Plane2,
											  LINE &		Line,
											  bool &		bMerged)
{
	bool	bResult = false;
	VECTOR	Normal1;
	VECTOR	Normal2;
	VECTOR	Point1;
	VECTOR	V1 , V2;
	VECTOR	ProjVector;
	VECTOR	Vector;
	VECTOR	LineVector;
	VECTOR	LinePoint;
	
	Plane1.GetNormal( Normal1 );
	Plane2.GetNormal( Normal2 );

	LineVector.Cross( Normal1 , Normal2 );

//	les plans sont paralleles
	if ( LineVector.IsNull() )
	{
		Plane1.GetPoint( Point1 );

//	les plans sont confondus
		if ( INTERSECTION::IntersectPointWithPlane( Point1 , Plane2 ) )
		{
			bMerged = true;
			bResult = true;
		}
//	les plans sont disjoints
		else
		{
			bMerged = false;
			bResult = false;
		}

		return ( bResult );
	}
	else
	{
		bResult = true;
	}

// LineVector est la direction de la ligne

	Plane1.GetPoint( Point1 );
	Plane1.GetVectors( V1 , V2 );
	
	Vector.Cross( LineVector , V1 );

	if ( ! Vector.IsNull() )
	{
		ProjVector.Copy( V1 );
	}
	else
	{
		ProjVector.Copy( V2 );
	}

	Plane2.Project( Point1 , ProjVector , LinePoint );

	Line.Set( LinePoint , LineVector );

	return ( bResult );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPlaneWithPlane
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPlaneWithPlane(const PLANE &	Plane1,
											  const PLANE &	Plane2,
											  bool &		bMerged)
{
	bool	bResult = false;
	VECTOR	Normal1;
	VECTOR	Normal2;
	VECTOR	Vector;
	VECTOR	Point1;
	
	Plane1.GetNormal( Normal1 );
	Plane2.GetNormal( Normal2 );

	Vector.Cross( Normal1 , Normal2 );

//	les plans sont paralleles
	if ( Vector.IsNull() )
	{
		Plane1.GetPoint( Point1 );

//	les plans sont confondus
		if ( INTERSECTION::IntersectPointWithPlane( Point1 , Plane2 ) )
		{
			bMerged = true;
			bResult = true;
		}
//	les plans sont disjoints
		else
		{
			bMerged = false;
			bResult = false;
		}
	}

	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		IntersectPointWithPlanes
//	Object:		
//	05-11-28:	SBE - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointWithPlanes(VECTOR &			POut,
											   const PLANE &	p0,
											   const PLANE &	p1,
											   const PLANE &	p2)
{
    VECTOR n0( p0.GetNormal().GetX(), p0.GetNormal().GetY(), p0.GetNormal().GetZ() );
    VECTOR n1( p1.GetNormal().GetX(), p1.GetNormal().GetY(), p1.GetNormal().GetZ() );
    VECTOR n2( p2.GetNormal().GetX(), p2.GetNormal().GetY(), p2.GetNormal().GetZ() );

    VECTOR n1_n2, n2_n0, n0_n1;  
    
    n1_n2.Cross(n1, n2);
    n2_n0.Cross(n2, n0);
    n0_n1.Cross(n0, n1);

    Float32 cosTheta = n0.Dot(n1_n2);
    
	if (MATH::EqualsEpsilon(cosTheta, 0.f))
        return false;

    Float32 secTheta = 1.f / cosTheta;

    n1_n2 = n1_n2 * p0.GetNormal().GetPad();
    n2_n0 = n2_n0 * p1.GetNormal().GetPad();
    n0_n1 = n0_n1 * p2.GetNormal().GetPad();

    POut = -(n1_n2 + n2_n0 + n0_n1) * secTheta;
    return true;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
