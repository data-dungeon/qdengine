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
//	CLASS:	2DFILTER
//
//	06-01-12:	PDE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "Filter2D.h"
#include INCL_KRENDERER(Renderer\IPixelShader)
#include INCL_KRENDERER(Renderer\IVertexShader)
#include INCL_KRENDERER(Renderer\IVertexBuffer)
#include INCL_KRENDERER(Renderer\IVertexDeclaration)
#include INCL_KRENDERER(Renderer\ITexture)
#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(Renderer\SVertexElement)
#include INCL_KRENDERER(Renderer\RenderContext)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#include "Shaders/Filter2D_vs.h"
#include "Shaders/IdentityFilter_ps.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

I_VERTEX_BUFFER*		FILTER_2D::g_VBuffer = NULL;
I_VERTEX_BUFFER*		FILTER_2D::g_VBufferQuad = NULL;
I_VERTEX_SHADER*		FILTER_2D::g_VShader = NULL;
I_PIXEL_SHADER*			FILTER_2D::g_Default_PShader = NULL;
I_PIXEL_SHADER*			FILTER_2D::g_Default_PShader_square = NULL;
I_VERTEX_DECLARATION*	FILTER_2D::g_VDecl = NULL;
Int32					FILTER_2D::g_refCount = 0;

//-----------------------------------------------------------------------------
//	Name:		FILTER_2D constructor
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
void FILTER_2D::InitStaticStuff()
{

		static S_VERTEX_ELEMENT g_decl[2] = {{0, 0,	 DT_FLOAT4,	DU_POSITION},
										     {0, 16, DT_FLOAT2,	DU_TEXCOORD0}};

		// Normalized clip plane quad defined with a triangle fan
		static S_FILTER_VERTEX geom[3] = {{-1.0f,  -1.0f, 1.0f, 1.0f , 0.0f, 1.0f},
										  { 3.0f,  -1.0f, 1.0f, 1.0f , 2.0f, 1.0f},
										  {-1.0f,   3.0f, 1.0f, 1.0f , 0.0f, -1.0f}};

		static S_FILTER_VERTEX geomOGL[3] = {{-1.0f,   3.0f, 1.0f, 1.0f , 0.0f, 2.0f},
											 { 3.0f,  -1.0f, 1.0f, 1.0f , 2.0f, 0.0f},
											 {-1.0f,  -1.0f, 1.0f, 1.0f , 0.0f, 0.0f}};

		// Normalized clip plane quad defined with a triangle fan
		static S_FILTER_VERTEX geomQuad[4] = {{-1.0f,  -1.0f, 1.0f, 1.0f , 0.0f, 1.0f},
									    { 1.0f,  -1.0f, 1.0f, 1.0f , 1.0f, 1.0f},
									    { 1.0f,   1.0f, 1.0f, 1.0f , 1.0f, 0.0f},
									    {-1.0f,   1.0f, 1.0f, 1.0f , 0.0f, 0.0f}};

		static S_FILTER_VERTEX geomOGLQuad[4] = {{-1.0f,   1.0f, 1.0f, 1.0f , 0.0f, 1.0f},
												{ 1.0f,   1.0f, 1.0f, 1.0f , 1.0f, 1.0f},
												{ 1.0f,  -1.0f, 1.0f, 1.0f , 1.0f, 0.0f},
												{-1.0f,  -1.0f, 1.0f, 1.0f , 0.0f, 0.0f}};


		g_VDecl = RENDER_CONTEXT::CreateVertexDeclaration(g_decl, 2);

		g_VBuffer     = RENDER_CONTEXT::CreateVertexBuffer(3, sizeof(S_FILTER_VERTEX));
		g_VBufferQuad = RENDER_CONTEXT::CreateVertexBuffer(4, sizeof(S_FILTER_VERTEX));
		
		void* vMem = g_VBuffer->Lock();
		if (RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
			QDT_MEMCOPY(vMem, geom, 3 * sizeof(S_FILTER_VERTEX));
		else
			QDT_MEMCOPY(vMem, geomOGL, 3 * sizeof(S_FILTER_VERTEX));
		g_VBuffer->Unlock();

		vMem = g_VBufferQuad->Lock();
		if (RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
			QDT_MEMCOPY(vMem, geomQuad, 4 * sizeof(S_FILTER_VERTEX));
		else
			QDT_MEMCOPY(vMem, geomOGLQuad, 4 * sizeof(S_FILTER_VERTEX));
		g_VBufferQuad->Unlock();

		g_VShader = QDT_CREATE_VERTEXSHADER(Filter2D_vs);

		g_Default_PShader = QDT_CREATE_PIXELSHADER(IdentityFilter_ps);

#ifdef PLATFORM_PC
		if (RENDER_CONTEXT::GetRenderContextType() != RENDER_CONTEXT::RCT_DIRECT3D9)
		{
			g_Default_PShader_square = QDT_CREATE_PIXELSHADER(IdentityFilter_ps_NORECT);
		}
		else
#endif
			g_Default_PShader_square = g_Default_PShader;
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
FILTER_2D::FILTER_2D()
{
	if (!g_refCount)
	{ // needs to create the common data
		InitStaticStuff();
	}

	_bQuadMode = false;
	_bIsFilterIdentity = true;
	_FilterShader = g_Default_PShader;
	_FilterVertexShader = g_VShader;
	g_refCount++;
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
FILTER_2D::FILTER_2D(I_PIXEL_SHADER* filterShader)
{
	if (!g_refCount)
	{ // needs to create the common data
		InitStaticStuff();
	}
	
	if (!filterShader)
	{
		_FilterShader = g_Default_PShader;
		_bIsFilterIdentity = true;
	}
	else
	{
		_FilterShader = filterShader;
		_bIsFilterIdentity = false;
	}

	_bQuadMode = false;
	_FilterVertexShader = g_VShader;
	g_refCount++;
	
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
void FILTER_2D::OverrideVertexShader(I_VERTEX_SHADER* vShader)
{
	if (vShader)
		_FilterVertexShader = vShader;
	else
		_FilterVertexShader = g_VShader;
}

//-----------------------------------------------------------------------------
//	Name:		2DFILTER destructor
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
FILTER_2D::~FILTER_2D()
{
	g_refCount--;
	
	if (g_refCount == 0)
	{
		if (g_Default_PShader != g_Default_PShader_square)
		{
			RENDER_CONTEXT::ReleasePixelShader(g_Default_PShader_square);
		}
		RENDER_CONTEXT::ReleaseVertexBuffer(g_VBuffer);
		RENDER_CONTEXT::ReleaseVertexBuffer(g_VBufferQuad);
		RENDER_CONTEXT::ReleaseVertexShader(g_VShader);
		RENDER_CONTEXT::ReleaseVertexDeclaration(g_VDecl);
		RENDER_CONTEXT::ReleasePixelShader(g_Default_PShader);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
void	FILTER_2D::Init(I_RENDER_TARGET *	InputTarget,
						I_RENDER_TARGET *	OutputTarget)
{
	QDT_ASSERT(InputTarget->GetWidth() == OutputTarget->GetWidth() &&
			   InputTarget->GetHeight() == OutputTarget->GetHeight());

	_InputTarget = InputTarget;

	Init(InputTarget->GetTexture(), OutputTarget);
}

void FILTER_2D::LightInit()
{
	QDT_ASSERT(_FilterVertexShader);
	QDT_ASSERT(_FilterShader);

	RENDER_CONTEXT::SetCullMode(CULL_NONE);
	RENDER_CONTEXT::EnableDepthTest(false);
	RENDER_CONTEXT::EnableDepthWrite(false);
	RENDER_CONTEXT::SetVertexShader(_FilterVertexShader);
	RENDER_CONTEXT::SetVertexDeclaration(g_VDecl);
	RENDER_CONTEXT::SetVertexBuffer(0, g_VBuffer);
	RENDER_CONTEXT::SetPixelShader(_FilterShader);
}

void FILTER_2D::Init(I_TEXTURE* InputImage,
					 I_RENDER_TARGET* OutputTarget,
					 UInt32 SrcX, UInt32 SrcY, UInt32 SrcWidth, UInt32 SrcHeight,
					 UInt32 DstX, UInt32 DstY, UInt32 DstWidth, UInt32 DstHeight)
{
	_InputImage = InputImage;
	_OutputTarget = OutputTarget;

	RENDER_CONTEXT::SetCurrentRenderTarget(OutputTarget);
	RENDER_CONTEXT::SetViewport(DstX, DstY, DstWidth, DstHeight);

	RENDER_CONTEXT::SetCullMode(CULL_NONE);
	RENDER_CONTEXT::EnableDepthTest(false);
	RENDER_CONTEXT::EnableDepthWrite(false);
	RENDER_CONTEXT::SetVertexShader(_FilterVertexShader);
	RENDER_CONTEXT::SetVertexDeclaration(g_VDecl);


	RENDER_CONTEXT::SetVertexBuffer(0, _bQuadMode? g_VBufferQuad : g_VBuffer);


	float inputWidth  = (float)InputImage->GetWidth();
	float inputHeight = (float)InputImage->GetHeight();

	if (_bIsFilterIdentity)
	{
		if (inputWidth == inputHeight)
			RENDER_CONTEXT::SetPixelShader(g_Default_PShader_square);
		else
			RENDER_CONTEXT::SetPixelShader(g_Default_PShader);
	}
	else
		RENDER_CONTEXT::SetPixelShader(_FilterShader);

	RENDER_CONTEXT::SetPixelShaderTexture(0, InputImage);


	float ratioX = (float)SrcWidth /  inputWidth;
	float ratioY = (float)SrcHeight / inputHeight;

	float offset = 0.0f;

	if (RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
	{
		offset = 0.5f;
	}

	if (!InputImage->GetUVNormalized())
	{
		RENDER_CONTEXT::SetVertexShaderConstant(0, InputImage->GetWidth() * ratioX,
												   InputImage->GetHeight() * ratioY,
												   (float)(SrcX + offset),
												   (float)(SrcY + offset));
	}
	else
	{
		RENDER_CONTEXT::SetVertexShaderConstant(0, ratioX,
												   ratioY,
												   (float)(SrcX + offset) / inputWidth,
												   (float)(SrcY + offset) / inputHeight);
	}
}


//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-01-12:	PDE - Created
//-----------------------------------------------------------------------------
void FILTER_2D::Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget)
{
	Init(InputImage, OutputTarget, 0, 0, InputImage->GetWidth(), InputImage->GetHeight(),
								   0, 0, OutputTarget->GetWidth(), OutputTarget->GetHeight());
}


void FILTER_2D::Apply()
{
	if (_bQuadMode)
	{
		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_FAN, 2, 0); 
	}
	else
	{
		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_LIST, 1, 0); 
	}
}

I_RENDER_TARGET* FILTER_2D::PingPong(UInt32 Iterations)
{
	for (UInt32 i = 0; i < Iterations; i++)
	{
		if (i & 0x1)
		{
			RENDER_CONTEXT::SetPixelShaderTexture(0, _OutputTarget->GetTexture());
			RENDER_CONTEXT::SetCurrentRenderTarget(_InputTarget);
		}
		else
		{
			RENDER_CONTEXT::SetPixelShaderTexture(0, _InputTarget->GetTexture());
			RENDER_CONTEXT::SetCurrentRenderTarget(_OutputTarget);
		}

		RENDER_CONTEXT::DrawPrimitive(PT_TRIANGLE_FAN, 2, 0);
	}

	if (Iterations & 0x1)
	{
		return _OutputTarget;
	}
	else
	{
		return _InputTarget;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
