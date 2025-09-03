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
// CLASS: LINE
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES1D_LINE_LINE_H__
#define __KMATH_MATH_SHAPES_SHAPES1D_LINE_LINE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(RAY)
DECL_KMATH(SEGMENT)
DECL_KMATH(MATRIX)

#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	LINE
		{
		public:
			_INLINE_ explicit		LINE();

			_INLINE_				LINE(const LINE &Line);
			_INLINE_				LINE(const VECTOR &LinePt, const VECTOR &LineDir);
			_INLINE_ explicit		LINE(const SEGMENT &Segment);
			_INLINE_ explicit		LINE(const RAY &Ray);

			_INLINE_ void			Set(const LINE &Line);
			_INLINE_ void			Set(const VECTOR &LinePt, const VECTOR &LineDir);
			_INLINE_ void			SetFromSegment(const SEGMENT &Segment);
			_INLINE_ void			SetFromRay(const RAY &Ray);

			_INLINE_ void			Transform(const LINE &Line, const MATRIX &Matrix);
			_INLINE_ void			Transform(const MATRIX &Matrix);
			_INLINE_ void			GetPoint(VECTOR &p)const;
			_INLINE_ void			GetDirection(VECTOR &p)const;
			_INLINE_ const VECTOR&  GetDirection() const;
			_INLINE_ const VECTOR&	GetPoint() const;


			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s) const;
			friend IO_STREAM& operator <<(IO_STREAM &s, const LINE &a)
			{
				a.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, LINE &a)
			{
				a.Read(s);
				return s;
			}

		private:

			VECTOR			_Point;
			VECTOR			_Direction;
		};
	}
}

#ifndef _DEBUG
#include "line.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES1D_LINE_LINE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
