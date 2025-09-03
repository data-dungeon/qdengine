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
//
//	04-03-15:	JJA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SPLINE_TEMPLATE constructor
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
SPLINE_TEMPLATE<VECTOR_T>::SPLINE_TEMPLATE()
:
_pReparametrizationAlong(NULL),
_pReparametrizationProjectionX(NULL)
{
	// Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		SPLINE_TEMPLATE destructor
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
SPLINE_TEMPLATE<VECTOR_T>::~SPLINE_TEMPLATE()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	03-09-18:	RMA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::Clear(void)
{
	_vHermitePoints.Clear();		//clear point list

	//delete ReparametrizationData
	if (_pReparametrizationAlong!=NULL)
	{
		delete _pReparametrizationAlong;
		_pReparametrizationAlong =NULL;
	}
	if (_pReparametrizationProjectionX!=NULL)
	{
		delete _pReparametrizationProjectionX;
		_pReparametrizationProjectionX =NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::AddPoint(const VECTOR_T& Point)
{
	HERMITE_POINT<VECTOR_T>	HermitePoint;

	HermitePoint._Point			=Point;
	HermitePoint._InTangent		=VECTOR_T::ZERO;
	HermitePoint._OutTangent	=VECTOR_T::ZERO;

	AddHermitePoint(HermitePoint);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPoints
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Int32 SPLINE_TEMPLATE<VECTOR_T>::GetNbrPoints(void) const
{
	return	_vHermitePoints.GetSize();
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPoints
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Int32 SPLINE_TEMPLATE<VECTOR_T>::GetNbrParts(void) const
{
	return GetNbrPoints()-1;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const VECTOR_T& SPLINE_TEMPLATE<VECTOR_T>::GetPoint(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return	_vHermitePoints[nIndex]._Point;
}

//-----------------------------------------------------------------------------
//	Name:		SetPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::SetPoint(const Int32 nIndex, const VECTOR_T& Value)
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	_vHermitePoints[nIndex]._Point=Value;
	CheckRebuildReparametrizationData();
}

//-----------------------------------------------------------------------------
//	Name:		GetInTangent
//	Object:		
//	04-11-19:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const VECTOR_T& SPLINE_TEMPLATE<VECTOR_T>::GetInTangent(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return _vHermitePoints[nIndex]._InTangent;
}

//-----------------------------------------------------------------------------
//	Name:		GetOutTangent
//	Object:		
//	04-11-19:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const VECTOR_T& SPLINE_TEMPLATE<VECTOR_T>::GetOutTangent(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return _vHermitePoints[nIndex]._OutTangent;
}

//-----------------------------------------------------------------------------
//	Name:		AddHermitePoint
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::AddHermitePoint(const HERMITE_POINT<VECTOR_T>& HermitePoint)
{
	_vHermitePoints.PushTail(HermitePoint);
	CheckRebuildReparametrizationData();
}

//-----------------------------------------------------------------------------
//	Name:		GetHermitePoint
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const HERMITE_POINT<VECTOR_T>& SPLINE_TEMPLATE<VECTOR_T>::GetHermitePoint(const Int32 nIndex) const
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<GetNbrPoints());
	return _vHermitePoints[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		GetTotalLength
//	Object:		
//	03-07-31:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Float32	SPLINE_TEMPLATE<VECTOR_T>::GetTotalLength(const REPARAMETRIZATION_DATA* pReparametrizationData) const
{
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));
	return	pReparametrizationData->_rTotalLength;
}

//-----------------------------------------------------------------------------
//	Name:		GetPartLength
//	Object:		
//	03-07-31:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Float32 SPLINE_TEMPLATE<VECTOR_T>::GetPartLength(const REPARAMETRIZATION_DATA* pReparametrizationData, const Int32 nPartIndex) const
{
	QDT_ASSERT(nPartIndex>=0);
	QDT_ASSERT(nPartIndex<GetNbrParts());
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));
	return pReparametrizationData->_vSplineInfo[nPartIndex]._rLength;
}

//-----------------------------------------------------------------------------
//	Name:		RetreiveIndex
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Int32 SPLINE_TEMPLATE<VECTOR_T>::RetreiveIndex(Float32 rCoeff) const
{
	Float32	rClampCoeff =MATH::Clamp(rCoeff, MATH::EPSILON(), 1.0f-MATH::EPSILON());
	return Int32(rClampCoeff*GetNbrParts());
}

