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
// CLASS: INTERSECTION_FRUSTRUM
//
//
//	01-06-19:	GGO - Created
//*****************************************************************************


//=============================================================================
// Name: INTERSECTION_FRUSTRUM
//	01-06-19:	GGO - Created
//=============================================================================

#ifndef __KMATH_MATH_INTERSECTION_INTERSECTIONFRUSTRUM_H__
#define __KMATH_MATH_INTERSECTION_INTERSECTIONFRUSTRUM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(FRUSTRUM)
DECL_KMATH(SPHERE)
DECL_KMATH(AABB)
DECL_KMATH(OBB)
DECL_KMATH(VECTOR)

namespace QDT
{
	namespace KMATH
	{
		class INTERSECTION_FRUSTRUM
		{
		public:
		    
			//*********************************************************
			// FRUSTRUM
			//*********************************************************

			/** Perform an intersection test between a sphere and a frustrum.
				Here the 'nSpeed' argument can be 0 or 1, with the following meanings:
				nSpeed = 0 => Slowest but safe test, this is the default.
				nSpeed = 1 => Much faster. Does not perform a complete test, so may return 'true' even if there is not intersection. But if the test returns 'false', we're sure there's no intersections. */
			static bool	TestIntersectionFrustrumSphere(const FRUSTRUM &Frustrum, const SPHERE &Sphere, int nSpeed = 0);


			/** Perform an intersection test between an AABB and a frustrum.
				Here the 'nSpeed' argument can be 0, 1 or 2, with the following meanings:
				nSpeed = 0 => Slowest but safe test, this is the default.
				nSpeed = 1 => Does not perform a complete test, so may return 'true' even if there is not intersection. But if the test returns 'false', we're sure there's no intersections.
				nSpeed = 2 => Fastest because stop even earlier in the intersection test. May be usefull if very fast processing is following. */
			static bool	TestIntersectionFrustrumAABB(const FRUSTRUM &Frustrum, const AABB &aabb, int nSpeed = 0);


			/** Perform an intersection test between an OBB and a frustrum.
				Here the 'nSpeed' argueent can be 0, 1 or 2, with the following meanings:
				nSpeed = 0 => Slowest but safe test, this is the default.
				nSpeed = 1 => Does not perform a complete test, so may return 'true' even if there is not intersection. But if the test returns 'false', we're sure there's no intersections.
				nSpeed = 2 => Fastest because stop even earlier in the intersection test. May be usefull if very fast processing is following. */
			static bool	TestIntersectionFrustrumOBB(const FRUSTRUM &Frustrum, const OBB &obb, int nSpeed = 0);

			/** Perform an intersection test between a frustrum and a point. */
			static bool	TestIntersectionFrustrumPoint(const FRUSTRUM &Frustrum, const VECTOR &Point);


		private:

			INTERSECTION_FRUSTRUM();
		~INTERSECTION_FRUSTRUM();
		};
	}
}

#endif //__KMATH_MATH_INTERSECTION_INTERSECTIONFRUSTRUM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
