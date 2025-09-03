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
//
//	02-12-18:	JJA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Numerical Analysis/Splines/Spline)
#include	INCL_KMATH(Math/Numerical Analysis/Interpolation/Interpolation)
#include	INCL_KMATH(Math/Geometry/Intersection/Intersection)

#ifdef _DEBUG
#include	"Spline.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SPLINE constructor
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
SPLINE::SPLINE()
:
_rIntersectionStep(0.1f),
_rTotalLength(-1.0f)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		SPLINE destructor
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
SPLINE::~SPLINE()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ComputeLenghts
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
void SPLINE::ComputeLenghts(void)
{
	_vSplineInfo.Clear();

	Int32 nSplineCount =GetNbrPoints()-1;

	if (nSplineCount<=0)
	{
		return;
	}

	_vSplineInfo.Reserve(nSplineCount);			//since resize doesn't exist

	Float32 rCurGlobalLength =0.0f;

	Int32 iSpline;
	for(iSpline =0; iSpline<nSplineCount; ++iSpline)
	{
		const Int32 nSampleCount=SPLINE_NBSEGS*SPLINE_INVERSION_QUALITY;

		Float32	aSamples[nSampleCount];

		const VECTOR& vPosControl1 =GetPoint(iSpline);
		const VECTOR& vPosControl2 =GetPoint(iSpline+1);

		VECTOR vPosCur= vPosControl1;

		//build a fine table of value
		Float32 rCurSplineLength =0.0f;
		Int32 iSample;
		for(iSample =0; iSample<nSampleCount; ++iSample)
		{
			Float32 rHermiteCoeff =Float32(iSample)/Float32(nSampleCount);
			VECTOR	vPosNext	  =INTERPOLATION<VECTOR>::HermiteInterpolation(GetPoint(iSpline), GetPoint(iSpline+1), GetOutTangent(iSpline), GetInTangent(iSpline+1), rHermiteCoeff);
			Float32 rDistance	  =DISTANCE::DistancePointPoint(vPosNext,vPosCur);
			aSamples[iSample]	  =rCurSplineLength+rDistance;
			rCurSplineLength	  +=rDistance;
			vPosCur				  =vPosNext;
		}

		SPLINE_INFO splInfo;
		splInfo._rLength =rCurSplineLength;

		//now inverse curve
		splInfo._arHermiteCoeffs[0] =0.0f;

		Int32 iSegment;
		Int32 iIndex=0;
		for(iSegment =0; iSegment<SPLINE_NBSEGS; ++iSegment)
		{
			Float32 rRatio=Float32(iSegment+1)/Float32(SPLINE_NBSEGS);

			//now find spline ratio for this ratio using fine table (aSamples)
			while((aSamples[iIndex]/rCurSplineLength)<rRatio)
			{
				++iIndex;
			}
			--iIndex;	//rewind a little

			Float32 rBefore =aSamples[iIndex]/rCurSplineLength;
			Float32 rAfter  =aSamples[iIndex+1]/rCurSplineLength;

			Float32 rGap =rAfter-rBefore;

			Float32 rGapRatio =(rRatio-rBefore)/rGap;

			Float32 rOutValue =MATH::Blend(Float32(iIndex+1)/nSampleCount,Float32(iIndex+2)/nSampleCount,rGapRatio);

			splInfo._arHermiteCoeffs[iSegment+1] =rOutValue;
		}

		rCurGlobalLength +=rCurSplineLength;

		//push
		_vSplineInfo.PushTail(splInfo);
	}

	_rTotalLength =rCurGlobalLength;
}

