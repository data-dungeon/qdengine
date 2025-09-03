//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include INCL_KMATH(Math/Math)

//-----------------------------------------------------------------------------
// Name: DISC constructor
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
DISC::DISC()
:
_Center(VECTOR::ZERO),
_rRadius( 10.0f ),
_Axis(VECTOR::NEG_UNIT_Z)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
// Name: DISC constructor
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
DISC::DISC(const DISC &Disc)
{
	Build(Disc);
}

//-----------------------------------------------------------------------------
// Name: DISC constructor
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
DISC::DISC(VECTOR &	DiscCenter,
		   float	rDiscRadius,
		   VECTOR &	DiscAxis )
{
	Build( DiscCenter , rDiscRadius , DiscAxis );
}

//-----------------------------------------------------------------------------
// Name: DISC::Build
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::Build( const DISC &Disc )
{
	_Center = Disc._Center;
	_rRadius = Disc._rRadius;
	_Axis = Disc._Axis;
}

//-----------------------------------------------------------------------------
// Name: DISC::Build
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::Build( VECTOR &DiscCenter,
					  float rDiscRadius,
					  VECTOR &DiscAxis )
{
	_Center = DiscCenter;
	_rRadius = rDiscRadius;
	_Axis = DiscAxis;
}

//-----------------------------------------------------------------------------
// Name: DISC::GetRadius
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::GetRadius( float &r )const
{
	r = _rRadius;
}


//-----------------------------------------------------------------------------
// Name: DISC::SetRadius
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::SetRadius( float r )
{
	_rRadius = r;
}

//-----------------------------------------------------------------------------
// Name: DISC::GetCenter
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::GetCenter( VECTOR &p )const
{
	p = _Center;
}

//-----------------------------------------------------------------------------
// Name: DISC::SetCenter
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::SetCenter( VECTOR &c )
{
	_Center = c;
}

//-----------------------------------------------------------------------------
// Name: DISC::GetAxis
// Object:
//00 - 12 - 18 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::GetAxis( VECTOR &v )const
{
	v.Copy( _Axis );
}

//-----------------------------------------------------------------------------
// Name: DISC::SetAxis
// Object:
//00 - 12 - 18 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::SetAxis( VECTOR &v )
{
	_Axis.Copy( v );
}


//-----------------------------------------------------------------------------
// Name: DISC::GetRandomPoint
// Object:
//00 - 12 - 07 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::GetRandomPoint( VECTOR &Point ) const
{
	// We assume that _Axis = (0.0 1.0 0.0)

	float	x , y , z;
	float	rRandAngle;
	VECTOR	Vector;
	float	rRandRadius;

	rRandAngle = MATH::Rand( MATH::PI() * 2.0f );
	
	x = MATH::Cos( rRandAngle );
	y = 0.0f;
	z = MATH::Sin( rRandAngle );

	Vector.SetXYZ( x , y , z );

	rRandRadius = MATH::Sqrt(MATH::Rand(1.0f)) *  _rRadius ;

	Vector.SetLength( rRandRadius );

	Point.SetX( Vector.GetX() );
	Point.SetY( Vector.GetY() );
	Point.SetZ( Vector.GetZ() );
}
//=============================================================================
// CODE ENDS HERE
//=============================================================================
