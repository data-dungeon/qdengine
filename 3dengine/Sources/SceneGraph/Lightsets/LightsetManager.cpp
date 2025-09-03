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
//	CLASS:	LIGHTSET_MANAGER
//
//	02-11-04:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_KRNCORE(Time)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

LIGHTSET_MANAGER *LIGHTSET_MANAGER::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		LIGHTSET_MANAGER constructor
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
LIGHTSET_MANAGER::LIGHTSET_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		LIGHTSET_MANAGER destructor
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
LIGHTSET_MANAGER::~LIGHTSET_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
LIGHTSET_MANAGER *	LIGHTSET_MANAGER::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new LIGHTSET_MANAGER;
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHTSET_MANAGER::Suicide()
{
	delete _pInstance;
	_pInstance = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		PushLightTrack
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHTSET_MANAGER::PushLightTrack(const COM_INSTANCE &LightTrackInst)
{
	_dlLightTracks.PushTail(LightTrackInst);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	LIGHTSET_MANAGER::Update()
{
	QDT_DLIST<COM_INSTANCE>::ITERATOR itEach = _dlLightTracks.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR itTail = _dlLightTracks.GetTail();
	QDT_DLIST<COM_INSTANCE>::ITERATOR itToRemove;
	LIGHT_TRACK *pLightTrack = NULL;
	bool bEnd;

	float rDeltaFrameTime = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime();

	while (itEach != itTail)
	{
		pLightTrack = static_cast<LIGHT_TRACK*>((*itEach).GetCommunicator());
		pLightTrack->Update(rDeltaFrameTime, bEnd);
		++itEach;
		if (bEnd)
		{
			itToRemove = itEach;
			--itToRemove;
			_dlLightTracks.Remove(itToRemove);
		}
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
