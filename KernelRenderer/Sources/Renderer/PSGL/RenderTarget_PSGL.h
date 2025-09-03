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
//	CLASS:	RENDER_TARGET_PSGL
//	The RENDER_TARGET_PSGL class implements ...
//
//	05-10-24:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDER_TARGET_PSGL_H__
#define __KRENDERER_RENDER_TARGET_PSGL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer\PSGL\RenderContext_PSGL)
#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(Renderer\PSGL\Texture_PSGL)

namespace	QDT
{
	namespace	KRENDERER
	{
		class RENDER_TARGET_PSGL : public I_RENDER_TARGET
		{
		public:

			RENDER_TARGET_PSGL();
			RENDER_TARGET_PSGL(RENDER_CONTEXT_PSGL*	pRenderContext,
				UInt32			uiWidth,
				UInt32			uiHeight,
				PIXEL_FORMAT		eFormat);
			~RENDER_TARGET_PSGL();

			virtual UInt32			GetWidth() const;
			virtual UInt32			GetHeight() const;
			virtual PIXEL_FORMAT	GetPixelFormat() const;

			const TEXTURE_PSGL *	GetPSGLTexture() const;
			GLuint *				GetPSGLSurface();
			virtual I_TEXTURE *		GetTexture() const { return (I_TEXTURE *) GetPSGLTexture(); }
			virtual void *			GetSurface(CUBEMAP_FACE CF = CF_POSITIVE_X) const { return (const_cast<RENDER_TARGET_PSGL *>(this))->GetPSGLSurface(); }

		private:
		public://debug !!
			friend class RENDER_CONTEXT_PSGL;

			GLuint				_uiSurface;

			TEXTURE_PSGL		_Texture;
		};
	}
}

#endif // __KRENDERER_RENDER_TARGET_PSGL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

