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
//	CLASS:	PERLIN_NOISE_1D
//
//	06-01-23:	BFA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/NumericalAnalysis/PerlinNoise/PerlinNoise)
#include	INCL_KMATH(Math/Math)

#ifdef _DEBUG
#include	"PerlinNoise.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PERLIN_NOISE_1D constructor
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
PERLIN_NOISE_1D::PERLIN_NOISE_1D()
: _pTableNoise(NULL)
{
}

//-----------------------------------------------------------------------------
//	Name:		PERLIN_NOISE_1D destructor
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
PERLIN_NOISE_1D::~PERLIN_NOISE_1D()
{
	Kill();
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::Init(UInt32 nOctaves, Float32 rPersistence)
{
	Kill();
	QDT_ASSERT(nOctaves>=0);

	_rPersistence	=rPersistence;
	_nOctaves		=nOctaves;
	_nTableSize		=MATH::PowInt(2, nOctaves)+1;
	_pTableNoise	=new Float32[_nTableSize];
	_nSeed			=0;
	BuildTable(_nSeed);
}

//-----------------------------------------------------------------------------
//	Name:		Kill
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::Kill(void)
{
	if (_pTableNoise!=NULL)
	{
		delete[] _pTableNoise;
		_pTableNoise =NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetTheoricalMax
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
Float32	PERLIN_NOISE_1D::GetTheoricalMax(void)
{
	Float32 rTheoricalMax =0.0f;
	Float32 rAmplitude	  =1.0f;
	for (UInt32 nCurrentOctave =0; nCurrentOctave<=_nOctaves; nCurrentOctave++)
	{
		rTheoricalMax +=rAmplitude;
		rAmplitude    *=_rPersistence;
	}
	return rTheoricalMax;
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
Float32 PERLIN_NOISE_1D::GetValue(Float32 rPosition)
{
	UInt32 nSeed =(UInt32)rPosition;
	if (nSeed!=_nSeed)
	{
		BuildTable(nSeed);	//a seed change need new table build
	}

	return GetValueRatio(rPosition-nSeed);
}

//-----------------------------------------------------------------------------
//	Name:		GetValueRatio
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
Float32 PERLIN_NOISE_1D::GetValueRatio(Float32 rRatio)
{
	QDT_ASSERT(rRatio>=0.0f);
	QDT_ASSERT(rRatio<=1.0f);
	Float32 rIdx	 =rRatio*(_nTableSize-1);
	UInt32	nIdx	 =(UInt32)rIdx;
	Float32 rIdxDiff =rIdx-nIdx;

	return MATH::Blend(_pTableNoise[nIdx], _pTableNoise[nIdx+1], rIdxDiff);
}

//-----------------------------------------------------------------------------
//	Name:		GetInTable
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
Float32	PERLIN_NOISE_1D::GetInTable(UInt32 nIndex)
{
	QDT_ASSERT(nIndex>=0);
	QDT_ASSERT(nIndex<_nTableSize);
	return _pTableNoise[nIndex];
}

//-----------------------------------------------------------------------------
//	Name:		EraseTable
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::EraseTable(void)
{
	for (UInt32 i=0; i<_nTableSize; i++)
	{
		_pTableNoise[i] =0.0f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		BuildTable
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::BuildTable(UInt32 nSeed)
{
	EraseTable();

	_nSeed	=nSeed;
	Float32 rAmplitude =1.0f;
	for (UInt32 nCurrentOctave=0; nCurrentOctave<=_nOctaves; nCurrentOctave++)
	{
		DoPass(nCurrentOctave, rAmplitude);
		rAmplitude *=_rPersistence;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DoPass
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::DoPass(UInt32 nOctave, Float32 rAmplitude)
{
	UInt32 nCurves=MATH::PowInt(2, nOctave);

	Float32	rRatioIncrement =1.0f/MATH::PowInt(2, _nOctaves-nOctave);
	Float32	rRatio =0.0f;

	UInt32	nSeedOffset	=_nSeed*nCurves;
	Float32 r0;
	Float32 r1	=RawNoise(nSeedOffset);
	Float32 r2	=RawNoise(nSeedOffset+1);
	Float32 r3	=RawNoise(nSeedOffset+2);

	Float32* pVal =_pTableNoise;

	for (UInt32	iCurve=0; iCurve<nCurves; iCurve++)
	{
		r0	=r1;
		r1	=r2;
		r2	=r3;
		r3	=RawNoise(iCurve+nSeedOffset+3);

		rRatio =0.0f;
		do
		{
			*pVal +=CubicInterpolate(r0, r1, r2, r3, rRatio)*rAmplitude;

			rRatio +=rRatioIncrement;
			pVal++;
		} while(rRatio<1.0f);
	}
	*pVal +=r2*rAmplitude;
}

//-----------------------------------------------------------------------------
//	Name:		RawNoise
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
Float32 PERLIN_NOISE_1D::RawNoise(UInt32 nSeed)
{
	return	1.0f-MATH::QuickRand(nSeed)/1073741824.0f;
}

//-----------------------------------------------------------------------------
//	Name:		CubicInterpolate
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
Float32 PERLIN_NOISE_1D::CubicInterpolate(Float32 r0, Float32 r1, Float32 r2, Float32 r3, Float32 rX)
{
	Float32 rA	=(r0-r1);
	Float32 rP	=(r3-r2) - rA;

	Float32 rXSq =MATH::Square(rX);

	return rP*rXSq*rX + (rA-rP)*rXSq + (r2-r0)*rX + r1;
}

// oooooooooooooooooooooooooooooooooooooooooooooo Debug & Test oooooooooooooooooooooooooooooooooooooooooooooo


//-----------------------------------------------------------------------------
//	Name:		PrintTable
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::PrintTable(void)
{
	for (UInt32 i=0; i<_nTableSize; i++)
	{
		QDT_Message("%f", _pTableNoise[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		TestRawNoise
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::TestRawNoise(void)
{
	const UInt32 nIterations =1000000;

	Float32 rMean	=0.0f;
	Float32 rMin	=0.0f;
	Float32 rMax	=0.0f;

	for (UInt32 iter=0; iter<nIterations; iter++)
	{
		Float32 rVal =RawNoise(iter);

		if(rVal>rMax)
		{
			rMax =rVal;
		}
		else if(rVal<rMin)
		{
			rMin =rVal;
		}

		rMean +=rVal;
	}
	rMean /=nIterations;
	QDT_Message("");
	QDT_Message("******************");
	QDT_Message("* Raw Noise test *");
	QDT_Message("******************");
	QDT_Message("* Min  = %f", rMin);
	QDT_Message("* Max  = %f", rMax);
	QDT_Message("* Mean = %f", rMean);
	QDT_Message("******************");
}

//-----------------------------------------------------------------------------
//	Name:		TestPerlinNoise
//	Object:		
//	06-01-02:	BFA - Created
//-----------------------------------------------------------------------------
void PERLIN_NOISE_1D::TestPerlinNoise(void)
{
	const UInt32 nIterations =1000;

	Float32 rMean	=0.0f;
	Float32 rMin	=0.0f;
	Float32 rMax	=0.0f;

	for (UInt32 iter=0; iter<nIterations; iter++)
	{
		BuildTable(iter);	//ask for a fresh new table
		for (UInt32 idx=0; idx<_nTableSize; idx++)
		{
			Float32 rVal =GetInTable(idx);

			if(rVal>rMax)
			{
				rMax =rVal;
			}
			else if(rVal<rMin)
			{
				rMin =rVal;
			}

			rMean +=rVal;
		}
	}
	rMean /=nIterations;
	QDT_Message("");
	QDT_Message("**************************");
	QDT_Message("* Perlin Noise self-test *");
	QDT_Message("**************************");
	QDT_Message("* Persistence  = %f", _rPersistence);
	QDT_Message("* Octaves      = %d", _nOctaves);
	QDT_Message("* TheoricalMax = %f", GetTheoricalMax());
	QDT_Message("**************************");
	QDT_Message("* Min  = %f", rMin);
	QDT_Message("* Max  = %f", rMax);
	QDT_Message("* Mean = %f", rMean);
	QDT_Message("**************************");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
