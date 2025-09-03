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
//	CLASS:	STREAM_TRACK_WRAPPER
//
//	03-05-30:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamTrackWrapper.h"
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include	INCL_3DENGINE(behavior/datas/streamedanimation/streamtrackheader)
#include	INCL_3DENGINE(behavior/datas/streamedanimation/streamtrack)
#include	INCL_3DENGINE(behavior/handlestreamanim/streamanimwrapper)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK_WRAPPER constructor
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_WRAPPER::STREAM_TRACK_WRAPPER()
:
_nTrackIndex(-1),
_pTrack(NULL),
_pAnimWrapper(NULL)
{

}


//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK_WRAPPER destructor
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_WRAPPER::~STREAM_TRACK_WRAPPER()
{
	_pTrack			= NULL;
	_pAnimWrapper	= NULL;
}


//-----------------------------------------------------------------------------
//	Name:		GetBaseRotation
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::GetBaseRotation(QUATERNION &	Q) const
{
	Q = _pTrack->GetAbsRot();
}

//-----------------------------------------------------------------------------
//	Name:		GetRotation
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::GetRotation(float			rTime,
										  QUATERNION &	Q) 
{
	STREAM_TRACK *pT;

	pT = _pAnimWrapper->GetStreamTrack( _nTrackIndex, rTime );
	if ( pT )
	{
		Q = pT->GetRot( _TrackIndex, rTime );
	}
	else
	{
		Q.Id();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetBaseTranslation
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::GetBaseTranslation(VECTOR &	V) const
{
	V = _pTrack->GetAbsTrans();
}

//-----------------------------------------------------------------------------
//	Name:		GetTranslation
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::GetTranslation(float		rTime,
											 VECTOR &	V) 
{
	STREAM_TRACK *pT;

	pT = _pAnimWrapper->GetStreamTrack( _nTrackIndex, rTime );
	if ( pT )
	{
		V = pT->GetTrans( _TrackIndex, rTime );
	}
	else
	{
		V.SetXYZ( 0.0f, 0.0f, 0.0f);
	}

}

//-----------------------------------------------------------------------------
//	Name:		GetTrackIndexes
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
const TRACK_INDEXES&	STREAM_TRACK_WRAPPER::GetTrackIndexes() const
{
	return ( _TrackIndex );
}


//-----------------------------------------------------------------------------
//	Name:		GetNbrRotKeys
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK_WRAPPER::GetNbrRotKeys() const
{
	STREAM_TRACK	*pT;
	int				n;
	float			rTime = 0;

	pT = _pAnimWrapper->GetStreamTrack( _nTrackIndex, rTime );
	if ( pT )
	{
		n = pT->GetNbrQuatKeys();
	}
	else
	{
		n = 0;
	}
	return ( n );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTransKeys
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK_WRAPPER::GetNbrTransKeys() const
{
	STREAM_TRACK	*pT;
	int				n;
	float			rTime = 0;

	pT = _pAnimWrapper->GetStreamTrack( _nTrackIndex, rTime );
	if ( pT )
	{
		n = pT->GetNbrQuatKeys();
	}
	else
	{
		n = 0;
	}
	return ( n );

}

//-----------------------------------------------------------------------------
//	Name:		GetTrackNbrFrames
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK_WRAPPER::GetTrackNbrFrames() const
{
	STREAM_TRACK	*pT;
	int				n;
	float			rTime = 0;

	pT = _pAnimWrapper->GetStreamTrack( _nTrackIndex, rTime );
	if ( pT )
	{
		n = pT->GetNbrQuatKeys();
	}
	else
	{
		n = 0;
	}
	return ( n );
}

//-----------------------------------------------------------------------------
//	Name:		GetTrackIndex
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK_WRAPPER::GetTrackIndex() const
{
	return ( _nTrackIndex );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimTops
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_TRACK_WRAPPER::GetAnimTops(const float			rFrame,
									  QDT_DLIST < int > &	vEvents)
{
	QDT_NOT_IMPLEMENTED();
	return ( -1 );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimPc
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
const ANIM_PC *	STREAM_TRACK_WRAPPER::GetAnimPc() const
{
	return ( NULL );
}

//-----------------------------------------------------------------------------
//	Name:		SetTrackIndex
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::SetTrackIndex(int	nTrackIndex)
{
	_nTrackIndex = nTrackIndex;
}

//-----------------------------------------------------------------------------
//	Name:		SetTrackHeader
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::SetTrackHeader(STREAM_TRACK_HEADER *	pT)
{
	_pTrack = pT;
}

//-----------------------------------------------------------------------------
//	Name:		SetAnimWrapper
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::SetAnimWrapper(STREAM_ANIM_WRAPPER *	pSAW)
{
	_pAnimWrapper = pSAW;
}

//-----------------------------------------------------------------------------
//	Name:		GetRotTrans
//	Object:		
//	03-06-18:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_WRAPPER::GetRotTrans(float	rTime,
										  QUATERNION_VECTOR & QV)
{
	STREAM_TRACK		*pT;
	float				rFrame;

	pT		= _pAnimWrapper->GetStreamTrack( _nTrackIndex, rTime );

	STREAM_HANDLE *pSH = _pAnimWrapper->GetStreamHandle();
	rFrame	= pSH->GetTheCurrentTime() * 30.0f;
	if ( pT )
	{
		QV = pT->GetRotTrans( _TrackIndex, rFrame );
	}
	else
	{
		QV.Id();
	}
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
