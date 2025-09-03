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
//	CLASS:	VOLUME_TEXTURE_D3D9_PC
//	The VOLUME_TEXTURE_D3D9_PC class implements ...
//
//	05-10-12:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_VOLUME_TEXTURE_D3D9_PC_H__
#define __KRENDERER_RENDERER_VOLUME_TEXTURE_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer\ITexture)

namespace	QDT
{
	namespace	KRENDERER
	{
		class VOLUME_TEXTURE_D3D9_PC : public I_TEXTURE
		{
		public:

			VOLUME_TEXTURE_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext,
				UInt32					uiWidth,
				UInt32					uiHeight,
				UInt32					uiDepth,
				PIXEL_FORMAT			eFormat,
				UInt32					nMipMapLevels);
			~VOLUME_TEXTURE_D3D9_PC();

			// Derivation from I_TEXTURE
			void *			Lock(UInt32 nMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X);
			void			Unlock(UInt32 nMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X);
			Bool			IsLocked() const;
			UInt32			GetWidth() const;
			UInt32			GetHeight() const;
			UInt32			GetDepth() const;
			PIXEL_FORMAT	GetPixelFormat() const;
			void			SetAddressUType(ADDRESS_TYPE eAddressType);
			void			SetAddressVType(ADDRESS_TYPE eAddressType);
			void			SetFilterType(FILTER_TYPE eFilterType);
			ADDRESS_TYPE	GetAddressUType() const;
			ADDRESS_TYPE	GetAddressVType() const;
			FILTER_TYPE		GetFilterType() const;
			//

			IDirect3DVolumeTexture9 *	GetD3DTexture() const;
			virtual UInt32				GetTarget() const { return 0; }
			virtual void *				GetHandle() const { return GetD3DTexture(); }
			virtual Bool	GetUVNormalized() {return true;};
			virtual UInt32          GetNbrMipMapLevels() {return _nMipMapLevels;};

		private:

			void			CreateTexture();
			void			ReleaseTexture();

			PIXEL_FORMAT				_eFormat;
			UInt32						_nMipMapLevels;
			UInt32						_uiWidth;
			UInt32						_uiHeight;
			UInt32						_uiDepth;
			Bool						_bLocked;
			ADDRESS_TYPE				_AddressUType;
			ADDRESS_TYPE				_AddressVType;
			FILTER_TYPE					_FilterType;
			RENDER_CONTEXT_D3D9_PC *	_pRenderContext;
			IDirect3DVolumeTexture9 *	_pD3DTexture;	
		};
	}
}

#endif // __KRENDERER_RENDERER_VOLUME_TEXTURE_D3D9_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

