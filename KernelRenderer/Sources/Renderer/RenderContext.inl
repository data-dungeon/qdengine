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
//	CLASS:	RENDER_CONTEXT
//	The RENDER_CONTEXT class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	INCL_KCORE(Time/Time_ABC)

//-----------------------------------------------------------------------------
//	Name:		BeginScene
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::BeginScene()
{
	_pRenderContext->BeginScene();
}

//-----------------------------------------------------------------------------
//	Name:		EndScene
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::EndScene()
{
	_pRenderContext->EndScene();
}

//-----------------------------------------------------------------------------
//	Name:		Present
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::Present()
{
	static Float64 fTimeStartFrame = 0;
	Float64 fTimeEndFrame  = TIME::Instance()->GetSystemTime();

	Float64 fTimeStartSwap = TIME::Instance()->GetSystemTime();

	if (_nRenderContextType != RCT_OPENGL_FOR_MAYA) // Maya make the present itself!
	{
		AUTO_LOCK	AL(_pMutex);
		_pRenderContext->Present();
	}

	Float64 fTimeEndSwap = TIME::Instance()->GetSystemTime();

	_fTimeSwap = fTimeEndSwap - fTimeStartSwap;
	_fTimeFrame = fTimeEndFrame - fTimeStartFrame;
	_fFPS = 1000.f/(fTimeEndSwap - fTimeStartFrame);

	fTimeStartFrame = TIME::Instance()->GetSystemTime();
}

