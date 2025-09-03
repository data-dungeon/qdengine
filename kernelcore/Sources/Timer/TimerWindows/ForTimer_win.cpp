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
// CLASS: FOR_TIMER__WIN
//
//
//00 - 11 - 22 FKE Created:
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( FOR_TIMER__WIN )
//
//	The FOR_TIMER__WIN class implements ...
//
//*****************************************************************************

#include 	"Root.h"
#include 	INCL_KCORE(Timer/TimerWindows/ForTimer_win)
#include 	INCL_KCORE(Timer/TimerCallback)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================
//----------------------------------------------
//Callback timer windows
//----------------------------------------------
void CALLBACK TimeProc( UINT uID,
						UINT uMsg,
						DWORD dwUser,
						DWORD dw1,
						DWORD dw2 )
{
	try//QDT_TRY
	{
		FOR_TIMER *pFT;
		pFT = reinterpret_cast < FOR_TIMER * > ( dwUser );
		pFT -> Check();
	}
	catch(...)
	{
		
	}
	//Prob d'include
	//QDT_UNHANDLED_CATCH
}

//------------------------------------------------
//FOR_TIMER
//------------------------------------------------




//-----------------------------------------------------------------------------
//	Name:		FOR_TIMER
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
FOR_TIMER::FOR_TIMER()
:
_nFlags( 0 ),
_pTimerCallback( NULL )
{
	
}


//-----------------------------------------------------------------------------
//	Name:		~FOR_TIMER
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
FOR_TIMER::~FOR_TIMER()
{
	TIMER_CALLBACK *pTC1 , *pTC2;
	Close();
	pTC1 = _pTimerCallback;
	while ( pTC1 != NULL )
	{
		pTC2 = pTC1;
		pTC1 = pTC1 -> GetNext();
		delete pTC2;
	}
}



//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
int	FOR_TIMER::Init()
{
	TIMECAPS Tc;
	if ( _nFlags & INIT )
	{
		return ( 1 );
	}
	
	timeGetDevCaps( & Tc , sizeof( TIMECAPS ) );
	timeBeginPeriod( 10 );
	_nTimerID = timeSetEvent( 10 , 10 , ( LPTIMECALLBACK ) TimeProc , ( int ) this , TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
	if ( _nTimerID == NULL )
	{
		return ( 0 );
	}
	_nFlags |= INIT;
	return ( 1 );
}


//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	FOR_TIMER::Close()
{
	if ( _nFlags & INIT )
	{
		timeEndPeriod( 1000 );
		timeKillEvent( _nTimerID );
	}
	_nFlags &= ~INIT;
}


//-----------------------------------------------------------------------------
//	Name:		Check
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	FOR_TIMER::Check()
{
	TIMER_CALLBACK *pTC;
	pTC = _pTimerCallback;
	while ( pTC != NULL )
	{
		pTC -> Check();
		pTC = pTC -> GetNext();
	}
}



//-----------------------------------------------------------------------------
//	Name:		AddCallback
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void FOR_TIMER::AddCallback( int p,
							 void ( *pF )( IntPtr ),
							 IntPtr nParam,
							 IntPtr nRef,
							 int nNbLoop)
{
	TIMER_CALLBACK *pTC , *pTC2;
	pTC2 = _pTimerCallback;
//on regarde si la référence existe déjà...si oui, on la vire et on la remplace
	pTC = pTC2;
	while ( pTC2 != NULL )
	{
		if ( pTC2 -> GetModule() == nRef )
		{
			if (pTC != pTC2)
			{
				pTC -> SetNext( pTC2 -> GetNext() );
				delete pTC2;
				break;
			}
			else
			{
				_pTimerCallback = pTC2 -> GetNext();
				delete pTC2;
				break;
			}
		}
		pTC = pTC2;
		pTC2 = pTC2 -> GetNext();
	}

	pTC = new TIMER_CALLBACK( p , pF , nParam , nRef, nNbLoop );
	pTC2 = _pTimerCallback;
	if ( pTC2 == NULL )
	{
		_pTimerCallback = pTC;
	}
	else
	{
		while ( pTC2 -> GetNext() != NULL )
		{
			pTC2 = pTC2 -> GetNext();
		}
		pTC2 -> SetNext( pTC );
	}
}


//-----------------------------------------------------------------------------
//	Name:		DelCallback
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	FOR_TIMER::DelCallback( IntPtr nRef )
{
	TIMER_CALLBACK *pTC , *pTC2;
	pTC2 = _pTimerCallback;
	if ( pTC2 == NULL )
	{
		return;
	}
	if ( pTC2 -> GetModule() == nRef )
	{
		_pTimerCallback = pTC2 -> GetNext();
		delete pTC2;
		return;
	}
	pTC = pTC2;
	pTC2 = pTC2 -> GetNext();
	while ( pTC2 != NULL )
	{
		if ( pTC2 -> GetModule() == nRef )
		{
			pTC -> SetNext( pTC2 -> GetNext() );
			delete pTC2;
			return ; 		
		}
		pTC = pTC2;
		pTC2 = pTC2 -> GetNext();
	}
	
}

void FOR_TIMER::PauseTimer(IntPtr nRef)
{
	TIMER_CALLBACK *pTC = _pTimerCallback;
	while (pTC != NULL)
	{
		if (pTC->GetModule() == nRef)
		{
			pTC->Pause();
			return;
		}
		pTC = pTC->GetNext();
	}
}

void FOR_TIMER::ResumeTimer(IntPtr nRef)
{
	TIMER_CALLBACK *pTC = _pTimerCallback;
	while (pTC != NULL)
	{
		if (pTC->GetModule() == nRef)
		{
			pTC->Resume();
			return;
		}
		pTC = pTC->GetNext();
	}
}

void FOR_TIMER::PauseTimers()
{
	TIMER_CALLBACK *pTC = _pTimerCallback;
	while (pTC != NULL)
	{
		pTC->Pause();
		pTC = pTC->GetNext();
	}
}

void FOR_TIMER::ResumeTimers()
{
	TIMER_CALLBACK *pTC = _pTimerCallback;
	while (pTC != NULL)
	{
		pTC->Resume();
		pTC = pTC->GetNext();
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