//-----------------------------------------------------------------------------
//	Name:		CartesianRatioToSplineRatio
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
Float32 SPLINE::CartesianRatioToSplineRatio(const Float32 rRatio) const
{
	QDT_ASSERT_MSG(_rTotalLength>=0.0f,"YOU MUST CALL ComputeLenghts() before using this method !");
	QDT_ASSERT(rRatio>=0.0f);
	QDT_ASSERT(rRatio<=1.0f);

	const Float32 rFullLengthRatio =rRatio*_rTotalLength;

	// seek to the good spline index
	Int32	iSpline =0;
	const Int32	nbSplines =_vSplineInfo.GetSize();
	Float32	rCurrentDist =_vSplineInfo[0]._rLength;
	while((iSpline<nbSplines) && (rCurrentDist<rFullLengthRatio))
	{
		++iSpline;
		rCurrentDist +=_vSplineInfo[iSpline]._rLength;
	}

	//found spline ... now search for the good Hermite coeff.
	Float32 rAfter	=rCurrentDist;
	Float32 rBefore =rCurrentDist-_vSplineInfo[iSpline]._rLength;
	Float32 rGap	=rAfter-rBefore;
	Float32 rGapRatio =(rFullLengthRatio-rBefore)/rGap;

	Float32 rLutIndex = rGapRatio*Float32(SPLINE_NBSEGS);
	Int32	nLutIndex = (Int32) rLutIndex;

	Float32 rHermiteCoeff;

	//linear interpolation
	Float32 rIntervalRatio =rLutIndex-Float32(nLutIndex);
	rHermiteCoeff =MATH::Blend(_vSplineInfo[iSpline]._arHermiteCoeffs[nLutIndex],_vSplineInfo[iSpline]._arHermiteCoeffs[nLutIndex+1],rIntervalRatio);

	QDT_ASSERT(rHermiteCoeff>=0.0f);
	QDT_ASSERT(rHermiteCoeff<=1.0f);

	Float32 rSplineRatio =(Float32(iSpline)+rHermiteCoeff)/Float32(nbSplines);

	return rSplineRatio;
}

//-----------------------------------------------------------------------------
//	Name:		GetCartesianPosition
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
void SPLINE::GetCartesianPosition(Float32 rRatio, VECTOR* pPosition, VECTOR* pTangent) const
{
	// Convert ratio
	Float32	rSplineRatio =CartesianRatioToSplineRatio(rRatio);

	// Apply position/tangent
	if	(pPosition)
	{
		*pPosition =GetInterpolatedPoint(rSplineRatio);
	}

	if	(pTangent)
	{
		*pTangent =GetInterpolatedTangent(rSplineRatio);
	}

}

