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
//	CLASS:	PIXEL_SHADER_OGL_PC
//	The PIXEL_SHADER_OGL_PC class implements ...
//
//	05-12-15:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_PIXEL_SHADER_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_PIXEL_SHADER_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KRENDERER(RENDER_CONTEXT_OGL_PC)

#include INCL_KRENDERER(Renderer\IPixelShader)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#define QDT_INCLUDE_PC_CG
#include INCL_KCORE(Includes_PC\Includes_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	PIXEL_SHADER_OGL_PC : public I_PIXEL_SHADER
		{
		public:

			PIXEL_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC * pRenderContext, const QDT_STRING & sFileName);
			PIXEL_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC * pRenderContext, UInt8 * pShader);
			PIXEL_SHADER_OGL_PC(const CGprogram & Program);
			~PIXEL_SHADER_OGL_PC();

			CGprogram	GetOGLPixelShader() const;
			GLuint		GetOGLPixelShaderId() const;

			// Should only be used in maya viewer !!
			inline SetNeedDelete(Bool bValue) { _bNeedDelete = bValue; }
			inline SetPixelProgram(const CGprogram & Program) { _OGLPixelShader = Program; }

		private:

			CGprogram	_OGLPixelShader;
			Bool		_bNeedDelete;
			GLuint		_OGLPixelShaderId;
		};
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_PIXEL_SHADER_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
