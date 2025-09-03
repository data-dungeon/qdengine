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
//	CLASS:	TRIANGLE_2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

#include	INCL_KMATH(Math/Math)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TRIANGLE2D constructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
TRIANGLE2D::TRIANGLE2D(bool	bFlagInit)
:
_Point1(bFlagInit),
_Point2(bFlagInit),
_Point3(bFlagInit)
{

}

//-----------------------------------------------------------------------------
//	Name:		TRIANGLE2D constructor
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
TRIANGLE2D::TRIANGLE2D(const POINT2D &	Point1,
					   const POINT2D &	Point2,
					   const POINT2D &	Point3)
:
_Point1(Point1),
_Point2(Point2),
_Point3(Point3)
{

}

//-----------------------------------------------------------------------------
//	Name:		TRIANGLE2D constructor
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
TRIANGLE2D::TRIANGLE2D(const TRIANGLE2D &T)
{
	Copy(T);
}
//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
void TRIANGLE2D::Copy(const TRIANGLE2D &T)
{
	_Point1 = T.GetPoint1();
	_Point2 = T.GetPoint2();
	_Point3 = T.GetPoint3();
}


//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
const TRIANGLE2D &	TRIANGLE2D::operator=(const TRIANGLE2D &T)
{
	if (this != &T)
		Copy(T);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
const POINT2D&	TRIANGLE2D::operator[](unsigned int	index) const 
{
	QDT_ASSERT(index<3);
	return (*(((POINT2D*)&_Point1)+index));
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
POINT2D& TRIANGLE2D::operator[](unsigned int	index)
{
	QDT_ASSERT(index<3);
	return (*(((POINT2D*)&_Point1)+index));
}


//-----------------------------------------------------------------------------
//	Name:		~TRIANGLE2D destructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
TRIANGLE2D::~TRIANGLE2D()
{

}

//-----------------------------------------------------------------------------
//	Name:		SetPoint1
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE2D::SetPoint1(const POINT2D &	Pt)
{
	_Point1 = Pt;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint2
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE2D::SetPoint2(const POINT2D &	Pt)
{
	_Point2 = Pt;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint3
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
void	TRIANGLE2D::SetPoint3(const POINT2D &	Pt)
{
	_Point3 = Pt;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint1
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
const POINT2D &	TRIANGLE2D::GetPoint1() const
{
	return (_Point1);
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint2
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
const POINT2D &	TRIANGLE2D::GetPoint2() const
{
	return (_Point2);
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint3
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
const POINT2D &	TRIANGLE2D::GetPoint3() const
{
	return (_Point3);
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
float TRIANGLE2D::GetArea() const
{
	return (0.5f * ((_Point2[_X_] - _Point1[_X_]) * (_Point3[_Y_] - _Point1[_Y_]) -
					(_Point2[_Y_] - _Point1[_Y_]) * (_Point3[_X_] - _Point1[_X_])));
}

//-----------------------------------------------------------------------------
//	Name:		GetXMin
//	Object:		
//	01-08-28:	JLT - Created
//-----------------------------------------------------------------------------
float TRIANGLE2D::GetXMin() const
{
	return ((_Point1[_X_] < _Point2[_X_]) ? 
				((_Point1[_X_] < _Point3[_X_]) ? _Point1[_X_] : _Point3[_X_]) : 
				((_Point2[_X_] < _Point3[_X_]) ? _Point2[_X_] : _Point3[_X_]));
}

//-----------------------------------------------------------------------------
//	Name:		GetXMax
//	Object:		
//	01-08-28:	JLT - Created
//-----------------------------------------------------------------------------
float TRIANGLE2D::GetXMax() const
{
	return ((_Point1[_X_] > _Point2[_X_]) ? 
				((_Point1[_X_] > _Point3[_X_]) ? _Point1[_X_] : _Point3[_X_]) : 
				((_Point2[_X_] > _Point3[_X_]) ? _Point2[_X_] : _Point3[_X_]));
}

//-----------------------------------------------------------------------------
//	Name:		GetYMin
//	Object:		
//	01-08-28:	JLT - Created
//-----------------------------------------------------------------------------
float TRIANGLE2D::GetYMin() const
{
	return ((_Point1[_Y_] < _Point2[_Y_]) ? 
				((_Point1[_Y_] < _Point3[_Y_]) ? _Point1[_Y_] : _Point3[_Y_]) : 
				((_Point2[_Y_] < _Point3[_Y_]) ? _Point2[_Y_] : _Point3[_Y_]));
}

//-----------------------------------------------------------------------------
//	Name:		GetYMax
//	Object:		
//	01-08-28:	JLT - Created
//-----------------------------------------------------------------------------
float TRIANGLE2D::GetYMax() const
{
	return ((_Point1[_Y_] > _Point2[_Y_]) ? 
				((_Point1[_Y_] > _Point3[_Y_]) ? _Point1[_Y_] : _Point3[_Y_]) : 
				((_Point2[_Y_] > _Point3[_Y_]) ? _Point2[_Y_] : _Point3[_Y_]));
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	01-08-29:	JLT - Created
//-----------------------------------------------------------------------------
const POINT2D* TRIANGLE2D::GetBuffer() const
{
	return (&_Point1);
}

//-----------------------------------------------------------------------------
//	Name:		IsDegenerated
//	Object:		
//	01-11-13:	RMA - Created
//-----------------------------------------------------------------------------
bool	TRIANGLE2D::IsDegenerated() const
{
	if ((_Point1 == _Point2) || (_Point1 == _Point3) || (_Point2 == _Point3))
	{
		return (true);
	}

	return (false);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
