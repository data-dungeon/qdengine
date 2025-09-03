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
// CLASS: CUBE_TEXTURE_D3D9_PC
//
//
//	05-10-11:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	"CubeTexture_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

QDT_DLIST<CUBE_TEXTURE_D3D9_PC *> CUBE_TEXTURE_D3D9_PC::_TextureList;

//-----------------------------------------------------------------------------
//	Name:		CUBE_TEXTURE_D3D9_PC Constructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
CUBE_TEXTURE_D3D9_PC::CUBE_TEXTURE_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext, 
								 UInt32						uiEdgeLength,						
								 PIXEL_FORMAT				eFormat,
								 UInt32						uiMipMapLevels,
								 Bool						bRenderTarget)
:
_pRenderContext(pRenderContext),
_pD3DTexture(NULL),
_uiEdgeLength(uiEdgeLength),
_eFormat(eFormat),
_uiMipMapLevels(uiMipMapLevels),
_bLocked(false),
_bRenderTarget(bRenderTarget),
_AddressUType(AT_WRAP),
_AddressVType(AT_WRAP),
_FilterType(FT_TRILINEAR)
{
	CreateTexture();
	_TextureList.PushTail(this);
}

//-----------------------------------------------------------------------------
//	Name:		CUBE_TEXTURE_D3D9_PC Destructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
CUBE_TEXTURE_D3D9_PC::~CUBE_TEXTURE_D3D9_PC()
{
	_TextureList.Remove(this);
	ReleaseTexture();
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
void CUBE_TEXTURE_D3D9_PC::CreateTexture()
{
	HRESULT hRes;

	QDT_ASSERT(_pRenderContext);

	UInt32 Usage = 0;
	D3DPOOL Pool = D3DPOOL_MANAGED;

	if (_bRenderTarget)
	{
		Usage = D3DUSAGE_RENDERTARGET;
		Pool = D3DPOOL_DEFAULT;
	}

	hRes = _pRenderContext->Device()->CreateCubeTexture(
				_uiEdgeLength, 
				_uiMipMapLevels, 
				Usage, 
				_pRenderContext->GetD3DPixelFormat(_eFormat),
				Pool, 
				&_pD3DTexture, 
				NULL);

	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(_pD3DTexture);
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseTexture
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void CUBE_TEXTURE_D3D9_PC::ReleaseTexture()
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
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void * CUBE_TEXTURE_D3D9_PC::Lock(UInt32 uiMipMapLevel,
								 UInt32 uiCubeFace)
{
	QDT_ASSERT(_pD3DTexture);
	QDT_ASSERT(_bLocked == false);

	D3DLOCKED_RECT	Rect;
	HRESULT			hRes;

	hRes = _pD3DTexture->LockRect((D3DCUBEMAP_FACES) uiCubeFace, uiMipMapLevel, &Rect, NULL, 0);
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = true;

	return (static_cast<void*>(Rect.pBits));
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void CUBE_TEXTURE_D3D9_PC::Unlock(UInt32 uiMipMapLevel,
								 UInt32 uiCubeFace)
{
	QDT_ASSERT(_pD3DTexture);
	QDT_ASSERT(_bLocked == true);

	HRESULT hRes;

	hRes = _pD3DTexture->UnlockRect((D3DCUBEMAP_FACES) uiCubeFace, uiMipMapLevel);
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = false;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
Bool CUBE_TEXTURE_D3D9_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
UInt32 CUBE_TEXTURE_D3D9_PC::GetWidth() const
{
	return (_uiEdgeLength);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
UInt32 CUBE_TEXTURE_D3D9_PC::GetHeight() const
{
	return (_uiEdgeLength);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT CUBE_TEXTURE_D3D9_PC::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DTexture
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
IDirect3DCubeTexture9 * CUBE_TEXTURE_D3D9_PC::GetD3DCubeTexture() const
{
	return (_pD3DTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void CUBE_TEXTURE_D3D9_PC::SetAddressUType(ADDRESS_TYPE eAddressType)
{
	_AddressUType = eAddressType;
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void CUBE_TEXTURE_D3D9_PC::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	_AddressVType = eAddressType;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void CUBE_TEXTURE_D3D9_PC::SetFilterType(FILTER_TYPE eFilterType)
{
	_FilterType = eFilterType;
}

//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
ADDRESS_TYPE CUBE_TEXTURE_D3D9_PC::GetAddressUType() const
{
	return (_AddressUType);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
ADDRESS_TYPE CUBE_TEXTURE_D3D9_PC::GetAddressVType() const
{
	return (_AddressVType);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
FILTER_TYPE CUBE_TEXTURE_D3D9_PC::GetFilterType() const
{
	return (_FilterType);
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseDynamicTextures
//	Object:		
//	06-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CUBE_TEXTURE_D3D9_PC::ReleaseDynamicTextures()
{
	QDT_DLIST<CUBE_TEXTURE_D3D9_PC *>::ITERATOR it = _TextureList.GetHead();
	QDT_DLIST<CUBE_TEXTURE_D3D9_PC *>::ITERATOR tail = _TextureList.GetTail();

	while (it != tail)
	{
		CUBE_TEXTURE_D3D9_PC * pTex = *it;
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
//	06-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	CUBE_TEXTURE_D3D9_PC::RecreateDynamicTextures()
{
	QDT_DLIST<CUBE_TEXTURE_D3D9_PC *>::ITERATOR it = _TextureList.GetHead();
	QDT_DLIST<CUBE_TEXTURE_D3D9_PC *>::ITERATOR tail = _TextureList.GetTail();

	while (it != tail)
	{
		CUBE_TEXTURE_D3D9_PC * pTex = *it;
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