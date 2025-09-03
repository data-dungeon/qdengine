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
//	CLASS:	INTERSECTION_2D
//	The INTERSECTION_2D class implements ...
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_INTERSECTION_INTERSECTION2D_H__
#define __KMATH_MATH_INTERSECTION_INTERSECTION2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(SEGMENT2D)
DECL_KMATH(POINT2D)
DECL_KMATH(TRIANGLE2D)
DECL_KMATH(AABR2D)

namespace QDT
{
	namespace KMATH
	{
		class	INTERSECTION2D
		{
		public:

			enum	INTERSECTION
			{
				//	No intersections cases : must be stricly negatives
				I_NO_INTERSECTION_PARRALEL_LINE			= -3,
				I_NO_INTERSECTION_COLINEAR_SEGMENT		= -2,
				I_NO_INTERSECTION_NO_COLINEAR_SEGMENT	= -1,

				//	Don't use the return value 0, don't make a boolean test !
				I_DO_NOT_USE							= 0,

				//	Intersections cases : must be strictly positives
				I_ONE_INTERSECTION,
				I_SEGMENT_INTERSECTION,
				I_LINE_INTERSECTION,
			};

			/** Various intersection between 2D shapes */
			static bool TestIntersectionSegment2DSegment2D(const SEGMENT2D & Sg1, const SEGMENT2D & Sg2);
			static bool TestIntersectionPoint2DTriangle2D(const POINT2D & Pt, const TRIANGLE2D & Triangle);
			static bool TestIntersectionPoint2DAABR2D(const POINT2D & Pt, const AABR2D & AABR2D);
			static bool TestIntersectionPoint2DAABR2D(const POINT2D & Pt, const AABR2D & AABR2D, float rEpsilon);

			static INTERSECTION FindIntersection(const POINT2D &Pt11, const POINT2D &Pt12, const POINT2D &Pt21, const POINT2D &Pt22, POINT2D &Intersection);

			//=============================================================================
			// JLT addons
			//=============================================================================
			/** Intersection between 2 segments [P0,P1] [P2,P3] */
			/** return the intersected vertex for one intersection */
			/** return true if there is an intersection false otherwise */
			static bool SegmentSegment(const POINT2D &P0, const POINT2D &P1, const POINT2D &P2, const POINT2D &P3, POINT2D &PInt, int & nNbrInt);

			/**	Intersection between segments, return exclusively an ENUM value, so nereter return false */
			/**	To avoid special case intersection, ALWAYS use switch with default QDT_FAIL to handle the result */
			/**	To ensure you handle all the specifics cases. */
			/**	Cases returned : */
			/**		I_ONE_INTERSECTION	*/
			/**		I_NO_INTERSECTION_NO_COLINEAR_SEGMENT	*/
			/**		I_NO_INTERSECTION_COLINEAR_SEGMENT	*/
			/**		I_SEGMENT_INTERSECTION	*/
			/**	The two parameters are filled depending the case returned	*/
			/**	Please care : All the parameters can be modified without correspondance with the case ! */
			static INTERSECTION	SegmentSegment(SEGMENT2D & S1, SEGMENT2D & S2, SEGMENT2D & IntSegment, POINT2D & IntPoint);

			static bool LineLine(const POINT2D &P0, const POINT2D &P1, const POINT2D &P2, const POINT2D &P3, POINT2D &P, int & nNbrInt, float T[2]);

			/** Clipping polygon along a aabb 2D */
			static bool ClipPolygon(const AABR2D &bbox, const POINT2D *InPoly, POINT2D *OutTable, int InCount, int &OutCount);

		private:

			static INTERSECTION	SegmentSegmentNoColinearity(SEGMENT2D & S1, SEGMENT2D & S2, POINT2D & IntPoint);
		};
	}
}

#endif //__KMATH_MATH_INTERSECTION_INTERSECTION2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
