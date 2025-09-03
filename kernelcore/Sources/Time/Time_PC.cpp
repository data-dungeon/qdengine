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
// CLASS: TIME_PC
//
//
//00 - 11 - 22 FKE Created:
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(Time/Time_PC)

#define		QDT_INCLUDE_PC_WINDOWS
#define		QDT_INCLUDE_PC_TIME
#include	INCL_KCORE(Includes_PC/Includes_PC)

#ifdef USE_RTDSC

void GetNbrTicks(LARGE_INTEGER & value)
{	
	unsigned int h,l;	
	__asm	
	{		
		RDTSC		
		mov [h],edx		
		mov [l],eax	
	}

	value.LowPart = l;
	value.HighPart = h;
}

void GetFrequency(LARGE_INTEGER & value) 
{		
	unsigned int time1,time2;	
	time1=GetTickCount();	
	time2=time1;
	LARGE_INTEGER t1, t2;
	GetNbrTicks(t1);	
	while(time2-time1<1000)	
	{		
		time2=GetTickCount();	
	}	
	GetNbrTicks(t2);
	value.QuadPart = t2.QuadPart -t1.QuadPart;
}

#endif

//-----------------------------------------------------------------------------
//	Name:		TIME_PC constructor
//	Object:		
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
TIME_PC::TIME_PC()
{
#ifdef USE_RTDSC
	GetFrequency(*((LARGE_INTEGER*)(&_nProcSpeed)));
	GetNbrTicks(*((LARGE_INTEGER*)(&_nTimeStartOffset)));
#else
	QueryPerformanceFrequency((LARGE_INTEGER*)(&_nProcSpeed));
	QueryPerformanceCounter((LARGE_INTEGER*)(&_nTimeStartOffset));
#endif

	Init();
}

//-----------------------------------------------------------------------------
//	Name:		~TIME_PC destructor
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
TIME_PC::~TIME_PC()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		This method is here to avoid a warning
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
TIME_PC &	TIME_PC::operator=(const TIME_PC &	TIME_PC)
{
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		GetSystemTime
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
Float64	TIME_PC::GetSystemTime() const
{
#ifdef USE_RTDSC
	__int64 nCurrentTime;
	__int64 nRelativeTime;

	GetNbrTicks(*((LARGE_INTEGER*)(&nCurrentTime)));
	nRelativeTime = nCurrentTime - _nTimeStartOffset;
	return ((float)(((double)nRelativeTime * 1000.0) / _nProcSpeed));
#else
	__int64	nCurrentTime;
	__int64	nRelativeTime;
	
	QueryPerformanceCounter((LARGE_INTEGER*)(&nCurrentTime));
	nRelativeTime = nCurrentTime - _nTimeStartOffset;

	Float64 fSystemTime = 1000.0f * (Float64)nRelativeTime / (Float64)_nProcSpeed;

	return (fSystemTime);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSystemTime
//	Object:		
//	02-06-20:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_PC::UpdateSystemTime()
{
#if (!defined(_MASTER) && defined(PLATFORM_PC))
	if( _rFrameRateLock > 0.0f )
	{
		_rFrameTime += _rFrameRateLock;
	}
	else
		_rFrameTime = (Float32)(GetSystemTime());
#else
	_rFrameTime = (Float32)(GetSystemTime());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetUniversalTime
//	Object:		
//	06-02-21:	CAU - Created
//-----------------------------------------------------------------------------
UInt64	TIME_PC::GetUniversalTime() const
{
	time_t Tmp;
	time(&Tmp);

	return	(static_cast<UInt64>(Tmp));
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
