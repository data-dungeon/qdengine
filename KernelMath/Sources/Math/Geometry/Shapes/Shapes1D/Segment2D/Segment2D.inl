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
//	CLASS:	SEGMENT_2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SEGMENT2D constructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT2D::SEGMENT2D(bool	bFlagInit)
:
_Point1(bFlagInit),
_Point2(bFlagInit)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SEGMENT2D constructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT2D::SEGMENT2D(const POINT2D &	Pt1,
					 const POINT2D &	Pt2)
:
_Point1(Pt1),
_Point2(Pt2)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~SEGMENT_2D destructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT2D::~SEGMENT2D()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint1
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
const POINT2D &	SEGMENT2D::GetPoint1() const
{
	return (_Point1);
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint2
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
const POINT2D &	SEGMENT2D::GetPoint2() const
{
	return (_Point2);
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint1
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT2D::SetPoint1(const POINT2D &	Pt)
{
	_Point1 = Pt;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint2
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT2D::SetPoint2(const POINT2D &	Pt)
{
	_Point2 = Pt;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
