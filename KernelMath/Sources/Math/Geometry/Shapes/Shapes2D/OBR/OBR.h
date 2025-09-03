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
//	CLASS:	OBR
//	The OBR class implements ...
//
//	01-10-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_OBR_OBR_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_OBR_OBR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(MATRIX)
DECL_KMATH(PLANE)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	OBR
		{
		public:
						OBR();
				OBR(const OBR & C);
						OBR(int nNbrPoints, VECTOR *pPoints, float rEpsilon = 0.1f);
					~OBR();

			OBR &	operator=(const OBR & C);
			void	Copy(const OBR & C);

			void					WriteBinary(IO_STREAM& Stream);

			_INLINE_ const VECTOR&	GetCenter() const;
			_INLINE_ const VECTOR&	GetDirection1() const;
			_INLINE_ const VECTOR&	GetDirection2() const;
			_INLINE_ const float	GetDistance1() const;
			_INLINE_ const float	GetDistance2() const;

			/** Display the OBB (wireframe). Usefull for debugging. */
			void					Display(const COLOR_FLOAT & Color) const;
			/** Generate bounding planes from a point. */
			bool					GeneratePlanes(const VECTOR& Point, PLANE (& pPlanes)[4]) const;
			/** Transform. */
			void					Transform(const MATRIX &Matrix);
			/** Get a plane given a OBR normal. */
			PLANE					GetPlane(const VECTOR &vNormal);

			float					GetArea() const;

		private:

			void					Compute(int nNbrPoints, VECTOR *pPoints, float rEpsilon);


			VECTOR		_Center;
			VECTOR		_vDirection1;
			VECTOR		_vDirection2;
			float		_rDistance1;
			float		_rDistance2;
			int			_Padding[2];
		};
	}
}

#ifndef _DEBUG
#include "OBR.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_OBR_OBR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
