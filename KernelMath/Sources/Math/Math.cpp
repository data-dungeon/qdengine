//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: MATH
//
//
//00 - 11 - 09 JVA Created:
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( MATH )
//
//	The MATH class implements ...
//
//*****************************************************************************
	
#include	"Root.h"
#include	INCL_KMATH(Math/Math)
#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: MATH::FastLog2
//	Object: 
//	01-06-13:	JLT - Created
//-----------------------------------------------------------------------------
int	MATH::FastLog2(int	n)
{
    int power = 0;
    
    if ((n < 2) || (n & 1)) return 0;    
    while(power <= 32) 
	{	
		n >>= 1;
		power++;
		if (n & 1) 
		{
			return (n > 1) ? 0 : power;
		}
    }
    return(0);
}

//-----------------------------------------------------------------------------
//	Name:		InitSeed
//	Object:		
//	02-08-09:	DAC - Created
//-----------------------------------------------------------------------------
void	MATH::Init()
{
	//call to init random seed generator
	SRand(static_cast<Int32>(TIME::Instance()->GetSystemTime()));

#ifdef PLATFORM_PC
	InitProcessorSpecificInstructions();
#endif
}

#ifdef PLATFORM_PC

//-----------------------------------------------------------------------------
//	Name:		OsSupport
//	Object:		
//	03-02-06:	RMA - Created
//-----------------------------------------------------------------------------
int MATH::GetCpuFeatures()
{
	int nFeature = _CPU_FEATURE_NONE;
	
	// These are the bit flags that get set on calling cpuid
	// with register eax set to 1
	const int _MMX_FEATURE_BIT	= 0x00800000;
	const int _SSE_FEATURE_BIT	= 0x02000000;
	const int _SSE2_FEATURE_BIT = 0x04000000;
	
	// This bit is set when cpuid is called with
	// register set to 80000001h (only applicable to AMD)
	const int _3DNOW_FEATURE_BIT = 0x80000000;

	int dwStandard = 0;
    int dwFeature = 0;
    int dwMax = 0;
    int dwExt = 0;

    union
    {
        char cBuf[12+1];
        struct
        {
            int	dw0;
            int dw1;
            int dw2;
        } 
		Array;
    } 
	Ident;

    _asm
    {
        push ebx
        push ecx
        push edx

        // get the vendor string
        xor eax,eax
        cpuid
        mov dwMax,eax
        mov Ident.Array.dw0,ebx
        mov Ident.Array.dw1,edx
        mov Ident.Array.dw2,ecx

        // get the Standard bits
        mov eax,1
        cpuid
        mov dwStandard,eax
        mov dwFeature,edx

        // get AMD-specials
        mov eax,80000000h
        cpuid
        cmp eax,80000000h
        jc notamd
        mov eax,80000001h
        cpuid
        mov dwExt,edx

notamd:
        pop ecx
        pop ebx
        pop edx
    }

    if (dwFeature & _MMX_FEATURE_BIT)
    {
        nFeature |= _CPU_FEATURE_MMX;
    }

    if (dwExt & _3DNOW_FEATURE_BIT)
    {
        nFeature |= _CPU_FEATURE_3DNOW;
    }

    if (dwFeature & _SSE_FEATURE_BIT)
    {
        nFeature |= _CPU_FEATURE_SSE;
    }

    if (dwFeature & _SSE2_FEATURE_BIT)
    {
        nFeature |= _CPU_FEATURE_SSE2;
    }    
	
	return (nFeature);
}

//-----------------------------------------------------------------------------
//	Name:		InitProcessorSpecificInstructions
//	Object:		
//	03-02-06:	RMA - Created
//-----------------------------------------------------------------------------
void	MATH::InitProcessorSpecificInstructions()
{
	int nFeature = GetCpuFeatures();

	if (nFeature & _CPU_FEATURE_MMX)
	{
		QDT_Message("MMX detected!");
	}

	if (nFeature & _CPU_FEATURE_SSE)
	{
		QDT_Message("SSE detected!");
//		MATRIX::PerformanceTest();
//		MATRIX::Init(_CPU_FEATURE_SSE);
//		MATRIX::NonRegression();
//		MATRIX::PerformanceTest();
	}
	else if (nFeature & _CPU_FEATURE_3DNOW)
	{
		QDT_Message("3DNow detected!");
//		MATRIX::Init(_CPU_FEATURE_3DNOW);
	}
}

#endif

