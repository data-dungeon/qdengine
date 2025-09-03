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
//	CLASS:	STR_ANIM
//
//	03-06-11:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	"StrAnim.h"

#ifdef _DEBUG
	#include "StrAnim.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STR_ANIM constructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM::STR_ANIM()
:
_pChildrenOutput(NULL),
_nNbrChildren ( 0 ),
_bNotifyCollideForTeleport( false )
{
}

//-----------------------------------------------------------------------------
//	Name:		STR_ANIM constructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM::STR_ANIM(const STR_ANIM & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		STR_ANIM destructor
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM::~STR_ANIM()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
STR_ANIM &	STR_ANIM::operator=(const STR_ANIM & C)
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
//	03-06-11:	CVI - Created
//-----------------------------------------------------------------------------
void	STR_ANIM::Copy(const STR_ANIM & C)
{
	_Own						= C._Own;
	_pChildrenOutput			= C._pChildrenOutput;
	_nNbrChildren				= C._nNbrChildren;
	_bNotifyCollideForTeleport	= C._bNotifyCollideForTeleport;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
