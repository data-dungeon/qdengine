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
//	CLASS:	PIXEL_SHADER_D3D9_PC
//	The PIXEL_SHADER_D3D9_PC class implements ...
//
//	05-09-27:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_PIXEL_SHADER_D3D9_PC_H__
#define __KRENDERER_RENDERER_PIXEL_SHADER_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/IPixelShader)

namespace	QDT
{
	namespace	KRENDERER
	{
		class PIXEL_SHADER_D3D9_PC : public I_PIXEL_SHADER
		{
		public:

			PIXEL_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC* pRenderContext, const QDT_STRING & sFileName);
			PIXEL_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC* pRenderContext, UInt8 * pShader);
			~PIXEL_SHADER_D3D9_PC();

			IDirect3DPixelShader9 * GetD3DPixelShader();

		private:

			RENDER_CONTEXT_D3D9_PC *		_pRenderContext;
			IDirect3DPixelShader9 *			_pD3DPixelShader;
		};
	}
}

#endif // __KRENDERER_RENDERER_PIXEL_SHADER_D3D9_PC_H__