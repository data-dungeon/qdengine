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
// CLASS: COLOR_FLOAT
//
//
//	03-07-31:	SBE - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT constructor
//	Object:		Default construtor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT::COLOR_FLOAT()
{
	_r[0] = _r[1] = _r[2] = _r[3] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT constructor
//	Object:		Default construtor
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT::COLOR_FLOAT(const bool	bFlagInit)
{
	QDT_ASSERT(bFlagInit == false);
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT constructor
//	Object:		COnstructor with parameters
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT::COLOR_FLOAT(const float	rA, 
						 const float	rR, 
						 const float	rG, 
						 const float	rB)
{
	_r[0] = rR;
	_r[1] = rG;
	_r[2] = rB;
	_r[3] = rA;
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT constructor
//	Object:		Copy constructor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT::COLOR_FLOAT(const COLOR_FLOAT & Color)
{
	_r[0] = Color._r[0];
	_r[1] = Color._r[1];
	_r[2] = Color._r[2];
	_r[3] = Color._r[3];
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT destructor
//	Object: 
//	01-03-19:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT::~COLOR_FLOAT()
{
	//	Do nothing...
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT &	COLOR_FLOAT::operator=(const COLOR_FLOAT &	Color)
{
	_r[3] = Color._r[3];
	_r[0] = Color._r[0];
	_r[1] = Color._r[1];
	_r[2] = Color._r[2];
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
bool	COLOR_FLOAT::operator==(const COLOR_FLOAT &	Color) const
{
	return ( ( MATH::EqualsEpsilon( _r[3], Color._r[3] ) == true ) &&
			 ( MATH::EqualsEpsilon( _r[0], Color._r[0] ) == true ) &&
			 ( MATH::EqualsEpsilon( _r[1], Color._r[1] ) == true ) &&
			 ( MATH::EqualsEpsilon( _r[2], Color._r[2] ) == true ) );
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
bool	COLOR_FLOAT::operator!=(const COLOR_FLOAT &	Color) const
{
	return ( ( MATH::EqualsEpsilon( _r[3], Color._r[3] ) == false ) ||
			 ( MATH::EqualsEpsilon( _r[0], Color._r[0] ) == false ) ||
			 ( MATH::EqualsEpsilon( _r[1], Color._r[1] ) == false ) ||
			 ( MATH::EqualsEpsilon( _r[2], Color._r[2] ) == false ) );
}

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT	COLOR_FLOAT::operator+(const COLOR_FLOAT &	Color) const
{
	float rA = _r[3] + Color._r[3];
	float rR = _r[0] + Color._r[0];
	float rG = _r[1] + Color._r[1];
	float rB = _r[2] + Color._r[2];

	return (COLOR_FLOAT(rA, rR, rG, rB));
}

//-----------------------------------------------------------------------------
//	Name:		operator-
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT	COLOR_FLOAT::operator-(const COLOR_FLOAT &	Color) const
{
	float rA = _r[3] - Color._r[3];
	float rR = _r[0] - Color._r[0];
	float rG = _r[1] - Color._r[1];
	float rB = _r[2] - Color._r[2];

	// No normalization here, use in export for LOD Interpolation
	return (COLOR_FLOAT(rA, rR, rG, rB));
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT &	COLOR_FLOAT::operator+=(const COLOR_FLOAT &	Color)
{
	_r[3] += Color._r[3];
	_r[0] += Color._r[0];
	_r[1] += Color._r[1];
	_r[2] += Color._r[2];

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT 	COLOR_FLOAT::operator *(const COLOR_FLOAT &	Color) const
{
	return (COLOR_FLOAT(_r[3] * Color._r[3], 
						_r[0] * Color._r[0], 
						_r[1] * Color._r[1], 
						_r[2] * Color._r[2]));
}

//-----------------------------------------------------------------------------
//	Name:		operator*=
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT & 	COLOR_FLOAT::operator *=(const COLOR_FLOAT &	Color)
{
	_r[3] *= Color._r[3];
	_r[0] *= Color._r[0];
	_r[1] *= Color._r[1];
	_r[2] *= Color._r[2];

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator*
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT 	COLOR_FLOAT::operator *(const float	rValue) const
{	
	float rA = _r[3] * rValue;
	float rR = _r[0] * rValue;
	float rG = _r[1] * rValue;
	float rB = _r[2] * rValue;

	return (COLOR_FLOAT(rA, rR, rG, rB)); 
}

//-----------------------------------------------------------------------------
//	Name:		operator*=
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT &	COLOR_FLOAT::operator*=(const float	rValue)
{
	_r[3] *= rValue;
	_r[0] *= rValue;
	_r[1] *= rValue;
	_r[2] *= rValue;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator/
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT	COLOR_FLOAT::operator/(const float	rValue) const
{
	float rInv = 1.0f / rValue;
	return (*this * rInv);
}

//-----------------------------------------------------------------------------
//	Name:		operator/=
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT &	COLOR_FLOAT::operator/=(const float	rValue)
{
	float rInv = 1.0f / rValue;
	*this *= rInv;
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		GetARGB
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::GetARGB(float &	rA,
							 float &	rR,
							 float &	rG,
							 float &	rB) const
{ 
	rA = _r[3]; 
	rR = _r[0]; 
	rG = _r[1]; 
	rB = _r[2]; 
}

//-----------------------------------------------------------------------------
//	Name:		GetRGB
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::GetRGB(float &	rR,
							float &	rG,
							float &	rB) const
{ 
	rR = _r[0]; 
	rG = _r[1]; 
	rB = _r[2]; 
}

//-----------------------------------------------------------------------------
//	Name:		GetAlpha
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
float	COLOR_FLOAT::GetAlpha() const
{ 
	return (_r[3]); 
}

//-----------------------------------------------------------------------------
//	Name:		GetRed
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
float	COLOR_FLOAT::GetRed() const
{ 
	return (_r[0]); 
}

//-----------------------------------------------------------------------------
//	Name:		GetGreen
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
float	COLOR_FLOAT::GetGreen() const
{ 
	return (_r[1]); 
}

//-----------------------------------------------------------------------------
//	Name:		GetBlue
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
float	COLOR_FLOAT::GetBlue() const
{ 
	return (_r[2]); 
}

//-----------------------------------------------------------------------------
//	Name:		GetAlphaRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	COLOR_FLOAT::GetAlphaRef() const
{
	return (_r[3]);
}

//-----------------------------------------------------------------------------
//	Name:		GetRedRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	COLOR_FLOAT::GetRedRef() const
{
	return (_r[0]);
}

//-----------------------------------------------------------------------------
//	Name:		GetGreenRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	COLOR_FLOAT::GetGreenRef() const
{
	return (_r[1]);
}

//-----------------------------------------------------------------------------
//	Name:		GetBlueRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	COLOR_FLOAT::GetBlueRef() const
{
	return (_r[2]);
}


//-----------------------------------------------------------------------------
//	Name:		SetARGB
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::SetARGB(float	rA,
							 float	rR,
							 float	rG,
							 float	rB)
{ 
	_r[3] = rA; 
	_r[0] = rR; 
	_r[1] = rG; 
	_r[2] = rB; 
}

//-----------------------------------------------------------------------------
//	Name:		SetAlpha
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::SetAlpha(float	rValue)
{ 
	_r[3] = rValue; 
}

//-----------------------------------------------------------------------------
//	Name:		SetRed
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::SetRed(float	rValue)
{ 
	_r[0] = rValue; 
}

//-----------------------------------------------------------------------------
//	Name:		SetGreen
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::SetGreen(float	rValue)
{ 
	_r[1] = rValue; 	
}

//-----------------------------------------------------------------------------
//	Name:		SetBlue
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::SetBlue(float	rValue)
{ 
	_r[2] = rValue; 
}

//-----------------------------------------------------------------------------
//	Name:		Interpolate
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::Interpolate(float			rT,
								 COLOR_FLOAT &	C1,
								 COLOR_FLOAT &	C2)
{
	_r[3] = C1._r[3] + rT * (C2._r[3] - C1._r[3]);
	_r[0] = C1._r[0] + rT * (C2._r[0] - C1._r[0]);
	_r[1] = C1._r[1] + rT * (C2._r[1] - C1._r[1]);
	_r[2] = C1._r[2] + rT * (C2._r[2] - C1._r[2]);
}

//-----------------------------------------------------------------------------
//	Name:		Clamp
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::Clamp()
{
	// We compare int value for speed optimization
	if (*(reinterpret_cast<int *>( &_r[3] )) > 0x3f800000)		*(reinterpret_cast<int *>( &_r[3] )) = 0x3f800000;
	if (*(reinterpret_cast<int *>( &_r[2] )) > 0x3f800000)		*(reinterpret_cast<int *>( &_r[2] )) = 0x3f800000;
	if (*(reinterpret_cast<int *>( &_r[0] )) > 0x3f800000)		*(reinterpret_cast<int *>( &_r[0] )) = 0x3f800000;
	if (*(reinterpret_cast<int *>( &_r[1] )) > 0x3f800000)		*(reinterpret_cast<int *>( &_r[1] )) = 0x3f800000;

	if (*(reinterpret_cast<int *>( &_r[3] )) < 0)					*(reinterpret_cast<int *>( &_r[3] )) = 0;
	if (*(reinterpret_cast<int *>( &_r[2] )) < 0)					*(reinterpret_cast<int *>( &_r[2] )) = 0;
	if (*(reinterpret_cast<int *>( &_r[0] )) < 0)					*(reinterpret_cast<int *>( &_r[0] )) = 0;
	if (*(reinterpret_cast<int *>( &_r[1] )) < 0)					*(reinterpret_cast<int *>( &_r[1] )) = 0;
}

//-----------------------------------------------------------------------------
//	Name:		IsNull
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
bool	COLOR_FLOAT::IsNull() const
{
	return ((_r[0] == 0.0f)&&(_r[1] == 0.0f)&&(_r[2] == 0.0f)&&(_r[3] == 0.0f));
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
float *	COLOR_FLOAT::ToPointer()
{ 
	return (&_r[_X_]); 
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	03-07-31:	SBE - Created
//-----------------------------------------------------------------------------
const float *	COLOR_FLOAT::ToPointer() const
{ 
	return (&_r[_X_]); 
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

