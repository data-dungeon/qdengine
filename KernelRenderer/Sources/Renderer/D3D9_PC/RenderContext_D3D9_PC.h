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
//	CLASS:	RENDER_CONTEXT_D3D9_PC
//	The RENDER_CONTEXT_D3D9_PC class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_RENDER_CONTEXT_D3D9_PC_H__
#define __KRENDERER_RENDERER_RENDER_CONTEXT_D3D9_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#define QDT_INCLUDE_PC_D3D9
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/IRenderContextImplementor)
#include INCL_KRENDERER(Renderer/RenderStats)

#define MAX_SIMULTANEOUS_RENDER_TARGET 4

// Very slow!!! But can access to advanced features of the Direct3D debugger
//#define _DEBUG_PIXEL_SHADERS
//#define _DEBUG_VERTEX_SHADERS

namespace	QDT
{
	namespace	KRENDERER
	{
		class RENDER_CONTEXT_D3D9_PC : public I_RENDER_CONTEXT_IMPLEMENTOR
		{
		public:

			// Constructor & destructor
			RENDER_CONTEXT_D3D9_PC(HWND hWnd, UInt32 uiWidth, UInt32 uiHeight, Bool bFullscreen, Bool bCreateDepthBuffer);
			~RENDER_CONTEXT_D3D9_PC();

			// Unherit from IRenderContextImplementor
			bool							ChangeFullScreenSize(UInt32 uiWidth, UInt32 uiHeight);
			void							ChangeWindowSize(UInt32 uiWidth, UInt32 uiHeight);
			void							ChangeWindowPos(UInt32 uiX, UInt32 uiY);
			void							SetFullScreen(Bool bFullScreen);
			UInt32							GetNbrRenderModes();
			const S_RENDER_MODE &			GetRenderMode(UInt32 i);
			HWND							GetRenderWindow();
			UInt32							GetWindowPosX();
			UInt32							GetWindowPosY();			
			void							SaveStates();
			void							RestoreStates();
			void							CleanStates();

			Bool							SupportMultipleRenderTargets();
			Bool							SupportShaderModel(SHADER_MODEL sm);
			Bool							SupportFloatingPointBlending();
			Bool							SupportHardwareShadowMap();

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

			I_SWAP_CHAIN *					CreateAdditionalSwapChain(HWND hWnd);
			I_VERTEX_BUFFER *				CreateVertexBuffer(UInt32 vertexCount, UInt32 vertexStride, Bool bDynamic);
			I_INDEX_BUFFER *				CreateIndexBuffer(UInt32 nNbrIndices);
			I_VERTEX_DECLARATION *			CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements, UInt32 numElements);
			I_TEXTURE *						CreateTexture(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum);
			I_TEXTURE *						CreateCubeTexture(UInt32 uiEdgeLength, PIXEL_FORMAT eFormat, UInt32 uiMipMapLevelNum);
			I_TEXTURE *						CreateVolumeTexture(UInt32 uiWidth, UInt32 uiHeight, UInt32 uiDepth, PIXEL_FORMAT format, UInt32 uiMipMapLevelNum);
			I_VERTEX_SHADER *				CreateVertexShaderFromFile(const QDT_STRING & sFileName);
			I_PIXEL_SHADER *				CreatePixelShaderFromFile(const QDT_STRING & sFileName);
			I_RENDER_TARGET *				CreateRenderTarget(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT eFormat);
			I_RENDER_TARGET *				CreateCubeRenderTarget(UInt32 uiEdgeLength, PIXEL_FORMAT format);
			I_VERTEX_SHADER *				CreateVertexShader(UInt8 * pShader);
			I_PIXEL_SHADER *				CreatePixelShader(UInt8 * pShader);
			I_VERTEX_SHADER *				CreateVertexShaderFromCgProgram(void * pCgProgram);
			I_PIXEL_SHADER *				CreatePixelShaderFromCgProgram(void * pCgProgram);
			I_TEXTURE *						CreateTextureFromID(void * pID, UInt32 uiWidth, UInt32 uiHeight);
			I_TEXTURE *						CreateCubeTextureFromID(void * pID, UInt32 uiEdgeLength);

