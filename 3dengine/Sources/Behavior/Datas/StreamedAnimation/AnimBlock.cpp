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
//	CLASS:	ANIM_BLOCK
//
//	03-05-28:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"AnimBlock.h"

#include	INCL_3DENGINE(Behavior/Datas/StreamedAnimation/StreamTrack)

#ifdef _DEBUG
	#include "AnimBlock.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ANIM_BLOCK constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_BLOCK::ANIM_BLOCK( int nID )
:
STREAM_BLOCK( nID ),
_nNbrTracks( 0 ),
_pTracks( NULL )
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_BLOCK constructor
//	Object:		
//	03-06-20:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_BLOCK::ANIM_BLOCK(int			nID,
					   float		rStartTime,
					   float		rEndTime,
					   const char *	pBuffer,
					   int			nSize)
:
STREAM_BLOCK( nID, rStartTime, rEndTime, pBuffer, nSize ),
_nNbrTracks( 0 ),
_pTracks( NULL )
{
	// Do Nothing...
}


//-----------------------------------------------------------------------------
//	Name:		ANIM_BLOCK destructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
ANIM_BLOCK::~ANIM_BLOCK()
{
	if ( _pTracks )
	{
		delete []_pTracks;
		_pTracks = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitTracks
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_BLOCK::InitTracks( const char *pC )
{
	int				i;
	void			*pBuffer;
	int				*pI;

	//	Get data buffer from stream
	pI		= (int*) ( pC );

	//	Init number of tracks
	_nNbrTracks = *pI;
	pI++;

	//	Init of all tracks
	_pTracks	= new STREAM_TRACK[_nNbrTracks];
	pBuffer		= (void*) pI;
	for ( i = 0; i < _nNbrTracks; ++i )
	{
		//	Return buffer to use as parameter for next track
		pBuffer = _pTracks[i].Init( pBuffer );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetTrack
//	Object:		
//	03-05-30:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK *	ANIM_BLOCK::GetTrack(int	nNum)
{
	//QDT_ASSERT( _pTracks );
	if ( !_pTracks )	return( NULL );
	return ( _pTracks + nNum );
}


//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
MEM_STREAM *		ANIM_BLOCK::GetBuffer()
{
	return ( NULL );
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferSize
//	Object:		
//	03-06-03:	CVI - Created
//-----------------------------------------------------------------------------
int	ANIM_BLOCK::GetBufferSize()
{
	return ( -1 );
}


//-----------------------------------------------------------------------------
//	Name:		FillStream
//	Object:		
//	03-06-20:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_BLOCK::FillStream(MEM_STREAM *	pMemStream,
							   float		rStartTime,
							   float		rEndTime,
							   float		rDuration,
							   bool			bFillWhithBlank,
							   bool			bFillAtBegin,
							   bool			bCompressed)
{
	QDT_FAIL();
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
