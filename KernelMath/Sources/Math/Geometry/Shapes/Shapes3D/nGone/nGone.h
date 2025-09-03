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
//	CLASS:	NGONE
//	The NGONE class implements ...
//
//	06-06-30:	BFA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES3D_NGONE_NGONE_H__
#define __KMATH_MATH_SHAPES_SHAPES3D_NGONE_NGONE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

#define MAX_NGONE	30

namespace	QDT
{
	namespace KMATH
	{
		class	NGONE
		{
		public:
						NGONE();
						NGONE(NGONE& NGone);

			void		AddVertex(const VECTOR& vPosition);

			UInt32		ClipOnXPlane(Float32 rPlanePositionX, Bool bClipSide);
			UInt32		ClipOnYPlane(Float32 rPlanePositionY, Bool bClipSide);
			UInt32		ClipOnZPlane(Float32 rPlanePositionZ, Bool bClipSide);
			Float32		ComputeSurface(const VECTOR& vNormal) const;

			//inlines
			_INLINE_ UInt32			GetVertexCount(void) const;
			_INLINE_ const VECTOR&	GetVertex(UInt32 nVertexId) const;

		protected:
			VECTOR		_aVertices[MAX_NGONE];
			UInt32		_nVertexCount;
		};
	}
}

#ifndef _DEBUG
#include "nGone.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES3D_NGONE_NGONE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
