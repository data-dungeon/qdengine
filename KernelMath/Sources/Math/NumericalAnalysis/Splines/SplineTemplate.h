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
//	CLASS:	SPLINE_TEMPLATE
//	The SPLINE_TEMPLATE class implements ...
//
//	02-12-18:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINETEMPLATE_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINETEMPLATE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Template/Qdt_Vector)
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Distance/Distance)
#include	INCL_KMATH(Math/NumericalAnalysis/Interpolation/Interpolation)

namespace	QDT
{
	namespace	KMATH
	{
		static const Int32 SPLINE_NBSEGS			=32;
		static const Int32 SPLINE_INVERSION_QUALITY	=16;

		struct	SPLINE_INFO
		{
			Float32		_rLength;							// Bezier real lenght
			Float32		_arHermiteCoeffs[SPLINE_NBSEGS+1];	// conversion table
		};

		class REPARAMETRIZATION_DATA
		{
		public:
			void Clear(void)
			{
				_vSplineInfo.Clear();
				_rTotalLength=-1.0f;
			}

			QDT_VECTOR<SPLINE_INFO>	_vSplineInfo;		// Spline Sampled/processed data
			Float32					_rTotalLength;		// Total Lenght of spline
		};

		template <class VECTOR_T>
		struct	HERMITE_POINT
		{
			VECTOR_T	_Point;
			VECTOR_T	_InTangent;
			VECTOR_T	_OutTangent;
		};

		template <class VECTOR_T>
		class	SPLINE_TEMPLATE
		{
		protected:
            //Measurement functions
			typedef Float32	(*MeasureFunc)(const VECTOR_T&, const VECTOR_T&);
			static	Float32	MeasureAlong		(const VECTOR_T& vBefore, const VECTOR_T& vAfter) { return DISTANCE::DistancePointPoint(vBefore,vAfter); }
			static	Float32	MeasureProjectionX	(const VECTOR_T& vBefore, const VECTOR_T& vAfter) { Float32 rDistance =vAfter.GetX()-vBefore.GetX(); QDT_ASSERT(rDistance>=0.0f); return rDistance; }

		public:

			//Ctor & Dtor
			_INLINE_						SPLINE_TEMPLATE();
			_INLINE_						~SPLINE_TEMPLATE();

			// - Standard Methods -
			_INLINE_	void					Clear(void);
			_INLINE_	void					AddPoint(const VECTOR_T& Point);
			_INLINE_	Int32					GetNbrPoints(void) const;
			_INLINE_	Int32					GetNbrParts(void) const;
			_INLINE_	const VECTOR_T&			GetPoint(const Int32 nIndex) const;
			_INLINE_	void					SetPoint(const Int32 nIndex, const VECTOR_T& Value);
			_INLINE_ 	const VECTOR_T&			GetInTangent(const Int32 nIndex) const;
			_INLINE_ 	const VECTOR_T&			GetOutTangent(const Int32 nIndex) const;
			_INLINE_	void					AddHermitePoint(const HERMITE_POINT<VECTOR_T>& HermitePoint);
			_INLINE_	const HERMITE_POINT<VECTOR_T>&	GetHermitePoint(const Int32 nIndex) const;
			_INLINE_	Int32					RetreiveIndex(Float32 rCoeff) const;
			_INLINE_	void					ComputeCardinalTangents(const Float32 rCardinalCoeff =MATH::PI()/4.0f);
			_INLINE_	VECTOR_T				GetInterpolatedPoint(const Float32 rCoeff) const;
			_INLINE_	VECTOR_T				GetInterpolatedTangent(const Float32 rCoeff) const;
			_INLINE_	VECTOR_T				GetHermiteInterpolatedPoint(const Float32 rCoeff) const;
			_INLINE_	VECTOR_T				GetNearestPointCoeff(const VECTOR_T& SourcePoint, Float32& rNearestCoeff, const Int32 nSubdivisions) const;

			// - Reparametrization Stuff -
			_INLINE_	Float32							GetTotalLength(const REPARAMETRIZATION_DATA* pReparametrizationData) const;
			_INLINE_	Float32							GetPartLength(const REPARAMETRIZATION_DATA* pReparametrizationData, const Int32 nPartIndex) const;
			_INLINE_	const SPLINE_INFO&				GetSplineInfo(const REPARAMETRIZATION_DATA* pReparametrizationData, const Int32 nPartIndex) const;
			_INLINE_	Float32							CartesianRatioToSplineRatio(const REPARAMETRIZATION_DATA* pReparametrizationData, const Float32 rRatio) const;
			_INLINE_	Float32							PartRatioToSplineRatio(const REPARAMETRIZATION_DATA* pReparametrizationData, const Int32 nPartIndex, const Float32 rPartRatio) const;
			_INLINE_	void							GetCartesianPosition(const REPARAMETRIZATION_DATA* pReparametrizationData, Float32 rRatio, VECTOR_T* pPosition, VECTOR_T* pTangent=NULL) const;
			_INLINE_	const REPARAMETRIZATION_DATA*	GetReparametrizationDataAlong(void) const;
			_INLINE_	const REPARAMETRIZATION_DATA*	GetReparametrizationDataProjectionX(void) const;

			// reparam builder/rebuilder
			_INLINE_	void				BuildReparametrizationDataAlong(void);
			_INLINE_	void				BuildReparametrizationDataProjectionX(void);
			_INLINE_	void				BuildReparametrizationData(REPARAMETRIZATION_DATA* pReparametrizationData, MeasureFunc pMesureFunc);
			_INLINE_	void				CheckRebuildReparametrizationData(void);
			_INLINE_	void				ResetReparametrizationData(REPARAMETRIZATION_DATA** ppReparametrizationData);

		protected:

			QDT_VECTOR< HERMITE_POINT<VECTOR_T> >	_vHermitePoints;

			REPARAMETRIZATION_DATA*		_pReparametrizationAlong;
			REPARAMETRIZATION_DATA*		_pReparametrizationProjectionX;
		};
	}
}

#include	"SplineTemplate.inl"

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_SPLINES_SPLINETEMPLATE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
