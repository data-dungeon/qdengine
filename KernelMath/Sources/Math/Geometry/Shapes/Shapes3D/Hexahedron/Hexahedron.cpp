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
//	CLASS:	HEXAHEDRON
//
//	01-10-09:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)
//#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
//#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Distance/Distance)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Obb/Obb)
#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
	#include "Hexahedron.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		HEXAHEDRON constructor
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
HEXAHEDRON::HEXAHEDRON()
:
_PlaneNear(		0.0f, 0.0f, 0.0f, 0.0f ),
_PlaneFar(		0.0f, 0.0f, 0.0f, 0.0f ),
_PlaneLeft(		0.0f, 0.0f, 0.0f, 0.0f ),
_PlaneRight(	0.0f, 0.0f, 0.0f, 0.0f ),
_PlaneTop(		0.0f, 0.0f, 0.0f, 0.0f ),
_PlaneBottom(	0.0f, 0.0f, 0.0f, 0.0f )

{
}


//-----------------------------------------------------------------------------
//	Name:		HEXAHEDRON constructor
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
HEXAHEDRON::HEXAHEDRON(const PLANE &	PlaneNear,
					   const PLANE &	PlaneFar,
					   const PLANE &	PlaneLeft,
					   const PLANE &	PlaneRight,
					   const PLANE &	PlaneTop,
					   const PLANE &	PlaneBottom)
:
_PlaneNear(		PlaneNear ),
_PlaneFar(		PlaneFar ),
_PlaneLeft(		PlaneLeft ),
_PlaneRight(	PlaneRight ),
_PlaneTop(		PlaneTop ),
_PlaneBottom(	PlaneBottom )
{
	NormalizeNormals();
}




