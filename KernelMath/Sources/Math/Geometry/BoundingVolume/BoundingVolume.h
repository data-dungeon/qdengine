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
//	CLASS:	BOUNDING_VOLUME
//	The BOUNDING_VOLUME class implements ...
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_BOUNDINGVOLUME_BOUNDINGVOLUME_H__
#define __KMATH_MATH_BOUNDINGVOLUME_BOUNDINGVOLUME_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(COLOR_FLOAT)
DECL_KMATH(VECTOR)
DECL_KMATH(MATRIX)
DECL_KMATH(RAY)
DECL_KMATH(PLANE)
DECL_KMATH(SPHERE)
DECL_KMATH(HEXAHEDRON)
DECL_KMATH(FRUSTRUM)
DECL_KMATH(CONE)
DECL_KMATH(VERTICAL_RAY)
DECL_KMATH(AABB)
DECL_KCORE(IO_STREAM)

namespace QDT
{
	namespace KMATH
	{				
		class BOUNDING_VOLUME
		{
		public :

			enum TYPE
			{
				TYPE_NONE = 0,
				TYPE_UNUSED,
				TYPE_AABB,
				TYPE_OBB,
				TYPE_CYLINDER,
				TYPE_SPHERE,

				TYPE_MAX = 0xffffffff
			};

			// Constructor & destructor
			_EXPLICIT_			BOUNDING_VOLUME(TYPE eType);
			virtual			   ~BOUNDING_VOLUME();

			virtual	BOUNDING_VOLUME &	operator = (const BOUNDING_VOLUME &BoundingVolume) = 0;

			// Type
			_INLINE_ TYPE		GetType() const	{ return (_eType); }

			virtual void		Display(const COLOR_FLOAT &Color) const = 0;

			virtual	void		ExtractPlanes() const;
			virtual bool		Intersect(const VECTOR  &PIn,
										VECTOR		&POut,
										const VECTOR  &vMotion,
										const float	rInertia = 1.0f,
										const float	rE = 0.0f) const;

			virtual void			Clear() = 0;
			virtual const VECTOR &	GetCenter() const = 0;
			virtual void			Transform(const MATRIX &Transform) = 0;
			virtual float			GetRadius() const;
			virtual float			GetSize() const = 0;

			// Intersection
			virtual bool		Intersect(const PLANE &Plane, bool &bSide) const = 0;
			virtual bool		Intersect(const RAY &Ray, VECTOR & HitPoint) const = 0;
			virtual bool		Intersect(const RAY &Ray, float &rDistance) const = 0;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron) const = 0;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const = 0;
			virtual bool		Intersect(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const = 0;
			virtual bool		IntersectFront(const HEXAHEDRON &Hexahedron, bool & bIntersectFront) const = 0;
			virtual bool		IntersectFront(const HEXAHEDRON &Hexahedron, bool & bInside, bool & bIntersectFront) const = 0;
			virtual bool		Intersect(const FRUSTRUM &Frustrum) const = 0;
			virtual bool		Intersect(const CONE &Cone) const = 0;
			virtual bool		Intersect(const SPHERE &Sphere) const = 0;
			virtual bool		Intersect(const AABB &Aabb) const = 0;
			virtual bool		Intersect(const VECTOR &Point) const = 0;
			virtual bool		Intersect(VERTICAL_RAY & VerticalRay) const = 0;

			virtual bool		IsInside(const FRUSTRUM &Frustrum) const;
			virtual bool		IsInside(const HEXAHEDRON &Hexahedron) const;

			// Stream
		#ifndef _MASTER_ONLY_DATABANK
			virtual void		Read(IO_STREAM &s)	= 0;
		#endif
		#ifndef _MASTER
			virtual void		Write(IO_STREAM &s) const = 0;
			virtual void		WriteBinary(IO_STREAM &s) = 0;
		#endif

			// For shadow maps
			virtual float		GetOptimalFocalAngle(const VECTOR & Pos, float rAngle, float & rNear, float & rIntersectNear, const VECTOR & FrontDir) const = 0;

		#ifndef _MASTER
			friend IO_STREAM& operator <<(IO_STREAM &Stream, const BOUNDING_VOLUME &BV)
			{
				BV.Write(Stream);
				return (Stream);
			}
		#endif
		#ifndef _MASTER_ONLY_DATABANK
			friend IO_STREAM& operator >>(IO_STREAM &Stream, BOUNDING_VOLUME &BV)
			{
				BV.Read(Stream);
				return (Stream);
			}
		#endif

		protected:

			TYPE	_eType;
		};
	}
}

#endif //__KMATH_MATH_BOUNDINGVOLUME_BOUNDINGVOLUME_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
