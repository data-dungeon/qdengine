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
// CLASS: RENDER_CONTEXT_PSGL
//
//
//	05-08-30:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "RenderContext_PSGL.h"
#include INCL_KCORE(Time/Time_ABC)
#define QDT_INCLUDE_STDIO
#include INCL_KCORE(Includes/Includes)
#define QDT_INCLUDE_PS3_DEBUGFONT
#include INCL_KCORE(Includes_PS3/Includes_PS3)
#include INCL_KCORE(Tools/QdtString)
#include INCL_KCORE(Core/Endian)
#include INCL_KCORE(File/FilesManagerABC)
#include INCL_KCORE(File/PS3/FilePS3Host)
#include INCL_KCORE(Profiler/Profiler)
#include INCL_KINPUT(Input/PS3/PadManager_PS3)
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer\PSGL\RenderTarget_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\VertexBuffer_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\IndexBuffer_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\VertexShader_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\PixelShader_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\VertexDeclaration_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\Texture_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\CubeTexture_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\VolumeTexture_PSGL)
#include INCL_KRENDERER(Renderer\PSGL\CubeRenderTarget_PSGL)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Vector\Vector)
#include INCL_KMATH(Math\Geometry\Matrix\Matrix)
#include INCL_KMATH(Math\Color\ColorFloat)

#define ENABLE_FAST_SET_SHADER_CONSTANTS
#define QDT_MessageLogRenderer if (0) QDT_Message
	
	
	
	
	//QDT_Message

//=============================================================================
// CODE STARTS HERE
//=============================================================================

static UInt32 PSGLCmpFonc[8] = 
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

static UInt32 PSGLBlendMode[11] = 
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

static UInt32 PSGLBlendOp[5] = 
{
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX,
};

static UInt32 PSGLCullModes[3] = 
{
	GL_CCW,
	GL_CW
};

static UInt32 PSGLFillModes[2] = 
{
	GL_LINE,
	GL_FILL,
};

static UInt32 PSGLPrimitiveTypeArray[6] = 
{
	GL_POINT,
	GL_LINES,
	GL_LINE_STRIP,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
};

static char PSGLRegisterNames[256][16];
static char PSGLTextureNames[16][16];

