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
//	CLASS:	TCB_SPLINE
//
//	03-01-31:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/NumericalAnalysis/Splines/TCB_Spline)

//-----------------------------------------------------------------------------
//	Name:		TCB_SPLINE constructor
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
TCB_SPLINE::TCB_SPLINE()
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		TCB_SPLINE constructor
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
TCB_SPLINE::TCB_SPLINE(const TCB_SPLINE & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		TCB_SPLINE destructor
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
TCB_SPLINE::~TCB_SPLINE()
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
TCB_SPLINE &	TCB_SPLINE::operator=(const TCB_SPLINE & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::Copy(const TCB_SPLINE & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		AddHermitePoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::AddHermitePoint(const HERMITE_POINT &	HermitePoint)
{
	_vPoints.PushTail( HermitePoint );
}

//-----------------------------------------------------------------------------
//	Name:		AddHermitePoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::AddHermitePoint(const VECTOR&	Point)
{
	AddHermitePoint( Point, VECTOR( 0.0, 0.0, 0.0 ), VECTOR( 0.0, 0.0, 0.0 ), 0.0, 0.0, 0.0 );
}

//-----------------------------------------------------------------------------
//	Name:		AddHermitePoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::AddHermitePoint(const VECTOR&	Point,
									const VECTOR &	TangentIn,
									const VECTOR &	TangentOut,
									const float		rTension)
{
	AddHermitePoint( Point, TangentIn, TangentOut, 0.0, 0.0, 0.0 );
}

//-----------------------------------------------------------------------------
//	Name:		AddHermitePoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::AddHermitePoint(const VECTOR &	Point,
									const VECTOR &	TangentIn,
									const VECTOR &	TangentOut,
									const float		rTension,
									const float		rContinuity,
									const float		rBias)
{
	HERMITE_POINT	HermitePoint;
	
	HermitePoint.Point			= Point;
	HermitePoint.TangentIn		= TangentIn;
	HermitePoint.TangentOut		= TangentOut;
	HermitePoint.rTension		= rTension;
	HermitePoint.rContinuity	= rContinuity;
	HermitePoint.rBias			= rBias;

	AddHermitePoint( HermitePoint );
}

//-----------------------------------------------------------------------------
//	Name:		GetHermitePoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
TCB_SPLINE::HERMITE_POINT	TCB_SPLINE::GetHermitePoint(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex] );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPoints
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
int	TCB_SPLINE::GetNbrPoints() const
{
	return	( _vPoints.GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::SetPoint(const int			nIndex,
							 const VECTOR &	Point)
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	_vPoints[nIndex].Point = Point;
}

//-----------------------------------------------------------------------------
//	Name:		SetTangentIn
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::SetTangentIn(const int		nIndex,
								 const VECTOR &	Tangent)
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	_vPoints[nIndex].TangentIn = Tangent;
}

//-----------------------------------------------------------------------------
//	Name:		SetTangentOut
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::SetTangentOut(const int			nIndex,
								  const VECTOR &	Tangent)
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	_vPoints[nIndex].TangentOut = Tangent;
}

//-----------------------------------------------------------------------------
//	Name:		SetTension
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::SetTension(const int	nIndex,
							   const float	rTension)
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	_vPoints[nIndex].rTension = rTension;
}

//-----------------------------------------------------------------------------
//	Name:		SetContinuity
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::SetContinuity(const int		nIndex,
								  const float	rContinuity)
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	_vPoints[nIndex].rContinuity = rContinuity;
}

//-----------------------------------------------------------------------------
//	Name:		SetBias
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
void	TCB_SPLINE::SetBias(const int	nIndex,
							const float	rBias)
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	_vPoints[nIndex].rBias = rBias;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR	TCB_SPLINE::GetPoint(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex].Point );
}

//-----------------------------------------------------------------------------
//	Name:		GetTangentIn
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR	TCB_SPLINE::GetTangentIn(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex].TangentIn );
}

//-----------------------------------------------------------------------------
//	Name:		GetTangentOut
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR	TCB_SPLINE::GetTangentOut(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex].TangentOut );
}

//-----------------------------------------------------------------------------
//	Name:		GetTension
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
float	TCB_SPLINE::GetTension(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex].rTension );
}

//-----------------------------------------------------------------------------
//	Name:		GetContinuity
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
float	TCB_SPLINE::GetContinuity(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex].rContinuity );
}

//-----------------------------------------------------------------------------
//	Name:		GetBias
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
float	TCB_SPLINE::GetBias(const int	nIndex) const
{
	QDT_ASSERT( nIndex < GetNbrPoints() );
	return	( _vPoints[nIndex].rBias );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
