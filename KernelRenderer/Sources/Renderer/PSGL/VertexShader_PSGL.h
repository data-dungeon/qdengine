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
//	CLASS:	VERTEX_SHADER_PSGL
//	The VERTEX_SHADER_PSGL class implements ...
//
//	05-09-20:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_VERTEX_SHADER_PSGL_H__
#define __KRENDERER_RENDERER_VERTEX_SHADER_PSGL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_CONTEXT_PSGL)
DECL_KCORE(QDT_STRING)

#include INCL_KRENDERER(Renderer\IVertexShader)
#define QDT_INCLUDE_PS3_PSGL
#include INCL_KCORE(Includes_PS3\Includes_PS3)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	VERTEX_SHADER_PSGL : public I_VERTEX_SHADER
		{
		public:

			VERTEX_SHADER_PSGL(RENDER_CONTEXT_PSGL * pRenderContext, const QDT_STRING & sName);
			VERTEX_SHADER_PSGL(RENDER_CONTEXT_PSGL * pRenderContext, UInt8 * pShader, UInt32 nShaderSize);
			~VERTEX_SHADER_PSGL();

			CGprogram				GetPSGLVertexShader() const;

		private:

			CGprogram	_PSGLVertexShader;
		};
	}
}

#endif // __KRENDERER_RENDERER_VERTEX_SHADER_PSGL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
