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
//	CLASS:	TIMER_CALLBACK
//
//	02-08-13:	XXX - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Timer/TimerCallback)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TIMER_CALLBACK constructor
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_CALLBACK::TIMER_CALLBACK()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		TIMER_CALLBACK constructor
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_CALLBACK::TIMER_CALLBACK(int	p,
							   void	( * pF)(IntPtr),
							   IntPtr	pa,
							   IntPtr	Ref,
							   int		NbLoop)
{
	_Periode = p;
	_pFnct = pF;
	_TimeCount = _Periode;
	_pNext = NULL;
	_Param = pa;
//module de la callback
	_Module = Ref;
	_Flags = 0;
	_nNbLoop = NbLoop;
}
	
//-----------------------------------------------------------------------------
//	Name:		TIMER_CALLBACK constructor
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_CALLBACK::TIMER_CALLBACK(const TIMER_CALLBACK & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		TIMER_CALLBACK destructor
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_CALLBACK::~TIMER_CALLBACK()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_CALLBACK &	TIMER_CALLBACK::operator=(const TIMER_CALLBACK & C)
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
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_CALLBACK::Copy(const TIMER_CALLBACK & C)
{
	QDT_NOT_IMPLEMENTED();
}


//-----------------------------------------------------------------------------
//	Name:		Check
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
bool	TIMER_CALLBACK::Check()
{
	if (_Flags & PAUSE)
	{
		return true;
	}
	if (_TimeCount == 0)
	{
		if (_nNbLoop == 0)
		{
			return false;
		}
		if (_nNbLoop > 0)
		{
			_nNbLoop--;
		}
		if (_pFnct != NULL)
		{
			_pFnct(_Param);
		}
		_TimeCount = _Periode;
		return true;
	}
	_TimeCount--;
	return true;
}


//-----------------------------------------------------------------------------
//	Name:		GetNext
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
TIMER_CALLBACK *	TIMER_CALLBACK::GetNext()
{
	return _pNext;
}

//-----------------------------------------------------------------------------
//	Name:		SetNext
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_CALLBACK::SetNext(TIMER_CALLBACK *	pTC)
{
	_pNext = pTC;
}

//-----------------------------------------------------------------------------
//	Name:		GetModule
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
int	TIMER_CALLBACK::GetModule()
{
	return _Module;
}

//-----------------------------------------------------------------------------
//	Name:		Pause
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_CALLBACK::Pause()
{
	_Flags |= PAUSE;
}

//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	02-08-13:	XXX - Created
//-----------------------------------------------------------------------------
void	TIMER_CALLBACK::Resume()
{
	_Flags &= ~PAUSE;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