//-----------------------------------------------------------------------------
//	Name:		GetRoundedFloat32
//	Object:		
//	02-03-04:	VPI - Created
//-----------------------------------------------------------------------------
double	MATH::GetRoundedFloat(const double	rValue,
							  const int		precision)
{
	double x, n, res;
	double fixpoint = MATH::Pow(10.0, precision);
	x = rValue * fixpoint;	//on passe en virgule fixe
	if (modf( x, &n ) > 0.5) //on recupere la partie entiere
	{
		n += 1;	//si la partie fracitonnaire est >0.5, on arrondit au dessus
	}
	
	res = n / fixpoint; //on recupere la virgule
	return (res);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertFloat32ToInt16
//	Object:		
//	03-04-07:	SBE - Created
//-----------------------------------------------------------------------------
unsigned short	MATH::ConvertFloat32ToInt16(Float32		rValue,
										  Float32		rRangeMin, 
										  Float32		rRangeMax)
{
	QDT_ASSERT((rValue > rRangeMin)||(EqualsEpsilon(rValue, rRangeMin)));
	QDT_ASSERT((rValue < rRangeMax)||(EqualsEpsilon(rValue, rRangeMax)));

	if ((rRangeMax - rRangeMin) <= 0.0f) return 0;

	return (unsigned short) (((rValue - rRangeMin) / (rRangeMax - rRangeMin)) * 65535.f);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertInt16ToFloat32
//	Object:		
//	03-04-07:	SBE - Created
//-----------------------------------------------------------------------------
Float32	MATH::ConvertInt16ToFloat32(unsigned short	iValue,
								  Float32				rRangeMin, 
								  Float32				rRangeMax)
{
	return ( ((Float32)iValue) / 65535.f) * (rRangeMax - rRangeMin) + rRangeMin;
}

//-----------------------------------------------------------------------------
//	Name:		ConvertFloat32ToInt16
//	Object:		
//	03-04-07:	SBE - Created
//-----------------------------------------------------------------------------
unsigned short	MATH::ConvertFloat32ToInt16(Float32		rValue)
{
	QDT_ASSERT((rValue > 0.0f)||(EqualsEpsilon(rValue, 0.0f)));
	QDT_ASSERT((rValue < 1.0f)||(EqualsEpsilon(rValue, 1.0f)));

	return (unsigned short) (rValue * 65535.f);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertInt16ToFloat32
//	Object:		
//	03-04-07:	SBE - Created
//-----------------------------------------------------------------------------
Float32	MATH::ConvertInt16ToFloat32(unsigned short	iValue)
{
	return ( ((Float32)iValue) / 65535.f);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertFloat32ToInt8
//	Object:		
//	03-04-10:	SBE - Created
//-----------------------------------------------------------------------------
unsigned char	MATH::ConvertFloat32ToInt8(Float32	rValue,
										 Float32	rRangeMin,
										 Float32	rRangeMax)
{
	QDT_ASSERT((rValue > rRangeMin)||(EqualsEpsilon(rValue, rRangeMin)));
	QDT_ASSERT((rValue < rRangeMax)||(EqualsEpsilon(rValue, rRangeMax)));

	if ((rRangeMax - rRangeMin) <= 0.0f) return 0;

	return (unsigned char) (((rValue - rRangeMin) / (rRangeMax - rRangeMin)) * 255.f);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertInt8ToFloat32
//	Object:		
//	03-04-10:	SBE - Created
//-----------------------------------------------------------------------------
Float32	MATH::ConvertInt8ToFloat32(unsigned char	iValue,
								   Float32			rRangeMin,
								   Float32			rRangeMax)
{
	return ( ((Float32)iValue) / 255.f) * (rRangeMax - rRangeMin) + rRangeMin;
}
//-----------------------------------------------------------------------------
//	Name:		ConvertFloat32ToInt8
//	Object:		
//	03-04-10:	SBE - Created
//-----------------------------------------------------------------------------
unsigned char	MATH::ConvertFloat32ToInt8(Float32	rValue)
{
	QDT_ASSERT((rValue > 0.0f)||(EqualsEpsilon(rValue, 0.0f)));
	QDT_ASSERT((rValue < 1.0f)||(EqualsEpsilon(rValue, 1.0f)));

	return (unsigned char) (rValue * 255.f);	
}

//-----------------------------------------------------------------------------
//	Name:		ConvertInt8ToFloat32
//	Object:		
//	03-04-10:	SBE - Created
//-----------------------------------------------------------------------------
Float32	MATH::ConvertInt8ToFloat32(unsigned char	iValue)
{
	return ( ((Float32)iValue) / 255.f);	
}

//-----------------------------------------------------------------------------
//	Name:		ConvertInt8ToFloat32
//	Object:		
//	03-05-06:	VMA - Created
//-----------------------------------------------------------------------------
int	MATH::SwapEndian(int word)
{
	int res;
	char *pword, *pres;
	pword = (char *)&word;
	pres  = (char *)&res;

	pres[0] = pword[3];
	pres[1] = pword[2];
	pres[2] = pword[1];
	pres[3] = pword[0];

	return(res);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
