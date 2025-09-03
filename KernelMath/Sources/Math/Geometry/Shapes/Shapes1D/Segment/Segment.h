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
// CLASS: SEGMENT
//
//
//00-11-10 JVA Created: 
//*****************************************************************************

#ifndef __KMATH_MATH_SHAPES_SHAPES1D_SEGMENT_SEGMENT_H__
#define __KMATH_MATH_SHAPES_SHAPES1D_SEGMENT_SEGMENT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(MATRIX)

#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace KMATH
	{
		class	SEGMENT
		{

		public:

			/** Constructor & destructor */
			_INLINE_ 					SEGMENT();
			_INLINE_					SEGMENT(const SEGMENT &Segment);
			_INLINE_					SEGMENT(const VECTOR &Pt1, const VECTOR &Pt2);

			/** Accessors */
			_INLINE_ void				SetPoint1(const VECTOR & Pt1);
			_INLINE_ void				SetPoint2(const VECTOR & Pt2);
			_INLINE_ void				SetPoints(const VECTOR & Pt1, const VECTOR & Pt2);
			_INLINE_ const VECTOR &		GetPoint1() const;
			_INLINE_ const VECTOR &		GetPoint2() const;
			_INLINE_ void				GetPoint(VECTOR & Pt1, VECTOR & Pt2) const;

					bool				IsOn(const VECTOR &P) const;

			/** Length */
					float				Length() const;
					float				SquareLength() const;

					float				SqrDistance(const VECTOR & P, Float32* prProjectionRatio=NULL);

			/** Transform */
					void				Transform(const MATRIX &Matrix);

			/** Display segment */
			void						Display(const COLOR_FLOAT & Color) const;

			_INLINE_ void				GetRandomPoint(VECTOR &Point) const;

			/** Stream */
			void						Read(IO_STREAM & Stream);
			void						Write(IO_STREAM & Stream) const;

			friend IO_STREAM&			operator << (IO_STREAM & Stream, const SEGMENT & Segment)
			{
				Segment.Write(Stream);
				return Stream;
			}
			friend IO_STREAM&			operator >> (IO_STREAM & Stream, SEGMENT & Segment)
			{
				Segment.Read(Stream);
				return Stream;
			}

		private:

			VECTOR			_Point1;
			VECTOR			_Point2;
		};
	}
}

#ifndef _DEBUG
#include "segment.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES1D_SEGMENT_SEGMENT_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
