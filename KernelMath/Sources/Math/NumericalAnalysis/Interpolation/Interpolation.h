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
//	The INTERPOLATION class implements ...
//
//	03-01-30:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_INTERPOLATION_INTERPOLATION_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_INTERPOLATION_INTERPOLATION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace	QDT
{
	namespace KMATH
	{
		template<class T> 
		class	INTERPOLATION
		{
		public:
			
			//static POINT3D HermiteInterpolation( const POINT3D P1, const POINT3D P2, const VECTOR T1, const VECTOR T2, const Float32 rCoeff );
			static T HermiteInterpolation( const T& P1, const T& P2, const T& T1, const T& T2, const Float32 rCoeff )
			{
				QDT_ASSERT( rCoeff >= 0.0f );
				QDT_ASSERT( rCoeff <= 1.0f );

				T		Result;
				Float32	s, s2, s3;
				Float32	h1, h2, h3, h4;

				// Compute Curvilinear Absicssa
				s	= rCoeff;
				s2	= s * s;
				s3	= s * s * s;

				// Compute the 4 Basis Polynomial Values
				h1 =  2.0f * s3	- 3.0f * s2		+ 1.0f;
				h2 = -2.0f * s3	+ 3.0f * s2;
				h3 =		 s3	- 2.0f * s2	+ s;
				h4 =		 s3	-		 s2;

				// 2003-04-30 : JPE - Swap order in the mul (that shouldn't affect anything, except for vc7.1 ;)
				// Result=  h1 * P1 + h2 * P2 + h3 * T1 + h4 * T2;
				Result=  P1 * h1 + P2 * h2 + T1 * h3 + T2 * h4;

				return	( Result );
			}

			static T GetHermiteTangent( const T& P1, const T& P2, const T& T1, const T& T2, const Float32 rCoeff )
			{
				QDT_ASSERT( rCoeff >= 0.0f );
				QDT_ASSERT( rCoeff <= 1.0f );

				T		Result;
				Float32	s, s2;
				Float32	h1, h2, h3, h4;

				// Compute Curvilinear Absicssa
				s	= rCoeff;
				s2	= s * s;

				// Compute the 4 Basis Polynomial Values
				h1 =  6.0f * s2	- 6.0f * s;
				h2 = -6.0f * s2	+ 6.0f * s;
				h3 =  3.0f * s2	- 4.0f * s	+ 1.0f;
				h4 =  3.0f * s2	- 2.0f * s;

				// 2003-04-30 : JPE - Swap order in the mul (that shouldn't affect anything, except for vc7.1 ;)
				// Result=  h1 * P1 + h2 * P2 + h3 * T1 + h4 * T2;
				Result=  P1 * h1 + P2 * h2 + T1 * h3 + T2 * h4;

				return	( Result );
			}

			
		private:
			INTERPOLATION()
			{
				QDT_NOT_IMPLEMENTED();
			}

			~INTERPOLATION()
			{
				QDT_NOT_IMPLEMENTED();
			}
		};
	}
}

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_INTERPOLATION_INTERPOLATION_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