void QDPSGLreportFunction(GLenum, GLuint , const char * pMsg)
{
	QDT_STRING s;
	s.Format("PSGL-Report:%s", pMsg);

	QDT_Message("%s\n", s.GetBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_PSGL Constructor
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
RENDER_CONTEXT_PSGL::RENDER_CONTEXT_PSGL(DISPLAY_MODE DisplayMode)
:
_pCurrentVertexShader(NULL),
_pCurrentPixelShader(NULL),
_pFrontBuffer(NULL),
_pDepthBuffer(NULL),
_pCurrentRenderTarget(NULL),
_nCurrentRenderTargetCubeFace(CF_NONE),
_pCurrentDepthBuffer(NULL),
_fFPS(0.f),
_fTimeStartFrame(0.f),
_fTimeEndFrame(0.f),
_fTimeLastMessage(0.f),
_fTimeEndSwap(0.f),
_fFrameDuration(0.f),
_nNbVertices(0),
_DisplayMode(DisplayMode)
{
	switch (_DisplayMode)
	{
		default:
			_uiWidth = 1024;
			_uiHeight = 768;
		break;
		case DM_720P:
			_uiWidth  = 1280;
			_uiHeight = 720;
		break;
		case DM_1080I:
			_uiWidth  = 1920;
			_uiHeight = 1080;
		break;
		case DM_1080P:
			_uiWidth  = 1920;
			_uiHeight = 1080;
		break;
		case DM_480I:
			_uiWidth  = 720;
			_uiHeight = 480;
		break;
		case DM_576I:
			_uiWidth  = 720;
			_uiHeight = 576;
		break;
	}

	_pDevice = NULL;
	_cgContext = NULL;

	_aSurfaces[0] = 0;
	_aSurfaces[1] = 0;
	_aSurfaces[2] = 0;
	_nCurrentSurface = 0;

	_iViewportX = 0;
	_iViewportY = 0;
	_iViewportWidth = 0;
	_iViewportHeight = 0;

	_bScissorTest = false; // Default value for PSGL

	_bNeedUpdateVertexContext = false;

	_pCurrentVertexDeclaration = NULL;
	for (UInt32 i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		_apCurrentVertexBuffer[i] = NULL;
	}

	for (UInt32 i = 0 ; i < MAX_TEXTURE_STAGES ; ++i)
	{
		_apCurrentTextures[i] = NULL;
		_aCurrentTexturesTarget[i] = 0;
	}

//#ifndef ENABLE_FAST_SET_SHADER_CONSTANTS // a remettre quand skinning validé : voir "A TESTER !! (skinning)"
	for (UInt32 i = 0 ; i < 256 ; ++i)
	{
		sprintf( PSGLRegisterNames[i], "register%d", i );
	}

	for (UInt32 i = 0 ; i < 16 ; ++i)
	{
		sprintf( PSGLTextureNames[i], "texture%d", i );
	}
//#endif


	Init();

	RENDER_TARGET_PSGL * pFrontBuffer = new RENDER_TARGET_PSGL;
	pFrontBuffer->_Texture._uiWidth = _uiWidth;
	pFrontBuffer->_Texture._uiHeight = _uiHeight;
	pFrontBuffer->_Texture._eFormat = PF_A8R8G8B8;
//    _pD3DDevice->GetRenderTarget(0, &pFrontBuffer->_pD3DSurface);
	_pCurrentRenderTarget = _pFrontBuffer = pFrontBuffer;

	RENDER_TARGET_PSGL * pDepthBuffer = new RENDER_TARGET_PSGL;
	pDepthBuffer->_Texture._uiWidth = _uiWidth;
	pDepthBuffer->_Texture._uiHeight = _uiHeight;
	pDepthBuffer->_Texture._eFormat = PF_D24S8;
//    _pD3DDevice->GetDepthStencilSurface(&pDepthBuffer->_pD3DSurface);
	_pCurrentDepthBuffer = _pDepthBuffer = pDepthBuffer;

	// default Fill Mode
	_eFillMode = FM_LINE;
	SetFillMode(FM_SOLID);

	// default Depth Mode
	_bEnableDepthTest = false;
	EnableDepthTest(true);
	_DepthTestFunc = CF_NEVER;
	SetDepthTestFunc(CF_LESSEQUAL);
	_bEnableDepthWrite = true;
	glDepthMask(true);	

	// color write
	glColorMask(true, true, true, true);
	_bColorWriteR = true;
	_bColorWriteG = true;
	_bColorWriteB = true; 
	_bColorWriteA = true;

	// default Alpha Mode
	_bEnableAlphaBlend = false;
	_eSrcBlendMode = BM_ONE;
	_eDstBlendMode = BM_ONE;
	_eBlendOp = BOP_ADD;
	SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);

	// default Cull Mode
	_eCullMode = CULL_CLOCKWISE;
	SetCullMode(CULL_NONE);
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_PSGL Destructor
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
RENDER_CONTEXT_PSGL::~RENDER_CONTEXT_PSGL()
{
	ShutDebugFont();
}

//-----------------------------------------------------------------------------
//	Name:		BeginScene
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::BeginScene()
{
	QDT_MessageLogRenderer("BeginScene()");
	// Do nothing!!
}

//-----------------------------------------------------------------------------
//	Name:		BeginScene
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::EndScene()
{
	QDT_MessageLogRenderer("EndScene()");
	// Do nothing!!
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::Init()
{
	// memory wrapper
	psglSetAllocatorFuncs(&Malloc, &MemAlign, &Realloc, &Free);

	_Options.enable = PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS | PSGL_INIT_FIFO_SIZE;
	_Options.maxSPUs = 1;
	_Options.initializeSPUs = GL_FALSE;

	// We're not specifying values for these options, the code is only here
	// to alleviate compiler warnings.
	_Options.persistentMemorySize = 0;
	_Options.transientMemorySize = 0;
	_Options.errorConsole = 0;
	_Options.fifoSize = 512*1024;

	psglInit(&_Options);
	SetupRenderer();
	MakeNewContext();
	ResetContext();
	InitDebugFont();

	psglSetReportFunction(QDPSGLreportFunction);
	psglEnableReport(PSGL_REPORT_ALL);
	psglSetDefaultReportParameters(true);
}

//-----------------------------------------------------------------------------
//	Name:		SetupRenderer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
Int32 RENDER_CONTEXT_PSGL::SetupRenderer()
{
	// Next, create the device
	PSGLbufferParameters glBufferParams={
		width        : _uiWidth,
		height       : _uiHeight,
		colorBits    : 24,
		alphaBits    : 8,
		depthBits    : 24,
		stencilBits  : 8,
		deviceType   : PSGL_DEVICE_TYPE_AUTO,
		TVStandard   : PSGL_TV_STANDARD_NONE,
		TVFormat     : PSGL_TV_FORMAT_AUTO,
		bufferingMode: PSGL_BUFFERING_MODE_DOUBLE,
		antiAliasing : GL_FALSE,
	};

	switch (_DisplayMode)
	{
		default:
			QDT_Message("Display Mode = DEFAULT !!!");
		break;
		case DM_720P:
			glBufferParams.deviceType = PSGL_DEVICE_TYPE_TV;
			glBufferParams.TVStandard = PSGL_TV_STANDARD_HD720P;
			glBufferParams.TVFormat = PSGL_TV_FORMAT_YPRPB;
			QDT_Message("Display Mode = DM_720P");
		break;
		case DM_1080I:
			glBufferParams.deviceType = PSGL_DEVICE_TYPE_TV;
			glBufferParams.TVStandard = PSGL_TV_STANDARD_HD1080I;
			glBufferParams.TVFormat = PSGL_TV_FORMAT_YPRPB;
			QDT_Message("Display Mode = DM_1080I");
		break;
		case DM_1080P:
			glBufferParams.deviceType = PSGL_DEVICE_TYPE_TV;
			glBufferParams.TVStandard = PSGL_TV_STANDARD_HD1080P;
			glBufferParams.TVFormat = PSGL_TV_FORMAT_YPRPB;
			QDT_Message("Display Mode = DM_1080P");
		break;
		case DM_480I:
			glBufferParams.deviceType = PSGL_DEVICE_TYPE_TV;
			glBufferParams.TVStandard = PSGL_TV_STANDARD_HD480I;
			glBufferParams.TVFormat = PSGL_TV_FORMAT_AUTO;
			QDT_Message("Display Mode = DM_480I");
		break;
		case DM_576I:
			glBufferParams.deviceType = PSGL_DEVICE_TYPE_TV;
			glBufferParams.TVStandard = PSGL_TV_STANDARD_HD576I;
			glBufferParams.TVFormat = PSGL_TV_FORMAT_AUTO;
			QDT_Message("Display Mode = DM_576I");
		break;
	}

	_pDevice = psglCreateDevice(&glBufferParams);

	if (_pDevice == NULL) return 1;
	return MakeNewContext();
}

//-----------------------------------------------------------------------------
//	Name:		MakeNewContext
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
Int32 RENDER_CONTEXT_PSGL::MakeNewContext()
{
	if (_cgContext != NULL) cgDestroyContext(_cgContext);

	PSGLcontext* oldContext = psglGetCurrentContext();
	if (oldContext != NULL)
	{
		psglDestroyContext(oldContext);
	}

	// Now create a PSGL context
	PSGLcontext *pContext = psglCreateContext();
	QDT_ENSURE(pContext != NULL);

	// Make this context current for the device we initialized
	psglMakeCurrent(pContext, _pDevice);

	// Since we're using fixed function stuff (i.e. not using our own shader
	// yet), we need to load shaders.bin that contains the fixed function 
	// shaders.
	//psglLoadShaderLibrary("/app_home/shaders.bin");

	ResetContext();

	_cgContext = cgCreateContext();
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		ResetContext
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::ResetContext()
{
	QDT_MessageLogRenderer("Resetcontext %d %d", _uiWidth, _uiHeight);
	// Reset the context
	psglResetCurrentContext();
	
	SetViewport(0, 0, _uiWidth-1, _uiHeight-1);
	Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, 0, 0, 0, 0, 1.0f);
	SetScissorRect(0, 0, _uiWidth-1, _uiHeight-1);
	
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_VSYNC_SCE);
	
	Present();
}

//-----------------------------------------------------------------------------
//	Name:		InitDebugFont
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::InitDebugFont()
{
	CellDbgFontConfig cfg;
	cfg.bufSize = 1024;
	cfg.screenWidth = _uiWidth;
	cfg.screenHeight = _uiHeight;
	cellDbgFontInit(&cfg);

	CellDbgFontConsoleConfig ccfg;
	ccfg.posLeft = 0.1f;
	ccfg.posTop  = 0.1f;
	ccfg.cnsWidth = 50;
	ccfg.cnsHeight = 10;
	ccfg.scale = 1.2f;
	ccfg.color = 0xFFFFFFFF;
	cellDbgFontConsoleOpen(&ccfg);
}

//-----------------------------------------------------------------------------
//	Name:		ShutDebugFont
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::ShutDebugFont()
{
	cellDbgFontConsoleClose(0);

	cellDbgFontExit();
}

//-----------------------------------------------------------------------------
//	Name:		DisplayDebugFont
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::DisplayDebugFont()
{
	cellDbgFontDraw();
}

//-----------------------------------------------------------------------------
//	Name:		PrintDebugMessage
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::PrintDebugMessage(Float32		fXPos,
											   Float32		fYPos,
											   const Char *	Text,
											   ...			)
{
	va_list	Args;
	
	va_start(Args, Text);
	PrintDebugMessageV(fXPos, fYPos, Text, (void*)Args);
	va_end(Args);
}

//-----------------------------------------------------------------------------
//	Name:		PrintDebugMessageV
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::PrintDebugMessageV(Float32		fXPos,
											   Float32		fYPos,
											   const Char *	Text,
											   void*		Args)
{
	cellDbgFontVprintf(fXPos, fYPos, 1.2f, 0xFFFFFFFF, Text, (va_list)Args);
}

//-----------------------------------------------------------------------------
//	Name:		PrintConsoleMessage
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::PrintConsoleMessage(const Char *	Text,
												 ...			)
{
	va_list	Args;

	va_start(Args, Text);
	PrintConsoleMessageV(Text, (void*)Args);
	va_end(Args);
}

//-----------------------------------------------------------------------------
//	Name:		PrintConsoleMessageV
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::PrintConsoleMessageV(const Char *	Text,
												 void*			Args)
{
	Char	Buffer[64];

	QDT_ASSERT(strlen(Text) < 63);
	sprintf(Buffer, "%s\n", Text);
	
	cellDbgFontConsoleVprintf(0, Buffer, (va_list)Args);
}

//-----------------------------------------------------------------------------
//	Name:		Present
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::Present()
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	BEGIN_PROFILE(PROFILE_VBLANK);

	QDT_MessageLogRenderer("============");
	QDT_MessageLogRenderer("Present %d %d", _uiWidth, _uiHeight);

	// Reset Viewport
	SetViewport(0, 0, _uiWidth, _uiHeight);

	_fTimeEndFrame = TIME::Instance()->GetSystemTime();

	DisplayDebugFont();

	psglSwap();

	QDT_TEST_GL_ERROR();

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddPresentTime(fTime1 - fTime0);
#endif

	if ( PAD_MANAGER_PS3::GetInstance()->GetPad(0)->IsButtonPressed(PAD_PS3::BT_L3) )
	{
		TakeScreenShot("");
	}

	PrintFPS();

	END_PROFILE(PROFILE_VBLANK);
}

//-----------------------------------------------------------------------------
//	Name:		DrawPrimitive
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::DrawPrimitive(PRIMITIVE_TYPE nType, 
										UInt32 nPrimitiveCount, 
										UInt32 nStartVertex)
{
	QDT_MessageLogRenderer("DrawPrimitive %d %d %d", nType, nPrimitiveCount, nStartVertex);

	BEGIN_PROFILE(PROFILE_DRAW_PRIMITIVE);

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

	UInt32 PSGLPrimType = GetPSGLPrimitiveType(nType);
	UInt32 nNumIndices = GetNumIndices(nType, nPrimitiveCount);

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	glDrawArrays(PSGLPrimType, nStartVertex, nNumIndices);
	QDT_TEST_GL_ERROR();

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddDrawPrimitiveTime(fTime1 - fTime0);
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	_RenderStats.AddDrawPrimitive(1);
	_RenderStats.AddPolygones(nPrimitiveCount);

	for ( UInt32 nTextureIndex = 0 ; nTextureIndex < 8 ; nTextureIndex++ )
	{
		glClientActiveTexture(GL_TEXTURE0 + nTextureIndex);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	for (UInt32 iAttrib=0; iAttrib <= 15; ++iAttrib)
	{
		cgGLDisableAttrib(iAttrib);
	}

	_nNbVertices += nNumIndices;

	END_PROFILE(PROFILE_DRAW_PRIMITIVE);
}

//-----------------------------------------------------------------------------
//	Name:		DrawIndexedPrimitive
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::DrawIndexedPrimitive(PRIMITIVE_TYPE nType, 
											   UInt32 nMinIndex, 
											   UInt32 nNumVertices, 
											   UInt32 nStartIndex, 
											   UInt32 nPrimitiveCount)
{
	QDT_MessageLogRenderer("DrawIndexedPrimitive %d %d %d %d %d", nType, nPrimitiveCount, nStartIndex, nMinIndex, nNumVertices);

	BEGIN_PROFILE(PROFILE_DRAW_PRIMITIVE);

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
	UpdateIndexContext();

	UInt32 PSGLPrimType = GetPSGLPrimitiveType(nType);
	UInt32 nNumIndices = GetNumIndices(nType, nPrimitiveCount);

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	glDrawRangeElements(PSGLPrimType, nMinIndex, nMinIndex + nNumVertices - 1, nNumIndices, GL_UNSIGNED_SHORT, (GLvoid*)(nStartIndex * sizeof(UInt16)) );
	QDT_TEST_GL_ERROR();

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddDrawPrimitiveTime(fTime1 - fTime0);
#endif

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	for ( UInt32 nTextureIndex = 0 ; nTextureIndex < 8 ; nTextureIndex++ )
	{
		glClientActiveTexture(GL_TEXTURE0 + nTextureIndex);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	for (UInt32 iAttrib=0; iAttrib <= 15; ++iAttrib)
	{
		cgGLDisableAttrib(iAttrib);
	}

	_nNbVertices += nNumIndices;

	_RenderStats.AddDrawPrimitive(1);
	_RenderStats.AddPolygones(nPrimitiveCount);

	END_PROFILE(PROFILE_DRAW_PRIMITIVE);
}

//-----------------------------------------------------------------------------
//	Name:		DrawPoints
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::DrawPoints(UInt32					uiNbrPoints,
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
void	RENDER_CONTEXT_PSGL::DrawLines(UInt32				uiNbrLines,
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
void	RENDER_CONTEXT_PSGL::DrawTriangles(UInt32				uiNbrTriangles,
										   const DEBUG_VERTEX *	pVertices,
										   const MATRIX &		ViewMatrix,
										   const MATRIX &		ProjectionMatrix)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
I_VERTEX_BUFFER * RENDER_CONTEXT_PSGL::CreateVertexBuffer(UInt32 vertexCount, 
														  UInt32 vertexStride, 
														  Bool bDynamic)
{
	return (new VERTEX_BUFFER_PSGL(vertexCount, vertexStride, bDynamic));
}

//-----------------------------------------------------------------------------
//	Name:		CreateIndexBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
I_INDEX_BUFFER *	RENDER_CONTEXT_PSGL::CreateIndexBuffer(UInt32	nIndexCount)
{
	return (new INDEX_BUFFER_PSGL(nIndexCount));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexDeclaration
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
I_VERTEX_DECLARATION * RENDER_CONTEXT_PSGL::CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements, UInt32 numElements)
{
	return (new VERTEX_DECLARATION_PSGL(pElements, numElements));
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
I_TEXTURE * RENDER_CONTEXT_PSGL::CreateTexture(UInt32		uiWidth, 
											   UInt32		uiHeight, 
											   PIXEL_FORMAT eFormat, 
											   UInt32		uiMipMapLevelNum)
{
	return (new TEXTURE_PSGL(uiWidth, uiHeight, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeTexture
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_PSGL::CreateCubeTexture(UInt32		uiEdgeLength,
												   PIXEL_FORMAT	eFormat,
												   UInt32		uiMipMapLevelNum)
{
	return (new CUBE_TEXTURE_PSGL(uiEdgeLength, eFormat, uiMipMapLevelNum));
}
	
//-----------------------------------------------------------------------------
//	Name:		CreateVolumeTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_PSGL::CreateVolumeTexture(UInt32			uiWidth,
													 UInt32			uiHeight,
													 UInt32			uiDepth,
													 PIXEL_FORMAT	eFormat,
													 UInt32			uiMipMapLevelNum)
{
	return (new VOLUME_TEXTURE_PSGL(uiWidth, uiHeight, uiDepth, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShaderFromFile
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT_PSGL::CreateVertexShaderFromFile(const QDT_STRING & sFileName)
{
	return (new VERTEX_SHADER_PSGL(this, sFileName));
}
	
//-----------------------------------------------------------------------------
//	Name:		CreatePixelShaderFromFile
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	RENDER_CONTEXT_PSGL::CreatePixelShaderFromFile(const QDT_STRING & sFileName)
{
	return (new PIXEL_SHADER_PSGL(this, sFileName));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-11-23:	VMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT_PSGL::CreateVertexShader(UInt8 *	pShader, UInt32 nShaderSize)
{
	return (new VERTEX_SHADER_PSGL(this, pShader, nShaderSize));
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShader
//	Object:		
//	05-11-23:	VMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	RENDER_CONTEXT_PSGL::CreatePixelShader(UInt8 *	pShader, UInt32 nShaderSize)
{
	return (new PIXEL_SHADER_PSGL(this, pShader, nShaderSize));
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderTarget
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------	
I_RENDER_TARGET * RENDER_CONTEXT_PSGL::CreateRenderTarget(UInt32			uiWidth, 
															 UInt32			uiHeight, 
															 PIXEL_FORMAT	eFormat)
{
	return (new RENDER_TARGET_PSGL(this, uiWidth, uiHeight, eFormat));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeRenderTarget
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_PSGL::CreateCubeRenderTarget(UInt32			uiEdgeLength,
																PIXEL_FORMAT	eFormat)
{
	return (new CUBE_RENDER_TARGET_PSGL(this, uiEdgeLength, eFormat));
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetVertexShader(I_VERTEX_SHADER *	pVertexShader)
{
	QDT_MessageLogRenderer("SetVertexShader %x", pVertexShader);

	if (pVertexShader != _pCurrentVertexShader)
	{
#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

		_pCurrentVertexShader = pVertexShader;

		if (pVertexShader != NULL)
		{
			cgGLBindProgram(((VERTEX_SHADER_PSGL *)pVertexShader)->GetPSGLVertexShader());
			cgGLEnableProfile(PLATFORM_PS3_GPU_VP_PROFILE);
		}
		else
		{
			cgGLDisableProfile(PLATFORM_PS3_GPU_VP_PROFILE);
		}

		QDT_TEST_CG_ERROR();

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime1 = TIME::Instance()->GetSystemTime();
		_RenderStats.AddSetVertexShaderTime(fTime1 - fTime0);
#endif

		_RenderStats.AddSetVertexShader(1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderMatrixParameter
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetVertexShaderConstant(UInt32 nRegister,
													 const VECTOR & Vec)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetVertexShaderConstant %d %f %f %f %f", nRegister, Vec.GetX(), Vec.GetY(), Vec.GetZ(), Vec.GetPad());

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetVertexRegister4fv(nRegister, Vec.ToPointer());
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_PSGL *)_pCurrentVertexShader)->GetPSGLVertexShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter3f(cgP, Vec.GetX(), Vec.GetY(), Vec.GetZ());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetVertexShaderConstant(UInt32 nRegister,
													 const MATRIX & m)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetVertexShaderConstant %d %f %f %f %f", nRegister, m(0, 0), m(0, 1), m(0, 2), m(0, 3));
	QDT_MessageLogRenderer("=> %f %f %f %f", m(1, 0), m(1, 1), m(1, 2), m(1, 3));
	QDT_MessageLogRenderer("=> %f %f %f %f", m(2, 0), m(2, 1), m(2, 2), m(2, 3));
	QDT_MessageLogRenderer("=> %f %f %f %f", m(3, 0), m(3, 1), m(3, 2), m(3, 3));

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

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetVertexRegisterBlock(nRegister, 4, &array[0]); // tester avec prochain SDK (081 => crash !)
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_PSGL *)_pCurrentVertexShader)->GetPSGLVertexShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetMatrixParameterfr(cgP, &array[0]);	
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetVertexShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetVertexShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetVertexShaderConstant(UInt32 nRegister,
													 const COLOR_FLOAT & Color)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetVertexShaderConstant %d %f %f %f %f", nRegister, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	Float32 fArray[4];
	fArray[0] = Color.GetRed();
	fArray[1] = Color.GetGreen();
	fArray[2] = Color.GetBlue();
	fArray[3] = Color.GetAlpha();

	cgGLSetVertexRegister4fv(nRegister, fArray);
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_PSGL *)_pCurrentVertexShader)->GetPSGLVertexShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetVertexShaderConstant(UInt32 nRegister,
													   Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetVertexShaderConstant %d %f %f %f %f", nRegister, f1, f2, f3, f4);

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	Float32 fArray[4];
	fArray[0] = f1;
	fArray[1] = f2;
	fArray[2] = f3;
	fArray[3] = f4;

	cgGLSetVertexRegister4fv(nRegister, fArray);
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_PSGL *)_pCurrentVertexShader)->GetPSGLVertexShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, f1, f2, f3, f4);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetVertexShaderArrayConstant(UInt32		nRegister,
														  UInt32		uiNbrValues,
														  const float *	pValues)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetVertexShaderConstant TODO");

	QDT_ASSERT((uiNbrValues % 4) == 0);

	// A TESTER !! (skinning)
#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	UInt32 nNbRegistersToUpdate = (uiNbrValues / 4);
	for (UInt32 i=0; i < nNbRegistersToUpdate; ++i)
	{
		cgGLSetVertexRegister4fv(nRegister + i, &pValues[i*4]);
	}
#else
	CGparameter cgP = cgGetNamedParameter(((VERTEX_SHADER_PSGL *)_pCurrentVertexShader)->GetPSGLVertexShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameterArray4f(cgP, 0, uiNbrValues / 4, pValues);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetPixelShader(I_PIXEL_SHADER *	pPixelShader)
{
	QDT_MessageLogRenderer("SetPixelShader %x", pPixelShader);

	if (pPixelShader != _pCurrentPixelShader)
	{
		_pCurrentPixelShader = pPixelShader;

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif
		
		if (pPixelShader != NULL)
		{
			cgGLBindProgram(((PIXEL_SHADER_PSGL *)pPixelShader)->GetPSGLPixelShader());
			QDT_TEST_CG_ERROR();
			cgGLEnableProfile(PLATFORM_PS3_GPU_FP_PROFILE);
			QDT_TEST_CG_ERROR();
		}
		else
		{
			cgGLDisableProfile(PLATFORM_PS3_GPU_FP_PROFILE);
			QDT_TEST_CG_ERROR();
		}

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime1 = TIME::Instance()->GetSystemTime();
		_RenderStats.AddSetPixelShaderTime(fTime1 - fTime0);
#endif

		_RenderStats.AddSetPixelShader(1);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetPixelShaderConstant(UInt32 nRegister,
													const VECTOR & Vec)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetPixelShader %x %f %f %f %f", nRegister, Vec.GetX(), Vec.GetY(), Vec.GetZ(), Vec.GetPad());

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetFragmentRegister4fv(nRegister, Vec.ToPointer());
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter3f(cgP, Vec.GetX(), Vec.GetY(), Vec.GetZ());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetPixelShaderConstant(UInt32 nRegister,
													Float32 r)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetPixelShaderConstant %d %f", nRegister, r);

	// ATTENTION ! 1 seul float utilisé sur 4 !!!
#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetFragmentRegister4fv(nRegister, &r);
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter1f(cgP, r);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetPixelShaderConstant(UInt32 nRegister,
													const MATRIX & m)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetPixelShaderConstant %d %f %f %f %f", nRegister, m(0, 0), m(0, 1), m(0, 2), m(0, 3));
	QDT_MessageLogRenderer("=> %f %f %f %f", m(1, 0), m(1, 1), m(1, 2), m(1, 3));
	QDT_MessageLogRenderer("=> %f %f %f %f", m(2, 0), m(2, 1), m(2, 2), m(2, 3));
	QDT_MessageLogRenderer("=> %f %f %f %f", m(3, 0), m(3, 1), m(3, 2), m(3, 3));

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

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetFragmentRegisterBlock(nRegister, 4, &array[0]); // tester avec prochain SDK (081 => crash !)
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetMatrixParameterfr(cgP, &array[0]);														
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetPixelShaderConstant(UInt32 nRegister,
													const COLOR_FLOAT & Color)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetPixelShaderConstant %d %f %f %f %f", nRegister, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());

	Float32 fArray[4];
	fArray[0] = Color.GetRed();
	fArray[1] = Color.GetGreen();
	fArray[2] = Color.GetBlue();
	fArray[3] = Color.GetAlpha();

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetFragmentRegister4fv(nRegister, fArray);
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, Color.GetRed(), Color.GetGreen(), Color.GetBlue(), Color.GetAlpha());
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetPixelShaderTexture(UInt32			nTexture,
												   I_TEXTURE *		pTexture)
{
	QDT_MessageLogRenderer("SetPixelShaderTexture %d %x", nTexture, pTexture);

	//SetTexture(nTexture, pTexture);

	// Test de debug pour les larmes
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLTextureNames[nTexture]);	
	QDT_TEST_CG_ERROR();

	if(cgP != 0)
	{
		if (pTexture != NULL)
		{
			cgGLSetTextureParameter(cgP, *(GLuint*)pTexture->GetHandle());							
			QDT_TEST_CG_ERROR();

			cgGLEnableTextureParameter(cgP);										
			QDT_TEST_CG_ERROR();
		}
		else
		{
			cgGLDisableTextureParameter(cgP);										
			QDT_TEST_CG_ERROR();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	26/01/06:	PDE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetPixelShaderConstant(UInt32 nRegister, 
													 Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetPixelShaderConstant %d %f %f %f %f", nRegister, f1, f2, f3, f4);
	
#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	Float32 fArray[4];
	fArray[0] = f1;
	fArray[1] = f2;
	fArray[2] = f3;
	fArray[3] = f4;

	cgGLSetFragmentRegister4fv(nRegister, fArray);
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameter4f(cgP, f1, f2, f3, f4);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderArrayConstant
//	Object:		
//	06-01-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetPixelShaderArrayConstant(UInt32			nRegister,
														 UInt32			uiNbrValues,
														 const float *	pValues)
{
#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	QDT_MessageLogRenderer("SetPixelShaderConstant TODO");

	QDT_ASSERT((uiNbrValues % 4) == 0);

#ifdef ENABLE_FAST_SET_SHADER_CONSTANTS
	cgGLSetFragmentRegisterBlock(nRegister, uiNbrValues / 4, pValues); // tester avec prochain SDK (081 => crash !)
	QDT_TEST_CG_ERROR();
#else
	CGparameter cgP = cgGetNamedParameter(((PIXEL_SHADER_PSGL *)_pCurrentPixelShader)->GetPSGLPixelShader(), PSGLRegisterNames[nRegister]); 
	QDT_TEST_CG_ERROR();

	cgGLSetParameterArray4f(cgP, 0, uiNbrValues / 4, pValues);
	QDT_TEST_CG_ERROR();
#endif

#ifdef QDT_PROFILE_API_PSGL
	Float64 fTime1 = TIME::Instance()->GetSystemTime();
	_RenderStats.AddSetPixelShaderConstantTime(fTime1 - fTime0);
#endif

	_RenderStats.AddSetPixelShaderConstant(1);
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetBlendMode(BLEND_MODE src, 
									   BLEND_OP op,
									   BLEND_MODE dest)
{
	QDT_MessageLogRenderer("SetBlendMode %d %d %d", src, op, dest);

	if (_eSrcBlendMode != src || _eDstBlendMode != dest)
	{
		_eSrcBlendMode = src;
		_eDstBlendMode = dest;
		glBlendFunc(PSGLBlendMode[src], PSGLBlendMode[dest]);
		QDT_TEST_GL_ERROR();
	}

	if (_eBlendOp != op)
	{
		_eBlendOp = op;
		glBlendEquation(PSGLBlendOp[op]);
	}

	if (_eSrcBlendMode == BM_ONE && _eDstBlendMode == BM_ZERO && _eBlendOp == BOP_ADD)
	{
		if (_bEnableAlphaBlend == true)
		{
			glDisable(GL_BLEND);
			QDT_TEST_GL_ERROR();
			_bEnableAlphaBlend = false;
		}
	}
	else if (_bEnableAlphaBlend == false)
	{
		glEnable(GL_BLEND);
		QDT_TEST_GL_ERROR();
		_bEnableAlphaBlend = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::GetBlendMode(BLEND_MODE & src,
									   BLEND_OP	  & op, 
									   BLEND_MODE & dest)
{
	src = _eSrcBlendMode;
	dest = _eDstBlendMode;
	op = _eBlendOp;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::Clear(UInt32 flags, 
								UInt8 r, 
								UInt8 g, 
								UInt8 b,
								UInt8 a,
								float z)
{
	QDT_MessageLogRenderer("Clear %d %d %d %d %d %f", flags, r, g, b, a, z);

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
	glClearDepthf(z);

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
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetViewport(Int32 x, 
									  Int32 y, 
									  Int32 width, 
									  Int32 height)
{
	QDT_MessageLogRenderer("SetViewport %d %d %d %d", x, y, width, height);

	_iViewportX = x;
	_iViewportY = y;
	_iViewportWidth = width;
	_iViewportHeight = height;

	glViewport(x, y, width, height);
	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	06-03-29:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::GetViewport(Int32 &	x,
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
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetTexture(UInt32			i, 
									 I_TEXTURE *	pTexture)
{
	QDT_MessageLogRenderer("SetTexture %d %x", i, pTexture);

	if (_apCurrentTextures[i] != pTexture)
	{
		_apCurrentTextures[i] = pTexture;

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

		if (pTexture)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			QDT_TEST_GL_ERROR();

			if (_aCurrentTexturesTarget[i] != 0)
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
			glActiveTexture(GL_TEXTURE0 + i);
			QDT_TEST_GL_ERROR();

			if (_aCurrentTexturesTarget[i] != 0)
			{
				glDisable(_aCurrentTexturesTarget[i]);
				QDT_TEST_GL_ERROR();
			} 

			//glBindTexture(_aCurrentTexturesTarget[i], 0);
			//QDT_TEST_GL_ERROR();

			_aCurrentTexturesTarget[i] = 0;

			_RenderStats.AddSetTexture(1);
		}

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime1 = TIME::Instance()->GetSystemTime();
		_RenderStats.AddSetTextureTime(fTime1 - fTime0);
#endif

		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexDeclaration
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration)
{
	QDT_MessageLogRenderer("SetVertexDeclaration %x", pVertexDeclaration);

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
void RENDER_CONTEXT_PSGL::SetCullMode(CULL_MODE eCullMode)
{
	QDT_MessageLogRenderer("SetCullMode %d", eCullMode);

	if (_eCullMode != eCullMode)
	{
		_eCullMode = eCullMode;

		if (_eCullMode == CULL_NONE)
		{
			glDisable(GL_CULL_FACE);
			QDT_TEST_GL_ERROR();
		}
		else
		{
			glEnable(GL_CULL_FACE);
			QDT_TEST_GL_ERROR();
			glFrontFace(PSGLCullModes[eCullMode-1]);
			QDT_TEST_GL_ERROR();
		}		
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCullMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------	
CULL_MODE RENDER_CONTEXT_PSGL::GetCullMode() const
{
	return _eCullMode;
}

//-----------------------------------------------------------------------------
//	Name:		SetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetFillMode(FILL_MODE	eFillMode)
{
	QDT_MessageLogRenderer("SetFillMode %d", eFillMode);

	if (_eFillMode != eFillMode)
	{
		_eFillMode = eFillMode;
		glPolygonMode(GL_FRONT_AND_BACK, PSGLFillModes[eFillMode]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
FILL_MODE	RENDER_CONTEXT_PSGL::GetFillMode() const
{
	return _eFillMode;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_PSGL::GetCurrentRenderTarget(UInt32 uiRenderTargetIndex) const
{
	QDT_ASSERT(uiRenderTargetIndex == 0);
	return _pCurrentRenderTarget;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	06-03-17:	RMA - Created
//-----------------------------------------------------------------------------
CUBEMAP_FACE RENDER_CONTEXT_PSGL::GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex) const
{
	return (_nCurrentRenderTargetCubeFace);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentDepthBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_PSGL::GetCurrentDepthBuffer() const
{
	return _pCurrentDepthBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		GetFrontBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_PSGL::GetFrontBuffer() const
{
	return _pFrontBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		GetDepthBuffer
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_PSGL::GetDepthBuffer() const
{
	return _pDepthBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentRenderTarget
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetCurrentRenderTarget(I_RENDER_TARGET *	pRenderTarget, 
													UInt32 uiRenderTargetIndex,
													CUBEMAP_FACE nCubeMapFace)
{
	QDT_MessageLogRenderer("SetCurrentRenderTarget %x %d %d", pRenderTarget, uiRenderTargetIndex, nCubeMapFace);

	QDT_ASSERT(pRenderTarget != NULL);
	QDT_ASSERT(uiRenderTargetIndex == 0);

	if ( (_pCurrentRenderTarget != pRenderTarget) || (_nCurrentRenderTargetCubeFace != nCubeMapFace) )
	{
#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

		GLuint gluiSurface = *(GLuint*) pRenderTarget->GetSurface(nCubeMapFace);

		glBindFramebufferOES(GL_FRAMEBUFFER_OES, gluiSurface);
		QDT_TEST_GL_ERROR();

		_pCurrentRenderTarget = pRenderTarget;
		_nCurrentRenderTargetCubeFace = nCubeMapFace;

		_pCurrentDepthBuffer = NULL; // SetCurrentDepthBuffer need to be called after SetCurrentRenderTarget in order to attach a renderbuffer to a framebuffer

		_RenderStats.AddSetRenderTarget(1);

#ifdef QDT_PROFILE_API_PSGL
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
void	RENDER_CONTEXT_PSGL::SetCurrentDepthBuffer(I_RENDER_TARGET *	pRT)
{
	QDT_MessageLogRenderer("SetCurrentDepthBuffer %x", pRT);

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

	if (pRT)
	{
		RENDER_TARGET_PSGL * pRenderTarget = (RENDER_TARGET_PSGL *) pRT;

		if (_pCurrentDepthBuffer != pRenderTarget)
		{
			GLuint gluiSurface = *(GLuint*) pRenderTarget->GetSurface();

			if (pRenderTarget->GetPixelFormat() == PF_D24S8_DEPTHSTENCIL)
			{
				glBindRenderbufferOES(GL_RENDERBUFFER_OES, gluiSurface);
				QDT_TEST_GL_ERROR();

				// attach a renderbuffer to a framebuffer => SetCurrentRenderTarget must be called before SetCurrentDepthBuffer !!!
				if (gluiSurface)
				{
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, gluiSurface);
					QDT_TEST_GL_ERROR();
				}
			}
			else
			{
				QDT_ASSERT(pRenderTarget->GetPixelFormat() == PF_D24S8)
				//QDT_ASSERT(_pCurrentRenderTarget == _pFrontBuffer);

				if (_pCurrentRenderTarget != _pFrontBuffer)
				{
					SetCurrentRenderTarget(GetFrontBuffer());
				}

				glBindFramebufferOES(GL_FRAMEBUFFER_OES, gluiSurface);
				QDT_TEST_GL_ERROR();
			}

			_pCurrentDepthBuffer = pRenderTarget;
			_RenderStats.AddSetDepthBuffer(1);
		}	
	}
	else
	{
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
		QDT_TEST_GL_ERROR();
	}	

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime1 = TIME::Instance()->GetSystemTime();
		_RenderStats.AddSetDepthBufferTime(fTime1 - fTime0);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsFullScreen
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
Bool   RENDER_CONTEXT_PSGL::IsFullScreen() const
{
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenWidth
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_PSGL::GetScreenWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenHeight
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_PSGL::GetScreenHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxScreenWidth
//	Object:		
//	06-05-22:	RMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_PSGL::GetMaxScreenWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxScreenHeight
//	Object:		
//	06-05-22:	RMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_PSGL::GetMaxScreenHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthTest
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::EnableDepthTest(bool	bState)
{
	QDT_MessageLogRenderer("EnableDepthTest %d", bState);

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
void	RENDER_CONTEXT_PSGL::SetDepthTestFunc(COMP_FUNC	CF)
{
	QDT_MessageLogRenderer("SetDepthTestFunc %d", CF);

	if (CF != _DepthTestFunc)
	{
		_DepthTestFunc = CF;
		glDepthFunc(PSGLCmpFonc[CF]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthWrite
//	Object:		
//	05-11-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::EnableDepthWrite(bool	bState)
{
	if (_bEnableDepthWrite != bState)
	{
		_bEnableDepthWrite = bState;
		glDepthMask(bState);	
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableScissorTest
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::EnableScissorTest(Bool bState)
{
	QDT_MessageLogRenderer("EnableScissorTest %d", bState);

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
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetScissorRect(UInt32 x, 
										 UInt32 y, 
										 UInt32 width, 
										 UInt32 height)
{
	QDT_MessageLogRenderer("SetScissorRect %d %d %d %d", x, y, width, height);

	glScissor(x, y, width, height);
}

//-----------------------------------------------------------------------------
//	Name:		CopySurface
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::CopySurface(I_RENDER_TARGET * pSrc, 
									  I_RENDER_TARGET * pDst) 
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::SetVertexBuffer(UInt32 stream, 
										  I_VERTEX_BUFFER * pVertexBuffer)
{
	QDT_MessageLogRenderer("SetVertexBuffer %d %x", stream, pVertexBuffer);

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
void RENDER_CONTEXT_PSGL::SetIndexBuffer(I_INDEX_BUFFER * pIndexBuffer)
{
	QDT_MessageLogRenderer("SetIndexBuffer %x", pIndexBuffer);

	if (_pCurrentIndexBuffer != pIndexBuffer)
	{
		_pCurrentIndexBuffer = pIndexBuffer;
		_bNeedUpdateIndexContext = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLPrimitiveType
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_PSGL::GetPSGLPrimitiveType(PRIMITIVE_TYPE PrimitiveType) const
{
	return (PSGLPrimitiveTypeArray[PrimitiveType]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNumIndex
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 RENDER_CONTEXT_PSGL::GetNumIndices(PRIMITIVE_TYPE eType, 
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

/*
POSITION ATTR0
BLENDWEIGHT ATTR1
NORMAL ATTR2
COLOR0 ATTR3
COLOR1 ATTR4
FOGCOORD ATTR5
PSIZE ATTR6
BLENDINDICES ATTR7
TEX0-7 ATTR8-15
TANGENT  ATTR14
BINORMAL ATTR15
*/
//-----------------------------------------------------------------------------
//	Name:		GetPSGLAttributeIndex
//	Object:		
//	06-03-06:	SBE - Created
//-----------------------------------------------------------------------------	
GLuint RENDER_CONTEXT_PSGL::GetPSGLAttributeIndex(Int8 nUsage)
{
	switch (nUsage)
	{
		default:
		QDT_FAIL(); // shouldn't happen...
		break;

		case DU_POSITION:
			return 0;//CG_ATTR0;
		break;

		case DU_NORMAL:
			return 2;//CG_ATTR2;
		break;

		case DU_BLENDWEIGHT:
			return 1;//CG_ATTR1;
		break;

		case DU_BLENDINDICES:
			return 7;//CG_ATTR7;
		break;

		case DU_COLOR:
			return 3;//CG_ATTR3;
		break;

		case DU_TEXCOORD0:
		case DU_TEXCOORD1:
		case DU_TEXCOORD2:
		case DU_TEXCOORD3:
		case DU_TEXCOORD4:
		case DU_TEXCOORD5:
		case DU_TEXCOORD6:
		case DU_TEXCOORD7:
			return  8 + (nUsage - DU_TEXCOORD0); //CG_ATTR8 + (nUsage - DU_TEXCOORD0);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateVertexContext
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_PSGL::UpdateVertexContext()
{
	//if (_bNeedUpdateVertexContext == true)
	{
		_bNeedUpdateVertexContext = false;

		VERTEX_DECLARATION_PSGL * pVertDecl = static_cast<VERTEX_DECLARATION_PSGL *>(_pCurrentVertexDeclaration);
		
		S_VERTEX_ELEMENT * pVertElt;
		UInt32 nNumElts;

		pVertDecl->GetDeclaration(pVertElt, nNumElts);

		for (UInt32 i = 0 ; i < nNumElts ; ++i)
		{
			UInt8 nStream = pVertElt[i]._nStream;

			VERTEX_BUFFER_PSGL * pVertBuffer = static_cast<VERTEX_BUFFER_PSGL *>(_apCurrentVertexBuffer[nStream]);

			QDT_ASSERT(pVertBuffer && "Renderer : You can draw anything if you have not set a stream defined in the vertex declaration!"); 
		
#ifdef QDT_PROFILE_API_PSGL
			Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

			glBindBuffer(GL_ARRAY_BUFFER, pVertBuffer->GetPSGLIndex());

#ifdef QDT_PROFILE_API_PSGL
			Float64 fTime1 = TIME::Instance()->GetSystemTime();
			_RenderStats.AddSetVertexBufferTime(fTime1 - fTime0);
#endif

			_RenderStats.AddSetVertexBuffer(1);

			GLubyte * pOffset = (GLubyte *) NULL + (pVertElt[i]._nOffset);

			// A OPTIMISER possibilite d'eviter le switch..
			// stocker ds VERTEX_ELEMENT :  index, fsize(nb coord per vertex), type (GL_FLOAT, GL_HALF...)
			// => directement faire
//			cgGLEnableAttrib(pVertElt[i]._nPSGLIndex);
//			cgGLAttribPointer(pVertElt[i]._nPSGLIndex, pVertElt[i]._nPSGLFSize, pVertElt[i]._nPSGLType, false, pVertBuffer->GetVertexStride(), pOffset);

			GLuint index = GetPSGLAttributeIndex(pVertElt[i]._nUsage);
			cgGLEnableAttrib(index);

			switch (pVertElt[i]._nUsage)
			{
			default:
				QDT_FAIL();

			case DU_POSITION:
				{
//					glEnableClientState(GL_VERTEX_ARRAY);

					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT(0 && "Unsupported type for DU_POSITION! Please use DT_FLOAT3, DT_FLOAT4 or DT_FLOAT16_4!");
						break;
					
					case DT_FLOAT3:	
//						glVertexPointer(3, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 3, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT4:
//						glVertexPointer(4, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT16_4:
//						glVertexPointer(4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_HALF_FLOAT_ARB, false, pVertBuffer->GetVertexStride(), pOffset);
						break;
					}

					QDT_TEST_CG_ERROR();
				}
				break;
			
			case DU_NORMAL:
				{
//					glEnableClientState(GL_NORMAL_ARRAY);

					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT(0 && "Unsupported type for DU_NORMAL! Please use DT_FLOAT3, DT_FIXED_11_11_10_PS3 or DT_FLOAT16_4!");
						break;

					case DT_FIXED_11_11_10_PS3:
//						glNormalPointer(GL_FIXED_11_11_10_SCE, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 3, GL_FIXED_11_11_10_SCE, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT3:
//						glNormalPointer(GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 3, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT16_4:
//						glNormalPointer(GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_HALF_FLOAT_ARB, false, pVertBuffer->GetVertexStride(), pOffset);
						break;
					}

					QDT_TEST_CG_ERROR();
				}
				break;

			case DU_COLOR:
				{
//					glEnableClientState(GL_COLOR_ARRAY);
					
					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT(0 && "Unsupported type for DU_COLOR! Please use DT_COLOR, DT_FLOAT4 or DT_FLOAT16_4!");
						break;

					case DT_COLOR:
//						glColorPointer(4, GL_UNSIGNED_BYTE, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_UNSIGNED_BYTE, true, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT4:
//						glColorPointer(4, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT16_4:
//						glColorPointer(4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_HALF_FLOAT_ARB, false, pVertBuffer->GetVertexStride(), pOffset);
						break;
					}

					QDT_TEST_CG_ERROR();
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
					glClientActiveTexture(GL_TEXTURE0 + nTextureIndex);
					QDT_TEST_GL_ERROR();

//					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					QDT_TEST_GL_ERROR();

					switch (pVertElt[i]._nType)
					{
					default:
						QDT_ASSERT(0 && "Unsupported type for DU_TEXCOORDn! Please use everything except DT_FLOAT1!");
						break;

					case DT_FLOAT2:
//						glTexCoordPointer(2, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 2, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT3:
//						glTexCoordPointer(3, GL_FLOAT, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 3, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FIXED_11_11_10_PS3:
//						glNormalPointer(GL_FIXED_11_11_10_SCE, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 3, GL_FIXED_11_11_10_SCE, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT4:
//						glTexCoordPointer(4, GL_FLOAT, pVertBuffer->GetVertexStride(),pOffset);
						cgGLAttribPointer(index, 4, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT16_2:
//						glTexCoordPointer(2, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 2, GL_HALF_FLOAT_ARB, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_FLOAT16_4:
//						glTexCoordPointer(4, GL_HALF_FLOAT_ARB, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;

					case DT_COLOR:
//						glTexCoordPointer(4, GL_BYTE, pVertBuffer->GetVertexStride(), pOffset);
						cgGLAttribPointer(index, 4, GL_FLOAT, false, pVertBuffer->GetVertexStride(), pOffset);
						break;
					}

					QDT_TEST_CG_ERROR();
				}
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateIndexContext
//	Object:		
//	05-09-16:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::UpdateIndexContext()
{
	if (_bNeedUpdateIndexContext == true)
	{
		_bNeedUpdateIndexContext = false;

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime0 = TIME::Instance()->GetSystemTime();
#endif

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((INDEX_BUFFER_PSGL *)_pCurrentIndexBuffer)->GetPSGLIndexBuffer());

#ifdef QDT_PROFILE_API_PSGL
		Float64 fTime1 = TIME::Instance()->GetSystemTime();
		_RenderStats.AddSetIndexBufferTime(fTime1 - fTime0);
#endif

		_RenderStats.AddSetIndexBuffer(1);

		QDT_TEST_GL_ERROR();
	}	
}

//-----------------------------------------------------------------------------
//	Name:		GetCgContext
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
CGcontext	RENDER_CONTEXT_PSGL::GetCgContext() 
{ 
	return _cgContext; 
}

//-----------------------------------------------------------------------------
//	Name:		EnableAlphaTest
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::EnableAlphaTest(Bool	bState)
{
	QDT_MessageLogRenderer("EnableAlphaTest %d", bState);

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
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestFunc
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetAlphaTestFunc(COMP_FUNC	CF)
{
	QDT_MessageLogRenderer("SetAlphaTestFunc %d", CF);

	if (CF != _AlphaTestFunc)
	{
		_AlphaTestFunc = CF;
		glAlphaFunc(PSGLCmpFonc[_AlphaTestFunc], ((Float32)_nAlphaTestRef) / 255.f);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestRef
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::SetAlphaTestRef(UInt8	Ref)
{
	QDT_MessageLogRenderer("SetAlphaTestRef %d", Ref);

	if (Ref != _nAlphaTestRef)
	{
		_nAlphaTestRef = Ref;
		glAlphaFunc(PSGLCmpFonc[_AlphaTestFunc], ((Float32)_nAlphaTestRef) / 255.f);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsDisplayWindowActive
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_PSGL::IsDisplayWindowActive() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		HasDisplayInputFocus
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_PSGL::HasDisplayInputFocus() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		PrintFPS
//	Object:		
//	05-12-08:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::PrintFPS()
{
	_fTimeEndSwap = TIME::Instance()->GetSystemTime();

	if ( _fTimeEndSwap - _fTimeLastMessage >= 500.f )
	{
		_fFPS = (1000.f / (_fTimeEndSwap - _fTimeStartFrame));
		_fFrameDuration = _fTimeEndFrame - _fTimeStartFrame;

		_fTimeLastMessage = TIME::Instance()->GetSystemTime();
	}

#ifdef _DEBUG
	PrintDebugMessage(0.09f, 0.03f, "DBG FPS: %3.1f Frame: %4.1fms NbVertices: %d", _fFPS, _fFrameDuration, _nNbVertices);
#else
	PrintDebugMessage(0.09f, 0.93f, "FPS: %3.1f Frame: %4.1fms NbVertices: %d", _fFPS, _fFrameDuration, _nNbVertices);
#endif

	_nNbVertices = 0;
	_fTimeStartFrame = TIME::Instance()->GetSystemTime();
}

//-----------------------------------------------------------------------------
//	Name:		SupportShaderModel
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_PSGL::SupportShaderModel(SHADER_MODEL	sm)
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		SupportFloatingPointBlending
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_PSGL::SupportFloatingPointBlending()
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		SupportMultipleRenderTargets
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_PSGL::SupportMultipleRenderTargets()
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		EnableColorWrite
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::EnableColorWrite(Bool		bR,
											  Bool		bG,
											  Bool		bB,
											  Bool		bA,
											  UInt32	uiRenderTargetIndex)
{
	QDT_MessageLogRenderer("EnableColorWrite %d %d %d %d", bR, bG, bB, bA);

	QDT_ASSERT(uiRenderTargetIndex == 0);

	if (_bColorWriteR != bR || 
		_bColorWriteG != bG || 
		_bColorWriteB != bB || 
		_bColorWriteA != bA)
	{
		glColorMask(bR, bG, bB, bA);
		_bColorWriteR = bR;
		_bColorWriteG = bG;
		_bColorWriteB = bB; 
		_bColorWriteA = bA;
	}
}

//-----------------------------------------------------------------------------
//	Name:		TakeScreenShot
//	Object:		
//	06-02-09:	VMA - Created
//  06-04-13:	FBO - Modified to implements from IRenderContextImplementor
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::TakeScreenShot(const QDT_STRING& sFilePath) const
{
	QDT_STRING	sShotFilePath = sFilePath;
	UInt32		nBufferSize;
	UInt8*		pScreenShotBuffer;

	nBufferSize = _uiWidth*_uiHeight*4;

	pScreenShotBuffer = new UInt8[nBufferSize];

	glReadPixels( 0, 0, _uiWidth, _uiHeight, GL_ARGB_SCE, GL_UNSIGNED_INT_8_8_8_8_REV, pScreenShotBuffer);

	QDT_TEST_GL_ERROR();


	struct BMP_HEADER
	{	Int8	Pad[2];
		Int8	Signature[2];
		UInt32	FileSize;
		UInt32	Reserved;
		UInt32	DataOffset;
		
		UInt32	Size;
		UInt32	Width;
		UInt32	Height;
		UInt16	Planes;
		UInt16	BitCount;
		UInt32	Compression;
		UInt32	ImageSize;
		UInt32	XpixelsPerM;
		UInt32	YpixelsPerM;
		UInt32	ColorsUsed;
		UInt32	ColorsImportant;
	} Header;

	ENDIAN::Instance()->EnableSwap(true);

	Header.Signature[0] = 'B';
	Header.Signature[1] = 'M';
	Header.FileSize = ENDIAN::Instance()->Convert((UInt32)(nBufferSize + sizeof(BMP_HEADER) - 2));
	Header.Reserved = ENDIAN::Instance()->Convert((UInt32)(0));
	Header.DataOffset = ENDIAN::Instance()->Convert((UInt32)(sizeof(BMP_HEADER) - 2));

	Header.Size = ENDIAN::Instance()->Convert((UInt32)(40));
	Header.Width = ENDIAN::Instance()->Convert((UInt32)(_uiWidth));
	Header.Height = ENDIAN::Instance()->Convert((UInt32)(_uiHeight));
	Header.Planes = ENDIAN::Instance()->Convert((UInt16)(1));
	Header.BitCount = ENDIAN::Instance()->Convert((UInt16)(32));
	Header.Compression = ENDIAN::Instance()->Convert((UInt32)(0));
	Header.ImageSize = ENDIAN::Instance()->Convert((UInt32)(0));
	Header.XpixelsPerM = ENDIAN::Instance()->Convert((UInt32)(2834));
	Header.YpixelsPerM = ENDIAN::Instance()->Convert((UInt32)(2834));
	Header.ColorsUsed = ENDIAN::Instance()->Convert((UInt32)(0));
	Header.ColorsImportant = ENDIAN::Instance()->Convert((UInt32)(0));

	ENDIAN::Instance()->EnableSwap(false);

	if(sShotFilePath == "")
	{
		UInt32	nShotId = 0;
		Char	FileName[32];

		while(1)
		{	
			sprintf(FileName, "[PS3] %s - %03d.bmp", __DATE__, nShotId);

			if ( FILES_MANAGER_ABC::Instance()->IsFileExist(FileName) == false )
			{
				sShotFilePath = FileName;
				break;
			}

			nShotId++;
		}
	}

	FILE_PS3_HOST File;
	File.Open(sShotFilePath, FILE_ABC::FM_WRITE, false);
	File.Write( &Header.Signature, 0, sizeof(Header) - 2, FILE_ABC::FS_WAIT);
	File.Write( pScreenShotBuffer, sizeof(Header) - 2, nBufferSize, FILE_ABC::FS_WAIT);
	File.Close();

	delete[] pScreenShotBuffer; 
}

//-----------------------------------------------------------------------------
//	Name:		GetFrameBuffer
//	Object:		
//	06-04-13:	FBO - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::GetRGBFrameBuffer(void** pRGBBuffer, UInt32& iWidth, UInt32& iHeight) const
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		ClearRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::ClearRenderStats()
{
	_RenderStats.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
RENDER_STATS &	RENDER_CONTEXT_PSGL::GetRenderStats()
{
	return (_RenderStats);
}

//-----------------------------------------------------------------------------
//	Name:		GetPlatformIntColor
//	Object:		
//	06-03-23:	VMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_PSGL::GetPlatformIntColor(const COLOR &	Color)
{
	return (Color.GetRGBAValue());
}


//-----------------------------------------------------------------------------
//	Name:		Malloc
//	Object:		
//	06-06-21:	VHA - Created
//-----------------------------------------------------------------------------
void *	RENDER_CONTEXT_PSGL::Malloc(size_t size)
{
	return MEM::GetInstance()->Allocate(size);
}

//-----------------------------------------------------------------------------
//	Name:		MemAlign
//	Object:		
//	06-06-21:	VHA - Created
//-----------------------------------------------------------------------------
void *	RENDER_CONTEXT_PSGL::MemAlign(size_t boundary,
									  size_t size)
{
	return MEM::GetInstance()->AllocateAlign(boundary, size);
}

//-----------------------------------------------------------------------------
//	Name:		Realloc
//	Object:		
//	06-06-21:	VHA - Created
//-----------------------------------------------------------------------------
void *	RENDER_CONTEXT_PSGL::Realloc(void	* oldPtr,
									 size_t	size)
{
	return MEM::GetInstance()->Realloc(oldPtr, size);
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	06-06-21:	VHA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_PSGL::Free(void	* pPtr)
{
	MEM::GetInstance()->Free(pPtr);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
