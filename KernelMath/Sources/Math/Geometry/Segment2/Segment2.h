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
//	CLASS:	SEGMENT_2
//	The SEGMENT_2 class implements ...
//
//	03-03-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SEGMENT2_SEGMENT2_H__
#define __KMATH_MATH_SEGMENT2_SEGMENT2_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)

namespace QDT
{
	namespace KMATH
	{
		class	SEGMENT_2
		{
		public:

			// Constructor & destructor
							SEGMENT_2();
							SEGMENT_2(const VECTOR_2 & Point0, const VECTOR_2 & Point1);
							SEGMENT_2(const SEGMENT_2 & C);
						~SEGMENT_2();

			// Operators
			SEGMENT_2 &		operator=(const SEGMENT_2 & C);

			// Accessors
			const VECTOR_2&	GetPoint0() const				{ return (_Point0); }
			const VECTOR_2& GetPoint1() const				{ return (_Point1); }
			const VECTOR_2&	GetOrigin() const				{ return (_Point0); }
			const VECTOR_2	GetDirection() const			{ VECTOR_2 Dir; Dir.Sub(_Point1, _Point0); return (Dir); }

			// Modifiors
			void			SetPoint0(const VECTOR_2 &P)	{ _Point0 = P; }
			void			SetPoint1(const VECTOR_2 &P)	{ _Point1 = P; }

			bool			FindIntersection(const SEGMENT_2 &S, int &nQuantity, float rT[2]) const;

		private:

			bool			Find(const VECTOR_2& vPos, const VECTOR_2& vDir, VECTOR_2& vDiff, float& rDirSqrLen, int &nQuantity, float rT[2]) const;
			
			VECTOR_2		_Point0;
			VECTOR_2		_Point1;
		};
	}
}

#endif //__KMATH_MATH_SEGMENT2_SEGMENT2_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
