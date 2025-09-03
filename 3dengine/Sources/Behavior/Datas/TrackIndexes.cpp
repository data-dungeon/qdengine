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
//	CLASS:	TRACK_INDEXES
//
//	02-01-14:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"TrackIndexes.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TRACK_INDEXES constructor
//	Object:		
//	01-12-07:	RMA - Created
//-----------------------------------------------------------------------------
TRACK_INDEXES::TRACK_INDEXES()
:
_nRotIndex			(0),
_nOldRotIndex		(0),
_nTransIndex		(0),
_nOldTransIndex		(0),
_nScaleIndex		(0),
_nOldScaleIndex		(0),
_nAnimTopIndex		(0),		
_nOldAnimTopIndex	(0)
{
	// DO NOTHING
}

//-----------------------------------------------------------------------------
//	Name:		TRACK_INDEXES destructor
//	Object:		
//	01-12-07:	RMA - Created
//-----------------------------------------------------------------------------
TRACK_INDEXES::~TRACK_INDEXES()
{
	// DO NOTHING
}

//-----------------------------------------------------------------------------
//	Name:		TRACK_INDEXES constructor
//	Object:		
//	01-12-07:	RMA - Created
//-----------------------------------------------------------------------------
TRACK_INDEXES::TRACK_INDEXES(const TRACK_INDEXES & TrackIndexes)
{
	Copy(TrackIndexes);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-07:	RMA - Created
//-----------------------------------------------------------------------------
TRACK_INDEXES &	TRACK_INDEXES::operator = (const TRACK_INDEXES & TrackIndexes)
{
	if (this != &TrackIndexes)
	{
		Copy(TrackIndexes);
	}

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-12-07:	RMA - Created
//-----------------------------------------------------------------------------
void	TRACK_INDEXES::Copy(const TRACK_INDEXES & TrackIndexes)
{
	_nRotIndex			= TrackIndexes._nRotIndex;
	_nOldRotIndex		= TrackIndexes._nOldRotIndex;	
	_nTransIndex		= TrackIndexes._nTransIndex;
	_nOldTransIndex		= TrackIndexes._nOldTransIndex;
	_nScaleIndex		= TrackIndexes._nScaleIndex;
	_nOldScaleIndex		= TrackIndexes._nOldScaleIndex;
	_nAnimTopIndex		= TrackIndexes._nAnimTopIndex;
	_nOldAnimTopIndex	= TrackIndexes._nOldAnimTopIndex; 
}


//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	03-06-06:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_INDEXES::Reset()
{
	_nRotIndex			= 0;
	_nOldRotIndex		= 0;	
	_nTransIndex		= 0;
	_nOldTransIndex		= 0;
	_nScaleIndex		= 0;
	_nOldScaleIndex		= 0;
	_nAnimTopIndex		= 0;
	_nOldAnimTopIndex	= 0; 

}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
