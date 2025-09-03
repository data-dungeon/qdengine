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
//	CLASS:	AABB
//	The AABB class implements ...
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_BOUNDINGVOLUME_AABB_AABB_H__
#define __KMATH_MATH_BOUNDINGVOLUME_AABB_AABB_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(OBB)
DECL_KMATH(VECTOR)

#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingVolume)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{				
		class AABB : public BOUNDING_VOLUME
		{
		public:

			// Constructor & Destructor
								AABB();
								AABB(const int nNbrPts, const VECTOR *pPoints);
								AABB(float rMinX, float rMinY, float rMinZ, float rMaxX, float rMaxY, float rMaxZ);
								AABB(const AABB & C);
			virtual			   ~AABB();

			static	void*		_pVTable;

			// Operators
					AABB&		operator=(const AABB &C);
			virtual	BOUNDING_VOLUME & operator = (const BOUNDING_VOLUME &BoundingVolume);

			// From BOUNDING_VOLUME
			virtual void			Display(const COLOR_FLOAT &Color) const;
			virtual void			Clear();
			virtual const VECTOR &	GetCenter() const;
			virtual void			Transform(const MATRIX &Transform);
			virtual	float			GetSize() const;

			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			virtual void		Read(IO_STREAM &s);
		#endif
		#ifndef _MASTER
			virtual void		Write(IO_STREAM &s)const;
			virtual void		WriteBinary(IO_STREAM &s);
		#endif

			// Intersections
			virtual	void		ExtractPlanes() const;
			virtual bool		Intersect(const VECTOR &PIn,
										VECTOR		&POut,
										const VECTOR  &vMotion,
										const float	rInertia = 1.0f,
										const float	rE = 0.0f) const;

					bool		Intersect(const PLANE & Plane) const;
			virtual bool		Intersect(const PLANE &Plane, bool &bSide) const;
			virtual bool		Intersect(const RAY &Ray, VECTOR & HitPoint) const;
			virtual bool		Intersect(const RAY &Ray, float &rSquareDistance) const;
			virtual bool		Intersect(const HEXAHEDRON & Hexahedron) const;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
			virtual	bool		IntersectFront(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const;
			virtual bool		IntersectFront(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const;
			virtual bool		Intersect(const FRUSTRUM &Frustrum) const;
			virtual bool		Intersect(const CONE &Cone) const;
			virtual bool		Intersect(const SPHERE &Sphere) const;
			virtual bool		Intersect(const AABB &Aabb) const;
			virtual bool		Intersect(const VECTOR &Point) const;
			virtual bool		Intersect(const VECTOR &Point, float rEpsilon) const;
			virtual bool		Intersect(VERTICAL_RAY & VerticalRay) const;

			_INLINE_ bool		IsOutsideOf(const PLANE &Plane) const;

			void				SetXYZ(const float rXMin, const float rXMax, const float rYMin, const float rYMax, const float rZMin, const float rZMax);
				
			// Accessors
			float				GetXMin() const			{ return (_Min.GetX()); }
			float				GetYMin() const			{ return (_Min.GetY()); }
			float				GetZMin() const			{ return (_Min.GetZ()); }
			float				GetXMax() const			{ return (_Max.GetX()); }
			float				GetYMax() const			{ return (_Max.GetY()); }
			float				GetZMax() const			{ return (_Max.GetZ()); }
			float				GetSizeX() const		{ return (_Size.GetX()); }
			float				GetSizeY() const		{ return (_Size.GetY()); }
			float				GetSizeZ() const		{ return (_Size.GetZ()); }

			float				GetVolume() const		{ return (GetSizeX() * GetSizeY() * GetSizeZ()); }
			const VECTOR &		GetMin() const			{ return (_Min); }
			const VECTOR &		GetMax() const			{ return (_Max); }
			const VECTOR &		GetSizeVector() const	{ return (_Size); }
			void				GetPoints(VECTOR (& Points)[8]) const;
			float				GetArea() const;
			
			void				Extend(float rValue);
			void				Extend(const VECTOR & v);
			void				Extend(const AABB & Aabb);
			void				Translate(const VECTOR &vTrans);
			void				Translate(float rdx, float rdy, float rdz);	
			void				Compute(int nNbrPoints, const VECTOR *pPoints);
			void				Compute(UInt32 nNbrPoints, const void * pPoints, UInt32 nStride);
			bool				IsInside(const AABB & Aabb) const;
			bool				IsInside(const AABB & Aabb, float rEpsilon) const;
			bool				IsInside(const OBB & Obb, float rEpsilon) const;
			bool				IsInfinite() const;
			 
			void				GrowX(float X);
			void				GrowY(float Y);
			void				GrowZ(float Z);
			void				Grow(const AABB &bbox);
			void				Grow(const OBB &bbox);
			void				Grow(const VECTOR& P);

			float				GetRadius() const;

			// For shadow maps
			virtual float		GetOptimalFocalAngle(const VECTOR & Pos, float rAngle, float & rNear, float & rIntersectNear, const VECTOR & FrontDir) const;
			
			void				SetFromSphere(const SPHERE & Sphere);

		private:

			
			_INLINE_ float		GetEffectiveRadius(const PLANE &Plane) const;

			VECTOR				_Min;
			VECTOR				_Max;
			VECTOR				_Center;
			VECTOR				_Size;
		};

	}
}

#ifndef _DEBUG
#include	"AABB.inl"
#endif

#endif //__KMATH_MATH_BOUNDINGVOLUME_AABB_AABB_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

