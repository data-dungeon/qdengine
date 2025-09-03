//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include INCL_KMATH(Math/Geometry/Matrix/Matrix)

//-----------------------------------------------------------------------------
//	Name:		RAY constructor
//	Object:		Default constructor
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
RAY::RAY()
{
}

//-----------------------------------------------------------------------------
//	Name:		RAY constructor
//	Object:		Copy constructor
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
RAY::RAY( const RAY & Ray )
:
_Origin(Ray._Origin),
_Direction(Ray._Direction)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAY constructor
//	Object:		Constructor with a point , a direction & a length
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
RAY::RAY( const VECTOR & 	Origin,
		  const VECTOR & 	Direction)
:
_Origin		(Origin),
_Direction	(Direction)
{
	// The director vector must be normalized
	QDT_ASSERT(Direction.IsNormalized()) ;
}

//-----------------------------------------------------------------------------
//	Name:		RAY destructor
//	Object:		Destructor
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
RAY::~RAY()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator = 
//	Object:		Operator =
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
const RAY & RAY::operator = ( const RAY & Ray )
{
	_Origin = Ray._Origin;
	_Direction = Ray._Direction;
	return ( *this );
}

//-----------------------------------------------------------------------------
//	Name:		GetOrigin
//	Object:		Get origin point of the ray
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
const VECTOR & RAY::GetOrigin() const
{
	return (_Origin) ;
}

//-----------------------------------------------------------------------------
//	Name:		SetOrigin
//	Object:		Set the origin point of the ray
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
void RAY::SetOrigin(const VECTOR & Origin)
{
	_Origin = Origin ;
}


//-----------------------------------------------------------------------------
//	Name:		GetDirection
//	Object:		Get Direction of the ray
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
const VECTOR & RAY::GetDirection() const
{
	return (_Direction);
}

//-----------------------------------------------------------------------------
//	Name:		SetDirection
//	Object:		Set direction of the ray
//	01-01-15:	RPA - Created
//-----------------------------------------------------------------------------
void RAY::SetDirection(const VECTOR & Direction)
{
	// The length of the director vector must be equal to one
	QDT_ASSERT(Direction.IsNormalized());

	_Direction = Direction;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
