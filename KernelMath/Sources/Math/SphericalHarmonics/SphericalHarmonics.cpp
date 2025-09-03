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
//	CLASS:	SPHERICAL_HARMONICS
//
//	03-03-10:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonics)

#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)
#include	INCL_KMATH(Math/SphericalHarmonics/SHRotateMatrix)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#	include	"SphericalHarmonics.inl"
#endif


#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//-----------------------------------------------------------------------------
//	Name:		ComputeSHIrradEnvMapConstants
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::ComputeSHIrradEnvMapConstants(Float32 *	coefficients)
{
    float* fLight[3] = { _redComponents, _greenComponents, _blueComponents };

    // These constants are described in the article by Peter-Pike Sloan titled 
    // "Efficient Evaluation of Irradiance Environment Maps" in the book 
    // "ShaderX 2 - Shader Programming Tips and Tricks" by Wolfgang F. Engel.

	static const float s_fSqrtPI = ((float)MATH::Sqrt(MATH::PI()));
    const float fC0 = 1.0f/(2.0f*s_fSqrtPI);
    const float fC1 = (float)MATH::Sqrt(3.0f) /(3.0f  * s_fSqrtPI);
    const float fC2 = (float)MATH::Sqrt(15.0f)/(8.0f  * s_fSqrtPI);
    const float fC3 = (float)MATH::Sqrt(5.0f) /(16.0f * s_fSqrtPI);
    const float fC4 = 0.5f*fC2;

    int iChannel;
	Float32* pCoeff = coefficients;

    for( iChannel=0; iChannel<3; iChannel++ )
    {
        *(pCoeff++) = -fC1*fLight[iChannel][3];
        *(pCoeff++) = -fC1*fLight[iChannel][1];
        *(pCoeff++) =  fC1*fLight[iChannel][2];
        *(pCoeff++) =  fC0*fLight[iChannel][0] - fC3*fLight[iChannel][6];
    }

    for( iChannel=0; iChannel<3; iChannel++ )
    {
        *(pCoeff++) =      fC2*fLight[iChannel][4];
        *(pCoeff++) =     -fC2*fLight[iChannel][5];
        *(pCoeff++) = 3.0f*fC3*fLight[iChannel][6];
        *(pCoeff++) =     -fC2*fLight[iChannel][7];
    }

    *(pCoeff++) = fC4*fLight[0][8];
    *(pCoeff++) = fC4*fLight[1][8];
    *(pCoeff++) = fC4*fLight[2][8];
    *(pCoeff++) = 1.0f;

    //V( m_pSHIrradEnvMapEffect->SetVector( "cC", &vCoefficients[0] ) );
}



//-----------------------------------------------------------------------------
//	Name:		Distance
//	Object:		
//	06-03-02:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SPHERICAL_HARMONICS::Distance(SPHERICAL_HARMONICS &	shReference)
{
	float MagRed   = 0.0f;
	float MagGreen = 0.0f;
	float MagBlue  = 0.0f;

	for (UInt32 i = 0; i < 9; i++)
	{
		float DeltaRed   = _redComponents[i] - shReference._redComponents[i];
		float DeltaGreen = _greenComponents[i] - shReference._greenComponents[i];
		float DeltaBlue  = _blueComponents[i] - shReference._blueComponents[i];
		MagRed   += DeltaRed * DeltaRed;
		MagGreen += DeltaGreen * DeltaGreen;
		MagBlue  += DeltaBlue * DeltaBlue;
	}
	return (MATH::Sqrt(MagRed) + MATH::Sqrt(MagGreen) +  MATH::Sqrt(MagBlue)) / 3.0f;
}

//-----------------------------------------------------------------------------
//	Name:		Magnitude
//	Object:		
//	06-03-02:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SPHERICAL_HARMONICS::Magnitude()
{	
	float MagRed   = 0.0f;
	float MagGreen = 0.0f;
	float MagBlue  = 0.0f;

	for (UInt32 i = 0; i < 9; i++)
	{
		MagRed   += _redComponents[i] * _redComponents[i];
		MagGreen += _redComponents[i] * _redComponents[i];
		MagBlue  += _redComponents[i] * _redComponents[i];
	}

	return (MATH::Sqrt(MagRed) + MATH::Sqrt(MagGreen) +  MATH::Sqrt(MagBlue)) / 3.0f;
}


