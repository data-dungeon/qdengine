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
//
//	05-09-20:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "VertexShader_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer\PSGL\RenderContext_PSGL)
#include INCL_KCORE(Tools\QdtString)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_PSGL constructor
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_PSGL::VERTEX_SHADER_PSGL(RENDER_CONTEXT_PSGL * pRenderContext,
									   const QDT_STRING & sFileName)
{
	QDT_STRING sExtendedFileName = QDT_STRING(HOME_DIR) + sFileName + "_PS3_TYPEC";
	_PSGLVertexShader =  cgCreateProgramFromFile(pRenderContext->GetCgContext(), CG_BINARY, sExtendedFileName.GetBuffer(), PLATFORM_PS3_GPU_VP_PROFILE, NULL, NULL);
	QDT_TEST_CG_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVertexShader(1);
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_PSGL constructor
//	Object:		
//	05-11-23:	VMA - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_PSGL::VERTEX_SHADER_PSGL(RENDER_CONTEXT_PSGL *	pRenderContext,
									   UInt8 *					pShader, 
									   UInt32					nShaderSize)
{
	_PSGLVertexShader =  cgCreateProgram(pRenderContext->GetCgContext(), CG_BINARY, (const char*)pShader, PLATFORM_PS3_GPU_VP_PROFILE, NULL, NULL);
	QDT_TEST_CG_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVertexShader(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfVertexShaders(nShaderSize);
}

//-----------------------------------------------------------------------------
//	Name:		~VERTEX_SHADER_PSGL destructor
//	Object:		
//	06-04-12:	SBE - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_PSGL::~VERTEX_SHADER_PSGL()
{
	cgDestroyProgram(_PSGLVertexShader);
	RENDER_CONTEXT::GetRenderStats().AddVertexShader(-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLVertexShader
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
CGprogram	VERTEX_SHADER_PSGL::GetPSGLVertexShader() const
{
	return _PSGLVertexShader;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
