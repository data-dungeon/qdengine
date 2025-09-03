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
// CLASS: TIME
//
//
//00 - 11 - 22 FKE Created:
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(Time/Time_ABC)

#ifdef PLATFORM_PC
	#include	INCL_KCORE(Time/Time_PC)
#endif

#ifdef PLATFORM_PS3
	#include	INCL_KCORE(Time/Time_PS3)
#endif

#ifdef PLATFORM_X360
	#include	INCL_KCORE(Time/Time_X360)
#endif

TIME*	TIME::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
TIME *	TIME::Instance()
{
	if (_pInstance == NULL)
	{
#if defined(PLATFORM_PC)	
		_pInstance = new TIME_PC;
#elif defined(PLATFORM_PS3)	
		_pInstance = new TIME_PS3;
#elif defined(PLATFORM_X360)	
		_pInstance = new TIME_X360;
#else
	#error PLATFORM UNIDENTIFIED, PLEASE CHECK
#endif
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		CreateInstance
//	Object:		
//	06-02-15:	VHA - Created
//-----------------------------------------------------------------------------
void TIME::CreateInstance()
{
	QDT_ASSERT(_pInstance == NULL);

	Instance();
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
void	TIME::Suicide()
{
	if (_pInstance)
	{
		delete _pInstance;
		_pInstance = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		TIME constructor
//	Object:		
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
TIME::TIME()
:
_rFPS					(0.0f),
_rTargetFPS				(30.0f),
_rComputeFPSPeriodTime	(1000.0f),
_nNbrFrames				(0),
_nNbrClocks				(0),
#if (!defined(_MASTER_FINAL_BUILD) && (defined(PLATFORM_PC)||defined(PLATFORM_PS2)))
_rFrameRateLock			(-1.0f),
#endif
_rFrameTime				(0)
{
}

//-----------------------------------------------------------------------------
//	Name:		~TIME destructor
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
TIME::~TIME()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		Init the high performance timer
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
void	TIME::Init()
{
	_rNextComputeFPSTime = ((Float32)GetSystemTime()) + _rComputeFPSPeriodTime;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-12-11:	RMA - Created
//-----------------------------------------------------------------------------
void	TIME::Update()
{
	UpdateSystemTime();
	_DefaultClock.Update();

	UInt32 nEachClock;

	for (nEachClock = 0 ; nEachClock < _nNbrClocks ; ++nEachClock)
	{
		_Clocks[nEachClock].Update();
	}
	
	UpdateFrameRate();
}

//-----------------------------------------------------------------------------
//	Name:		UpdateFrameRate
//	Object:		
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void	TIME::UpdateFrameRate()
{
	++_nNbrFrames;

	if (((Float32)GetSystemTime()) > _rNextComputeFPSTime)
	{
		_rNextComputeFPSTime += _rComputeFPSPeriodTime;
		_rFPS = _nNbrFrames / (_rComputeFPSPeriodTime * 0.001f);
		_nNbrFrames = 0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateNewClock
//	Object:		The new clock has the same parameters as default clock.
//				The method return the numero of the clock for access.
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
int	TIME::CreateNewClock()
{
	CLOCK NewClock(_DefaultClock);

	_Clocks[_nNbrClocks++] = NewClock;

	return (_nNbrClocks - 1);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		This method is here to avoid a warning
//	02-02-27:	RMA - Created
//-----------------------------------------------------------------------------
TIME &	TIME::operator=(const TIME &	Time)
{
	return (*this);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
