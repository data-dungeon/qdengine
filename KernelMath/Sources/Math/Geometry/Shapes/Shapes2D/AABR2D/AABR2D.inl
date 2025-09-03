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
//	CLASS:	AABR2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)	

//-----------------------------------------------------------------------------
//	Name:		AABR2D constructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
AABR2D::AABR2D(bool bFlagInit)
{
	if (bFlagInit)
	{
		_rXMin = QDT_FLOAT32_MAX;
		_rYMin = QDT_FLOAT32_MAX;
		_rXMax = -QDT_FLOAT32_MAX;
		_rYMax = -QDT_FLOAT32_MAX;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AABR2D constructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
AABR2D::AABR2D(const POINT2D &	Min,
	           const POINT2D &	Max)
:
_rXMin(Min.GetX()),
_rYMin(Min.GetY()),
_rXMax(Max.GetX()),
_rYMax(Max.GetY())
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AABR2D constructor
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
AABR2D::AABR2D(const AABR2D &aabb)
{
	Copy(aabb);
}

//-----------------------------------------------------------------------------
//	Name:		copy
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
void AABR2D::Copy(const AABR2D &aabb)
{
	_rXMin = aabb.GetXMin();
	_rYMin = aabb.GetYMin();
	_rXMax = aabb.GetXMax();
	_rYMax = aabb.GetYMax();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
const AABR2D& AABR2D::operator=(const AABR2D &aabb)
{
	if (this != &aabb)
		Copy(aabb);

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		~AABR2D destructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
AABR2D::~AABR2D()
{

}

//-----------------------------------------------------------------------------
//	Name:		GetXMin
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetXMin() const
{
	return (_rXMin);
}

//-----------------------------------------------------------------------------
//	Name:		GetYMin
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetYMin() const
{
	return (_rYMin);
}

//-----------------------------------------------------------------------------
//	Name:		GetXMax
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetXMax() const
{
	return (_rXMax);
}

//-----------------------------------------------------------------------------
//	Name:		GetYMax
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetYMax() const
{
	return (_rYMax);
}

//-----------------------------------------------------------------------------
//	Name:		GetXCenter
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetXCenter() const
{
	return ((_rXMax + _rXMin) / 2.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GetYCenter
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetYCenter() const
{
	return ((_rYMax + _rYMin) / 2.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GetSizeX
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetSizeX() const
{
	return (_rXMax - _rXMin);
}

//-----------------------------------------------------------------------------
//	Name:		GetSizeY
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
float	AABR2D::GetSizeY() const
{
	return (_rYMax - _rYMin);
}

//-----------------------------------------------------------------------------
//	Name:		SetXMin
//	Object:		
//	01-08-07:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::SetXMin(float	rValue)
{
	_rXMin = rValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetYMin
//	Object:		
//	01-08-07:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::SetYMin(float	rValue)
{
	_rYMin = rValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetXMax
//	Object:		
//	01-08-07:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::SetXMax(float	rValue)
{
	_rXMax = rValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetYMax
//	Object:		
//	01-08-07:	RMA - Created
//-----------------------------------------------------------------------------
void	AABR2D::SetYMax(float	rValue)
{
	_rYMax = rValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetMin
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
POINT2D	AABR2D::GetMin() const
{
	return (POINT2D(_rXMin, _rYMin));
}

//-----------------------------------------------------------------------------
//	Name:		GetMax
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
POINT2D	AABR2D::GetMax() const
{
	return (POINT2D(_rXMax, _rYMax));
}

//-----------------------------------------------------------------------------
//	Name:		Contain
//	Object:		
//	01-08-28:	JLT - Created
//-----------------------------------------------------------------------------
bool AABR2D::Contain(const POINT2D &P) const
{
	return ((_rXMin - 0.1f) <= P[_X_] && 
			P[_X_] <= (_rXMax + 0.1f) && 
			(_rYMin - 0.1f) <= P[_Y_] && 
			P[_Y_] <= (_rYMax + 0.1f));
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-29:	JLT - Created
//-----------------------------------------------------------------------------
POINT2D	AABR2D::operator[](unsigned int	index) const
{
	QDT_ASSERT(index<4);
	POINT2D P;
	
/*
	it was funny :) 
	P[_X_] = *(((float *)&_rXMin)+(((index&0x1)^((index&0x2)>>1))<<1));
	P[_Y_] = *(((float *)&_rYMin)+((index&0x2)));
*/
	switch(index)
	{
	case 0 :
		P[_X_] = _rXMin;
		P[_Y_] = _rYMin;
		break;
	case 1:
		P[_X_] = _rXMax;
		P[_Y_] = _rYMin;
		break;
	case 2 :
		P[_X_] = _rXMax;
		P[_Y_] = _rYMax;
		break;
	case 3 :
		P[_X_] = _rXMin;
		P[_Y_] = _rYMax;
		break;
	}
	
	return P;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
