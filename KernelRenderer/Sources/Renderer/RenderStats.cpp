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
// CLASS: RENDER_STATS
//
//
//	06-03-24:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"RenderStats.h"

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RENDER_STATS constructor
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
RENDER_STATS::RENDER_STATS()
{
	_uiNbrOfVertexBuffers = 0;
	_uiSizeOfVertexBuffers = 0;

	_uiNbrOfIndexBuffers = 0;
	_uiSizeOfIndexBuffers = 0;

	_uiNbrOfRenderTargets = 0;
	_uiSizeOfRenderTargets = 0;

	_uiNbrOfTextures = 0;
	_uiSizeOfTextures = 0;

	_uiNbrOfCubeRenderTargets = 0;
	_uiSizeOfCubeRenderTargets = 0;

	_uiNbrOfCubeTextures = 0;
	_uiSizeOfCubeTextures = 0;

	_uiNbrOfVolumeTextures = 0;
	_uiSizeOfVolumeTextures = 0;

	_uiNbrOfVertexShaders = 0;
	_uiSizeOfVertexShaders = 0;

	_uiNbrOfPixelShaders = 0;
	_uiSizeOfPixelShaders = 0;

	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_STATS::Clear()
{
	_uiNbrOfDrawPrimitive = 0;
	_uiNbrOfPolygones = 0;
	_uiNbrOfVertexBufferLock = 0;
	_uiNbrOfIndexBufferLock = 0;
	_uiNbrOfTextureLock = 0;
	_uiNbrOfSetRenderTarget = 0;
	_uiNbrOfSetDepthBuffer = 0;
	_uiNbrOfSetTexture = 0;
	_uiNbrOfSetPixelShaderConstant = 0;
	_uiNbrOfSetVertexShaderConstant = 0;
	_uiNbrOfSetPixelShader = 0;
	_uiNbrOfSetVertexShader = 0;
	_uiNbrOfSetVertexBuffer = 0;
	_uiNbrOfSetIndexBuffer = 0;

	_fDrawPrimitiveTime = 0.0f;
	_fSetPixelShaderConstantTime = 0.0f;
	_fSetVertexShaderConstantTime = 0.0f;
	_fSetRenderTargetTime = 0.0f;
	_fSetDepthBufferTime = 0.0f;
	_fSetTextureTime = 0.0f;
	_fSetPixelShaderTime = 0.0f;
	_fSetVertexShaderTime = 0.0f;
	_fSetVertexBufferTime = 0.0f;
	_fSetIndexBufferTime = 0.0f;
	_fPresentTime = 0.0f;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
