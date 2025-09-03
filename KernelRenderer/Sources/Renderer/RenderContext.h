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

#ifndef __KRENDERER_RENDERER_RENDER_CONTEXT_H__
#define __KRENDERER_RENDERER_RENDER_CONTEXT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING)
DECL_KMATH(VECTOR)
DECL_KMATH(MATRIX)
DECL_KMATH(COLOR_FLOAT)
DECL_KRENDERER(I_SWAP_CHAIN)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_VERTEX_DECLARATION)
DECL_KRENDERER(I_INDEX_BUFFER)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(S_VERTEX_ELEMENT)
DECL_KRENDERER(S_RENDER_MODE)
DECL_KRENDERER(I_RENDER_CONTEXT_IMPLEMENTOR)
DECL_KRENDERER(RENDER_STATS)

#include "RendererTypeDefs.h"
#include "IRenderContextImplementor.h"
#include	INCL_KCORE(Thread/Mutex)

namespace	QDT
{
	namespace	KRENDERER
	{
		class RENDER_CONTEXT
		{
		public:

			enum RENDER_CONTEXT_TYPE
			{
				RCT_UNDEFINED,
				RCT_DIRECT3D9,
				RCT_OPENGL,
				RCT_OPENGL_FOR_MAYA,
				RCT_PSGL
			};

			// Pre-init methods
			static void								SetRenderContextType(RENDER_CONTEXT_TYPE RCT) { _nRenderContextType = RCT; }
			static RENDER_CONTEXT_TYPE				GetRenderContextType() { return _nRenderContextType; }

			_INLINE_ static void					ClearRenderStats();
			_INLINE_ static RENDER_STATS &			GetRenderStats();

#ifdef PLATFORM_PC	
			
			 static void							Init(HWND hWnd, UInt32 uiWidth, UInt32 uiHeight, Bool bFullscreen, Bool bCreateDepthBuffer = true);		
			
			// Windows specific management		 
			_INLINE_ static Bool					SetFullScreenSize(UInt32 uiWidth, UInt32 uiHeight);
			_INLINE_ static void					SetFullScreen(Bool bFullScreen);
			_INLINE_ static UInt32					GetNbrRenderModes();
			_INLINE_ static const S_RENDER_MODE &	GetRenderMode(UInt32 i);
			_INLINE_ static HWND					GetRenderWindow();
			_INLINE_ static Int32					GetWindowPosX();
			_INLINE_ static Int32					GetWindowPosY();

			// Multi-Windows
			_INLINE_ static I_SWAP_CHAIN *			CreateAdditionalSwapChain(HWND hWnd);
			_INLINE_ static I_SWAP_CHAIN *			FindSwapChain(HWND hWnd);
			_INLINE_ static void					SetCurrentSwapChain(I_SWAP_CHAIN * pSwapChain);
					 static void					ReleaseSwapChain(I_SWAP_CHAIN * pSwapChain);

			// Methods used to restore state for Maya
			_INLINE_ static	void					SaveStates();
			_INLINE_ static	void					RestoreStates();
			_INLINE_ static	void					CleanStates();
			_INLINE_ static void					SetWindowHandle(HWND hWnd);
		
#else
			static void								Init(DISPLAY_MODE DisplayMode = DM_DEFAULT);
#endif
			
			_INLINE_ static Bool					IsInit();
			static void								Release();

			// Hardware capacity
			_INLINE_ static Bool					SupportMultipleRenderTargets();
			_INLINE_ static Bool					SupportShaderModel(SHADER_MODEL sm);
			_INLINE_ static Bool					SupportFloatingPointBlending();

			_INLINE_ static void					BeginScene();
			_INLINE_ static void					EndScene();
			_INLINE_ static void					Present();

			_INLINE_ static I_VERTEX_BUFFER *		CreateVertexBuffer(UInt32 vertexCount, UInt32 stride, Bool bDynamic = false);
			_INLINE_ static I_INDEX_BUFFER *		CreateIndexBuffer(UInt32 indexCount);
			_INLINE_ static I_TEXTURE *				CreateTexture(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT eFormat, UInt32 uiMipMapLevelNum = 0);
			_INLINE_ static I_TEXTURE *				CreateCubeTexture(UInt32 uiEdgeLength, PIXEL_FORMAT eFormat, UInt32 uiMipMapLevelNum = 0);
			_INLINE_ static I_TEXTURE *				CreateVolumeTexture(UInt32 uiWidth, UInt32 uiHeight, UInt32 uiDepth, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum = 0);
			_INLINE_ static I_VERTEX_DECLARATION *	CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements, UInt32 numElements);
			_INLINE_ static I_VERTEX_SHADER *		CreateVertexShaderFromFile(const QDT_STRING & sFileName);
			_INLINE_ static I_PIXEL_SHADER *		CreatePixelShaderFromFile(const QDT_STRING & sFileName);
			_INLINE_ static I_VERTEX_SHADER *		CreateVertexShader(UInt8 * pShader, UInt32 nShaderSize);
			_INLINE_ static I_PIXEL_SHADER *		CreatePixelShader(UInt8 * pShader, UInt32 nShaderSize);
			_INLINE_ static I_RENDER_TARGET *		CreateRenderTarget(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT format);
			_INLINE_ static I_RENDER_TARGET *		CreateCubeRenderTarget(UInt32 uiEdgeLength, PIXEL_FORMAT format);

