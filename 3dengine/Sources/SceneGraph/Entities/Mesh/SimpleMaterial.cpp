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
//	CLASS:	SIMPLE_MATERIAL
//
//	05-10-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/SimpleMaterial)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/SimpleTexture)

#include	INC_KRNRENDERER(Renderer)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SIMPLE_MATERIAL constructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
SIMPLE_MATERIAL::SIMPLE_MATERIAL()
:
_pVertexShader	(NULL),
_pPixelShader	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SIMPLE_MATERIAL destructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
SIMPLE_MATERIAL::~SIMPLE_MATERIAL()
{
	if	(_pVertexShader)
	{
		RENDER_CONTEXT::ReleaseVertexShader(_pVertexShader);
	}

	if	(_pPixelShader)
	{
		RENDER_CONTEXT::ReleasePixelShader(_pPixelShader);
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitDisplay
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	SIMPLE_MATERIAL::InitDisplay() const
{
	RENDER_CONTEXT::SetVertexShader(_pVertexShader);
	RENDER_CONTEXT::SetPixelShader(_pPixelShader);
	RENDER_CONTEXT::SetTexture(0, _pTexture->GetTexture());
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexShader
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
const I_VERTEX_SHADER *	SIMPLE_MATERIAL::GetVertexShader() const
{
	return	(_pVertexShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelShader
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
const I_PIXEL_SHADER *	SIMPLE_MATERIAL::GetPixelShader() const
{
	return	(_pPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetTexture
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
const SIMPLE_TEXTURE *	SIMPLE_MATERIAL::GetTexture() const
{
	return	(_pTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	SIMPLE_MATERIAL::SetVertexShader(I_VERTEX_SHADER *	pVertexShader)
{
	_pVertexShader = pVertexShader;
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShader
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	SIMPLE_MATERIAL::SetPixelShader(I_PIXEL_SHADER *	pPixelShader)
{
	_pPixelShader = pPixelShader;
}

//-----------------------------------------------------------------------------
//	Name:		SetTexture
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	SIMPLE_MATERIAL::SetTexture(SIMPLE_TEXTURE *	pTexture)
{
	_pTexture = pTexture;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
