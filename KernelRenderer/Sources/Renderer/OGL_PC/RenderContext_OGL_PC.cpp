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
// CLASS: RENDER_CONTEXT_OGL_PC
//
//
//	05-12-15:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "RenderContext_OGL_PC.h"
#include INCL_KCORE(Time\Time_ABC)
#include INCL_KMATH(Math\Geometry\Matrix\Matrix)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Vector\Vector)
#include INCL_KMATH(Math\Color\Color)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#include "glh_extensions.h"
#include INCL_KRENDERER(Renderer/OGL_PC/RenderTarget_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/VertexBuffer_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/IndexBuffer_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/VertexDeclaration_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/CubeTexture_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/CubeRenderTarget_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/VertexShader_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/PixelShader_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/VolumeTexture_OGL_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#include INCL_KRENDERER(Renderer/SRenderMode)
#include INCL_KRENDERER(Renderer/RenderContext)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

static UInt32 OGLCmpFonc[8] = 
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

static UInt32 OGLBlendMode[11] = 
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA_SATURATE,
};

static UInt32 OGLBlendOp[5] = 
{
	GL_FUNC_ADD_EXT,
	GL_FUNC_SUBTRACT_EXT,
	GL_FUNC_REVERSE_SUBTRACT_EXT,
	GL_MIN,
	GL_MAX,
};

static UInt32 OGLCullModes[3] = 
{
	GL_CCW,
	GL_CW
};

static UInt32 OGLFillModes[2] = 
{
	GL_LINE,
	GL_FILL,
};

static UInt32 OGLTextureAddress[2] = 
{
	GL_REPEAT,
	GL_CLAMP,
};

static UInt32 OGLPrimitiveTypeArray[6] = 
{
	GL_POINT,
	GL_LINES,
	GL_LINE_STRIP,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
};

#define GET_PROCADDR(p,f) f = (p) wglGetProcAddress(#f);
#define DEC_PROCADDR(p,f) p f = NULL;

DEC_PROCADDR(PFNGLBINDBUFFERARBPROC, glBindBufferARB);
DEC_PROCADDR(PFNGLDELETEBUFFERSARBPROC, glDeleteBuffersARB);
DEC_PROCADDR(PFNGLGENBUFFERSARBPROC, glGenBuffersARB);
DEC_PROCADDR(PFNGLISBUFFERARBPROC, glIsBufferARB);
DEC_PROCADDR(PFNGLBUFFERDATAARBPROC, glBufferDataARB);
DEC_PROCADDR(PFNGLBUFFERSUBDATAARBPROC, glBufferSubDataARB);
DEC_PROCADDR(PFNGLGETBUFFERSUBDATAARBPROC, glGetBufferSubDataARB);
DEC_PROCADDR(PFNGLMAPBUFFERARBPROC, glMapBufferARB);
DEC_PROCADDR(PFNGLUNMAPBUFFERARBPROC, glUnmapBufferARB);
DEC_PROCADDR(PFNGLGETBUFFERPARAMETERIVARBPROC, glGetBufferParameterivARB);
DEC_PROCADDR(PFNGLGETBUFFERPOINTERVARBPROC, glGetBufferPointervARB);
DEC_PROCADDR(PFNGLCLIENTACTIVETEXTUREARBPROC, glClientActiveTextureARB);
DEC_PROCADDR(PFNGLACTIVETEXTUREARBPROC, glActiveTextureARB);
DEC_PROCADDR(PFNGLCOMPRESSEDTEXIMAGE1DARBPROC, glCompressedTexImage1DARB);
DEC_PROCADDR(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC, glCompressedTexImage2DARB);
DEC_PROCADDR(PFNGLCOMPRESSEDTEXIMAGE3DARBPROC, glCompressedTexImage3DARB);

DEC_PROCADDR(PFNGLPROGRAMSTRINGARBPROC, glProgramStringARB);
DEC_PROCADDR(PFNGLBINDPROGRAMARBPROC, glBindProgramARB);
DEC_PROCADDR(PFNGLDELETEPROGRAMSARBPROC, glDeleteProgramsARB);
DEC_PROCADDR(PFNGLGENPROGRAMSARBPROC, glGenProgramsARB);
DEC_PROCADDR(PFNGLPROGRAMENVPARAMETER4DARBPROC, glProgramEnvParameter4dARB);
DEC_PROCADDR(PFNGLPROGRAMENVPARAMETER4DVARBPROC, glProgramEnvParameter4dvARB);
DEC_PROCADDR(PFNGLPROGRAMENVPARAMETER4FARBPROC, glProgramEnvParameter4fARB);
DEC_PROCADDR(PFNGLPROGRAMENVPARAMETER4FVARBPROC, glProgramEnvParameter4fvARB);
DEC_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4DARBPROC, glProgramLocalParameter4dARB);
DEC_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4DVARBPROC, glProgramLocalParameter4dvARB);
DEC_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4FARBPROC, glProgramLocalParameter4fARB);
DEC_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC, glProgramLocalParameter4fvARB);

DEC_PROCADDR(PFNGLDRAWRANGEELEMENTSEXTPROC, glDrawRangeElementsEXT);
DEC_PROCADDR(PFNGLTEXIMAGE3DPROC, glTexImage3D);
DEC_PROCADDR(PFNGLISRENDERBUFFEREXTPROC, glIsRenderbufferEXT);
DEC_PROCADDR(PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbufferEXT);
DEC_PROCADDR(PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffersEXT);
DEC_PROCADDR(PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffersEXT);
DEC_PROCADDR(PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorageEXT);
DEC_PROCADDR(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC, glGetRenderbufferParameterivEXT);
DEC_PROCADDR(PFNGLISFRAMEBUFFEREXTPROC, glIsFramebufferEXT);
DEC_PROCADDR(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT);
DEC_PROCADDR(PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffersEXT);
DEC_PROCADDR(PFNGLGENFRAMEBUFFERSEXTPROC, glGenFramebuffersEXT);
DEC_PROCADDR(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatusEXT);
DEC_PROCADDR(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC, glFramebufferTexture1DEXT);
DEC_PROCADDR(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC, glFramebufferTexture2DEXT);
DEC_PROCADDR(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC, glFramebufferTexture3DEXT);
DEC_PROCADDR(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT);
DEC_PROCADDR(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC, glGetFramebufferAttachmentParameterivEXT);
DEC_PROCADDR(PFNGLGENERATEMIPMAPEXTPROC, glGenerateMipmapEXT);
DEC_PROCADDR(PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT);

DEC_PROCADDR(PFNGLGENPROGRAMSNVPROC, glGenProgramsNV);
DEC_PROCADDR(PFNGLLOADPROGRAMNVPROC, glLoadProgramNV);
DEC_PROCADDR(PFNGLBINDPROGRAMNVPROC, glBindProgramNV);
DEC_PROCADDR(PFNGLPROGRAMPARAMETER4FNVPROC, glProgramParameter4fNV);
DEC_PROCADDR(PFNGLPROGRAMPARAMETER4FVNVPROC, glProgramParameter4fvNV);
DEC_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4FNVPROC, glProgramNamedParameter4fNV);
DEC_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4DNVPROC, glProgramNamedParameter4dNV);
DEC_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC, glProgramNamedParameter4fvNV);
DEC_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC, glProgramNamedParameter4dvNV);
DEC_PROCADDR(PFNGLISPROGRAMNVPROC, glIsProgramNV);
DEC_PROCADDR(PFNGLVERTEXATTRIBPOINTERNVPROC, glVertexAttribPointerNV);
DEC_PROCADDR(PFNGLREQUESTRESIDENTPROGRAMSNVPROC, glRequestResidentProgramsNV);

DEC_PROCADDR(PFNGLBLENDEQUATIONPROC, glBlendEquation);
DEC_PROCADDR(PFNGLCOPYTEXSUBIMAGE2DEXTPROC, glCopyTexSubImage2DEXT);

static char OGLRegisterNames[256][16];
static char OGLTextureNames[16][16];

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_OGL_PC constructor
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_CONTEXT_OGL_PC::RENDER_CONTEXT_OGL_PC(UInt32 uiWidth, 
											 UInt32 uiHeight)
