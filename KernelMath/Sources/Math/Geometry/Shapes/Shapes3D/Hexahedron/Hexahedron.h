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
//	CLASS:	HEXAHEDRON
//	The HEXAHEDRON class implements ...
//
//	01-10-09:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES3D_HEXAHEDRON_HEXAHEDRON_H__
#define __KMATH_MATH_SHAPES_SHAPES3D_HEXAHEDRON_HEXAHEDRON_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(OBB)
DECL_KMATH(VECTOR)
DECL_KMATH(MATRIX)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KCORE(Template/QDT_Vector)

namespace	QDT
{
	namespace KMATH
	{
		class	HEXAHEDRON
		{
		public:

			enum	HEXADRON_PLANE
			{
				HP_LEFT,
				HP_RIGHT,
				HP_TOP,
				HP_BOTTOM,
				HP_NEAR,
				HP_FAR,
			};


										HEXAHEDRON();
										HEXAHEDRON(const PLANE & PlaneNear, const PLANE & PlaneFar, const PLANE & PlaneLeft, const PLANE & PlaneRight, const PLANE & PlaneTop, const PLANE & PlaneBottom);
			_INLINE_					HEXAHEDRON(const HEXAHEDRON & C);
									~HEXAHEDRON();

						HEXAHEDRON &	operator=(const HEXAHEDRON & C);
						void			Copy(const HEXAHEDRON & C);

			_INLINE_	const PLANE &	GetPlaneNear() const;
			_INLINE_	const PLANE &	GetPlaneFar() const;
			_INLINE_	const PLANE &	GetPlaneLeft() const;
			_INLINE_	const PLANE &	GetPlaneRight() const;
			_INLINE_	const PLANE &	GetPlaneTop() const;
			_INLINE_	const PLANE &	GetPlaneBottom() const;
						const PLANE &	GetPlane(HEXADRON_PLANE Plane) const;

						void			ComputePoints(QDT_VECTOR<VECTOR> & Points);
						void			ComputePoints(VECTOR * pPoints) const;

						bool			Intersect(const VECTOR & Point) const;

						void			SetPlanes(const PLANE & PlaneNear, const PLANE & PlaneFar, const PLANE &PlaneLeft, const PLANE &PlaneRight, const PLANE &PlaneTop, const PLANE & PlaneBottom);
						void			Transform(const MATRIX & Matrix);
						void			NormalizeNormals();

						bool			GetCenter(VECTOR & Center) const;

						void			SetFromObb(const OBB & Obb);
			// This method is not optimized and should not be called at frame time.
						bool			SetFromPointsAndHeight(const VECTOR& Point1, const VECTOR& Point2, const VECTOR& Point3, const VECTOR& Point4, const float rHeight);

						bool			GetPoint(VECTOR & Point, const HEXADRON_PLANE Plane1, const HEXADRON_PLANE Plane2, const HEXADRON_PLANE Plane3) const;

			// Returns 'true' is the normals are outside oriented and if the hexahedron isn't self-crossing.
			// WARNING : this method is heavy CPU consuming and not optimized, so don't use it at frame time.
			// Typically, use it in a QDT_ASSERT.
						bool			IsValid() const;

						void			Display(COLOR_FLOAT & Color) const;

			// Stream
						void			Read(IO_STREAM &s);
						void			Write(IO_STREAM &s) const;

			friend IO_STREAM & operator << (IO_STREAM & Stream, const HEXAHEDRON & BV)
			{
				BV.Write(Stream);
				return (Stream);
			}

			friend IO_STREAM & operator >> (IO_STREAM & Stream, HEXAHEDRON & BV)
			{
				BV.Read(Stream);
				return (Stream);
			}


		private:

						void			SetNormals();


		private:

			PLANE			_PlaneNear;
			PLANE			_PlaneFar;
			PLANE			_PlaneLeft;
			PLANE			_PlaneRight;
			PLANE			_PlaneTop;
			PLANE			_PlaneBottom;
		};
	}
}

#ifndef _DEBUG
#include "Hexahedron.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES3D_HEXAHEDRON_HEXAHEDRON_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