			// For maya only (directly receive cg programs)
#if defined (PLATFORM_PC)
			_INLINE_ static I_VERTEX_SHADER *		CreateVertexShaderFromCgProgram(void * pCgProgram);
			_INLINE_ static I_PIXEL_SHADER *		CreatePixelShaderFromCgProgram(void * pCgProgram);
			_INLINE_ static I_TEXTURE *				CreateTextureFromID(void * pID, UInt32 iWidth, UInt32 iHeight);
			_INLINE_ static I_TEXTURE *				CreateCubeTextureFromID(void * pID, UInt32 iLength);
#endif

			static void					ReleaseVertexBuffer(I_VERTEX_BUFFER * pVertexBuffer);
			static void					ReleaseIndexBuffer(I_INDEX_BUFFER * pIndexBuffer);
			static void					ReleaseTexture(I_TEXTURE * pTexture);
			static void					ReleaseVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration);
			static void					ReleaseVertexShader(I_VERTEX_SHADER * pVertexShader);
			static void					ReleasePixelShader(I_PIXEL_SHADER * pPixelShader);
			static void					ReleaseRenderTarget(I_RENDER_TARGET * pRenderTarget);

			_INLINE_ static void					SetVertexShader(I_VERTEX_SHADER * pVertexShader);
			_INLINE_ static void					SetVertexShaderConstant(UInt32 Register, const VECTOR & v);
			_INLINE_ static void					SetVertexShaderConstant(UInt32 Register, const MATRIX & m);
			_INLINE_ static void					SetVertexShaderConstant(UInt32 Register, const COLOR_FLOAT & c);
			_INLINE_ static void					SetVertexShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4);

			_INLINE_ static void					SetVertexShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues);


			_INLINE_ static void					SetPixelShader(I_PIXEL_SHADER * pPixelShader);
			_INLINE_ static void					SetPixelShaderConstant(UInt32 Register, const VECTOR & v);
			_INLINE_ static void					SetPixelShaderConstant(UInt32 Register, const MATRIX & m);
			_INLINE_ static void					SetPixelShaderConstant(UInt32 Register, const COLOR_FLOAT & c);
			_INLINE_ static void					SetPixelShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4);
			_INLINE_ static void					SetPixelShaderTexture(UInt32 Texture, I_TEXTURE * pTexture);

			_INLINE_ static void					SetPixelShaderConstant(UInt32 nRegister, Float32 r);
			_INLINE_ static void					SetPixelShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues);

			_INLINE_ static void					SetBlendMode(BLEND_MODE src, BLEND_OP op, BLEND_MODE dest);
			_INLINE_ static void					GetBlendMode(BLEND_MODE & src, BLEND_OP &op, BLEND_MODE & dest);

			_INLINE_ static void					SetVertexBuffer(UInt32 stream, I_VERTEX_BUFFER * pBuffer);

			_INLINE_ static void					SetIndexBuffer(I_INDEX_BUFFER * pBuffer);

			_INLINE_ static void					SetTexture(UInt32 i, I_TEXTURE * pTexture);
			
			_INLINE_ static void					SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration);

			_INLINE_ static void					DrawPrimitive(PRIMITIVE_TYPE nType, UInt32 nPrimitiveCount, UInt32 nStartVertex);
			_INLINE_ static void					DrawIndexedPrimitive(PRIMITIVE_TYPE nType, UInt32 nMinIndex, UInt32 nNumVertices, UInt32 nStartIndex, UInt32 nPrimitiveCount);
			_INLINE_ static void					DrawPoints(UInt32 uiNbrPoints, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix);
			_INLINE_ static void					DrawLines(UInt32 uiNbrLines, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix);
			_INLINE_ static void					DrawTriangles(UInt32 uiNbrTriangles, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix);

			_INLINE_ static void					SetViewport(Int32 x, Int32 y, Int32 width, Int32 height);
			_INLINE_ static void					GetViewport(Int32& x, Int32& y, Int32& width, Int32& height); 
			_INLINE_ static void					Clear(UInt32 flags, UInt8 r, UInt8 g, UInt8 b, UInt8 a, Float32 z = 1.0f);

			_INLINE_ static void					SetCullMode(CULL_MODE eCullMode);
			_INLINE_ static CULL_MODE				GetCullMode();

			_INLINE_ static void					SetFillMode(FILL_MODE eFillMode);
			_INLINE_ static FILL_MODE				GetFillMode();

			_INLINE_ static I_RENDER_TARGET *		GetCurrentRenderTarget(UInt32 uiRenderTargetIndex = 0);
			_INLINE_ static CUBEMAP_FACE 			GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex = 0);
			_INLINE_ static I_RENDER_TARGET *		GetCurrentDepthBuffer();
			_INLINE_ static I_RENDER_TARGET *		GetFrontBuffer();
			_INLINE_ static I_RENDER_TARGET *		GetDepthBuffer();

			_INLINE_ static void					EnableColorWrite(Bool bR, Bool bG, Bool bB, Bool bA, UInt32 uiRenderTargetIndex = 0);
			_INLINE_ static void					SetCurrentRenderTarget(I_RENDER_TARGET * pSurface, UInt32 uiRenderTargetIndex = 0, CUBEMAP_FACE nCubeMapFace = CF_NONE);
			_INLINE_ static void					SetCurrentDepthBuffer(I_RENDER_TARGET * pSurface);
			_INLINE_ static void					CopySurface(I_RENDER_TARGET * pSrc, I_RENDER_TARGET * pDst);

			_INLINE_ static Bool					IsFullScreen();
			_INLINE_ static UInt32					GetScreenWidth();
			_INLINE_ static UInt32					GetScreenHeight();
			// Use the following value to create render target that match screen
			_INLINE_ static UInt32					GetMaxScreenWidth();
			_INLINE_ static UInt32					GetMaxScreenHeight();

			_INLINE_ static void					EnableDepthTest(Bool bState);
			_INLINE_ static Bool					IsDepthTestEnabled();
			_INLINE_ static void					SetDepthTestFunc(COMP_FUNC CF);
			_INLINE_ static void					EnableDepthWrite(Bool bState);
			_INLINE_ static Bool					IsDepthWriteEnabled();

			_INLINE_ static void					EnableScissorTest(Bool bState);
			_INLINE_ static void					SetScissorRect(UInt32 x, UInt32 y, UInt32 width, UInt32 height);

			_INLINE_ static void					EnableAlphaTest(Bool bState);
			_INLINE_ static void					SetAlphaTestFunc(COMP_FUNC CF);
			_INLINE_ static void					SetAlphaTestRef(UInt8 Ref);

			_INLINE_ static Bool					IsDisplayWindowActive();
			_INLINE_ static Bool					HasDisplayInputFocus();

			_INLINE_ static void					TakeScreenShot(const QDT_STRING& sFilePath);
			_INLINE_ static void					GetRGBFrameBuffer(void** pRGBBuffer, UInt32& iWidth, UInt32& iHeight);

			_INLINE_ static Float64					GetTimeFrame();
			_INLINE_ static Float64					GetTimeSwap();
			_INLINE_ static Float64					GetFPS();

			// Utility methods
					static UInt32					SizeCompressed(UInt32 nWidth, UInt32 nHeight, PIXEL_FORMAT eFormat);

			// Debug Messages
					static void						PrintDebugMessage(Float32 fXPos, Float32 fYPos, const Char* Text, ...);
					static void						PrintDebugMessageLog(const Char* Text, ...);
					static void						PrintConsoleMessage(const Char* Text, ...);

			// Platform specific Color Conversions
			//			OGL		DX
			//	PC		ABGR	ARGB
			//	PS3		RGBA	 X
					static UInt32					GetPlatformIntColor(const COLOR& Color);


			static Float32  GetAspectRatio()  {return 4.0f / 3.0f;}  //TODO: handle the 16/9 case 

			static MUTEX*	GetMutex()		  {return (_pMutex);}

		private:

			static Float64							_fTimeSwap;
			static Float64							_fTimeFrame;
			static Float64							_fFPS;

			static I_RENDER_CONTEXT_IMPLEMENTOR *	_pRenderContext;
			static RENDER_CONTEXT_TYPE				_nRenderContextType;
			static MUTEX*							_pMutex;
		};
	}
}

#ifndef _DEBUG
	#include "RenderContext.inl"
#endif

#endif //  __KRENDERER_RENDERER_RENDER_CONTEXT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
