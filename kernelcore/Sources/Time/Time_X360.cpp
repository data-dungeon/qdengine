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
// CLASS: TIME_X360
//
//
//06 - 06 - 13 VMA Created:
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(Time/Time_X360)

#define		QDT_INCLUDE_X360_XTL
#define		QDT_INCLUDE_X360_TIME
#include	INCL_KCORE(Includes_X360/Includes_X360)

//-----------------------------------------------------------------------------
//	Name:		TIME_X360 constructor
//	Object:		
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
TIME_X360::TIME_X360()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)(&_nProcSpeed));
	QueryPerformanceCounter((LARGE_INTEGER*)(&_nTimeStartOffset));

	Init();
}

//-----------------------------------------------------------------------------
//	Name:		~TIME_X360 destructor
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
TIME_X360::~TIME_X360()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		This method is here to avoid a warning
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
TIME_X360 &	TIME_X360::operator=(const TIME_X360 &	TIME_X360)
{
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		GetSystemTime
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
Float64	TIME_X360::GetSystemTime() const
{
	Int64	nCurrentTime;
	Int64	nRelativeTime;
	
	QueryPerformanceCounter((LARGE_INTEGER*)(&nCurrentTime));
	nRelativeTime = nCurrentTime - _nTimeStartOffset;

	Float64 fSystemTime = 1000.0f * (Float64)nRelativeTime / (Float64)_nProcSpeed;

	return (fSystemTime);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSystemTime
//	Object:		
//	02-06-20:	CVI - Created
//-----------------------------------------------------------------------------
void	TIME_X360::UpdateSystemTime()
{
#if (!defined(_MASTER) && defined(PLATFORM_X360))
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
UInt64	TIME_X360::GetUniversalTime() const
{
	time_t Tmp;
	time(&Tmp);

	return	(static_cast<UInt64>(Tmp));
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
