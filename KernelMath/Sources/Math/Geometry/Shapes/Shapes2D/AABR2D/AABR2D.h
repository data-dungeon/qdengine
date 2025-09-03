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
//	CLASS:	AABR2D
//	The AABR2D class implements ...
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES2D_AABR2D_AABR2D_H__
#define __KMATH_MATH_SHAPES_SHAPES2D_AABR2D_AABR2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(POINT2D)
DECL_KMATH(TRIANGLE2D)
DECL_KCORE(IO_STREAM)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment2D/Segment2D)

namespace QDT
{
	namespace KMATH
	{
		class	AABR2D
		{
		protected :
			//=========================================================================
			//  NE PAS MODIFIER LA STRUCTURE EN MEMOIRE
			//                       ou alors modifier l'operateur []
			//=========================================================================
			float _rXMin;
			float _rYMin;
			float _rXMax;
			float _rYMax;

		public:
			//=============================================================================
			// ONA addons : Side Disposition
			//
			//			NORTH : 0	EAST : 1
			//			SOUTH : 3	WEST : 2
			//
			// take care : the orientation of the current space is  with a direct orientation
			//
			//					WEST	--------> x		EAST
			//
			//
			//						NORTH
			//
			//						|
			//						|
			//						|
			//						|
			//						|
			//						v y
			//
			//						SOUTH
			//
			//=============================================================================

			enum	SIDE
			{
				SIDE_NORTH,
				SIDE_EAST,
				SIDE_SOUTH,
				SIDE_WEST,
				SIDE_MAX
			};

			/** Constructor & Destructor */
			_INLINE_ AABR2D(bool bFlagInit = true);
			_INLINE_ AABR2D(const AABR2D &aabb);
			_INLINE_ AABR2D(const POINT2D & Min, const POINT2D & Max);
			_INLINE_ virtual ~AABR2D();

			/** Operators */
			_INLINE_ const AABR2D& operator=(const AABR2D &aabb);
			_INLINE_ POINT2D operator[] (unsigned int index) const;

			/** Accessors */
			_INLINE_ float		GetXMin() const;
			_INLINE_ float		GetYMin() const;
			_INLINE_ float		GetXMax() const;
			_INLINE_ float		GetYMax() const;
			_INLINE_ float		GetXCenter() const;
			_INLINE_ float		GetYCenter() const;
			_INLINE_ float		GetSizeX() const;
			_INLINE_ float		GetSizeY() const;
			_INLINE_ POINT2D	GetMin() const;
			_INLINE_ POINT2D	GetMax() const;

			/** Modifiors */
			_INLINE_ void		SetXMin(float rValue);
			_INLINE_ void		SetYMin(float rValue);
			_INLINE_ void		SetXMax(float rValue);
			_INLINE_ void		SetYMax(float rValue);
			
			_INLINE_ bool		Contain(const POINT2D &P) const;

			float				GetIntersectedArea(const TRIANGLE2D &T) const;
			float				GetArea() const;

			void				Grow(const POINT2D &P);
			void				Grow(const AABR2D &Aabr);

			/** Streams */
			virtual void Read(IO_STREAM & Stream);
			virtual void Write(IO_STREAM & Stream) const;

			friend IO_STREAM&	operator << (IO_STREAM & Stream, const AABR2D & Aabb)
								{
									Aabb.Write(Stream);
									return Stream;
								}
			friend IO_STREAM&	operator >> (IO_STREAM & Stream, AABR2D & Aabb)
								{
									Aabb.Read(Stream);
	 								return Stream;
								}

			SEGMENT2D	GetSidedSegment(const SIDE eSide) const;

		protected :
			//=============================================================================
			// JLT addons
			//=============================================================================
			/** */
			_INLINE_ void Copy(const AABR2D &aabb);
		};
	}
}

#ifndef _DEBUG
#include "AABR2D.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES2D_AABR2D_AABR2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
