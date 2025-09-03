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
// CLASS: INTERSECTION
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: INTERSECTION
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_INTERSECTION_INTERSECTION_H__
#define __KMATH_MATH_INTERSECTION_INTERSECTION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KMATH(RAY)
DECL_KMATH(SEGMENT)
DECL_KMATH(LINE)
DECL_KMATH(TRIANGLE)
DECL_KMATH(SPHERE)
DECL_KMATH(PLANE)
DECL_KMATH(AABB)
DECL_KMATH(OBB)
DECL_KMATH(HEXAHEDRON)
DECL_KMATH(FRUSTRUM)
DECL_KMATH(CYLINDER)

namespace QDT
{
	namespace KMATH
	{
		enum REFERENCE_PLANE
		{
			PLANE_UNKNOWN,
			PLANE_XY,
			PLANE_XZ,
			PLANE_YZ,
		};

		class INTERSECTION
		{
		public:

			/** POINT */
			_INLINE_ static bool	TestIntersectionPointPoint		(const VECTOR &Point1, const VECTOR &Point2);	
			_INLINE_ static bool	TestIntersectionPointSphere		(const VECTOR &Point,  const SPHERE &Sphere);	
					static bool	TestIntersectionPointTriangle	(const VECTOR &Point, const TRIANGLE &Triangle);

			/** RAY */
					static bool	FindIntersectionRayTriangle		(const RAY &Ray, const TRIANGLE &Triangle, Float32 &rDistance) ;
			_INLINE_ static bool	FindIntersectionRayTriangle		(const RAY &Ray, const TRIANGLE &Triangle, VECTOR	&HitPoint) ;

					static bool	FindIntersectionRayTriangleWithCulling(const RAY &Ray, const TRIANGLE &Triangle, Float32 &rDistance) ;
			_INLINE_ static bool	FindIntersectionRayTriangleWithCulling(const RAY &Ray, const TRIANGLE &Triangle, VECTOR &HitPoint) ;

			//*********************************************************
			// SEGMENT
			//*********************************************************

					static bool		TestIntersectionSegmentTriangle		(const SEGMENT &Segment, const TRIANGLE &Triangle);
					static bool		TestIntersectionSegmentAABB			(const SEGMENT &Segment, const AABB &Aabb);

			//*********************************************************
			// LINE
			//*********************************************************

			/** Test intersection between a line & a sphere */
			_INLINE_ static bool	TestIntersectionLineSphere		(const LINE &Line, const SPHERE &Sphere);
			/** Find intersection between a line & a sphere */
					static bool	FindIntersectionLineSphere		(const LINE &Line, const SPHERE &Sphere, int &nNbrHitPoints, VECTOR &HitPoint1, VECTOR &HitPoint2);


			//*********************************************************
			// AABB
			//*********************************************************

					static bool	TestIntersectionAABB_AABB		(const AABB & aabb1, const AABB & aabb2);
					static bool	TestIntersectionAABB_AABB		(const AABB & aabb1, const AABB & aabb2, Float32 rEpsilon);	
					static bool	TestIntersectionTriangleAABB	(const TRIANGLE & Triangle, const AABB & bbox);
					static bool	TestIntersectionPointAABB		(const VECTOR & Point, const AABB & Bbox);
					static bool	TestIntersectionPointAABB		(const VECTOR & Point, const AABB & Bbox, Float32 rEpsilon);
					static bool	TestIntersectionSphereAABB		(const SPHERE & Sphere, const AABB & Aabb);


			//*********************************************************
			// OBB
			//*********************************************************

			/** Safe but slow test of intersection between two OBB. */
					static bool		TestIntersectionOBB_OBB			(const OBB & obb1, const OBB & obb2);

			/** A much faster method that may return true even if the two OBB don't really intersect
				(but in this case, they are quite close anyway). */
					static bool	FastTestIntersectionOBB_OBB		(const OBB & obb1, const OBB & obb2);

			/** Test if a point is inside an OBB. */
					static bool	TestIntersectionPointOBB		(const OBB & obb, const VECTOR & Point);


			//*********************************************************
			// SPHERE
			//*********************************************************

