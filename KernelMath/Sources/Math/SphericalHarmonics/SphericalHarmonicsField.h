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
//	The VECTOR_2 class implements ...
//
//	03-03-10:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_SPHERICAL_HARMONICS_FIELD_H__
#define __KMATH_MATH_SPHERICAL_HARMONICS_FIELD_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KMATH(SPHERICAL_HARMONICS_FIELD)

#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonics)
#include	INCL_KMATH(Math/SphericalHarmonics/CompressedSphericalHarmonics)
#include	INCL_KCORE(Stream/iostream)

namespace	QDT
{
	namespace KMATH
	{
		class	SPHERICAL_HARMONICS_FIELD
		{
		public:
			SPHERICAL_HARMONICS_FIELD();
			~SPHERICAL_HARMONICS_FIELD();

			void Initialize(SPHERICAL_HARMONICS& centralSample, SPHERICAL_HARMONICS& negX, SPHERICAL_HARMONICS& posX, 
							SPHERICAL_HARMONICS& negY, SPHERICAL_HARMONICS& posY, SPHERICAL_HARMONICS& negZ,
							SPHERICAL_HARMONICS& posZ, Float32 distance);


			void TaylorExpansion(SPHERICAL_HARMONICS& result, VECTOR delta);


			void GetSample(SPHERICAL_HARMONICS& result) {_shCentralSample.DeCompress(result, _valuePrecision);};

			// Streams
			void				Read(IO_STREAM &s);
			void				Write(IO_STREAM &s) const;

			friend IO_STREAM&	operator <<(IO_STREAM &s, const SPHERICAL_HARMONICS_FIELD &m)
			{
				m.Write(s);
				return s;
			}

			friend IO_STREAM&	operator >>(IO_STREAM &s, SPHERICAL_HARMONICS_FIELD &m)
			{
				m.Read(s);
				return s;
			}

		private:

			void ComputeGradientComponent(SPHERICAL_HARMONICS& neg, SPHERICAL_HARMONICS& pos, SPHERICAL_HARMONICS& out, Float32 distance);
		
			char _valuePrecision;
			char _gradientPrecision;
			COMPRESSED_SPHERICAL_HARMONICS _shCentralSample;
			COMPRESSED_SPHERICAL_HARMONICS _shGradientX;
			COMPRESSED_SPHERICAL_HARMONICS _shGradientY;
			COMPRESSED_SPHERICAL_HARMONICS _shGradientZ;
		};
	}
}

#endif //__SPHERICAL_HARMONICS_FIELD_H__
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
