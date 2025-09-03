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

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KMATH_MATH_NUMERICAL_ANALYSIS_PERLINNOISE_PERLINNOISE_H__
#define __KMATH_MATH_NUMERICAL_ANALYSIS_PERLINNOISE_PERLINNOISE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace	KMATH
	{
		//
		// PERLIN_NOISE_1D class :	(mostly inspired from http://freespace.virgin.net/hugo.elias/models/m_perlin.htm)
		// 
		// 1. Call Init()
		// if you want to call another time Init() to change settings you need to call Kill() first
		//
		// 2. You can Get a value using GetValue()
		//
		class PERLIN_NOISE_1D
		{
		public:
			PERLIN_NOISE_1D();
			~PERLIN_NOISE_1D();

			void	Init(UInt32 nOctaves, Float32 rPersistence =0.5f);	// Initializer (call first)
			void	Kill(void);											// Destroy

			void	BuildTable(UInt32 nSeed);							// Ask for a perlin noise table rebuild

			Float32	GetValue(Float32 rPosition);						// [0...UInt limit] with automatic table building if needed
			Float32	GetValueRatio(Float32 rRatio);						// [0...1] never table rebuild

			Float32	GetInTable(UInt32 nIndex);							// Direct table access [0..._nTableSize]
			UInt32	GetTableSize(void) const		{ return _nTableSize; }

			Float32	GetTheoricalMax(void);

			//Debug
			void	PrintTable(void);									// Debug purpose (tip: copy output content in M$-Excel and build a graphic)

			//Noise tests
			void	TestRawNoise(void);									// Test base noise (unfiltered)
			void	TestPerlinNoise(void);								// Test perlin

			Float32	GetPersistence(void) const		{ return _rPersistence; }
			UInt32	GetOctaves(void) const 			{ return _nOctaves; }

		protected:

			void	EraseTable(void);									// Fill table with 0.0f value
			void	DoPass(UInt32 nOctave, Float32 rAmplitude);			// Do a frequency pass (1 per octave + base frequency)

			Float32 RawNoise(UInt32 nSeed);								// Unfiltered Raw noise
			Float32 CubicInterpolate(Float32 r0, Float32 r1, Float32 r2, Float32 r3, Float32 rX);	// Do a Cubic interpolation using 4 points

			UInt32		_nOctaves;			// number of octaves used for table (0 will generate pure random)
			Float32		_rPersistence;		// power of octaves (0.1=smoother 0.5=normal 0.9=randomy)
			UInt32		_nTableSize;		// size = 2^oct + 1
			Float32*	_pTableNoise;		// computed table
			UInt32		_nSeed;				// seed=random offset
		};
	}
}

#ifndef _DEBUG
#include	"PerlinNoise.inl"
#endif

#endif //__KMATH_MATH_NUMERICAL_ANALYSIS_PERLINNOISE_PERLINNOISE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