			_INLINE_ static bool	TestIntersectSphereSphere		(const SPHERE &Sphere1, const SPHERE & Sphere2);

			// Anciennes methodes (qu'il faudra à terme éliminer ou remplacer) because pas sures du tout
			_INLINE_ static bool	IntersectPointWithLine(const VECTOR &Point, const LINE &Line);
			_INLINE_ static bool	IntersectPointWithPlane(const VECTOR &Pt, const PLANE &Plane);
			_INLINE_ static bool	IntersectPointWithPlane(const VECTOR &Pt, const PLANE &Plane, bool &bSide);

					static bool	IntersectLineWithLine(const LINE &Line1, const LINE &Line2, VECTOR	&HitPoint, bool &bMerged);

					static bool	IntersectPlaneWithPlane(const PLANE &Plane1, const PLANE &Plane2, bool &bMerged);
					static bool	IntersectPlaneWithPlane(const PLANE &Plane1, const PLANE &Plane2, LINE &Line, bool &bMerged);
			_INLINE_ static bool	IntersectPlaneWithSphere(const PLANE &Plane, const SPHERE &Sphere);
			_INLINE_ static bool	IntersectPlaneWithSphere(const PLANE &Plane, const SPHERE &Sphere, bool &bSide);
					static bool	IntersectPlaneWithRay(PLANE &Plane, const RAY &Ray, VECTOR &HitPoint);
					static bool	IntersectPlaneWithRay(PLANE &Plane, const RAY &Ray, Float32 &rDistance);


					static bool	IntersectPointWithPlanesTab(VECTOR & Point, int nNbrSrcPlanes, const PLANE *pSrcPlanesTab);
					static bool	IntersectSphereWithPlanesTab(SPHERE &Sphere, int nNbrSrcPlanes, const PLANE *pSrcPlanesTab, int &nNbrResPlanes, int *pnResPlanesIndexes);
					static bool	IntersectSphereWithPlanesTab(SPHERE &Sphere, int nNbrSrcPlanes, const PLANE *pSrcPlanesTab);
					static bool	IntersectPointsTabWithPlane(int nNbrPoints, const VECTOR *PointsTab, const PLANE &Plane, bool &bSide);
					static bool	IntersectPointsTabWithPlanesTab(int	nNbrPoints, VECTOR *PointsTab, int	nNbrPlanes, const PLANE *PlanesTab);
					static bool	IntersectPointWithPlanes(VECTOR & POut, const PLANE & P1,  const PLANE & P2, const PLANE & P3);

			static bool	TestIntersectionHexahedronPoint( const HEXAHEDRON &Hexahedron, const VECTOR& Point);
			static bool	TestIntersectionFrustrumPoint( const FRUSTRUM &Frustrum, const VECTOR& Point);

			static SEGMENT*	BuildPlanesTabSegments(int nNbrPlanes, PLANE *, int &nNbrSegments);

			/** */
			static bool PlaneCylinder(const PLANE &P, const CYLINDER &C);
			/** */
			static bool SphereCylinder(const SPHERE &S, const CYLINDER &C);

		private :

			static bool AxisTest_X(Float32 rA, Float32 rB, Float32 rFA, Float32 rFB, const VECTOR &	v0, const VECTOR & v1, const VECTOR & BoxHalfSize);
			static bool AxisTest_Y(Float32 rA, Float32 rB, Float32 rFA, Float32 rFB, const VECTOR &	v0, const VECTOR & v1, const VECTOR & BoxHalfSize);
			static bool AxisTest_Z(Float32 rA, Float32 rB, Float32 rFA, Float32 rFB, const VECTOR &	v0, const VECTOR & v1, const VECTOR & BoxHalfSize);

			// We can't build an objet INTERSECTION, it's used just for static methods
			INTERSECTION();
			~INTERSECTION();
		};
	}
}

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)	
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Line/Line)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle/Triangle)	
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/Geometry/Distance/Distance)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)		
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)

#ifndef _DEBUG
#include	"intersection.inl"
#endif

#endif //__KMATH_MATH_INTERSECTION_INTERSECTION_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
