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

#include	"Root.h"
#include	INCL_KMATH(Math/NumericalAnalysis/Splines/Spline3)

#ifdef _DEBUG
#include	"Spline3.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Ctor
//	Object:		
//	06-01-27:	BFA - Created
//-----------------------------------------------------------------------------
SPLINE_3::SPLINE_3()
:
SPLINE_TEMPLATE<VECTOR>(),
_rIntersectionStep(0.1f)
{
}

//-----------------------------------------------------------------------------
//	Name:		FindPointVectorIntersection
//	Object:		
//	02-12-19:	JJA - Created
//-----------------------------------------------------------------------------
void SPLINE_3::FindPointVectorIntersection(const VECTOR& Position, const VECTOR& Direction)
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
void SPLINE_3::FindPointVectorIntersectionLinear(const VECTOR& Position, const VECTOR& Direction)
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
void SPLINE_3::FindPointVectorIntersectionSpline(const VECTOR& Position, const VECTOR& Direction)
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
void SPLINE_3::FindPlaneIntersection(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect)
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
void SPLINE_3::FindPlaneIntersectionSpline(const VECTOR& Position, const VECTOR& FirstVect, const VECTOR& SecondVect)
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

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
