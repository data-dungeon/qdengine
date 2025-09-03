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
//	CLASS:	SEGMENT_2D
//	The SEGMENT_2D class implements ...
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SHAPES_SHAPES1D_SEGMENT2D_SEGMENT2D_H__
#define __KMATH_MATH_SHAPES_SHAPES1D_SEGMENT2D_SEGMENT2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)

namespace QDT
{
	namespace KMATH
	{
		class	SEGMENT2D
		{
		public:

			/** Constructor & Destructor */
			_INLINE_					SEGMENT2D(bool bFlagInit = true);
			_INLINE_					SEGMENT2D(const POINT2D & Pt1, const POINT2D & Pt2);
			_INLINE_				   ~SEGMENT2D();

			/** Accessors & modifiors */
			_INLINE_ const POINT2D &	GetPoint1() const;
			_INLINE_ const POINT2D &	GetPoint2() const;
			_INLINE_ void				SetPoint1(const POINT2D & Pt);
			_INLINE_ void				SetPoint2(const POINT2D & Pt); 

			/** Streams */
			void						Read(IO_STREAM & Stream);
			void						Write(IO_STREAM & Stream) const;

			friend IO_STREAM&			operator << (IO_STREAM & Stream, const SEGMENT2D & Sg)
										{
											Sg.Write(Stream);
											return Stream;
										}
			friend IO_STREAM&			operator >> (IO_STREAM & Stream, SEGMENT2D & Sg)
										{
											Sg.Read(Stream);
											return Stream;
										}

		private:

			POINT2D	_Point1;
			POINT2D _Point2;
		};
	}
}

#ifndef _DEBUG
#include "Segment2D.inl"
#endif

#endif //__KMATH_MATH_SHAPES_SHAPES1D_SEGMENT2D_SEGMENT2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
