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
//	CLASS:	SPLINE
//
//	04-03-15:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	03-09-18:	RMA - Created
//-----------------------------------------------------------------------------
void SPLINE::Clear(void)
{
	_rTotalLength =-1.0f;
	_vPointVectorIntersections.Clear();
	_vPlaneIntersections.Clear();
	_vHermitePoints.Clear();
	_vSplineInfo.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		AddPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::AddPoint(const VECTOR& Point)
{
	HERMITE_POINT	HermitePoint;

	HermitePoint._Point			=Point;
	HermitePoint._InTangent		=VECTOR::ZERO;
	HermitePoint._OutTangent	=VECTOR::ZERO;

	AddHermitePoint(HermitePoint);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPoints
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
Int32 SPLINE::GetNbrPoints(void) const
{
	return	_vHermitePoints.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
const VECTOR& SPLINE::GetPoint(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return	(_vHermitePoints[nIndex]._Point);
}

//-----------------------------------------------------------------------------
//	Name:		GetInTangent
//	Object:		
//	04-11-19:	JJA - Created
//-----------------------------------------------------------------------------
const VECTOR& SPLINE::GetInTangent(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return _vHermitePoints[nIndex]._InTangent;
}

//-----------------------------------------------------------------------------
//	Name:		GetOutTangent
//	Object:		
//	04-11-19:	JJA - Created
//-----------------------------------------------------------------------------
const VECTOR& SPLINE::GetOutTangent(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return _vHermitePoints[nIndex]._OutTangent;
}

//-----------------------------------------------------------------------------
//	Name:		AddHermitePoint
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::AddHermitePoint(const HERMITE_POINT& HermitePoint)
{
	_vHermitePoints.PushTail(HermitePoint);
	if (GetNbrPoints()>1)
	{
		ComputeLenghts();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetHermitePoint
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
SPLINE::HERMITE_POINT SPLINE::GetHermitePoint(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return _vHermitePoints[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		GetLength
//	Object:		
//	03-07-31:	JJA - Created
//-----------------------------------------------------------------------------
Float32	SPLINE::GetLength(void) const
{
	QDT_ASSERT_MSG(_rTotalLength>=0.0f,"YOU MUST CALL ComputeLenghts() before using this method !");
	return	_rTotalLength;
}

//-----------------------------------------------------------------------------
//	Name:		SetIntersectionStep
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::SetIntersectionStep(const Float32 rStep)
{
	QDT_ASSERT(rStep>0.0f);
	_rIntersectionStep =rStep;
}

//-----------------------------------------------------------------------------
//	Name:		GetIntersectionStep
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
Float32 SPLINE::GetIntersectionStep(void) const
{
	return _rIntersectionStep;
}

//-----------------------------------------------------------------------------
//	Name:		GetPointVectorIntersectionValue
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR	SPLINE::GetPointVectorIntersectionValue(const Int32	nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPointVectorIntersections());
	return _vPointVectorIntersections[nIndex]._HitPoint;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPointVectorIntersections
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
Int32 SPLINE::GetNbrPointVectorIntersections(void) const
{
	return _vPointVectorIntersections.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneIntersectionValue
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE::GetPlaneIntersectionValue(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPlaneIntersections());
	return _vPlaneIntersections[nIndex]._HitPoint;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPlaneIntersections
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
Int32 SPLINE::GetNbrPlaneIntersections(void) const
{
	return _vPlaneIntersections.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetPointVectorIntersectionCoeff
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
Float32 SPLINE::GetPointVectorIntersectionCoeff(const Int32	nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPointVectorIntersections());
	return _vPointVectorIntersections[nIndex]._rHitCoeff;
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneIntersectionCoeff
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
Float32 SPLINE::GetPlaneIntersectionCoeff(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPlaneIntersections());
	return _vPlaneIntersections[nIndex]._rHitCoeff;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
