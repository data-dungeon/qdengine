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
//	CLASS:	CUBE_RENDER_TARGET_OGL_PC
//	The CUBE_RENDER_TARGET_OGL_PC class implements ...
//
//	05-10-24:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_CUBE_RENDER_TARGET_OGL_PC_H__
#define __KRENDERER_RENDERER_CUBE_RENDER_TARGET_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_OGL_PC)

#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(Renderer\OGL_PC\CubeTexture_OGL_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class CUBE_RENDER_TARGET_OGL_PC : public I_RENDER_TARGET
		{
		public:

			CUBE_RENDER_TARGET_OGL_PC();
			CUBE_RENDER_TARGET_OGL_PC(RENDER_CONTEXT_OGL_PC*	pRenderContext,
				UInt32					uiEdgeLength,
				PIXEL_FORMAT		eFormat);
			~CUBE_RENDER_TARGET_OGL_PC();

			virtual UInt32			GetWidth() const;
			virtual UInt32			GetHeight() const;
			virtual PIXEL_FORMAT	GetPixelFormat() const;

			const CUBE_TEXTURE_OGL_PC *	GetOGLTexture() const;
			GLuint *					GetOGLSurface(CUBEMAP_FACE CF);
			virtual I_TEXTURE *			GetTexture() const { return (I_TEXTURE *) GetOGLTexture(); }
			virtual void *				GetSurface(CUBEMAP_FACE CF = CF_POSITIVE_X) const { return (const_cast<CUBE_RENDER_TARGET_OGL_PC *>(this))->GetOGLSurface(CF); }


			void ReadSurfaceContent(CUBEMAP_FACE CF, void* buffer, Bool returnFloat = true);

		private:

			friend class RENDER_CONTEXT_OGL_PC;

			GLuint				_uiSurface[6];

			CUBE_TEXTURE_OGL_PC	_Texture;
		};
	}
}

#endif // __KRENDERER_RENDERER_CUBE_RENDER_TARGET_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

