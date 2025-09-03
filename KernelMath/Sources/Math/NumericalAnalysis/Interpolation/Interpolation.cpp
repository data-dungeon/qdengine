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
//	CLASS:	INTERPOLATION
//
//	03-01-30:	JJA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/NumericalAnalysis/Interpolation/Interpolation)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INTERPOLATION constructor
//	Object:		
//	03-01-30:	JJA - Created
//-----------------------------------------------------------------------------

/*
//-----------------------------------------------------------------------------
//	Name:		HermiteInterpolation
//	Object:		P1:		First Interpolation Point
//				T1:		First Point Tangent
//				P2:		Second Interpolation Point
//				T2:		Second Point Tangent
//				rCoeff: Interpolation Coeff between 0.0 and 1.0
//
//	03-01-30:	JJA - Created
//-----------------------------------------------------------------------------
POINT3D	INTERPOLATION::HermiteInterpolation(const POINT3D	P1,
											const POINT3D	P2,
											const VECTOR	T1,
											const VECTOR	T2,
											const Float32		rCoeff)
{
	QDT_ASSERT( rCoeff >= 0.0 );
	QDT_ASSERT( rCoeff <= 1.0 );

	POINT3D		Result;
	Float32		s, s2, s3;
	Float32		h1, h2, h3, h4;

	// Compute Curvilinear Absicssa
	s	= rCoeff;
	s2	= s * s;
	s3	= s * s * s;

	// Compute the 4 Basis Polynomial Values
	h1 =  2.0 * s3	- 3.0 * s2		+ 1;
	h2 = -2.0 * s3	+ 3.0 * s2;
	h3 =		s3	- 2.0 * s2	+ s;
	h4 =		s3	-		s2;

	Result=  h1 * P1 + h2 * P2 + h3 * T1 + h4 * T2;

	return	( Result );
}

//-----------------------------------------------------------------------------
//	Name:		HermiteInterpolation
//	Object:		
//	03-01-31:	JJA - Created
//-----------------------------------------------------------------------------
template < class T > static T	INTERPOLATION::HermiteInterpolation(const T		P1,
																	const T		P2,
																	const T		T1,
																	const T		T2,
																	const Float32	rCoeff)
{
	QDT_ASSERT( rCoeff >= 0.0 );
	QDT_ASSERT( rCoeff <= 1.0 );

	T		Result;
	Float32	s, s2, s3;
	Float32	h1, h2, h3, h4;

	// Compute Curvilinear Absicssa
	s	= rCoeff;
	s2	= s * s;
	s3	= s * s * s;

	// Compute the 4 Basis Polynomial Values
	h1 =  2.0 * s3	- 3.0 * s2		+ 1;
	h2 = -2.0 * s3	+ 3.0 * s2;
	h3 =		s3	- 2.0 * s2	+ s;
	h4 =		s3	-		s2;

	Result=  h1 * P1 + h2 * P2 + h3 * T1 + h4 * T2;

	return	( Result );
}
*/
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
