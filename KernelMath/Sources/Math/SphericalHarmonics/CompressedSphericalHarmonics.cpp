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

#include	"CompressedSphericalHarmonics.h"

#define MAX_VAL     32767.0f   //(2^16 / 2 - 1)


void COMPRESSED_SPHERICAL_HARMONICS::Compress(SPHERICAL_HARMONICS& sh, Float32 ClampVal)
{
	float* red   = sh.GetRedComponents();
	float* green = sh.GetGreenComponents();
	float* blue  = sh.GetBlueComponents();

	for (UInt32 i = 0; i < 9; i++)
	{
		_redComponents[i]   = (Int16)(MATH::Min(MATH::Max(-ClampVal, red[i]), ClampVal) * (MAX_VAL / ClampVal));
		_greenComponents[i] = (Int16)(MATH::Min(MATH::Max(-ClampVal, green[i]), ClampVal) * (MAX_VAL / ClampVal));
		_blueComponents[i]  = (Int16)(MATH::Min(MATH::Max(-ClampVal, blue[i]), ClampVal) * (MAX_VAL / ClampVal));
	}
}

void COMPRESSED_SPHERICAL_HARMONICS::DeCompress(SPHERICAL_HARMONICS& sh, Float32 ClampVal)
{
	float* red   = sh.GetRedComponents();
	float* green = sh.GetGreenComponents();
	float* blue  = sh.GetBlueComponents();

	for (UInt32 i = 0; i < 9; i++)
	{
		red[i]   = (Float32)_redComponents[i]   * (ClampVal / MAX_VAL);
		green[i] = (Float32)_greenComponents[i] * (ClampVal / MAX_VAL); 
		blue[i]  = (Float32)_blueComponents[i]  * (ClampVal / MAX_VAL); 
	}
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	06-03-15:	PDE - Created
//-----------------------------------------------------------------------------
void	COMPRESSED_SPHERICAL_HARMONICS::Read(IO_STREAM &	s)
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
void	COMPRESSED_SPHERICAL_HARMONICS::Write(IO_STREAM &	s) const
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



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