//-----------------------------------------------------------------------------
//	Name:		AutoComputeTangents
//	Object:		
//	03-01-29:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::AutoComputeTangents(void)
{
	const Float32 rTangentScaleValue =MATH::PI()/4.0f;		// perfect for 4points circles

	if	(GetNbrPoints()>0)
	{
		// Fill tangents ...
		Int32	nPointIdx;
		for	(nPointIdx=1; nPointIdx<GetNbrPoints()-1; ++nPointIdx)
		{
			VECTOR	Tangent;
			Tangent.SetFromPoints(GetPoint(nPointIdx-1), GetPoint(nPointIdx+1));
			Tangent *=rTangentScaleValue;

			// Assign In And Out Tangent
			_vHermitePoints[nPointIdx]._InTangent	=Tangent;
			_vHermitePoints[nPointIdx]._OutTangent	=Tangent;
		}

		// Fill tangents (extremities)
		if	(GetHermitePoint(0)._Point==GetHermitePoint(GetNbrPoints()-1)._Point)
		{	// Closed spline
			VECTOR	Tangent;
			Tangent.SetFromPoints(GetPoint(GetNbrPoints()-2), GetPoint(1));
			Tangent *=rTangentScaleValue;

			_vHermitePoints[0]._OutTangent				 =Tangent;
			_vHermitePoints[GetNbrPoints()-1]._InTangent =Tangent;
		}
		else
		{
			// Open spline -> Zero Extremities Tangents
			_vHermitePoints[0]._OutTangent				 =VECTOR::ZERO;
			_vHermitePoints[GetNbrPoints()-1]._InTangent =VECTOR::ZERO;
		}
	}
	ComputeLenghts();	//rebuild lenghts
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolatedPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE::GetInterpolatedPoint(const Float32 rCoeff) const
{
	switch(GetNbrPoints())
	{
	case 0:	// No Point
		return VECTOR::ZERO;
		break;

	case 1: // Single Point
		return GetPoint(0);
		break;

	default: // Hermite ( Cubic )
		return GetHermiteInterpolatedPoint(rCoeff);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RetreiveIndex
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
Int32 SPLINE::RetreiveIndex(Float32 rCoeff) const
{
	Float32	rClampCoeff =MATH::Clamp(rCoeff, MATH::EPSILON(), 1.0f-MATH::EPSILON());

	return (Int32) rClampCoeff*(GetNbrPoints()-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetHermiteInterpolatedPoint
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE::GetHermiteInterpolatedPoint(const Float32 rCoeff) const
{
	QDT_ASSERT(rCoeff>=0.0f);
	QDT_ASSERT(rCoeff<=1.0f);

	Int32 nInter =RetreiveIndex(rCoeff);

	// Compute Hermite Coeff
	Float32	rSplineInterval=1.0f/Float32(GetNbrPoints()-1);
	Float32 rHermiteCoeff =(rCoeff-nInter*rSplineInterval) / rSplineInterval;

	// Return Hermite Interpolation
	return INTERPOLATION<VECTOR>::HermiteInterpolation(GetPoint(nInter), GetPoint(nInter+1), GetOutTangent(nInter), GetInTangent(nInter+1), rHermiteCoeff);
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolatedTangent
//	Object:		
//	04-11-19:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE::GetInterpolatedTangent(const Float32	rCoeff) const
{
	QDT_ASSERT(rCoeff>=0.0f);
	QDT_ASSERT(rCoeff<=1.0f);

	Int32 nInter =RetreiveIndex(rCoeff);

	// Compute Hermite Coeff
	Float32	rSplineInterval=1.0f/Float32(GetNbrPoints()-1);
	Float32 rHermiteCoeff =(rCoeff-nInter*rSplineInterval) / rSplineInterval;

	// Return Hermite Tangent
	return INTERPOLATION<VECTOR>::GetHermiteTangent(GetPoint(nInter), GetPoint(nInter+1), GetOutTangent(nInter), GetInTangent(nInter+1), rHermiteCoeff);
}

//-----------------------------------------------------------------------------
//	Name:		FindPointVectorIntersection
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::FindPointVectorIntersection(const VECTOR& Position, const VECTOR& Direction)
{
	// Clear vector
	_vPointVectorIntersections.Clear();

	switch(GetNbrPoints())
	{
	case 0:
		break;

	case 1:
		{
			VECTOR	v;
			v.SetFromPoints(Position, GetPoint(0));

			v.Normalize();

			VECTOR	Dir =Direction;
			Dir.Normalize();

			if	(v==Dir)
			{
				INTERSECTION_INFO	IntersectionInfo;

				IntersectionInfo._HitPoint	=GetPoint(0);
				IntersectionInfo._rHitCoeff	=0.0f;

				_vPointVectorIntersections.PushTail(IntersectionInfo);
			}
		}
		break;

	case 2:
		FindPointVectorIntersectionLinear(Position, Direction);
		break;

	default:
		FindPointVectorIntersectionSpline(Position, Direction);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindPointVectorIntersectionLinear
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::FindPointVectorIntersectionLinear(const VECTOR& Position, const VECTOR& Direction)
{
	SEGMENT	Seg(GetPoint(0), GetPoint(1));
	LINE	SegLine(Seg);
	LINE	RayLine(Position, Direction);
	VECTOR	HitPoint;
	Bool	bMerged;

	if	(INTERSECTION::IntersectLineWithLine(SegLine, RayLine, HitPoint, bMerged))
	{
		VECTOR	v1, v2;

		v1.SetFromPoints(GetPoint(0), HitPoint);
		v2.SetFromPoints(GetPoint(0), GetPoint(1));

		Float32	rCoeff =v1.Dot(v2)/v2.SquareLength();

		if	((rCoeff>=0.0f) && (rCoeff<=1.0f))
		{
			INTERSECTION_INFO	IntersectionInfo;

			IntersectionInfo._HitPoint	=GetInterpolatedPoint(rCoeff);
			IntersectionInfo._rHitCoeff	=rCoeff;

			_vPointVectorIntersections.PushTail(IntersectionInfo);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindPointVectorIntersectionSpline
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::FindPointVectorIntersectionSpline(const VECTOR& Position, const VECTOR& Direction)
{
	Float32 rIntersectionStep	=GetIntersectionStep();
	Float32 rCoeff				=-rIntersectionStep;

	while(rCoeff<(1.0f-rIntersectionStep))
	{
		// Increment Coeff
		rCoeff +=rIntersectionStep;

		VECTOR P1 =GetInterpolatedPoint(rCoeff);
		VECTOR P2 =GetInterpolatedPoint(rCoeff+rIntersectionStep);

		VECTOR	Dir =Direction;
		Dir.Normalize();

		SEGMENT	Seg(P1, P2);
		LINE	SegLine(Seg);
		LINE	RayLine(Position, Direction);
		VECTOR	HitPoint;
		Bool	bMerged;

		if (INTERSECTION::IntersectLineWithLine(SegLine, RayLine, HitPoint, bMerged))
		{
			VECTOR	IntersectVect, RayVect(Direction);

			IntersectVect.SetFromPoints(Position, HitPoint);

			IntersectVect.Normalize();
			RayVect.Normalize();

			VECTOR	v1, v2;
			v1.SetFromPoints(P1, HitPoint);
			v2.SetFromPoints(P1, P2);

			Float32	rCoeffIntersection =v1.Dot(v2) / v2.SquareLength();

			if	((rCoeffIntersection>=0.0f) && (rCoeffIntersection<=1.0f))
			{
				INTERSECTION_INFO	IntersectionInfo;

				IntersectionInfo._HitPoint	=GetInterpolatedPoint(rCoeff+rCoeffIntersection*rIntersectionStep);
				IntersectionInfo._rHitCoeff	=rCoeff+rCoeffIntersection*rIntersectionStep;

				_vPointVectorIntersections.PushTail(IntersectionInfo);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindPlaneIntersection
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::FindPlaneIntersection(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect)
{
	// Check Plane Vectors Colinearity
	VECTOR	v1 =FirstVect;
	VECTOR	v2 =SecondVect;

	v1.Normalize();
	v2.Normalize();

	Float32	rDot =v1.Dot(v2);
	if	(MATH::Abs((MATH::Abs(rDot)-1.0f)) < MATH::EPSILON())
	{
		QDT_Error(QDT_COMMENT("Plane Vectors are colinear"));
		return;
	}

	// Clear vector
	_vPlaneIntersections.Clear();

	switch	(GetNbrPoints())
	{
		case 0:
		{
			break;
		}

		case 1:
		{
			PLANE	Plane(Position, Position+FirstVect, Position+SecondVect);
			if	(Plane.IsOn(GetPoint(0)))
			{
				INTERSECTION_INFO	IntersectionInfo;

				IntersectionInfo._HitPoint	=GetPoint(0);
				IntersectionInfo._rHitCoeff	=0.0f;

				_vPlaneIntersections.PushTail(IntersectionInfo);
			}
			break;
		}

		default:
		{
			FindPlaneIntersectionSpline(Position, FirstVect, SecondVect);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindPlaneIntersectionSpline
//	Object:		
//	03-01-23:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE::FindPlaneIntersectionSpline(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect)
{
	PLANE	Plane( Position, Position + FirstVect, Position + SecondVect );
	Float32	rIntersectionStep	=GetIntersectionStep();
	Float32	rCoeff				=-rIntersectionStep;

	while (MATH::Abs(rCoeff-(1.0f-rIntersectionStep)) > MATH::EPSILON())
	{
		// Increment Coeff
		rCoeff +=rIntersectionStep;

		VECTOR P1(GetInterpolatedPoint(rCoeff));
		VECTOR P2(GetInterpolatedPoint(rCoeff+rIntersectionStep));

		VECTOR	Dir;
		Dir.SetFromPoints(P1, P2);
		Dir.Normalize();

		RAY		Ray(P1, Dir);
		VECTOR	HitPoint;

		if (INTERSECTION::IntersectPlaneWithRay(Plane, Ray, HitPoint))
		{
			VECTOR	v1, v2;
			v1.Sub(HitPoint, P1);
			v2.Sub(P2, P1);

			Float32	rCoeffIntersection =v1.Dot( v2 ) / v2.SquareLength();

			if	((rCoeffIntersection>=0.0f) && (rCoeffIntersection<=1.0f))
			{
				INTERSECTION_INFO	IntersectionInfo;

				IntersectionInfo._HitPoint	=GetInterpolatedPoint(rCoeff+rCoeffIntersection*rIntersectionStep);
				IntersectionInfo._rHitCoeff	=rCoeff+rCoeffIntersection*rIntersectionStep;

				_vPlaneIntersections.PushTail(IntersectionInfo);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNearestPointCoeff
//	Object:		
//	03-09-18:	JJA - Created
//-----------------------------------------------------------------------------
VECTOR SPLINE::GetNearestPointCoeff(const VECTOR& SourcePoint, Float32& rNearestCoeff, const Int32 nSubdivisions) const
{
	Float32	rReason	=1.0f/(Float32)nSubdivisions;

	// Get First Point
	rNearestCoeff =0.0f;
	VECTOR	NearestPoint =GetInterpolatedPoint(0.0f);
	Float32	rMinDistance =DISTANCE::DistancePointPoint(SourcePoint, NearestPoint);

	Float32	rCoeff;
	for	(rCoeff=rReason; rCoeff<1.0f; rCoeff+=rReason)
	{
		VECTOR	SplinePoint =GetInterpolatedPoint(rCoeff);
		Float32	rDistance	=DISTANCE::DistancePointPoint(SourcePoint,SplinePoint);

		if	(rDistance<rMinDistance)
		{
			NearestPoint	=SplinePoint;
			rMinDistance	=rDistance;
			rNearestCoeff	=rCoeff;
		}
	}

	return NearestPoint;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
