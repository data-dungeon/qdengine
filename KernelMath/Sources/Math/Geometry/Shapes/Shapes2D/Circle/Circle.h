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
// CLASS: CIRCLE
//
//
//00-12-18 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_CIRCLE_CIRCLE_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_CIRCLE_CIRCLE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(VECTOR)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)	

namespace QDT
{
	namespace KMATH
	{
		class	CIRCLE
		{
		public:

			/** Constructors & destructor */
			_INLINE_					CIRCLE(bool bInit = true);
			_INLINE_					CIRCLE(const CIRCLE & Circle);
			_INLINE_					CIRCLE(const POINT2D &Center, float rRadius);

			/** Operators */
			_INLINE_ CIRCLE &			operator = (const CIRCLE & Circle);

			/** Accessors & Modifiors */
			_INLINE_ void				SetCenter(const POINT2D &Center);
			_INLINE_ void				SetRadius(float rRadius);
			_INLINE_ float				GetRadius() const;
			_INLINE_ const POINT2D &	GetCenter() const;

			/** Get a random point in the circle */
			void				GetRandomPoint(VECTOR &Point) const;

			/** Stream */
			void				Read(IO_STREAM &s);
			void				Write(IO_STREAM &s)const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const CIRCLE &a)
								{
									a.Write(s);
									return s;
								}
			friend IO_STREAM& operator >>(IO_STREAM &s, CIRCLE &a)
								{
									a.Read(s);
									return s;
								}

		private:

			POINT2D			_Center;
			float			_rRadius;	
		};
	}
}

#ifndef _DEBUG
#include "Circle.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_CIRCLE_CIRCLE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
