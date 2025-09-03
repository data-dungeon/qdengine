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
// CLASS: Frustrum
//
//
//00-11-09 GGO Created: 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)	
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point4D/Point4D)	
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)
#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
	#include	"Frustrum.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Read(IO_STREAM &	Stream)
{
	Stream  >> _Origin
			>> _LeftDirection
			>> _TopDirection
			>> _FrontDirection
			>> _NormalTop
			>> _NormalBottom
			>> _NormalLeft
			>> _NormalRight
			>> _rNear
			>> _rFar
			>> _rLeft
			>> _rTop;

	ExtractClippingPlanes();
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Write(IO_STREAM &	Stream) const
{
	Stream  << _Origin
			<< _LeftDirection
			<< _TopDirection
			<< _FrontDirection
			<< _NormalTop
			<< _NormalBottom
			<< _NormalLeft
			<< _NormalRight
			<< _rNear
			<< _rFar
			<< _rLeft
			<< _rTop;
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	01-09-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Set(Float32	rFocalAngle,
					  Float32	rNearDistance,
					  Float32	rFarDistance,
					  Float32	rAspectRatio,
					  Float32 rScale)
{
	QDT_ASSERT( rFocalAngle > 0.0f );
	QDT_ASSERT( rNearDistance >= 0.0f );
	QDT_ASSERT( rNearDistance < rFarDistance );
	QDT_ASSERT( rAspectRatio > 0.0f );

	_Origin.SetX( 0.0f );
	_Origin.SetY( 0.0f );
	_Origin.SetZ( 0.0f );

	_FrontDirection.SetX( 0.0f );
	_FrontDirection.SetY( 0.0f );
	_FrontDirection.SetZ(-1.0f );

	_LeftDirection.SetX(-1.0f );
	_LeftDirection.SetY( 0.0f );
	_LeftDirection.SetZ( 0.0f );

	_TopDirection.SetX( 0.0f );
	_TopDirection.SetY( 1.0f );
	_TopDirection.SetZ( 0.0f );

	_rNear = rNearDistance;
	_rFar  = rFarDistance;

	_rTop  = rNearDistance * MATH::Tan( rFocalAngle * 0.5f ) * rScale;
	_rLeft = _rTop * rAspectRatio * rScale; 

	_NormalTop.SetX(  0.0f );
	_NormalTop.SetY( _rNear );
	_NormalTop.SetZ( _rTop );
	_NormalTop.Normalize();

	_NormalBottom.SetX(  0.0f );
	_NormalBottom.SetY(-_rNear );
	_NormalBottom.SetZ( _rTop );
	_NormalBottom.Normalize();

	_NormalLeft.SetX(-_rNear );
	_NormalLeft.SetY(  0.0f );
	_NormalLeft.SetZ( _rLeft );
	_NormalLeft.Normalize();

	_NormalRight.SetX( _rNear );
	_NormalRight.SetY(  0.0f );
	_NormalRight.SetZ( _rLeft );
	_NormalRight.Normalize();

	ExtractClippingPlanes();
}

//-----------------------------------------------------------------------------
//	Name:		FRUSTRUM constructor
//	Object:		
//	06-02-17:	RMA - Created
//-----------------------------------------------------------------------------
FRUSTRUM::FRUSTRUM(Float32	fHorizontalFieldOfView,
				   Float32	fAspectRatio,
				   Float32	fViewportAspectRatio,
				   Float32	fNearClippingDistance,
				   Float32	fFarClippingDistance,
				   Float32	fScale)
{
	_Origin.SetX( 0.0f );
	_Origin.SetY( 0.0f );
	_Origin.SetZ( 0.0f );
	_FrontDirection.SetX( 0.0f );
	_FrontDirection.SetY( 0.0f );
	_FrontDirection.SetZ(-1.0f );
	_LeftDirection.SetX(-1.0f );
	_LeftDirection.SetY( 0.0f );
	_LeftDirection.SetZ( 0.0f );
	_TopDirection.SetX( 0.0f );
	_TopDirection.SetY( 1.0f );
	_TopDirection.SetZ( 0.0f );

	_rNear = fNearClippingDistance;
	_rFar  = fFarClippingDistance;

	_rTop  = fNearClippingDistance * MATH::Tan( fHorizontalFieldOfView * 0.5f ) * fScale;
	_rLeft = _rTop * fViewportAspectRatio / fAspectRatio; 

	_NormalTop.SetX(  0.0f );
	_NormalTop.SetY( _rNear );
	_NormalTop.SetZ( _rTop );
	_NormalTop.Normalize();

	_NormalBottom.SetX(  0.0f );
	_NormalBottom.SetY(-_rNear );
	_NormalBottom.SetZ( _rTop );
	_NormalBottom.Normalize();

	_NormalLeft.SetX(-_rNear );
	_NormalLeft.SetY(  0.0f );
	_NormalLeft.SetZ( _rLeft );
	_NormalLeft.Normalize();

	_NormalRight.SetX( _rNear );
	_NormalRight.SetY(  0.0f );
	_NormalRight.SetZ( _rLeft );
	_NormalRight.Normalize();

	ExtractClippingPlanes();
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-04-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	FRUSTRUM::Intersect(const FRUSTRUM &	Frustum) const
{
	VECTOR	Points[8];

	Frustum.GetPoints(Points);

	if (_ClippingPlanes[CP_NEAR].AreOutside(8, &Points[0])   ||
		_ClippingPlanes[CP_FAR].AreOutside(8, &Points[0])	 ||
		_ClippingPlanes[CP_BOTTOM].AreOutside(8, &Points[0]) ||
		_ClippingPlanes[CP_TOP].AreOutside(8, &Points[0])	 ||
		_ClippingPlanes[CP_RIGHT].AreOutside(8, &Points[0])	 ||
		_ClippingPlanes[CP_LEFT].AreOutside(8, &Points[0]))
	{
		return	(false);
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::Transform
//	Object: Transform a frustrum with a matrix.
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Transform(const MATRIX &	Matrix,
							bool			bInvertLeftRight)
{
	Matrix.TransformPosition( _Origin );

	Matrix.TransformDirection( _FrontDirection );
	Matrix.TransformDirection( _LeftDirection );
	Matrix.TransformDirection( _TopDirection );

	Matrix.TransformDirection( _NormalTop );
	Matrix.TransformDirection( _NormalBottom );
	Matrix.TransformDirection( _NormalLeft );
	Matrix.TransformDirection( _NormalRight );

	if ( bInvertLeftRight == true )
	{
		_LeftDirection.Neg();

		// Swap
		VECTOR temp = _NormalLeft;
		_NormalLeft = _NormalRight;
		_NormalRight = temp;
	}

	_ClippingPlanes[CP_NEAR].Transform(Matrix);
	_ClippingPlanes[CP_FAR].Transform(Matrix);
	_ClippingPlanes[CP_LEFT].Transform(Matrix);
	_ClippingPlanes[CP_RIGHT].Transform(Matrix);
	_ClippingPlanes[CP_TOP].Transform(Matrix);
	_ClippingPlanes[CP_BOTTOM].Transform(Matrix);

	_Hexahedron.SetPlanes(_ClippingPlanes[CP_NEAR],
						  _ClippingPlanes[CP_FAR],
						  _ClippingPlanes[CP_LEFT],
						  _ClippingPlanes[CP_RIGHT],
						  _ClippingPlanes[CP_TOP],
						  _ClippingPlanes[CP_BOTTOM]);
}


//-----------------------------------------------------------------------------
//	Name:		Reduce
//	Object:		
//	01-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Reduce(const OBB &	obb)
{
	VECTOR Points[ 8 ];
	obb.GetPoints( Points );
	Reduce( Points );
}


//-----------------------------------------------------------------------------
//	Name:		Reduce
//	Object:		
//	01-09-17:	GGO - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Reduce(const AABB &	aabb)
{
	VECTOR Points[ 8 ];
	aabb.GetPoints( Points );
	Reduce( Points );
}


//-----------------------------------------------------------------------------
//	Name:		Reduce
//	Object:		
//	01-09-17:	GGO - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Reduce(VECTOR	(& Points)[8])
{
	// Locals.
	int		i;
	float	pProjFront[8];
	float	rMin, rMax, rTemp, rAngle, rCos, rSin;
	VECTOR	vDiff;



	// Try to increase the near (if a point of the obb is behind the near, we return).
	vDiff			=  Points[ 0 ] - _Origin;
	pProjFront[ 0 ] = _FrontDirection.Dot( vDiff );
	if ( pProjFront[ 0 ] < _rNear )
	{
		return;
	}
	rMin = pProjFront[ 0 ];
	for ( i = 1; i < 8; ++i )
	{
		vDiff			=  Points[ i ] - _Origin;
		pProjFront[ i ]	= _FrontDirection.Dot( vDiff );

		if ( pProjFront[ i ] < _rNear )
		{
			return;
		}
		if ( pProjFront[ i ] < rMin )
		{
			rMin = pProjFront[ i ];
		}
	}

	QDT_ASSERT( rMin >= _rNear );

	rTemp   = rMin / _rNear;

	_rLeft *= rTemp;
	_rTop  *= rTemp;
	_rNear  = rMin;


	// Try to reduce the left.
	pProjFront[ 0 ] = 1.0f / pProjFront[ 0 ];
	vDiff			= Points[ 0 ] - _Origin;
	rMax			= MATH::Abs( _LeftDirection.Dot( vDiff ) * pProjFront[ 0 ] );
	for ( i = 1; i < 8; ++i )
	{
		pProjFront[ i ] = 1.0f / pProjFront[ i ];
		vDiff			= Points[ i ] - _Origin;
		rTemp			= MATH::Abs( _LeftDirection.Dot( vDiff ) * pProjFront[ i ] );

		if ( rTemp > rMax )
		{
			rMax = rTemp;
		}
	}
	if ( _rLeft > rMax * _rNear )
	{
		_rLeft = rMax * _rNear;

		rAngle = MATH::ATan( _rLeft, _rNear );
		MATH::GetCosinusAndSinus(rAngle, rCos, rSin);
		_NormalLeft  = -rSin * _FrontDirection + rCos * _LeftDirection;
		_NormalRight = -rSin * _FrontDirection - rCos * _LeftDirection;
	}

	
	// Try to reduce the top.
	vDiff = Points[ 0 ] - _Origin;
	rMax  = MATH::Abs( _TopDirection.Dot( vDiff ) ) * pProjFront[ 0 ];
	for ( i = 1; i < 8; ++i )
	{
		vDiff = Points[ i ] - _Origin;
		rTemp = MATH::Abs( _TopDirection.Dot( vDiff ) ) * pProjFront[ i ];

		if ( rTemp > rMax )
		{
			rMax = rTemp;
		}
	}
	if ( _rTop > rMax * _rNear )
	{
		_rTop = rMax * _rNear;

		rAngle = MATH::ATan( _rTop, _rNear );
		MATH::GetCosinusAndSinus(rAngle, rCos, rSin);
		_NormalTop    = -rSin * _FrontDirection + rCos * _TopDirection;
		_NormalBottom = -rSin * _FrontDirection - rCos * _TopDirection;
	}

	ExtractClippingPlanes();
}


//-----------------------------------------------------------------------------
//	Name:		ComputeNormals
//	Object:		
//	01-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::ComputeNormals()
{
	VECTOR vTemp;


	// Left.
	vTemp = _rNear * _FrontDirection + _rLeft * _LeftDirection + _rTop * _TopDirection;
	_NormalLeft.Cross( _TopDirection, vTemp );
	_NormalLeft.Normalize();

	// Top.
	_NormalTop.Cross( vTemp, _LeftDirection );
	_NormalTop.Normalize();

	// Right.
	vTemp = _rNear * _FrontDirection - _rLeft * _LeftDirection - _rTop * _TopDirection;
	_NormalRight.Cross( vTemp, _TopDirection );
	_NormalRight.Normalize();

	// Bottom.
	_NormalBottom.Cross( _LeftDirection, vTemp );
	_NormalBottom.Normalize();

	ExtractClippingPlanes();
}


//-----------------------------------------------------------------------------
//	Name: FRUSTRUM::Display
//	Object: 
//	01-06-07:	GGO - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::Display(COLOR_FLOAT &Color) const
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		ExtractClippingPlanes
//	Object:		
//	02-01-03:	ELE - Created
//-----------------------------------------------------------------------------
void FRUSTRUM::ExtractClippingPlanes()
{	
	VECTOR	P;
	VECTOR	vL, vT;

	P = _Origin + (_rNear * _FrontDirection);
	vL = _rLeft * _LeftDirection;
	vT = _rTop * _TopDirection;

	_ClippingPlanes[CP_RIGHT].SetFromPointAndNormal	(P - vL, _NormalRight);
	_ClippingPlanes[CP_LEFT].SetFromPointAndNormal	(P + vL, _NormalLeft);
	_ClippingPlanes[CP_BOTTOM].SetFromPointAndNormal(P - vT, _NormalBottom);
	_ClippingPlanes[CP_TOP].SetFromPointAndNormal	(P + vT, _NormalTop);
	_ClippingPlanes[CP_NEAR].SetFromPointAndNormal	(P, -_FrontDirection);
	_ClippingPlanes[CP_FAR].SetFromPointAndNormal	(P + (_rFar * _FrontDirection), _FrontDirection);

	_Hexahedron.SetPlanes(_ClippingPlanes[CP_NEAR],
						  _ClippingPlanes[CP_FAR],
						  _ClippingPlanes[CP_LEFT],
						  _ClippingPlanes[CP_RIGHT],
						  _ClippingPlanes[CP_TOP],
						  _ClippingPlanes[CP_BOTTOM]);
}

//-----------------------------------------------------------------------------
//	Name:		ExtractFarClippingPlane
//	Object:		
//	03-11-18:	RMA - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::ExtractFarClippingPlane()
{
	VECTOR	P = _Origin + (_rNear * _FrontDirection);
	
	_ClippingPlanes[CP_FAR].SetFromPointAndNormal	(P + (_rFar * _FrontDirection), _FrontDirection);

	_Hexahedron.SetPlanes(_ClippingPlanes[CP_NEAR],
						  _ClippingPlanes[CP_FAR],
						  _ClippingPlanes[CP_LEFT],
						  _ClippingPlanes[CP_RIGHT],
						  _ClippingPlanes[CP_TOP],
						  _ClippingPlanes[CP_BOTTOM]);
}

//-----------------------------------------------------------------------------
//	Name:		ExtractNearFarClippingPlane
//	Object:		
//	03-11-18:	RMA - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::ExtractNearClippingPlane()
{
	VECTOR	P = _Origin + (_rNear * _FrontDirection);
	
	_ClippingPlanes[CP_NEAR].SetFromPointAndNormal	(P, -_FrontDirection);

	_Hexahedron.SetPlanes(_ClippingPlanes[CP_NEAR],
						  _ClippingPlanes[CP_FAR],
						  _ClippingPlanes[CP_LEFT],
						  _ClippingPlanes[CP_RIGHT],
						  _ClippingPlanes[CP_TOP],
						  _ClippingPlanes[CP_BOTTOM]);
}

//-----------------------------------------------------------------------------
//	Name:		GetPoints
//	Object:		
//	03-04-29:	ELE - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::GetPoints(VECTOR	(& Points)	[8]) const
{
	VECTOR	PFront	= _Origin + _rNear * _FrontDirection;
	VECTOR	PFar	= _Origin + _rFar * _FrontDirection;
	VECTOR	vL		= _rLeft * _LeftDirection;
	VECTOR	vT		= _rTop * _TopDirection;
	float	rRatio	= _rFar / _rNear;

//	Points[0] = _Origin + _rNear * _FrontDirection - _rLeft * _LeftDirection - _rTop * _TopDirection;
//	Points[1] = _Origin + _rNear * _FrontDirection - _rLeft * _LeftDirection + _rTop * _TopDirection;
//	Points[2] = _Origin + _rNear * _FrontDirection + _rLeft * _LeftDirection + _rTop * _TopDirection;
//	Points[3] = _Origin + _rNear * _FrontDirection + _rLeft * _LeftDirection - _rTop * _TopDirection;
//
//	Points[4] = _Origin + _rFar * _FrontDirection + _rFar / _rNear * ( -_rLeft * _LeftDirection - _rTop * _TopDirection );
//	Points[5] = _Origin + _rFar * _FrontDirection + _rFar / _rNear * ( -_rLeft * _LeftDirection + _rTop * _TopDirection );
//	Points[6] = _Origin + _rFar * _FrontDirection + _rFar / _rNear * (  _rLeft * _LeftDirection + _rTop * _TopDirection );
//	Points[7] = _Origin + _rFar * _FrontDirection + _rFar / _rNear * (  _rLeft * _LeftDirection - _rTop * _TopDirection );

	Points[0] = PFront - vL - vT;
	Points[1] = PFront - vL + vT;
	Points[2] = PFront + vL + vT;
	Points[3] = PFront + vL - vT;

	Points[4] = PFar + rRatio * ( -vL - vT );
	Points[5] = PFar + rRatio * ( -vL + vT );
	Points[6] = PFar + rRatio * (  vL + vT );
	Points[7] = PFar + rRatio * (  vL - vT );
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
const FRUSTRUM &	FRUSTRUM::operator=(const FRUSTRUM &	Frustrum)
{
	_Origin			= Frustrum._Origin;
	_LeftDirection	= Frustrum._LeftDirection;
	_TopDirection	= Frustrum._TopDirection;
	_FrontDirection	= Frustrum._FrontDirection;
	_NormalTop		= Frustrum._NormalTop;
	_NormalBottom	= Frustrum._NormalBottom;
	_NormalLeft		= Frustrum._NormalLeft;
	_NormalRight	= Frustrum._NormalRight;
	_rNear			= Frustrum._rNear;
	_rFar			= Frustrum._rFar;
	_rLeft			= Frustrum._rLeft;
	_rTop			= Frustrum._rTop;

	int i;
	for (i=0; i < (int)PLANES_COUNT; ++i)
		_ClippingPlanes[i] = Frustrum._ClippingPlanes[i];
	
	_Hexahedron		= Frustrum._Hexahedron;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		FRUSTRUM constructor
//	Object:		
//	06-02-24:	RMA - Created
//-----------------------------------------------------------------------------
void FRUSTRUM::CreateFromProjMatrix(const MATRIX & mProj, 
									Float32 fNear, 
									Float32 fFar)
{
	_Origin.SetX( 0.0f );
	_Origin.SetY( 0.0f );
	_Origin.SetZ( 0.0f );

	_FrontDirection.SetX( 0.0f );
	_FrontDirection.SetY( 0.0f );
	_FrontDirection.SetZ(-1.0f );

	_LeftDirection.SetX(-1.0f );
	_LeftDirection.SetY( 0.0f );
	_LeftDirection.SetZ( 0.0f );

	_TopDirection.SetX( 0.0f );
	_TopDirection.SetY( 1.0f );
	_TopDirection.SetZ( 0.0f );

	POINT4D column1( mProj(0,0), mProj(1,0), mProj(2,0), mProj(3,0) );
	POINT4D column2( mProj(0,1), mProj(1,1), mProj(2,1), mProj(3,1) );
	POINT4D column3( mProj(0,2), mProj(1,2), mProj(2,2), mProj(3,2) );
	POINT4D column4( mProj(0,3), mProj(1,3), mProj(2,3), mProj(3,3) );

	POINT4D Right(column4); 
	Right.Sub4(column1); 
    _ClippingPlanes[CP_RIGHT] = Right;
	_ClippingPlanes[CP_RIGHT].NormalizePlane();
	_ClippingPlanes[CP_RIGHT].FlipNormal();

	POINT4D Left(column4); 
	Left.Add4(column1);
	_ClippingPlanes[CP_LEFT] = Left;
	_ClippingPlanes[CP_LEFT].NormalizePlane();
	_ClippingPlanes[CP_LEFT].FlipNormal();

	POINT4D Bottom(column4); 
	Bottom.Add4(column2);
    _ClippingPlanes[CP_BOTTOM] = Bottom;
	_ClippingPlanes[CP_BOTTOM].NormalizePlane();
	_ClippingPlanes[CP_BOTTOM].FlipNormal();

	POINT4D Top(column4); 
	Top.Sub4(column2);
    _ClippingPlanes[CP_TOP] = Top;
	_ClippingPlanes[CP_TOP].NormalizePlane();
	_ClippingPlanes[CP_TOP].FlipNormal();

    _ClippingPlanes[CP_NEAR].SetCoeff(0.0f, 0.0f, -1.0f, fNear);
    _ClippingPlanes[CP_FAR].SetCoeff(0.0f, 0.0f, 1.0f, -fFar);

	_rNear = fNear;
	_rFar = fFar;

	VECTOR P = _Origin + (_rNear * _FrontDirection);
	_ClippingPlanes[CP_NEAR].SetFromPointAndNormal	(P, -_FrontDirection);
	_ClippingPlanes[CP_FAR].SetFromPointAndNormal	(P + (_rFar * _FrontDirection), _FrontDirection);

	_Hexahedron.SetPlanes(_ClippingPlanes[CP_NEAR],
						  _ClippingPlanes[CP_FAR],
						  _ClippingPlanes[CP_LEFT],
						  _ClippingPlanes[CP_RIGHT],
						  _ClippingPlanes[CP_TOP],
						  _ClippingPlanes[CP_BOTTOM]);

	_NormalTop = _ClippingPlanes[CP_TOP].GetNormal();
	_NormalBottom = _ClippingPlanes[CP_BOTTOM].GetNormal();
	_NormalLeft = _ClippingPlanes[CP_LEFT].GetNormal();
	_NormalRight = _ClippingPlanes[CP_RIGHT].GetNormal();

	QDT_ASSERT(_NormalTop.IsNormalized());
	QDT_ASSERT(_NormalBottom.IsNormalized());
	QDT_ASSERT(_NormalLeft.IsNormalized());
	QDT_ASSERT(_NormalRight.IsNormalized());

	_rLeft = _NormalLeft.GetZ();
	_rTop = _NormalTop.GetZ();

	_Hexahedron.SetPlanes(_ClippingPlanes[CP_NEAR],
						  _ClippingPlanes[CP_FAR],
						  _ClippingPlanes[CP_LEFT],
						  _ClippingPlanes[CP_RIGHT],
						  _ClippingPlanes[CP_TOP],
						  _ClippingPlanes[CP_BOTTOM]);
}	

//-----------------------------------------------------------------------------
//	Name:		FRUSTRUM constructor
//	Object:		
//	05-11-28:	SBE - Created
//-----------------------------------------------------------------------------
FRUSTRUM::FRUSTRUM(const MATRIX &	mProj)
{
	// special constructor : only used with trapezoidal directionnal shadow (not initializing all members)
    //  build a view frustum based on the current view & projection matrices...
    
	POINT4D column1( mProj(0,0), mProj(1,0), mProj(2,0), mProj(3,0) );
	POINT4D column2( mProj(0,1), mProj(1,1), mProj(2,1), mProj(3,1) );
	POINT4D column3( mProj(0,2), mProj(1,2), mProj(2,2), mProj(3,2) );
	POINT4D column4( mProj(0,3), mProj(1,3), mProj(2,3), mProj(3,3) );

	POINT4D Right(column4); 
	Right.Add4(column1); 
    _ClippingPlanes[CP_RIGHT] = Right;

	POINT4D Left(column4); 
	Left.Sub4(column1);
	_ClippingPlanes[CP_LEFT] = Left;

	POINT4D Bottom(column4); 
	Bottom.Sub4(column2);
    _ClippingPlanes[CP_BOTTOM] = Bottom;

	POINT4D Top(column4); 
	Top.Add4(column2);
    _ClippingPlanes[CP_TOP] = Top;

	POINT4D Far(column4); 
	Far.Add4(column3);
    _ClippingPlanes[CP_FAR] = Far;

	POINT4D Near(column4); 
	Near.Sub4(column3);
    _ClippingPlanes[CP_NEAR] = Near;

	for (UInt32 i=0; i<6; i++)
	{
		VECTOR vNormal(_ClippingPlanes[i].GetNormal());
		Float32 rPad = _ClippingPlanes[i].GetD();

		float rInvNorm = 1.0f / vNormal.Length();

		vNormal	*= rInvNorm;

		_ClippingPlanes[i].SetNormal(vNormal);
		_ClippingPlanes[i].SetD(rPad * rInvNorm);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPoints2
//	Object:		
//	05-11-28:	SBE - Created
//-----------------------------------------------------------------------------
void	FRUSTRUM::GetPoints2(VECTOR	( & Points)[8]) const
{
    for (int i=0; i<8; i++)  // compute extrema
    {
        PLANE p0 = (i&1)?(_ClippingPlanes[5]):(_ClippingPlanes[4]);
        PLANE p1 = (i&2)?(_ClippingPlanes[3]):(_ClippingPlanes[2]);
        PLANE p2 = (i&4)?(_ClippingPlanes[1]):(_ClippingPlanes[0]);

		INTERSECTION::IntersectPointWithPlanes(Points[i], p0, p1, p2);
    }
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
