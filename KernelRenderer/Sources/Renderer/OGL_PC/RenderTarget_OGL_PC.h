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
//	CLASS:	RENDER_TARGET_OGL_PC
//	The RENDER_TARGET_OGL_PC class implements ...
//
//	05-12-19:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_RENDER_TARGET_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_RENDER_TARGET_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(Renderer\OGL_PC\Texture_OGL_PC)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#include INCL_KCORE(Includes_PC\Includes_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class RENDER_TARGET_OGL_PC : public I_RENDER_TARGET
		{
		public:

			RENDER_TARGET_OGL_PC(RENDER_STATS & RenderStats);
			RENDER_TARGET_OGL_PC(RENDER_CONTEXT_OGL_PC *	pRenderContext,
								 UInt32						uiWidth,
								 UInt32						uiHeight,
								 PIXEL_FORMAT				eFormat);
			~RENDER_TARGET_OGL_PC();

			virtual UInt32			GetWidth() const;
			virtual UInt32			GetHeight() const;
			virtual PIXEL_FORMAT	GetPixelFormat() const;

			const TEXTURE_OGL_PC *	GetOGLTexture() const;
			GLuint *				GetOGLSurface();
			virtual I_TEXTURE *		GetTexture() const { return (I_TEXTURE *) GetOGLTexture(); }
			virtual void *			GetSurface(CUBEMAP_FACE CF = CF_POSITIVE_X) const { return (const_cast<RENDER_TARGET_OGL_PC *>(this))->GetOGLSurface(); }

		private:
			friend class RENDER_CONTEXT_OGL_PC;

			GLuint				_uiSurface;

			TEXTURE_OGL_PC		_Texture;
		};
	}
}

#endif // #define __KRENDERER_RENDERER_OGL_PC_RENDER_TARGET_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

