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
// CLASS: COLOR
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_COLOR_COLOR_H__
#define __KMATH_MATH_COLOR_COLOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
#include INCL_KMATH(Math/Color/ColorFloat)

namespace QDT
{
	namespace KMATH
	{				
		class	COLOR_INT;

		class	COLOR
		{
		public:
			// Commonly used colors...
			static const COLOR	WHITE;
			static const COLOR	GREY;
			static const COLOR	BLACK;
			static const COLOR	RED;
			static const COLOR	GREEN;
			static const COLOR	BLUE;
			static const COLOR	YELLOW;
			static const COLOR	MAGNETA;
			static const COLOR	CYAN;
			static const COLOR	ORANGE;
			static const COLOR	VIOLET;
			static const COLOR	DARK_RED;
			static const COLOR	DARK_GREEN;
			static const COLOR	DARK_BLUE;
			static const COLOR	DARK_GREY;

			/**	Constructors & Destructor */
			explicit				COLOR(const Bool bFlagInit = true);
									COLOR(const COLOR & Color);
									COLOR(const UInt32 nARGB);
									COLOR(const UChar cA, const UChar cR, const UChar cG, const UChar cB);
									COLOR(const UChar cR, const UChar cG, const UChar cB);
									COLOR(const COLOR_FLOAT & C);
									COLOR(const COLOR_INT & C);
									~COLOR();

			/** Operators */
			_INLINE_ bool			operator ==(const COLOR& Color) const
									{
										return (_nInt32 == Color._nInt32);
									}

			/** Operator != */
			_INLINE_ bool			operator !=(const COLOR& Color) const
									{
										return (_nInt32 != Color._nInt32);
									}

			/** Operator = */
			_INLINE_ COLOR &		operator = (const COLOR_FLOAT &Color) 
									{
										COLOR_FLOAT ClampedColor = Color;
										ClampedColor.Clamp();

										_cA = static_cast<UChar>(ClampedColor.GetAlpha() * 255);
										_cR = static_cast<UChar>(ClampedColor.GetRed() * 255);
										_cG = static_cast<UChar>(ClampedColor.GetGreen() * 255);
										_cB = static_cast<UChar>(ClampedColor.GetBlue() * 255);

										return (*this);
									}

			/** Opetator - */ 
			_INLINE_ COLOR 			operator - (const COLOR &C) const
									{
										Int32 nA = _cA - C._cA;
										Int32 nR = _cR - C._cR;
										Int32 nG = _cG - C._cG;
										Int32 nB = _cB - C._cB;

										MATH::ClipMin(nA,0);
										MATH::ClipMin(nR,0);
										MATH::ClipMin(nG,0);
										MATH::ClipMin(nB,0);

										UChar cA = static_cast<UChar>(nA);
										UChar cR = static_cast<UChar>(nR);
										UChar cG = static_cast<UChar>(nG);
										UChar cB = static_cast<UChar>(nB);

										return (COLOR(cA, cR, cG, cB));
									}

			/** Operator + */
			_INLINE_ COLOR 			operator + (const COLOR &C) const
									{
										Int32 nA = _cA + C._cA;
										Int32 nR = _cR + C._cR;
										Int32 nG = _cG + C._cG;
										Int32 nB = _cB + C._cB;
										
										MATH::ClipMax(nA,255);
										MATH::ClipMax(nR,255);
										MATH::ClipMax(nG,255);
										MATH::ClipMax(nB,255);

										UChar cA = static_cast<UChar>(nA);
										UChar cR = static_cast<UChar>(nR);
										UChar cG = static_cast<UChar>(nG);
										UChar cB = static_cast<UChar>(nB);

										return (COLOR(cA, cR, cG, cB));
									}

			/** Operator * */  
			_INLINE_ COLOR			operator * (Float32 r) const
									{
										UInt32 nFactor = static_cast<UChar>(r * 255.0f);
										
										Int32 nA = (_cA * nFactor) >> 8;
										Int32 nR = (_cR * nFactor) >> 8;
										Int32 nG = (_cG * nFactor) >> 8;
										Int32 nB = (_cB * nFactor) >> 8;

										MATH::ClipMax(nA,255);
										MATH::ClipMax(nR,255);
										MATH::ClipMax(nG,255);
										MATH::ClipMax(nB,255);

										UChar cA = static_cast<UChar>(nA);
										UChar cR = static_cast<UChar>(nR);
										UChar cG = static_cast<UChar>(nG);
										UChar cB = static_cast<UChar>(nB);

										return (COLOR(cA, cR, cG, cB));
									}

			/** Operator / */
			_INLINE_ COLOR			operator / (Float32 r) const
									{								
										return (*this * (1.0f / r));
									}

			/** Linear interpolation */
			_INLINE_ void			Interpolate(const Float32 rT, const COLOR & C1, const COLOR & C2)
									{
										QDT_ASSERT((rT >= 0.0f) && (rT <= 1.0f));

										Int32 da = C2._cA - C1._cA;
										Int32 dr = C2._cR - C1._cR;
										Int32 dg = C2._cG - C1._cG;
										Int32 db = C2._cB - C1._cB;
										Int32 a = C1._cA;
										Int32 r = C1._cR;
										Int32 g = C1._cG;
										Int32 b = C1._cB;

										_cA = static_cast<Char> (a + rT * (da));
										_cR = static_cast<Char> (r + rT * (dr));
										_cG = static_cast<Char> (g + rT * (dg));
										_cB = static_cast<Char> (b + rT * (db));
									}

