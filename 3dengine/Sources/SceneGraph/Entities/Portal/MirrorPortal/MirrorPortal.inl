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
// CLASS: MIRROR_PORTAL
//
//
//	01-03-29:	ELE - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		DisplayMirror
//	Object:		Display the sorted mirror
//	01-08-06:	ELE - Created
//-----------------------------------------------------------------------------
void	MIRROR_PORTAL::DisplayMirror(DISPLAY_PARAMETERS &	DisplayParameters)
{
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	if	(pContext->IsWireFrameEnabled())
	{
		pContext->EnableWireFrame(false);
		pContext->EnableColorWrite(false);

		PORTAL_ABC::Display(DisplayParameters);

		pContext->EnableWireFrame(true);
		pContext->EnableColorWrite(true);
	}

	PORTAL_ABC::Display(DisplayParameters);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================