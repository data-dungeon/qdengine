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
//	CLASS:	I_RENDER_TARGET
//	The I_RENDER_TARGET class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_I_RENDER_TARGET_H__
#define __KRENDERER_RENDERER_I_RENDER_TARGET_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_TEXTURE)

#include INCL_KRENDERER(Renderer/RendererTypeDefs)

namespace	QDT
{
	namespace	KRENDERER
	{
		class I_RENDER_TARGET
		{
		public :

			virtual ~I_RENDER_TARGET() {}

			virtual UInt32			GetWidth() const = 0;
			virtual UInt32			GetHeight() const = 0;
			virtual PIXEL_FORMAT	GetPixelFormat() const = 0;
			virtual void *			GetSurface(CUBEMAP_FACE CF = CF_POSITIVE_X) const = 0;
			virtual I_TEXTURE *		GetTexture() const = 0;
		};
	}
}

#endif // __KRENDERER_RENDERER_I_RENDER_TARGET_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
