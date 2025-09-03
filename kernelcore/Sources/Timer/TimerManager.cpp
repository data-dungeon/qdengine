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
// CLASS: TIMER_MANAGER
//
//
//00-11-22 FKE Created: 
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (TIMER_MANAGER)
//
//	The TIMER_MANAGER class implements ...
//
//*****************************************************************************

#include "Root.h"
#include INCL_KCORE(Timer/TimerManager)

#if defined(PLATFORM_PC)
	#include INCL_KCORE(Timer/TimerWindows/ForTimer_win)
#elif defined(PLATFORM_PS3)
	#include INCL_KCORE(Timer/TimerPS3/ForTimer_ps3)
#elif defined(PLATFORM_X360)
	#include INCL_KCORE(Timer/TimerX360/ForTimer_X360)
#else
	#error CAN T IDENTIFY TARGET PLATFORM, PLEASE CHECK
#endif

TIMER_MANAGER *TIMER_MANAGER::_pInstance = NULL; 
int TIMER_MANAGER::_bDestroyedOnce = 0; 

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//------------------------------------------------
//TIMER_MANAGER
//------------------------------------------------
TIMER_MANAGER::TIMER_MANAGER()
{
	_pForTimer = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		~TIMER_MANAGER destructor
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_MANAGER::~TIMER_MANAGER()
{
	Close();
}


//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
int	TIMER_MANAGER::Init()
{
	int nerr;
	if (_pForTimer != NULL)
	{
		return 1;
	}
	_pForTimer = new FOR_TIMER;	
	nerr = _pForTimer->Init();
	if (!nerr)
	{
		Close();
		return 0;
	}
	return 1;
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::Close()
{
	if (_pForTimer != NULL)
	{
		delete _pForTimer;
		_pForTimer = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddCallback
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::AddCallback(int	p,
								   void	( * pF)(IntPtr),
								   IntPtr	Param,
								   IntPtr	Ref,
								   int	nNbLoop)
{
	if (_pForTimer != NULL)
	{
		_pForTimer->AddCallback(p, pF, Param, Ref, nNbLoop);
	}
}

//-----------------------------------------------------------------------------
//	Name:		DelCallback
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::DelCallback(IntPtr	ref)
{
	if (_pForTimer != NULL)
	{
		_pForTimer->DelCallback(ref);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PauseTimer
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::PauseTimer(IntPtr	ref)
{
	if (_pForTimer != NULL)
	{
		_pForTimer->PauseTimer(ref);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResumeTimer
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::ResumeTimer(IntPtr	ref)
{
	if (_pForTimer != NULL)
	{
		_pForTimer->ResumeTimer(ref);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PauseTimers
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::PauseTimers()
{
	if (_pForTimer != NULL)
	{
		_pForTimer->PauseTimers();
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResumeTimers
//	Object:		
//	xx-xx-xx:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::ResumeTimers()
{
	if (_pForTimer != NULL)
	{
		_pForTimer->PauseTimers();
	}
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-08-13:	DAC - Created
//-----------------------------------------------------------------------------
TIMER_MANAGER *	TIMER_MANAGER::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new TIMER_MANAGER();
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	02-08-13:	DAC - Created
//-----------------------------------------------------------------------------
void	TIMER_MANAGER::Suicide()
{
	if (_pInstance != NULL)
	{
		delete _pInstance;
		_pInstance = NULL; // :)
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsInstancied
//	Object:		
//	02-08-13:	DAC - Created
//-----------------------------------------------------------------------------
bool	TIMER_MANAGER::IsInstancied()
{
	return (_pInstance != NULL);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
