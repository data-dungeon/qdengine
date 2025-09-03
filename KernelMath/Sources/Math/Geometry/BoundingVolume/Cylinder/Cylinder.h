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
//	CLASS:	CYLINDER
//	The CYLINDER class implements ...
//
//	02-04-02:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_BOUNDINGVOLUME_CYLINDER_CYLINDER_H__
#define __KMATH_MATH_BOUNDINGVOLUME_CYLINDER_CYLINDER_H__

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

#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingVolume)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{				
		class CYLINDER : public BOUNDING_VOLUME
		{
		private :
			
			VECTOR	_Center;
			VECTOR  _XDir;	
			VECTOR  _YDir;
			VECTOR  _ZDir;

			//float	_rRadius; -> Packed in _Center.GetPad() => GetRadius()
			//float	_rHeight; -> Packed in _XDir.GetPad() => GetHeight()
			
		public:


			CYLINDER();
			CYLINDER(const CYLINDER &C);
			CYLINDER(const VECTOR &Center, const VECTOR &CylDir, const float rCylRadius, const float rCylHeight);
			virtual ~CYLINDER() {}

			
			static	void*		_pVTable;
			/** */
			CYLINDER& operator=(const CYLINDER &C);
			virtual	BOUNDING_VOLUME & operator = (const BOUNDING_VOLUME &BoundingVolume);
			void Copy(const CYLINDER &C);


			/** */
			virtual void		Display(const COLOR_FLOAT &Color) const; 
			virtual	void		DisplayFilled(const COLOR_FLOAT &Color, const int nStep = 24.0f) const;
			/** */
			virtual void			Clear();
			virtual float			GetSize() const;
			virtual const VECTOR &	GetCenter() const;
			void					Transform(const MATRIX &Matrix);
			void					Transform(const CYLINDER &Cylinder, const MATRIX &Matrix);	


			virtual bool		Intersect(const PLANE &Plane, bool &bSide) const;
			virtual bool		Intersect(const RAY &Ray, VECTOR & HitPoint) const;
			virtual bool		Intersect(const RAY &Ray, float &rDistance) const;
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
			virtual bool		Intersect(VERTICAL_RAY & VerticalRay) const;

			virtual bool	Intersect(const VECTOR &PIn,
									VECTOR		&POut,
									const VECTOR  &vMotion,
									const float	rInertia = 1.0f,
									const float	rE = 0.0f) const;

			void	GetRandomPoint(VECTOR &Point) const;

			_INLINE_ void	Build(const VECTOR &Center, const VECTOR &CylDir, const float rCylRadius, const float rCylHeight);
			
			// Accessors
			_INLINE_ float			GetHeight() const;
			_INLINE_ float			GetRadius() const;
			_INLINE_ const VECTOR & GetAxis() const;
			_INLINE_ const VECTOR & GetXDir() const;
			_INLINE_ const VECTOR & GetYDir() const;

			// Mutators
			_INLINE_ void			SetCenter(const VECTOR &Pos);
			_INLINE_ void			SetAxis(const VECTOR &V);
			_INLINE_ void			SetHeight(const float V);
			_INLINE_ void			SetRadius(const float V);
			
			bool operator == (const CYLINDER& C) const;

		#ifndef _MASTER_ONLY_DATABANK
			void Read(IO_STREAM &s);
		#endif
			/** */
		#ifndef _MASTER
			void Write(IO_STREAM &s)const;
			/** */
			void WriteBinary(IO_STREAM &s);
		#endif
			/** */
		//	static bool Intersect(const CYLINDER& C, const BOUNDING_BOX& S);
			/** */
		#ifndef _MASTER
			friend IO_STREAM& operator <<(IO_STREAM &s, const CYLINDER &a)
			{
				a.Write(s);
				return s;
			}
		#endif
			/** */
		#ifndef _MASTER_ONLY_DATABANK
			friend IO_STREAM& operator >>(IO_STREAM &s, CYLINDER &a)
			{
				a.Read(s);
				return s;
			}
		#endif

			// For shadow maps
			virtual float		GetOptimalFocalAngle(const VECTOR & Pos, float rAngle, float & rNear, float & rIntersectNear, const VECTOR & FrontDir) const;
		};
	}
}

#ifndef _DEBUG
#include "Cylinder.inl"
#endif

#endif //__KMATH_MATH_BOUNDINGVOLUME_CYLINDER_CYLINDER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
