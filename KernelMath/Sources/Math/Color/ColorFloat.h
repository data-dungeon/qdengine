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
// CLASS: COLOR_FLOAT
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: COLOR_FLOAT
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_COLOR_COLORFLOAT_H__
#define __KMATH_MATH_COLOR_COLORFLOAT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(COLOR)

namespace QDT
{
	namespace KMATH
	{				
		class	COLOR_FLOAT
		{
		public:
			/**	Constructors & destructor */
			_INLINE_				COLOR_FLOAT();
			_INLINE_				COLOR_FLOAT(const bool bFlagInit);
			_INLINE_				COLOR_FLOAT(const COLOR_FLOAT & C);
									COLOR_FLOAT(const COLOR & Color);
			_INLINE_				COLOR_FLOAT(const float rA, const float rR, const float rG, const float rB);
			_INLINE_			   ~COLOR_FLOAT();

			/** Operators */
			_INLINE_ COLOR_FLOAT &	operator = (const COLOR_FLOAT& Color);
			_INLINE_ bool			operator ==(const COLOR_FLOAT& Color) const;
			_INLINE_ bool			operator !=(const COLOR_FLOAT& Color) const;
			_INLINE_ COLOR_FLOAT	operator + (const COLOR_FLOAT & Color) const;
			_INLINE_ COLOR_FLOAT	operator - (const COLOR_FLOAT & Color) const;
			_INLINE_ COLOR_FLOAT &	operator += (const COLOR_FLOAT & Color);
			_INLINE_ COLOR_FLOAT	operator * (const COLOR_FLOAT & Color) const;
			_INLINE_ COLOR_FLOAT &	operator *= (const COLOR_FLOAT & Color);
			_INLINE_ COLOR_FLOAT	operator * (const float rValue) const;
			_INLINE_ COLOR_FLOAT &	operator *= (const float rValue);
			_INLINE_ COLOR_FLOAT	operator / (const float rValue) const;
			_INLINE_ COLOR_FLOAT &	operator /= (const float rValue);

			/** Accessors */
			_INLINE_ void			GetARGB(float & rA, float & rR, float & rG, float & rB) const;
			_INLINE_ void			GetRGB(float & rR, float & rG, float & rB) const;
			_INLINE_ float			GetAlpha() const;
			_INLINE_ float			GetRed() const;
			_INLINE_ float			GetGreen() const;
			_INLINE_ float			GetBlue() const;
			_INLINE_ const Float32&	GetAlphaRef() const;
			_INLINE_ const Float32&	GetRedRef() const;
			_INLINE_ const Float32&	GetGreenRef() const;
			_INLINE_ const Float32&	GetBlueRef() const;

			/** Modifiors */
			_INLINE_ void			SetARGB(float rA, float rR, float rG, float rB);
			_INLINE_ void			SetAlpha(float rValue);
			_INLINE_ void			SetRed(float rValue);
			_INLINE_ void			SetGreen(float rValue);
			_INLINE_ void			SetBlue(float rValue);

			/** Interpolation between two colors */ 
			_INLINE_ void			Interpolate(float rT, COLOR_FLOAT & C1, COLOR_FLOAT & C2);

			_INLINE_ void			Clamp();
			_INLINE_ bool			IsNull() const;

			_INLINE_ float*			ToPointer();
			_INLINE_ const float*	ToPointer() const;

			void					Read(IO_STREAM &s);
			void					Write(IO_STREAM &s) const;

			friend IO_STREAM&		operator <<(IO_STREAM &s, const COLOR_FLOAT &a)
									{
										a.Write(s);
										return s;
									}

			friend IO_STREAM&		operator >>(IO_STREAM &s, COLOR_FLOAT &a)
									{
										a.Read(s);
										return s;
									}

		private:

			union
			{
				struct 
				{
					Float32		_rR, _rG, _rB, _rA;
				};

				Float32		_r[4];
			};	
		};
	}
}

#include INCL_KMATH(Math/Math)

#ifndef _DEBUG
#include "ColorFloat.inl"
#endif

#endif //__KMATH_MATH_COLOR_COLORFLOAT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
