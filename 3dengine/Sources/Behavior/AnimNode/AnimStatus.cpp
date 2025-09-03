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
//	CLASS:	ANIM_STATUS
//
//	01-10-11:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	"AnimNodeStruct.h"

#ifdef _DEBUG
	#include "AnimStatus.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ANIM_STATUS constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_STATUS::ANIM_STATUS()
{
	SetRelative( true );
	SetTeleport( false );
	SetLoop( false );
	SetPingPong( false );
	SetPreemptive( false );
	SetMix( false );
	SetDelegateBlended( true );
	EnableDeletation( true );
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	03-06-12:	CVI - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::Reset()
{
	SetRelative( true );
	SetTeleport( false );
	SetLoop( false );
	SetPingPong( false );
	SetPreemptive( false );
	SetMix( false );
	SetDelegateBlended( true );
	EnableDeletation( true );

}



//-----------------------------------------------------------------------------
//	Name:		ANIM_STATUS constructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_STATUS::ANIM_STATUS(const ANIM_STATUS & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		ANIM_STATUS destructor
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_STATUS::~ANIM_STATUS()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
ANIM_STATUS &	ANIM_STATUS::operator=(const ANIM_STATUS & C)
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
//	01-10-11:	cvi - Created
//-----------------------------------------------------------------------------
void	ANIM_STATUS::Copy(const ANIM_STATUS & C)
{
	_nFlags = C._nFlags;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