//-----------------------------------------------------------------------------
//	Name:		GetSplineInfo
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const SPLINE_INFO& SPLINE_TEMPLATE<VECTOR_T>::GetSplineInfo(const REPARAMETRIZATION_DATA* pReparametrizationData, const Int32 nPartIndex) const
{
	QDT_ASSERT(nPartIndex>=0);
	QDT_ASSERT(nPartIndex<GetNbrParts());
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));
	return pReparametrizationData->_vSplineInfo[nPartIndex];
}

//-----------------------------------------------------------------------------
//	Name:		GetReparametrizationDataAlong
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const REPARAMETRIZATION_DATA* SPLINE_TEMPLATE<VECTOR_T>::GetReparametrizationDataAlong(void) const
{
	return _pReparametrizationAlong;
}

//-----------------------------------------------------------------------------
//	Name:		GetReparametrizationDataProjectionX
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
const REPARAMETRIZATION_DATA* SPLINE_TEMPLATE<VECTOR_T>::GetReparametrizationDataProjectionX(void) const
{
	return _pReparametrizationProjectionX;
}

//-----------------------------------------------------------------------------
//	Name:		ResetReparametrizationData
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::ResetReparametrizationData(REPARAMETRIZATION_DATA** ppReparametrizationData)
{
	if ((*ppReparametrizationData)!=NULL)
	{
		(*ppReparametrizationData)->Clear();
	}
	else
	{
		(*ppReparametrizationData) =new REPARAMETRIZATION_DATA;
	}
}

//-----------------------------------------------------------------------------
//	Name:		BuildReparametrizationDataAlong
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::BuildReparametrizationDataAlong(void)
{
	ResetReparametrizationData(&_pReparametrizationAlong);
	BuildReparametrizationData(_pReparametrizationAlong, MeasureAlong);
}

//-----------------------------------------------------------------------------
//	Name:		BuildReparametrizationDataProjectionX
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::BuildReparametrizationDataProjectionX(void)
{
	ResetReparametrizationData(&_pReparametrizationProjectionX);
	BuildReparametrizationData(_pReparametrizationProjectionX, MeasureProjectionX);
}

//-----------------------------------------------------------------------------
//	Name:		BuildReparametrizationData
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::BuildReparametrizationData(REPARAMETRIZATION_DATA* pReparametrizationData, MeasureFunc pMesureFunc)
{
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));
	QDT_ASSERT(pMesureFunc!=NULL);

	pReparametrizationData->_vSplineInfo.Clear();

	Int32 nSplineCount =GetNbrParts();

	if (nSplineCount<=0)	//no Spline = no ReparametrizationData
	{
		return;
	}

	pReparametrizationData->_vSplineInfo.Reserve(nSplineCount);			//since resize doesn't exist

	Float32 rCurGlobalLength =0.0f;

	Int32 iSpline;
	for(iSpline =0; iSpline<nSplineCount; ++iSpline)
	{
		const Int32 nSampleCount =SPLINE_NBSEGS*SPLINE_INVERSION_QUALITY;

		Float32	aSamples[nSampleCount];

		const VECTOR_T& vPosControl1 =GetPoint(iSpline);
		const VECTOR_T& vPosControl2 =GetPoint(iSpline+1);

		VECTOR_T vPosCur= vPosControl1;

		//build a fine table of value
		Float32 rCurSplineLength =0.0f;
		Int32 iSample;
		for(iSample =0; iSample<nSampleCount; ++iSample)
		{
			Float32		rHermiteCoeff	=Float32(iSample)/Float32(nSampleCount);
			VECTOR_T	vPosNext		=INTERPOLATION<VECTOR_T>::HermiteInterpolation(GetPoint(iSpline), GetPoint(iSpline+1), GetOutTangent(iSpline), GetInTangent(iSpline+1), rHermiteCoeff);
			Float32		rDistance		=pMesureFunc(vPosCur, vPosNext);
			aSamples[iSample] =rCurSplineLength+rDistance;
			rCurSplineLength  +=rDistance;
			vPosCur			  =vPosNext;
		}

		QDT_ASSERT_MSG(rCurSplineLength!=0.0f, ("Zero lenght Spline !"));

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
		pReparametrizationData->_vSplineInfo.PushTail(splInfo);
	}

	pReparametrizationData->_rTotalLength =rCurGlobalLength;
}

