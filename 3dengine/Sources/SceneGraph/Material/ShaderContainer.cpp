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
//	CLASS:	SHADER_CONTAINER
//
//	05-12-13:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Material/ShaderContainer)
#include	INCL_KRENDERER(Renderer/RenderContext)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SHADER_CONTAINER Constructor
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_CONTAINER::SHADER_CONTAINER()
:
_uiDescription(0),
_pPixelShader(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SHADER_CONTAINER Destructor
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
SHADER_CONTAINER::~SHADER_CONTAINER()
{
	if (_pPixelShader)
	{
		RENDER_CONTEXT::ReleasePixelShader(_pPixelShader);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDescription
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SHADER_CONTAINER::GetDescription() const
{
	return (_uiDescription);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetDescription
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER * SHADER_CONTAINER::GetPixelShader() const
{
	return (_pPixelShader);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetDescription
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CONTAINER::SetPixelShader(I_PIXEL_SHADER * pShader)
{
	_pPixelShader = pShader;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetDescriptor
//	Object:		
//	05-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void SHADER_CONTAINER::SetDescription(UInt32 uiDescription)
{
	_uiDescription = uiDescription;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
