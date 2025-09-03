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
//	CLASS:	TRACK_DATA_WRAPPER
//
//	01-10-17:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"TrackDataWrapper.h"
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include	INCL_3DENGINE(Behavior/Datas/TrackData)
#include	INCL_3DENGINE(Behavior/Datas/AnimPC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TRACK_DATA_WRAPPER constructor
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
TRACK_DATA_WRAPPER::TRACK_DATA_WRAPPER()
:
_pTrack			(NULL),
_pAnimPc		(NULL),
_nTrackIndex	(-1)
{
}

//-----------------------------------------------------------------------------
//	Name:		TRACK_DATA_WRAPPER constructor
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
TRACK_DATA_WRAPPER::TRACK_DATA_WRAPPER(const TRACK_DATA_WRAPPER & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		TRACK_DATA_WRAPPER destructor
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
TRACK_DATA_WRAPPER::~TRACK_DATA_WRAPPER()
{
	_pTrack			= NULL;
	_pAnimPc		= NULL;
	_nTrackIndex	= -1;

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
TRACK_DATA_WRAPPER &	TRACK_DATA_WRAPPER::operator=(const TRACK_DATA_WRAPPER & C)
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
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::Copy(const TRACK_DATA_WRAPPER & C)
{
	_pAnimPc		= C._pAnimPc;
	_pTrack			= C._pTrack;
	_nTrackIndex	= C._nTrackIndex;
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseRotation
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::GetBaseRotation(QUATERNION& Q) const
{
	///////////////////////////////
	Q.Id();
	if ( !_pTrack )
	{
		return;
	}
	///////////////////////////////

	///////////////////////////////
	_pTrack->GetBaseRotation(Q);
	///////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetRotation
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::GetRotation(float			rTime, 
										QUATERNION &	Q) 
{
	if (!_pTrack) return;

	_pTrack->SetTrackIndexes(&_TrackIndex);

	_pTrack->GetRot(rTime, Q);

//	_pAnimPc->SetFramesPlayed(rTime);
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseTranslation
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::GetBaseTranslation( VECTOR& V ) const
{
	///////////////////////////////
	V.SetXYZ( 0.0f, 0.0f, 0.0f );
	if ( !_pTrack )
	{
		return;
	}
	///////////////////////////////

	///////////////////////////////
	_pTrack->GetBaseTranslation( V );
	///////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetTranslation
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::GetTranslation(float	rTime, 
										   VECTOR & V) 
{
	V.SetXYZ(0.0f, 0.0f, 0.0f);
	if (!_pTrack || (!_pTrack->HasTranslation()))
	{
		return;
	}

	_pTrack->SetTrackIndexes(&_TrackIndex);
	_pTrack->GetTrans(rTime, V);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRotKeys
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA_WRAPPER::GetNbrRotKeys() const
{
	QDT_NOT_IMPLEMENTED();
	return ( 0 );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTransKeys
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA_WRAPPER::GetNbrTransKeys() const
{
	QDT_NOT_IMPLEMENTED();
	return ( 0 );
}

//-----------------------------------------------------------------------------
//	Name:		GetTrackNbrFrames
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA_WRAPPER::GetTrackNbrFrames() const
{
	///////////////////////////////
	if ( !_pAnimPc )
	{
		QDT_NOT_IMPLEMENTED();
		return 0;
	}
	///////////////////////////////

	///////////////////////////////
	return ( _pAnimPc->GetNbFrames() );
	///////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		GetTrackIndex
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA_WRAPPER::GetTrackIndex() const
{
	return ( _nTrackIndex );
}

//-----------------------------------------------------------------------------
//	Name:		SetTrack
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::SetTrack(TRACK_DATA *pTrack)
{
	_pTrack = pTrack;

	if (pTrack )
	{
		pTrack->SetTrackIndexes(&_TrackIndex);
		pTrack->ResetTrack();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTrackIndex
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::SetTrackIndex(int	nIndex)
{
	_nTrackIndex = nIndex;
}

//-----------------------------------------------------------------------------
//	Name:		SetAnim
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::SetAnim(ANIM_PC *	pAnim)
{
	_pAnimPc = pAnim;
}

//-----------------------------------------------------------------------------
//	Name:		GetTrack
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
const TRACK_DATA *	TRACK_DATA_WRAPPER::GetTrack() const
{
	return ( _pTrack );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimPc
//	Object:		
//	01-10-17:	cvi - Created
//-----------------------------------------------------------------------------
const ANIM_PC *	TRACK_DATA_WRAPPER::GetAnimPc() const
{
	return ( _pAnimPc );
}

//-----------------------------------------------------------------------------
//	Name:		GetEvent
//	Object:		
//	01-10-26:	cvi - Created
//-----------------------------------------------------------------------------
int	TRACK_DATA_WRAPPER::GetAnimTops(const float			rFrame,
								 QDT_DLIST < int > &	vEvents)
{
	return	(_pTrack->GetAnimTops(rFrame, vEvents));
}

//-----------------------------------------------------------------------------
//	Name:		GetRotTrans
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	TRACK_DATA_WRAPPER::GetRotTrans(float rTime, QUATERNION_VECTOR & QV)
{
	if (!_pTrack)	return;

	_pTrack->SetTrackIndexes(&_TrackIndex);
	_pTrack->GetRotTrans(rTime, QV);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
