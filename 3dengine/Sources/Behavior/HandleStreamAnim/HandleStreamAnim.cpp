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
//	CLASS:	STREAM_HANDLE_ANIM
//
//	03-05-30:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"HandleStreamAnim.h"
#include	INCL_KCOM(Streaming\Streamable)
#include	INCL_3DENGINE(behavior\datas\streamedanimation\animblock)
#include	INCL_3DENGINE(behavior\handlestreamanim\streamanimwrapper)
#include	INCL_3DENGINE(behavior\handlestreamanim\streamtrackWrapper)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_HANDLE_ANIM constructor
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_ANIM::STREAM_HANDLE_ANIM()
{
	// Do Nothing...
}


//-----------------------------------------------------------------------------
//	Name:		STREAM_HANDLE_ANIM destructor
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_ANIM::~STREAM_HANDLE_ANIM()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamTrack
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK *	STREAM_HANDLE_ANIM::GetStreamTrack(int		nNum,
												   float	rFrame)
{
	ANIM_BLOCK		*pB;
	STREAM_TRACK	*pT;

	//	To do : include num frame in the process
//	UpdateCurrentBlock( rFrame );
	pB = GetCurrentAnimBlock();
	if ( pB )
	{
		pT = pB->GetTrack( nNum );
	}
	else
	{
		return ( NULL );
	}

	return ( pT );
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentAnimBlock
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_BLOCK *	STREAM_HANDLE_ANIM::GetCurrentAnimBlock()
{
	ANIM_BLOCK		*pB;

	pB = static_cast<ANIM_BLOCK*> ( GetCurrentBlock() );
	return ( pB );
}

//-----------------------------------------------------------------------------
//	Name:		Play
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_ANIM::Play()
{

}

//-----------------------------------------------------------------------------
//	Name:		Stop
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_ANIM::Stop()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		Pause
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_ANIM::Pause()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_ANIM::Resume()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		IsPlaying
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE_ANIM::IsPlaying()
{
	
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		PrepareBlock
//	Object:		
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_ANIM::PrepareBlock(float	rFrame, STREAM_ANIM_WRAPPER *pSAW )
{
	STREAMABLE			*pS;
	float				rTime;
	int					nID;
	bool				bPreLoad = true;
	bool				bLoaded;
	STREAM_BLOCK		*pB;
	int					nCurID = -1;


	//////////////////////////////////////////////////////////
	if ( GetIsManaged() )
	{
		rTime = GetTheCurrentTime();
	}
	else
	{
		rTime	= rFrame / 30.0f;
		SetCurrentTime( rTime );
	}
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	pS = static_cast<STREAMABLE*> ( _Anim.GetCommunicator() );
	nID		= pS->GetBlockIndexFromTime( rTime );
	//////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////
	if ( GetCurrentBlock() )
	{
		nCurID = GetCurrentBlock()->GetID();
	}
	if ( nID == nCurID )	return;	// block already set
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	if ( GetIsManaged() )
	{
		pB = pS->GetBlock( nID );
	}
	else
	{
		pB = pS->QueryBlock( nID, bPreLoad, bLoaded );
	}
	if ( !pB )	return;
	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////
	if ( GetCurrentBlock() )
	{
		pS->ReleaseBlock( GetCurrentBlock()->GetID() );

	}
	SetCurrentBlock( pB );
	QDT_ASSERT( ((ANIM_BLOCK*)pB)->GetTrack( 0 ) );
	pSAW->ResetTracks();
	//////////////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		SetAnim
//	Object:		
//	03-06-23:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE_ANIM::SetAnim(const COM_INSTANCE &	CI)
{
	_Anim = CI;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
