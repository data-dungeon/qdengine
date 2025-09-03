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
//	CLASS:	STREAM_HANDLE
//
//	03-05-15:	AMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Streaming/StreamHandle)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_HANDLE constructor
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE::STREAM_HANDLE()
:
_nCurrentID		(-1),
_bPaused		(false),
_pCurrentBlock	(NULL),
_rCurrentTime	(0),
_bIsManaged		(false)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_HANDLE destructor
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE::~STREAM_HANDLE()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentPosition
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::SetCurrentPosition(int	nPosition)
{
	_nCurrentID = nPosition;
}

//-----------------------------------------------------------------------------
//	Name:		IncCurrentPosition
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::IncCurrentPosition()
{
	++_nCurrentID;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentPosition
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
int	STREAM_HANDLE::GetCurrentPosition()
{
	return (_nCurrentID);
}

//-----------------------------------------------------------------------------
//	Name:		IsPaused
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE::IsPaused()
{
	return (_bPaused);
}

//-----------------------------------------------------------------------------
//	Name:		SetPause
//	Object:		
//	03-05-15:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::SetPause(bool	bPause)
{
	_bPaused = bPause;
	if ( bPause )
	{
		Pause();
	}
	else
	{
		Resume();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentBlock
//	Object:		
//	03-05-28:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::SetCurrentBlock(STREAM_BLOCK *	pCurrentBlock)
{
	_pCurrentBlock = pCurrentBlock;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentBlock
//	Object:		
//	03-05-28:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK *	STREAM_HANDLE::GetCurrentBlock()
{
	return (_pCurrentBlock);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentTime
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
float	STREAM_HANDLE::GetTheCurrentTime()
{
	return (_rCurrentTime);
}

//-----------------------------------------------------------------------------
//	Name:		GetIsManaged
//	Object:		
//	03-06-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	STREAM_HANDLE::GetIsManaged()
{
	return (_bIsManaged);
}

//-----------------------------------------------------------------------------
//	Name:		SetIsManaged
//	Object:		
//	03-06-18:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::SetIsManaged(bool	bIsManaged)
{
	_bIsManaged = bIsManaged;
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentTime
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::SetCurrentTime(float	rCurrentTime)
{
	int nIndex, nNbrStreamHandle;
	_rCurrentTime = rCurrentTime;

	nNbrStreamHandle = GetNbrStreamHandle();

	for ( nIndex = 0 ; nIndex < nNbrStreamHandle ; ++nIndex )
	{
		GetStreamHandle(nIndex)->SetCurrentTime(rCurrentTime);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PushStreamHandle
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_HANDLE::PushStreamHandle(STREAM_HANDLE *	pStreamHandle)
{
	_vStreamHandle.PushTail(pStreamHandle);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrStreamHandle
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
int	STREAM_HANDLE::GetNbrStreamHandle()
{
	return (_vStreamHandle.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamHandle
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE *	STREAM_HANDLE::GetStreamHandle(int nIndex)
{
	return (_vStreamHandle[nIndex]);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
