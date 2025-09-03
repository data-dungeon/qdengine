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
//	CLASS:	SPLINE3
//	The SPLINE class implements ...
//
//	06-01-27:	BFA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	03-09-18:	RMA - Created
//-----------------------------------------------------------------------------
void SPLINE_3::Clear(void)
{
	SPLINE_TEMPLATE<VECTOR>::Clear();
	_vPointVectorIntersections.Clear();
	_vPlaneIntersections.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SetIntersectionStep
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE_3::SetIntersectionStep(const Float32 rStep)
{
	QDT_ASSERT(rStep>0.0f);
	_rIntersectionStep =rStep;
}

//-----------------------------------------------------------------------------
//	Name:		GetIntersectionStep
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
Float32 SPLINE_3::GetIntersectionStep(void) const
{
	return _rIntersectionStep;
}

//-----------------------------------------------------------------------------
//	Name:		GetPointVectorIntersectionValue
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE_3::GetPointVectorIntersectionValue(const Int32	nIndex) const
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
Int32 SPLINE_3::GetNbrPointVectorIntersections(void) const
{
	return _vPointVectorIntersections.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneIntersectionValue
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE_3::GetPlaneIntersectionValue(const Int32 nIndex) const
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
Int32 SPLINE_3::GetNbrPlaneIntersections(void) const
{
	return _vPlaneIntersections.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetPointVectorIntersectionCoeff
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
Float32 SPLINE_3::GetPointVectorIntersectionCoeff(const Int32	nIndex) const
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
Float32 SPLINE_3::GetPlaneIntersectionCoeff(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPlaneIntersections());
	return _vPlaneIntersections[nIndex]._rHitCoeff;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
