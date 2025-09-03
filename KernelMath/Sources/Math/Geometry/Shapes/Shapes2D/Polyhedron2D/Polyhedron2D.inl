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
//	CLASS:	POLYHEDRON_2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		POLYHEDRON2D constructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
POLYHEDRON2D::POLYHEDRON2D()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~POLYHEDRON2D destructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
POLYHEDRON2D::POLYHEDRON2D(const POINT2D &	Pt1,
						   const POINT2D &	Pt2,
						   const POINT2D &	Pt3)
{
	AddFirstThreePoints(Pt1, Pt2, Pt3);
}

//-----------------------------------------------------------------------------
//	Name:		~POLYHEDRON2D destructor
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
POLYHEDRON2D::~POLYHEDRON2D()
{
	_PointsList.Clear();
}


//-----------------------------------------------------------------------------
//	Name:		GetNbrSegments
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
int	POLYHEDRON2D::GetNbrSegments() const
{
	return (_PointsList.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPoints
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
int	POLYHEDRON2D::GetNbrPoints() const
{
	return (_PointsList.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetSegment
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT2D	POLYHEDRON2D::GetSegment(int	nNumSegment) const
{
	QDT_ASSERT(nNumSegment < GetNbrSegments());

	if (nNumSegment == GetNbrSegments() - 1)
	{
		QDT_DLIST<POINT2D>::ITERATOR FirstPoint		= _PointsList.GetLast();    
		QDT_DLIST<POINT2D>::ITERATOR SecondPoint	= _PointsList.GetHead();
		
		return (SEGMENT2D(*FirstPoint, *SecondPoint));
	}

	int i = 0;
	
	QDT_DLIST<POINT2D>::ITERATOR FirstPoint		= _PointsList.GetHead();    
	QDT_DLIST<POINT2D>::ITERATOR SecondPoint	= FirstPoint;
	++SecondPoint;

	while (i != nNumSegment)
	{
		++i;
		++FirstPoint;
		++SecondPoint;
	}

	return (SEGMENT2D(*FirstPoint, *SecondPoint));
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D	POLYHEDRON2D::GetPoint(int	nNumPoint)
{
	QDT_ASSERT(nNumPoint <= GetNbrPoints());
	
	QDT_DLIST<POINT2D>::ITERATOR ItPoint = _PointsList.GetHead();    

	int i = 0;

	while (i != nNumPoint)
	{
		++i;
		++ItPoint;
	}

	return (*ItPoint);
}

//-----------------------------------------------------------------------------
//	Name:		AddFirstThreePoints
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	POLYHEDRON2D::AddFirstThreePoints(const POINT2D &	Pt1,
										  const POINT2D &	Pt2,
										  const POINT2D &	Pt3)
{
	_PointsList.InsertTail(Pt1);
	_PointsList.InsertTail(Pt2);
	_PointsList.InsertTail(Pt3);
}

//-----------------------------------------------------------------------------
//	Name:		GetAABB
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
AABR2D	POLYHEDRON2D::GetAABR2D()
{
	int i;
	int nNbrPoints = GetNbrPoints();

	QDT_ASSERT(nNbrPoints >= 1);
	
	POINT2D Point = GetPoint(0);

	float rMaxX = Point.GetX();
	float rMaxY	= Point.GetY();
	float rMinX = Point.GetX();
	float rMinY = Point.GetY();	 

	for(i = 1 ; i < nNbrPoints ; ++i)
	{
		Point = GetPoint(i);

		if (Point.GetX() > rMaxX) rMaxX = Point.GetX();
		if (Point.GetY() > rMaxY) rMaxY = Point.GetY();
		if (Point.GetX() < rMinX) rMinX = Point.GetX();
		if (Point.GetY() < rMinY) rMinY = Point.GetY();
	}

	return (AABR2D(POINT2D(rMinX, rMinY), POINT2D(rMaxX, rMaxY)));
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
