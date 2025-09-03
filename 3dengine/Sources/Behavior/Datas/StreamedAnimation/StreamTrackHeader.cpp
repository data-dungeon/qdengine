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
//	CLASS:	STREAM_TRACK_HEADER
//
//	03-05-28:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamTrackHeader.h"

#ifdef _DEBUG
	#include "StreamTrackHeader.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK_HEADER constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_HEADER::STREAM_TRACK_HEADER()
:
_AbsoluteRot(1.0f, 0.0f, 0.0f, 0.0f ),
_AbsoluteTrans( 0.0f, 0.0f, 0.0f ),
_AbsoluteScale( 1.0f, 1.0f, 1.0f )
{
	
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK_HEADER constructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_HEADER::STREAM_TRACK_HEADER(const STREAM_TRACK_HEADER & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		STREAM_TRACK_HEADER destructor
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_HEADER::~STREAM_TRACK_HEADER()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
STREAM_TRACK_HEADER &	STREAM_TRACK_HEADER::operator=(const STREAM_TRACK_HEADER & C)
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
//	03-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_HEADER::Copy(const STREAM_TRACK_HEADER & C)
{
	_AbsoluteRot	= C._AbsoluteRot;
	_AbsoluteTrans	= C._AbsoluteTrans;
	_AbsoluteScale	= C._AbsoluteScale;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_HEADER::Read(IO_STREAM &	s)
{
	s >> _AbsoluteRot;
	s >> _AbsoluteTrans;
	s >> _AbsoluteScale;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-06-04:	CVI - Created
//-----------------------------------------------------------------------------
void	STREAM_TRACK_HEADER::Write(IO_STREAM &	s)
{
	s << _AbsoluteRot;
	s << _AbsoluteTrans;
	s << _AbsoluteScale;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
