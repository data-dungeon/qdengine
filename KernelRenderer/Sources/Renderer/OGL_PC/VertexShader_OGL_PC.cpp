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
//
//	05-09-20:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "VertexShader_OGL_PC.h"	
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#define QDT_INCLUDE_STDIO
#include INCL_KCORE(Includes/Includes)
#include INCL_KCORE(Tools/QdtString)
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#include INCL_KRENDERER(Renderer/OGL_PC/RenderContext_OGL_PC)
#include INCL_KRENDERER(Renderer/RenderContext)
#include "GLExtensions.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_OGL_PC constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_OGL_PC::VERTEX_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC *	pRenderContext,
										   const QDT_STRING &		sFileName)
:
_bNeedDelete(true)
{
	QDT_STRING sExtendedFileName = QDT_STRING(HOME_DIR) + sFileName + "_OGL_3_0";
		
	FILE * pFile = fopen(sExtendedFileName.GetBuffer(), "r");

	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		UInt32 nFileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		Char * pShader = new Char[nFileSize];

		fread(pShader, 1, nFileSize, pFile);

		char api[16];
		strcpy(api, "-DAPI_OPENGL");

		const char * args[2];
		args[0] = &api[0];
		args[1] = NULL;

		_OGLVertexShader = cgCreateProgram(
			pRenderContext->GetCGContext(), 
			CG_SOURCE, 
			pShader, 
			pRenderContext->GetCGVertexShaderProfile(),
			"main", 
			&args[0]);

		if (cgGetError() != CG_NO_ERROR)
		{
			QDT_Message("ERROR WHILE COMPILING VERTEX SHADER!!!");
			QDT_STRING ErrorString = cgGetLastListing(pRenderContext->GetCGContext());
			QDT_Message(ErrorString.GetBuffer());
			QDT_FAIL();

			delete [] pShader;
			fclose(pFile);

			return;
		}

		cgCompileProgram(_OGLVertexShader);
		QDT_TEST_GL_ERROR();

		cgGLLoadProgram(_OGLVertexShader);
		QDT_TEST_GL_ERROR();

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
		_OGLVertexShaderId = cgGLGetProgramID(_OGLVertexShader);
		QDT_TEST_CG_ERROR();

		glRequestResidentProgramsNV(1, &_OGLVertexShaderId);
		QDT_TEST_GL_ERROR();

		QDT_ASSERT(glIsProgramNV(_OGLVertexShaderId));
#endif

		RENDER_CONTEXT::GetRenderStats().AddVertexShader(1);

		delete [] pShader;
		fclose(pFile);
	}
	else
	{
		QDT_Error("Can't find vertex shader %s on disk!", sExtendedFileName);
	}
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_OGL_PC constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_OGL_PC::VERTEX_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC *	pRenderContext,
									   UInt8 *						pShader)
:
_bNeedDelete(true)
{
	char api[16];
	strcpy(api, "-DAPI_OPENGL");

	const char * args[2];
	args[0] = &api[0];
	args[1] = NULL;

	_OGLVertexShader = cgCreateProgram(
			pRenderContext->GetCGContext(), 
			CG_SOURCE, 
			(Char*)pShader, 
			pRenderContext->GetCGVertexShaderProfile(),
			"main", 
			&args[0]);

	if (cgGetError() != CG_NO_ERROR)
	{
		QDT_Message("ERROR WHILE COMPILING VERTEX SHADER!!!");
		QDT_STRING ErrorString = cgGetLastListing(pRenderContext->GetCGContext());
		QDT_Message(ErrorString.GetBuffer());
		QDT_FAIL();
		return;
	}

	cgCompileProgram(_OGLVertexShader);
	QDT_TEST_CG_ERROR();

	cgGLLoadProgram(_OGLVertexShader);
	QDT_TEST_CG_ERROR();

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	_OGLVertexShaderId = cgGLGetProgramID(_OGLVertexShader);
	QDT_TEST_CG_ERROR();

	glRequestResidentProgramsNV(1, &_OGLVertexShaderId);
	QDT_TEST_GL_ERROR();

	QDT_ASSERT(glIsProgramNV(_OGLVertexShaderId));
#endif

	RENDER_CONTEXT::GetRenderStats().AddVertexShader(1);
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_OGL_PC constructor
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_OGL_PC::VERTEX_SHADER_OGL_PC(const CGprogram &	cgProgram)
:
_bNeedDelete(false),
_OGLVertexShader(cgProgram)
{
	QDT_ASSERT(_OGLVertexShader);

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	_OGLVertexShaderId = cgGLGetProgramID(_OGLVertexShader);
	QDT_TEST_CG_ERROR();

	glRequestResidentProgramsNV(1, &_OGLVertexShaderId);
	QDT_TEST_GL_ERROR();

	QDT_ASSERT(glIsProgramNV(_OGLVertexShaderId));
#endif

	RENDER_CONTEXT::GetRenderStats().AddVertexShader(1);
}

//-----------------------------------------------------------------------------
//	Name:		~VERTEX_SHADER_OGL_PC destructor
//	Object:		
//	06-01-25:	RMA - Created
//-----------------------------------------------------------------------------
VERTEX_SHADER_OGL_PC::~VERTEX_SHADER_OGL_PC()
{
	if (_bNeedDelete)
	{
		cgDestroyProgram(_OGLVertexShader);
		QDT_TEST_CG_ERROR();
	}

	RENDER_CONTEXT::GetRenderStats().AddVertexShader(-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLVertexShader
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
CGprogram	VERTEX_SHADER_OGL_PC::GetOGLVertexShader() const
{
	return (_OGLVertexShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLVertexShaderId
//	Object:		
//	06-03-24:	RMA - Created
//-----------------------------------------------------------------------------
GLuint	VERTEX_SHADER_OGL_PC::GetOGLVertexShaderId() const
{
	return (_OGLVertexShaderId);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
