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
// CLASS: DISC
//
//
//00-12-18 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_DISC_DISC_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_DISC_DISC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(COLOR_FLOAT)
DECL_KMATH(MATRIX)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	DISC
		{
		public:

			_INLINE_ explicit	DISC();

			_INLINE_ DISC(const DISC &);
			_INLINE_			DISC(VECTOR &, float rDiscRadius, VECTOR &);

			_INLINE_ void			Build(const DISC &);
			_INLINE_ void			Build(VECTOR &, float rDiscRadius, VECTOR &);

					void			Transform(const DISC &, const MATRIX &);
					void			Transform(const MATRIX &);
			_INLINE_ void			GetRadius(float &)const;
			_INLINE_ void			SetRadius(float r);
			_INLINE_ void			GetCenter(VECTOR &)const;
			_INLINE_ void			SetCenter(VECTOR &c);
			_INLINE_ void			GetAxis(VECTOR &)const;
			_INLINE_ void			SetAxis(VECTOR &v);

			_INLINE_ void			GetRandomPoint(VECTOR &Point) const;

					void			Display(const COLOR_FLOAT &Color, int nStep = 16) const;

			void	Read(IO_STREAM &s);
			void	Write(IO_STREAM &s)const;

			friend IO_STREAM& operator <<(IO_STREAM &s, const DISC &a)
			{
				a.Write(s);
				return s;
			}
			friend IO_STREAM& operator >>(IO_STREAM &s, DISC &a)
			{
				a.Read(s);
				return s;
			}

		private:

			VECTOR			_Center;
			float			_rRadius;	
			VECTOR			_Axis;
		};
	}
}

#ifndef _DEBUG
#include "disc.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_DISC_DISC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
