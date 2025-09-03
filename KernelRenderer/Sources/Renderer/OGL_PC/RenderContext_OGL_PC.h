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
//	CLASS:	RENDER_CONTEXT_OGL_PC
//	The RENDER_CONTEXT_OGL_PC class implements ...
//
//	05-12-15:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_RENDER_CONTEXT_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_RENDER_CONTEXT_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer/IRenderContextImplementor)
#include INCL_KRENDERER(Renderer/RenderStats)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#define QDT_INCLUDE_PC_CG
#include INCL_KCORE(Includes_PC/Includes_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class RENDER_CONTEXT_OGL_PC : public I_RENDER_CONTEXT_IMPLEMENTOR
		{
		public:

			RENDER_CONTEXT_OGL_PC(HWND hWnd, UInt32 uiWidth, UInt32 uiHeight, Bool bFullscreen, Bool bCreateDepthBuffer);
			RENDER_CONTEXT_OGL_PC(UInt32 uiWidth, UInt32 uiHeight); // only use via gl_for_maya
			~RENDER_CONTEXT_OGL_PC();
			
			bool							ChangeFullScreenSize(UInt32 uiWidth, UInt32 uiHeight);
			I_SWAP_CHAIN *					CreateAdditionalSwapChain(HWND hWnd);
			void							ChangeWindowSize(UInt32 uiWidth, UInt32 uiHeight);
			void							ChangeWindowPos(UInt32 uiX, UInt32 uiY);
			void							SetFullScreen(bool bFullScreen);
			UInt32							GetNbrRenderModes();
			const S_RENDER_MODE &			GetRenderMode(UInt32 i);
			HWND							GetRenderWindow();
			UInt32							GetWindowPosX();
			UInt32							GetWindowPosY();
			Bool							SupportShaderModel(SHADER_MODEL sm);
			Bool							SupportFloatingPointBlending();
			Bool							SupportHardwareShadowMap();

			void							SaveStates();
			void							RestoreStates();
			void							CleanStates();

			Bool							SupportMultipleRenderTargets();
	
			void							BeginScene();
			void							EndScene();
			void							Present();

			void							ClearRenderStats();
			RENDER_STATS &					GetRenderStats();

			void							DrawPrimitive(PRIMITIVE_TYPE nType, UInt32 nPrimitiveCount, UInt32 nStartVertex);
			void							DrawIndexedPrimitive(PRIMITIVE_TYPE nType, UInt32 nMinIndex, UInt32 nNumVertices, UInt32 nStartIndex, UInt32 nPrimitiveCount);
			void							DrawPoints(UInt32 uiNbrPoints, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix);
			void							DrawLines(UInt32 uiNbrLines, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix);
			void							DrawTriangles(UInt32 uiNbrTriangles, const DEBUG_VERTEX * pVertices, const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix);

			I_VERTEX_BUFFER *				CreateVertexBuffer(UInt32 vertexCount, UInt32 vertexStride, Bool bDynamic);
			I_INDEX_BUFFER *				CreateIndexBuffer(UInt32 nIndexCount);
			I_VERTEX_DECLARATION *			CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements, UInt32 numElements);
			I_TEXTURE *						CreateTexture(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum);
			I_TEXTURE *						CreateCubeTexture(UInt32 uiEdgeLength, PIXEL_FORMAT eFormat, UInt32 uiMipMapLevelNum);
			I_TEXTURE *						CreateVolumeTexture(UInt32 uiWidth, UInt32 uiHeight, UInt32 uiDepth, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum);
			I_VERTEX_SHADER *				CreateVertexShaderFromFile(const QDT_STRING & sFileName);
			I_PIXEL_SHADER *				CreatePixelShaderFromFile(const QDT_STRING & sFileName);
			I_VERTEX_SHADER *				CreateVertexShader(UInt8 * pShader);
			I_PIXEL_SHADER *				CreatePixelShader(UInt8 * pShader);
			I_RENDER_TARGET *				CreateRenderTarget(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT format);	
			I_RENDER_TARGET *				CreateCubeRenderTarget(UInt32 uiEdgeLength, PIXEL_FORMAT format);
			I_VERTEX_SHADER *				CreateVertexShaderFromCgProgram(void * pCgProgram);
			I_PIXEL_SHADER *				CreatePixelShaderFromCgProgram(void * pCgProgram);
			I_TEXTURE *						CreateTextureFromID(void * pID, UInt32 iWidth, UInt32 iHeight);
			I_TEXTURE *						CreateCubeTextureFromID(void * pID, UInt32 iLength);

			void							SetVertexShader(I_VERTEX_SHADER * pVertexShader);
			void							SetVertexShaderConstant(UInt32 nRegister, const VECTOR & v);
			void							SetVertexShaderConstant(UInt32 nRegister, const MATRIX & m);
			void							SetVertexShaderConstant(UInt32 nRegister, const COLOR_FLOAT & c);
			void							SetVertexShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4);
			void							SetVertexShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues);
			void							SetPixelShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues);

			void							SetPixelShader(I_PIXEL_SHADER * pPixelShader);
			void							SetPixelShaderConstant(UInt32 nRegister, const VECTOR & v);
			void							SetPixelShaderConstant(UInt32 nRegister, const MATRIX & m);
			void							SetPixelShaderConstant(UInt32 nRegister, const COLOR_FLOAT & c);
			void							SetPixelShaderConstant(UInt32 nRegister, Float32 r);
			void							SetPixelShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4);
			void							SetPixelShaderTexture(UInt32 nTexture, I_TEXTURE * pTexture);

			void							SetBlendMode(BLEND_MODE src, BLEND_OP op, BLEND_MODE dest);
			void							GetBlendMode(BLEND_MODE & src, BLEND_OP & op, BLEND_MODE & dest);
			void							Clear(UInt32 flags, UInt8 r, UInt8 g, UInt8 b, UInt8 a, float z);
			void							SetViewport(Int32 x, Int32 y, Int32 width, Int32 height);
			void							GetViewport(Int32& x, Int32& y, Int32& width, Int32& height);

			void							SetVertexBuffer(UInt32 stream, I_VERTEX_BUFFER * pVertexBuffer);

			void							SetIndexBuffer(I_INDEX_BUFFER * pBuffer);

			void							SetTexture(UInt32 i, I_TEXTURE * pTexture);

			void							SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration);

			void							SetCullMode(CULL_MODE eCullMode);
			CULL_MODE						GetCullMode() const;

			void							SetFillMode(FILL_MODE eFillMode);
			FILL_MODE						GetFillMode() const;

			I_RENDER_TARGET *				GetCurrentRenderTarget(UInt32 uiRenderTargetIndex = 0) const;
			CUBEMAP_FACE					GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex = 0) const;
			I_RENDER_TARGET *				GetCurrentDepthBuffer() const;
			I_RENDER_TARGET *				GetFrontBuffer() const;
			I_RENDER_TARGET *				GetDepthBuffer() const;
			
			void							EnableColorWrite(Bool bR, Bool bG, Bool bB, Bool bA, UInt32 uiRenderTargetIndex);
			void							SetCurrentRenderTarget(I_RENDER_TARGET * pRenderTarget, UInt32 uiRenderTargetIndex = 0, CUBEMAP_FACE nCubeMapFace = CF_NONE);
			void							SetCurrentDepthBuffer(I_RENDER_TARGET * pRenderTarget);

			void							SetAddressUType(UInt32 nStage, ADDRESS_TYPE eAddressType);
			void							SetAddressVType(UInt32 nStage, ADDRESS_TYPE eAddressType);
			void							SetFilterType(UInt32 nStage, FILTER_TYPE eFilterType);

			Bool							IsFullScreen() const;
			UInt32							GetScreenWidth() const;
			UInt32							GetScreenHeight() const;

			void							EnableDepthTest(bool bState);
			void							SetDepthTestFunc(COMP_FUNC CF);
			void							EnableDepthWrite(bool bState);

			void							EnableScissorTest(Bool bState);
			void							SetScissorRect(UInt32 x, UInt32 y, UInt32 width, UInt32 height);

			bool							IsDisplayWindowActive() const;
			bool							HasDisplayInputFocus() const;

			void							CopySurface(I_RENDER_TARGET * pSrc, I_RENDER_TARGET * pDst);

			void							EnableAlphaTest(Bool bState);
			void							SetAlphaTestFunc(COMP_FUNC CF);
			void							SetAlphaTestRef(UInt8 Ref);

			UInt32							CleanShader(UInt8 * pShader, UInt8 * pCleanedShader, UInt32 nFileSize);

			CGcontext &						GetCGContext() { return (_CgContext); }
			const CGprofile &				GetCGVertexShaderProfile() { return (_CgVertexShaderProfile); }
			const CGprofile &				GetCGPixelShaderProfile() { return (_CgPixelShaderProfile); }

			// Platform specific Color Conversions
			UInt32							GetPlatformIntColor(const COLOR& Color);

		private:

			void							SetDefaultValues();
			void							InitGLContext();
			void							InitGLExtensions();
			void							CreateGLContext();
			void							UpdateVertexContext();
			void							UpdateIndexContext();
			void							RetrieveFrontAndDepthBuffer();
			UInt32							GetOGLPrimitiveType(PRIMITIVE_TYPE nType) const;
			UInt32							GetNumIndices(PRIMITIVE_TYPE eType, UInt32 nPrimitiveCount) const;

			/////////////////////////////////////////////////////////////////////////////////////////////
			// Attributes

			HWND							_hWindow;
			HDC								_hDC;
			HGLRC							_hRC;

			CGcontext						_CgContext;
			CGprofile						_CgVertexShaderProfile;
			CGprofile						_CgPixelShaderProfile;

			UInt32							_uiWidth;
			UInt32							_uiHeight;
			UInt32							_uiPosX;
			UInt32							_uiPosY;

			Bool							_bFullScreen;
			UInt32							_nNbrRenderModes;
			S_RENDER_MODE *					_pRenderModesArray;

			// Blend mode
			Bool							_bEnableAlphaBlend;
			BLEND_MODE						_eSrcBlendMode;
			BLEND_MODE						_eDstBlendMode;
			BLEND_OP						_eBlendOp;

			Bool							_bNeedUpdateVertexContext;
			I_VERTEX_DECLARATION *			_pCurrentVertexDeclaration;
			I_VERTEX_BUFFER *				_apCurrentVertexBuffer[MAX_VERTEX_STREAMS];
			Bool							_bNeedUpdateIndexContext;
			I_INDEX_BUFFER *				_pCurrentIndexBuffer;

			I_VERTEX_SHADER *				_pCurrentVertexShader;
			I_PIXEL_SHADER *				_pCurrentPixelShader;

			I_RENDER_TARGET *				_pCurrentRenderTarget;
			CUBEMAP_FACE					_nCurrentRenderTargetCubeFace;
			I_RENDER_TARGET *				_pCurrentDepthBuffer;
			I_RENDER_TARGET *				_pFrontBuffer;
			I_RENDER_TARGET *				_pDepthBuffer;

			// Texture stages states
			I_TEXTURE *						_apCurrentTextures[MAX_TEXTURE_STAGES];
			GLenum							_aCurrentTexturesTarget[MAX_TEXTURE_STAGES];
			ADDRESS_TYPE					_aAddressUType[MAX_TEXTURE_STAGES];
			ADDRESS_TYPE					_aAddressVType[MAX_TEXTURE_STAGES];
			FILTER_TYPE						_aFilterType[MAX_TEXTURE_STAGES];	

			// Render states
			Bool							_bScissorTest;

			// Viewport
			Int32							_iViewportX; 
			Int32							_iViewportY;
			Int32							_iViewportWidth; 
			Int32							_iViewportHeight;

			// Fill Mode
			FILL_MODE						_eFillMode;

			// Cull Mode
			CULL_MODE						_eCullMode;

			// Depth Mode
			Bool							_bEnableDepthTest,
				_bEnableDepthWrite;
			COMP_FUNC						_DepthTestFunc;

			// Alpha Test
			Bool							_bEnableAlphaTest;
			COMP_FUNC						_AlphaTestFunc;
			UInt8							_nAlphaTestRef;

			RENDER_STATS					_RenderStats;
		};
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_RENDER_CONTEXT_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

