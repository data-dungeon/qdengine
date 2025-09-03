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
// CLASS: CUBE_RENDER_TARGET_D3D9_PC
//
//
//	05-11-16:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	"CubeRenderTarget_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

QDT_DLIST<CUBE_RENDER_TARGET_D3D9_PC *> CUBE_RENDER_TARGET_D3D9_PC::_RenderTargetList;

//-----------------------------------------------------------------------------
//	Name:		CUBE_RENDER_TARGET_D3D9_PC constructor
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_D3D9_PC::CUBE_RENDER_TARGET_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext,
													   UInt32					uiEdgeLength,
													   PIXEL_FORMAT				eFormat)
:
_Texture(pRenderContext, uiEdgeLength, eFormat, 1, true)
{
	_Texture.SetAddressUType(AT_CLAMP);
	_Texture.SetAddressVType(AT_CLAMP);
	CreateRenderTarget();
	_RenderTargetList.PushTail(this);
}

//-----------------------------------------------------------------------------
//	Name:		~CUBE_RENDER_TARGET_D3D9_PC destructor
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_D3D9_PC::~CUBE_RENDER_TARGET_D3D9_PC()
{
	_RenderTargetList.Remove(this);
	ReleaseRenderTarget();
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderTarget
//	Object:		
//	06-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CUBE_RENDER_TARGET_D3D9_PC::CreateRenderTarget()
{
	for (UInt32 iCubeFace=0; iCubeFace<6; iCubeFace++)
	{
		_Texture._pD3DTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)iCubeFace, 0, &_pD3DSurfaces[iCubeFace]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseRenderTarget
//	Object:		
//	06-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CUBE_RENDER_TARGET_D3D9_PC::ReleaseRenderTarget()
{
	for (UInt32 iCubeFace=0; iCubeFace<6; iCubeFace++)
	{
		if (_pD3DSurfaces[iCubeFace])
		{
			UInt32 uiCount = _pD3DSurfaces[iCubeFace]->Release();
			_pD3DSurfaces[iCubeFace] = NULL;
		}
	}	
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DTexture
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
const CUBE_TEXTURE_D3D9_PC *	CUBE_RENDER_TARGET_D3D9_PC::GetD3DTexture() const
{
	return &_Texture;
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DSurface
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
IDirect3DSurface9 *	CUBE_RENDER_TARGET_D3D9_PC::GetD3DSurface(CUBEMAP_FACE CF) const
{
	QDT_ASSERT(CF != CF_NONE);

	return _pD3DSurfaces[CF];
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_RENDER_TARGET_D3D9_PC::GetWidth() const
{
	return _Texture.GetWidth();
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_RENDER_TARGET_D3D9_PC::GetHeight() const
{
	return _Texture.GetHeight();
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT	CUBE_RENDER_TARGET_D3D9_PC::GetPixelFormat() const
{
	return _Texture.GetPixelFormat();
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseRenderTargets
//	Object:		
//	06-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CUBE_RENDER_TARGET_D3D9_PC::ReleaseRenderTargets()
{
	QDT_DLIST<CUBE_RENDER_TARGET_D3D9_PC *>::ITERATOR it = _RenderTargetList.GetHead();
	QDT_DLIST<CUBE_RENDER_TARGET_D3D9_PC *>::ITERATOR tail = _RenderTargetList.GetTail();

	while (it != tail)
	{
		CUBE_RENDER_TARGET_D3D9_PC * pRT = *it;
		pRT->ReleaseRenderTarget();
		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RecreateRenderTargets
//	Object:		
//	06-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CUBE_RENDER_TARGET_D3D9_PC::RecreateRenderTargets()
{
	QDT_DLIST<CUBE_RENDER_TARGET_D3D9_PC *>::ITERATOR it = _RenderTargetList.GetHead();
	QDT_DLIST<CUBE_RENDER_TARGET_D3D9_PC *>::ITERATOR tail = _RenderTargetList.GetTail();

	while (it != tail)
	{
		CUBE_RENDER_TARGET_D3D9_PC * pRT = *it;
		pRT->CreateRenderTarget();
		++it;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================