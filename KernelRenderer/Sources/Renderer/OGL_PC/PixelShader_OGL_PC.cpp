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
//
//	05-09-20:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "PixelShader_OGL_PC.h"	
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#define	 QDT_INCLUDE_STDIO
#include INCL_KCORE(Includes/Includes)
#include INCL_KCORE(Tools/QdtString)
#include INCL_KRENDERER(Renderer/OGL_PC/RenderContext_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)
#include "GLExtensions.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PIXEL_SHADER_OGL_PC constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_SHADER_OGL_PC::PIXEL_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC *	pRenderContext,
										 const QDT_STRING &			sFileName)
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

		_OGLPixelShader = cgCreateProgram(
			pRenderContext->GetCGContext(), 
			CG_SOURCE, 
			pShader, 
			pRenderContext->GetCGPixelShaderProfile(),
			"main", 
			&args[0]);

		if (cgGetError() != CG_NO_ERROR)
		{
			QDT_Message("ERROR WHILE COMPILING PIXEL SHADER!!!");
			QDT_STRING ErrorString = cgGetLastListing(pRenderContext->GetCGContext());
			QDT_Message(ErrorString.GetBuffer());
			QDT_FAIL();

			delete [] pShader;
			fclose(pFile);

			return;
		}

		cgCompileProgram(_OGLPixelShader);
		QDT_TEST_CG_ERROR();

		cgGLLoadProgram(_OGLPixelShader);
		QDT_TEST_CG_ERROR();

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
		_OGLPixelShaderId = cgGLGetProgramID(_OGLPixelShader);
		QDT_TEST_CG_ERROR();

		glRequestResidentProgramsNV(1, &_OGLPixelShaderId);
		QDT_TEST_GL_ERROR();

		QDT_ASSERT(glIsProgramNV(_OGLPixelShaderId));
#endif

		RENDER_CONTEXT::GetRenderStats().AddPixelShader(1);

		delete [] pShader;
		fclose(pFile);
	}
	else
	{
		QDT_Error("Can't find vertex shader %s on disk!", sExtendedFileName);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PIXEL_SHADER_OGL_PC constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_SHADER_OGL_PC::PIXEL_SHADER_OGL_PC(RENDER_CONTEXT_OGL_PC *	pRenderContext,
									     UInt8 *					pShader)
:
_bNeedDelete(true)
{
	char api[16];
	strcpy(api, "-DAPI_OPENGL");

	const char * args[2];
	args[0] = &api[0];
	args[1] = NULL;

	_OGLPixelShader = cgCreateProgram(
			pRenderContext->GetCGContext(), 
			CG_SOURCE, 
			(Char*)pShader, 
			pRenderContext->GetCGPixelShaderProfile(),
			"main", 
			&args[0]);

	if (cgGetError() != CG_NO_ERROR)
	{
		QDT_Message("ERROR WHILE COMPILING PIXEL SHADER!!!");
		QDT_STRING ErrorString = cgGetLastListing(pRenderContext->GetCGContext());
		QDT_Message(ErrorString.GetBuffer());
		QDT_FAIL();
		return;
	}

	QDT_TEST_CG_ERROR();

	cgCompileProgram(_OGLPixelShader);
	QDT_TEST_CG_ERROR();

	cgGLLoadProgram(_OGLPixelShader);
	QDT_TEST_CG_ERROR();

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	_OGLPixelShaderId = cgGLGetProgramID(_OGLPixelShader);
	QDT_TEST_CG_ERROR();

	glRequestResidentProgramsNV(1, &_OGLPixelShaderId);
	QDT_TEST_GL_ERROR();

	QDT_ASSERT(glIsProgramNV(_OGLPixelShaderId));
#endif

	RENDER_CONTEXT::GetRenderStats().AddPixelShader(1);
}

//-----------------------------------------------------------------------------
//	Name:		PIXEL_SHADER_OGL_PC constructor
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
PIXEL_SHADER_OGL_PC::PIXEL_SHADER_OGL_PC(const CGprogram &	Program)
:
_bNeedDelete(false),
_OGLPixelShader(Program)
{
	QDT_ASSERT(_OGLPixelShader);

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	_OGLPixelShaderId = cgGLGetProgramID(_OGLPixelShader);
	QDT_TEST_CG_ERROR();

	glRequestResidentProgramsNV(1, &_OGLPixelShaderId);
	QDT_TEST_GL_ERROR();

	QDT_ASSERT(glIsProgramNV(_OGLPixelShaderId));
#endif

	RENDER_CONTEXT::GetRenderStats().AddPixelShader(1);
}

//-----------------------------------------------------------------------------
//	Name:		~PIXEL_SHADER_OGL_PC destructor
//	Object:		
//	06-01-25:	RMA - Created
//-----------------------------------------------------------------------------
PIXEL_SHADER_OGL_PC::~PIXEL_SHADER_OGL_PC()
{
	if (_bNeedDelete)
	{
		cgDestroyProgram(_OGLPixelShader);
		QDT_TEST_CG_ERROR();

		RENDER_CONTEXT::GetRenderStats().AddPixelShader(-1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLPixelShader
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
CGprogram	PIXEL_SHADER_OGL_PC::GetOGLPixelShader() const
{
	return (_OGLPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLPixelShaderId
//	Object:		
//	06-03-24:	RMA - Created
//-----------------------------------------------------------------------------
GLuint	PIXEL_SHADER_OGL_PC::GetOGLPixelShaderId() const
{
	return (_OGLPixelShaderId);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
