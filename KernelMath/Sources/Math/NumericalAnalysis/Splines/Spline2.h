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
//	CLASS:	SPLINE2
//	The SPLINE class implements ...
//
//	06-01-27:	BFA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE2_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE2_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/NumericalAnalysis/Splines/SplineTemplate)
#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)

namespace	QDT
{
	namespace	KMATH
	{
		class	SPLINE_2 : public SPLINE_TEMPLATE<VECTOR_2>
		{
		public:
		};
	}
}

#ifndef _DEBUG
#include	"Spline2.inl"
#endif

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE2_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
