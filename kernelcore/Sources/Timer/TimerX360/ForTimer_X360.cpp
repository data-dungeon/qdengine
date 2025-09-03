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
#include 	INCL_KCORE(Timer/TimerX360/ForTimer_X360)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//------------------------------------------------
//FOR_TIMER
//------------------------------------------------

//-----------------------------------------------------------------------------
//	Name:		FOR_TIMER
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
FOR_TIMER::FOR_TIMER()
{
}


//-----------------------------------------------------------------------------
//	Name:		~FOR_TIMER
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
FOR_TIMER::~FOR_TIMER()
{
}



//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
int	FOR_TIMER::Init()
{
	return 1;
}


//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	FOR_TIMER::Close()
{
}


//-----------------------------------------------------------------------------
//	Name:		Check
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	FOR_TIMER::Check()
{
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
}


//-----------------------------------------------------------------------------
//	Name:		DelCallback
//	Object:		
//	01-01-16:	RPA - Created
//-----------------------------------------------------------------------------
void	FOR_TIMER::DelCallback( IntPtr nRef )
{
}

void FOR_TIMER::PauseTimer(IntPtr nRef)
{
}

void FOR_TIMER::ResumeTimer(IntPtr nRef)
{
}

void FOR_TIMER::PauseTimers()
{
}

void FOR_TIMER::ResumeTimers()
{
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
