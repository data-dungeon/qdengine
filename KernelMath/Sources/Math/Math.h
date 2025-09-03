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
// CLASS: MATH
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: MATH
//00-11-09 JVA Created: 
//=============================================================================

#ifndef __KMATH_MATH_MATH_H__
#define __KMATH_MATH_MATH_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define		QDT_INCLUDE_STDLIB
#define		QDT_INCLUDE_MATH
#include	INCL_KCORE(Includes/Includes)

namespace QDT
{
	namespace KMATH
	{				
		#include QDT_DISABLE_FORCE_USE_MATH

		//very useful :) 
		//this is the construction of our world
		enum 
		{
			_X_ = 0,
			_Y_ = 1,
			_Z_ = 2,
			_W_ = 3,
		};

		#ifdef PLATFORM_PC

		enum _CPU_FEATURE
		{
			_CPU_FEATURE_NONE	= 0,
			_CPU_FEATURE_MMX	= 1 << 0,
			_CPU_FEATURE_SSE	= 1 << 1,
			_CPU_FEATURE_SSE2	= 1 << 2,
			_CPU_FEATURE_3DNOW  = 1 << 3,
		};

		#endif

		class MATH
		{
		public:

			static void			Init();

			// Constants
			static Float32		PI()					{ return (3.1415926535f); }				
			static Float32		TWO_PI()				{ return (6.2831853072f); }				
			static Float32		HALF_PI()				{ return (1.57079632675f); }
			static Float32		IRT2()					{ return (0.707106781186548f); }
			static Float32		SQRT2()					{ return (1.414213562373095f); }
			static Float32		EPSILON()				{ return (0.000001f); }

			// Trigonometry
			static Float32		Cos(Float32 rValue)							{ return (cosf(rValue)); }
			static Float32		Sin(Float32 rValue) 						{ return (sinf(rValue)); }

			static Float32		ACos(Float32 rValue)
								{
									QDT_ASSERT_MSG(rValue>=-1.0f && rValue<=1.0f, ("Value out of [-1..1] domain ! Before the call you may use MATH::Clamp(Value, -1.0f, 1.0f)"));
									return (acosf(rValue));
								}

			static Float32		ASin(Float32 rValue)
								{
									QDT_ASSERT_MSG(rValue>=-1.0f && rValue<=1.0f, ("Value out of [-1..1] domain ! Before the call you may use MATH::Clamp(Value, -1.0f, 1.0f)"));
									return (asinf(rValue));
								}

			static Int32		GetNearestInt(Float32 rValue)
			{
				return (Int32)(rValue + 0.5f);
			}

			static Float32		Tan(Float32 rValue)							{ return (tanf(rValue)); }
			static Float32		ATan(Float32 rValue)						{ return (atanf(rValue)); }
			static Float32		ATan(Float32 rY, Float32 rX)				{ return (atan2f(rY, rX)); }
			static Float32		DegreToRadian(Float32 rValue)				{ return (rValue * 0.01745329f); }
			static Float32		RadianToDegre(Float32 rValue)				{ return (rValue * 57.2957795f); }
			
			static Float32		Ln(Float32 rValue)							{ return (logf(rValue)) ; }
			static Float32		Log10(Float32 rValue)						{ return (log10f(rValue)); }
			static Float32		Log10(Int32 nValue)							{ return (log10(static_cast<Float32>(nValue))); }

			static Int32		FastLog2(Int32 n);
			static Float32		Exp(Float32 rValue)							{ return (expf(rValue)); }
			static Float32		Sqrt(Float32 rValue)						{ return (sqrtf(rValue)); }
			static Int32		Abs(Int32 nValue)							{ return (::abs(nValue)); }
#ifndef PLATFORM_PS3
			static Float32		Abs(Float32 rValue)							{ Int32 i=((*(Int32*)&rValue)&0x7fffffff);return (*(Float32*)&i);}
#else
			// Bug de compilo PS3
			static Float32		Abs(Float32 rValue)							{ return (fabsf(rValue));}
#endif
			static Float64		Abs(Float64 rValue)							{ return (fabs(rValue)); }
			static Float32		Floor(Float32 rValue)						{ return (floorf(rValue)); }
			static Float64		Floor(Float64 rValue)						{ return (floor(rValue)); }
			static Float32		Ceil(Float32 rValue)						{ return (ceilf(rValue)); }
			static Float32		Pow(Float32 rValue, Float32 rPower)			{ return (powf(rValue, rPower)); }
			static Float64		Pow(Float64 rValue, Float64 rPower)			{ return (pow(rValue, rPower)); }


			static UInt32		QuickRand(UInt32 nSeed)						{ UInt32 x =(nSeed<<13)^nSeed; return (x*(x*x*15731+789221)+1376312589)&0x7FFFFFFF; }
			static Int32		Rand()										{ return (rand()); }
			static Int32		Rand(Int32 nMaxValue)						
			{
				Int32 nResult = rand() % nMaxValue;
				QDT_ASSERT(nResult < nMaxValue);
				return (nResult);
			}

			static Float32		UnitRand(void)									{ return ((static_cast<Float32>(rand()) / RAND_MAX) ); }
			static Float32		Rand(Float32 rMaxValue)							{ return ((static_cast<Float32>(rand()) / RAND_MAX) * rMaxValue); }
			static void			SRand(UInt32 nSeed)								{ srand( nSeed ); }
			static Int32		IsNegative(Float32 rValue)						{ return (*(reinterpret_cast<Int32*>(&rValue)) & 0x80000000); }
			static Float32		InterLin(Float32 rT, Float32 rV1, Float32 rV2)	{ return (rV1 + rT * (rV2 - rV1)); }