:
_hDC(NULL),
_hRC(NULL),
_uiWidth(uiWidth),
_uiHeight(uiHeight),
_uiPosX(0),
_uiPosY(0),
_pCurrentVertexShader(NULL),
_pCurrentPixelShader(NULL),
_bEnableAlphaBlend(false),
_pFrontBuffer(NULL),
_pDepthBuffer(NULL),
_pCurrentRenderTarget(NULL),
_nCurrentRenderTargetCubeFace(CF_NONE),
_pCurrentDepthBuffer(NULL),
_pRenderModesArray(NULL),
_bFullScreen(false)
{
	_iViewportX = 0;
	_iViewportY = 0;
	_iViewportWidth = 0;
	_iViewportHeight = 0;

	_bScissorTest = false; // Default value for OGL
	_bNeedUpdateVertexContext = false;

	for (UInt32 i = 0 ; i < 256 ; ++i)
	{
		sprintf(OGLRegisterNames[i], "register%d", i);
	}

	for (UInt32 i = 0 ; i < 16 ; ++i)
	{
		sprintf( OGLTextureNames[i], "texture%d", i );
	}

	InitGLExtensions();
	InitGLContext();

	_CgContext = cgCreateContext();
	QDT_TEST_CG_ERROR();

	_CgVertexShaderProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	QDT_TEST_CG_ERROR();

	if (_CgVertexShaderProfile == CG_PROFILE_VP40)
	{
		// Unsupported nvidia extension! switching to 3.0
		_CgVertexShaderProfile = CG_PROFILE_VP30;
	}

	_CgPixelShaderProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	QDT_TEST_CG_ERROR();

	if (_CgPixelShaderProfile == CG_PROFILE_FP40)
	{
		// Unsupported nvidia extension! switching to 3.0
		_CgPixelShaderProfile = CG_PROFILE_FP30;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_OGL_PC constructor
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_CONTEXT_OGL_PC::RENDER_CONTEXT_OGL_PC(HWND	hWnd,
											 UInt32	uiWidth,
											 UInt32	uiHeight,
											 Bool	bFullscreen,
											 Bool	bCreateDepthBuffer)
:
_hWindow(hWnd),
_hDC(NULL),
_hRC(NULL),
_uiWidth(uiWidth),
_uiHeight(uiHeight),
_pCurrentVertexShader(NULL),
_pCurrentPixelShader(NULL),
_bEnableAlphaBlend(false),
_pFrontBuffer(NULL),
_pDepthBuffer(NULL),
_pCurrentRenderTarget(NULL),
_nCurrentRenderTargetCubeFace(CF_NONE),
_pCurrentDepthBuffer(NULL),
_pRenderModesArray(NULL)
{
	for (UInt32 i = 0 ; i < 256 ; ++i)
	{
		sprintf(OGLRegisterNames[i], "register%d", i);
	}

	for (UInt32 i = 0 ; i < 16 ; ++i)
	{
		sprintf( OGLTextureNames[i], "texture%d", i );
	}

	CreateGLContext();
	InitGLExtensions();
	InitGLContext();

	glEnable( GL_DEPTH_TEST );

	Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, 255, 0, 0, 0, 1.0);

	_CgContext = cgCreateContext();
	QDT_TEST_CG_ERROR();

	_CgVertexShaderProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	QDT_TEST_CG_ERROR();

	if (_CgVertexShaderProfile == CG_PROFILE_VP40)
	{
		// Unsupported nvidia extension! switching to 3.0
		_CgVertexShaderProfile = CG_PROFILE_VP30;
	}

	_CgPixelShaderProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	QDT_TEST_CG_ERROR();

	if (_CgPixelShaderProfile == CG_PROFILE_FP40)
	{
		// Unsupported nvidia extension! switching to 3.0
		_CgPixelShaderProfile = CG_PROFILE_FP30;
	}
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_OGL_PC Destructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
RENDER_CONTEXT_OGL_PC::~RENDER_CONTEXT_OGL_PC()
{
}

//-----------------------------------------------------------------------------
//	Name:		BeginScene
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::BeginScene()
{
	// Do nothing!!
}

//-----------------------------------------------------------------------------
//	Name:		BeginScene
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::EndScene()
{
	// Do nothing!!
}

//-----------------------------------------------------------------------------
//	Name:		CreateGLContext
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_OGL_PC::CreateGLContext()
{
	_hDC = GetDC(_hWindow);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;

	GLuint gluiPixelFormat = ChoosePixelFormat(_hDC, &pfd);
	SetPixelFormat(_hDC, gluiPixelFormat, &pfd);
	_hRC = wglCreateContext(_hDC);
	wglMakeCurrent(_hDC, _hRC);
	QDT_TEST_GL_ERROR();

	////////////////
	_bFullScreen = false;

	_nNbrRenderModes = 1;
	_pRenderModesArray = new S_RENDER_MODE[_nNbrRenderModes];
}

//-----------------------------------------------------------------------------
//	Name:		InitGLExtensions
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::InitGLExtensions()
{

	//glh_init_extensions( "WGL_ARB_pbuffer WGL_ARB_pixel_format GL_NV_texture_rectangle ");


	char *ext = (char*)glGetString( GL_EXTENSIONS );

	QDT_ASSERT(strstr(ext, "ARB_vertex_buffer_object") != NULL);


	GET_PROCADDR(PFNGLBINDBUFFERARBPROC, glBindBufferARB);
	GET_PROCADDR(PFNGLDELETEBUFFERSARBPROC, glDeleteBuffersARB);
	GET_PROCADDR(PFNGLGENBUFFERSARBPROC, glGenBuffersARB);
	GET_PROCADDR(PFNGLISBUFFERARBPROC, glIsBufferARB);
	GET_PROCADDR(PFNGLBUFFERDATAARBPROC, glBufferDataARB);
	GET_PROCADDR(PFNGLBUFFERSUBDATAARBPROC, glBufferSubDataARB);
	GET_PROCADDR(PFNGLGETBUFFERSUBDATAARBPROC, glGetBufferSubDataARB);
	GET_PROCADDR(PFNGLMAPBUFFERARBPROC, glMapBufferARB);
	GET_PROCADDR(PFNGLUNMAPBUFFERARBPROC, glUnmapBufferARB);
	GET_PROCADDR(PFNGLGETBUFFERPARAMETERIVARBPROC, glGetBufferParameterivARB);
	GET_PROCADDR(PFNGLGETBUFFERPOINTERVARBPROC, glGetBufferPointervARB);
	GET_PROCADDR(PFNGLCLIENTACTIVETEXTUREARBPROC, glClientActiveTextureARB);
	GET_PROCADDR(PFNGLACTIVETEXTUREARBPROC, glActiveTextureARB);
	GET_PROCADDR(PFNGLCOMPRESSEDTEXIMAGE1DARBPROC, glCompressedTexImage1DARB);
	GET_PROCADDR(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC, glCompressedTexImage2DARB);
	GET_PROCADDR(PFNGLCOMPRESSEDTEXIMAGE3DARBPROC, glCompressedTexImage3DARB);

	GET_PROCADDR(PFNGLPROGRAMSTRINGARBPROC, glProgramStringARB);
	GET_PROCADDR(PFNGLBINDPROGRAMARBPROC, glBindProgramARB);
	GET_PROCADDR(PFNGLDELETEPROGRAMSARBPROC, glDeleteProgramsARB);
	GET_PROCADDR(PFNGLGENPROGRAMSARBPROC, glGenProgramsARB);
	GET_PROCADDR(PFNGLPROGRAMENVPARAMETER4DARBPROC, glProgramEnvParameter4dARB);
	GET_PROCADDR(PFNGLPROGRAMENVPARAMETER4DVARBPROC, glProgramEnvParameter4dvARB);
	GET_PROCADDR(PFNGLPROGRAMENVPARAMETER4FARBPROC, glProgramEnvParameter4fARB);
	GET_PROCADDR(PFNGLPROGRAMENVPARAMETER4FVARBPROC, glProgramEnvParameter4fvARB);
	GET_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4DARBPROC, glProgramLocalParameter4dARB);
	GET_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4DVARBPROC, glProgramLocalParameter4dvARB);
	GET_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4FARBPROC, glProgramLocalParameter4fARB);
	GET_PROCADDR(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC, glProgramLocalParameter4fvARB);

	GET_PROCADDR(PFNGLDRAWRANGEELEMENTSEXTPROC, glDrawRangeElementsEXT);
	GET_PROCADDR(PFNGLTEXIMAGE3DPROC, glTexImage3D);
	GET_PROCADDR(PFNGLISRENDERBUFFEREXTPROC, glIsRenderbufferEXT);
	GET_PROCADDR(PFNGLBINDRENDERBUFFEREXTPROC, glBindRenderbufferEXT);
	GET_PROCADDR(PFNGLDELETERENDERBUFFERSEXTPROC, glDeleteRenderbuffersEXT);
	GET_PROCADDR(PFNGLGENRENDERBUFFERSEXTPROC, glGenRenderbuffersEXT);
	GET_PROCADDR(PFNGLRENDERBUFFERSTORAGEEXTPROC, glRenderbufferStorageEXT);
	GET_PROCADDR(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC, glGetRenderbufferParameterivEXT);
	GET_PROCADDR(PFNGLISFRAMEBUFFEREXTPROC, glIsFramebufferEXT);
	GET_PROCADDR(PFNGLBINDFRAMEBUFFEREXTPROC, glBindFramebufferEXT);
	GET_PROCADDR(PFNGLDELETEFRAMEBUFFERSEXTPROC, glDeleteFramebuffersEXT);
	GET_PROCADDR(PFNGLGENFRAMEBUFFERSEXTPROC, glGenFramebuffersEXT);
	GET_PROCADDR(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC, glCheckFramebufferStatusEXT);
	GET_PROCADDR(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC, glFramebufferTexture1DEXT);
	GET_PROCADDR(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC, glFramebufferTexture2DEXT);
	GET_PROCADDR(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC, glFramebufferTexture3DEXT);
	GET_PROCADDR(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC, glFramebufferRenderbufferEXT);
	GET_PROCADDR(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC, glGetFramebufferAttachmentParameterivEXT);
	GET_PROCADDR(PFNGLGENERATEMIPMAPEXTPROC, glGenerateMipmapEXT);
	GET_PROCADDR(PFNWGLSWAPINTERVALEXTPROC, wglSwapIntervalEXT);

	QDT_ASSERT(strstr(ext, "GL_NV_vertex_program2") != NULL);
	QDT_ASSERT(strstr(ext, "GL_NV_fragment_program") != NULL);
	GET_PROCADDR(PFNGLGENPROGRAMSNVPROC, glGenProgramsNV);
	GET_PROCADDR(PFNGLLOADPROGRAMNVPROC, glLoadProgramNV);
	GET_PROCADDR(PFNGLBINDPROGRAMNVPROC, glBindProgramNV);
	GET_PROCADDR(PFNGLPROGRAMPARAMETER4FNVPROC, glProgramParameter4fNV);
	GET_PROCADDR(PFNGLPROGRAMPARAMETER4FVNVPROC, glProgramParameter4fvNV);
	GET_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4FNVPROC, glProgramNamedParameter4fNV);
	GET_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4DNVPROC, glProgramNamedParameter4dNV);
	GET_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC, glProgramNamedParameter4fvNV);
	GET_PROCADDR(PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC, glProgramNamedParameter4dvNV);
	GET_PROCADDR(PFNGLISPROGRAMNVPROC, glIsProgramNV);
	GET_PROCADDR(PFNGLVERTEXATTRIBPOINTERNVPROC, glVertexAttribPointerNV);
	GET_PROCADDR(PFNGLREQUESTRESIDENTPROGRAMSNVPROC, glRequestResidentProgramsNV);

	GET_PROCADDR(PFNGLBLENDEQUATIONPROC, glBlendEquation);
	GET_PROCADDR(PFNGLCOPYTEXSUBIMAGE2DEXTPROC, glCopyTexSubImage2DEXT);

	// todo gestion erreur
	if( !glBindBufferARB || !glDeleteBuffersARB || !glGenBuffersARB || 
		!glIsBufferARB || !glBufferDataARB || !glBufferSubDataARB || 
		!glGetBufferSubDataARB || !glMapBufferARB || !glUnmapBufferARB || 
		!glGetBufferParameterivARB || !glGetBufferPointervARB || !glClientActiveTextureARB ||
		!glActiveTextureARB || !glCompressedTexImage1DARB || !glCompressedTexImage2DARB || 
		!glCompressedTexImage3DARB || !glDrawRangeElementsEXT )
	{
		MessageBox(NULL,"One or more ARB_vertex_buffer_object functions were not found",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
	}	
}