			void							SetVertexShader(I_VERTEX_SHADER * pVertexShader);
			void							SetVertexShaderConstant(UInt32 nRegister, const VECTOR & v);
			void							SetVertexShaderConstant(UInt32 nRegister, const MATRIX & m);
			void							SetVertexShaderConstant(UInt32 nRegister, const COLOR_FLOAT & c);
			void							SetVertexShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4);
			void							SetVertexShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues);
			void							SetPixelShader(I_PIXEL_SHADER * pPixelShader);
			void							SetPixelShaderConstant(UInt32 nRegister, const VECTOR & v);
			void							SetPixelShaderConstant(UInt32 nRegister, const MATRIX & m);
			void							SetPixelShaderConstant(UInt32 nRegister, const COLOR_FLOAT & c);
			void							SetPixelShaderConstant(UInt32 nRegister, Float32 r);
			void							SetPixelShaderConstant(UInt32 nRegister, Float32 f1, Float32 f2, Float32 f3, Float32 f4);
			void							SetPixelShaderTexture(UInt32 nRegister, I_TEXTURE * pTexture);
			void							SetPixelShaderArrayConstant(UInt32 nRegister, UInt32 uiNbrValues, const float * pValues);

			void							SetBlendMode(BLEND_MODE src, BLEND_OP op, BLEND_MODE dest);
			void							GetBlendMode(BLEND_MODE & src, BLEND_OP &op, BLEND_MODE & dest);

			void							Clear(UInt32 flags, UInt8 r, UInt8 g, UInt8 b, UInt8, Float32 z);
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

			Bool							IsFullScreen() const;
			UInt32							GetScreenWidth() const;
			UInt32							GetScreenHeight() const;

			void							EnableDepthTest(Bool bState);
			void							SetDepthTestFunc(COMP_FUNC CF);
			void							EnableDepthWrite(Bool bState);	

			void							EnableScissorTest(Bool bState);
			void							SetScissorRect(UInt32 x, UInt32 y, UInt32 width, UInt32 height);

			void							EnableAlphaTest(Bool bState);
			void							SetAlphaTestFunc(COMP_FUNC CF);
			void							SetAlphaTestRef(UInt8 Ref);

			bool							IsDisplayWindowActive() const;
			bool							HasDisplayInputFocus() const;

			void							CopySurface(I_RENDER_TARGET * pSrc, I_RENDER_TARGET * pDst);

			IDirect3DDevice9 *				Device() const { return _pD3DDevice; }
			const D3DCAPS9 &				Caps() const { return _Caps; }

			D3DFORMAT						GetD3DPixelFormat(PIXEL_FORMAT eFormat) const;

			// Platform specific Color Conversions
			UInt32							GetPlatformIntColor(const COLOR& Color);

		private:

			HRESULT							CheckResourceFormatSupport(IDirect3DDevice9 *	m_pd3dDevice,
																	   D3DFORMAT			fmt,
																	   D3DRESOURCETYPE		resType,
																	   DWORD				dwUsage);

			D3DPRIMITIVETYPE				GetD3DPrimitiveType(PRIMITIVE_TYPE PrimitiveType) const;
			void							Reset();
			void							SetDefaultValues();
			void							CreateRenderModesArray();
			void							DetectNVPerfHUD();
			bool							IsResolutionSupported(UInt32 nWidth, UInt32 nHeight);
			void							UpdateSampler();
			void							SetTexAddressU(UInt32 tex, D3DTEXTUREADDRESS type);
			void							SetTexAddressV(UInt32 tex, D3DTEXTUREADDRESS type);
			void							SetMinFilter(UInt32 tex, D3DTEXTUREFILTERTYPE type);
			void							SetMagFilter(UInt32 tex, D3DTEXTUREFILTERTYPE type);
			void							SetMipFilter(UInt32 tex, D3DTEXTUREFILTERTYPE type);
			void							SetMaxAnisotropy(UInt32 tex, UInt32 maxAniso);
			void							RetrieveFrontAndDepthBuffer();

			IDirect3D9 *					_pD3D;
			IDirect3DDevice9 *				_pD3DDevice;
			HWND							_hWindow;
			UInt32							_nAdapter;
			D3DCAPS9						_Caps;
			UInt32							_uiWindowWidth;
			UInt32							_uiWindowHeight;
			UInt32							_uiWindowPosX;
			UInt32							_uiWindowPosY;
			UInt32							_uiFullScreenWidth;
			UInt32							_uiFullScreenHeight;
			Bool							_bFullScreen;

			Int32							_iViewportX;
			Int32							_iViewportY;
			Int32							_iViewportWidth;
			Int32							_iViewportHeight;

			CULL_MODE						_cullMode;
			FILL_MODE						_fillMode;
			Bool							_bCreateDepthBuffer;
			BLEND_MODE						_eSrcBlend;
			BLEND_OP						_eBlendOp;
			BLEND_MODE						_eDestBlend;

			I_RENDER_TARGET *				_pCurrentRenderTarget[MAX_SIMULTANEOUS_RENDER_TARGET];
			CUBEMAP_FACE					_nCurrentRenderTargetCubeFace[MAX_SIMULTANEOUS_RENDER_TARGET];
			I_RENDER_TARGET *				_pCurrentDepthBuffer;
			I_RENDER_TARGET *				_pFrontBuffer;
			I_RENDER_TARGET *				_pDepthBuffer;

			I_TEXTURE *						_pTextures[MAX_TEXTURE_STAGES];
			D3DTEXTUREADDRESS				_TexAddressU[MAX_TEXTURE_STAGES];
			D3DTEXTUREADDRESS				_TexAddressV[MAX_TEXTURE_STAGES];
			D3DTEXTUREFILTERTYPE			_MinFilter[MAX_TEXTURE_STAGES];
			D3DTEXTUREFILTERTYPE			_MagFilter[MAX_TEXTURE_STAGES];
			D3DTEXTUREFILTERTYPE			_MipFilter[MAX_TEXTURE_STAGES];
			UInt32							_MaxAnisotropy[MAX_TEXTURE_STAGES];

			I_VERTEX_DECLARATION *			_pCurrentVertexDeclaration;
			I_VERTEX_BUFFER *				_apCurrentVertexBuffer[MAX_VERTEX_STREAMS];
			I_INDEX_BUFFER *				_pCurrentIndexBuffer;

			UInt32							_nNbrRenderModes;
			S_RENDER_MODE *					_pRenderModesArray;

			// Render states
			Bool							_bEnableDepthTest;
			Bool							_bEnableDepthWrite;
			Bool							_bEnableAlphaBlend;
			Bool							_bScissorTest;
			Bool							_bEnableAlphaTest;

			COMP_FUNC						_DepthTestFunc;
			COMP_FUNC						_AlphaTestFunc;
			UInt8							_nAlphaTestRef;

			RENDER_STATS					_RenderStats;
		};
	}
}

#endif // __KRENDERER_RENDERER_RENDER_CONTEXT_D3D9_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================