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
//	CLASS:	STREAM_BLOCK
//
//	03-05-19:	AMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Streaming/StreamBlock)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_BLOCK constructor
//	Object:		
//	03-06-03:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK::STREAM_BLOCK()
:
_nID		( -1 ),
_nNbrRef	( 0 ),
_rStartTime	( 0 ),
_rEndTime	( 0 ),
_pBuffer	( NULL ),
_nSize		( 0 )
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_BLOCK constructor
//	Object:		
//	03-06-10:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK::STREAM_BLOCK(int	nID)
:
_nID		( nID ),
_nNbrRef	( 0 ),
_rStartTime	( 0 ),
_rEndTime	( 0 ),
_pBuffer	( NULL ),
_nSize		( 0 )

{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_BLOCK constructor
//	Object:		
//	03-05-19:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK::STREAM_BLOCK(int			nID,
						   float		rStartTime,
						   float		rEndTime,
						   const char *	pBuffer,
						   int			nSize)
:
_nID		( nID ),
_nNbrRef	( 0 ),
_rStartTime	( rStartTime ),
_rEndTime	( rEndTime ),
_pBuffer	( pBuffer ),
_nSize		( nSize )
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		~STREAM_BLOCK destructor
//	Object:		
//	03-05-19:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_BLOCK::~STREAM_BLOCK()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		GetID
//	Object:		
//	03-05-19:	AMA - Created
//-----------------------------------------------------------------------------
int	STREAM_BLOCK::GetID()
{
	return ( _nID );
}

//-----------------------------------------------------------------------------
//	Name:		SetID
//	Object:		
//	03-06-03:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_BLOCK::SetID(int	nID)
{
	_nID = nID;
}

//-----------------------------------------------------------------------------
//	Name:		IncreaseReference
//	Object:		
//	03-05-19:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_BLOCK::IncreaseReference()
{
	++_nNbrRef;
}

//-----------------------------------------------------------------------------
//	Name:		DecreaseReference
//	Object:		
//	03-05-19:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_BLOCK::DecreaseReference()
{
	--_nNbrRef;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrReference
//	Object:		
//	03-05-19:	AMA - Created
//-----------------------------------------------------------------------------
int	STREAM_BLOCK::GetNbrReference()
{
	return (_nNbrRef);
}

//-----------------------------------------------------------------------------
//	Name:		GetStartTime
//	Object:		
//	03-05-27:	AMA - Created
//-----------------------------------------------------------------------------
float	STREAM_BLOCK::GetStartTime()
{
	return (_rStartTime);
}

//-----------------------------------------------------------------------------
//	Name:		GetEndTime
//	Object:		
//	03-05-27:	AMA - Created
//-----------------------------------------------------------------------------
float	STREAM_BLOCK::GetEndTime()
{
	return (_rEndTime);
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
const char *	STREAM_BLOCK::GetBuffer()
{
	return (_pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferSize
//	Object:		
//	03-06-12:	AMA - Created
//-----------------------------------------------------------------------------
int	STREAM_BLOCK::GetBufferSize()
{
	return (_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetBufferPadding
//	Object:		
//	03-06-26:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_BLOCK::SetBufferPadding(int	nOffset)
{
	_pBuffer	+= nOffset;
	_nSize		-= nOffset;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendingTime
//	Object:		
//	04-03-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAM_BLOCK::SetBlendingTime(float	r)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetLastBlendingTime
//	Object:		
//	04-03-05:	ELE - Created
//-----------------------------------------------------------------------------
float	STREAM_BLOCK::GetLastBlendingTime() const
{
	return	(0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GetLastFrameWeights
//	Object:		
//	04-03-04:	ELE - Created
//-----------------------------------------------------------------------------
float *	STREAM_BLOCK::GetLastFrameWeights()
{
	return	(NULL);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
