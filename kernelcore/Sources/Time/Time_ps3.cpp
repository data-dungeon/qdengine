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
// CLASS: TIME_PS2
//
//
//00 - 11 - 22 FKE Created:
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( TIME_PS2 )
//
//	The TIME_PS2 class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Time/Time_PS3)

#include <sys/sys_time.h>
#include <sys/time_util.h>
//#include <time.h>

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TIME_PS3 constructor
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
TIME_PS3::TIME_PS3()
{
	_fTimeBase = float( sys_time_get_timebase_frequency() / 1000 );
}

//-----------------------------------------------------------------------------
//	Name:		~TIME_PS3 destructor
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
TIME_PS3::~TIME_PS3()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetSystemTime
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
Float64	TIME_PS3::GetSystemTime() const
{
	UInt64 nTime;
	SYS_TIMEBASE_GET(nTime);
	return ( Float64(nTime) /  _fTimeBase );
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSystemTime
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
void	TIME_PS3::UpdateSystemTime()
{
	_rFrameTime = GetSystemTime();
}

//-----------------------------------------------------------------------------
//	Name:		GetUniversalTime
//	Object:		
//	06-02-21:	CAU - Created
//-----------------------------------------------------------------------------
UInt64	TIME_PS3::GetUniversalTime() const
{
	//time_t Tmp;
	//time(&Tmp);

	//return	(static_cast<UInt64>(Tmp));

	// time not yet available in sdk 0.6.0
	return	(0);
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
