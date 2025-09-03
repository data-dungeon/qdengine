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
//	CLASS:	TRIANGLE_2D
//	The TRIANGLE_2D class implements ...
//
//	01-07-24:	RMA - Created
//	01-08-28:	JLT - Modified
//
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_TRIANGLE2D_TRIANGLE2D_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_TRIANGLE2D_TRIANGLE2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

namespace QDT
{
	namespace KMATH
	{
		class	TRIANGLE2D
		{
		private:
			POINT2D	_Point1;
			POINT2D	_Point2;
			POINT2D	_Point3;
			
		public:

			/** Constructor & destructor */
			_INLINE_ TRIANGLE2D(bool bFlagInit = true);
			_INLINE_ TRIANGLE2D(const POINT2D &Point1, const POINT2D &Point2, const POINT2D &Point3);
			_INLINE_ virtual ~TRIANGLE2D();

			/** Accessors & modifiors */
			_INLINE_ void				SetPoint1(const POINT2D & Pt);
			_INLINE_ void				SetPoint2(const POINT2D & Pt);
			_INLINE_ void				SetPoint3(const POINT2D & Pt);
			_INLINE_ const POINT2D &	GetPoint1() const;
			_INLINE_ const POINT2D &	GetPoint2() const;
			_INLINE_ const POINT2D &	GetPoint3() const;
			_INLINE_ bool				IsDegenerated() const;
			
			//=============================================================================
			// JLT addons
			//=============================================================================
			/** */
			_INLINE_ TRIANGLE2D(const TRIANGLE2D &T);
			/** */
			_INLINE_ const TRIANGLE2D& operator=(const TRIANGLE2D &T);
			/** */
			_INLINE_ const POINT2D& operator[] (unsigned int index) const;
			/** */
			_INLINE_ POINT2D& operator[] (unsigned int index);
			/** */
			_INLINE_ float GetArea() const;
			/** */
			_INLINE_ float GetXMin() const;
			/** */
			_INLINE_ float GetXMax() const;
			/** */
			_INLINE_ float GetYMin() const;
			/** */
			_INLINE_ float GetYMax() const;
			/** */
			bool Contain(const POINT2D &P) const;
			/** */
			_INLINE_ const POINT2D* GetBuffer() const;
			
		protected :
			_INLINE_ void Copy(const TRIANGLE2D &T);
			
		};
	}
}

#ifndef _DEBUG
#include	"Triangle2D.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_TRIANGLE2D_TRIANGLE2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
