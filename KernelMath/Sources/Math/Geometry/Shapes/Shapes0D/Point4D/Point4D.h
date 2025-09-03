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
//	CLASS:	POINT4D
//	The POINT4D class implements ...
//
//	01-09-26:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES0D_POINT4D_POINT4D_H__
#define __KMATH_MATH_SHAPES_SHAPES0D_POINT4D_POINT4D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KMATH
	{
		class	POINT4D
		{
		public:
			/** Constructors & destructor */
			_INLINE_			POINT4D();
			_INLINE_			POINT4D(const POINT4D & Point);
			_INLINE_			POINT4D(const float rX, const float rY, const float rZ, const float rW);
			_INLINE_			POINT4D(const VECTOR & Vector);

			/** Operators */
			_INLINE_ POINT4D&	operator = (const POINT4D& Point);
			_INLINE_ bool		operator ==(const POINT4D& Point) const;
			_INLINE_ bool		operator !=(const POINT4D& Point) const;
			

			/** Accessors. */
			_INLINE_ float		GetX() const;
			_INLINE_ float		GetY() const;
			_INLINE_ float		GetZ() const;
			_INLINE_ float		GetW() const;
			_INLINE_ void		GetXYZW(float &rX, float &rY, float &rZ, float &rW) const;

			/** Modifiers. */
			_INLINE_ void		SetX(const float rX);
			_INLINE_ void		SetY(const float rY);
			_INLINE_ void		SetZ(const float rZ);
			_INLINE_ void		SetW(const float rW);
			_INLINE_ void		SetXYZW(const float rX, const float rY, const float rZ, const float rW) ;


			//=========================================================================
			// Below are the arithmetic method, that are safer than the operators
			// but less readable.
			//=========================================================================

			/** Add two points. */
			_INLINE_ void		Add(const POINT4D &Point);
			_INLINE_ void		Add4(const POINT4D &Point);
			/** Add a vector to a point. */
			_INLINE_ void		Add(const VECTOR &Vector);
			/** Add values to the points coordinates. */
			_INLINE_ void		Add(const float rX, const float rY, const float rZ);
			/** Set a point from the sum of a point and a vector. */
			_INLINE_ void		Add(const POINT4D &Point, const VECTOR &Vector);
			/** Remove a point from another. */
			_INLINE_ void		Sub(const POINT4D &Point);
			_INLINE_ void		Sub4(const POINT4D &Point);
			/** Remove a vector from a point. */
			_INLINE_ void		Sub(const VECTOR &Vector);
			/** Change the sign of the point coordinates. */
			_INLINE_ void		Neg();
			/** Multiply each point coordinates by a value. */
			_INLINE_ void		Mul( float rValue);
			/** Multiply a point by a VECTOR. */
			_INLINE_ void		Mul(const VECTOR &Scale);
			/** Multiply a point by a Point4D. */
			_INLINE_ void		Mul(const POINT4D &Point);
			_INLINE_ void		Mul(const POINT4D &Point1, const POINT4D &Point2);

			_INLINE_ POINT4D	MulW(float rValue);
			_INLINE_ float		Dot(const POINT4D & Point);

			//=========================================================================


			//=========================================================================
			// Below are the operators that might be slower because of unecessary
			// objects allocation, but their use makes the code more readable.
			//=========================================================================

			/** Operator + */
			_INLINE_ POINT4D	operator +(const POINT4D & Point) const;
			/** Operator += */
			_INLINE_ POINT4D &  operator +=(const POINT4D & Point) ;
			/** Operator that allow to write: VECTOR V = Pt1 - Pt2. */
			_INLINE_ VECTOR		operator -(const POINT4D& Point) const;
			/** Substract a vector from point. */
			_INLINE_ POINT4D	operator -(const VECTOR& Vector) const;
			/** Change the sign of the point coordinates. You can write: Pt2 = -Pt1. */
			_INLINE_ POINT4D	operator -() const;
			/** Remove a point from another. Same as Sub(const POINT4D &Point). You can write: Pt1 -= Pt2. */
			_INLINE_ POINT4D &	operator -=(const POINT4D& Point);
			/** Remove a vector from a point. Same as Sub(const VECTOR &Vector). You can write: Pt -= V. */
			_INLINE_ POINT4D &	operator -=(const VECTOR& Vector);
			/** Operator * */
			_INLINE_ POINT4D	operator * (const float rValue);
			_INLINE_ POINT4D	operator * (const POINT4D& Point);
			/** Multiply each point coordinates by a value. Same as Mul(const float rValue). You can write: Pt *= 2.0f. */
			_INLINE_ POINT4D &	operator *=(float rValue);
			/** Multiply a point by a Scale3D. Same as Mul(const SCALE3D &Scale). */
			_INLINE_ POINT4D &	operator *=(const VECTOR &Scale);
			/** Operator / */
			_INLINE_ POINT4D	operator / (const float rValue);
			/** Operator /= */
			_INLINE_ POINT4D &	operator /= (const float rValue);

			//=============================================================================
			// SBE addons
			//=============================================================================

			/** Copy method. */
			_INLINE_ void			Copy(const POINT4D & V);
			/** SlowCopy method. (to manage unaligned datas) */
			_INLINE_ void			SlowCopy(const POINT4D & V);

			/** */
			_INLINE_ float*			ToPointer();
			/** */
			_INLINE_ const float*	ToPointer() const;
			/** */
			_INLINE_ float			operator[](const unsigned int index) const;
			/** */
			_INLINE_ float&			operator[](const unsigned int index);
			/** */
			_INLINE_ void			Zero();
			/** */
			_INLINE_ void			SlowZero();
			/** */
			_INLINE_ float			Length() const;

			//=============================================================================

			void				Read(IO_STREAM &s);
			void				Write(IO_STREAM &s) const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const POINT4D &m)
			{
				m.Write(s);
				return s;

			}
			friend IO_STREAM& operator >>(IO_STREAM &s, POINT4D &m)
			{
				m.Read(s);
				return s;
			}

			/** Copy method. */
			_INLINE_ void			Copy(const VECTOR & V);
			_INLINE_ void			SlowCopy(const VECTOR & V);

		private:
			Float32		_r[4];
		};
	}
}

#include INCL_KMATH(Math/Math)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_KMATH(Math/Geometry/More/Angle3D)
#include INCL_KCORE(Core/Assert)

#ifndef _DEBUG
#include "point4d.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES0D_POINT4D_POINT4D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