//-----------------------------------------------------------------------------
//	Name:		Lerp
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::Lerp(SPHERICAL_HARMONICS &	a,
								  SPHERICAL_HARMONICS &	b,
								  Float32				ratio)
{
	Float32* AR = a.GetRedComponents();
	Float32* AG = a.GetGreenComponents();
	Float32* AB = a.GetBlueComponents();
	Float32* BR = b.GetRedComponents();
	Float32* BG = b.GetGreenComponents();
	Float32* BB = b.GetBlueComponents();

	for (UInt32 i = 0; i < 9; i++)
	{
		_redComponents[i]   = AR[i] + ratio * (BR[i] - AR[i]);
		_greenComponents[i] = AG[i] + ratio * (BG[i] - AG[i]);
		_blueComponents[i]  = AB[i] + ratio * (BB[i] - AB[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitNormal
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::InitNormal()
{
	// Match a normalization cube map
	_redComponents[0] = 1.7724000f;
	_redComponents[0] = 1.7724000f;
	_redComponents[1] = -5.2646087e-006f;
	_redComponents[2] = -1.5403208e-008f;
	_redComponents[3] = -1.0232921f;
	_redComponents[4] = 1.2637692e-007f;
	_redComponents[5] = -1.6827795e-005f;
	_redComponents[6] = -2.1935966e-006f;
	_redComponents[7] = 2.5332891e-005f;
	_redComponents[8] = 1.0925678e-006f;
	
	_greenComponents[0]	= 1.7723593f;
	_greenComponents[1]	= -1.0233142f;
	_greenComponents[2]	= 1.2177117e-006f;
	_greenComponents[3]	= -1.8768770e-006f;
	_greenComponents[4]	= 6.7036956e-007f;
	_greenComponents[5]	= 9.2068276e-006f;
	_greenComponents[6]	= -2.8455022e-006f;
	_greenComponents[7]	= -1.5244531e-005f;
	_greenComponents[8]	= 5.3491217e-006f;

	_blueComponents[0] = 1.7723459f;
	_blueComponents[1] = -5.8848318e-006f;
	_blueComponents[2] = 1.0232980f;
	_blueComponents[3] = 5.6479344e-006f;
	_blueComponents[4] = -2.0962487e-005f;
	_blueComponents[5] = 7.6746046e-006f;
	_blueComponents[6] = 8.0136706e-006f;
	_blueComponents[7] = -2.5431247e-005f;
	_blueComponents[8] = 1.2430805e-006f;
}

//-----------------------------------------------------------------------------
//	Name:		InitNormal
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::Init()
{
	memset(this, 0, sizeof(SPHERICAL_HARMONICS));
}


//-----------------------------------------------------------------------------
//	Name:		AddLight
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::AddSoftLight(const VECTOR& direction, Float32 r, Float32 g, Float32 b)
{
	static const Float32 piExp = MATH::Sqrt(3.0f) * MATH::Pow(MATH::PI(), 3.0f / 2.0f);

	Float32 SignumDz = (direction[2] > 0.0f)? +1.0f : -1.0f;

	/*
	Float32 c1 = (0.25f) * piExp * direction[1] * SignumDz;
	Float32 c2 = (0.50f) * piExp * direction[2] * SignumDz;
	Float32 c3 = (0.25f) * piExp * direction[0] * SignumDz;
	*/

	Float32 c0 = 0.5f * 2.784163999f;
	Float32 c1 = 0.5f * 1.205578376f * direction[1];
	Float32 c2 = 0.5f * 2.411156751f * direction[2];
	Float32 c3 = 0.5f * 1.205578376f * direction[0];
	Float32 c6 = 0.5f * 1.556394990f;

	_redComponents[0] += c0 * r;
	_redComponents[1] += c1 * r;
	_redComponents[2] += c2 * r;
	_redComponents[3] += c3 * r;
	//_redComponents[6] += c6 * r;

	_greenComponents[0] += c0 * g;
	_greenComponents[1] += c1 * g;
	_greenComponents[2] += c2 * g;
	_greenComponents[3] += c3 * g;
	//_greenComponents[6] += c6 * g;

	_blueComponents[0] += c0 * b;
	_blueComponents[1] += c1 * b;
	_blueComponents[2] += c2 * b;
	_blueComponents[3] += c3 * b;
	//_blueComponents[6] += c6 * b;
}


//-----------------------------------------------------------------------------
//	Name:		AddLightSquared
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::AddLight(const VECTOR &	direction,
									 Float32		r,
									 Float32		g,
									 Float32		b)
{
	Float32 signum = 1 + (direction[2] > 0? 1.0f: -1.0f);
	Float32 c1 = 1.2057f * direction[1];// * signum;
	Float32 c2 = 2.4114f * direction[2];// * signum;
	Float32 c3 = 1.2057f * direction[0];// * signum;
	
	_redComponents[1] += c1 * r;
	_redComponents[2] += c2 * r;
	_redComponents[3] += c3 * r;

	_greenComponents[1] += c1 * g;
	_greenComponents[2] += c2 * g;
	_greenComponents[3] += c3 * g;

	_blueComponents[1] += c1 * b;
	_blueComponents[2] += c2 * b;
	_blueComponents[3] += c3 * b;
}


//-----------------------------------------------------------------------------
//	Name:		AddConstant
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::AddConstant(Float32	r,
										 Float32	g,
										 Float32	b)
{
	//static const Float32 piExp = MATH::Pow(MATH::PI(), 1.5f);
	static const Float32 Constant = 2 * MATH::Sqrt(MATH::PI());

	_redComponents[0] += Constant * r;
	_greenComponents[0] += Constant * g;
	_blueComponents[0] += Constant * b;
}

//-----------------------------------------------------------------------------
//	Name:		AddHemisphere
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::AddHemisphere(Float32 upRed, Float32 upGreen, Float32 upBlue,
  								   Float32 downRed, Float32 downGreen, Float32 downBlue)
{

                   
	_redComponents[0] +=   2.784163999f * downRed   + 2.784163999f * upRed;
	_greenComponents[0] += 2.784163999f * downGreen + 2.784163999f * upGreen;
	_blueComponents[0] +=  2.784163999f * downBlue  + 2.784163999f * upBlue;

	_redComponents[1] +=   1.205578376f * downRed   - 1.205578376f * upRed;
	_greenComponents[1] += 1.205578376f * downGreen - 1.205578376f * upGreen; 
	_blueComponents[1] +=  1.205578376f * downBlue  - 1.205578376f * upBlue; 

}

//-----------------------------------------------------------------------------
//	Name:		AddHorizon
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::AddHorizon(Float32	Red,
										Float32	Green,
										Float32	Blue)
{
	static const Float32 c0 = 1.0548f * 0.3f;
	static const Float32 c6 = 3.1130f * 0.3f;
	static const Float32 c8 = 1.9422f * 0.3f;

	_redComponents[0] +=   c0 * Red;
	_greenComponents[0] += c0 * Green; 
	_blueComponents[0] +=  c0 * Blue; 

	_redComponents[6] +=   c6 * Red;
	_greenComponents[6] += c6 * Green; 
	_blueComponents[6] +=  c6 * Blue; 

	_redComponents[8] +=   c8 * Red;
	_greenComponents[8] += c8 * Green; 
	_blueComponents[8] +=  c8 * Blue; 

}


//-----------------------------------------------------------------------------
//	Name:		MultiplyColor
//	Object:		
//	06-03-08:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::MultiplyColor(Float32	r,
										   Float32	g,
										   Float32	b)
{
	for (UInt32 i = 0; i < 9; i++)
	{
		_redComponents[i] *= r;
		_greenComponents[i] *= g;
		_blueComponents[i] *= b;
	}
}


//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-03-15:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::Read(IO_STREAM &	s)
{
	for (UInt32 r = 0; r < 9; r++)
	{
		s >> _redComponents[r];
	}

	for (UInt32 g = 0; g < 9; g++)
	{
		s >> _greenComponents[g];
	}

	for (UInt32 b = 0; b < 9; b++)
	{
		s >> _blueComponents[b];
	}
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-03-15:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::Write(IO_STREAM &	s) const
{
	for (UInt32 r = 0; r < 9; r++)
	{
		s << _redComponents[r];
	}

	for (UInt32 g = 0; g < 9; g++)
	{
		s << _greenComponents[g];
	}

	for (UInt32 b = 0; b < 9; b++)
	{
		s << _blueComponents[b];
	}
}


//-----------------------------------------------------------------------------
//	Name:		GetHigherEigenValue
//	Object:		
//	06-03-16:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SPHERICAL_HARMONICS::GetHigherEigenValue()
{
	Float32 val = -1;

	for (UInt32 i = 0; i < 9; i++)
	{
		if (val < MATH::Abs(_redComponents[i])) val   = MATH::Abs(_redComponents[i]);
		if (val < MATH::Abs(_greenComponents[i])) val = MATH::Abs(_greenComponents[i]);
		if (val < MATH::Abs(_blueComponents[i])) val  = MATH::Abs(_blueComponents[i]);
	}

	return val;
}

//-----------------------------------------------------------------------------
//	Name:		Multiply
//	Object:		
//	06-03-16:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::Multiply(const SPHERICAL_HARMONICS &	a,
									  const SPHERICAL_HARMONICS &	b)
{
	Multiply(_redComponents, a._redComponents, b._redComponents);
	Multiply(_greenComponents, a._greenComponents, b._greenComponents);
	Multiply(_blueComponents, a._blueComponents, b._blueComponents);
}

//-----------------------------------------------------------------------------
//	Name:		Multiply
//	Object:		
//	06-03-16:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::MultiplySimple(const SPHERICAL_HARMONICS &	a,
										    const SPHERICAL_HARMONICS &	b)
{
	MultiplySimple(_redComponents, a._redComponents, b._redComponents);
	MultiplySimple(_greenComponents, a._greenComponents, b._greenComponents);
	MultiplySimple(_blueComponents, a._blueComponents, b._blueComponents);
}


//-----------------------------------------------------------------------------
//	Name:		Multiply
//	Object:		
//	06-03-16:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::Multiply(Float32 *			res,
									  const Float32 *	e,
									  const Float32 *	g)
{
	res[0] =  1.7725f  * e[0] * g[0] + 0.76163f * e[6] * g[6] + 0.66468f * e[2] * g[2] + 0.49542f * e[0] * g[6]
			+ 0.49542f * e[6] * g[0] + 0.31157f * e[4] * g[4] + 0.41542f * e[7] * g[7] + 0.33234f * e[3] * g[3]
			+ 0.41542f * e[5] * g[5] + 1.3294f  * e[1] * g[1] + 0.31157f * e[8] * g[8];


	res[1] =  1.3294f  * e[1] * g[0] + 1.3294f  * e[0] * g[1] - 0.18579f * e[6] * g[1] - 0.18579f * e[1] * g[6]
			+ 0.24135f * e[4] * g[3] + 0.32180f * e[2] * g[5] - 0.48272f * e[1] * g[8] + 0.24135f * e[3] * g[4]
			- 0.48272f * e[8] * g[1] + 0.32180f * e[5] * g[2];

	res[2] =  0.64359f * e[1] * g[5] + 0.92892f * e[6] * g[2] + 0.92892f * e[2] * g[6] + 0.32180f * e[3] * g[7]
			+ 0.32180f * e[7] * g[3] + 0.64359f * e[5] * g[1] + 1.3294f  * e[2] * g[0] + 1.3294f  * e[0] * g[2];


	res[3] = -0.092897f * e[6] * g[3] -0.092897f * e[3] * g[6] + 0.32180f * e[2] * g[7] + 0.66470f * e[3] * g[0]
		    + 0.24135f  * e[8] * g[3] + 0.24135f * e[3] * g[8] + 0.32180f * e[7] * g[2] + 0.48272f * e[1] * g[4]
			+ 0.48272f  * e[4] * g[1] + 0.66470f * e[0] * g[3];

	res[4] =  0.48270f  * e[3] * g[1] + 0.20113f * e[7] * g[5] - 0.17418f * e[6] * g[4] + 0.48270f * e[1] * g[3]
			+ 0.20113f  * e[5] * g[7] + 0.62315f * e[4] * g[0] - 0.17418f * e[4] * g[6] + 0.62315f * e[0] * g[4];


	res[5] =  0.23224f  * e[5] * g[6] + 0.23224f * e[6] * g[5] - 0.20113f * e[8] * g[5] + 0.64360f * e[1] * g[2]
	        + 0.20113f  * e[4] * g[7] - 0.20113f * e[5] * g[8] + 0.20113f * e[7] * g[4] + 0.64360f * e[2] * g[1]
			+ 0.83086f  * e[5] * g[0] + 0.83086f * e[0] * g[5];

	res[6] =  0.99082f * e[0] * g[0] + 1.5232f  * e[0] * g[6] + 1.5232f  * e[6] * g[0] - 0.092893f * e[3] * g[3]
			+ 0.92893f * e[2] * g[2] + 0.23225f * e[7] * g[7] - 0.37158f * e[1] * g[1] + 1.1225f   * e[6] * g[6]
			- 0.17417f * e[4] * g[4] - 0.17417f * e[8] * g[8] + 0.23225f * e[5] * g[5];

	res[7] =  0.23224f * e[6] * g[7] + 0.23224f * e[7] * g[6] + 0.32180f * e[3] * g[2] + 0.20113f * e[5] * g[4]
            + 0.20113f * e[7] * g[8] + 0.20113f * e[8] * g[7] + 0.32180f * e[2] * g[3] + 0.20113f * e[4] * g[5]
			+ 0.83086f * e[0] * g[7] + 0.83086f * e[7] * g[0];


	res[8] =  0.62315f * e[8] * g[0] + 0.62315f * e[0] * g[8] - 0.17418f * e[6] * g[8] - 0.17418f * e[8] * g[6]
            - 0.96542f * e[1] * g[1] - 0.20113f * e[5] * g[5] + 0.20113f * e[7] * g[7] + 0.24135f * e[3] * g[3];

}



//-----------------------------------------------------------------------------
//	Name:		Multiply
//	Object:		
//	06-03-16:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS::MultiplySimple(Float32 *			res,
											const Float32 *	e,
											const Float32 *	g)
{
	QDT_ASSERT(e[4] == 0.0f);
	QDT_ASSERT(e[5] == 0.0f);
	QDT_ASSERT(e[6] == 0.0f);
	QDT_ASSERT(e[7] == 0.0f);
	QDT_ASSERT(e[8] == 0.0f);

	res[0] =  1.7725f  * e[0] * g[0] + 0.76163f * e[6] * g[6] + 0.66468f * e[2] * g[2] + 0.49542f * e[0] * g[6]
			+ 0.49542f * e[6] * g[0] + 0.31157f * e[4] * g[4] + 0.41542f * e[7] * g[7] + 0.33234f * e[3] * g[3]
			+ 0.41542f * e[5] * g[5] + 1.3294f  * e[1] * g[1] + 0.31157f * e[8] * g[8];


	res[1] =  1.3294f  * e[1] * g[0] + 1.3294f  * e[0] * g[1] - 0.18579f * e[6] * g[1] - 0.18579f * e[1] * g[6]
			+ 0.24135f * e[4] * g[3] + 0.32180f * e[2] * g[5] - 0.48272f * e[1] * g[8] + 0.24135f * e[3] * g[4]
			- 0.48272f * e[8] * g[1] + 0.32180f * e[5] * g[2];

	res[2] =  0.64359f * e[1] * g[5] + 0.92892f * e[6] * g[2] + 0.92892f * e[2] * g[6] + 0.32180f * e[3] * g[7]
			+ 0.32180f * e[7] * g[3] + 0.64359f * e[5] * g[1] + 1.3294f  * e[2] * g[0] + 1.3294f  * e[0] * g[2];


	res[3] = -0.092897f * e[6] * g[3] -0.092897f * e[3] * g[6] + 0.32180f * e[2] * g[7] + 0.66470f * e[3] * g[0]
		    + 0.24135f  * e[8] * g[3] + 0.24135f * e[3] * g[8] + 0.32180f * e[7] * g[2] + 0.48272f * e[1] * g[4]
			+ 0.48272f  * e[4] * g[1] + 0.66470f * e[0] * g[3];

	res[4] =  0.48270f  * e[3] * g[1] + 0.20113f * e[7] * g[5] - 0.17418f * e[6] * g[4] + 0.48270f * e[1] * g[3]
			+ 0.20113f  * e[5] * g[7] + 0.62315f * e[4] * g[0] - 0.17418f * e[4] * g[6] + 0.62315f * e[0] * g[4];


	res[5] =  0.23224f  * e[5] * g[6] + 0.23224f * e[6] * g[5] - 0.20113f * e[8] * g[5] + 0.64360f * e[1] * g[2]
	        + 0.20113f  * e[4] * g[7] - 0.20113f * e[5] * g[8] + 0.20113f * e[7] * g[4] + 0.64360f * e[2] * g[1]
			+ 0.83086f  * e[5] * g[0] + 0.83086f * e[0] * g[5];

	res[6] =  0.99082f * e[0] * g[0] + 1.5232f  * e[0] * g[6] + 1.5232f  * e[6] * g[0] - 0.092893f * e[3] * g[3]
			+ 0.92893f * e[2] * g[2] + 0.23225f * e[7] * g[7] - 0.37158f * e[1] * g[1] + 1.1225f   * e[6] * g[6]
			- 0.17417f * e[4] * g[4] - 0.17417f * e[8] * g[8] + 0.23225f * e[5] * g[5];

	res[7] =  0.23224f * e[6] * g[7] + 0.23224f * e[7] * g[6] + 0.32180f * e[3] * g[2] + 0.20113f * e[5] * g[4]
            + 0.20113f * e[7] * g[8] + 0.20113f * e[8] * g[7] + 0.32180f * e[2] * g[3] + 0.20113f * e[4] * g[5]
			+ 0.83086f * e[0] * g[7] + 0.83086f * e[7] * g[0];


	res[8] =  0.62315f * e[8] * g[0] + 0.62315f * e[0] * g[8] - 0.17418f * e[6] * g[8] - 0.17418f * e[8] * g[6]
            - 0.96542f * e[1] * g[1] - 0.20113f * e[5] * g[5] + 0.20113f * e[7] * g[7] + 0.24135f * e[3] * g[3];
	
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
