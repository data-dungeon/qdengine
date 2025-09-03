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
//	CLASS:	BASE_TIMER_ATTR
//
//	02-03-01:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"BaseTimerAttr.h"

#ifdef _DEBUG
	#include "BaseTimerAttr.inl"
#endif
	
//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BASE_TIMER_ATTR constructor
//	Object:		
//	02-03-01:	cvi - Created
//-----------------------------------------------------------------------------
BASE_TIMER_ATTR::BASE_TIMER_ATTR()
:
_rDelay ( 0.0f ),
_rNewDelay ( 0.0f ),
_rCurrentTime ( 0.0f ),
_rTimeLong( 10.0f ),
_rTimeToGo( 10.0f ),
_rSpeed ( 1.0f ),
_bFinished ( false )
{
	
}

//-----------------------------------------------------------------------------
//	Name:		BASE_TIMER_ATTR constructor
//	Object:		
//	02-03-01:	cvi - Created
//-----------------------------------------------------------------------------
BASE_TIMER_ATTR::BASE_TIMER_ATTR(const BASE_TIMER_ATTR & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		BASE_TIMER_ATTR destructor
//	Object:		
//	02-03-01:	cvi - Created
//-----------------------------------------------------------------------------
BASE_TIMER_ATTR::~BASE_TIMER_ATTR()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-03-01:	cvi - Created
//-----------------------------------------------------------------------------
BASE_TIMER_ATTR &	BASE_TIMER_ATTR::operator=(const BASE_TIMER_ATTR & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-03-01:	cvi - Created
//-----------------------------------------------------------------------------
void	BASE_TIMER_ATTR::Copy(const BASE_TIMER_ATTR & C)
{
	_rDelay				= C._rDelay;
	_rNewDelay			= C._rNewDelay;
	_rCurrentTime		= C._rCurrentTime;
	_rTimeLong			= C._rTimeLong;
	_rSpeed				= C._rSpeed;
	_bFinished			= C._bFinished;

}

//-----------------------------------------------------------------------------
//	Name:		UpdateTime
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
bool	BASE_TIMER_ATTR::UpdateTime(float &	rDelay)
{
	///////////////////////////////////////////////
	_rDelay = rDelay;
	///////////////////////////////////////////////
	/*if ( IsPaused() )
	{
		rDelay		= 0.0f;
		_rNewDelay	= 0.0f;
		return ( false );
	}*/
	///////////////////////////////////////////////
	if ( IsFinished() )
	{
		_rNewDelay = rDelay;
		return ( true );
	}
	///////////////////////////////////////////////
	ComputeTime();
	rDelay = _rNewDelay;
	return ( IsFinished() );
	///////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	02-03-07:	CVI - Created
//-----------------------------------------------------------------------------
void	BASE_TIMER_ATTR::Reset()
{
	_rDelay			= 0.0f;
	_rNewDelay		= 0.0f;
	_rCurrentTime	= 0.0f;
	_bFinished		= false;
}

//-----------------------------------------------------------------------------
//	Name:		ComputeTime
//	Object:		
//	02-03-05:	cvi - Created
//-----------------------------------------------------------------------------
bool	BASE_TIMER_ATTR::ComputeTime()
{
	float		rTimeToGo;
	float		rIncTime;

	////////////////////////////////////////
	rTimeToGo	= _rTimeLong - _rCurrentTime;
	rIncTime	= _rDelay * _rSpeed;
	////////////////////////////////////////
	if ( rIncTime > rTimeToGo )
	{
		_bFinished		= true;
		_rNewDelay		= rIncTime - _rTimeToGo;
		_rCurrentTime	= _rTimeLong;
		_rTimeToGo		= 0.0f; //	Finished
		return ( true );
	}
	////////////////////////////////////////
	_rCurrentTime	+= rIncTime;
	_rNewDelay		= 0.0f;
	if ( _rSpeed >= 0.0f )
	{
		_rTimeToGo		= ( _rTimeLong - _rCurrentTime ) / _rSpeed;
	}
	return ( false );
	////////////////////////////////////////
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
