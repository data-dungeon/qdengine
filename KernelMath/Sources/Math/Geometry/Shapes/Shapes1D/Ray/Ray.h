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
// CLASS: RAY
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES1D_RAY_RAY_H__
#define __KMATH_MATH_SHAPES_SHAPES1D_RAY_RAY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(SEGMENT)
DECL_KMATH(MATRIX)

#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	RAY
		{
		public:

			// Constructors & destructor
			_INLINE_ explicit		RAY() ;
			_INLINE_				RAY(const RAY& Ray) ;
			_INLINE_				RAY(const VECTOR & Origin, const VECTOR & Direction) ;
			_INLINE_			   ~RAY() ;

			/** Operator = */
			_INLINE_ const RAY&		operator = (const RAY& Ray) ;

			/** Transform with a matrix */
			void			Transform(const MATRIX &Matrix);

			/** Get origin */
			_INLINE_ const VECTOR& GetOrigin() const;

			/** Set origin */
			_INLINE_ void			SetOrigin(const VECTOR & Origin) ;

			/** Get Direction (normalized) */
			_INLINE_ const VECTOR&	GetDirection() const;

			/** Set normalized direction */
			_INLINE_ void			SetDirection(const VECTOR & Direction) ;

			/** Read a ray from a stream */
			void					Read(IO_STREAM &s);

			/** Write a ray in a stream */
			void					Write(IO_STREAM &s) const;

			friend IO_STREAM& operator << (IO_STREAM & s, const RAY & m)
			{
				m.Write(s);
				return s;
			}

			friend IO_STREAM& operator >>(IO_STREAM & s, RAY & m)
			{
				m.Read(s);
				return s;
			}

		private:

			VECTOR		_Origin;
			VECTOR		_Direction;	
		};
	}
}

#ifndef _DEBUG
#include "ray.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES1D_RAY_RAY_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
