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
//	CLASS:	VERTICAL_RAY
//	The VERTICAL_RAY class implements a strictly vertical ray. 
//	The ray has an origin & is casted verticaly both in positive & negative
//	direction.
//
//	02-12-16:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_VERTICALRAY_VERTICALRAY_H__
#define __KMATH_MATH_VERTICALRAY_VERTICALRAY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(IO_STREAM)
DECL_KMATH(TRIANGLE)

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace KMATH
	{
		class	VERTICAL_RAY
		{
		public:

								VERTICAL_RAY();
								VERTICAL_RAY(const VECTOR & Origin);
								VERTICAL_RAY(const VERTICAL_RAY & C);

							~VERTICAL_RAY();

			VERTICAL_RAY &		operator = (const VERTICAL_RAY & C);

			void				TestIntersection(const TRIANGLE & Triangle, bool & bAbove, bool & bBelow);
			_INLINE_ bool		TestIntersection(const VECTOR & P1, const VECTOR & P2, const VECTOR & P3, bool & bAbove, bool & bBelow);

			const VECTOR &		GetOrigin() const					{ return (_Origin); }
			float				GetPosDistance() const				{ return (_rPosDistance); }
			float				GetNegDistance() const				{ return (_rNegDistance); }

			void				SetOrigin(const VECTOR & Origin)	{ _Origin = Origin; }
			void				SetPosDistance(float rPosDistance)	{ _rPosDistance = rPosDistance; }
			void				SetNegDistance(float rNegDistance)	{ _rNegDistance = rNegDistance; }

			bool				TestIntersectionAbove(const TRIANGLE & Triangle);
			_INLINE_ bool		TestIntersectionAbove(const VECTOR & P1, const VECTOR & P2, const VECTOR & P3);
			
			bool				TestIntersectionBelow(const TRIANGLE & Triangle);
			_INLINE_ bool		TestIntersectionBelow(const VECTOR & P1, const VECTOR & P2, const VECTOR & P3);

		private:

			VECTOR				_Origin;
			float				_rPosDistance;
			float				_rNegDistance;
		};
	}
}

#ifndef	_DEBUG
#include	"VerticalRay.inl"
#endif

#endif //__KMATH_MATH_VERTICALRAY_VERTICALRAY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
