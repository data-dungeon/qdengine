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
// CLASS: COLOR_FLOAT_ACCUMULATOR
//
//
//	01-04-04:	RMA - Created
//*****************************************************************************


//=============================================================================
// Name: COLOR_FLOAT_ACCUMULATOR
//	01-04-04:	RMA - Created
//=============================================================================

#ifndef __KMATH_MATH_COLOR_COLORFLOATACCUMULATOR_H__
#define __KMATH_MATH_COLOR_COLORFLOATACCUMULATOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KMATH(Math/Color/Color)
#include INCL_KMATH(Math/Color/ColorFloat)

namespace QDT
{
	namespace KMATH
	{				
		class COLOR_FLOAT_ACCUMULATOR
		{
		public:
			/** Constructors & Destructor */
			explicit	COLOR_FLOAT_ACCUMULATOR();
						COLOR_FLOAT_ACCUMULATOR(const float rA, const float rR, const float rG, const float rB);
					~COLOR_FLOAT_ACCUMULATOR();

			/** Operators */
			_INLINE_ COLOR_FLOAT_ACCUMULATOR	operator = (const COLOR_FLOAT & Color)
												{
													_rA = Color.GetAlpha();
													_rR = Color.GetRed();
													_rG = Color.GetGreen();
													_rB = Color.GetBlue();

													return (*this);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR	operator + (const COLOR_FLOAT & Color) const	
												{ 
													return COLOR_FLOAT_ACCUMULATOR(_rA + Color.GetAlpha(),
				 																_rR + Color.GetRed(),
																				_rG + Color.GetGreen(),
																				_rB + Color.GetBlue()); 
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR &	operator += (const COLOR_FLOAT & Color)			
												{ 
													_rA += Color.GetAlpha();
													_rR += Color.GetRed();
													_rG += Color.GetGreen();
													_rB += Color.GetBlue();
													return (*this); 
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR	operator + (const COLOR_FLOAT_ACCUMULATOR & Accumulator) const
												{
													return COLOR_FLOAT_ACCUMULATOR(_rA + Accumulator._rA,
																				_rR + Accumulator._rR,
																				_rG + Accumulator._rG,
																				_rB + Accumulator._rB);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR &	operator += (const COLOR_FLOAT_ACCUMULATOR & Accumulator)
												{
													_rA += Accumulator._rA;
													_rR += Accumulator._rR;
													_rG += Accumulator._rG;
													_rB += Accumulator._rB;
													return (*this);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR	operator / (float rValue) const
												{
													return COLOR_FLOAT_ACCUMULATOR(_rA / rValue,
																				_rR / rValue,
																				_rG / rValue,
																				_rB / rValue);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR &	operator /= (float rValue)
												{
													_rA /= rValue;
													_rR /= rValue;
													_rG /= rValue;
													_rB /= rValue;
													return (*this);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR	operator * (float rValue) const
												{
													return COLOR_FLOAT_ACCUMULATOR(_rA * rValue,
																				_rR * rValue,
																				_rG * rValue,
																				_rB * rValue);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR &	operator *= (float rValue)
												{
													_rA *= rValue;
													_rR *= rValue;
													_rG *= rValue;
													_rB *= rValue;
													return (*this);
												}

			_INLINE_ COLOR_FLOAT_ACCUMULATOR &	operator *= (const COLOR_FLOAT &C)
												{
													_rA *= C.GetAlpha();
													_rR *= C.GetRed();
													_rG *= C.GetGreen();
													_rB *= C.GetBlue();
													return (*this);
												}

			/** Modifiors */
			_INLINE_ void						SetARGB(float rA, float rR, float rG, float rB)
												{
													_rA = rA; _rR = rR; _rG = rG; _rB = rB;
												}

			_INLINE_ void						SetAlpha(float rAlpha)	{ _rA = rAlpha; }
			_INLINE_ void						SetRed(float rRed)		{ _rR = rRed; }
			_INLINE_ void						SetGreen(float rGreen)	{ _rG = rGreen; }
			_INLINE_ void						SetBlue(float rBlue)	{ _rB = rBlue; }

			/** Accessors */					
			_INLINE_ float						GetAlpha() const		{ return (_rA); }
			_INLINE_ float						GetRed() const			{ return (_rR); }
			_INLINE_ float						GetGreen() const		{ return (_rG); }
			_INLINE_ float						GetBlue() const			{ return (_rB); }

			/** Reset the accumulator (set to (0.0f 0.0f 0.0f 0.0f)) */
			_INLINE_ void						Reset()	
												{ 
													_rA = 0.0f; _rR = 0.0f; _rG = 0.0f; _rB = 0.0f; 
												}	

			/** Get color diffuse or specular color */
			COLOR_FLOAT							GetDiffuseColor() const;
			COLOR_FLOAT							GetSpecularColor() const;

			void								Read(IO_STREAM &s);
			void								Write(IO_STREAM &s) const;

			_INLINE_ friend IO_STREAM&			operator <<(IO_STREAM &s, const COLOR_FLOAT_ACCUMULATOR &a)
												{
													a.Write(s);
													return s;
												}

			_INLINE_ friend IO_STREAM&			operator >>(IO_STREAM &s, COLOR_FLOAT_ACCUMULATOR &a)
												{
													a.Read(s);
													return s;
												}

		private:

			/** Alpha, Red, Green & Blue components */
			float _rA, _rR, _rG, _rB;
		};
	}
}

#endif //__KMATH_MATH_COLOR_COLORFLOATACCUMULATOR_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
