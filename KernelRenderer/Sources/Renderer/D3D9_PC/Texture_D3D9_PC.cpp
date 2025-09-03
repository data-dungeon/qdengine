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
// CLASS: TEXTURE_D3D9_PC
//
//
//	05-09-01:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"Texture_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

QDT_DLIST<TEXTURE_D3D9_PC *> TEXTURE_D3D9_PC::_TextureList;

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_D3D9_PC constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_D3D9_PC::TEXTURE_D3D9_PC()
:
_pRenderContext(NULL),
_pD3DTexture(NULL),
_uiWidth(0),
_uiHeight(0),
_nMipMapLevels(0),
_bLocked(false),
_AddressUType(AT_WRAP),
_AddressVType(AT_WRAP),
_FilterType(FT_TRILINEAR)
{
	_TextureList.PushTail(this);
}

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_D3D9_PC Constructor
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
TEXTURE_D3D9_PC::TEXTURE_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext, 
								 UInt32						nWidth,						
								 UInt32						nHeight,							
								 PIXEL_FORMAT				eFormat,
								 UInt32						nMipMapLevels,
								 Bool						bRenderTarget)
:
_pRenderContext(pRenderContext),
_pD3DTexture(NULL),
_uiWidth(nWidth),
_uiHeight(nHeight),
_eFormat(eFormat),
_nMipMapLevels(nMipMapLevels),
_bLocked(false),
_AddressUType(AT_WRAP),
_AddressVType(AT_WRAP),
_FilterType(FT_TRILINEAR),
_bRenderTarget(bRenderTarget)
{
	CreateTexture();
	_TextureList.PushTail(this);
}

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_D3D9_PC Destructor
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
TEXTURE_D3D9_PC::~TEXTURE_D3D9_PC()
{
	_TextureList.Remove(this);
	ReleaseTexture();
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
void TEXTURE_D3D9_PC::CreateTexture()
{
	HRESULT hRes;

	QDT_ASSERT(_pRenderContext);

	D3DFORMAT D3DFormat = _pRenderContext->GetD3DPixelFormat(_eFormat);

	if (_bRenderTarget)
	{
		if (_eFormat == PF_D24S8_DEPTHSTENCIL)
		{
			// Do nothing (special case for render target)
		}
		else if (_eFormat == PF_D24S8)
		{
			hRes =_pRenderContext->Device()->CreateTexture(
				_uiWidth, _uiHeight,
				1, D3DUSAGE_DEPTHSTENCIL,
				_pRenderContext->GetD3DPixelFormat(_eFormat),
				D3DPOOL_DEFAULT,
				&_pD3DTexture,
				NULL);

			QDT_ASSERT(hRes == D3D_OK);
			QDT_ASSERT(_pD3DTexture);
		}
		else
		{
			hRes =_pRenderContext->Device()->CreateTexture(_uiWidth, _uiHeight,
				1, D3DUSAGE_RENDERTARGET,
				_pRenderContext->GetD3DPixelFormat(_eFormat),
				D3DPOOL_DEFAULT,
				&_pD3DTexture,
				NULL);
			
			QDT_ASSERT(hRes == D3D_OK);
			QDT_ASSERT(_pD3DTexture);
		}
	}
	else
	{
		hRes = _pRenderContext->Device()->CreateTexture(
			_uiWidth, 
			_uiHeight, 
			_nMipMapLevels, 
			0, 
			D3DFormat,
			D3DPOOL_MANAGED, 
			&_pD3DTexture, 
			NULL);

		QDT_ASSERT(hRes == D3D_OK);
		QDT_ASSERT(_pD3DTexture);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseTexture
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_D3D9_PC::ReleaseTexture()
{
	if(_pD3DTexture)
	{
        UInt32 nCount = _pD3DTexture->Release();
		QDT_ASSERT(nCount == 0);
		_pD3DTexture = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void * TEXTURE_D3D9_PC::Lock(UInt32 nMipMapLevel,
							 UInt32 uiCubeFace)
{
	QDT_ASSERT(_pD3DTexture);
	QDT_ASSERT(_bLocked == false);
	QDT_ASSERT(uiCubeFace == 0);

	D3DLOCKED_RECT	Rect;
	HRESULT			hRes;

	hRes = _pD3DTexture->LockRect(nMipMapLevel, &Rect, NULL, 0);
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = true;

	return (static_cast<void*>(Rect.pBits));
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_D3D9_PC::Unlock(UInt32 uiMipMapLevel,
							 UInt32 uiCubeFace)
{
	QDT_ASSERT(_pD3DTexture);
	QDT_ASSERT(_bLocked == true);
	QDT_ASSERT(uiCubeFace == 0);

	HRESULT hRes;

	hRes = _pD3DTexture->UnlockRect(uiMipMapLevel);
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = false;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
Bool TEXTURE_D3D9_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 TEXTURE_D3D9_PC::GetWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 TEXTURE_D3D9_PC::GetHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT TEXTURE_D3D9_PC::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DTexture
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------
IDirect3DTexture9 * TEXTURE_D3D9_PC::GetD3DTexture() const
{
	return (_pD3DTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_D3D9_PC::SetAddressUType(ADDRESS_TYPE eAddressType)
{
	_AddressUType = eAddressType;
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_D3D9_PC::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	_AddressVType = eAddressType;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void TEXTURE_D3D9_PC::SetFilterType(FILTER_TYPE eFilterType)
{
	_FilterType = eFilterType;
}

//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
ADDRESS_TYPE TEXTURE_D3D9_PC::GetAddressUType() const
{
	return (_AddressUType);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
ADDRESS_TYPE TEXTURE_D3D9_PC::GetAddressVType() const
{
	return (_AddressVType);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
FILTER_TYPE TEXTURE_D3D9_PC::GetFilterType() const
{
	return (_FilterType);
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseDynamicTextures
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_D3D9_PC::ReleaseDynamicTextures()
{
	QDT_DLIST<TEXTURE_D3D9_PC *>::ITERATOR it = _TextureList.GetHead();
	QDT_DLIST<TEXTURE_D3D9_PC *>::ITERATOR tail = _TextureList.GetTail();

	while (it != tail)
	{
		TEXTURE_D3D9_PC * pTex = *it;
		if (pTex->_bRenderTarget)
		{
			pTex->ReleaseTexture();
		}
		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RecreateDynamicTextures
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	TEXTURE_D3D9_PC::RecreateDynamicTextures()
{
	QDT_DLIST<TEXTURE_D3D9_PC *>::ITERATOR it = _TextureList.GetHead();
	QDT_DLIST<TEXTURE_D3D9_PC *>::ITERATOR tail = _TextureList.GetTail();

	while (it != tail)
	{
		TEXTURE_D3D9_PC * pTex = *it;
		if (pTex->_bRenderTarget)
		{
			pTex->CreateTexture();
		}
		++it;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================