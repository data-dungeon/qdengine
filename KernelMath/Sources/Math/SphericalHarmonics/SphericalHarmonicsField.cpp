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
#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonicsField)

#include	INCL_KMATH(Math/Geometry/Vector2/Vector2)
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/SphericalHarmonics/SHRotateMatrix)
#include    INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/SphericalHarmonics/CompressedSphericalHarmonics)
#include	INCL_KCORE(Stream/iostream)

//-----------------------------------------------------------------------------
//	Name:		SPHERICAL_HARMONICS_FIELD constructor
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
SPHERICAL_HARMONICS_FIELD::SPHERICAL_HARMONICS_FIELD()
{}

//-----------------------------------------------------------------------------
//	Name:		~SPHERICAL_HARMONICS_FIELD destructor
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
SPHERICAL_HARMONICS_FIELD::~SPHERICAL_HARMONICS_FIELD()
{

}


//-----------------------------------------------------------------------------
//	Name:		Initialize
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS_FIELD::Initialize(SPHERICAL_HARMONICS& centralSample, SPHERICAL_HARMONICS& negX, SPHERICAL_HARMONICS& posX,
							SPHERICAL_HARMONICS& negY, SPHERICAL_HARMONICS& posY, SPHERICAL_HARMONICS& negZ,
							SPHERICAL_HARMONICS& posZ, Float32 distance)
{

	Float32 maxValue = centralSample.GetHigherEigenValue();
	_valuePrecision = (char)MATH::Min(MATH::Ceil(maxValue), 255.0f);
	_shCentralSample.Compress(centralSample, _valuePrecision);

	
	SPHERICAL_HARMONICS gradX, gradY, gradZ;
	ComputeGradientComponent(negX, posX, gradX, distance);
	ComputeGradientComponent(negY, posY, gradY, distance);
	ComputeGradientComponent(negZ, posZ, gradZ, distance);

	maxValue = gradX.GetHigherEigenValue();
	maxValue = MATH::Max(maxValue, gradY.GetHigherEigenValue());
	maxValue = MATH::Max(maxValue, gradZ.GetHigherEigenValue());
	_gradientPrecision = (char)MATH::Min(MATH::Ceil(maxValue), 255.0f);

	_shGradientX.Compress(gradX, _gradientPrecision);
	_shGradientY.Compress(gradY, _gradientPrecision);
	_shGradientZ.Compress(gradZ, _gradientPrecision);


}


//-----------------------------------------------------------------------------
//	Name:		ComputeGradientComponent
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS_FIELD::ComputeGradientComponent(SPHERICAL_HARMONICS &	neg,
															SPHERICAL_HARMONICS &	pos,
															SPHERICAL_HARMONICS &	out,
															Float32 distance)
{

	Float32* negRed   = neg.GetRedComponents();
	Float32* negGreen = neg.GetGreenComponents();
	Float32* negBlue  = neg.GetBlueComponents();

	Float32* posRed   = pos.GetRedComponents();
	Float32* posGreen = pos.GetGreenComponents();
	Float32* posBlue  = pos.GetBlueComponents();

	Float32* outRed   = out.GetRedComponents();
	Float32* outGreen = out.GetGreenComponents();
	Float32* outBlue  = out.GetBlueComponents();

	for (UInt32 i = 0; i < 9; i++)
	{
		*(outRed++)   = (*(posRed++)   - *(negRed++)   ) / distance;
		*(outGreen++) = (*(posGreen++) - *(negGreen++) ) / distance;
		*(outBlue++)  = (*(posBlue++)  - *(negBlue++)  ) / distance;
	}
}



//-----------------------------------------------------------------------------
//	Name:		TaylorExpansion
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS_FIELD::TaylorExpansion(SPHERICAL_HARMONICS &	result,
												   VECTOR					delta)
{

	SPHERICAL_HARMONICS _shGradientX_dcmp,  _shGradientY_dcmp, _shGradientZ_dcmp, _shCentralSample_dcmp;
	
	_shGradientX.DeCompress(_shGradientX_dcmp, _gradientPrecision);
	Float32* gradXRed   = _shGradientX_dcmp.GetRedComponents();
	Float32* gradXGreen = _shGradientX_dcmp.GetGreenComponents();
	Float32* gradXBlue  = _shGradientX_dcmp.GetBlueComponents();

	_shGradientY.DeCompress(_shGradientY_dcmp, _gradientPrecision);
	Float32* gradYRed   = _shGradientY_dcmp.GetRedComponents();
	Float32* gradYGreen = _shGradientY_dcmp.GetGreenComponents();
	Float32* gradYBlue  = _shGradientY_dcmp.GetBlueComponents();

	_shGradientZ.DeCompress(_shGradientZ_dcmp, _gradientPrecision);
	Float32* gradZRed   = _shGradientZ_dcmp.GetRedComponents();
	Float32* gradZGreen = _shGradientZ_dcmp.GetGreenComponents();
	Float32* gradZBlue  = _shGradientZ_dcmp.GetBlueComponents();

	_shCentralSample.DeCompress(_shCentralSample_dcmp, _valuePrecision);
	Float32* sampleRed   = _shCentralSample_dcmp.GetRedComponents();
	Float32* sampleGreen = _shCentralSample_dcmp.GetGreenComponents();
	Float32* sampleBlue  = _shCentralSample_dcmp.GetBlueComponents();

	Float32* outRed   = result.GetRedComponents();
	Float32* outGreen = result.GetGreenComponents();
	Float32* outBlue  = result.GetBlueComponents();


	for (UInt32 i = 0; i < 9; i++)
	{								   // extend along X		    // extend along Y            // extend along Z
 		*outRed++   = (*sampleRed++)   + (*gradXRed++)   * delta[0] + (*gradYRed++)   * delta[1] + (*gradZRed++)   * delta[2];
		*outGreen++ = (*sampleGreen++) + (*gradXGreen++) * delta[0] + (*gradYGreen++) * delta[1] + (*gradZGreen++) * delta[2]; 
		*outBlue++  = (*sampleBlue++)  + (*gradXBlue++)  * delta[0] + (*gradYBlue++)  * delta[1] + (*gradZBlue++)  * delta[2]; 
	}

}


//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-03-15:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS_FIELD::Read(IO_STREAM &	s)
{
	s >> _valuePrecision;
	s >> _gradientPrecision;
	s >> _shCentralSample;
	s >> _shGradientX;
	s >> _shGradientY;
	s >> _shGradientZ;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	06-03-15:	PDE - Created
//-----------------------------------------------------------------------------
void	SPHERICAL_HARMONICS_FIELD::Write(IO_STREAM &	s) const
{
	s << _valuePrecision;
	s << _gradientPrecision;
	s << _shCentralSample;
	s << _shGradientX;
	s << _shGradientY;
	s << _shGradientZ;
}






//=============================================================================
//	CODE ENDS HERE
//=============================================================================
