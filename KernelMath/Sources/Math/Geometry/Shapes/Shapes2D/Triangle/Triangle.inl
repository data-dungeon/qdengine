//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TRIANGLE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
TRIANGLE::TRIANGLE()
{
}


//-----------------------------------------------------------------------------
//	Name:		TRIANGLE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
TRIANGLE::TRIANGLE( const TRIANGLE & Triangle )
:
_Point0( Triangle._Point0 ),
_Point1( Triangle._Point1 ),
_Point2( Triangle._Point2 )
{
		
}


//-----------------------------------------------------------------------------
//	Name:		TRIANGLE constructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
TRIANGLE::TRIANGLE(const VECTOR & Point0, 
				   const VECTOR & Point1, 
				   const VECTOR & Point2)
:
_Point0( Point0 ),
_Point1( Point1 ),
_Point2( Point2 )
{
}


//-----------------------------------------------------------------------------
//	Name:		TRIANGLE destructor
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
TRIANGLE::~TRIANGLE()
{
}


//-----------------------------------------------------------------------------
//	Name:		operator = 
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
TRIANGLE & TRIANGLE::operator = ( const TRIANGLE & Triangle )
{
	if ( this != & Triangle )
	{
		_Point0 = Triangle._Point0;
		_Point1 = Triangle._Point1;
		_Point2 = Triangle._Point2;
	}
	
	return ( *this );
}


//-----------------------------------------------------------------------------
//	Name:		Build
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::SetPoints(const VECTOR & Point0, 
							const VECTOR & Point1, 
							const VECTOR & Point2)
{
	_Point0 = Point0;
	_Point1 = Point1;
	_Point2 = Point2;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint0
//	Object:		
//	01-10-04:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::SetPoint0(const VECTOR &	Point)
{
	_Point0 = Point;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint1
//	Object:		
//	01-10-04:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::SetPoint1(const VECTOR &	Point)
{
	_Point1 = Point;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint2
//	Object:		
//	01-10-04:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE::SetPoint2(const VECTOR &	Point)
{
	_Point2 = Point;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint0
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
const VECTOR & TRIANGLE::GetPoint0() const
{
	return ( _Point0 );
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint1
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
const VECTOR & TRIANGLE::GetPoint1() const
{
	return ( _Point1 );
}


//-----------------------------------------------------------------------------
//	Name:		GetPoint2
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
const VECTOR & TRIANGLE::GetPoint2() const
{
	return ( _Point2 );
}


//-----------------------------------------------------------------------------
//	Name: TRIANGLE::GetArea
//	Object: 
//	01-03-01:	GGO - Created
//-----------------------------------------------------------------------------
float	TRIANGLE::GetArea()
{
	VECTOR Cross;
	VECTOR V1,V2;
	V1.Sub(_Point1, _Point0);
	V2.Sub(_Point2, _Point0);
	Cross.Cross(V1, V2);
	return (0.5f * Cross.Length());
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
