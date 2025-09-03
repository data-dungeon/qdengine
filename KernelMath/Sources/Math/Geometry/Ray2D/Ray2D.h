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
//	CLASS:	RAY_2D
//	The RAY_2D class implements ...
//
//	03-03-14:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_RAY2D_RAY2D_H__
#define __KMATH_MATH_RAY2D_RAY2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(SEGMENT_2)

#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)

namespace QDT
{
	namespace KMATH
	{
		class	RAY_2D
		{
		public:

			// Constructor & destructor
							RAY_2D();
							RAY_2D(const VECTOR_2 &vOrigin, const VECTOR_2 &vDirection);
							RAY_2D(const RAY_2D & C);
						~RAY_2D();

			// Operators
			RAY_2D &		operator = (const RAY_2D & C);

			// Accessors
			const VECTOR_2&	GetOrigin() const				{ return (_vOrigin); }
			const VECTOR_2& GetDirection() const			{ return (_vDirection); }

			// Modifiors
			void			SetPos(const VECTOR_2 &vO)	{ _vOrigin = vO; }
			void			SetDir(const VECTOR_2 &vD)	{ _vDirection = vD; }

			bool			FindIntersection(const SEGMENT_2 &Segment, int &nQuantity, float rT[2]) const;
			bool			FindIntersection(const VECTOR_2	&vCenter, float rRadius, int &nQuantity, VECTOR_2 vT[2]) const;
			bool			FindIntersection(const VECTOR_2	&vCenter, float rRadius, VECTOR_2 &vIntersection) const;

		private:

			bool			Find(const VECTOR_2& vPos, const VECTOR_2& vDir, VECTOR_2& vDiff, float& rD0SqrLen, int &nQuantity, float rT[2]) const;
			bool			Find(const VECTOR_2& vCenter, float rRadius, int &nQuantity, float rT[2]) const;

			VECTOR_2		_vOrigin;
			VECTOR_2		_vDirection;
		};
	}
}

#endif //__KMATH_MATH_RAY2D_RAY2D_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
