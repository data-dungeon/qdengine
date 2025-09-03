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
// CLASS: VOLUME_TEXTURE_D3D9_PC
//
//
//	05-10-12:	SBE - Created
//*****************************************************************************

#include	"Root.h"
#include	"VolumeTexture_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VOLUME_TEXTURE_D3D9_PC Constructor
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
VOLUME_TEXTURE_D3D9_PC::VOLUME_TEXTURE_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext, 
								 UInt32						uiWidth,						
								 UInt32						uiHeight,							
								 UInt32						uiDepth,	
								 PIXEL_FORMAT				eFormat,
								 UInt32						nMipMapLevels)
:
_pRenderContext(pRenderContext),
_pD3DTexture(NULL),
_uiWidth(uiWidth),
_uiHeight(uiHeight),
_uiDepth(uiDepth),
_eFormat(eFormat),
_nMipMapLevels(nMipMapLevels),
_bLocked(false),
_AddressUType(AT_WRAP),
_AddressVType(AT_WRAP),
_FilterType(FT_TRILINEAR)
{
	CreateTexture();
}

//-----------------------------------------------------------------------------
//	Name:		VOLUME_TEXTURE_D3D9_PC Destructor
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
VOLUME_TEXTURE_D3D9_PC::~VOLUME_TEXTURE_D3D9_PC()
{
	ReleaseTexture();
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
void VOLUME_TEXTURE_D3D9_PC::CreateTexture()
{
	HRESULT hRes;

	QDT_ASSERT(_pRenderContext);

	static D3DFORMAT D3DFormatsArray[11] = 
	{
		D3DFMT_A8R8G8B8,
		D3DFMT_X8R8G8B8,
		D3DFMT_A16B16G16R16F,
		D3DFMT_A32B32G32R32F,
		D3DFMT_R16F,
		D3DFMT_DXT1,
		D3DFMT_DXT3,
		D3DFMT_DXT5,
		D3DFMT_D24S8,
		D3DFMT_D16,
		D3DFMT_L8
	};

	D3DFORMAT D3DFormat = D3DFormatsArray[_eFormat];

	hRes = _pRenderContext->Device()->CreateVolumeTexture(
				_uiWidth, 
				_uiHeight, 
				_uiDepth,
				_nMipMapLevels, 
				0, 
				D3DFormat,
				D3DPOOL_MANAGED, 
				&_pD3DTexture, 
				NULL);

	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(_pD3DTexture);
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
void VOLUME_TEXTURE_D3D9_PC::ReleaseTexture()
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
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
void * VOLUME_TEXTURE_D3D9_PC::Lock(UInt32 nMipMapLevel,
									UInt32 nCubeFace)
{
	QDT_ASSERT(_pD3DTexture);
	QDT_ASSERT(_bLocked == false);

	Char * pDatas;
	D3DLOCKED_BOX	Box;
	HRESULT			hRes;

	hRes = _pD3DTexture->LockBox(nMipMapLevel, &Box, NULL, 0);
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = true;
	pDatas = (Char *) Box.pBits;
	pDatas += Box.SlicePitch * nCubeFace;

	return (static_cast<void*>(pDatas));
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
void VOLUME_TEXTURE_D3D9_PC::Unlock(UInt32 uiMipMapLevel,
							 UInt32 uiCubeFace)
{
	QDT_ASSERT(_pD3DTexture);
	QDT_ASSERT(_bLocked == true);

	HRESULT hRes;

	hRes = _pD3DTexture->UnlockBox(uiMipMapLevel);
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = false;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
Bool VOLUME_TEXTURE_D3D9_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32 VOLUME_TEXTURE_D3D9_PC::GetWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32 VOLUME_TEXTURE_D3D9_PC::GetHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetDepth
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32 VOLUME_TEXTURE_D3D9_PC::GetDepth() const
{
	return (_uiDepth);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT VOLUME_TEXTURE_D3D9_PC::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
IDirect3DVolumeTexture9 * VOLUME_TEXTURE_D3D9_PC::GetD3DTexture() const
{
	return (_pD3DTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------
void VOLUME_TEXTURE_D3D9_PC::SetAddressUType(ADDRESS_TYPE eAddressType)
{
	_AddressUType = eAddressType;
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------
void VOLUME_TEXTURE_D3D9_PC::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	_AddressVType = eAddressType;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------
void VOLUME_TEXTURE_D3D9_PC::SetFilterType(FILTER_TYPE eFilterType)
{
	_FilterType = eFilterType;
}

//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------
ADDRESS_TYPE VOLUME_TEXTURE_D3D9_PC::GetAddressUType() const
{
	return (_AddressUType);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------
ADDRESS_TYPE VOLUME_TEXTURE_D3D9_PC::GetAddressVType() const
{
	return (_AddressVType);
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------
FILTER_TYPE VOLUME_TEXTURE_D3D9_PC::GetFilterType() const
{
	return (_FilterType);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================