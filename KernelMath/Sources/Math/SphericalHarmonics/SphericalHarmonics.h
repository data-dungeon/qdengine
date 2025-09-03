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
//	CLASS:	SPHERICAL_HARMONICS
//	The VECTOR_2 class implements ...
//
//	03-03-10:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SPHERICAL_HARMONICS_H__
#define __KMATH_MATH_SPHERICAL_HARMONICS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(MATRIX)
DECL_KMATH(SHROTATE_MATRIX)
DECL_KMATH(VECTOR)

#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

namespace	QDT
{
	namespace KMATH
	{
		class	SPHERICAL_HARMONICS
		{
		public:
			_INLINE_				SPHERICAL_HARMONICS();
			_INLINE_				~SPHERICAL_HARMONICS();


			_INLINE_   Float32*	 GetRedComponents();
			_INLINE_   Float32*	 GetGreenComponents();
			_INLINE_   Float32*  GetBlueComponents();



			void  ComputeSHIrradEnvMapConstants(Float32* coefficients);

			Float32  Distance(SPHERICAL_HARMONICS& shReference);
			void     Lerp(SPHERICAL_HARMONICS& a, SPHERICAL_HARMONICS& b, Float32 ratio);
			Float32  Magnitude();

			void	 InitNormal();
			void	 Init();
			void	 AddLight(const VECTOR& direction, Float32 r, Float32 g, Float32 b);
			void	 AddSoftLight(const VECTOR& direction, Float32 r, Float32 g, Float32 b);
			void	 AddConstant(Float32 r, Float32 g, Float32 b);
			void	 AddHemisphere(Float32 upRed, Float32 upGreen, Float32 upBlue,
  								   Float32 downRed, Float32 downGreen, Float32 downBlue);

			void	 AddHorizon(Float32 upRed, Float32 upGreen, Float32 upBlue);

			void	 MultiplyColor(Float32 r, Float32 g, Float32 b);

			void	 Multiply(const SPHERICAL_HARMONICS &a, const SPHERICAL_HARMONICS &b);
			void	 MultiplySimple(const SPHERICAL_HARMONICS &a, const SPHERICAL_HARMONICS &b);

			// Streams
			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s) const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const SPHERICAL_HARMONICS &m)
			{
				m.Write(s);
				return s;
			}

			friend IO_STREAM&	operator >>(IO_STREAM &s, SPHERICAL_HARMONICS &m)
			{
				m.Read(s);
				return s;
			}

			Float32 GetHigherEigenValue();

		private:

			void	 Multiply(Float32* res, const Float32* a, const Float32* b);
			void	 MultiplySimple(Float32* res, const Float32* a, const Float32* b);

			Float32 _redComponents[9];
			Float32 _greenComponents[9];
			Float32 _blueComponents[9];
		};
	}
}

#ifndef _DEBUG
#include	"SphericalHarmonics.inl"
#endif

#endif //__SPHERICAL_HARMONICS_H__
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
