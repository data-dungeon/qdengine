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
//	CLASS:	SPLINE3
//	The SPLINE class implements ...
//
//	06-01-27:	BFA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE3_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE3_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/NumericalAnalysis/Splines/SplineTemplate)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)

namespace	QDT
{
	namespace	KMATH
	{
		class	SPLINE_3 : public SPLINE_TEMPLATE<VECTOR>
		{
		public:
			struct	INTERSECTION_INFO
			{
				VECTOR	_HitPoint;
				Float32	_rHitCoeff;
			};
											SPLINE_3();

			// Accessors
			_INLINE_	void				Clear(void);
			_INLINE_ 	void				SetIntersectionStep(const Float32 rStep);
			_INLINE_ 	Float32				GetIntersectionStep(void) const;
			_INLINE_ 	VECTOR				GetPointVectorIntersectionValue(const Int32 nIndex) const;
			_INLINE_ 	Int32				GetNbrPointVectorIntersections(void) const;
			_INLINE_ 	VECTOR				GetPlaneIntersectionValue(const Int32 nIndex) const;
			_INLINE_ 	Int32				GetNbrPlaneIntersections(void) const;
			_INLINE_ 	Float32				GetPointVectorIntersectionCoeff(const Int32 nIndex) const;
			_INLINE_ 	Float32				GetPlaneIntersectionCoeff(const Int32 nIndex) const;

            // Spline Intersections
						void				FindPointVectorIntersection(const VECTOR& Position, const VECTOR& Direction);
						void				FindPointVectorIntersectionLinear(const VECTOR& Position, const VECTOR& Direction);
						void				FindPointVectorIntersectionSpline(const VECTOR& Position, const VECTOR& Direction);

						void				FindPlaneIntersection(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect);
						void				FindPlaneIntersectionLinear(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect);
						void				FindPlaneIntersectionSpline(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect);

		protected:
			//collision
			Float32							_rIntersectionStep;
			QDT_VECTOR<INTERSECTION_INFO>	_vPointVectorIntersections;
			QDT_VECTOR<INTERSECTION_INFO>	_vPlaneIntersections;

		};
	}
}

#ifndef _DEBUG
#include	"Spline3.inl"
#endif

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE3_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
