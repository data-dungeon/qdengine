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
// CLASS: ANGLE3D
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: ANGLE_3D
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_MORE_ANGLE3D_H__
#define __KMATH_MATH_MORE_ANGLE3D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KMATH
	{
		class	ANGLE3D
		{

		public:

			_INLINE_ explicit	ANGLE3D() ;

			_INLINE_			ANGLE3D( const ANGLE3D& Angle3D) ;
			_INLINE_			ANGLE3D( const float rAngleAlpha, const float rAngleBeta, const float rAngleGamma);

			/** Operator = */
			_INLINE_ ANGLE3D&		operator = (const ANGLE3D& Angle3D) ;
			/** Operator == */
			_INLINE_ bool			operator ==(const ANGLE3D& Angle3D) const;
			/** Operator != */
			_INLINE_ bool			operator !=(const ANGLE3D& Angle3D) const;
			_INLINE_ void			Set( const ANGLE3D& ) ;
			_INLINE_ void			Set( const float, const float, const float ) ;
			_INLINE_ void			GetAngles(float &, float &, float &) const ;
			_INLINE_ float			GetAlpha() const;
			_INLINE_ float			GetBeta() const;
			_INLINE_ float			GetGamma() const;
			_INLINE_ const Float32& GetAlphaRef() const;
			_INLINE_ const Float32& GetBetaRef() const;
			_INLINE_ const Float32& GetGammaRef() const;
			_INLINE_ void			SetAlpha(const float &rAlpha);
			_INLINE_ void			SetBeta(const float &rBeta);
			_INLINE_ void			SetGamma(const float &rGamma);

			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s) const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const ANGLE3D &a)
			{
				a.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, ANGLE3D &a)
			{
				a.Read(s);
				return s;
			}

		private:
			Float32			_rAlpha;
			Float32			_rBeta;
			Float32			_rGamma;
		};
	}
}

#include	INCL_KMATH(Math/Math)

#ifndef _DEBUG
#include "angle3d.inl"
#endif

#endif //__KMATH_MATH_MORE_ANGLE3D_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
