//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::POINT2D_PACKED
//	Object:		Default constructor
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
POINT2D_PACKED::POINT2D_PACKED(const bool	bFlagInit)
{
	if	(bFlagInit)
	{
		SetXY(0, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::POINT2D_PACKED
//	Object:		Copy constructor
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
POINT2D_PACKED::POINT2D_PACKED(const POINT2D_PACKED &P)
{
	Set(P);
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::POINT2D_PACKED
//	Object:		Members constructor
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
POINT2D_PACKED::POINT2D_PACKED(const unsigned char cX, 
							   const unsigned char cY)
{
	SetXY(cX, cY);
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::POINT2D_PACKED
//	Object:		Constructor from a POINT2D
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
POINT2D_PACKED::POINT2D_PACKED(const POINT2D &	P)
{
	Set(P);
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::~POINT2D_PACKED destructor
//	01-01-10:	ONA - Created
//-----------------------------------------------------------------------------
POINT2D_PACKED::~POINT2D_PACKED()
{
	//	Do notinhg
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::Set
//	Object:		Set from a POINT2D_PACKED
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
void	POINT2D_PACKED::Set(const POINT2D_PACKED &	P)
{
	SetXY(P._cX, P._cY);
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::Set
//	Object:		Set from a POINT2D
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
void	POINT2D_PACKED::Set(const POINT2D &P)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::SetXY
//	Object:		Set its members
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
void	POINT2D_PACKED::SetXY(const unsigned char	cX,
							  const unsigned char	cY)
{
	_cX = cX;
	_cY = cY;
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::GetXY
//	Object:		Get its members
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
void	POINT2D_PACKED::GetXY(unsigned char &	cX,
							  unsigned char &	cY) const
{
	cX = _cX;
	cY = _cY;
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::GetX
//	Object:		Get X
//	01-04-19:	RMA - Created
//-----------------------------------------------------------------------------
unsigned char	POINT2D_PACKED::GetX() const
{
	return (_cX);
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::GetY
//	Object:		Get Y
//	01-04-19:	RMA - Created
//-----------------------------------------------------------------------------
unsigned char	POINT2D_PACKED::GetY() const
{
	return (_cY);
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::SetX
//	Object:		Set X
//	01-04-19:	RMA - Created
//-----------------------------------------------------------------------------
void POINT2D_PACKED::SetX(const unsigned char cX)
{
	_cX = cX;
}

//-----------------------------------------------------------------------------
//	Name:		POINT2D_PACKED::SetY
//	Object:		Set Y
//	01-04-19:	RMA - Created
//-----------------------------------------------------------------------------
void POINT2D_PACKED::SetY(const unsigned char cY)
{
	_cY = cY;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