			/** Same as above, but check if values are between 0 & 255 */
			_INLINE_ void			InterpolateSat(const Float32 rT, const COLOR & C1, const COLOR & C2)
									{
										Int32 da = C2._cA - C1._cA;
										Int32 dr = C2._cR - C1._cR;
										Int32 dg = C2._cG - C1._cG;
										Int32 db = C2._cB - C1._cB;
										Int32 a = C1._cA;
										Int32 r = C1._cR;
										Int32 g = C1._cG;
										Int32 b = C1._cB;

										Int32 cA = a + ((Int32)rT) * da;
										Int32 cR = r + ((Int32)rT) * dr;
										Int32 cG = g + ((Int32)rT) * dg;
										Int32 cB = b + ((Int32)rT) * db;

										_cA = static_cast<Char>(cA);
										_cR = static_cast<Char>(cR);
										_cG = static_cast<Char>(cG);
										_cB = static_cast<Char>(cB);

										MATH::Clip(cA,0,255);
										MATH::Clip(cR,0,255);
										MATH::Clip(cG,0,255);
										MATH::Clip(cB,0,255);
									}

			/** Accessors */
			_INLINE_ void			GetARGB(UInt32 & nA, UInt32 & nR, UInt32 & nG, UInt32 & nB) const
									{
										nA = _cA;
										nR = _cR;
										nG = _cG;
										nB = _cB;
									}

			_INLINE_ UInt32			GetARGBValue(void) const
									{
										UInt32	nARGB;

										nARGB  = (UInt32)(_cA) << 24;
										nARGB |= (UInt32)(_cR) << 16;
										nARGB |= (UInt32)(_cG) <<  8;
										nARGB |= (UInt32)(_cB);

										return (nARGB);
									}

			_INLINE_ UInt32			GetABGRValue() const
									{
										UInt32	nABGR;

										nABGR  = (UInt32)(_cA) << 24;
										nABGR |= (UInt32)(_cB) << 16;
										nABGR |= (UInt32)(_cG) <<  8;
										nABGR |= (UInt32)(_cR);

										return (nABGR);
									}

			_INLINE_ UInt32			GetRGBAValue() const
									{
										UInt32	nRGBA;

										nRGBA  = (UInt32)(_cR) << 24;
										nRGBA |= (UInt32)(_cG) << 16;
										nRGBA |= (UInt32)(_cB) <<  8;
										nRGBA |= (UInt32)(_cA);

										return (nRGBA);
									}

			_INLINE_ UChar			GetAlpha() const			{ return (_cA); }
			_INLINE_ UChar			GetRed() const				{ return (_cR); }
			_INLINE_ UChar			GetGreen() const			{ return (_cG); }
			_INLINE_ UChar			GetBlue() const				{ return (_cB); }
			_INLINE_ const UChar&	GetAlphaRef() const			{ return (_cA); }
			_INLINE_ const UChar&	GetRedRef() const			{ return (_cR); }
			_INLINE_ const UChar&	GetGreenRef() const			{ return (_cG); }
			_INLINE_ const UChar&	GetBlueRef() const			{ return (_cB); }
			
			/** Modifiors */
			_INLINE_ void			SetIntegerValue(UInt32 nARGB) 						{ _nInt32 = nARGB; }
			_INLINE_ void			SetARGB(UChar cA, UChar cR, UChar cG, UChar cB)		{ _cA = cA; _cR = cR; _cG = cG; _cB = cB; }	
			_INLINE_ void			SetRGBA(UChar cR, UChar cG, UChar cB, UChar cA)		{ _cR = cR; _cG = cG; _cB = cB; _cA = cA; }	
			_INLINE_ void			SetAlpha(UChar cAlpha)								{ _cA = cAlpha; }
			_INLINE_ void			SetRed(UChar cRed)									{ _cR = cRed; }
			_INLINE_ void			SetGreen(UChar cGreen)								{ _cG = cGreen; }
			_INLINE_ void			SetBlue(UChar cBlue)								{ _cB = cBlue; }

			_INLINE_ COLOR			GetInverse() const			{ return COLOR(0xFFFFFFFF - _nInt32); }
			_INLINE_ void			Inverse()					{ _nInt32 = 0xFFFFFFFF - _nInt32; }

			void					Read(IO_STREAM &s);
			void					Write(IO_STREAM &s) const;
			friend IO_STREAM&		operator <<(IO_STREAM &s, const COLOR &a)
									{
										a.Write(s);
										return s;
									}
			friend IO_STREAM&		operator >>(IO_STREAM &s, COLOR &a)
									{
										a.Read(s);
										return s;
									}

		private:
			union
			{
				UInt32	_nInt32;

#if defined PLATFORM_PC
				struct
				{
					UChar	_cB;
					UChar	_cG;
					UChar	_cR;
					UChar	_cA;
				};
#elif (defined PLATFORM_PS3) || (defined PLATFORM_X360)
				struct
				{
					UChar	_cA;
					UChar	_cR;
					UChar	_cG;
					UChar	_cB;
				};
#else
#error	UNKNOWN PLATFORM
#endif
			};
		};
	}
}

#endif //__KMATH_MATH_COLOR_COLOR_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
