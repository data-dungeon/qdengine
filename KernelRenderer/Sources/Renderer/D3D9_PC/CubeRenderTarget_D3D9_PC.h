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
//	05-11-16:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_CUBE_RENDER_TARGET_D3D9_PC_H__
#define __KRENDERER_RENDERER_CUBE_RENDER_TARGET_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/IRenderTarget)
#include INCL_KRENDERER(Renderer/D3D9_PC/CubeTexture_D3D9_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class CUBE_RENDER_TARGET_D3D9_PC : public I_RENDER_TARGET
		{
		public:

			CUBE_RENDER_TARGET_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext,
				UInt32					uiEdgeLength,
				PIXEL_FORMAT				eFormat);
			~CUBE_RENDER_TARGET_D3D9_PC();

			virtual UInt32					GetWidth() const;
			virtual UInt32					GetHeight() const;
			virtual PIXEL_FORMAT			GetPixelFormat() const;

			const CUBE_TEXTURE_D3D9_PC *	GetD3DTexture() const;
			IDirect3DSurface9 *				GetD3DSurface(CUBEMAP_FACE) const;
			virtual I_TEXTURE *				GetTexture() const { return (I_TEXTURE *) GetD3DTexture(); }
			virtual void *					GetSurface(CUBEMAP_FACE CF = CF_NONE) const { return GetD3DSurface(CF); }
			
			// These methods are used to clean and restore the memory for a fullscreen switch
			static void ReleaseRenderTargets();
			static void RecreateRenderTargets();

		private:

			friend class RENDER_CONTEXT_D3D9_PC;

			void CreateRenderTarget();
			void ReleaseRenderTarget();

			// This list is used to clean and restore the memory for a fullscreen switch
			static QDT_DLIST<CUBE_RENDER_TARGET_D3D9_PC *> _RenderTargetList;

			IDirect3DSurface9 *	_pD3DSurfaces[6];

			CUBE_TEXTURE_D3D9_PC	_Texture;
		};
	}
}

#endif // __KRENDERER_RENDERER_CUBE_RENDER_TARGET_D3D9_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