//-----------------------------------------------------------------------------
//	Name:		CheckRebuildReparametrizationData
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::CheckRebuildReparametrizationData(void)
{
	if (_pReparametrizationAlong!=NULL)
	{
		BuildReparametrizationDataAlong();
	}
	if (_pReparametrizationProjectionX!=NULL)
	{
		BuildReparametrizationDataProjectionX();
	}
}

//-----------------------------------------------------------------------------
//	Name:		CartesianRatioToSplineRatio
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Float32 SPLINE_TEMPLATE<VECTOR_T>::CartesianRatioToSplineRatio(const REPARAMETRIZATION_DATA* pReparametrizationData, const Float32 rRatio) const
{
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));
	QDT_ASSERT(rRatio>=0.0f);
	QDT_ASSERT(rRatio<=1.0f);

	const Float32 rFullLengthRatio =rRatio*pReparametrizationData->_rTotalLength;

	// seek to the good spline index
	Int32	iSpline =0;
	const Int32	nbSplines =pReparametrizationData->_vSplineInfo.GetSize();
	Float32	rCurrentDist =pReparametrizationData->_vSplineInfo[0]._rLength;
	while((iSpline<nbSplines) && (rCurrentDist<rFullLengthRatio))
	{
		++iSpline;
		rCurrentDist +=pReparametrizationData->_vSplineInfo[iSpline]._rLength;
	}

	//found spline ... now search for the good Hermite coeff.
	Float32 rAfter	=rCurrentDist;
	Float32 rBefore =rCurrentDist-pReparametrizationData->_vSplineInfo[iSpline]._rLength;
	Float32 rGap	=rAfter-rBefore;
	Float32 rGapRatio =(rFullLengthRatio-rBefore)/rGap;

	return PartRatioToSplineRatio(pReparametrizationData, iSpline, rGapRatio);
}

//-----------------------------------------------------------------------------
//	Name:		CartesianRatioToSplineRatio
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
Float32 SPLINE_TEMPLATE<VECTOR_T>::PartRatioToSplineRatio(const REPARAMETRIZATION_DATA* pReparametrizationData, const Int32 nPartIndex, const Float32 rPartRatio) const
{
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));
	QDT_ASSERT(rPartRatio>=0.0f);
	QDT_ASSERT(rPartRatio<=1.0f);
	QDT_ASSERT(nPartIndex>=0);
	QDT_ASSERT(nPartIndex<GetNbrParts());	//NbParts =NbPoints-1

    Float32 rLutIndex =rPartRatio*Float32(SPLINE_NBSEGS);
	Int32	nLutIndex =Int32(rLutIndex);

	//linear interpolation
	Float32 rIntervalRatio =rLutIndex-Float32(nLutIndex);
	Float32 rHermiteCoeff =MATH::Blend(pReparametrizationData->_vSplineInfo[nPartIndex]._arHermiteCoeffs[nLutIndex],pReparametrizationData->_vSplineInfo[nPartIndex]._arHermiteCoeffs[nLutIndex+1],rIntervalRatio);

	QDT_ASSERT(rHermiteCoeff>=0.0f);
	QDT_ASSERT(rHermiteCoeff<=1.0f);

	const Int32	nbSplines =pReparametrizationData->_vSplineInfo.GetSize();
	Float32 rSplineRatio =(Float32(nPartIndex)+rHermiteCoeff)/Float32(nbSplines);

	return rSplineRatio;
}

