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
//	CLASS:	SPHERE
//	The SPHERE class implements ...
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_BOUNDINGVOLUME_SPHERE_SPHERE_H__
#define __KMATH_MATH_BOUNDINGVOLUME_SPHERE_SPHERE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(PLANE)
DECL_KMATH(RAY)
DECL_KMATH(HEXAHEDRON)
DECL_KMATH(FRUSTRUM)
DECL_KMATH(CONE)
DECL_KMATH(SPHERE)
DECL_KMATH(AABB)
DECL_KMATH(VERTICAL_RAY)
DECL_KMATH(CYLINDER)

#include 	INCL_KMATH(Math/Math)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingVolume)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include 	INCL_KCORE(Template/QDT_Vector)
namespace QDT
{
	namespace KMATH
	{				
		class SPHERE : public BOUNDING_VOLUME
		{
		public:

			// Constructors & destructor
							SPHERE();
							SPHERE(const SPHERE &C);
							SPHERE(const VECTOR& Centre, Float32 rRadius);
							SPHERE(int nNbrPoints, VECTOR *pPoints);
			virtual        ~SPHERE();

			static	void*		_pVTable;

			// Operators
			SPHERE&				operator=(const SPHERE &C);
			BOUNDING_VOLUME &	operator = (const BOUNDING_VOLUME &BoundingVolume);

			// From BOUNDING_VOLUME
			void				Display(const COLOR_FLOAT &Color) const;
			bool				Intersect(const VECTOR & PIn, VECTOR & POut, const VECTOR & vMotion, const Float32 rInertia = 1.0f, const Float32 rE = 0.0f) const;

			void				Clear();
			void				Transform(const MATRIX &Matrix);
			const VECTOR &		GetCenter() const;
			Float32				GetSize() const;
			void				Extend(Float32 r);
			
			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			virtual void		Read(IO_STREAM &s);
		#endif
		#ifndef _MASTER
			virtual void		Write(IO_STREAM &s)const;
			virtual void		WriteBinary(IO_STREAM &	s);
		#endif

			// Intersections
			bool				Intersect(const PLANE &Plane, bool &bSide) const;
			bool				Intersect(const RAY &Ray, VECTOR & HitPoint) const;
			bool				Intersect(const RAY &Ray, Float32 &rDistance) const;
			bool				Intersect(const HEXAHEDRON &Hexahedron) const;
			bool				Intersect(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
			bool				Intersect(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
			bool				IntersectFront(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
			bool				IntersectFront(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
			bool				Intersect(const FRUSTRUM &Frustrum) const;
			bool				Intersect(const CONE &Cone) const;
			bool				Intersect(const SPHERE &Sphere) const;
			bool				Intersect(const AABB &Aabb) const;
			bool				Intersect(const VECTOR &Point) const;
			bool				Intersect(VERTICAL_RAY & VerticalRay) const;

			bool			IsOutsideOf(const PLANE &Plane) const;

			// Accessors
			Float32			GetRadius() const	{ return (_Center.GetPad()); }
			Float32			GetVolume() const	{ return (4 / 3 * MATH::PI() * GetRadius() * GetRadius() * GetRadius()); }

			// Modifiors
			void			Set(const CONE & Cone);
			void			Set(const AABB & BoundingAABB);
			void			SetCenter(const VECTOR & V)			{ _Center = V; }
			void			SetRadius(Float32 rRadius)			{ _Center.SetPad(rRadius); }

			void			Compute(int nNbrPoints, VECTOR *pPoints);
			void			Compute(const QDT_VECTOR< VECTOR > &Points);
			void			Compute(UInt32 nNbrPoints, const void * pPoints, UInt32 nStride);
			void			GetRandomPoint(VECTOR &Point) const;
			
			void			Display(const COLOR_FLOAT &Color, int nStep) const;
			void			DisplayFilled(const COLOR_FLOAT &Color, const int nStep) const;

			/** */
			bool Grow(const SPHERE &S);
			/** */
			bool Grow(const VECTOR &P);

			// For shadow maps
			virtual Float32		GetOptimalFocalAngle(const VECTOR & Pos, Float32 rAngle, Float32 & rNear, Float32 & rIntersectNear, const VECTOR & FrontDir) const;

		private:

			
			
			VECTOR			_Center;
			// Float32		_rRadius; -> Packed in _Center.GetPad()
		};
	}
}

#endif //

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
