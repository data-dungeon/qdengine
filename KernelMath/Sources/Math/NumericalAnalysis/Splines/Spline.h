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
//	CLASS:	SPLINE
//	The SPLINE class implements ...
//
//	02-12-18:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Template/Qdt_Vector)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace	KMATH
	{
		class	SPLINE
		{
		public:
			static const Int32 SPLINE_NBSEGS			=32;
			static const Int32 SPLINE_INVERSION_QUALITY	=16;

			struct SPLINE_INFO
			{
				Float32	_rLength;							// Bezier real lenght
				Float32	_arHermiteCoeffs[SPLINE_NBSEGS+1];	// conversion table
			};

			struct	HERMITE_POINT
			{
				VECTOR	_Point;
				VECTOR	_InTangent;
				VECTOR	_OutTangent;
			};

			struct	INTERSECTION_INFO
			{
				VECTOR	_HitPoint;
				Float32	_rHitCoeff;
			};

			//Ctor & Dtor
						SPLINE();
						~SPLINE();

			// manipulators
			_INLINE_	void					Clear(void);
			_INLINE_	void					AddPoint(const VECTOR& Point);
			_INLINE_	Int32					GetNbrPoints(void) const;
			_INLINE_	const VECTOR&			GetPoint(const Int32 nIndex) const;
			_INLINE_ 	const VECTOR&			GetInTangent(const Int32 nIndex) const;
			_INLINE_ 	const VECTOR&			GetOutTangent(const Int32 nIndex) const;

			_INLINE_	void					AddHermitePoint(const HERMITE_POINT& HermitePoint);
			_INLINE_	HERMITE_POINT			GetHermitePoint(const Int32 nIndex) const;

			_INLINE_	Float32					GetLength(void) const;

			_INLINE_ 	void					SetIntersectionStep(const Float32 rStep);
			_INLINE_ 	Float32					GetIntersectionStep(void) const;
			_INLINE_ 	VECTOR					GetPointVectorIntersectionValue(const Int32 nIndex) const;
			_INLINE_ 	Int32					GetNbrPointVectorIntersections(void) const;
			_INLINE_ 	VECTOR					GetPlaneIntersectionValue(const Int32 nIndex) const;
			_INLINE_ 	Int32					GetNbrPlaneIntersections(void) const;
			_INLINE_ 	Float32					GetPointVectorIntersectionCoeff(const Int32 nIndex) const;
			_INLINE_ 	Float32					GetPlaneIntersectionCoeff(const Int32 nIndex) const;

			// Spline Math
						void					ComputeLenghts(void);
						Float32					CartesianRatioToSplineRatio(const Float32 rRatio) const;
						void					GetCartesianPosition(Float32 rRatio, VECTOR* pPosition, VECTOR* pTangent=NULL) const;

						void					AutoComputeTangents(void);

						Int32					RetreiveIndex(Float32 rCoeff) const;
						VECTOR					GetInterpolatedPoint(const Float32 rCoeff) const;
						VECTOR					GetInterpolatedTangent(const Float32 rCoeff) const;
						VECTOR					GetHermiteInterpolatedPoint(const Float32 rCoeff) const;

						VECTOR					GetNearestPointCoeff(const VECTOR& SourcePoint, Float32& rNearestCoeff, const Int32 nSubdivisions) const;

			// Spline Intersections
						void					FindPointVectorIntersection(const VECTOR& Position, const VECTOR& Direction);
						void					FindPointVectorIntersectionLinear(const VECTOR& Position, const VECTOR& Direction);
						void					FindPointVectorIntersectionSpline(const VECTOR& Position, const VECTOR& Direction);

						void					FindPlaneIntersection(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect);
						void					FindPlaneIntersectionLinear(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect);
						void					FindPlaneIntersectionSpline(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect);

		private:
			QDT_VECTOR<HERMITE_POINT>		_vHermitePoints;
			QDT_VECTOR<SPLINE_INFO>			_vSplineInfo;
			Float32							_rTotalLength;			// Total Lenght of spline

			//collision
			Float32							_rIntersectionStep;
			QDT_VECTOR<INTERSECTION_INFO>	_vPointVectorIntersections;
			QDT_VECTOR<INTERSECTION_INFO>	_vPlaneIntersections;

		};
	}
}

#ifndef _DEBUG
#include	"Spline.inl"
#endif

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