//-----------------------------------------------------------------------------
//	Name:		RetrieveFrontAndDepthBuffer
//	Object:		
//	06-01-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::RetrieveFrontAndDepthBuffer()
{
	RENDER_TARGET_OGL_PC * pFrontBuffer = new RENDER_TARGET_OGL_PC(_RenderStats);
	pFrontBuffer->_Texture._uiWidth = _uiWidth;
	pFrontBuffer->_Texture._uiHeight = _uiHeight;
	pFrontBuffer->_Texture._eFormat = PF_A8R8G8B8;
	_pCurrentRenderTarget = _pFrontBuffer = pFrontBuffer;

	RENDER_TARGET_OGL_PC * pDepthBuffer = new RENDER_TARGET_OGL_PC(_RenderStats);
	pDepthBuffer->_Texture._uiWidth = _uiWidth;
	pDepthBuffer->_Texture._uiHeight = _uiHeight;
	pDepthBuffer->_Texture._eFormat = PF_D24S8;
	_pCurrentDepthBuffer = _pDepthBuffer = pDepthBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		InitGLContext
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::InitGLContext()
{
	RetrieveFrontAndDepthBuffer();
	QDT_TEST_GL_ERROR();

	SetDefaultValues();
	QDT_TEST_GL_ERROR();

	// disable VSync
	wglSwapIntervalEXT(0);
	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		Present
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::Present()
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	// Reset Viewport
	SetViewport(0, 0, _uiWidth, _uiHeight);
	QDT_TEST_GL_ERROR();

	SwapBuffers(_hDC);
	QDT_TEST_GL_ERROR();

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddPresentTime(fTime1 - fTime0);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DrawPrimitive
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::DrawPrimitive(PRIMITIVE_TYPE nType, 
										UInt32 nPrimitiveCount, 
										UInt32 nStartVertex)
{
	QDT_ASSERT(_pCurrentVertexDeclaration && "Renderer : You have to set a vertex declaration before to draw anything!");
	
	#ifdef QDT_DEBUG
	bool bTestVertexBufferValidity = false;
	for (UInt32 i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		if (_apCurrentVertexBuffer[i])
		{
			bTestVertexBufferValidity = true;
		}
	}
	QDT_ASSERT(bTestVertexBufferValidity && "Renderer : You have to set at least one vertex buffer before to draw anything!");
	#endif

	UpdateVertexContext();
	QDT_TEST_GL_ERROR();

	UInt32 OGLPrimType = GetOGLPrimitiveType(nType);
	UInt32 nNumIndices = GetNumIndices(nType, nPrimitiveCount);
		
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	glDrawArrays(OGLPrimType, nStartVertex, nNumIndices);
	QDT_TEST_GL_ERROR();

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddDrawPrimitiveTime(fTime1 - fTime0);
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	QDT_TEST_GL_ERROR();

	_RenderStats.AddDrawPrimitive(1);

	switch (nType)
	{
	default:
		break;

	case PT_TRIANGLE_LIST:
		_RenderStats.AddPolygones(nPrimitiveCount / 3);
		break;

	case PT_TRIANGLE_STRIP:
		_RenderStats.AddPolygones(nPrimitiveCount - 2);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DrawIndexedPrimitive
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::DrawIndexedPrimitive(PRIMITIVE_TYPE	nType,
													UInt32			nMinIndex,
													UInt32			nNumVertices,
													UInt32			nStartIndex,
													UInt32			nPrimitiveCount)
{
	QDT_ASSERT(_pCurrentVertexDeclaration && "Renderer : You have to set a vertex declaration before to draw anything!");
	
	#ifdef QDT_DEBUG
	bool bTestVertexBufferValidity = false;
	for (UInt32 i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		if (_apCurrentVertexBuffer[i])
		{
			bTestVertexBufferValidity = true;
		}
	}
	QDT_ASSERT(bTestVertexBufferValidity && "Renderer : You have to set at least one vertex buffer before to draw anything!");
	#endif

	UpdateVertexContext();
	QDT_TEST_GL_ERROR();

	UpdateIndexContext();
	QDT_TEST_GL_ERROR();

	UInt32 OGLPrimType = GetOGLPrimitiveType(nType);
	UInt32 nNumIndices = GetNumIndices(nType, nPrimitiveCount);

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif
	
	glDrawRangeElementsEXT(OGLPrimType, nMinIndex, nMinIndex + nNumVertices, nNumIndices, GL_UNSIGNED_SHORT, (GLvoid*)(nStartIndex * sizeof(UInt16)) );
	QDT_TEST_GL_ERROR();

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddDrawPrimitiveTime(fTime1 - fTime0);
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	QDT_TEST_GL_ERROR();

	_RenderStats.AddDrawPrimitive(1);

	switch (nType)
	{
	default:
		break;

	case PT_TRIANGLE_LIST:
		_RenderStats.AddPolygones(nPrimitiveCount / 3);
		break;

	case PT_TRIANGLE_STRIP:
		_RenderStats.AddPolygones(nPrimitiveCount - 2);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DrawPoints
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::DrawPoints(UInt32					uiNbrPoints,
										const DEBUG_VERTEX *	pVertices,
										const MATRIX &			ViewMatrix,
										const MATRIX &			ProjectionMatrix)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		DrawLines
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::DrawLines(UInt32				uiNbrLines,
									   const DEBUG_VERTEX *	pVertices,
									   const MATRIX &		ViewMatrix,
									   const MATRIX &		ProjectionMatrix)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		DrawTriangles
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::DrawTriangles(UInt32				uiNbrTriangles,
										   const DEBUG_VERTEX *	pVertices,
										   const MATRIX &		ViewMatrix,
										   const MATRIX &		ProjectionMatrix)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexBuffer
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
I_VERTEX_BUFFER * RENDER_CONTEXT_OGL_PC::CreateVertexBuffer(UInt32 vertexCount, 
														  UInt32 vertexStride, 
														  Bool bDynamic)
{
	return (new VERTEX_BUFFER_OGL_PC(vertexCount, vertexStride, bDynamic));
}

//-----------------------------------------------------------------------------
//	Name:		CreateIndexBuffer
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
I_INDEX_BUFFER *	RENDER_CONTEXT_OGL_PC::CreateIndexBuffer(UInt32	nIndexCount)
{
	return (new INDEX_BUFFER_OGL_PC(nIndexCount));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexDeclaration
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
I_VERTEX_DECLARATION * RENDER_CONTEXT_OGL_PC::CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements, UInt32 numElements)
{
	return (new VERTEX_DECLARATION_OGL_PC(pElements, numElements));
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
I_TEXTURE * RENDER_CONTEXT_OGL_PC::CreateTexture(UInt32		uiWidth, 
											   UInt32		uiHeight, 
											   PIXEL_FORMAT eFormat, 
											   UInt32		uiMipMapLevelNum)
{
	return (new TEXTURE_OGL_PC(uiWidth, uiHeight, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeTexture
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_OGL_PC::CreateCubeTexture(UInt32		uiEdgeLength,
												   PIXEL_FORMAT	eFormat,
												   UInt32		uiMipMapLevelNum)
{
	return (new CUBE_TEXTURE_OGL_PC(uiEdgeLength, eFormat, uiMipMapLevelNum));
}
	
//-----------------------------------------------------------------------------
//	Name:		CreateVolumeTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_OGL_PC::CreateVolumeTexture(UInt32			uiWidth,
													 UInt32			uiHeight,
													 UInt32			uiDepth,
													 PIXEL_FORMAT	eFormat,
													 UInt32			uiMipMapLevelNum)
{
	return (new VOLUME_TEXTURE_OGL_PC(uiWidth, uiHeight, uiDepth, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShaderFromFile
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT_OGL_PC::CreateVertexShaderFromFile(const QDT_STRING & sFileName)
{
	return (new VERTEX_SHADER_OGL_PC(this, sFileName));
}
	
//-----------------------------------------------------------------------------
//	Name:		CreatePixelShaderFromFile
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	RENDER_CONTEXT_OGL_PC::CreatePixelShaderFromFile(const QDT_STRING & sFileName)
{
	return (new PIXEL_SHADER_OGL_PC(this, sFileName));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-11-23:	VMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT_OGL_PC::CreateVertexShader(UInt8 *	pShader)
{
	return (new VERTEX_SHADER_OGL_PC(this, pShader));
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShader
//	Object:		
//	05-11-23:	VMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	RENDER_CONTEXT_OGL_PC::CreatePixelShader(UInt8 *	pShader)
{
	return (new PIXEL_SHADER_OGL_PC(this, pShader));
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderTarget
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------	
I_RENDER_TARGET * RENDER_CONTEXT_OGL_PC::CreateRenderTarget(UInt32			uiWidth, 
															 UInt32			uiHeight, 
															 PIXEL_FORMAT	eFormat)
{
	return (new RENDER_TARGET_OGL_PC(this, uiWidth, uiHeight, eFormat));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeRenderTarget
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_OGL_PC::CreateCubeRenderTarget(UInt32			uiEdgeLength,
																PIXEL_FORMAT	eFormat)
{
	return (new CUBE_RENDER_TARGET_OGL_PC(this, uiEdgeLength, eFormat));
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetVertexShader(I_VERTEX_SHADER *	pVertexShader)
{
	if (pVertexShader != _pCurrentVertexShader)
	{
		_pCurrentVertexShader = pVertexShader;

		if (pVertexShader != NULL)
		{
#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
			GLuint id = ((VERTEX_SHADER_OGL_PC *)pVertexShader)->GetOGLVertexShaderId();

			QDT_ASSERT(glIsProgramNV(id));

			glBindProgramNV(GL_VERTEX_PROGRAM_NV, id);
			QDT_TEST_GL_ERROR();

			glEnable(GL_VERTEX_PROGRAM_NV);
			QDT_TEST_GL_ERROR();

#else
			cgGLEnableProfile(_CgVertexShaderProfile);
			QDT_TEST_CG_ERROR();
			cgGLBindProgram(((VERTEX_SHADER_OGL_PC *)pVertexShader)->GetOGLVertexShader());
			QDT_TEST_CG_ERROR();
#endif
		}
		else
		{
#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
			glDisable(GL_VERTEX_PROGRAM_NV);
			QDT_TEST_GL_ERROR();

			glBindProgramNV(GL_VERTEX_PROGRAM_NV, 0);
			QDT_TEST_GL_ERROR();
#else
			cgGLUnbindProgram(_CgVertexShaderProfile);
			QDT_TEST_CG_ERROR();
			cgGLDisableProfile(_CgVertexShaderProfile);
			QDT_TEST_CG_ERROR();
#endif
		}

		RENDER_CONTEXT::GetRenderStats().AddSetVertexShader(1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderMatrixParameter
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetVertexShaderConstant(UInt32 nRegister,
													   const VECTOR & Vec)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister, Vec.GetX(), Vec.GetY(), Vec.GetZ(), 1.0f);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_OGL_PC *)_pCurrentVertexShader)->GetOGLVertexShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter3f(cgP, Vec.GetX(), Vec.GetY(), Vec.GetZ());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetVertexShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetVertexShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderParameter
//	Object:		
//	26/01/06:	PDE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetVertexShaderConstant(UInt32 nRegister,
													   Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister, f1, f2, f3, f4);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_OGL_PC *)_pCurrentVertexShader)->GetOGLVertexShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, f1, f2, f3, f4);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetVertexShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetVertexShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderMatrixParameter
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetVertexShaderConstant(UInt32 nRegister,
													   const MATRIX & m)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	Float32 array[16];

	// Flip matrice order!!!
	array[0] = m(0, 0);
	array[1] = m(1, 0);
	array[2] = m(2, 0);
	array[3] = m(3, 0);
	array[4] = m(0, 1);
	array[5] = m(1, 1);
	array[6] = m(2, 1);
	array[7] = m(3, 1);
	array[8] = m(0, 2);
	array[9] = m(1, 2);
	array[10] = m(2, 2);
	array[11] = m(3, 2);
	array[12] = m(0, 3);
	array[13] = m(1, 3);
	array[14] = m(2, 3);
	array[15] = m(3, 3);

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister+0, array[0],  array[1],  array[2],  array[3]);
	QDT_TEST_GL_ERROR();
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister+1, array[4],  array[5],  array[6],  array[7]);
	QDT_TEST_GL_ERROR();
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister+2, array[8],  array[9],  array[10], array[11]);
	QDT_TEST_GL_ERROR();
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister+3, array[12], array[13], array[14], array[15]);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_OGL_PC *)_pCurrentVertexShader)->GetOGLVertexShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetMatrixParameterfr(cgP, &array[0]);	
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetVertexShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetVertexShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetVertexShaderConstant(UInt32 nRegister,
													   const COLOR_FLOAT & Color)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_OGL_PC *)_pCurrentVertexShader)->GetOGLVertexShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetVertexShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetVertexShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderArrayConstant
//	Object:		
//	05-11-29:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetVertexShaderArrayConstant(UInt32			nRegister,
														    UInt32			uiNbrValues,
														    const float *	pValues)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	for (UInt32 i = 0 ; i < uiNbrValues / 4 ; ++i)
	{
		glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, nRegister + i, pValues[i*4], pValues[i*4+1], pValues[i*4+2], pValues[i*4+3]);
		QDT_TEST_GL_ERROR();
	}
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_OGL_PC *)_pCurrentVertexShader)->GetOGLVertexShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameterArray4f(cgP, 0, uiNbrValues / 4, pValues);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetVertexShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetVertexShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShader
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShader(I_PIXEL_SHADER *	pPixelShader)
{
	if (pPixelShader != _pCurrentPixelShader)
	{
		_pCurrentPixelShader = pPixelShader;
		
		if (pPixelShader != NULL)
		{
#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
			GLuint id = ((PIXEL_SHADER_OGL_PC *)pPixelShader)->GetOGLPixelShaderId();
			QDT_ASSERT(glIsProgramNV(id));

			glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, id);
			QDT_TEST_GL_ERROR();

			glEnable(GL_FRAGMENT_PROGRAM_NV);
			QDT_TEST_GL_ERROR();
#else
			cgGLEnableProfile(_CgPixelShaderProfile);
			QDT_TEST_CG_ERROR();

			cgGLBindProgram(((PIXEL_SHADER_OGL_PC *)pPixelShader)->GetOGLPixelShader());
			QDT_TEST_CG_ERROR();
#endif
		}
		else
		{
#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
			glDisable(GL_FRAGMENT_PROGRAM_NV);
			QDT_TEST_GL_ERROR();

			glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, 0);
			QDT_TEST_GL_ERROR();
#else
			cgGLUnbindProgram(_CgPixelShaderProfile);
			QDT_TEST_CG_ERROR();
			cgGLDisableProfile(_CgPixelShaderProfile);
			QDT_TEST_CG_ERROR();
#endif
		}

		RENDER_CONTEXT::GetRenderStats().AddSetPixelShader(1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShaderConstant(UInt32 nRegister,
													  const VECTOR & Vec)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister, Vec.GetX(), Vec.GetY(), Vec.GetZ(), 1.0f);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter3f(cgP, Vec.GetX(), Vec.GetY(), Vec.GetZ());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	26/01/06:	PDE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetPixelShaderConstant(UInt32 nRegister, 
												   Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister, f1, f2, f3, f4);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, f1, f2, f3, f4);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderArrayConstant
//	Object:		
//	05-11-29:	PDE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShaderArrayConstant(UInt32		nRegister,
														   UInt32		uiNbrValues,
														   const float *pValues)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif


#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	for (UInt32 i = 0 ; i < uiNbrValues / 4 ; ++i)
	{
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister + i, pValues[i*4], pValues[i*4+1], pValues[i*4+2], pValues[i*4+3]);
		QDT_TEST_GL_ERROR();
	}
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameterArray4f(cgP, 0, uiNbrValues / 4, pValues);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-10-14:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShaderConstant(UInt32 nRegister,
													  Float32 r)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister, r, 1.0f, 1.0f, 1.0f);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter1f(cgP, r);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShaderConstant(UInt32 nRegister,
													  const MATRIX & m)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	Float32 array[16];

	// Flip matrice order!!!
	array[0] = m(0, 0);
	array[1] = m(1, 0);
	array[2] = m(2, 0);
	array[3] = m(3, 0);
	array[4] = m(0, 1);
	array[5] = m(1, 1);
	array[6] = m(2, 1);
	array[7] = m(3, 1);
	array[8] = m(0, 2);
	array[9] = m(1, 2);
	array[10] = m(2, 2);
	array[11] = m(3, 2);
	array[12] = m(0, 3);
	array[13] = m(1, 3);
	array[14] = m(2, 3);
	array[15] = m(3, 3);

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister,   array[0],  array[1],  array[2],  array[3]);
	QDT_TEST_GL_ERROR();

	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister+1, array[4],  array[5],  array[6],  array[7]);
	QDT_TEST_GL_ERROR();

	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister+2, array[8],  array[9],  array[10], array[11]);
	QDT_TEST_GL_ERROR();

	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister+3, array[12], array[13], array[14], array[15]);
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetMatrixParameterfr(cgP, &array[0]);														
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShaderConstant(UInt32 nRegister,
													  const COLOR_FLOAT & Color)
{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_NV, nRegister, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
	QDT_TEST_GL_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderTextureParameter
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetPixelShaderTexture(UInt32			nTexture,
												     I_TEXTURE *	pTexture)
{
#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
	SetTexture(nTexture, pTexture);
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_OGL_PC *)_pCurrentPixelShader)->GetOGLPixelShader(), OGLTextureNames[nTexture]);	
	QDT_TEST_CG_ERROR();

	if (pTexture != NULL)
	{
		cgGLSetTextureParameter(cgP, *(GLuint*)pTexture->GetHandle());							
		QDT_TEST_CG_ERROR();

		cgGLEnableTextureParameter(cgP);										
		QDT_TEST_CG_ERROR();

		_RenderStats.AddSetTexture(1);
	}
	else
	{
		cgGLDisableTextureParameter(cgP);										
		QDT_TEST_CG_ERROR();

		_RenderStats.AddSetTexture(1);
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetBlendMode(BLEND_MODE src,
										 BLEND_OP op,
									     BLEND_MODE dest)
{
	if (_eSrcBlendMode != src || _eDstBlendMode != dest)
	{
		_eSrcBlendMode = src;
		_eDstBlendMode = dest;
		glBlendFunc(OGLBlendMode[src], OGLBlendMode[dest]);
	}

	if (_eBlendOp != op)
	{
		_eBlendOp = op;
		glBlendEquation(OGLBlendOp[_eBlendOp]);
	}

	if (_eSrcBlendMode == BM_ONE && _eDstBlendMode == BM_ZERO  && _eBlendOp == BOP_ADD)
	{
		if (_bEnableAlphaBlend == true)
		{
			glDisable(GL_BLEND);
			_bEnableAlphaBlend = false;
		}
	}
	else if (_bEnableAlphaBlend == false)
	{
		glEnable(GL_BLEND);
		_bEnableAlphaBlend = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::GetBlendMode(BLEND_MODE & src, 
										 BLEND_OP & op,
									     BLEND_MODE & dest)
{	
	src = _eSrcBlendMode;
	op = _eBlendOp;
	dest = _eDstBlendMode;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::Clear(UInt32 flags, 
								UInt8 r, 
								UInt8 g, 
								UInt8 b,
								UInt8 a,
								float z)
{
	GLint glFlags = 0;

	if (flags & CF_CLEAR_COLOR)
	{
		glFlags |= GL_COLOR_BUFFER_BIT;
	}

	if (flags & CF_CLEAR_ZBUFFER)
	{
		glFlags |= GL_DEPTH_BUFFER_BIT;
	}

	const Float32 rConv = 1.0f / 255.0f;

	Float32 rR = static_cast<Float32>(r) * rConv;
	Float32 rG = static_cast<Float32>(g) * rConv;
	Float32 rB = static_cast<Float32>(b) * rConv;
	Float32 rA = static_cast<Float32>(a) * rConv;

	glClearColor(rR, rG, rB, rA);
	glClearDepth(z);

	Int32 iSafeViewportY = 0;

	if (_iViewportY > 0)		
	{
		iSafeViewportY = _uiHeight - (_iViewportY + _iViewportHeight);
	}

	glScissor(_iViewportX, iSafeViewportY, _iViewportWidth, _iViewportHeight);
	QDT_TEST_GL_ERROR();
	
	glEnable(GL_SCISSOR_TEST);	
	QDT_TEST_GL_ERROR();

	glClear(glFlags);
	QDT_TEST_GL_ERROR();

	glDisable(GL_SCISSOR_TEST);
	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		SetViewport
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetViewport(Int32 x, 
									  Int32 y, 
									  Int32 width, 
									  Int32 height)
{
	_iViewportX = x;
	_iViewportY = y;
	_iViewportWidth = width;
	_iViewportHeight = height;

	glViewport(x, y, width, height);
}

//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	06-03-29:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::GetViewport(Int32 &	x,
										   Int32 &	y,
										   Int32 &	width,
										   Int32 &	height)
{
	x = _iViewportX;
	y = _iViewportY;
	width = _iViewportWidth;
	height = _iViewportHeight;
}

//-----------------------------------------------------------------------------
//	Name:		SetTexture
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetTexture(UInt32			i, 
									   I_TEXTURE *	pTexture)
{
	if (_apCurrentTextures[i] != pTexture)
	{
		_apCurrentTextures[i] = pTexture;

		if (pTexture)
		{
			glActiveTextureARB(GL_TEXTURE0 + i);
			QDT_TEST_GL_ERROR();

			if (_aCurrentTexturesTarget[i])
			{
				glDisable(_aCurrentTexturesTarget[i]);
				QDT_TEST_GL_ERROR();
			}

			_aCurrentTexturesTarget[i] = pTexture->GetTarget();
			glEnable(_aCurrentTexturesTarget[i]);
			QDT_TEST_GL_ERROR();
			
			glBindTexture(_aCurrentTexturesTarget[i], *(GLuint*)pTexture->GetHandle());	
			QDT_TEST_GL_ERROR();

			_RenderStats.AddSetTexture(1);
		}
		else
		{
			glActiveTextureARB(GL_TEXTURE0 + i);
			QDT_TEST_GL_ERROR();

			glDisable(_aCurrentTexturesTarget[i]);
			QDT_TEST_GL_ERROR();

			glBindTexture(_aCurrentTexturesTarget[i], NULL);
			QDT_TEST_GL_ERROR();

			_RenderStats.AddSetTexture(1);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexDeclaration
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration)
{
	if (_pCurrentVertexDeclaration != pVertexDeclaration)
	{
		_pCurrentVertexDeclaration = pVertexDeclaration;
		_bNeedUpdateVertexContext = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCullMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetCullMode(CULL_MODE eCullMode)
{
	if (_eCullMode != eCullMode)
	{
		_eCullMode = eCullMode;

		if (_eCullMode == CULL_NONE)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(OGLCullModes[eCullMode-1]);
		}		
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCullMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------	
CULL_MODE RENDER_CONTEXT_OGL_PC::GetCullMode() const
{
	return _eCullMode;
}

//-----------------------------------------------------------------------------
//	Name:		SetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetFillMode(FILL_MODE	eFillMode)
{
	if (_eFillMode != eFillMode)
	{
		_eFillMode = eFillMode;
		glPolygonMode(GL_FRONT_AND_BACK, OGLFillModes[eFillMode]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
FILL_MODE	RENDER_CONTEXT_OGL_PC::GetFillMode() const
{
	return _eFillMode;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_OGL_PC::GetCurrentRenderTarget(UInt32 uiRenderTargetIndex) const
{
	QDT_ASSERT(uiRenderTargetIndex == NULL);
	return _pCurrentRenderTarget;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	06-03-17:	RMA - Created
//-----------------------------------------------------------------------------
CUBEMAP_FACE RENDER_CONTEXT_OGL_PC::GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex) const
{
	return (_nCurrentRenderTargetCubeFace);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentDepthBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_OGL_PC::GetCurrentDepthBuffer() const
{
	return _pCurrentDepthBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		GetFrontBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_OGL_PC::GetFrontBuffer() const
{
	return _pFrontBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		GetDepthBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_OGL_PC::GetDepthBuffer() const
{
	return _pDepthBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		SupportMultipleRenderTargets
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_OGL_PC::SupportMultipleRenderTargets()
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		EnableColorWrite
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::EnableColorWrite(Bool	bR,
												Bool	bG,
												Bool	bB,
												Bool	bA,
												UInt32	uiRenderTargetIndex)
{
	glColorMask(bR, bG, bB, bA);
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentRenderTarget
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetCurrentRenderTarget(I_RENDER_TARGET *	pRenderTarget, 
													  UInt32 uiCurrentRenderTarget,
													  CUBEMAP_FACE nCubeMapFace)
{
	QDT_ASSERT(pRenderTarget);
	QDT_ASSERT(uiCurrentRenderTarget == 0);

	if ( (_pCurrentRenderTarget != pRenderTarget) || (_nCurrentRenderTargetCubeFace != nCubeMapFace) )
	{
#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

		GLuint gluiSurface = *(GLuint*) pRenderTarget->GetSurface(nCubeMapFace);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gluiSurface);
		QDT_TEST_GL_ERROR();

		_pCurrentRenderTarget = pRenderTarget;
		_nCurrentRenderTargetCubeFace = nCubeMapFace;

		_RenderStats.AddSetRenderTarget(1);

#ifdef QDT_PROFILE_API_OPENGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetRenderTargetTime(fTime1 - fTime0);
#endif
	}	
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentDepthBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetCurrentDepthBuffer(I_RENDER_TARGET *	pRenderTarget)
{
	//if (_pCurrentDepthBuffer != pRenderTarget)
	if (pRenderTarget)
	{
		GLuint gluiSurface = *(GLuint*) pRenderTarget->GetSurface();

		if (pRenderTarget->GetPixelFormat() == PF_D24S8_DEPTHSTENCIL)
		{
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, gluiSurface);
			QDT_TEST_GL_ERROR();

			// attach a renderbuffer to a framebuffer => SetCurrentRenderTarget must be called before SetCurrentDepthBuffer !!!
			if (gluiSurface)
			{
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, gluiSurface);
				QDT_TEST_GL_ERROR();
			}
		}
		else
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gluiSurface);
			QDT_TEST_GL_ERROR();

			QDT_ASSERT(pRenderTarget->GetPixelFormat() == PF_D24S8)
			QDT_ASSERT(_pCurrentRenderTarget == _pFrontBuffer);
		}
	}	
	else
	{
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
		QDT_TEST_GL_ERROR();
	}


	_pCurrentDepthBuffer = pRenderTarget;

	_RenderStats.AddSetDepthBuffer(1);

/*
	QDT_ASSERT(pRenderTarget);

	//if (_pCurrentDepthBuffer != pRenderTarget)
	{
		GLuint gluiSurface = *(GLuint*) pRenderTarget->GetSurface();

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, gluiSurface);
		QDT_TEST_GL_ERROR();

		// attach a renderbuffer to a framebuffer => SetCurrentRenderTarget must be called before SetCurrentDepthBuffer !!!
		if (gluiSurface)
		{
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, gluiSurface);
			QDT_TEST_GL_ERROR();
		}
	}
*/}
	
//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetAddressUType(UInt32 nStage, 
										  ADDRESS_TYPE eAddressType)
{
	if (_aAddressUType[nStage] != eAddressType)
	{
		Int32 bindingTarget = _aCurrentTexturesTarget[nStage];

		_aAddressUType[nStage] = eAddressType;

		glActiveTextureARB(GL_TEXTURE0 + nStage);
		glTexParameterf(bindingTarget, GL_TEXTURE_WRAP_S, OGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetAddressVType(UInt32 nStage, 
										  ADDRESS_TYPE eAddressType)
{
	if (_aAddressVType[nStage] != eAddressType)
	{
		_aAddressVType[nStage] = eAddressType;

		Int32 bindingTarget = _aCurrentTexturesTarget[nStage];

		glActiveTextureARB(GL_TEXTURE0 + nStage);
		glTexParameterf(bindingTarget, GL_TEXTURE_WRAP_T, OGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetFilterType(UInt32 nStage, 
										  FILTER_TYPE eFilterType)
{
	Int32 bindingTarget = _aCurrentTexturesTarget[nStage];
	
	if (_aFilterType[nStage] != eFilterType)
	{
		_aFilterType[nStage] = eFilterType;

		glActiveTextureARB(GL_TEXTURE0 + nStage);

		switch (eFilterType)
		{
		default:
			QDT_ASSERT(0);

		case FT_NONE:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			QDT_TEST_GL_ERROR();
			break;

		case FT_BILINEAR:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;

		case FT_TRILINEAR:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f); // Disabled!
			QDT_TEST_GL_ERROR();
			break;

		case FT_ANISOTROPIC_LEVEL_2:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			QDT_TEST_GL_ERROR();
			
			glTexParameterf(bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f);
			QDT_TEST_GL_ERROR();
			break;

		case FT_ANISOTROPIC_LEVEL_4:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			QDT_TEST_GL_ERROR();
			
			glTexParameterf(bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
			QDT_TEST_GL_ERROR();
			break;

		case FT_ANISOTROPIC_LEVEL_8:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			QDT_TEST_GL_ERROR();
			
			glTexParameterf(bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
			QDT_TEST_GL_ERROR();
			break;

		case FT_ANISOTROPIC_LEVEL_16:
			glTexParameterf(bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			QDT_TEST_GL_ERROR();

			glTexParameterf(bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			QDT_TEST_GL_ERROR();
			
			glTexParameterf(bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
			QDT_TEST_GL_ERROR();
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsFullScreen
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
Bool   RENDER_CONTEXT_OGL_PC::IsFullScreen() const
{
	return _bFullScreen;
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenWidth
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_OGL_PC::GetScreenWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenHeight
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_OGL_PC::GetScreenHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthTest
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::EnableDepthTest(bool	bState)
{
	if (_bEnableDepthTest != bState)
	{
		_bEnableDepthTest = bState;

		if (_bEnableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);	
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDepthTestFunc
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetDepthTestFunc(COMP_FUNC	CF)
{
	if (CF != _DepthTestFunc)
	{
		_DepthTestFunc = CF;
		glDepthFunc(OGLCmpFonc[CF]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthWrite
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::EnableDepthWrite(bool	bState)
{
/*	if (_bDepthWrite != bState)
	{
		_bDepthWrite = bState;

		if (_bDepthWrite)
		{
			glEnable(GL_DEPTH_WRITE);	
		}
		else
		{
			glDisable(GL_DEPTH_WRITE);
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		EnableScissorTest
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::EnableScissorTest(Bool bState)
{
	if (_bScissorTest != bState)
	{
		_bScissorTest = bState;

		if (_bScissorTest)
		{
			glEnable(GL_SCISSOR_TEST);	
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetScissorRect
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetScissorRect(UInt32 x, 
										 UInt32 y, 
										 UInt32 width, 
										 UInt32 height)
{
	glScissor(x, y, width, height);
}

//-----------------------------------------------------------------------------
//	Name:		CopySurface
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::CopySurface(I_RENDER_TARGET * pSrc, 
									    I_RENDER_TARGET * pDst)
{
	I_RENDER_TARGET * pOldRenderTarget = GetCurrentRenderTarget();
	SetCurrentRenderTarget(pSrc);

	//glCopyTexSubImage2DEXT();

	// Restore render target
	SetCurrentRenderTarget(pOldRenderTarget);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBuffer
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::SetVertexBuffer(UInt32 stream, 
										  I_VERTEX_BUFFER * pVertexBuffer)
{
	QDT_ASSERT(stream < MAX_VERTEX_STREAMS);
	if (_apCurrentVertexBuffer[stream] != pVertexBuffer)
	{
		_apCurrentVertexBuffer[stream] = pVertexBuffer;
		_bNeedUpdateVertexContext = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_OGL_PC::SetIndexBuffer(I_INDEX_BUFFER * pIndexBuffer)
{
	if (_pCurrentIndexBuffer != pIndexBuffer)
	{
		_pCurrentIndexBuffer = pIndexBuffer;
		_bNeedUpdateIndexContext = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLPrimitiveType
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_OGL_PC::GetOGLPrimitiveType(PRIMITIVE_TYPE PrimitiveType) const
{
	return (OGLPrimitiveTypeArray[PrimitiveType]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNumIndex
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_OGL_PC::GetNumIndices(PRIMITIVE_TYPE eType, 
										UInt32 nPrimitiveCount) const
{
	switch (eType)
	{
	default:
		QDT_FAIL();
		return (0);

	case PT_POINT_LIST:
		return (nPrimitiveCount);

	case PT_LINE_LIST:
		return (nPrimitiveCount * 2);

	case PT_LINE_STRIP:
		return (nPrimitiveCount + 1);

	case PT_TRIANGLE_LIST:
		return (nPrimitiveCount * 3);

	case PT_TRIANGLE_STRIP:
	case PT_TRIANGLE_FAN:
		return (nPrimitiveCount + 2);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateVertexContext
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_OGL_PC::UpdateVertexContext()
{
	//if (_bNeedUpdateVertexContext == true)
	{
		_bNeedUpdateVertexContext = false;

		VERTEX_DECLARATION_OGL_PC * pVertDecl = static_cast<VERTEX_DECLARATION_OGL_PC *>(_pCurrentVertexDeclaration);
		
		S_VERTEX_ELEMENT * pVertElt;
		UInt32 nNumElts;

		pVertDecl->GetDeclaration(pVertElt, nNumElts);

		for (UInt32 i = 0 ; i < nNumElts ; ++i)
		{
			UInt8 nStream = pVertElt[i]._nStream;

			VERTEX_BUFFER_OGL_PC * pVertBuffer = static_cast<VERTEX_BUFFER_OGL_PC *>(_apCurrentVertexBuffer[nStream]);

			QDT_ASSERT(pVertBuffer && "Renderer : You can draw anything if you have not set a stream defined in the vertex declaration!"); 
		
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, pVertBuffer->GetOGLIndex());
			QDT_TEST_GL_ERROR();

			GLubyte * pOffset = (GLubyte *) NULL + (pVertElt[i]._nOffset);

#ifdef QDT_DONT_USE_CG_SET_CONSTANTS
			
			// In this case, we have to use NV extension!

			UInt32 uiIndex = 0;

			switch (pVertElt[i]._nUsage)
			{
			default:
				QDT_FAIL();

			case DU_POSITION:
				uiIndex = 0;
				break;

			case DU_NORMAL:
				uiIndex = 2;
				break;

			case DU_COLOR:
				uiIndex = 3;
				break;

			case DU_TEXCOORD0:
			case DU_TEXCOORD1:
			case DU_TEXCOORD2:
			case DU_TEXCOORD3:
			case DU_TEXCOORD4:
			case DU_TEXCOORD5:
			case DU_TEXCOORD6:
			case DU_TEXCOORD7:
				uiIndex = 8 + pVertElt[i]._nUsage - DU_TEXCOORD0;
				break;
			}

			glEnableClientState( GL_VERTEX_ATTRIB_ARRAY0_NV  + uiIndex);
	
			switch (pVertElt[i]._nType)
			{
			default:
				QDT_ASSERT("Unsupported type for DU_POSITION! Please use DT_FLOAT3, DT_FLOAT4 or DT_FLOAT16_4!");
				break;

			case DT_COLOR:
				glVertexAttribPointerNV(uiIndex, 4, GL_UNSIGNED_BYTE, pVertBuffer->GetVertexStride(), pOffset);
				QDT_TEST_GL_ERROR();
				break;

			case DT_FLOAT2:
				glVertexAttribPointerNV(uiIndex, 2, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
				QDT_TEST_GL_ERROR();
				break;

			case DT_FLOAT3:	
				glVertexAttribPointerNV(uiIndex, 3, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
				QDT_TEST_GL_ERROR();
				break;

			case DT_FLOAT4:
				glVertexAttribPointerNV(uiIndex, 4, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
				QDT_TEST_GL_ERROR();
				break;

			case DT_FLOAT16_2:
				glVertexAttribPointerNV(uiIndex, 2, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
				QDT_TEST_GL_ERROR();
				break;

			case DT_FLOAT16_4:
				glVertexAttribPointerNV(uiIndex, 4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
				QDT_TEST_GL_ERROR();
				break;
			}
#else
			switch (pVertElt[i]._nUsage)
			{
			default:
				QDT_FAIL();

			case DU_POSITION:
				{
					glEnableClientState(GL_VERTEX_ARRAY);

					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT("Unsupported type for DU_POSITION! Please use DT_FLOAT3, DT_FLOAT4 or DT_FLOAT16_4!");
						break;
					
					case DT_FLOAT3:	
						glVertexPointer(3, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT4:
						glVertexPointer(4, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT16_4:
						glVertexPointer(4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;
					}					
				}
				break;
			
			case DU_NORMAL:
				{
					glEnableClientState(GL_NORMAL_ARRAY);

					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT("Unsupported type for DU_NORMAL! Please use DT_FLOAT3 or DT_FLOAT16_4!");
						break;

					case DT_FLOAT3:
						glNormalPointer(GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT16_4:
						glNormalPointer(GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;
					}					
				}
				break;

			case DU_COLOR:
				{
					glEnableClientState(GL_COLOR_ARRAY);
					
					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT("Unsupported type for DU_COLOR! Please use DT_COLOR, DT_FLOAT4 or DT_FLOAT16_4!");
						break;

					case DT_COLOR:
						glColorPointer(4, GL_UNSIGNED_BYTE, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT4:
						glColorPointer(4, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT16_4:
						glColorPointer(4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;
					}					
				}
				break;

			case DU_TEXCOORD0:
			case DU_TEXCOORD1:
			case DU_TEXCOORD2:
			case DU_TEXCOORD3:
			case DU_TEXCOORD4:
			case DU_TEXCOORD5:
			case DU_TEXCOORD6:
			case DU_TEXCOORD7:
				{
					UInt32 nTextureIndex = pVertElt[i]._nUsage - DU_TEXCOORD0;

					// Important to choose the right uv
					glClientActiveTextureARB(GL_TEXTURE0 + nTextureIndex);					

					glEnableClientState(GL_TEXTURE_COORD_ARRAY);

					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT("Unsupported type for DU_TEXCOORDn! Please use everything except DT_FLOAT1!");
						break;

					case DT_FLOAT2:
						glTexCoordPointer(2, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT3:
						glTexCoordPointer(3, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT4:
						glTexCoordPointer(4, GL_FLOAT, pVertBuffer->GetVertexStride(),pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT16_2:
						glTexCoordPointer(2, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_FLOAT16_4:
						glTexCoordPointer(4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;

					case DT_COLOR:
						glTexCoordPointer(4, GL_BYTE, pVertBuffer->GetVertexStride(), pOffset);
						QDT_TEST_GL_ERROR();
						break;
					}					
				}
				break;
			}
#endif
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateIndexContext
//	Object:		
//	05-09-16:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::UpdateIndexContext()
{
	if (_bNeedUpdateIndexContext == true)
	{
		_bNeedUpdateIndexContext = false;

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ((INDEX_BUFFER_OGL_PC *)_pCurrentIndexBuffer)->GetOGLIndexBuffer());
		QDT_TEST_GL_ERROR();
	}	
}

//-----------------------------------------------------------------------------
//	Name:		EnableAlphaTest
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::EnableAlphaTest(Bool	bState)
{
	if (bState != _bEnableAlphaTest)
	{
		_bEnableAlphaTest = bState;		

		if (bState)
		{
			glEnable(GL_ALPHA_TEST);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
		}

		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestFunc
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetAlphaTestFunc(COMP_FUNC	CF)
{
	if (CF != _AlphaTestFunc)
	{
		_AlphaTestFunc = CF;
		glAlphaFunc(OGLCmpFonc[_AlphaTestFunc], ((Float32)_nAlphaTestRef) / 255.f);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestRef
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetAlphaTestRef(UInt8	Ref)
{
	if (Ref != _nAlphaTestRef)
	{
		_nAlphaTestRef = Ref;
		glAlphaFunc(OGLCmpFonc[_AlphaTestFunc], ((Float32)_nAlphaTestRef) / 255.f);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsDisplayWindowActive
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_OGL_PC::IsDisplayWindowActive() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		HasDisplayInputFocus
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_OGL_PC::HasDisplayInputFocus() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		CreateAdditionalSwapChain
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
I_SWAP_CHAIN *	RENDER_CONTEXT_OGL_PC::CreateAdditionalSwapChain(HWND	hWnd)
{
	QDT_NOT_IMPLEMENTED();
	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowSize
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::ChangeWindowSize(UInt32	uiWidth,
												UInt32	uiHeight)
{
	if	(_bFullScreen == false && _uiWidth != uiWidth && _uiHeight && uiHeight)
	{
		_uiWidth = uiWidth;
		_uiHeight = uiHeight;

		delete _pFrontBuffer;
		delete _pDepthBuffer;

		RetrieveFrontAndDepthBuffer();
	}

	_uiWidth = uiWidth;
	_uiHeight = uiHeight;
}

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowPos
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::ChangeWindowPos(UInt32	uiX,
											   UInt32	uiY)
{
	_uiPosX = uiX;
	_uiPosY = uiY;
}

//-----------------------------------------------------------------------------
//	Name:		SetFullScreen
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetFullScreen(bool	bFullScreen)
{
	// Dont managed in opengl
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRenderModes
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_OGL_PC::GetNbrRenderModes()
{
	QDT_NOT_IMPLEMENTED();
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderMode
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
const S_RENDER_MODE &	RENDER_CONTEXT_OGL_PC::GetRenderMode(UInt32	i)
{
	QDT_NOT_IMPLEMENTED();
	return _pRenderModesArray[i];
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderWindow
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
HWND	RENDER_CONTEXT_OGL_PC::GetRenderWindow()
{
	return _hWindow;
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosX
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_OGL_PC::GetWindowPosX()
{
	QDT_NOT_IMPLEMENTED();
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosY
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_OGL_PC::GetWindowPosY()
{
	QDT_NOT_IMPLEMENTED();
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		SupportShaderModel
//	Object:		
//	05-12-14:	SBE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_OGL_PC::SupportShaderModel(SHADER_MODEL	sm)
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		LineContains
//	Object:		
//	05-12-16:	SBE - Created
//-----------------------------------------------------------------------------
Bool LineContains(UInt8 * pShader, UInt8 nChar)
{
	while (*pShader != '\n')
	{
		if (*pShader == nChar)
			return true;

		pShader++;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		CleanShader
//	Object:		
//	05-12-16:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_OGL_PC::CleanShader(UInt8 *	pShader,
										   UInt8 *	pCleanedShader,
										   UInt32	nFileSize)
{
	UInt32 iCur = 0, iCleanedCur = 0;
	Bool bEndFounded = false;

	while (iCur < nFileSize)
	{
		if (pShader[iCur] == '#') 
		{
			// skip comment
			while (pShader[iCur] != '\n') iCur++;
		}
		else if (LineContains(&pShader[iCur], ':'))
		{
			// skip line
			while (pShader[iCur] != '\n') iCur++;
		}
		else if (pShader[iCur] == '\n')
		{
		}
		else if ((pShader[iCur-2] == 'E') && (pShader[iCur-1] == 'N') && (pShader[iCur] == 'D'))
		{
			pCleanedShader[iCleanedCur] = pShader[iCur];
			iCleanedCur++;
			bEndFounded = true;
			break;
		}
		else if (pShader[iCur] == '!') 
		{
			// skip FP/VP type
			while (pShader[iCur] != '\n') pCleanedShader[iCleanedCur++] = pShader[iCur++];
			pCleanedShader[iCleanedCur] = pShader[iCur];
			iCleanedCur++;
		}
		else
		{
			pCleanedShader[iCleanedCur] = pShader[iCur];
			iCleanedCur++;
		}

		iCur++;
	}

	QDT_ASSERT(bEndFounded);	

	return iCleanedCur;
}

//-----------------------------------------------------------------------------
//	Name:		SaveStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_OGL_PC::SaveStates()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS); 
	QDT_TEST_GL_ERROR();

	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	QDT_TEST_GL_ERROR();
}
		
//-----------------------------------------------------------------------------
//	Name:		RestoreStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::RestoreStates()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	QDT_TEST_GL_ERROR();

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	QDT_TEST_GL_ERROR();

	SetVertexShader(NULL);
	SetPixelShader(NULL);

	// Also disable vertex & pixels shaders
	cgGLUnbindProgram(_CgVertexShaderProfile);
	QDT_TEST_CG_ERROR();
	cgGLDisableProfile(_CgVertexShaderProfile);
	QDT_TEST_CG_ERROR();
	cgGLUnbindProgram(_CgPixelShaderProfile);
	QDT_TEST_CG_ERROR();
	cgGLDisableProfile(_CgPixelShaderProfile);
	QDT_TEST_CG_ERROR();

	SetCurrentRenderTarget(_pFrontBuffer);
	SetCurrentDepthBuffer(_pDepthBuffer);

	glPopAttrib(); 
	QDT_TEST_GL_ERROR();

	glPopClientAttrib();
	QDT_TEST_GL_ERROR();
}
	
//-----------------------------------------------------------------------------
//	Name:		CleanStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::CleanStates()
{
	SetDefaultValues();
}

//-----------------------------------------------------------------------------
//	Name:		SetDefaultValues
//	Object:		
//	06-01-23:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::SetDefaultValues()
{
	_iViewportX = 0;
	_iViewportY = 0;
	_iViewportWidth = 0;
	_iViewportHeight = 0;

	_bScissorTest = false; // Default value for OGL
	_bNeedUpdateVertexContext = true;

	glBlendFunc(GL_ONE, GL_ZERO);
	glBlendEquation(GL_FUNC_ADD_EXT);
	_eSrcBlendMode = BM_ONE;
	_eDstBlendMode = BM_ZERO;
	_eBlendOp = BOP_ADD;
	glDisable(GL_BLEND);
	_bEnableAlphaBlend = false;

	// default Fill Mode
	_eFillMode = FM_LINE;
	SetFillMode(FM_SOLID);

	// default Depth Mode
	_bEnableDepthTest = false;
	EnableDepthTest(true);
	_DepthTestFunc = CF_NEVER;
	SetDepthTestFunc(CF_LESSEQUAL);
	_bEnableDepthWrite = false;
	EnableDepthWrite(true);	

	// Set default alpha test
	_AlphaTestFunc = CF_GREATER;
	_nAlphaTestRef = 0;
	glAlphaFunc(OGLCmpFonc[_AlphaTestFunc], ((Float32)_nAlphaTestRef) / 255.f);

	// default Cull Mode
	_eCullMode = CULL_CLOCKWISE;
	SetCullMode(CULL_NONE);

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading

	_pCurrentVertexDeclaration = NULL;
	_pCurrentIndexBuffer = NULL;
	for (UInt32 i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		_apCurrentVertexBuffer[i] = NULL;
	}

	for (UInt32 i = 0 ; i < MAX_TEXTURE_STAGES ; ++i)
	{
		_apCurrentTextures[i] = NULL;
		_aCurrentTexturesTarget[i] = 0;
	}

	_pCurrentVertexShader = NULL;
	_pCurrentPixelShader = NULL;

	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		ChangeFullScreenSize
//	Object:		
//	06-01-25:	RMA - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_OGL_PC::ChangeFullScreenSize(UInt32	uiWidth,
													UInt32	uiHeight)
{
	// Fullscreen isn't supported in OpenGL in our renderer!
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShaderFromCgProgram
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT_OGL_PC::CreateVertexShaderFromCgProgram(void *	pCgProgram)
{
	CGprogram * pPrg = static_cast<CGprogram *>(pCgProgram);
	return (new VERTEX_SHADER_OGL_PC(*pPrg));
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShaderFromCgProgram
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	RENDER_CONTEXT_OGL_PC::CreatePixelShaderFromCgProgram(void *	pCgProgram)
{
	CGprogram * pPrg = static_cast<CGprogram *>(pCgProgram);
	return (new PIXEL_SHADER_OGL_PC(*pPrg));
}

//-----------------------------------------------------------------------------
//	Name:		CreateTextureFromID
//	Object:		
//	06-02-09:	FBO - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_OGL_PC::CreateTextureFromID(void *	pID, 
													   UInt32 iWidth, 
													   UInt32 iHeight)
{
	GLuint * pglID = static_cast<GLuint *>(pID);
	return (new TEXTURE_OGL_PC(*pglID, iWidth, iHeight));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeTextureFromID
//	Object:		
//	06-03-03:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_OGL_PC::CreateCubeTextureFromID(void *	pID,
														   UInt32 iLength)
{
	GLuint * pglID = static_cast<GLuint *>(pID);
	return (new CUBE_TEXTURE_OGL_PC(*pglID, iLength));
}

//-----------------------------------------------------------------------------
//	Name:		SupportFloatingPointBlending
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_OGL_PC::SupportFloatingPointBlending()
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		ClearRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_OGL_PC::ClearRenderStats()
{
	_RenderStats.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
RENDER_STATS &	RENDER_CONTEXT_OGL_PC::GetRenderStats()
{
	return (_RenderStats);
}

//-----------------------------------------------------------------------------
//	Name:		GetPlatformIntColor
//	Object:		
//	06-03-23:	VMA - Created
//-----------------------------------------------------------------------------

UInt32	RENDER_CONTEXT_OGL_PC::GetPlatformIntColor(const COLOR &	Color)
{
	return (Color.GetABGRValue());
}

//-----------------------------------------------------------------------------
//	Name:		SupportHardwareShadowMap
//	Object:		
//	06-03-20:	SBE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_OGL_PC::SupportHardwareShadowMap()
{
	// toclean...
	return false;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

