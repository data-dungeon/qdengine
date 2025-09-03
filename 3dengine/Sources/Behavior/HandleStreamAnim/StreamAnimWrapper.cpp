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
//	CLASS:	STREAM_ANIM_WRAPPER
//
//	03-05-30:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamAnimWrapper.h"
#include	INCL_KMATH(Math\Geometry\QuaternionVector\QuaternionVector)
#include	INCL_3DENGINE(behavior\datas\streamedanimation\streamanimation)
#include	INCL_3DENGINE(behavior\handlestreamanim\streamtrackWrapper)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_ANIM_WRAPPER constructor
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_ANIM_WRAPPER::STREAM_ANIM_WRAPPER()
:
_pTracks(NULL),
_nNbrTracks(0),
_nNbrFrames(0)
{

}

//-----------------------------------------------------------------------------
//	Name:		STREAM_ANIM_WRAPPER destructor
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_ANIM_WRAPPER::~STREAM_ANIM_WRAPPER()
{
	if ( _pTracks )
	{
		delete [] _pTracks;
		_pTracks = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrFrames
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_ANIM_WRAPPER::GetNbrFrames()
{
	return ( _nNbrFrames );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTracks
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
int	STREAM_ANIM_WRAPPER::GetNbrTracks()
{
	return ( _nNbrTracks );
}

//-----------------------------------------------------------------------------
//	Name:		SetAnim
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::SetAnim(COM_INSTANCE &	Anim)
{
	STREAM_ANIMATION	*pA;

	////////////////////////////
	_Anim = Anim;
	if ( !_Anim.HasHandle() )
	{
		return;
	}
	////////////////////////////

	////////////////////////////
	pA = static_cast<STREAM_ANIMATION*> ( _Anim.GetCommunicator() );
	_nNbrFrames = pA->GetNbrFrames();
	_nNbrTracks = pA->GetNbrTracks();
	CreateAllTracks();
	InitHandleStream();
	////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		CreateAllTracks
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::CreateAllTracks()
{
	int					i;
	STREAM_ANIMATION	*pSA;

	////////////////////////////////////////////////
	if ( _nNbrTracks <= 0 )
	{
		return;
	}
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	if ( _pTracks ) delete [] _pTracks;
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	pSA = STREAM_ANIMATION::GetStreamAnimFromInstance( _Anim );
	QDT_ASSERT( pSA );
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	_pTracks = new STREAM_TRACK_WRAPPER[_nNbrTracks];
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].SetTrackIndex( i );
		_pTracks[i].SetAnimWrapper( this );
		_pTracks[i].SetTrackHeader( pSA->GetTrackHeader( i ) );
	}
	////////////////////////////////////////////////
}


//-----------------------------------------------------------------------------
//	Name:		GetAnim
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
COM_INSTANCE &	STREAM_ANIM_WRAPPER::GetAnim()
{
	return ( _Anim );
}


//-----------------------------------------------------------------------------
//	Name:		GetStreamTrack
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK *	STREAM_ANIM_WRAPPER::GetStreamTrack( int nNum, float	rFrame)
{
	//////////////////////////////////////
	if ( nNum == 0 )
	{
		STREAMABLE	*pS = static_cast<STREAMABLE*> ( _Anim.GetCommunicator() );
		_HandleStream.PrepareBlock( rFrame, this );
//		printf("Anim time : %f\n", _HandleStream.GetCurrentTime() );
	}
	//////////////////////////////////////

	//////////////////////////////////////
	return ( _HandleStream.GetStreamTrack( nNum, rFrame ) );
	//////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetTrack
//	Object:		
//	03-06-02:	CVI - Created
//-----------------------------------------------------------------------------
ITRACK_WRAPPER *	STREAM_ANIM_WRAPPER::GetTrack(int	nNum)
{
	return ( &_pTracks[nNum] );
}

//-----------------------------------------------------------------------------
//	Name:		InitHandleStream
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::InitHandleStream()
{
	_HandleStream.SetAnim( _Anim );
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamHandle
//	Object:		
//	03-06-20:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE *	STREAM_ANIM_WRAPPER::GetStreamHandle()
{
	return ( &_HandleStream );
}

//-----------------------------------------------------------------------------
//	Name:		ResetTracks
//	Object:		
//	03-06-30:	VMA - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::ResetTracks()
{
	int		i;

	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].ResetTrackIndex();
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReadRotTrans
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::ReadRotTrans(float					rTime,
										  QUATERNION_VECTOR *	pQV)
{
	int						i;
	STREAM_TRACK_WRAPPER	*pTW;

	pTW = _pTracks +1;

	for ( i = 1; i < _nNbrTracks; ++i )
	{
		pTW->GetRotTrans( rTime, *pQV );
		pQV++;
		pTW++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseAnim
//	Object:		
//	03-10-14:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::ReleaseAnim()
{
	if ( _Anim.HasHandle() )
	{
		_Anim.Release();
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateCommunicator
//	Object:		
//	04-01-23:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIM_WRAPPER::UpdateCommunicator()
{
	int					i;
	STREAM_ANIMATION	*pSA;

	if ( !_pTracks )	return;


	pSA = STREAM_ANIMATION::GetStreamAnimFromInstance( _Anim );
	QDT_ASSERT( pSA );
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].SetTrackHeader( pSA->GetTrackHeader( i ) );
	}		
	////////////////////////////////////////////////
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
