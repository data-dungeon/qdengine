//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: INTERSECTION::TestIntersectionPointPoint
// Object: This method checks if two points are at the same position.
//00-11-10 RMA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionPointPoint(const VECTOR & Point1,
												 const VECTOR & Point2)
{
	return (DISTANCE::DistanceSquarePointPoint(Point1, Point2 ) <= MATH::EPSILON());
}


//-----------------------------------------------------------------------------
// Name: INTERSECTION::TestIntersectionPointSphere
// Object: This method checks if a point is inside a sphere or not.
//01-01-04 RMA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionPointSphere(const VECTOR &	Point,
												  const SPHERE &	Sphere)
{
	return (DISTANCE::DistanceSquarePointPoint(Sphere.GetCenter(), Point) <= MATH::Square(Sphere.GetRadius())) ;
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::FindIntersectionRayTriangle
// Object:
// 01-01-04 RMA Created:
// 01-02-12 GGO Implemented
//-----------------------------------------------------------------------------
bool	INTERSECTION::FindIntersectionRayTriangle ( const RAY		 & Ray,
												    const TRIANGLE	 & Triangle,
												    VECTOR			 & Point3d )
{
	float rDistance;

	if ( INTERSECTION::FindIntersectionRayTriangle( Ray, Triangle, rDistance ) == true )
	{
		Point3d.SetX( Ray.GetOrigin().GetX() + rDistance * Ray.GetDirection().GetX() );
		Point3d.SetY( Ray.GetOrigin().GetY() + rDistance * Ray.GetDirection().GetY() );
		Point3d.SetZ( Ray.GetOrigin().GetZ() + rDistance * Ray.GetDirection().GetZ() );

		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersectionRayTriangleWithCulling
//	Object:		
//	01-09-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::FindIntersectionRayTriangleWithCulling(const RAY &		Ray,
															 const TRIANGLE &	Triangle,
															 VECTOR &			HitPoint)
{
	float rDistance;

	if ( INTERSECTION::FindIntersectionRayTriangleWithCulling(Ray, Triangle, rDistance ) == true)
	{
		HitPoint.SetX( Ray.GetOrigin().GetX() + rDistance * Ray.GetDirection().GetX() );
		HitPoint.SetY( Ray.GetOrigin().GetY() + rDistance * Ray.GetDirection().GetY() );
		HitPoint.SetZ( Ray.GetOrigin().GetZ() + rDistance * Ray.GetDirection().GetZ() );

		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPointWithLine
// Object:
//00-11-10 JVA Created:
//01-02-13 GGO optimized.
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointWithLine(const VECTOR	& Pt,
											 const LINE	& Line)
{
	return ( DISTANCE::DistanceSquarePointLine( Pt, Line ) <= MATH::EPSILON() );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPointWithPlane
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointWithPlane( const VECTOR & Pt,
											   const PLANE & Plane )
{
	return ( Plane.IsOn( Pt ) );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPointWithPlane
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPointWithPlane(const VECTOR &	Pt,
											  const PLANE &		Plane,
											  bool &			bSide)
{
	return ( Plane.IsOn( Pt , bSide ) );
}

//-----------------------------------------------------------------------------
//	Name: INTERSECTION::TestIntersectionLineSphere
//	Object: Same as above.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectionLineSphere( const LINE	& Line,
												  const SPHERE	& Sphere )
{
	return ( DISTANCE::DistanceSquarePointLine( Sphere.GetCenter(), Line ) < MATH::Square( Sphere.GetRadius() ) );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPlaneWithSphere
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPlaneWithSphere(const PLANE &	Plane,
											   const SPHERE &	Sphere)
{
	return ( DISTANCE::DistancePointPlane( Plane, Sphere.GetCenter() ) <= Sphere.GetRadius() );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectPlaneWithSphere
// Object:
//00-11-10 JVA Created:
//-----------------------------------------------------------------------------
bool	INTERSECTION::IntersectPlaneWithSphere(const PLANE &	Plane,
											   const SPHERE &	Sphere,
											   bool &			bSide)
{
	bool			bResult;

	bResult = INTERSECTION::IntersectPlaneWithSphere(Plane , Sphere);

	if ( bResult == false )
	{
		bSide = Plane.GetSide(Sphere.GetCenter());
	}

	return ( bResult );
}

//-----------------------------------------------------------------------------
// Name: INTERSECTION::IntersectSphereWithSphere
// Object: Intersection between two sphere
//00-11-10 JVA Created:
//01-02-12 GGO Revisited: use squared distance in order to avoid computing a square root.
//-----------------------------------------------------------------------------
bool	INTERSECTION::TestIntersectSphereSphere(const SPHERE &	Sphere1,
												const SPHERE &	Sphere2)
{
	return ( ( DISTANCE::DistanceSquarePointPoint( Sphere1.GetCenter(), Sphere2.GetCenter() ) )
		     <=
			 ( MATH::Square( Sphere1.GetRadius() ) + MATH::Square( Sphere2.GetRadius() ) ) );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
