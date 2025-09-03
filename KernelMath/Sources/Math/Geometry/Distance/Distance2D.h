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
//	CLASS:	DISTANCE2D
//	The DISTANCE2D class implements ...
//
//	01-09-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_DISTANCE_DISTANCE2D_H__
#define __KMATH_MATH_DISTANCE_DISTANCE2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(POINT2D)

namespace QDT
{
	namespace KMATH
	{
		class	DISTANCE2D
		{
		public:
			
			_INLINE_ static float DistancePointPoint(const POINT2D & Pt1, const POINT2D & Pt2);
			_INLINE_ static float DistanceSquarePointPoint(const POINT2D & Pt1, const POINT2D & Pt2);

		private:

			DISTANCE2D();
			~DISTANCE2D();
		};
	}
}

#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

#ifndef _DEBUG
#include	"distance2d.inl"
#endif

#endif //__KMATH_MATH_DISTANCE_DISTANCE2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
