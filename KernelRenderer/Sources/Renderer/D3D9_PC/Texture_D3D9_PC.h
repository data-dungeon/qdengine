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
//	CLASS:	TEXTURE_D3D9_PC
//	The TEXTURE_D3D9_PC class implements ...
//
//	05-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_TEXTURE_D3D9_PC_H__
#define __KRENDERER_RENDERER_TEXTURE_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KCORE(Template\QDT_DList)
#include INCL_KRENDERER(Renderer\ITexture)

namespace	QDT
{
	namespace	KRENDERER
	{
		class TEXTURE_D3D9_PC : public I_TEXTURE
		{
		public:

			TEXTURE_D3D9_PC();
			TEXTURE_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext,
				UInt32					nWidth,
				UInt32					nHeight,
				PIXEL_FORMAT			eFormat,
				UInt32					nMipMapLevels,
				Bool					bRenderTarget = false);
			~TEXTURE_D3D9_PC();

			// Derivation from I_TEXTURE
			void *			Lock(UInt32 nMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X);
			void			Unlock(UInt32 nMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X);
			Bool			IsLocked() const;
			UInt32			GetWidth() const;
			UInt32			GetHeight() const;
			UInt32			GetDepth() const { return 0; }
			PIXEL_FORMAT	GetPixelFormat() const;
			void			SetAddressUType(ADDRESS_TYPE eAddressType);
			void			SetAddressVType(ADDRESS_TYPE eAddressType);
			void			SetFilterType(FILTER_TYPE eFilterType);
			ADDRESS_TYPE	GetAddressUType() const;
			ADDRESS_TYPE	GetAddressVType() const;
			FILTER_TYPE		GetFilterType() const;

			// Accessors
			IDirect3DTexture9 * GetD3DTexture() const;
			virtual UInt32		GetTarget() const { return 0; }
			virtual void *		GetHandle() const { return GetD3DTexture(); }
			virtual Bool		GetUVNormalized() {return true;};
			virtual UInt32      GetNbrMipMapLevels() {return _nMipMapLevels;};

			// These methods are used to clean and restore the memory for a fullscreen switch
			static void ReleaseDynamicTextures();
			static void RecreateDynamicTextures();

		private:

			friend class RENDER_TARGET_D3D9_PC;
			friend class RENDER_CONTEXT_D3D9_PC;

			// This list is used to clean and restore the memory for a fullscreen switch
			static QDT_DLIST<TEXTURE_D3D9_PC *> _TextureList;

			void CreateTexture();
			void ReleaseTexture();

			PIXEL_FORMAT				_eFormat;
			UInt32						_nMipMapLevels;
			UInt32						_uiWidth;
			UInt32						_uiHeight;
			Bool						_bRenderTarget;
			Bool						_bLocked;
			ADDRESS_TYPE				_AddressUType;
			ADDRESS_TYPE				_AddressVType;
			FILTER_TYPE					_FilterType;
			RENDER_CONTEXT_D3D9_PC *	_pRenderContext;
			IDirect3DTexture9 *			_pD3DTexture;	
		};
	}
}

#endif // __KRENDERER_RENDERER_TEXTURE_D3D9_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

