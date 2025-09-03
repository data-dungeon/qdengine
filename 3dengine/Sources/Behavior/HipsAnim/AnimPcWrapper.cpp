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
//	CLASS:	ANIM_PC_WRAPPER
//
//	01-10-18:	cvi - Created
//*****************************************************************************

#include	"Root.h" 
#include	"AnimPcWrapper.h"
#include	INCL_3DENGINE(Behavior/TrackAnim/TrackDataWrapper)
#include	INCL_3DENGINE(Behavior/Datas/AnimPC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ANIM_PC_WRAPPER constructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_PC_WRAPPER::ANIM_PC_WRAPPER()
:
_pTracks ( NULL ),
_nNbrFrames( 0 ),
_nNbrTracks( 0 )
{
	
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_PC_WRAPPER constructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_PC_WRAPPER::ANIM_PC_WRAPPER(const ANIM_PC_WRAPPER & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_PC_WRAPPER destructor
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_PC_WRAPPER::~ANIM_PC_WRAPPER()
{
	if ( _pTracks )	delete [] _pTracks;
	_pTracks = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_PC_WRAPPER &	ANIM_PC_WRAPPER::operator=(const ANIM_PC_WRAPPER & C)
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
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_PC_WRAPPER::Copy(const ANIM_PC_WRAPPER & C)
{
//	_AnimPc = C._pAnimPc;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrFrames
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_PC_WRAPPER::GetNbrFrames() 
{
	///////////////////////
	return ( _nNbrFrames );
	///////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrTracks
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
int	ANIM_PC_WRAPPER::GetNbrTracks() 
{
	//////////////////////
	return ( _nNbrTracks );
	//////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		SetAnim
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_PC_WRAPPER::SetAnim( COM_INSTANCE& Anim )
{
	ANIM_PC					*pA;

	////////////////////////////
	_AnimPc = Anim;
	if ( !_AnimPc.HasHandle() )
	{
		return;
	}
	////////////////////////////

	///////////////////////////////////////
	pA = GetAnimPc();
	_nNbrFrames = pA->GetNbFrames();
	_nNbrTracks = pA->GetNbTracks();
	CreateAllTracks();
	////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		CreateAllTracks
//	Object:		
//	03-06-02:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC_WRAPPER::CreateAllTracks()
{
	int					i;
	ANIM_PC				*pA;

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
	pA = GetAnimPc();
	QDT_ASSERT( pA );
	////////////////////////////////////////////////

	////////////////////////////////////////////////
	_pTracks = new TRACK_DATA_WRAPPER[_nNbrTracks];
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].SetAnim(pA);
		_pTracks[i].SetTrackIndex(i);		
		_pTracks[i].SetTrack(pA->GetTrack(i));
	}
	////////////////////////////////////////////////

}

//-----------------------------------------------------------------------------
//	Name:		GetAnimPc
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_PC *	ANIM_PC_WRAPPER::GetAnimPc() 
{
	COM_INSTANCE	CI;
	ANIM_PC			*pAnim;

	////////////////////////////////////////////////////
	CI = GetAnim();
	if ( !CI.HasHandle() )	return ( NULL );
	pAnim = static_cast<ANIM_PC*> ( CI.GetCommunicator() );
	////////////////////////////////////////////////////
	return ( pAnim );
}

//-----------------------------------------------------------------------------
//	Name:		GetAnim
//	Object:		
//	01-10-18:	cvi - Created
//-----------------------------------------------------------------------------
COM_INSTANCE&	ANIM_PC_WRAPPER::GetAnim() 
{
	return ( _AnimPc );
}

//-----------------------------------------------------------------------------
//	Name:		GetTrack
//	Object:		
//	03-06-02:	CVI - Created
//-----------------------------------------------------------------------------
ITRACK_WRAPPER *	ANIM_PC_WRAPPER::GetTrack(int	nNum)
{
	return ( &_pTracks[ nNum ] );
}

//-----------------------------------------------------------------------------
//	Name:		ReadRotTrans
//	Object:		
//	03-07-25:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC_WRAPPER::ReadRotTrans(float					rTime,
									  QUATERNION_VECTOR *	pQV)
{
	int					i;
	TRACK_DATA_WRAPPER	*pTW;

	pTW = _pTracks +1;

	for ( i = 1; i < _nNbrTracks; ++i )
	{
		pTW->GetRotTrans( rTime, *pQV);
		pQV++;
		pTW++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseAnim
//	Object:		
//	03-10-14:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC_WRAPPER::ReleaseAnim()
{
	if ( _AnimPc.HasHandle() )
	{
		_AnimPc.Release();
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateCommunicator
//	Object:		
//	04-01-23:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_PC_WRAPPER::UpdateCommunicator()
{
	int				i;
	ANIM_PC			*pA;
	
	if ( !_pTracks )	return;

	pA = GetAnimPc();
	QDT_ASSERT( pA );
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		_pTracks[i].SetAnim( pA );
		_pTracks[i].SetTrack( pA->GetTrack( i ) );
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