//-----------------------------------------------------------------------------
//	Name:		HEXAHEDRON destructor
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
HEXAHEDRON::~HEXAHEDRON()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
HEXAHEDRON &	HEXAHEDRON::operator=(const HEXAHEDRON & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::Copy(const HEXAHEDRON & C)
{
	_PlaneTop.Copy(		C._PlaneTop );
	_PlaneBottom.Copy(	C._PlaneBottom );
	_PlaneLeft.Copy(	C._PlaneLeft );
	_PlaneRight.Copy(	C._PlaneRight );
	_PlaneNear.Copy(	C._PlaneNear );
	_PlaneFar.Copy(		C._PlaneFar );
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-09-16:	ELE - Created
//-----------------------------------------------------------------------------
bool	HEXAHEDRON::Intersect(const VECTOR &	Point) const
{
	if (_PlaneNear.IsOutside(Point)  ||
		_PlaneLeft.IsOutside(Point)	 ||
		_PlaneRight.IsOutside(Point) ||
		_PlaneTop.IsOutside(Point)	 ||
		_PlaneBottom.IsOutside(Point)||
		_PlaneFar.IsOutside(Point))
	{
		return	(false);
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::Transform(const MATRIX &	Matrix)
{
	_PlaneTop.Transform(	Matrix );
	_PlaneBottom.Transform( Matrix );
	_PlaneLeft.Transform(	Matrix );
	_PlaneRight.Transform(	Matrix );
	_PlaneNear.Transform(	Matrix );
	_PlaneFar.Transform(	Matrix );
}

//-----------------------------------------------------------------------------
//	Name:		NormalizeNormals
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::NormalizeNormals()
{
	_PlaneNear.NormalizeNormal();
	_PlaneFar.NormalizeNormal();
	_PlaneLeft.NormalizeNormal();
	_PlaneRight.NormalizeNormal();
	_PlaneTop.NormalizeNormal();
	_PlaneBottom.NormalizeNormal();
}

//-----------------------------------------------------------------------------
//	Name:		SetPlanes
//	Object:		
//	01-11-15:	JLT - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::SetPlanes(const PLANE &	PlaneNear,
							  const PLANE &	PlaneFar,
							  const PLANE &	PlaneLeft,
							  const PLANE &	PlaneRight,
							  const PLANE &	PlaneTop,
							  const PLANE &	PlaneBottom)
{
	_PlaneNear		= PlaneNear;
	_PlaneFar		= PlaneFar;
	_PlaneLeft		= PlaneLeft;
	_PlaneRight		= PlaneRight;
	_PlaneTop		= PlaneTop;
	_PlaneBottom	= PlaneBottom;

	NormalizeNormals();
}

//-----------------------------------------------------------------------------
//	Name:		GetPlane
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlane(HEXADRON_PLANE	Plane) const
{
	switch ( Plane )
	{
	case HP_LEFT:
		return ( _PlaneLeft );

	case HP_RIGHT:
		return ( _PlaneRight );

	case HP_TOP:
		return ( _PlaneTop );

	case HP_BOTTOM:
		return ( _PlaneBottom );

	case HP_NEAR:
		return ( _PlaneNear );

	case HP_FAR:
		return ( _PlaneFar );

	default:
		QDT_FAIL();
		return ( _PlaneLeft );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	04-04-23:	RMA - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::ComputePoints(QDT_VECTOR<VECTOR> &vPoints)
{
	VECTOR P;

	// Near
	GetPoint(P, HP_LEFT, HP_TOP, HP_NEAR);		vPoints.PushTail(P);
	GetPoint(P, HP_RIGHT, HP_TOP, HP_NEAR);		vPoints.PushTail(P);
	GetPoint(P, HP_LEFT, HP_BOTTOM, HP_NEAR);	vPoints.PushTail(P);	
	GetPoint(P, HP_RIGHT, HP_BOTTOM, HP_NEAR);  vPoints.PushTail(P);
	
	// Far
	GetPoint(P, HP_LEFT, HP_TOP, HP_FAR);		vPoints.PushTail(P);
	GetPoint(P, HP_RIGHT, HP_TOP, HP_FAR);		vPoints.PushTail(P);
	GetPoint(P, HP_LEFT, HP_BOTTOM, HP_FAR);	vPoints.PushTail(P);
	GetPoint(P, HP_RIGHT, HP_BOTTOM, HP_FAR);	vPoints.PushTail(P);
}

//-----------------------------------------------------------------------------
//	Name:		ComputePoints
//	Object:		
//	06-05-12:	ELE - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::ComputePoints(VECTOR *	pPoints) const
{
	// Near
	GetPoint(pPoints[0], HP_LEFT, HP_TOP, HP_NEAR);
	GetPoint(pPoints[1], HP_RIGHT, HP_TOP, HP_NEAR);
	GetPoint(pPoints[2], HP_LEFT, HP_BOTTOM, HP_NEAR);
	GetPoint(pPoints[3], HP_RIGHT, HP_BOTTOM, HP_NEAR);  

	// Far
	GetPoint(pPoints[4], HP_LEFT, HP_TOP, HP_FAR);
	GetPoint(pPoints[5], HP_RIGHT, HP_TOP, HP_FAR);
	GetPoint(pPoints[6], HP_LEFT, HP_BOTTOM, HP_FAR);
	GetPoint(pPoints[7], HP_RIGHT, HP_BOTTOM, HP_FAR);
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	HEXAHEDRON::GetPoint(VECTOR &				Point,
							 const HEXADRON_PLANE	Plane1,
							 const HEXADRON_PLANE	Plane2,
							 const HEXADRON_PLANE	Plane3) const
{
	QDT_ASSERT( Plane1 != Plane2 );
	QDT_ASSERT( Plane1 != Plane3 );
	QDT_ASSERT( Plane2 != Plane3 );


	float	m11, m12, m13, m21, m22, m23, m31, m32, m33, v1, v2, v3, rDet, rX, rY, rZ;


	GetPlane( Plane1 ).GetCoeff( m11, m12, m13, v1 );
	GetPlane( Plane2 ).GetCoeff( m21, m22, m23, v2 );
	GetPlane( Plane3 ).GetCoeff( m31, m32, m33, v3 );

	rDet = m11*m22*m33 - m11*m23*m32 - m21*m12*m33 + m21*m13*m32 + m31*m12*m23 - m31*m13*m22;

	if ( MATH::Abs( rDet ) < 0.00001f )
	{
		return ( false );
	}

	rX = v1*(m23*m32-m22*m33) + v2*(m12*m33-m13*m32) + v3*(m13*m22-m12*m23);
	rY = v1*(m33*m21-m31*m23) + v2*(m31*m13-m11*m33) + v3*(m11*m23-m21*m13);
	rZ = v1*(m22*m31-m32*m21) + v2*(m11*m32-m31*m12) + v3*(m21*m12-m11*m22);

	Point.SetX( rX );
	Point.SetY( rY );
	Point.SetZ( rZ );

	Point /= rDet;

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::Display(COLOR_FLOAT &	Color) const
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		SetFromObb
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::SetFromObb(const OBB &	Obb)
{
	_PlaneNear.SetFromPointAndNormal(   Obb.GetCenter() + Obb.GetDirection1() * Obb.GetDistance1(),  Obb.GetDirection1() );
	_PlaneFar.SetFromPointAndNormal(    Obb.GetCenter() - Obb.GetDirection1() * Obb.GetDistance1(), -Obb.GetDirection1() );
	_PlaneLeft.SetFromPointAndNormal(   Obb.GetCenter() + Obb.GetDirection2() * Obb.GetDistance2(),  Obb.GetDirection2() );
	_PlaneRight.SetFromPointAndNormal(  Obb.GetCenter() - Obb.GetDirection2() * Obb.GetDistance2(), -Obb.GetDirection2() );
	_PlaneTop.SetFromPointAndNormal(    Obb.GetCenter() + Obb.GetDirection3() * Obb.GetDistance3(),  Obb.GetDirection3() );
	_PlaneBottom.SetFromPointAndNormal( Obb.GetCenter() - Obb.GetDirection3() * Obb.GetDistance3(), -Obb.GetDirection3() );

	QDT_ASSERT( IsValid() == true );
}

//-----------------------------------------------------------------------------
//	Name:		SetFromPointsAndHeight
//	Object:		
//	03-01-24:	GGO - Created
//-----------------------------------------------------------------------------
bool	HEXAHEDRON::SetFromPointsAndHeight(const VECTOR&	Point1,
										   const VECTOR&	Point2,
										   const VECTOR&	Point3,
										   const VECTOR&	Point4,
										   const float		rHeight)
{
			VECTOR		TopVector( 0.0f, rHeight, 0.0f );
			VECTOR		TopPoint1, TopPoint2, TopPoint3, TopPoint4;
	static	const float	EPSILON = 0.001f;


	if ( MATH::Abs( rHeight ) < EPSILON )
	{
		return ( false );
	}

	if ( ( DISTANCE::DistanceSquarePointPoint( Point1, Point2 ) < EPSILON ) ||
		 ( DISTANCE::DistanceSquarePointPoint( Point1, Point3 ) < EPSILON ) ||
		 ( DISTANCE::DistanceSquarePointPoint( Point1, Point4 ) < EPSILON ) ||
		 ( DISTANCE::DistanceSquarePointPoint( Point2, Point3 ) < EPSILON ) ||
		 ( DISTANCE::DistanceSquarePointPoint( Point2, Point4 ) < EPSILON ) ||
		 ( DISTANCE::DistanceSquarePointPoint( Point3, Point4 ) < EPSILON ) )
	{
		return ( false );
	}


	// Build the plane.
	TopPoint1 = Point1 + TopVector;
	TopPoint2 = Point2 + TopVector;

	TopPoint3 = Point3 + TopVector;
	TopPoint4 = Point4 + TopVector;

	_PlaneLeft.SetFromPoints(  Point1, Point2, TopPoint2 );
	_PlaneFar.SetFromPoints(   Point2, Point3, TopPoint3 );
	_PlaneRight.SetFromPoints( Point3, Point4, TopPoint4 );
	_PlaneNear.SetFromPoints(  Point4, Point1, TopPoint1 );

	_PlaneBottom.SetFromPoints( Point1,    Point2,    Point3 );
	_PlaneTop.SetFromPoints(    TopPoint1, TopPoint2, TopPoint3 );

	SetNormals();
	NormalizeNormals();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		SetNormals
//	Object:		
//	03-01-27:	GGO - Created
//-----------------------------------------------------------------------------
void	HEXAHEDRON::SetNormals()
{
	VECTOR	NearLeftTop, NearTopRight, NearRightBottom, NearBottomLeft;
	VECTOR	FarLeftTop,  FarTopRight,  FarRightBottom,  FarBottomLeft;

	// Retrieve points.
	if ( ( GetPoint( NearLeftTop,     HP_NEAR, HP_LEFT,   HP_TOP    ) == false ) ||
		 ( GetPoint( NearTopRight,    HP_NEAR, HP_TOP,    HP_RIGHT  ) == false ) ||
		 ( GetPoint( NearRightBottom, HP_NEAR, HP_RIGHT,  HP_BOTTOM ) == false ) ||
		 ( GetPoint( NearBottomLeft,  HP_NEAR, HP_BOTTOM, HP_LEFT   ) == false ) ||
		 ( GetPoint( FarLeftTop,      HP_FAR,  HP_LEFT,   HP_TOP    ) == false ) ||
		 ( GetPoint( FarTopRight,     HP_FAR,  HP_TOP,    HP_RIGHT  ) == false ) ||
		 ( GetPoint( FarRightBottom,  HP_FAR,  HP_RIGHT,  HP_BOTTOM ) == false ) ||
		 ( GetPoint( FarBottomLeft,   HP_FAR,  HP_BOTTOM, HP_LEFT   ) == false ) )
	{
		return;
	}

	// Near.
	if ( ( _PlaneNear.GetSide( FarLeftTop )     == true ) &&
		 ( _PlaneNear.GetSide( FarTopRight )    == true ) &&
		 ( _PlaneNear.GetSide( FarRightBottom ) == true ) &&
		 ( _PlaneNear.GetSide( FarBottomLeft )  == true ) )
	{
		_PlaneNear.Flip();
	}

	// Far.
	if ( ( _PlaneFar.GetSide( NearLeftTop )     == true ) &&
		 ( _PlaneFar.GetSide( NearTopRight )    == true ) &&
		 ( _PlaneFar.GetSide( NearRightBottom ) == true ) &&
		 ( _PlaneFar.GetSide( NearBottomLeft )  == true ) )
	{
		_PlaneFar.Flip();
	}

	// Left
	if ( ( _PlaneLeft.GetSide( NearTopRight )    == true ) &&
		 ( _PlaneLeft.GetSide( NearRightBottom ) == true ) &&
		 ( _PlaneLeft.GetSide( FarTopRight )     == true ) &&
		 ( _PlaneLeft.GetSide( FarRightBottom )  == true ) )
	{
		_PlaneLeft.Flip();
	}
	
	// Right
	if ( ( _PlaneRight.GetSide( NearLeftTop )    == true ) &&
		 ( _PlaneRight.GetSide( NearBottomLeft ) == true ) &&
		 ( _PlaneRight.GetSide( FarLeftTop )     == true ) &&
		 ( _PlaneRight.GetSide( FarBottomLeft )  == true ) )
	{
		_PlaneRight.Flip();
	}

	// Top
	if ( ( _PlaneTop.GetSide( NearRightBottom ) == true ) &&
		 ( _PlaneTop.GetSide( NearBottomLeft )  == true ) &&
		 ( _PlaneTop.GetSide( FarRightBottom )  == true ) &&
		 ( _PlaneTop.GetSide( FarBottomLeft )   == true ) )
	{
		_PlaneTop.Flip();
	}
	
	// Bottom
	if ( ( _PlaneBottom.GetSide( NearLeftTop )  == true ) &&
		 ( _PlaneBottom.GetSide( NearTopRight ) == true ) &&
		 ( _PlaneBottom.GetSide( FarLeftTop )   == true ) &&
		 ( _PlaneBottom.GetSide( FarTopRight )  == true ) )
	{
		_PlaneBottom.Flip();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	HEXAHEDRON::GetCenter(VECTOR &	Center) const
{
	VECTOR	Point;

	Center = VECTOR::ZERO;

	// Near.
	if ( GetPoint( Point, HP_LEFT, HP_TOP, HP_NEAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}
	
	if ( GetPoint( Point, HP_TOP, HP_RIGHT, HP_NEAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}

	if ( GetPoint( Point, HP_RIGHT, HP_BOTTOM, HP_NEAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}

	if ( GetPoint( Point, HP_BOTTOM, HP_LEFT, HP_NEAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}

	// Far.
	if ( GetPoint( Point, HP_LEFT, HP_TOP, HP_FAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}
	
	if ( GetPoint( Point, HP_TOP, HP_RIGHT, HP_FAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}

	if ( GetPoint( Point, HP_RIGHT, HP_BOTTOM, HP_FAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}

	if ( GetPoint( Point, HP_BOTTOM, HP_LEFT, HP_FAR ) == true )
	{
		Center += Point;
	}
	else
	{
		return ( false );
	}

	Center /= 8.0f;

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	HEXAHEDRON::Read(IO_STREAM &	s)
{
	s >> _PlaneNear >> _PlaneFar >> _PlaneLeft >> _PlaneRight >> _PlaneTop >> _PlaneBottom;

	SetNormals();
	NormalizeNormals();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-01-23:	GGO - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	HEXAHEDRON::Write(IO_STREAM &	s) const
{
	s.DebugOutput("Near : ");
	s << _PlaneNear;
	s.DebugOutput("\t");
	
	s.DebugOutput("Far : ");
	s << _PlaneFar;
	s.DebugOutput("\t");
	
	s.DebugOutput("Left : ");
	s << _PlaneLeft;
	s.DebugOutput("\t");
	
	s.DebugOutput("Right : ");
	s << _PlaneRight;
	s.DebugOutput("\t");
	
	s.DebugOutput("Top : ");
	s << _PlaneTop;
	s.DebugOutput("\t");
	
	s.DebugOutput("Bottom : ");
	s << _PlaneBottom;
	s.DebugOutput("\t");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	03-01-27:	GGO - Created
//-----------------------------------------------------------------------------
bool	HEXAHEDRON::IsValid() const
{
	VECTOR	NearLeftTop, NearTopRight, NearRightBottom, NearBottomLeft;
	VECTOR	FarLeftTop,  FarTopRight,  FarRightBottom,  FarBottomLeft;

	if ( ( GetPoint( NearLeftTop,     HP_NEAR, HP_LEFT,   HP_TOP    ) == false ) ||
		 ( GetPoint( NearTopRight,    HP_NEAR, HP_TOP,    HP_RIGHT  ) == false ) ||
		 ( GetPoint( NearRightBottom, HP_NEAR, HP_RIGHT,  HP_BOTTOM ) == false ) ||
		 ( GetPoint( NearBottomLeft,  HP_NEAR, HP_BOTTOM, HP_LEFT   ) == false ) ||
		 ( GetPoint( FarLeftTop,      HP_FAR,  HP_LEFT,   HP_TOP    ) == false ) ||
		 ( GetPoint( FarTopRight,     HP_FAR,  HP_TOP,    HP_RIGHT  ) == false ) ||
		 ( GetPoint( FarRightBottom,  HP_FAR,  HP_RIGHT,  HP_BOTTOM ) == false ) ||
		 ( GetPoint( FarBottomLeft,   HP_FAR,  HP_BOTTOM, HP_LEFT   ) == false ) )
	{
		return ( false );
	}

	// Near.
	if ( ( _PlaneNear.GetSide( FarLeftTop )     == true ) ||
		 ( _PlaneNear.GetSide( FarTopRight )    == true ) ||
		 ( _PlaneNear.GetSide( FarRightBottom ) == true ) ||
		 ( _PlaneNear.GetSide( FarBottomLeft )  == true ) )
	{
		return ( false );
	}

	// Far.
	if ( ( _PlaneFar.GetSide( NearLeftTop )     == true ) ||
		 ( _PlaneFar.GetSide( NearTopRight )    == true ) ||
		 ( _PlaneFar.GetSide( NearRightBottom ) == true ) ||
		 ( _PlaneFar.GetSide( NearBottomLeft )  == true ) )
	{
		return ( false );
	}

	// Left
	if ( ( _PlaneLeft.GetSide( NearTopRight )    == true ) ||
		 ( _PlaneLeft.GetSide( NearRightBottom ) == true ) ||
		 ( _PlaneLeft.GetSide( FarTopRight )     == true ) ||
		 ( _PlaneLeft.GetSide( FarRightBottom )  == true ) )
	{
		return ( false );
	}
	
	// Right
	if ( ( _PlaneRight.GetSide( NearLeftTop )    == true ) ||
		 ( _PlaneRight.GetSide( NearBottomLeft ) == true ) ||
		 ( _PlaneRight.GetSide( FarLeftTop )     == true ) ||
		 ( _PlaneRight.GetSide( FarBottomLeft )  == true ) )
	{
		return ( false );
	}

	// Top
	if ( ( _PlaneTop.GetSide( NearRightBottom ) == true ) ||
		 ( _PlaneTop.GetSide( NearBottomLeft )  == true ) ||
		 ( _PlaneTop.GetSide( FarRightBottom )  == true ) ||
		 ( _PlaneTop.GetSide( FarBottomLeft )   == true ) )
	{
		return ( false );
	}
	
	// Bottom
	if ( ( _PlaneBottom.GetSide( NearLeftTop )  == true ) ||
		 ( _PlaneBottom.GetSide( NearTopRight ) == true ) ||
		 ( _PlaneBottom.GetSide( FarLeftTop )   == true ) ||
		 ( _PlaneBottom.GetSide( FarTopRight )  == true ) )
	{
		return ( false );
	}

	return ( true );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
