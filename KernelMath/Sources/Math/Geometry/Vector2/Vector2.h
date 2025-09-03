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
//	CLASS:	VECTOR_2
//	The VECTOR_2 class implements ...
//
//	03-03-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_VECTOR2_VECTOR2_H__
#define __KMATH_MATH_VECTOR2_VECTOR2_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

namespace	QDT
{
	namespace KMATH
	{
		class	VECTOR_2
		{
		public:
			static const VECTOR_2		ZERO;

			// Constructors & destructor
			VECTOR_2() {}
			_INLINE_				VECTOR_2(float rX, float rY);
			_INLINE_				VECTOR_2(const VECTOR_2& C);
			_INLINE_				~VECTOR_2();

			// Accessors 
			_INLINE_	void		SetX(float r);
			_INLINE_	void		SetY(float r);

			// Modifiors
			_INLINE_	float		GetX(void) const;
			_INLINE_	float		GetY(void) const;

			// Arithmetics

			// Copy
			_INLINE_	VECTOR_2&	operator=(const VECTOR_2& C);

			// Add
			_INLINE_	void		Add(const VECTOR_2 &V0, const VECTOR_2 &V1);
			_INLINE_	void		Add(const VECTOR_2 &V);
			_INLINE_	VECTOR_2	Add(const VECTOR_2& VectorAdded) const;
			_INLINE_	VECTOR_2&	AddAssign(const VECTOR_2 &V);
			_INLINE_	VECTOR_2	operator+( const VECTOR_2& VectorAdded) const;
			_INLINE_	VECTOR_2&	operator+=(const VECTOR_2& VectorAdded);

			// Sub
			_INLINE_	void		Sub(const VECTOR_2 &V0, const VECTOR_2 &V1);
			_INLINE_	void		Sub(const VECTOR_2 &V);

			// Mul
			_INLINE_	void		Mul(float r);
			_INLINE_	VECTOR_2	Mul(float rValue) const;
			_INLINE_	VECTOR_2&	MulAssign(float r);
			_INLINE_	VECTOR_2	operator*(const float rValue) const;
			_INLINE_	VECTOR_2&	operator*=(float rValue);

			// Compare
			_INLINE_	bool		operator==(const VECTOR_2& Vector) const;

			// advanced
			_INLINE_	float		Length(void);
			_INLINE_	float		SquareLength(void) const;
			_INLINE_	float		Cross(const VECTOR_2 &V) const;
			_INLINE_	float		Dot(const VECTOR_2 &V) const;
			_INLINE_	void		Normalize(void);

			// Projection of  vector P onto a vector Q
			_INLINE_	void		Proj(const VECTOR_2 &P, const VECTOR_2 &Q);

			// Component of P that is perpendicular to Q
			_INLINE_	void		Perp(const VECTOR_2 &P, const VECTOR_2 &Q);
		 
						bool		IsInsideTriangle(const VECTOR_2 Triangle[3]) const; 

			// Stream
			void		Read(IO_STREAM &Stream);
			void		Write(IO_STREAM &Stream) const;

			friend IO_STREAM&		operator << (IO_STREAM &Stream, const VECTOR_2 &V)
			{
				V.Write(Stream);
				return (Stream);
			}

			friend IO_STREAM&		operator >> (IO_STREAM &Stream, VECTOR_2 &V)
			{
				V.Read(Stream);
				return (Stream);
			}

		private:

			float _rX, _rY;
		};
	}
}

#ifndef _DEBUG
#include	"Vector2.inl"
#endif

#endif //__KMATH_MATH_VECTOR2_VECTOR2_H__
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
