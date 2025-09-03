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
//	CLASS:	I_RENDER_CONTEXT_IMPLEMENTOR
//	The I_RENDER_CONTEXT_IMPLEMENTOR class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_I_RENDER_CONTEXT_IMPLEMENTOR_H__
#define __KRENDERER_RENDERER_I_RENDER_CONTEXT_IMPLEMENTOR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KMATH(MATRIX)
DECL_KMATH(VECTOR)
DECL_KMATH(COLOR_FLOAT)
DECL_KRENDERER(I_SWAP_CHAIN)
DECL_KRENDERER(S_RENDER_MODE)
DECL_KRENDERER(S_VERTEX_ELEMENT)
DECL_KRENDERER(I_INDEX_BUFFER)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_VERTEX_DECLARATION)
DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(I_SWAP_CHAIN)
DECL_KRENDERER(RENDER_STATS)

#define QDT_INCLUDE_PC_WINDOWS
#include INCL_KCORE(Includes_PC/Includes_PC)

#include INCL_KRENDERER(Renderer/RendererTypeDefs)

#define MAX_TEXTURE_STAGES 16
#define MAX_VERTEX_STREAMS 16

namespace	QDT
{
	namespace	KRENDERER
	{
		class I_RENDER_CONTEXT_IMPLEMENTOR
		{
		public:

			virtual						   ~I_RENDER_CONTEXT_IMPLEMENTOR() {}

#ifdef PLATFORM_PC
			virtual I_SWAP_CHAIN *			CreateAdditionalSwapChain(HWND hWnd) = 0;
			virtual I_SWAP_CHAIN *			FindSwapChain(HWND hWnd) = 0;
			virtual void					SetCurrentSwapChain(I_SWAP_CHAIN * pSwapChain) = 0;

			virtual bool					SetFullScreenSize(UInt32 uiWidth, UInt32 uiHeight) = 0;
			virtual void					SetFullScreen(bool bFullScreen) = 0;
			virtual UInt32					GetNbrRenderModes() = 0;
			virtual const S_RENDER_MODE &	GetRenderMode(UInt32 i) = 0;
			virtual HWND					GetRenderWindow() = 0;
			virtual Int32					GetWindowPosX() = 0;
			virtual Int32					GetWindowPosY() = 0;
			virtual void					SaveStates() = 0;
			virtual void					RestoreStates() = 0;
			virtual void					CleanStates() = 0;
			virtual void					SetWindowHandle(HWND hWnd) = 0;
#endif

			virtual Bool					SupportMultipleRenderTargets() = 0;
			virtual Bool					SupportShaderModel(SHADER_MODEL sm) = 0;
			virtual Bool					SupportFloatingPointBlending() = 0;

			virtual void					BeginScene() = 0;
			virtual void					EndScene() = 0;
			virtual void					Present() = 0;

			virtual void					ClearRenderStats() = 0;
			virtual RENDER_STATS &			GetRenderStats() = 0;

			virtual void					DrawPrimitive(PRIMITIVE_TYPE nType, UInt32 nPrimitiveCount, UInt32 nStartVertex) = 0;
			virtual void					DrawIndexedPrimitive(PRIMITIVE_TYPE nType, UInt32 nMinIndex, UInt32 nMaxIndex, UInt32 nStartIndex, UInt32 nPrimitiveCount) = 0;
			virtual void					DrawPoints(UInt32 uiNbrPoints, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix) = 0;
			virtual void					DrawLines(UInt32 uiNbrLines, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix) = 0;
			virtual void					DrawTriangles(UInt32 uiNbrTriangles, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix) = 0;

			virtual I_VERTEX_BUFFER *		CreateVertexBuffer(UInt32 vertexCount, UInt32 vertexStride, bool bDynamic) = 0;
			virtual I_INDEX_BUFFER *		CreateIndexBuffer(UInt32 nIndexCount) = 0;
			virtual I_VERTEX_DECLARATION *	CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements, UInt32 numElements) = 0;
			virtual I_TEXTURE *				CreateTexture(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum) = 0;
			virtual	I_TEXTURE *				CreateCubeTexture(UInt32 uiEdgeLength, PIXEL_FORMAT eFormat, UInt32 uiMipMapLevelNum) = 0;
			virtual I_TEXTURE *				CreateVolumeTexture(UInt32 uiWidth, UInt32 uiHeight, UInt32 uiDepth, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum) = 0;
			virtual I_VERTEX_SHADER *		CreateVertexShaderFromFile(const QDT_STRING & sFileName) = 0;
			virtual I_PIXEL_SHADER *		CreatePixelShaderFromFile(const QDT_STRING & sFileName) = 0;
			virtual I_VERTEX_SHADER *		CreateVertexShader(UInt8 * pShader, UInt32 nShaderSize) = 0;
			virtual I_PIXEL_SHADER *		CreatePixelShader(UInt8 * pShader, UInt32 nShaderSize) = 0;
			virtual I_RENDER_TARGET *		CreateRenderTarget(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT format) = 0;
			virtual I_RENDER_TARGET *		CreateCubeRenderTarget(UInt32 uiEdgeLength, PIXEL_FORMAT format) = 0;

			// For maya only (directly receive cg programs)
#if defined (PLATFORM_PC)
			virtual I_VERTEX_SHADER *		CreateVertexShaderFromCgProgram(void * pCgProgram) = 0;
			virtual I_PIXEL_SHADER *		CreatePixelShaderFromCgProgram(void * pCgProgram) = 0;
			virtual I_TEXTURE *				CreateTextureFromID(void * pID, UInt32 iWidth, UInt32 iHeight) = 0;
			virtual I_TEXTURE *				CreateCubeTextureFromID(void * pID, UInt32 iLength) = 0;
#endif

