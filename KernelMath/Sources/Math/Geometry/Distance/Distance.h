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
// CLASS: DISTANCE
//
//
//00-12-20 RMA Created: 
//*****************************************************************************


//=============================================================================
// Name: DISTANCE
//00-12-20 RMA Created: 
//=============================================================================
#ifndef __KMATH_MATH_DISTANCE_DISTANCE_H__
#define __KMATH_MATH_DISTANCE_DISTANCE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KMATH(VECTOR_2)
DECL_KMATH(LINE)
DECL_KMATH(PLANE)
DECL_KMATH(RAY)

namespace QDT
{
	namespace KMATH
	{
		class DISTANCE
		{
		public:
			//------- 2D --------
			_INLINE_ static float DistancePointPoint(const VECTOR_2& Point1, const VECTOR_2& Point2);
			_INLINE_ static float DistanceSquarePointPoint(const VECTOR_2& Point1, const VECTOR_2& Point2);

			//------- 3D --------

			//Point - Line
					static Float32 DistanceSquarePointLine(const VECTOR & Point, const LINE &Line );
			_INLINE_ static Float32 DistancePointLine(const VECTOR & Point, const LINE &	Line);


			//Ray - Point
			_INLINE_ static Float32 DistanceRayPoint(const RAY& Ray, const VECTOR& Point) ;
					static Float32 DistanceSquareRayPoint(const RAY& Ray, const VECTOR& Point) ;


			//Point - Point
			_INLINE_ static Float32 DistancePointPoint(const VECTOR& Point1, const VECTOR& Point2) ;
			_INLINE_ static Float32 DistanceSquarePointPoint(const VECTOR& Point1, const VECTOR& Point2) ;


			//Point - Plane
					static Float32 DistancePointPlane( const PLANE& Plane, const VECTOR& Point) ;
					static Float32 DistancePointNormalizedPlane(const PLANE & NormalizedPlane, const VECTOR & Point) ;

		private:

			// We can't build an objet DISTANCE, it's used just for static methods
			explicit  DISTANCE() ;
					~DISTANCE() ;
		};
	}
}

#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Line/Line)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)

#ifndef _DEBUG
#include "distance.inl"
#endif

#endif //__KMATH_MATH_DISTANCE_DISTANCE_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
