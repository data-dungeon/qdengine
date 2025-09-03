//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DistancePointPoint
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
Float32	DISTANCE::DistancePointPoint(const VECTOR_2& Point1, const VECTOR_2& Point2)
{
	return (MATH::Sqrt(DistanceSquarePointPoint(Point1, Point2)));
}

//-----------------------------------------------------------------------------
//	Name:		DistanceSquarePointPoint
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
Float32	DISTANCE::DistanceSquarePointPoint(const VECTOR_2& Point1, const VECTOR_2& Point2)
{
	return (MATH::Square(Point2.GetX() - Point1.GetX()) + MATH::Square(Point2.GetY() - Point1.GetY()));
}

//-----------------------------------------------------------------------------
// Name: DISTANCE::DistanceRayPoint
// Object: This method implements the calcul of the distance between a ray & a 
//		   point.
// Reference : http://www.magic-software.com/MgcIntersection.html
//00-12-20 RMA Created: 
//01-02-09 GGO Revisited
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistanceRayPoint(const RAY &		Ray,
								 const VECTOR &		Point)
{
	return ( MATH::Sqrt( DistanceSquareRayPoint(Ray, Point) ) ) ;
}

//-----------------------------------------------------------------------------
// Name: DISTANCE::DistanceSquarePointPoint
// Object: This method implements the calcul of the square of the distance
//		   between 2 points
//00-12-21 RMA Created: 
//01-02-08 GGO Revisited
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistanceSquarePointPoint(const VECTOR &	Point1,
										 const VECTOR &	Point2)
{
	return MATH::Square( Point1.GetX() - Point2.GetX() )
		 + MATH::Square( Point1.GetY() - Point2.GetY() )
		 + MATH::Square( Point1.GetZ() - Point2.GetZ() );
}

//-----------------------------------------------------------------------------
// Name: DISTANCE::PointPoint
// Object: This method implements the calcul of the distance between 2 points 
//00-12-20 RMA Created: 
//01-02-08 GGO Revisited
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistancePointPoint(const VECTOR &	Point1, 
								   const VECTOR &	Point2)
{
	return MATH::Sqrt( DistanceSquarePointPoint( Point1, Point2) );
}

//-----------------------------------------------------------------------------
//	Name: DISTANCE::DistanceSquarePointLine
//	Object: Distance between a line and a point.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
Float32 DISTANCE::DistancePointLine(const VECTOR &	Point,
								  const LINE &		Line)
{
	return ( MATH::Sqrt( DistanceSquarePointLine( Point, Line ) ) );
}
//=============================================================================
// CODE ENDS HERE
//=============================================================================
