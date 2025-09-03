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
//	CLASS:	VERTEX_SHADER_OGL_PC
//	The VERTEX_SHADER_OGL_PC class implements ...
//
//	05-12-15:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_VERTEX_SHADER_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_VERTEX_SHADER_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KRENDERER(RENDER_CONTEXT_OGL_PC)

#include INCL_KRENDERER(Renderer\IVertexShader)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#define QDT_INCLUDE_PC_CG
#include INCL_KCORE(Includes_PC\Includes_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	VERTEX_SHADER_OGL_PC : public I_VERTEX_SHADER
		{
		public:

			VERTEX_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC * pRenderContext, const QDT_STRING & sName);
			VERTEX_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC * pRenderContext, UInt8 * pShader);
			VERTEX_SHADER_OGL_PC(const CGprogram & cgProgram);
			~VERTEX_SHADER_OGL_PC();
			
			CGprogram	GetOGLVertexShader() const;
			GLuint		GetOGLVertexShaderId() const;

			// Should only be used in maya viewer !!
			inline SetNeedDelete(Bool bValue) { _bNeedDelete = bValue; }
			inline SetVertexProgram(const CGprogram & Program) { _OGLVertexShader = Program; }

		private:

			CGprogram	_OGLVertexShader;
			Bool		_bNeedDelete;
			GLuint		_OGLVertexShaderId;
		};
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_VERTEX_SHADER_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
