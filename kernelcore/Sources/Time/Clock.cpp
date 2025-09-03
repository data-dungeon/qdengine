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
//	CLASS:	CLOCK
//
//	02-02-27:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Time/Clock)
#include	INCL_KCORE(Time/Time_ABC)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CLOCK constructor
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
CLOCK::CLOCK()
:
_bPause						(false),
_rCurrentTimeDecalage		(0),
_rPausedCurrentTime			(0),
_rDeltaFrameTime			(0),
_rScale						(1.0f),
_rDeltaFrameTimeInS			(0.0f),
_rOldTime					(0.0f),
_rPausePausedCurrentTime	(0.0f),
_rPauseOldTime				(0.0f),
_rPauseCurrentTimeDecalage	(0.0f),
_rPauseDeltaFrameTime		(0.0f),
_rPauseDeltaFrameTimeInS	(0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CLOCK constructor
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
CLOCK::CLOCK(const CLOCK & Clock)
{
	Copy(Clock);
}

//-----------------------------------------------------------------------------
//	Name:		CLOCK destructor
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
CLOCK::~CLOCK()
{
	// Do nothing	
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
CLOCK &	CLOCK::operator=(const CLOCK &	Clock)
{
	if (this != &Clock)
	{
		Copy(Clock);
	}

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOCK::Copy(const CLOCK & Clock)
{
	_bPause					= Clock._bPause;
	_rCurrentTimeDecalage	= Clock._rCurrentTimeDecalage;
	_rPausedCurrentTime		= Clock._rPausedCurrentTime;
	_rDeltaFrameTime		= Clock._rDeltaFrameTime;
	_rScale					= Clock._rScale;
	_rDeltaFrameTimeInS		= Clock._rDeltaFrameTimeInS;
}

//-----------------------------------------------------------------------------
//	Name:		SetPause
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOCK::Pause()
{
	if (IsPaused() == false)
	{
		_rPauseOldTime				= _rOldTime;
		_rPauseCurrentTimeDecalage	= _rCurrentTimeDecalage;
		_rPauseDeltaFrameTime		= _rDeltaFrameTime;
		_rPauseDeltaFrameTimeInS	= _rDeltaFrameTimeInS;	
		_rPausedCurrentTime			= TIME::Instance()->GetFrameTime() - _rCurrentTimeDecalage;

		_rOldTime					= 0.0f;
		_rCurrentTimeDecalage		= 0.0f;
		_rDeltaFrameTime			= 0.0f;
		_rDeltaFrameTimeInS			= 0.0f;

		_bPause = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPause
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOCK::Continue()
{
	if	(IsPaused())
	{
		_rOldTime				= _rPauseOldTime;
		_rCurrentTimeDecalage	+= (TIME::Instance()->GetFrameTime() - _rCurrentTimeDecalage) - _rPausedCurrentTime;
		_rDeltaFrameTime		= _rPauseDeltaFrameTime;
		_rDeltaFrameTimeInS		= _rPauseDeltaFrameTimeInS;

		_rPauseOldTime					= 0.0f;
		_rPauseCurrentTimeDecalage		= 0.0f;
		_rPauseDeltaFrameTime			= 0.0f;
		_rPauseDeltaFrameTimeInS		= 0.0f;
		_rPausedCurrentTime				= 0.0f;

		_bPause = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentPreciseTime
//	Object:		
//	03-06-11:	RMA - Created
//-----------------------------------------------------------------------------
Float32	CLOCK::GetCurTime() const
{
	if (IsPaused())
	{
		return (_rPausedCurrentTime);
	}

	return (TIME::Instance()->GetFrameTime() - _rCurrentTimeDecalage);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOCK::Update()
{
	if	(IsPaused() == false)
	{
		Float32 rCurrentTime = GetCurTime();

		if (_rOldTime == 0.0f)
		{
			// First frame
			_rDeltaFrameTime = 0.0f;
		}
		else
		{
			_rDeltaFrameTime = rCurrentTime - _rOldTime;
		}

		_rOldTime = rCurrentTime;

		_rDeltaFrameTimeInS = _rDeltaFrameTime * 0.001f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDeltaFrameTime
//	Object:		
//	06-05-09:	NHO - Created
//-----------------------------------------------------------------------------
void	CLOCK::SetDeltaFrameTime(const Float32 &	rDeltaFrameTime)
{
	_rDeltaFrameTime	= rDeltaFrameTime;
	_rDeltaFrameTimeInS = _rDeltaFrameTime * 0.001f;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