			static bool			EqualsEpsilon(Float32 rValue1, Float32 rValue2, Float32 rEpsilon = 0.00001f)	
								{
									return ( MATH::Abs( rValue1 - rValue2 ) < rEpsilon );
								}

			// Template
			template <class C> static void	Swap(C & Value1, C & Value2)
											{
												C Temp;
												Temp = Value1;
												Value1 = Value2;
												Value2 = Temp;
											}

			template <class C> static void	Clip(C& ValueToClamp, C Min, C Max)
											{
													if (ValueToClamp < Min)
														ValueToClamp = Min;
													else if (ValueToClamp > Max)
														ValueToClamp = Max;
											}			
			template <class C> static void	ClipMin(C& ValueToClamp, C Min)
											{
													if (ValueToClamp < Min)
														ValueToClamp = Min;
											}

			template <class C> static void	ClipMax(C& ValueToClamp, C Max)
											{
													if (ValueToClamp > Max)
														ValueToClamp = Max;
											}

			template <class C> static C		Clamp(C ValueToClamp, C Min, C Max)
											{
													if (ValueToClamp < Min)		return (Min);
													if (ValueToClamp > Max)		return (Max);
													return (ValueToClamp);
											}

			template <class C> static Int32	Signum(C Value)
											{
												if (Value < 0) return (-1);
												if (Value > 0) return (1);
												return (0);
											}

			template <class C> static C		Square(C Value)
											{
												return (Value * Value);
											}

			template <class C> static C		Cube(C Value)
											{
												return (Value * Value * Value);
											}

			template <class C> static C		PowInt(C Value, Int32 nExp)
											{
												if (nExp < 0)		return 0;
												if (nExp == 0)		return 1;
			
												C Result = Value;

												Int32 i = 1;
												while (i < nExp)
												{
													++i;
													Result *= Value;
												}

												return (Result);
											}

			template <class C> static C		Max(C Value1, C Value2)
											{
												if (Value1 > Value2)	return (Value1);
												else					return (Value2);
											}

			template <class C> static C		Min(C Value1, C Value2)
											{
												if (Value1 < Value2)	return (Value1);
												else					return (Value2);
											}

			template <class C> static C		Max(Int32 nNbrValues, C *pValues)
											{
												QDT_ASSERT( nNbrValues > 0);

												C Max = pValues[0];

												for (Int32 i = 1; i < nNbrValues; i++)
												{
													if ( pValues[i] > Max )
													{
														Max = pValues[i];
													}
												}

												return ( Max );
											}

			template <class C> static C		Min(Int32 nNbrValues, C *pValues)
											{
												QDT_ASSERT( nNbrValues > 0);

												C Min = pValues[0];

												for (Int32 i = 1; i < nNbrValues; i++)
												{
													if ( pValues[i] < Min )
													{
														Min = pValues[i];
													}
												}

												return (Min);
											}	 

			template <class C> static C		Blend(C Source, C Destination, Float32 Ratio)
											{
												QDT_ASSERT( Ratio >= 0.0f);
												QDT_ASSERT( Ratio <= 1.0f);
												return (Source+(Destination-Source)*Ratio);
											}

			template <class C> static C		ChangeRange(C SourceRangeMin, C SourceRangeMax, C DestinationRangeMin, C DestinationRangeMax, Float32 Value)
											{
												C dx=SourceRangeMax-SourceRangeMin;
												C dy=DestinationRangeMax-DestinationRangeMin;
												C k=(Value-SourceRangeMin)/dx;
												return DestinationRangeMin+dy*k;
											}

			static Float64					GetRoundedFloat(const Float64 rValue, const Int32 precision = 2);

			static void						GetCosinusAndSinus(Float32 rAngle, Float32 & rCosResult, Float32 & rSinResult)
			{
				rCosResult = cosf(rAngle);
				rSinResult = sinf(rAngle);
			}

			static unsigned short			ConvertFloat32ToInt16(Float32 rValue, Float32	rRangeMin, Float32 rRangeMax);										  
			static Float32					ConvertInt16ToFloat32(unsigned short iValue, Float32	rRangeMin, Float32 rRangeMax);
			static unsigned short			ConvertFloat32ToInt16(Float32 rValue);										  
			static Float32					ConvertInt16ToFloat32(unsigned short iValue);

			static unsigned char			ConvertFloat32ToInt8(Float32 rValue, Float32	rRangeMin, Float32 rRangeMax);										  
			static Float32					ConvertInt8ToFloat32(unsigned char iValue, Float32	rRangeMin, Float32 rRangeMax);
			static unsigned char			ConvertFloat32ToInt8(Float32 rValue);										  
			static Float32					ConvertInt8ToFloat32(unsigned char iValue);

			static Int32					SwapEndian(Int32 iValue);

		private :

		// PC
		#ifdef PLATFORM_PC

			static void		InitProcessorSpecificInstructions();
			static Int32	GetCpuFeatures();

		#endif
		};

		#include QDT_FORCE_USE_MATH
	}
}

#endif //__KMATH_MATH_MATH_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
