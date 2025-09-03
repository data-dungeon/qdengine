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
// CLASS: RENDER_TARGET_D3D9_PC
//
//
//	05-10-24:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	"RenderTarget_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)
#include	INCL_KRENDERER(Renderer/D3D9_PC/Texture_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

QDT_DLIST<RENDER_TARGET_D3D9_PC *> RENDER_TARGET_D3D9_PC::_RenderTargetList;

//-----------------------------------------------------------------------------
//	Name:		RENDER_TARGET_D3D9_PC constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_D3D9_PC::RENDER_TARGET_D3D9_PC()
:
_pD3DSurface(NULL)
{
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_TARGET_D3D9_PC constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_D3D9_PC::RENDER_TARGET_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext,
											 UInt32						uiWidth,
											 UInt32						uiHeight,
											 PIXEL_FORMAT				eFormat)
:
_pD3DSurface(NULL),
_Texture(pRenderContext, uiWidth, uiHeight, eFormat, 0, true)
{
	_Texture.SetAddressUType(AT_CLAMP);
	_Texture.SetAddressVType(AT_CLAMP);
	CreateRenderTarget();
	_RenderTargetList.PushTail(this);
}

//-----------------------------------------------------------------------------
//	Name:		~RENDER_TARGET_D3D9_PC destructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_D3D9_PC::~RENDER_TARGET_D3D9_PC()
{
	_RenderTargetList.Remove(this);
	ReleaseRenderTarget();
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderTarget
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_TARGET_D3D9_PC::CreateRenderTarget()
{
	if (_Texture._eFormat == PF_D24S8_DEPTHSTENCIL)
	{
		// In this format, we have a pure DirectX render target (the texture is empty)
		_Texture._pRenderContext->Device()->CreateDepthStencilSurface(
			_Texture.GetWidth(), _Texture.GetHeight(),
			_Texture._pRenderContext->GetD3DPixelFormat(_Texture.GetPixelFormat()),
			D3DMULTISAMPLE_NONE,
			0, TRUE,
			&_pD3DSurface,
			NULL);
	}
	else
	{
		_Texture._pD3DTexture->GetSurfaceLevel(0, &_pD3DSurface);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseRenderTarget
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_TARGET_D3D9_PC::ReleaseRenderTarget()
{
	if(_pD3DSurface)
	{
        UInt32 nCount = _pD3DSurface->Release();
		QDT_ASSERT(nCount == 1 || nCount == 0); // Should be released with the destruction
												// of the texture in some cases
		_pD3DSurface = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DTexture
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
const TEXTURE_D3D9_PC *	RENDER_TARGET_D3D9_PC::GetD3DTexture() const
{
	return (&_Texture);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DSurface
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
IDirect3DSurface9 *	RENDER_TARGET_D3D9_PC::GetD3DSurface() const
{
	return (_pD3DSurface);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_TARGET_D3D9_PC::GetWidth() const
{
	return (_Texture.GetWidth());
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_TARGET_D3D9_PC::GetHeight() const
{
	return (_Texture.GetHeight());
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT	RENDER_TARGET_D3D9_PC::GetPixelFormat() const
{
	return (_Texture.GetPixelFormat());
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseRenderTargets
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_TARGET_D3D9_PC::ReleaseRenderTargets()
{
	QDT_DLIST<RENDER_TARGET_D3D9_PC *>::ITERATOR it = _RenderTargetList.GetHead();
	QDT_DLIST<RENDER_TARGET_D3D9_PC *>::ITERATOR tail = _RenderTargetList.GetTail();

	while (it != tail)
	{
		RENDER_TARGET_D3D9_PC * pRT = *it;
		pRT->ReleaseRenderTarget();
		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RecreateRenderTargets
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_TARGET_D3D9_PC::RecreateRenderTargets()
{
	QDT_DLIST<RENDER_TARGET_D3D9_PC *>::ITERATOR it = _RenderTargetList.GetHead();
	QDT_DLIST<RENDER_TARGET_D3D9_PC *>::ITERATOR tail = _RenderTargetList.GetTail();

	while (it != tail)
	{
		RENDER_TARGET_D3D9_PC * pRT = *it;
		pRT->CreateRenderTarget();
		++it;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================