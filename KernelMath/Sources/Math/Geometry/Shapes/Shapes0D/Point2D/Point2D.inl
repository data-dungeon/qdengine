//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		POINT2D
//	Object:		Default constructor
//	00-11-13:	JVA - Created
//	01-01-09:	ONA - Modified : Added boolean flags for initialization
//-----------------------------------------------------------------------------
POINT2D::POINT2D(const bool	bFlagInit)
{
	if	(bFlagInit)
	{
		SetXY(0.0f, 0.0f);
	}
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D
//	Object:		Copy constructor
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
POINT2D::POINT2D(const POINT2D &	Point)
{
	_rX = Point._rX;
	_rY = Point._rY;
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D
//	Object:		Members constructor
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
POINT2D::POINT2D(const Float32	rX,
				 const Float32	rY)
{
	SetXY(rX, rY);
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	02-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	POINT2D::Set(IO_STREAM &	Stream)
{
	Stream >> _rX >> _rY;
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D
//	Object:		Constructor from a POINT2D_PACKED
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
POINT2D::POINT2D(const POINT2D_PACKED &	Point)
{
	Set(Point);
}

//-----------------------------------------------------------------------------
//	Name:		~POINT2D
//	Object:		Destructor
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
POINT2D::~POINT2D()
{
	//	Do nothing...
}

//-----------------------------------------------------------------------------
//	Name: POINT2D::operator
//	Object: 
//	01-06-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	POINT2D::operator==(const POINT2D& Point) const
{
	return ( ( MATH::EqualsEpsilon( _rX, Point._rX ) == true ) &&
			 ( MATH::EqualsEpsilon( _rY, Point._rY ) == true ) );
}

//-----------------------------------------------------------------------------
//	Name: POINT2D::operator
//	Object: 
//	01-06-25:	GGO - Created
//-----------------------------------------------------------------------------
bool	POINT2D::operator!=(const POINT2D& Point) const
{
	return ( ( MATH::EqualsEpsilon( _rX, Point._rX ) == false ) ||
			 ( MATH::EqualsEpsilon( _rY, Point._rY ) == false ) );
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D &	POINT2D::operator=(const POINT2D &	Point)
{
	 _rX = Point._rX;
	 _rY = Point._rY;

	 return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator-
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D	POINT2D::operator - (const POINT2D &	Point) const
{
	return (POINT2D(_rX - Point._rX, _rY - Point._rY));
}

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D	POINT2D::operator + (const POINT2D &	Point) const
{
	return (POINT2D(_rX + Point._rX, _rY + Point._rY));
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		Set from a POINT2D
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
void	POINT2D::Set(const POINT2D &	Point)
{
	_rX = Point._rX;
	_rY = Point._rY;
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		Set from a POINT2D_PACKED
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
void	POINT2D::Set(const POINT2D_PACKED &	Point)
{
	unsigned char	x, y;

	Point.GetXY(x, y);
	_rX = static_cast<Float32>( (x / 255.0f) );
	_rY = static_cast<Float32>( (y / 255.0f) );
}

//-----------------------------------------------------------------------------
//	Name:		SetXY
//	Object:		Set its two members
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
void	POINT2D::SetXY(const Float32	rX, 
					   const Float32	rY)
{
	_rX = rX;
	_rY = rY;
}

//-----------------------------------------------------------------------------
//	Name:		GetXY
//	Object:		Get its two members
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
void	POINT2D::GetXY(Float32 &	rX,
					   Float32 &	rY) const
{
	rX = _rX;
	rY = _rY;
}

//-----------------------------------------------------------------------------
//	Name:		GetX
//	Object:		Get the _rX member
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
Float32	POINT2D::GetX() const
{
	return	(_rX);
}

//-----------------------------------------------------------------------------
//	Name:		GetY
//	Object:		Get the _rY member
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
Float32	POINT2D::GetY() const
{
	return	(_rY);
}

//-----------------------------------------------------------------------------
//	Name:		GetXRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	POINT2D::GetXRef() const
{
	return	(_rX);
}

//-----------------------------------------------------------------------------
//	Name:		GetYRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	POINT2D::GetYRef() const
{
	return	(_rY);
}


//-----------------------------------------------------------------------------
//	Name:		AddScaled
//	Object:		Add a POINT2D scaled by rS
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
void	POINT2D::AddScaled(const POINT2D &	Point,
						   Float32			rS)
{
	_rX += Point._rX * rS;
	_rY += Point._rY * rS;
}

//-----------------------------------------------------------------------------
//	Name:		Add
//	Object:		
//	03-04-10:	SBE - Created
//-----------------------------------------------------------------------------
void	POINT2D::Add(const POINT2D &	Point)
{
	_rX += Point._rX;
	_rY += Point._rY;
}

//-----------------------------------------------------------------------------
//	Name:		Sub
//	Object:		
//	03-04-10:	SBE - Created
//-----------------------------------------------------------------------------
void	POINT2D::Sub(const POINT2D &	Point)
{
	_rX -= Point._rX;
	_rY -= Point._rY;
}

//-----------------------------------------------------------------------------
//	Name:		Interpolate
//	Object:		Make a linear interpolation between two POINT2D
//	01-01-09:	ONA - Created
//-----------------------------------------------------------------------------
void	POINT2D::Interpolate(const Float32		rT,
							 const POINT2D &	P1,
							 const POINT2D &	P2)
{
	_rX = P1._rX + rT * (P2._rX - P1._rX);
	_rY = P1._rY + rT * (P2._rY - P1._rY);
}

//-----------------------------------------------------------------------------
//	Name:		Projection
//	Object:		Project a POINT2D between -1.0f and 1.0f in screen coordinate
//	00-11-23:	ELE - Created
//	01-01-09:	ONA - Modified : Added const and rename members
//-----------------------------------------------------------------------------
void	POINT2D::Projection(const Float32	rCenterX,
							const Float32	rCenterY,
							const Float32	rWidth,
							const Float32	rHeight)
{
	_rX = (rCenterX - (rWidth / 2)) + (_rX * rWidth);
	_rY = (rCenterY - (rHeight / 2)) + (_rY * rHeight);
}

//-----------------------------------------------------------------------------
// Name: POINT2D::GetX
// Object: 
//00-11-21 ELE Created: 
//-----------------------------------------------------------------------------
void	POINT2D::SetX(Float32 rX)
{
	_rX = rX;
}

//-----------------------------------------------------------------------------
// Name: POINT2D::GetY
// Object: 
//00-11-21 ELE Created: 
//-----------------------------------------------------------------------------
void	POINT2D::SetY(Float32 rY)
{
	_rY = rY;
}

//-----------------------------------------------------------------------------
//	Name:		Cross
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
Float32	POINT2D::Cross(const POINT2D &	Pt) const
{
	return ((_rX * Pt._rY) - (_rY * Pt._rX));
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
Float32 POINT2D::operator[](unsigned int index) const
{
	QDT_ASSERT(index<2);
	return (*(((Float32*)&_rX)+index));
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
Float32&	POINT2D::operator[](unsigned int index)
{
	QDT_ASSERT(index<2);
	return (*(((Float32*)&_rX)+index));
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	02-03-01:	ELE - Created
//-----------------------------------------------------------------------------
POINT2D	POINT2D::operator*(const POINT2D &	Point) const
{
	POINT2D	P;

	P[_X_] = _rX * Point._rX;
	P[_Y_] = _rY * Point._rY;

	return P;
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
POINT2D POINT2D::operator*(Float32 k) const
{
	POINT2D P;

	P[_X_] = _rX * k;
	P[_Y_] = _rY * k;

	return P;
}

//-----------------------------------------------------------------------------
//	Name:		operator/
//	Object:		
//	01-09-26:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D	POINT2D::operator / (Float32	k) const
{
	QDT_ASSERT(k != 0);

	POINT2D P;

	P[_X_] = _rX / k;
	P[_Y_] = _rY / k;

	return P;
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	01-09-26:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D & POINT2D::operator *= (Float32	k)
{
	_rX *= k;
	_rY *= k;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-11-21:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D &	POINT2D::operator+=(const POINT2D &	Point)
{
	_rX += Point._rX;
	_rY += Point._rY;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-11-21:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D &	POINT2D::operator-=(const POINT2D & Point)
{
	_rX -= Point._rX;
	_rY -= Point._rY;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator/=
//	Object:		
//	01-11-21:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D &	POINT2D::operator/=(Float32	k)
{
	_rX /= k;
	_rY /= k;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		IsInRange
//	Object:		
//	02-07-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	POINT2D::IsInRange(const POINT2D &	P,
						   const Float32		rRadius) const
{
	POINT2D Diff = (*this) - P;
	Float32 rSquareDist = (Diff.GetX() * Diff.GetX()) + (Diff.GetY() * Diff.GetY());

	return (rSquareDist < rRadius);
}


//-----------------------------------------------------------------------------
//	Name:		Dot
//	Object:		
//	03-08-13:	JPE - Created
//-----------------------------------------------------------------------------
Float32	POINT2D::Dot(const POINT2D & Pt) const
{
	return ( (_rX * Pt._rX) + (_rY * Pt._rY) );
}

//-----------------------------------------------------------------------------
//	Name:		GetLenght
//	Object:		
//	03-08-13:	JPE - Created
//-----------------------------------------------------------------------------
Float32	POINT2D::GetLength(void)
{
	return MATH::Sqrt( GetSquareLength() );
}

//-----------------------------------------------------------------------------
//	Name:		GetLenght
//	Object:		
//	03-08-13:	JPE - Created
//-----------------------------------------------------------------------------
Float32	POINT2D::GetSquareLength(void)
{
	return ( (_rX*_rX) + (_rY*_rY) );
}

//-----------------------------------------------------------------------------
//	Name:		Inverse
//	Object:		
//	03-08-13:	JPE - Created
//-----------------------------------------------------------------------------
void	POINT2D::Inverse(void)
{
	_rX = -_rX;
	_rY = -_rY;
}

//-----------------------------------------------------------------------------
//	Name:		IsNull
//	Object:		returns true if both x & y are close to epsilon
//	03-08-13:	JPE - Created
//-----------------------------------------------------------------------------
bool	POINT2D::IsNull(void)
{
	if( (MATH::Abs(_rX) < 0.0001f) && (MATH::Abs(_rY) < 0.0001f) )
		return true;

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		Normalize
//	Object:		
//	03-08-13:	JPE - Created
//-----------------------------------------------------------------------------
void	POINT2D::Normalize(void)
{
	Float32 Len = MATH::Sqrt( MATH::Square( _rX ) + MATH::Square( _rY ) );

	_rX /= Len;
	_rY /= Len;
}

//-----------------------------------------------------------------------------
//	Name:		MinimizeWith
//	Object:		
//	04-03-23:	SBE - Created
//-----------------------------------------------------------------------------
void	POINT2D::MinimizeWith(const POINT2D &	P)
{
	_rX = MATH::Min(_rX, P._rX);
	_rY = MATH::Min(_rY, P._rY);
}


//-----------------------------------------------------------------------------
//	Name:		MaximizeWith
//	Object:		
//	04-03-23:	SBE - Created
//-----------------------------------------------------------------------------
void	POINT2D::MaximizeWith(const POINT2D &	P)
{
	_rX = MATH::Max(_rX, P._rX);
	_rY = MATH::Max(_rY, P._rY);
}

//-----------------------------------------------------------------------------
//	Name:		IsZero
//	Object:		
//	03-11-07:	SBE - Created
//-----------------------------------------------------------------------------
_INLINE_ bool	POINT2D::IsZero() const
{
	return ((_rX == 0.0f) && (_rY == 0.0f));
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
