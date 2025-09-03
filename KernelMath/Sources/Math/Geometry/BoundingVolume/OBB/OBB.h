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
//	CLASS:	OBB
//	The OBB class implements ...
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_BOUNDINGVOLUME_OBB_OBB_H__
#define __KMATH_MATH_BOUNDINGVOLUME_OBB_OBB_H__

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

#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingVolume)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{				
		class OBB : public BOUNDING_VOLUME
		{
		public:

			// Constructor & destructor
							OBB();
							OBB(const OBB& C);
							OBB(const int nNbrPts, VECTOR *pPoints);
			virtual		   ~OBB();

			static	void*		_pVTable;

			// Operators
			OBB&			operator = (const OBB &C);
			virtual	BOUNDING_VOLUME & operator = (const BOUNDING_VOLUME &BoundingVolume);
			
			// From BOUNDING_VOLUME
			virtual void			Display(const COLOR_FLOAT &Color) const;
			virtual const VECTOR &	GetCenter() const;
			virtual void			Clear();
			virtual void			Transform(const MATRIX& Matrix);
			
			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			virtual void		Read(IO_STREAM &s);
		#endif
		#ifndef _MASTER
			virtual void		Write(IO_STREAM &s)const;
			virtual void		WriteBinary(IO_STREAM &s);
		#endif


			// Intersections
			virtual bool		Intersect(const PLANE &Plane, bool &bSide) const;
			virtual bool		Intersect(const RAY &Ray, VECTOR & HitPoint) const;
			virtual bool		Intersect(const RAY &Ray, Float32 &rDistance) const;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron) const;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
			virtual	bool		IntersectFront(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
			virtual bool		IntersectFront(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
			virtual bool		Intersect(const FRUSTRUM &Frustrum) const;
			virtual bool		Intersect(const CONE &Cone) const;
			virtual bool		Intersect(const SPHERE &Sphere) const;
			virtual bool		Intersect(const AABB &Aabb) const;
			virtual bool		Intersect(const VECTOR &Point) const;
					bool		Intersect(const VECTOR &Point, Float32 rEpsilon) const;
			virtual bool		Intersect(const CYLINDER &Cylinder) const;
			virtual bool		Intersect(VERTICAL_RAY & VerticalRay) const;

					bool		IsSeparatingAxis(const PLANE & Plane) const;

					void		Grow(const VECTOR& P);
			
			// Accessors
			Float32				GetDistance1() const	{ return (_rDistance1); }
			Float32				GetDistance2() const	{ return (_rDistance2);	}
			Float32				GetDistance3() const	{ return (_rDistance3);	}
			const VECTOR &		GetDirection1() const	{ return (_Direction1); }
			const VECTOR &		GetDirection2() const	{ return (_Direction2); }
			const VECTOR &		GetDirection3() const	{ return (_Direction3); }
			Float32				GetVolume() const		{ return (8 * _rDistance1 * _rDistance2 * _rDistance3); }
			Float32				GetArea() const;

			void				GetRandomPoint(VECTOR &Point) const;
			void				GetPoints(VECTOR (& Points)[8]) const;
			Float32				GetSize() const;

			// Modifiors
			void				Set(const CONE & Cone);
			void				SetCenter(const VECTOR &P)		{ _Center = P; }
			void				SetDistance1(Float32 r)			{ _rDistance1 = r; _Axe1.Mul(_Direction1, _rDistance1); }
			void				SetDistance2(Float32 r)			{ _rDistance2 = r; _Axe2.Mul(_Direction2, _rDistance2); }
			void				SetDistance3(Float32 r)			{ _rDistance3 = r; _Axe3.Mul(_Direction3, _rDistance3); }
			void				SetDirection1(const VECTOR &V)	{ _Direction1 = V; _Axe1.Mul(_Direction1, _rDistance1); }
			void				SetDirection2(const VECTOR &V)	{ _Direction2 = V; _Axe2.Mul(_Direction2, _rDistance2); }
			void				SetDirection3(const VECTOR &V)	{ _Direction3 = V; _Axe3.Mul(_Direction3, _rDistance3); }

			void				Compute(int nNbrPoints, VECTOR *pPoints);
			void				Compute(Float32 rY, Float32 rX1, Float32 rZ1, Float32 rX2, Float32 rZ2, Float32 rX3, Float32 rZ3, Float32 rX4, Float32 rZ4);

			void				SetFromAabb(const AABB & Aabb);
			void				Extend(Float32 r);
			void				Extend(Float32	rx, Float32	ry, Float32	rz);

			// For shadow maps
			virtual Float32		GetOptimalFocalAngle(const VECTOR & Pos, Float32 rAngle, Float32 & rNear, Float32 & rIntersectNear, const VECTOR & FrontDir) const;

			float				GetRadius() const {return MATH::Sqrt(_rDistance1 * _rDistance1 + _rDistance2 * _rDistance2 + _rDistance3 * _rDistance3);}


		private:

			void				FindDirections(int nNbrPoints, VECTOR *pPoints);

			int		_nPadding[2];

			VECTOR _Center;

			VECTOR _Direction1;
			VECTOR _Direction2;
			VECTOR _Direction3;

			VECTOR _Axe1;
			VECTOR _Axe2;
			VECTOR _Axe3;

			Float32 _rDistance1;
			Float32 _rDistance2;
			Float32 _rDistance3;

			int		_nPadding2;
		};
	}
}

#endif //__KMATH_MATH_BOUNDINGVOLUME_OBB_OBB_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