//-----------------------------------------------------------------------------
//	Name:		GetCartesianPosition
//	Object:		
//	06-01-17:	BFA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::GetCartesianPosition(const REPARAMETRIZATION_DATA* pReparametrizationData, Float32 rRatio, VECTOR_T* pPosition, VECTOR_T* pTangent) const
{
	QDT_ASSERT_MSG(pReparametrizationData!=NULL,("YOU MUST CALL BuildReparametrizationDataxxxx() before using this method !"));

	// Convert ratio
	Float32	rSplineRatio =CartesianRatioToSplineRatio(pReparametrizationData, rRatio);

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
//	Name:		AutoComputeTangents (see http://www.cubic.org/docs/hermite.htm)
//	Object:		
//	03-01-29:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
void SPLINE_TEMPLATE<VECTOR_T>::ComputeCardinalTangents(const Float32 rCardinalCoeff)
{
	if	(GetNbrPoints()>0)
	{
		// Fill tangents ...
		Int32	nPointIdx;
		for	(nPointIdx=1; nPointIdx<GetNbrPoints()-1; ++nPointIdx)
		{
			VECTOR_T	Tangent;
			Tangent.Sub(GetPoint(nPointIdx+1), GetPoint(nPointIdx-1));
			Tangent *=rCardinalCoeff;

			// Assign In And Out Tangent
			_vHermitePoints[nPointIdx]._InTangent	=Tangent;
			_vHermitePoints[nPointIdx]._OutTangent	=Tangent;
		}

		// Fill tangents (extremities)
		if	(GetHermitePoint(0)._Point==GetHermitePoint(GetNbrPoints()-1)._Point)
		{	// Closed spline
			VECTOR_T	Tangent;
			Tangent.Sub(GetPoint(1), GetPoint(GetNbrPoints()-2));
			Tangent *=rCardinalCoeff;

			_vHermitePoints[0]._OutTangent				 =Tangent;
			_vHermitePoints[GetNbrPoints()-1]._InTangent =Tangent;
		}
		else
		{
			// Open spline -> Zero Extremities Tangents
			_vHermitePoints[0]._OutTangent				 =VECTOR_T::ZERO;
			_vHermitePoints[GetNbrPoints()-1]._InTangent =VECTOR_T::ZERO;
		}
	}

	CheckRebuildReparametrizationData();
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolatedPoint
//	Object:		
//	02-12-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
VECTOR_T SPLINE_TEMPLATE<VECTOR_T>::GetInterpolatedPoint(const Float32 rCoeff) const
{
	switch(GetNbrPoints())
	{
	case 0:	// No Point
		return VECTOR_T::ZERO;
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
//	Name:		GetHermiteInterpolatedPoint
//	Object:		
//	03-01-28:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
VECTOR_T SPLINE_TEMPLATE<VECTOR_T>::GetHermiteInterpolatedPoint(const Float32 rCoeff) const
{
	QDT_ASSERT(rCoeff>=0.0f);
	QDT_ASSERT(rCoeff<=1.0f);

	Int32 nInter =RetreiveIndex(rCoeff);

	// Compute Hermite Coeff
	Float32	rSplineInterval=1.0f/Float32(GetNbrParts());
	Float32 rHermiteCoeff =(rCoeff-nInter*rSplineInterval) / rSplineInterval;

	rHermiteCoeff =MATH::Clamp(rHermiteCoeff, 0.0f, 1.0f);
	// Return Hermite Interpolation
	return INTERPOLATION<VECTOR_T>::HermiteInterpolation(GetPoint(nInter), GetPoint(nInter+1), GetOutTangent(nInter), GetInTangent(nInter+1), rHermiteCoeff);
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolatedTangent
//	Object:		
//	04-11-19:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
VECTOR_T SPLINE_TEMPLATE<VECTOR_T>::GetInterpolatedTangent(const Float32 rCoeff) const
{
	QDT_ASSERT(rCoeff>=0.0f);
	QDT_ASSERT(rCoeff<=1.0f);

	Int32 nInter =RetreiveIndex(rCoeff);

	// Compute Hermite Coeff
	Float32	rSplineInterval=1.0f/Float32(GetNbrParts());
	Float32 rHermiteCoeff =(rCoeff-nInter*rSplineInterval) / rSplineInterval;

	// Return Hermite Tangent
	return INTERPOLATION<VECTOR_T>::GetHermiteTangent(GetPoint(nInter), GetPoint(nInter+1), GetOutTangent(nInter), GetInTangent(nInter+1), rHermiteCoeff);
}


//-----------------------------------------------------------------------------
//	Name:		GetNearestPointCoeff
//	Object:		
//	03-09-18:	JJA - Created
//-----------------------------------------------------------------------------
template <class VECTOR_T>
VECTOR_T SPLINE_TEMPLATE<VECTOR_T>::GetNearestPointCoeff(const VECTOR_T& SourcePoint, Float32& rNearestCoeff, const Int32 nSubdivisions) const
{
	Float32	rReason	=1.0f/(Float32)nSubdivisions;

	// Get First Point
	rNearestCoeff =0.0f;
	VECTOR_T	NearestPoint =GetInterpolatedPoint(0.0f);
	Float32	rMinDistance =DISTANCE::DistancePointPoint(SourcePoint, NearestPoint);

	Float32	rCoeff;
	for	(rCoeff=rReason; rCoeff<1.0f; rCoeff+=rReason)
	{
		VECTOR_T	SplinePoint =GetInterpolatedPoint(rCoeff);
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