			virtual void					SetVertexShader(I_VERTEX_SHADER * pVertexShader) = 0;
			virtual void					SetVertexShaderConstant(UInt32 nRegister, const VECTOR & v) = 0;
			virtual void					SetVertexShaderConstant(UInt32 nRegister, const MATRIX & m) = 0;
			virtual void					SetVertexShaderConstant(UInt32 nRegister, const COLOR_FLOAT & c) = 0;
			virtual void					SetVertexShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4) = 0;
			virtual	void					SetVertexShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues) = 0;
			virtual void					SetPixelShader(I_PIXEL_SHADER * pPixelShader) = 0;
			virtual void					SetPixelShaderConstant(UInt32 nRegister, const VECTOR & v) = 0;
			virtual void					SetPixelShaderConstant(UInt32 nRegister, const MATRIX & m) = 0;
			virtual void					SetPixelShaderConstant(UInt32 nRegister, const COLOR_FLOAT & c) = 0;
			virtual void					SetPixelShaderConstant(UInt32 nRegister, Float32 r) = 0;
			virtual void					SetPixelShaderTexture(UInt32 nTexture, I_TEXTURE * pTexture) = 0;
			virtual void					SetPixelShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4) = 0;
			virtual	void					SetPixelShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues) = 0;

			virtual void					SetBlendMode(BLEND_MODE src, BLEND_OP op, BLEND_MODE dest) = 0;
			virtual void					GetBlendMode(BLEND_MODE & src, BLEND_OP & op, BLEND_MODE & dest) = 0;

			virtual void					SetVertexBuffer(UInt32 stream, I_VERTEX_BUFFER * pBuffer) = 0;

			virtual void					SetIndexBuffer(I_INDEX_BUFFER * pBuffer) = 0;

			virtual void					SetTexture(UInt32 i, I_TEXTURE * pTexture) = 0;

			virtual void					SetViewport(Int32 x, Int32 y, Int32 width, Int32 height) = 0;
			virtual void					GetViewport(Int32& x, Int32& y, Int32& width, Int32& height) = 0;

			virtual void					SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration) = 0;

			virtual void					Clear(UInt32 flags, UInt8 r, UInt8 g, UInt8 b, UInt8 a, float z) = 0;

			virtual void					SetCullMode(CULL_MODE eCullMode) = 0;
			virtual CULL_MODE				GetCullMode() const = 0;

			virtual void					SetFillMode(FILL_MODE eFillMode) = 0;
			virtual FILL_MODE				GetFillMode() const = 0;

			virtual I_RENDER_TARGET *		GetCurrentRenderTarget(UInt32 uiRenderTargetIndex = 0) const = 0;
			virtual CUBEMAP_FACE			GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex = 0) const = 0;
			virtual I_RENDER_TARGET *		GetCurrentDepthBuffer() const = 0;
			virtual I_RENDER_TARGET *		GetFrontBuffer() const = 0;
			virtual I_RENDER_TARGET *		GetDepthBuffer() const = 0;
			
			virtual void					EnableColorWrite(Bool bR, Bool bG, Bool bB, Bool bA, UInt32 uiRenderTargetIndex) = 0;
			virtual void					SetCurrentRenderTarget(I_RENDER_TARGET * pSurface, UInt32 uiRenderTargetIndex = 0, CUBEMAP_FACE nCubeMapFace = CF_NONE) = 0;
			virtual void					SetCurrentDepthBuffer(I_RENDER_TARGET * pSurface) = 0;

			virtual bool					IsFullScreen() const = 0;
			virtual UInt32					GetScreenWidth() const = 0;
			virtual UInt32					GetScreenHeight() const = 0;
			virtual UInt32					GetMaxScreenWidth() const = 0;
			virtual UInt32					GetMaxScreenHeight() const = 0;

			virtual void					EnableDepthTest(bool bState) = 0;
			virtual Bool					IsDepthTestEnabled() const = 0;
			virtual void					SetDepthTestFunc(COMP_FUNC CF) = 0;
			virtual void					EnableDepthWrite(bool bState) = 0;
			virtual Bool					IsDepthWriteEnabled() const = 0;

			virtual void					EnableScissorTest(bool bState) = 0;
			virtual void					SetScissorRect(UInt32 x, UInt32 y, UInt32 width, UInt32 height) = 0;

			virtual void					EnableAlphaTest(Bool bState) = 0;
			virtual void					SetAlphaTestFunc(COMP_FUNC CF) = 0;
			virtual void					SetAlphaTestRef(UInt8 Ref) = 0;

			virtual void					CopySurface(I_RENDER_TARGET * pSrc, I_RENDER_TARGET * pDst) = 0;

			virtual bool					IsDisplayWindowActive() const = 0;
			virtual bool					HasDisplayInputFocus() const = 0;

			virtual void					TakeScreenShot(const QDT_STRING& sFilePath) const = 0;
			virtual void					GetRGBFrameBuffer(void** pRGBBuffer, UInt32& iWidth, UInt32& iHeight) const = 0;

#ifdef PLATFORM_PS3
			virtual	void					PrintDebugMessageV(Float32 fXPos, Float32 fYPos, const Char* Text, void* Args) = 0;
			virtual void					PrintConsoleMessageV(const Char* Text, void* Args) = 0;
#endif

			// Platform specific Color Conversions
			//			OGL		DX
			//	PC		ABGR	ARGB
			//	PS3		RGBA	 X
			virtual UInt32					GetPlatformIntColor(const COLOR& Color) = 0;
		};
	}
}

#endif // __KRENDERER_RENDERER_I_RENDER_CONTEXT_IMPLEMENTOR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

