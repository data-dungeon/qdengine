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
//	CLASS:	VERTEX_SHADER_D3D9_PC
//	The VERTEX_SHADER_D3D9_PC class implements ...
//
//	05-09-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_VERTEX_SHADER_D3D9_PC_H__
#define __KRENDERER_RENDERER_VERTEX_SHADER_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KRENDERER(RENDER_CONTEXT_D3D9_PC)

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer\IVertexShader)

namespace	QDT
{
	namespace	KRENDERER
	{
		class VERTEX_SHADER_D3D9_PC : public I_VERTEX_SHADER
		{
		public:

			VERTEX_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext, const QDT_STRING & sFileName);
			VERTEX_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC*	pRenderContext, UInt8 * pShader);
			~VERTEX_SHADER_D3D9_PC();

			IDirect3DVertexShader9 * GetD3DVertexShader();

		private:

			RENDER_CONTEXT_D3D9_PC *		_pRenderContext;
			IDirect3DVertexShader9 *		_pD3DVertexShader;
		};
	}
}

#endif // __KRENDERER_RENDERER_VERTEX_SHADER_D3D9_PC_H__

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
