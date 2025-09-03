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
// CLASS: TRIANGLE
//
//
//00-11-10 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: TRIANGLE
//00-11-10 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_TRIANGLE_TRIANGLE_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_TRIANGLE_TRIANGLE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(MATRIX)
DECL_KMATH(COLOR_FLOAT)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	TRIANGLE
		{
		public:
			_INLINE_  TRIANGLE() ; 
			_INLINE_  TRIANGLE(const TRIANGLE& Triangle) ;
			_INLINE_  TRIANGLE(const VECTOR& Point0, const VECTOR& Point1, const VECTOR& Point2) ;
			_INLINE_ ~TRIANGLE() ;

			_INLINE_  TRIANGLE& operator = (const TRIANGLE &Triangle) ;

					void	Transform(const MATRIX &Matrix) ;
			
					void	GetRandomPoint(VECTOR &Point) const ;

			// Accessors - Modifiers
			_INLINE_  const VECTOR&		GetPoint0() const ;
			_INLINE_  const VECTOR&		GetPoint1() const ;
			_INLINE_  const VECTOR&		GetPoint2() const ;
			_INLINE_  void				SetPoint0(const VECTOR& Point) ;
			_INLINE_  void				SetPoint1(const VECTOR& Point) ;
			_INLINE_  void				SetPoint2(const VECTOR& Point) ;
			_INLINE_  void				SetPoints(const VECTOR& Point0, const VECTOR& Point1, const VECTOR& Point2) ;

			/** Return the area of a triangle. */
			_INLINE_ float				GetArea();

			/** Display triangle */
			void						Display(const COLOR_FLOAT & Color) const;

			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s) const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const TRIANGLE &m)
			{
				m.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, TRIANGLE &m)
			{
				m.Read(s);
				return s;
			}

		private:
			VECTOR			_Point0 ;
			VECTOR			_Point1 ;
			VECTOR			_Point2 ;
		};
	}
}

#ifndef _DEBUG
#include	"Triangle.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_TRIANGLE_TRIANGLE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