//-----------------------------------------------------------------------------
//	Name:		CreateAdditionalSwapChain
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
I_SWAP_CHAIN * RENDER_CONTEXT::CreateAdditionalSwapChain(HWND hWnd)
{
	return (_pRenderContext->CreateAdditionalSwapChain(hWnd));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetFullScreenSize
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
bool RENDER_CONTEXT::SetFullScreenSize(UInt32 uiWidth, 
										  UInt32 uiHeight)
{
	return (_pRenderContext->SetFullScreenSize(uiWidth, uiHeight));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetFullScreen
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void RENDER_CONTEXT::SetFullScreen(Bool bFullScreen)
{
	_pRenderContext->SetFullScreen(bFullScreen);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetNbrRenderModes
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
UInt32 RENDER_CONTEXT::GetNbrRenderModes()
{
	return (_pRenderContext->GetNbrRenderModes());
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetRenderMode
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
const S_RENDER_MODE & RENDER_CONTEXT::GetRenderMode(UInt32 i)
{
	return (_pRenderContext->GetRenderMode(i));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetRenderWindow
//	Object:		
//	05-11-10:	ELE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
HWND	RENDER_CONTEXT::GetRenderWindow()
{
	return	(_pRenderContext->GetRenderWindow());
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosX
//	Object:		
//	05-12-13:	ELE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
Int32	RENDER_CONTEXT::GetWindowPosX()
{
	return	(_pRenderContext->GetWindowPosX());
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosY
//	Object:		
//	05-12-13:	ELE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
Int32	RENDER_CONTEXT::GetWindowPosY()
{
	return	(_pRenderContext->GetWindowPosY());
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SupportShaderModel
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
Bool RENDER_CONTEXT::SupportShaderModel(SHADER_MODEL sm)
{
	return	(_pRenderContext->SupportShaderModel(sm));
}

//-----------------------------------------------------------------------------
//	Name:		IsInit
//	Object:		
//	05-11-10:	ELE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT::IsInit()
{
	return	(_pRenderContext != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_BUFFER * RENDER_CONTEXT::CreateVertexBuffer(UInt32 vertexCount, 
												     UInt32 vertexStride,
												     Bool bDynamic)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateVertexBuffer(vertexCount, vertexStride, bDynamic));
}

//-----------------------------------------------------------------------------
//	Name:		CreateIndexBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_INDEX_BUFFER * RENDER_CONTEXT::CreateIndexBuffer(UInt32 nIndexCount)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateIndexBuffer(nIndexCount));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexDeclaration
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_DECLARATION * RENDER_CONTEXT::CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements,
															   UInt32 numElements)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateVertexDeclaration(pElements, numElements));
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT::CreateTexture(UInt32 uiWidth,
										  UInt32 uiHeight,
										  PIXEL_FORMAT Format,
										  UInt32 uiMipMapLevelNum)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateTexture(uiWidth, uiHeight, Format, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeTexture
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT::CreateCubeTexture(UInt32 uiEdgeLength,
											  PIXEL_FORMAT Format,
											  UInt32 uiMipMapLevelNum)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateCubeTexture(uiEdgeLength, Format, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVolumeTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT::CreateVolumeTexture(UInt32			uiWidth,
												UInt32			uiHeight,
												UInt32			uiDepth,
												PIXEL_FORMAT	Format,
												UInt32			uiMipMapLevelNum)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateVolumeTexture(uiWidth, uiHeight, uiDepth, Format, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT::CreateVertexShaderFromFile(const QDT_STRING & sFileName)
{
	return (_pRenderContext->CreateVertexShaderFromFile(sFileName));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER * RENDER_CONTEXT::CreatePixelShaderFromFile(const QDT_STRING & sFileName)
{
	return (_pRenderContext->CreatePixelShaderFromFile(sFileName));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER * RENDER_CONTEXT::CreateVertexShader(UInt8 * pShader, UInt32 nShaderSize)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreateVertexShader(pShader, nShaderSize));
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShader
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER * RENDER_CONTEXT::CreatePixelShader(UInt8 * pShader, UInt32 nShaderSize)
{
	AUTO_LOCK	AL(_pMutex);
	return (_pRenderContext->CreatePixelShader(pShader, nShaderSize));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT::CreateRenderTarget(UInt32 uiWidth, 
													   UInt32 uiHeight, 
													   PIXEL_FORMAT format)
{
	QDT_ASSERT(_pRenderContext);
	return (_pRenderContext->CreateRenderTarget(uiWidth, uiHeight, format));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeRenderTarget
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT::CreateCubeRenderTarget(UInt32		uiEdgeLength,
														   PIXEL_FORMAT	format)
{
	QDT_ASSERT(_pRenderContext);
	return (_pRenderContext->CreateCubeRenderTarget(uiEdgeLength, format));
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexShader(I_VERTEX_SHADER * pVertexShader)
{
	_pRenderContext->SetVertexShader(pVertexShader);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexShaderConstant(UInt32 nRegister, 
											 const VECTOR &	v)
{
	_pRenderContext->SetVertexShaderConstant(nRegister, v);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexShaderConstant(UInt32 nRegister, 
											 const MATRIX &	m)
{
	_pRenderContext->SetVertexShaderConstant(nRegister, m);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexShaderConstant(UInt32 nRegister, 
											 const COLOR_FLOAT & c)
{
	_pRenderContext->SetVertexShaderConstant(nRegister, c);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	26/01/06:	PDE - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexShaderConstant(UInt32 nRegister, 
											 Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
	_pRenderContext->SetVertexShaderConstant(nRegister, f1, f2, f3, f4);
}


//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderArrayConstant
//	Object:		
//	05-11-29:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetVertexShaderArrayConstant(UInt32			nRegister,
													 UInt32			uiNbrValues,
													 const float *	pValues)
{
	_pRenderContext->SetVertexShaderArrayConstant(nRegister, uiNbrValues, pValues);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderArrayConstant
//	Object:		
//	06-01-16:	PDE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetPixelShaderArrayConstant(UInt32			nRegister,
													UInt32			uiNbrValues,
													const float *	pValues)
{
	_pRenderContext->SetPixelShaderArrayConstant(nRegister, uiNbrValues, pValues);
}


//-----------------------------------------------------------------------------
//	Name:		SetPixelShader
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetPixelShader(I_PIXEL_SHADER * pPixelShader)
{
	_pRenderContext->SetPixelShader(pPixelShader);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstantF
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetPixelShaderConstant(UInt32 nRegister, 
										    const VECTOR & v)
{
	_pRenderContext->SetPixelShaderConstant(nRegister, v);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstantF
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetPixelShaderConstant(UInt32 nRegister, 
										    const MATRIX & m)
{
	_pRenderContext->SetPixelShaderConstant(nRegister, m);
}


//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	26/01/06:	PDE - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetPixelShaderConstant(UInt32 nRegister, 
											Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
	_pRenderContext->SetPixelShaderConstant(nRegister, f1, f2, f3, f4);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetPixelShaderConstant(UInt32 nRegister, 
											const COLOR_FLOAT & c)
{
	_pRenderContext->SetPixelShaderConstant(nRegister, c);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-10-14:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetPixelShaderConstant(UInt32 nRegister, 
											Float32 r)
{
	_pRenderContext->SetPixelShaderConstant(nRegister, r);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderTextureParameter
//	Object:		
//	05-09-20:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetPixelShaderTexture(UInt32		nTexture,
											  I_TEXTURE *	pTexture)
{
	_pRenderContext->SetPixelShaderTexture(nTexture, pTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetBlendMode(BLEND_MODE src,
								  BLEND_OP op,
								  BLEND_MODE dest)
{
	_pRenderContext->SetBlendMode(src, op, dest);
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendMode
//	Object:		
//	05-09-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::GetBlendMode(BLEND_MODE & src,
								  BLEND_OP & op,
								  BLEND_MODE & dest)
{
	_pRenderContext->GetBlendMode(src, op, dest);
}

//-----------------------------------------------------------------------------
//	Name:		DrawPrimitive
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::DrawPrimitive(PRIMITIVE_TYPE nType, 
								   UInt32 nPrimitiveCount, 
								   UInt32 nStartVertex)
{
	AUTO_LOCK	AL(_pMutex);
	_pRenderContext->DrawPrimitive(nType, nPrimitiveCount, nStartVertex);
}

//-----------------------------------------------------------------------------
//	Name:		DrawIndexedPrimitive
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::DrawIndexedPrimitive(PRIMITIVE_TYPE nType, 
										  UInt32 nMinIndex, 
										  UInt32 nNumVertices, 
										  UInt32 nStartIndex, 
										  UInt32 nPrimitiveCount)
{
	AUTO_LOCK	AL(_pMutex);
	_pRenderContext->DrawIndexedPrimitive(nType, nMinIndex, nNumVertices, nStartIndex, nPrimitiveCount);
}

//-----------------------------------------------------------------------------
//	Name:		DrawPoints
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::DrawPoints(UInt32				uiNbrPoints,
								   const DEBUG_VERTEX *	pVertices,
								   const MATRIX &		ViewMatrix,
								   const MATRIX &		ProjectionMatrix)
{
	AUTO_LOCK	AL(_pMutex);
	_pRenderContext->DrawPoints(uiNbrPoints, pVertices, ViewMatrix, ProjectionMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		DrawLines
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::DrawLines(UInt32				uiNbrLines,
								  const DEBUG_VERTEX *	pVertices,
								  const MATRIX &		ViewMatrix,
								  const MATRIX &		ProjectionMatrix)
{
	AUTO_LOCK	AL(_pMutex);
	_pRenderContext->DrawLines(uiNbrLines, pVertices, ViewMatrix, ProjectionMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		DrawTriangles
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::DrawTriangles(UInt32				uiNbrTriangles,
									  const DEBUG_VERTEX *	pVertices,
									  const MATRIX &		ViewMatrix,
									  const MATRIX &		ProjectionMatrix)
{
	AUTO_LOCK	AL(_pMutex);
	_pRenderContext->DrawTriangles(uiNbrTriangles, pVertices, ViewMatrix, ProjectionMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		SetCullMode
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetCullMode(CULL_MODE eCullMode)
{
	_pRenderContext->SetCullMode(eCullMode);
}

//-----------------------------------------------------------------------------
//	Name:		GetCullMode
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
CULL_MODE RENDER_CONTEXT::GetCullMode()
{
	return (_pRenderContext->GetCullMode());
}

//-----------------------------------------------------------------------------
//	Name:		SetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetFillMode(FILL_MODE	eFillMode)
{
	_pRenderContext->SetFillMode(eFillMode);
}

//-----------------------------------------------------------------------------
//	Name:		GetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
FILL_MODE	RENDER_CONTEXT::GetFillMode()
{
	return (_pRenderContext->GetFillMode());
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT::GetCurrentRenderTarget(UInt32 uiRenderTargetIndex)
{
	return (_pRenderContext->GetCurrentRenderTarget(uiRenderTargetIndex));
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTargetCubeFace
//	Object:		
//	06-03-17:	RMA - Created
//-----------------------------------------------------------------------------
CUBEMAP_FACE RENDER_CONTEXT::GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex)
{
	return (_pRenderContext->GetCurrentRenderTargetCubeFace(uiRenderTargetIndex));
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentDepthBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT::GetCurrentDepthBuffer()
{
	return (_pRenderContext->GetCurrentDepthBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		GetFrontBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT::GetFrontBuffer()
{
	return (_pRenderContext->GetFrontBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		GetDepthBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT::GetDepthBuffer()
{
	return (_pRenderContext->GetDepthBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentRenderTarget
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetCurrentRenderTarget(I_RENDER_TARGET * pSurface, UInt32 uiRenderTarget, CUBEMAP_FACE nCubeMapFace)
{
	_pRenderContext->SetCurrentRenderTarget(pSurface, uiRenderTarget, nCubeMapFace);
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentDepthBuffer
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetCurrentDepthBuffer(I_RENDER_TARGET * pSurface)
{
	_pRenderContext->SetCurrentDepthBuffer(pSurface);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::Clear(UInt32 flags, 
						   UInt8 r,
						   UInt8 g,
						   UInt8 b, 
						   UInt8 a,
						   float z)
{
	_pRenderContext->Clear(flags, r, g, b, a, z);
}

//-----------------------------------------------------------------------------
//	Name:		SetViewport
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetViewport(Int32 x, 
								 Int32 y, 
								 Int32 width, 
								 Int32 height)
{
	_pRenderContext->SetViewport(x, y, width, height);
}

//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	06-03-29:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::GetViewport(Int32 &	x,
									Int32 &	y,
									Int32 &	width,
									Int32 &	height)
{
	_pRenderContext->GetViewport(x, y, width, height);
}

//-----------------------------------------------------------------------------
//	Name:		IsFullScreen
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
Bool RENDER_CONTEXT::IsFullScreen()
{
	return (_pRenderContext->IsFullScreen());
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenWidth
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 RENDER_CONTEXT::GetScreenWidth()
{
	return (_pRenderContext->GetScreenWidth());
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenHeight
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 RENDER_CONTEXT::GetScreenHeight()
{
	return (_pRenderContext->GetScreenHeight());
}

//-----------------------------------------------------------------------------
//	Name:		CopySurface
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::CopySurface(I_RENDER_TARGET * pSrc, 
								 I_RENDER_TARGET * pDst)
{
	QDT_ASSERT(pSrc);
	QDT_ASSERT(pDst);
	
	_pRenderContext->CopySurface(pSrc, pDst);
}

//-----------------------------------------------------------------------------
//	Name:		SetTexture
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetTexture(UInt32		i, 
								I_TEXTURE *	pTexture)
{
	_pRenderContext->SetTexture(i, pTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexDeclaration
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration)
{
	QDT_ASSERT(pVertexDeclaration);
	_pRenderContext->SetVertexDeclaration(pVertexDeclaration);
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthTest
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::EnableDepthTest(Bool bState)
{
	_pRenderContext->EnableDepthTest(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsDepthTestEnabled>
//	Object:		
//	06-06-19:	SBE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT::IsDepthTestEnabled()
{
	return (_pRenderContext->IsDepthTestEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetDepthTestFunc
//	Object:		
//	05-11-22:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetDepthTestFunc(COMP_FUNC	CF)
{
	_pRenderContext->SetDepthTestFunc(CF);
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthWrite
//	Object:		
//	05-11-22:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::EnableDepthWrite(bool	bState)
{
	_pRenderContext->EnableDepthWrite(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsDepthWriteEnabled
//	Object:		
//	06-06-19:	SBE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT::IsDepthWriteEnabled()
{
	return (_pRenderContext->IsDepthWriteEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		EnableScissorTest
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::EnableScissorTest(Bool bState)
{
	_pRenderContext->EnableScissorTest(bState);
}

//-----------------------------------------------------------------------------
//	Name:		SetScissorRect
//	Object:		
//	05-08-30:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetScissorRect(UInt32 x, 
									UInt32 y, 
									UInt32 width, 
									UInt32 height)
{
	_pRenderContext->SetScissorRect(x, y, width, height);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetVertexBuffer(UInt32 stream, 
									 I_VERTEX_BUFFER * pBuffer)
{
	_pRenderContext->SetVertexBuffer(stream, pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		SetIndexBuffer
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::SetIndexBuffer(I_INDEX_BUFFER * pBuffer)
{
	_pRenderContext->SetIndexBuffer(pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		EnableAlphaTest
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::EnableAlphaTest(Bool	bState)
{
	_pRenderContext->EnableAlphaTest(bState);
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestFunc
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetAlphaTestFunc(COMP_FUNC	CF)
{
	_pRenderContext->SetAlphaTestFunc(CF);
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestRef
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::SetAlphaTestRef(UInt8	Ref)
{
	_pRenderContext->SetAlphaTestRef(Ref);
}

//-----------------------------------------------------------------------------
//	Name:		IsDisplayWindowActive
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT::IsDisplayWindowActive()
{
	return	(_pRenderContext->IsDisplayWindowActive());
}

//-----------------------------------------------------------------------------
//	Name:		HasDisplayInputFocus
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT::HasDisplayInputFocus()
{
	return	(_pRenderContext->HasDisplayInputFocus());
}

//-----------------------------------------------------------------------------
//	Name:		EnableColorWrite
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::EnableColorWrite(Bool	bR,
										 Bool	bG,
										 Bool	bB,
										 Bool	bA,
										 UInt32	uiRenderTargetIndex)
{
	_pRenderContext->EnableColorWrite(bR, bG, bB, bA, uiRenderTargetIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SupportMultipleRenderTargets
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT::SupportMultipleRenderTargets()
{
	return (_pRenderContext->SupportMultipleRenderTargets());
}

//-----------------------------------------------------------------------------
//	Name:		SaveStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void RENDER_CONTEXT::SaveStates()
{
	_pRenderContext->SaveStates();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		RestoreStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void RENDER_CONTEXT::RestoreStates()
{
	_pRenderContext->RestoreStates();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CleanStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void RENDER_CONTEXT::CleanStates()
{
	_pRenderContext->CleanStates();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShaderFromCgProgram
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
I_VERTEX_SHADER *	RENDER_CONTEXT::CreateVertexShaderFromCgProgram(void *	pCgProgram)
{
	return (_pRenderContext->CreateVertexShaderFromCgProgram(pCgProgram));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShaderFromCgProgram
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
I_PIXEL_SHADER *	RENDER_CONTEXT::CreatePixelShaderFromCgProgram(void *	pCgProgram)
{
	return (_pRenderContext->CreatePixelShaderFromCgProgram(pCgProgram));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CreateTextureFromID
//	Object:		
//	06-02-09:	FBO - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
I_TEXTURE *	RENDER_CONTEXT::CreateTextureFromID(void *	pID, 
												UInt32 iWidth, 
												UInt32 iHeight)
{
	return (_pRenderContext->CreateTextureFromID(pID, iWidth, iHeight));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CreateCubeTextureFromID
//	Object:		
//	06-03-03:	SBE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
I_TEXTURE *	RENDER_CONTEXT::CreateCubeTextureFromID(void *	pID, 
													UInt32 iLength)
{
	return (_pRenderContext->CreateCubeTextureFromID(pID, iLength));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SupportFloatingPointBlending
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT::SupportFloatingPointBlending()
{
	return (_pRenderContext->SupportFloatingPointBlending());
}
//-----------------------------------------------------------------------------
//	Name:		ClearRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::ClearRenderStats()
{
	_pRenderContext->ClearRenderStats();
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
RENDER_STATS &	RENDER_CONTEXT::GetRenderStats()
{
	return (_pRenderContext->GetRenderStats());
}

//-----------------------------------------------------------------------------
//	Name:		TakeScreenShot
//	Object:		
//	06-04-13:	FBO - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::TakeScreenShot(const QDT_STRING& sFilePath)
{
	AUTO_LOCK	AL(_pMutex);
	_pRenderContext->TakeScreenShot(sFilePath);
}

//-----------------------------------------------------------------------------
//	Name:		GetRGBFrameBuffer
//	Object:		
//	06-04-13:	FBO - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::GetRGBFrameBuffer(void** pRGBBuffer, UInt32& iWidth, UInt32& iHeight)
{
	_pRenderContext->GetRGBFrameBuffer(pRGBBuffer, iWidth, iHeight);
}

//-----------------------------------------------------------------------------
//	Name:		FindSwapChain
//	Object:		
//	06-05-18:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
I_SWAP_CHAIN *	RENDER_CONTEXT::FindSwapChain(HWND	hWnd)
{
	return (_pRenderContext->FindSwapChain(hWnd));
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetCurrentSwapChain
//	Object:		
//	06-05-19:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void	RENDER_CONTEXT::SetCurrentSwapChain(I_SWAP_CHAIN *	pSwapChain)
{
	_pRenderContext->SetCurrentSwapChain(pSwapChain);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetMaxScreenWidth
//	Object:		
//	06-05-22:	RMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT::GetMaxScreenWidth()
{
	return (_pRenderContext->GetMaxScreenWidth());
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxScreenSize
//	Object:		
//	06-05-22:	RMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT::GetMaxScreenHeight()
{
	return (_pRenderContext->GetMaxScreenHeight());
}

//-----------------------------------------------------------------------------
//	Name:		SetWindowHandle
//	Object:		
//	06-06-13:	SBE - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void	RENDER_CONTEXT::SetWindowHandle(HWND	hWnd)
{
	_pRenderContext->SetWindowHandle(hWnd);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetTimeFrame
//	Object:		
//	06-07-18:	VHA - Created
//-----------------------------------------------------------------------------
Float64	RENDER_CONTEXT::GetTimeFrame()
{
	return _fTimeFrame;
}

//-----------------------------------------------------------------------------
//	Name:		GetTimeSwap
//	Object:		
//	06-07-18:	VHA - Created
//-----------------------------------------------------------------------------
Float64	RENDER_CONTEXT::GetTimeSwap()
{
	return _fTimeSwap;
}

//-----------------------------------------------------------------------------
//	Name:		GetFPS
//	Object:		
//	06-07-18:	VHA - Created
//-----------------------------------------------------------------------------
Float64	RENDER_CONTEXT::GetFPS()
{
	return _fFPS;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
