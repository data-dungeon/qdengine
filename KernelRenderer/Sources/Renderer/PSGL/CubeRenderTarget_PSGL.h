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
//	CLASS:	CUBE_RENDER_TARGET_PSGL
//	The CUBE_RENDER_TARGET_PSGL class implements ...
//
//	05-10-24:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_CUBE_RENDER_TARGET_PSGL_H__
#define __KRENDERER_RENDERER_CUBE_RENDER_TARGET_PSGL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_PSGL)

#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(Renderer\PSGL\CubeTexture_PSGL)

namespace	QDT
{
	namespace	KRENDERER
	{
		class CUBE_RENDER_TARGET_PSGL : public I_RENDER_TARGET
		{
		public:

			CUBE_RENDER_TARGET_PSGL();
			CUBE_RENDER_TARGET_PSGL(RENDER_CONTEXT_PSGL*	pRenderContext,
									UInt32					uiEdgeLength,
									PIXEL_FORMAT			eFormat);
			~CUBE_RENDER_TARGET_PSGL();

			virtual UInt32			GetWidth() const;
			virtual UInt32			GetHeight() const;
			virtual PIXEL_FORMAT	GetPixelFormat() const;

			const CUBE_TEXTURE_PSGL *	GetPSGLTexture() const;
			GLuint *					GetPSGLSurface(CUBEMAP_FACE CF);
			virtual I_TEXTURE *			GetTexture() const { return (I_TEXTURE *) GetPSGLTexture(); }
			virtual void *				GetSurface(CUBEMAP_FACE CF = CF_POSITIVE_X) const { return (const_cast<CUBE_RENDER_TARGET_PSGL *>(this))->GetPSGLSurface(CF); }

		private:

			friend class RENDER_CONTEXT_PSGL;

			GLuint				_uiSurface[6];

			CUBE_TEXTURE_PSGL	_Texture;
		};
	}
}

#endif // __KRENDERER_RENDERER_CUBE_RENDER_TARGET_PSGL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

