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
//	CLASS:	PAD_MANAGER_X360
//
//	06-29-06:	VMA - Created
//						GRI : rajout des deep-switch. il faudrait renommer cette classe 
//						en INPUT_X360_MANAGER
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/X360/PadManagerX360)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

PAD_MANAGER_X360 PAD_MANAGER_X360::sPadManager;

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_X360::Init()
{
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_X360::Close()
{
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_X360::Update()
{
	for (UInt32 iPad=0; iPad<MAX_PAD; ++iPad)
	{
		_Pads[iPad].Update();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPad
//	Object:		
//	05-10-05:	VMA - Created
//------------------------------------------------------	-----------------------
PAD_X360 *	PAD_MANAGER_X360::GetPad(UInt32	i)
{
	_Pads[i]._bUsed = true;
	return &_Pads[i]; 
}

//-----------------------------------------------------------------------------
//	Name:		ReleasePad
//	Object:		
//	05-10-05:	VMA - Created
//-----------------------------------------------------------------------------
void	PAD_MANAGER_X360::ReleasePad(UInt32	i)
{
	_Pads[i]._bUsed = false;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
