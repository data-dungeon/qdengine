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
//	CLASS:	POLYHEDRON_2D
//	The POLYHEDRON_2D class implements ...
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_POLYHEDRON2D_POLYHEDRON2D_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_POLYHEDRON2D_POLYHEDRON2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(POINT2D)

#include	INCL_KCORE(Template/QDT_DList)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment2D/Segment2D)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/AABR2D/AABR2D)

namespace QDT
{
	namespace KMATH
	{
		class	POLYHEDRON2D
		{
		public:

			/** Constructor & destructor */
			_INLINE_ 			POLYHEDRON2D();
			_INLINE_			POLYHEDRON2D(const POINT2D & Pt1, const POINT2D & Pt2, const POINT2D & Pt3);
			_INLINE_		   ~POLYHEDRON2D();

			/** Get the number of segments */
			_INLINE_ int		GetNbrSegments() const;
			_INLINE_ int		GetNbrPoints() const;
			_INLINE_ SEGMENT2D	GetSegment(int nNumSegment) const;
			_INLINE_ POINT2D	GetPoint(int nNumPoint);
			_INLINE_ AABR2D		GetAABR2D();
			
			/** Polyhedron generation */
			_INLINE_ void		AddFirstThreePoints(const POINT2D & Pt1, const POINT2D & Pt2, const POINT2D & Pt3);		// For the first 3 points

			/** Streams */
			void				Read(IO_STREAM & Stream);
			void				Write(IO_STREAM & Stream) const;

			friend IO_STREAM&	operator << (IO_STREAM & Stream, const POLYHEDRON2D & Poly)
								{
									Poly.Write(Stream);
									return Stream;
								}
			friend IO_STREAM&	operator >> (IO_STREAM & Stream, POLYHEDRON2D & Poly)
								{
									Poly.Read(Stream);
									return Stream;
								}

		private:

			QDT_DLIST<POINT2D>	_PointsList;
		};

		#ifndef _DEBUG
		#include "Polyhedron2D.inl"
		#endif
	}
}

#endif //__KMATH_MATH_SHAPES_SHAPES2D_POLYHEDRON2D_POLYHEDRON2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
