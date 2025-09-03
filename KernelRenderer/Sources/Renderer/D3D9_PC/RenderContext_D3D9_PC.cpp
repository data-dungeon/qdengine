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
// CLASS: RENDER_CONTEXT_D3D9_PC
//
//
//	05-08-24:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"RenderContext_D3D9_PC.h"
#include	INCL_KMATH(Math\Geometry\Matrix\Matrix)
#include	INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Vector\Vector)
#include	INCL_KMATH(Math\Color\ColorFloat)
#include	INCL_KRENDERER(Renderer\D3D9_PC\RenderTarget_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\CubeRenderTarget_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\VertexBuffer_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\IndexBuffer_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\VertexDeclaration_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\CubeTexture_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\VolumeTexture_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\VertexShader_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\PixelShader_D3D9_PC)
#include	INCL_KRENDERER(Renderer\D3D9_PC\Texture_D3D9_PC)
#include	INCL_KRENDERER(Renderer\SRenderMode)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

static D3DCMPFUNC D3DCmpFonc[8] = 
{
	D3DCMP_NEVER,
	D3DCMP_LESS,
	D3DCMP_EQUAL,
	D3DCMP_LESSEQUAL,
	D3DCMP_GREATER,
	D3DCMP_NOTEQUAL,
	D3DCMP_GREATEREQUAL,
	D3DCMP_ALWAYS
};

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_D3D9_PC Constructor
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
RENDER_CONTEXT_D3D9_PC::RENDER_CONTEXT_D3D9_PC(HWND hWnd,
											   UInt32 uiWidth,
											   UInt32 uiHeight,
											   Bool bFullScreen,
											   Bool bCreateDepthBuffer)
:
_pD3D(NULL),
_pD3DDevice(NULL),
_hWindow(hWnd),
_uiWindowWidth(640),
_uiWindowHeight(480),
_uiWindowPosX(20),
_uiWindowPosY(20),
_uiFullScreenWidth(640),
_uiFullScreenHeight(480),
_bFullScreen(false),
_pCurrentDepthBuffer(NULL),
_pCurrentVertexDeclaration(NULL),
_pCurrentIndexBuffer(NULL),
_pFrontBuffer(NULL),
_pDepthBuffer(NULL),
_bScissorTest(false),
_bCreateDepthBuffer(bCreateDepthBuffer),
_nNbrRenderModes(0),
_pRenderModesArray(NULL),
_nAdapter(D3DADAPTER_DEFAULT)
{
	HRESULT hRes;

	for (UInt32 i = 0 ; i < MAX_SIMULTANEOUS_RENDER_TARGET ; ++i)
	{
		_pCurrentRenderTarget[i] = NULL;
		_nCurrentRenderTargetCubeFace[i] = CF_NONE;
	}

	// Create DIRECT3D object
	_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	QDT_ASSERT(_pD3D);

	DetectNVPerfHUD();

	CreateRenderModesArray();

	_uiWindowWidth = uiWidth;
	_uiWindowHeight = uiHeight;

	_bFullScreen = bFullScreen;

	if (IsResolutionSupported(uiWidth, uiHeight))
	{		
		_uiFullScreenWidth = uiWidth;
		_uiFullScreenHeight = uiHeight;
	}
	else
	{
		QDT_Message("Unsupported fullscreen resolution! switching to 640 * 480.");
		_uiFullScreenWidth = 640;
		_uiFullScreenHeight = 480;
	}

	// Initialize D3DPRESENT_PARAMETERS
	D3DPRESENT_PARAMETERS	PresentParameters;

	ZeroMemory(&PresentParameters, sizeof(PresentParameters));
	PresentParameters.Windowed = !_bFullScreen;
	PresentParameters.hDeviceWindow = hWnd;
	PresentParameters.BackBufferCount = 1;
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (_bFullScreen)
	{
		PresentParameters.BackBufferWidth = _uiFullScreenWidth;
		PresentParameters.BackBufferHeight = _uiFullScreenHeight;
	}
	else
	{
		PresentParameters.BackBufferWidth = _uiWindowWidth;
		PresentParameters.BackBufferHeight = _uiWindowHeight;
	}

	if (bCreateDepthBuffer)
	{
		PresentParameters.EnableAutoDepthStencil = true;
		PresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	else
	{
		PresentParameters.EnableAutoDepthStencil = false;
	}

	D3DDEVTYPE DevType = D3DDEVTYPE_HAL;

	UInt32 BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	if (_nAdapter != D3DADAPTER_DEFAULT)
	{
		// NvPerfHUD detected! Need to use ref device.
		DevType = D3DDEVTYPE_REF;
	}
	else
	{
		BehaviorFlags |= D3DCREATE_PUREDEVICE;
	}

	#if defined(_DEBUG_PIXEL_SHADERS)
		DevType = D3DDEVTYPE_REF;
	#endif

	

	#if defined(_DEBUG_VERTEX_SHADERS) || defined(_DEBUG_PIXEL_SHADERS)
		BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	#endif

	// Create device
	hRes = _pD3D->CreateDevice(_nAdapter,
							   DevType,
							   _hWindow,
                               BehaviorFlags,
                               &PresentParameters,
							   &_pD3DDevice);

	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(_pD3DDevice);

	hRes = _pD3DDevice->GetDeviceCaps(&_Caps);
	QDT_ASSERT(hRes == D3D_OK);

	UInt32 nVSMaxVersion = D3DSHADER_VERSION_MAJOR(_Caps.VertexShaderVersion);

	SetDefaultValues();

	RetrieveFrontAndDepthBuffer();
	
	_bEnableAlphaTest = true;
	EnableAlphaTest(false);
	_AlphaTestFunc = CF_LESS;
	SetAlphaTestFunc(CF_NEVER);
	_nAlphaTestRef = 0x80;
	SetAlphaTestRef(0);

	_bEnableDepthTest = false;
	EnableDepthTest(true);
	_DepthTestFunc = CF_LESS;
	SetAlphaTestFunc(CF_NEVER);
	_bEnableDepthWrite = false;
	EnableDepthWrite(true);	

    QDT_ENSURE(CheckResourceFormatSupport(_pD3DDevice, D3DFMT_D24S8, D3DRTYPE_TEXTURE, D3DUSAGE_DEPTHSTENCIL) == S_OK);
}

//-----------------------------------------------------------------------------
//	Name:		RetrieveFrontAndDepthBuffer
//	Object:		
//	06-01-19:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::RetrieveFrontAndDepthBuffer()
{
	RENDER_TARGET_D3D9_PC * pFrontBuffer = new RENDER_TARGET_D3D9_PC;
	if (_bFullScreen)
	{
		pFrontBuffer->_Texture._uiWidth = _uiFullScreenWidth;
		pFrontBuffer->_Texture._uiHeight = _uiFullScreenHeight;
	}
	else
	{
		pFrontBuffer->_Texture._uiWidth = _uiWindowWidth;
		pFrontBuffer->_Texture._uiHeight = _uiWindowHeight;
	}
	pFrontBuffer->_Texture._eFormat = PF_A8R8G8B8;
	_pD3DDevice->GetRenderTarget(0, &pFrontBuffer->_pD3DSurface);
	_pCurrentRenderTarget[0] = _pFrontBuffer = pFrontBuffer;

	RENDER_TARGET_D3D9_PC * pDepthBuffer = new RENDER_TARGET_D3D9_PC;
	if (_bFullScreen)
	{
		pDepthBuffer->_Texture._uiWidth = _uiFullScreenWidth;
		pDepthBuffer->_Texture._uiHeight = _uiFullScreenHeight;
	}
	else
	{
		pDepthBuffer->_Texture._uiWidth = _uiWindowWidth;
		pDepthBuffer->_Texture._uiHeight = _uiWindowHeight;
	}
	pDepthBuffer->_Texture._eFormat = PF_D24S8;
	_pD3DDevice->GetDepthStencilSurface(&pDepthBuffer->_pD3DSurface);
	_pCurrentDepthBuffer = _pDepthBuffer = pDepthBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		CheckResourceFormatSupport
//	Object:		
//	05-11-04:	SBE - Created
//-----------------------------------------------------------------------------
HRESULT	RENDER_CONTEXT_D3D9_PC::CheckResourceFormatSupport(IDirect3DDevice9 *	m_pd3dDevice,
														   D3DFORMAT			fmt,
														   D3DRESOURCETYPE		resType,
														   DWORD				dwUsage)
{
    HRESULT hr = S_OK;
    IDirect3D9* tempD3D = NULL;
    m_pd3dDevice->GetDirect3D(&tempD3D);
    D3DCAPS9 devCaps;
    m_pd3dDevice->GetDeviceCaps(&devCaps);
    
    D3DDISPLAYMODE displayMode;
    tempD3D->GetAdapterDisplayMode(devCaps.AdapterOrdinal, &displayMode);
    
    hr = tempD3D->CheckDeviceFormat(devCaps.AdapterOrdinal, devCaps.DeviceType, displayMode.Format, dwUsage, resType, fmt);
    
    tempD3D->Release(), tempD3D = NULL;
    
    return hr;
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_CONTEXT_D3D9_PC Destructor
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
RENDER_CONTEXT_D3D9_PC::~RENDER_CONTEXT_D3D9_PC()
{
	UInt32 refCount;

	delete _pFrontBuffer;
	delete _pDepthBuffer;

	refCount = _pD3DDevice->Release();
	QDT_ASSERT(refCount == 0);
	_pD3DDevice = NULL;

	refCount = _pD3D->Release();
	QDT_ASSERT(refCount == 0);
	_pD3D = NULL;

	delete [] _pRenderModesArray;
}

//-----------------------------------------------------------------------------
//	Name:		DetectNVPerfHUD
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::DetectNVPerfHUD()
{
	D3DADAPTER_IDENTIFIER9 Adaptater;
	UInt32 nCount = _pD3D->GetAdapterCount();
		
	for (UInt32 i = 0 ; i < nCount ; ++i)
	{
		_pD3D->GetAdapterIdentifier(i, 0, &Adaptater);

		if (strcmp(Adaptater.Description, "NVIDIA NVPerfHUD") == 0)
		{
			_nAdapter = i;
			QDT_Message("NVPerfHUD detected!");
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderModesArray
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::CreateRenderModesArray()
{
	D3DDISPLAYMODE DispMode;
	UInt32 i, j, k, nCount = _pD3D->GetAdapterModeCount(_nAdapter, D3DFMT_X8R8G8B8);
	
	// First loop to count the number of render modes available
	// Useful to clean redondant modes!
	for (i = 0 ; i < nCount ; ++i)
	{
		_pD3D->EnumAdapterModes(_nAdapter, D3DFMT_X8R8G8B8, i,  &DispMode);

		if (DispMode.Width >= 640 && DispMode.Height >= 480)
		{
			Bool bDoublon = false;
			for (j = 0 ; j < i ; ++j)
			{
				D3DDISPLAYMODE Doublon;
				_pD3D->EnumAdapterModes(_nAdapter, D3DFMT_X8R8G8B8, j,  &Doublon);
				if (Doublon.Width == DispMode.Width && Doublon.Height == DispMode.Height)
				{
					bDoublon = true;
					break;
				}
			}

			if (bDoublon == false)
			{
				_nNbrRenderModes++;
			}
		}
	}

	_pRenderModesArray = new S_RENDER_MODE[_nNbrRenderModes];

	// Second loop to fill the render modes available
	k = 0;
	for (i = 0 ; i < nCount ; ++i)
	{
		_pD3D->EnumAdapterModes(_nAdapter, D3DFMT_X8R8G8B8, i,  &DispMode);

		if (DispMode.Width >= 640 && DispMode.Height >= 480)
		{
			Bool bDoublon = false;
			for (j = 0 ; j < i ; ++j)
			{
				D3DDISPLAYMODE Doublon;
				_pD3D->EnumAdapterModes(_nAdapter, D3DFMT_X8R8G8B8, j,  &Doublon);
				if (Doublon.Width == DispMode.Width && Doublon.Height == DispMode.Height)
				{
					bDoublon = true;
					break;
				}
			}

			if (bDoublon == false)
			{
				_pRenderModesArray[k]._nWidth = DispMode.Width;
				_pRenderModesArray[k]._nHeight = DispMode.Height;
				k++;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsResolutionSupported
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------
bool RENDER_CONTEXT_D3D9_PC::IsResolutionSupported(UInt32 nWidth, 
												   UInt32 nHeight)
{	
	for (UInt32 i = 0 ; i < _nNbrRenderModes ; ++i)
	{
		if (_pRenderModesArray[i]._nWidth == nWidth && _pRenderModesArray[i]._nHeight == nHeight)
		{
			return (true);
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		SetDefaultValues
//	Object:		
//	05-09-29:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetDefaultValues()
{
	_pD3DDevice->SetRenderState(D3DRS_COLORVERTEX, true);
	_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	_bEnableAlphaBlend = false;
	_eSrcBlend = BM_ONE;
	_eDestBlend = BM_ZERO;
	_eBlendOp = BOP_ADD;

	_fillMode = FM_SOLID;

	_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_cullMode = CULL_NONE;

	for (UInt32 i = 0 ; i < MAX_TEXTURE_STAGES ; ++i)
	{
		_pTextures[i] = NULL;
		
		_pD3DDevice->SetTexture(i, NULL);
		_pD3DDevice->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		_pD3DDevice->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		_pD3DDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 1);

		_TexAddressU[i] = D3DTADDRESS_WRAP;
		_TexAddressV[i] = D3DTADDRESS_WRAP;
		_MinFilter[i] = D3DTEXF_LINEAR;
		_MagFilter[i] = D3DTEXF_LINEAR;
		_MipFilter[i] = D3DTEXF_LINEAR;
		_MaxAnisotropy[i] = 1;
	}

	for (UInt32 i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		_apCurrentVertexBuffer[i] = NULL;
		_pD3DDevice->SetStreamSource(i, NULL, 0, 0);
	}

	_pCurrentVertexDeclaration = NULL;
	//_pD3DDevice->SetVertexDeclaration(NULL); <= ELE : Show a D3D WARNING with DirectX debug

	_pCurrentIndexBuffer = NULL;
	_pD3DDevice->SetIndices(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		ChangeFullScreenSize
//	Object:		
//	06-01-25:	RMA - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_D3D9_PC::ChangeFullScreenSize(UInt32	uiWidth,
													 UInt32	uiHeight)
{
	if (IsResolutionSupported(uiWidth, uiHeight) == false)
	{
		return (false);
	}
	
	if (uiWidth != _uiFullScreenWidth || uiHeight != _uiFullScreenHeight)
	{
		QDT_ASSERT(_pD3DDevice);

		_uiFullScreenWidth = uiWidth;
		_uiFullScreenHeight = uiHeight;

		if (_bFullScreen)
		{
			Reset();
		}
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowSize
//	Object:		
//	05-11-02:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::ChangeWindowSize(UInt32	uiWidth,
												 UInt32	uiHeight)
{
	if	(_bFullScreen == false && (_uiWindowWidth != uiWidth || _uiWindowHeight != uiHeight))
	{
		_uiWindowWidth = uiWidth;
		_uiWindowHeight = uiHeight;
		Reset();
	}
	else
	{
		_uiWindowWidth = uiWidth;
		_uiWindowHeight = uiHeight;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ChangeWindowPos
//	Object:		
//	05-12-13:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::ChangeWindowPos(UInt32	uiX,
												UInt32	uiY)
{
	if	(_bFullScreen == false)
	{
		_uiWindowPosX = uiX;
		_uiWindowPosY = uiY;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::Reset()
{
	HRESULT hRes;

	// Initialize D3DPRESENT_PARAMETERS
	D3DPRESENT_PARAMETERS	PresentParameters;

	ZeroMemory(&PresentParameters, sizeof(PresentParameters));
	PresentParameters.Windowed   = !_bFullScreen;
	PresentParameters.hDeviceWindow = _hWindow;
	PresentParameters.BackBufferCount = 1;
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	PresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (_bFullScreen)
	{
		PresentParameters.BackBufferWidth = _uiFullScreenWidth;
		PresentParameters.BackBufferHeight = _uiFullScreenHeight;
	}
	else
	{
		PresentParameters.BackBufferWidth = _uiWindowWidth;
		PresentParameters.BackBufferHeight = _uiWindowHeight;
	}

	if (_bCreateDepthBuffer)
	{
		PresentParameters.EnableAutoDepthStencil = true;
		PresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	else
	{
		PresentParameters.EnableAutoDepthStencil = false;
	}

	delete _pFrontBuffer;
	delete _pDepthBuffer;

	RENDER_TARGET_D3D9_PC::ReleaseRenderTargets();
	CUBE_RENDER_TARGET_D3D9_PC::ReleaseRenderTargets();
	TEXTURE_D3D9_PC::ReleaseDynamicTextures();
	CUBE_TEXTURE_D3D9_PC::ReleaseDynamicTextures();

	hRes = _pD3DDevice->Reset(&PresentParameters);
	QDT_ASSERT(hRes == D3D_OK);

	SetDefaultValues();

	TEXTURE_D3D9_PC::RecreateDynamicTextures();
	CUBE_TEXTURE_D3D9_PC::RecreateDynamicTextures();
	RENDER_TARGET_D3D9_PC::RecreateRenderTargets();
	CUBE_RENDER_TARGET_D3D9_PC::RecreateRenderTargets();

	RetrieveFrontAndDepthBuffer();
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetFullScreen(bool bFullScreen)
{
	if (bFullScreen != _bFullScreen)
	{
		_bFullScreen = bFullScreen;

		Reset();

		// Switch back to original window!
		MoveWindow(_hWindow, _uiWindowPosX, _uiWindowPosY, _uiWindowWidth, _uiWindowHeight, true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BeginScene
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::BeginScene()
{
	HRESULT hRes;
	QDT_ASSERT(_pD3DDevice);			
	hRes = _pD3DDevice->BeginScene();
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		EndScene
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::EndScene()
{
	HRESULT hRes;
	QDT_ASSERT(_pD3DDevice);
	hRes = _pD3DDevice->EndScene();
	QDT_ASSERT(hRes == D3D_OK);	
}

//-----------------------------------------------------------------------------
//	Name:		Present
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::Present()
{
	HRESULT hRes;
	QDT_ASSERT(_pD3DDevice);
	hRes = _pD3DDevice->Present( NULL, NULL, NULL, NULL );
	if (hRes == D3DERR_DEVICELOST)
	{
		Reset();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DPrimitiveType
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
D3DPRIMITIVETYPE RENDER_CONTEXT_D3D9_PC::GetD3DPrimitiveType(PRIMITIVE_TYPE PrimitiveType) const
{
	static D3DPRIMITIVETYPE D3DPrimitiveTypeArray[6] = 
	{
		D3DPT_POINTLIST,
		D3DPT_LINELIST,
		D3DPT_LINESTRIP,
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_TRIANGLEFAN,
	};

	return (D3DPrimitiveTypeArray[PrimitiveType]);
}

//-----------------------------------------------------------------------------
//	Name:		DrawPrimitive
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::DrawPrimitive(PRIMITIVE_TYPE nType,  
										   UInt32 nPrimitiveCount, 
										   UInt32 nStartVertex)
{
	QDT_ASSERT(_pD3DDevice);

	UpdateSampler();

	D3DPRIMITIVETYPE d3dPT = GetD3DPrimitiveType(nType);

	HRESULT hRes;
	hRes = _pD3DDevice->DrawPrimitive(d3dPT, nStartVertex, nPrimitiveCount);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		DrawIndexedPrimitive
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::DrawIndexedPrimitive(PRIMITIVE_TYPE nType, 
									 			  UInt32 nMinIndex, 
									 			  UInt32 nNumVertices, 
												  UInt32 nStartIndex, 
												  UInt32 nPrimitiveCount)
{
	QDT_ASSERT(_pD3DDevice);

	UpdateSampler();

	D3DPRIMITIVETYPE d3dPT = GetD3DPrimitiveType(nType);

	HRESULT hRes;
	hRes = _pD3DDevice->DrawIndexedPrimitive(d3dPT, 0, nMinIndex, nNumVertices, nStartIndex, nPrimitiveCount);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		DrawPoints
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::DrawPoints(UInt32				uiNbrPoints,
										   const DEBUG_VERTEX *	pVertices,
										   const MATRIX &		ViewMatrix,
										   const MATRIX &		ProjectionMatrix)
{
	UInt32 i;
	MATRIX	Matrix = MATRIX::IDENTITY;
	_pD3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)Matrix.ToPointer());
	_pD3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)ViewMatrix.ToPointer());
	_pD3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)ProjectionMatrix.ToPointer());

	SetVertexDeclaration(NULL);
	SetVertexShader(NULL);
	SetPixelShader(NULL);
	SetCullMode(CULL_NONE);
	SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);

	HRESULT hRes;
	
	hRes = _pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	QDT_ASSERT(hRes == D3D_OK);

	hRes = _pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, uiNbrPoints, pVertices, sizeof(DEBUG_VERTEX));
	QDT_ASSERT(hRes == D3D_OK);

	for (i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		_apCurrentVertexBuffer[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DrawLines
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::DrawLines(UInt32				uiNbrLines,
										  const DEBUG_VERTEX *	pVertices,
										  const MATRIX &		ViewMatrix,
										  const MATRIX &		ProjectionMatrix)
{
	UInt32 i;
	MATRIX	Matrix = MATRIX::IDENTITY;
	_pD3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)Matrix.ToPointer());
	_pD3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)ViewMatrix.ToPointer());
	_pD3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)ProjectionMatrix.ToPointer());

	SetVertexDeclaration(NULL);
	SetVertexShader(NULL);
	SetPixelShader(NULL);
	SetCullMode(CULL_NONE);
	SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);

	HRESULT hRes;
	
	hRes = _pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	QDT_ASSERT(hRes == D3D_OK);

	hRes = _pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, uiNbrLines, pVertices, sizeof(DEBUG_VERTEX));
	QDT_ASSERT(hRes == D3D_OK);

	for (i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		_apCurrentVertexBuffer[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DrawTriangles
//	Object:		
//	05-12-06:	ELE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::DrawTriangles(UInt32				uiNbrTriangles,
											  const DEBUG_VERTEX *	pVertices,
											  const MATRIX &		ViewMatrix,
											  const MATRIX &		ProjectionMatrix)
{
	UInt32 i;
	MATRIX	Matrix = MATRIX::IDENTITY;
	_pD3DDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)Matrix.ToPointer());
	_pD3DDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)ViewMatrix.ToPointer());
	_pD3DDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)ProjectionMatrix.ToPointer());

	SetVertexDeclaration(NULL);
	SetVertexShader(NULL);
	SetPixelShader(NULL);
	SetCullMode(CULL_CLOCKWISE);
	SetBlendMode(BM_ONE, BOP_ADD, BM_ONE);
	EnableDepthWrite(false);

	HRESULT hRes;

	hRes = _pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	QDT_ASSERT(hRes == D3D_OK);

	hRes = _pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, uiNbrTriangles, pVertices, sizeof(DEBUG_VERTEX));
	QDT_ASSERT(hRes == D3D_OK);

	for (i = 0 ; i < MAX_VERTEX_STREAMS ; ++i)
	{
		_apCurrentVertexBuffer[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateAdditionalSwapChain
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_SWAP_CHAIN * RENDER_CONTEXT_D3D9_PC::CreateAdditionalSwapChain(HWND hWnd)
{
	// return (new CD3DSwapChain(this, hWnd));
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexBuffer
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_VERTEX_BUFFER * RENDER_CONTEXT_D3D9_PC::CreateVertexBuffer(UInt32 nVertexCount, 
															 UInt32 vertexStride,
															 Bool bDynamic)
{
	return (new VERTEX_BUFFER_D3D9_PC(this, nVertexCount, vertexStride, bDynamic));
}

//-----------------------------------------------------------------------------
//	Name:		CreateIndexBuffer
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_INDEX_BUFFER * RENDER_CONTEXT_D3D9_PC::CreateIndexBuffer(UInt32 nNbrIndices)
{
	return (new INDEX_BUFFER_D3D9_PC(this, nNbrIndices));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexDeclaration
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_VERTEX_DECLARATION * RENDER_CONTEXT_D3D9_PC::CreateVertexDeclaration(S_VERTEX_ELEMENT * pElements,
																	   UInt32 numElements)
{
	return (new VERTEX_DECLARATION_D3D9_PC(this, pElements, numElements));
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_TEXTURE *	RENDER_CONTEXT_D3D9_PC::CreateTexture(UInt32 uiWidth, 
												  UInt32 uiHeight, 
												  PIXEL_FORMAT eFormat, 
												  UInt32 uiMipMapLevelNum)
{
	return (new TEXTURE_D3D9_PC(this, uiWidth, uiHeight, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeTexture
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_D3D9_PC::CreateCubeTexture(UInt32 uiEdgeLength,
													  PIXEL_FORMAT eFormat, 
													  UInt32 uiMipMapLevelNum)
{
	return (new CUBE_TEXTURE_D3D9_PC(this, uiEdgeLength, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVolumeTexture
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
I_TEXTURE *	RENDER_CONTEXT_D3D9_PC::CreateVolumeTexture(UInt32 uiWidth, 
												  UInt32 uiHeight, 
												  UInt32 uiDepth,
												  PIXEL_FORMAT eFormat, 
												  UInt32 uiMipMapLevelNum)
{
	return (new VOLUME_TEXTURE_D3D9_PC(this, uiWidth, uiHeight, uiDepth, eFormat, uiMipMapLevelNum));
}

//-----------------------------------------------------------------------------
//	Name:		VCreateVertexShader
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_VERTEX_SHADER * RENDER_CONTEXT_D3D9_PC::CreateVertexShaderFromFile(const QDT_STRING & sFileName)
{
	return (new VERTEX_SHADER_D3D9_PC(this, sFileName));
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShaderFromFile
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_PIXEL_SHADER * RENDER_CONTEXT_D3D9_PC::CreatePixelShaderFromFile(const QDT_STRING & sFileName)
{
	return (new PIXEL_SHADER_D3D9_PC(this, sFileName));
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShader
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_VERTEX_SHADER * RENDER_CONTEXT_D3D9_PC::CreateVertexShader(UInt8 * pShader)
{
	return (new VERTEX_SHADER_D3D9_PC(this, pShader));
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShader
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
I_PIXEL_SHADER * RENDER_CONTEXT_D3D9_PC::CreatePixelShader(UInt8 * pShader)
{
	return (new PIXEL_SHADER_D3D9_PC(this, pShader));
}

//-----------------------------------------------------------------------------
//	Name:		CreateRenderTarget
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------	
I_RENDER_TARGET * RENDER_CONTEXT_D3D9_PC::CreateRenderTarget(UInt32			uiWidth, 
															 UInt32			uiHeight, 
															 PIXEL_FORMAT	eFormat)
{
	return (new RENDER_TARGET_D3D9_PC(this, uiWidth, uiHeight, eFormat));
}

//-----------------------------------------------------------------------------
//	Name:		CreateCubeRenderTarget
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET *	RENDER_CONTEXT_D3D9_PC::CreateCubeRenderTarget(UInt32		uiEdgeLength,
																   PIXEL_FORMAT	eFormat)
{
	return (new CUBE_RENDER_TARGET_D3D9_PC(this, uiEdgeLength, eFormat));
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShader
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetVertexShader(I_VERTEX_SHADER * pVertexShader)
{
	if (pVertexShader)
	{
		VERTEX_SHADER_D3D9_PC * pD3DShader = static_cast<VERTEX_SHADER_D3D9_PC*>(pVertexShader);
		Device()->SetVertexShader(pD3DShader->GetD3DVertexShader());
	}
	else
	{
		Device()->SetVertexShader(NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetVertexShaderConstant(UInt32 nRegister, 
													 const VECTOR & v)
{
	HRESULT hRes = Device()->SetVertexShaderConstantF(nRegister, v.ToPointer(), 1);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetVertexShaderConstant(UInt32 nRegister, 
													 const MATRIX & m)
{
	HRESULT hRes = Device()->SetVertexShaderConstantF(nRegister, m.ToPointer(), 4);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetVertexShaderConstant(UInt32 nRegister, 
													 const COLOR_FLOAT & c)
{
	Float32 array[4];

	array[0] = c.GetRed();
	array[1] = c.GetGreen();
	array[2] = c.GetBlue();
	array[3] = c.GetAlpha();

	HRESULT hRes = Device()->SetVertexShaderConstantF(nRegister, &array[0], 1);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetVertexShaderConstant(UInt32 nRegister, 
													 Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
	Float32 array[4];

	array[0] = f1;
	array[1] = f2;
	array[2] = f3;
	array[3] = f4;

	HRESULT hRes = Device()->SetVertexShaderConstantF(nRegister, &array[0], 1);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexShaderArrayConstant
//	Object:		
//	05-11-29:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetVertexShaderArrayConstant(UInt32			nRegister,
															 UInt32			uiNbrValues,
															 const float *	pValues)
{
	QDT_ASSERT(uiNbrValues % 4 == 0);
	UInt32	uiNbrVectors = uiNbrValues / 4;

	HRESULT hRes = Device()->SetVertexShaderConstantF(nRegister, pValues, uiNbrVectors);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderArrayConstant
//	Object:		
//	05-11-29:	PDE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetPixelShaderArrayConstant(UInt32			nRegister,
															 UInt32			uiNbrValues,
															 const float *	pValues)
{
	QDT_ASSERT(uiNbrValues % 4 == 0);

	UInt32	uiNbrVectors = uiNbrValues / 4;

	HRESULT hRes = Device()->SetPixelShaderConstantF(nRegister, pValues, uiNbrVectors);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShader
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetPixelShader(I_PIXEL_SHADER * pPixelShader)
{
	if (pPixelShader)
	{
		PIXEL_SHADER_D3D9_PC* pD3DShader = static_cast<PIXEL_SHADER_D3D9_PC*>(pPixelShader);
		Device()->SetPixelShader(pD3DShader->GetD3DPixelShader());
	}
	else
	{
		Device()->SetPixelShader(NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetPixelShaderConstant(UInt32 nRegister, 
													const VECTOR & v)
{
	const Float32 * p = v.ToPointer();
	HRESULT hRes = Device()->SetPixelShaderConstantF(nRegister, p, 1);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetPixelShaderConstant(UInt32 nRegister, 
													Float32 r)
{
	HRESULT hRes = Device()->SetPixelShaderConstantF(nRegister, &r, 1);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetPixelShaderConstant(UInt32 nRegister, 
													const MATRIX & m)
{
	Float32 array[16];

	array[0] = m(0, 0);
	array[1] = m(0, 1);
	array[2] = m(0, 2);
	array[3] = m(0, 3);
	array[4] = m(1, 0);
	array[5] = m(1, 1);
	array[6] = m(1, 2);
	array[7] = m(1, 3);
	array[8] = m(2, 0);
	array[9] = m(2, 1);
	array[10] = m(2, 2);
	array[11] = m(2, 3);
	array[12] = m(3, 0);
	array[13] = m(3, 1);
	array[14] = m(3, 2);
	array[15] = m(3, 3);

	HRESULT hRes = Device()->SetPixelShaderConstantF(nRegister, &array[0], 4);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetPixelShaderConstant(UInt32 nRegister, 
													const COLOR_FLOAT & c)
{
	Float32 array[4];

	array[0] = c.GetRed();
	array[1] = c.GetGreen();
	array[2] = c.GetBlue();
	array[3] = c.GetAlpha();

	HRESULT hRes = Device()->SetPixelShaderConstantF(nRegister, array, 1);
	QDT_ASSERT(hRes == D3D_OK);
}


//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderConstant
//	Object:		
//	26/01/06:	PDE - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetPixelShaderConstant(UInt32 nRegister, 
													 Float32 f1, Float32 f2, Float32 f3, Float32 f4)
{
	Float32 array[4];

	array[0] = f1;
	array[1] = f2;
	array[2] = f3;
	array[3] = f4;

	HRESULT hRes = Device()->SetPixelShaderConstantF(nRegister, &array[0], 1);
	QDT_ASSERT(hRes == D3D_OK);
}


//-----------------------------------------------------------------------------
//	Name:		SetPixelShaderTextureParameter
//	Object:		
//	05-09-26:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetPixelShaderTexture(UInt32		nTexture,
													  I_TEXTURE *	pTexture)
{
	SetTexture(nTexture, pTexture);
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetBlendMode(BLEND_MODE src, 
										  BLEND_OP op,
										  BLEND_MODE dest)
{
	static D3DBLEND D3DBlendMode[11] = 
	{
		D3DBLEND_ZERO,
		D3DBLEND_ONE,
		D3DBLEND_SRCCOLOR,
		D3DBLEND_INVSRCCOLOR,
		D3DBLEND_SRCALPHA,
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_DESTALPHA,
		D3DBLEND_INVDESTALPHA,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_INVDESTCOLOR,
		D3DBLEND_SRCALPHASAT,
	};

	static D3DBLENDOP D3DBlendOp[5] = 
	{
		D3DBLENDOP_ADD,
		D3DBLENDOP_SUBTRACT,
		D3DBLENDOP_REVSUBTRACT,
		D3DBLENDOP_MIN,
		D3DBLENDOP_MAX,
	};

	if (_eSrcBlend != src)
	{
		_eSrcBlend = src;
		_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBlendMode[src]);
	}
	
	if (_eBlendOp != op)
	{
		_eBlendOp = op;
		_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBlendOp[op]);
	}

	if (_eDestBlend != dest)
	{
		_eDestBlend = dest;
		_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBlendMode[dest]);
	}

	if (_eSrcBlend == BM_ONE && _eDestBlend == BM_ZERO && _eBlendOp == BOP_ADD)
	{
		if (_bEnableAlphaBlend == true)
		{
			_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			_bEnableAlphaBlend = false;
		}
	}
	else if (_bEnableAlphaBlend == false)
	{
		_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		_bEnableAlphaBlend = true;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::GetBlendMode(BLEND_MODE & src,
										  BLEND_OP & op,
										  BLEND_MODE & dest)
{
	src = _eSrcBlend;
	dest = _eDestBlend;
	op = _eBlendOp;
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendMode
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------	
void RENDER_CONTEXT_D3D9_PC::SetCullMode(CULL_MODE cullMode)
{
	static D3DCULL D3DCullMode[3] = 
	{
		D3DCULL_NONE,
		D3DCULL_CW,
		D3DCULL_CCW,	
	};

	if (_cullMode != cullMode)
	{
		_cullMode = cullMode;
		_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCullMode[_cullMode]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCullMode
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
CULL_MODE RENDER_CONTEXT_D3D9_PC::GetCullMode() const
{
	return (_cullMode);
}

//-----------------------------------------------------------------------------
//	Name:		SetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetFillMode(FILL_MODE	fillMode)
{
	static D3DFILLMODE D3DFillMode[2] = 
	{
		D3DFILL_WIREFRAME,
		D3DFILL_SOLID,	
	};

	if (_fillMode != fillMode)
	{
		_fillMode = fillMode;
		_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFillMode[_fillMode]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFillMode
//	Object:		
//	05-09-29:	SBE - Created
//-----------------------------------------------------------------------------
FILL_MODE	RENDER_CONTEXT_D3D9_PC::GetFillMode() const
{
	return _fillMode;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::Clear(UInt32 flags, 
								   UInt8 r,
								   UInt8 g, 
								   UInt8 b,
								   UInt8 a,
								   Float32 z)
{
	int d3dFlags = 0;

	if (flags & CF_CLEAR_COLOR)
	{
		d3dFlags |= D3DCLEAR_TARGET;
	}

	if (flags & CF_CLEAR_ZBUFFER)
	{
		d3dFlags |= D3DCLEAR_ZBUFFER;
	}

	QDT_ASSERT(_pD3DDevice);

	if (d3dFlags != 0)
	{
		HRESULT hr = _pD3DDevice->Clear(0, NULL, d3dFlags, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0L); 
		QDT_ASSERT(hr == D3D_OK);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetViewport
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetViewport(Int32 x, 
										 Int32 y, 
										 Int32 width, 
										 Int32 height)
{
	D3DVIEWPORT9 viewport;
	viewport.X = x;
	viewport.Y = y;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	QDT_ASSERT(_pD3DDevice);
	HRESULT hr = _pD3DDevice->SetViewport(&viewport);
	QDT_ASSERT(hr == D3D_OK);

	_iViewportX = x;
	_iViewportY = y;
	_iViewportWidth = width;
	_iViewportHeight = height;
}

//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	06-03-29:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::GetViewport(Int32 &	x,
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
//	Name:		IsFullScreen
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
bool RENDER_CONTEXT_D3D9_PC::IsFullScreen() const
{
	return _bFullScreen;
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenWidth
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 RENDER_CONTEXT_D3D9_PC::GetScreenWidth() const
{
	if (_bFullScreen)
	{
		return (_uiFullScreenWidth);
	}

	return (_uiWindowWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetScreenHeight
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 RENDER_CONTEXT_D3D9_PC::GetScreenHeight() const
{
	if (_bFullScreen)
	{
		return (_uiFullScreenHeight);
	}

	return (_uiWindowHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT_D3D9_PC::GetCurrentRenderTarget(UInt32 uiRenderTargetIndex) const
{
	QDT_ASSERT(uiRenderTargetIndex < MAX_SIMULTANEOUS_RENDER_TARGET);
	return (_pCurrentRenderTarget[uiRenderTargetIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentRenderTarget
//	Object:		
//	06-03-17:	RMA - Created
//-----------------------------------------------------------------------------
CUBEMAP_FACE RENDER_CONTEXT_D3D9_PC::GetCurrentRenderTargetCubeFace(UInt32 uiRenderTargetIndex) const
{
	QDT_ASSERT(uiRenderTargetIndex < MAX_SIMULTANEOUS_RENDER_TARGET);
	return (_nCurrentRenderTargetCubeFace[uiRenderTargetIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentDepthBuffer
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT_D3D9_PC::GetCurrentDepthBuffer() const
{
	return (_pCurrentDepthBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetFrontBuffer
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT_D3D9_PC::GetFrontBuffer() const
{
	return (_pFrontBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetDepthBuffer
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
I_RENDER_TARGET * RENDER_CONTEXT_D3D9_PC::GetDepthBuffer() const
{
	return (_pDepthBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		SupportMultipleRenderTargets
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_D3D9_PC::SupportMultipleRenderTargets()
{
	if (_Caps.NumSimultaneousRTs == 4)
	{
		if (_Caps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING)
		{
			return (true);
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		EnableColorWrite
//	Object:		
//	06-01-26:	RMA - Created
//-----------------------------------------------------------------------------

void	RENDER_CONTEXT_D3D9_PC::EnableColorWrite(Bool	bR,
												 Bool	bG,
												 Bool	bB,
												 Bool	bA,
												 UInt32	uiRenderTargetIndex)
{
	static UInt32 cachedStates[4] = {666, 666, 666, 666};

	QDT_ASSERT(uiRenderTargetIndex < MAX_SIMULTANEOUS_RENDER_TARGET);

	UInt32 uiFlag = 0;

	if (bR) uiFlag |= D3DCOLORWRITEENABLE_RED;
	if (bG) uiFlag |= D3DCOLORWRITEENABLE_GREEN;
	if (bB) uiFlag |= D3DCOLORWRITEENABLE_BLUE;
	if (bA) uiFlag |= D3DCOLORWRITEENABLE_ALPHA;
	
	if (cachedStates[uiRenderTargetIndex] != uiFlag)
	{
		switch (uiRenderTargetIndex)
		{
		default:
			QDT_FAIL();

		case 0:
			_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, uiFlag);
			break;

		case 1:
			_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE1, uiFlag);
			break;

		case 2:
			_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE2, uiFlag);
			break;

		case 3:
			_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE3, uiFlag);
			break;
		}

		cachedStates[uiRenderTargetIndex] = uiFlag;
	}
	
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentRenderTarget
//	Object:		
//	05-11-16:	SBE - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetCurrentRenderTarget(I_RENDER_TARGET * pSurface, 
													UInt32 uiRenderTargetIndex,
													CUBEMAP_FACE nCubeMapFace)
{
	QDT_ASSERT(uiRenderTargetIndex < MAX_SIMULTANEOUS_RENDER_TARGET);

	if ( (_pCurrentRenderTarget[uiRenderTargetIndex] != pSurface) || (_nCurrentRenderTargetCubeFace[uiRenderTargetIndex] != nCubeMapFace) )
	{
		if (pSurface)
		{
			_pD3DDevice->SetRenderTarget(uiRenderTargetIndex, (IDirect3DSurface9 *) pSurface->GetSurface(nCubeMapFace));
		}
		else
		{
			_pD3DDevice->SetRenderTarget(uiRenderTargetIndex, NULL);	
		}

		_pCurrentRenderTarget[uiRenderTargetIndex] = pSurface;
		_nCurrentRenderTargetCubeFace[uiRenderTargetIndex] = nCubeMapFace;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentRenderTarget
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetCurrentDepthBuffer(I_RENDER_TARGET * pSurface)
{
	//QDT_ASSERT(pSurface);

	if (_pCurrentDepthBuffer != pSurface)
	{
		if (pSurface)
		{
			_pD3DDevice->SetDepthStencilSurface((IDirect3DSurface9 *) pSurface->GetSurface());
		}
		else
		{
			_pD3DDevice->SetDepthStencilSurface(NULL);
		}
	}
		
	_pCurrentDepthBuffer = pSurface;
}

//-----------------------------------------------------------------------------
//	Name:		CopySurface
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::CopySurface(I_RENDER_TARGET * pSrc, 
										 I_RENDER_TARGET * pDst) 
{
/*	IDirect3DSurface9 * pSrcSurface = static_cast<IDirect3DSurface9 *>(pSrc->GetSurfaceData());
	IDirect3DSurface9 * pDstSurface = static_cast<IDirect3DSurface9 *>(pDst->GetSurfaceData());

	Device()->StretchRect(pSrcSurface, NULL, pDstSurface, NULL, D3DTEXF_NONE);

	if (pSrc->VIsTexture())
	{
		pSrcSurface->Release();
	}

	if (pDst->VIsTexture())
	{
		pDstSurface->Release();
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		CopySurface
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetTexture(UInt32		i, 
										I_TEXTURE *	pTexture)
{
	if (_pTextures[i] != pTexture)
	{
		HRESULT hRes;
		_pTextures[i] = pTexture;
		if (pTexture)
		{
			hRes = Device()->SetTexture(i, (IDirect3DBaseTexture9 *) pTexture->GetHandle());
		}
		else
		{
			hRes = Device()->SetTexture(i, NULL);
		}

		QDT_ASSERT(hRes == D3D_OK && "Fail to set texture");
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexDeclaration
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration)
{
	if (_pCurrentVertexDeclaration != pVertexDeclaration)
	{
		HRESULT hRes;
		if (pVertexDeclaration)
		{
			hRes = Device()->SetVertexDeclaration(static_cast<VERTEX_DECLARATION_D3D9_PC *>(pVertexDeclaration)->GetD3DVertexDeclaration());
		}
		else
		{
			hRes = Device()->SetVertexDeclaration(NULL);
		}

		QDT_ASSERT(hRes == D3D_OK && "Fail to set vertex declaration");
		_pCurrentVertexDeclaration = pVertexDeclaration;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthTest
//	Object:		
//	05-11-22:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::EnableDepthTest(Bool	bState)
{
	if (_bEnableDepthTest != bState)
	{
		_pD3DDevice->SetRenderState(D3DRS_ZENABLE, bState);
		_bEnableDepthTest = bState;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDepthTestFunc
//	Object:		
//	05-11-22:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetDepthTestFunc(COMP_FUNC	CF)
{
	if (CF != _DepthTestFunc)
	{
		_DepthTestFunc = CF;
		_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCmpFonc[CF]);	
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthWrite
//	Object:		
//	05-11-22:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::EnableDepthWrite(Bool	bState)
{
	if (_bEnableDepthWrite != bState)
	{
		_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, bState);
		_bEnableDepthWrite = bState;
	}	
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthBuffer
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::EnableScissorTest(bool bState)
{
	if (_bScissorTest == bState)
	{
		_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, bState);
		_bScissorTest = bState;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetScissorRect
//	Object:		
//	05-08-25:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetScissorRect(UInt32 x, 
											UInt32 y, 
											UInt32 width, 
											UInt32 height)
{
	RECT Rect;
	Rect.bottom = x;
	Rect.left	= y;
	Rect.right	= x + width;
	Rect.top	= y + height;
	_pD3DDevice->SetScissorRect(&Rect);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSampler
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::UpdateSampler()
{
	static D3DTEXTUREADDRESS D3DTextureAddress[2] = 
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_CLAMP,
	};

	for (UInt32 i = 0 ; i < MAX_TEXTURE_STAGES ; ++i)
	{
		// Cast to avoid virtual calls!
		TEXTURE_D3D9_PC * pTex = static_cast<TEXTURE_D3D9_PC*>(_pTextures[i]);

		if (pTex)
		{
			SetTexAddressU(i, D3DTextureAddress[pTex->GetAddressUType()]);
			SetTexAddressV(i, D3DTextureAddress[pTex->GetAddressVType()]);

			switch (pTex->GetFilterType())
			{
			default:
				QDT_ASSERT(0);

			case FT_NONE:
				SetMinFilter(i, D3DTEXF_POINT);
				SetMagFilter(i, D3DTEXF_POINT);
				SetMipFilter(i, D3DTEXF_NONE);
				break;

			case FT_BILINEAR:
				SetMinFilter(i, D3DTEXF_LINEAR);
				SetMagFilter(i, D3DTEXF_LINEAR);
				if (pTex->GetNbrMipMapLevels())	SetMipFilter(i, D3DTEXF_POINT);
				else							SetMipFilter(i, D3DTEXF_NONE);
				break;

			case FT_TRILINEAR:
				SetMinFilter(i, D3DTEXF_LINEAR);
				SetMagFilter(i, D3DTEXF_LINEAR);
				if (pTex->GetNbrMipMapLevels())	SetMipFilter(i, D3DTEXF_LINEAR);
				else							SetMipFilter(i, D3DTEXF_NONE);
				break;

			case FT_ANISOTROPIC_LEVEL_2:
				SetMinFilter(i, D3DTEXF_ANISOTROPIC);
				SetMagFilter(i, D3DTEXF_LINEAR);
				if (pTex->GetNbrMipMapLevels())	SetMipFilter(i, D3DTEXF_LINEAR);
				else							SetMipFilter(i, D3DTEXF_NONE);
				SetMaxAnisotropy(i, 2);
				break;

			case FT_ANISOTROPIC_LEVEL_4:
				SetMinFilter(i, D3DTEXF_ANISOTROPIC);
				SetMagFilter(i, D3DTEXF_LINEAR);
				if (pTex->GetNbrMipMapLevels())	SetMipFilter(i, D3DTEXF_LINEAR);
				else							SetMipFilter(i, D3DTEXF_NONE);
				SetMaxAnisotropy(i, 4);
				break;

			case FT_ANISOTROPIC_LEVEL_8:
				SetMinFilter(i, D3DTEXF_ANISOTROPIC);
				SetMagFilter(i, D3DTEXF_LINEAR);
				if (pTex->GetNbrMipMapLevels())	SetMipFilter(i, D3DTEXF_LINEAR);
				else							SetMipFilter(i, D3DTEXF_NONE);
				SetMaxAnisotropy(i, 8);
				break;

			case FT_ANISOTROPIC_LEVEL_16:
				SetMinFilter(i, D3DTEXF_ANISOTROPIC);
				SetMagFilter(i, D3DTEXF_LINEAR);
				if (pTex->GetNbrMipMapLevels())	SetMipFilter(i, D3DTEXF_LINEAR);
				else							SetMipFilter(i, D3DTEXF_NONE);
				SetMaxAnisotropy(i, 16);
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTexAddressU
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetTexAddressU(UInt32 tex,
											D3DTEXTUREADDRESS type)
{
	if (_TexAddressU[tex] != type)
	{
		_TexAddressU[tex] = type;
		_pD3DDevice->SetSamplerState(tex, D3DSAMP_ADDRESSU, type);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTexAddressV
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetTexAddressV(UInt32 tex,
											D3DTEXTUREADDRESS type)
{
	if (_TexAddressV[tex] != type)
	{
		_TexAddressV[tex] = type;
		_pD3DDevice->SetSamplerState(tex, D3DSAMP_ADDRESSV, type);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetMinFilter
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetMinFilter(UInt32 tex,
										  D3DTEXTUREFILTERTYPE type)
{
	if (_MinFilter[tex] != type)
	{
		_MinFilter[tex] = type;
		_pD3DDevice->SetSamplerState(tex, D3DSAMP_MINFILTER, type);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetMagFilter
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetMagFilter(UInt32 tex,
										  D3DTEXTUREFILTERTYPE type)
{
	if (_MagFilter[tex] != type)
	{
		_MagFilter[tex] = type;
		_pD3DDevice->SetSamplerState(tex, D3DSAMP_MAGFILTER, type);
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		SetMipFilter
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetMipFilter(UInt32 tex,
										  D3DTEXTUREFILTERTYPE type)
{
	if (_MipFilter[tex] != type)
	{
		_MipFilter[tex] = type;
		_pD3DDevice->SetSamplerState(tex, D3DSAMP_MIPFILTER, type);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxAnisotropy
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetMaxAnisotropy(UInt32 tex, UInt32 maxAniso)
{
	if (_MaxAnisotropy[tex] != maxAniso)
	{
		_MaxAnisotropy[tex] = maxAniso;
		_pD3DDevice->SetSamplerState(tex, D3DSAMP_MAXANISOTROPY, maxAniso);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetIndexBuffer(I_INDEX_BUFFER * pIndexBuffer)
{
	if (_pCurrentIndexBuffer != pIndexBuffer)
	{
		if (pIndexBuffer)
		{
			INDEX_BUFFER_D3D9_PC * pIndices = static_cast<INDEX_BUFFER_D3D9_PC *>(pIndexBuffer);
			_pD3DDevice->SetIndices(pIndices->GetD3DIndexBuffer());
		}
		else
		{
			_pD3DDevice->SetIndices(NULL);
		}

		_pCurrentIndexBuffer = pIndexBuffer;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT_D3D9_PC::SetVertexBuffer(UInt32 stream, 
											 I_VERTEX_BUFFER * pVertexBuffer)
{
	QDT_ASSERT(stream < MAX_VERTEX_STREAMS && "Error : you try to set a vertex buffer to an invalid stream!");

	if (_apCurrentVertexBuffer[stream] != pVertexBuffer)
	{
		if (pVertexBuffer)
		{
			_pD3DDevice->SetStreamSource(
				stream, 
				static_cast<VERTEX_BUFFER_D3D9_PC *>(pVertexBuffer)->GetD3DVertexBuffer(), 
				0, 
				static_cast<VERTEX_BUFFER_D3D9_PC *>(pVertexBuffer)->GetVertexStride());
		}
		else
		{
			_pD3DDevice->SetStreamSource(stream, NULL, 0, 0);
		}

		_apCurrentVertexBuffer[stream] = pVertexBuffer;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrRenderModes
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 RENDER_CONTEXT_D3D9_PC::GetNbrRenderModes()
{
	return (_nNbrRenderModes);
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderMode
//	Object:		
//	05-09-03:	RMA - Created
//-----------------------------------------------------------------------------
const S_RENDER_MODE & RENDER_CONTEXT_D3D9_PC::GetRenderMode(UInt32 i)
{
	return (_pRenderModesArray[i]);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DPixelFormat
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
D3DFORMAT	RENDER_CONTEXT_D3D9_PC::GetD3DPixelFormat(PIXEL_FORMAT	eFormat) const
{
	static D3DFORMAT D3DFormatsArray[13] = 
	{
		D3DFMT_A8R8G8B8,
		D3DFMT_X8R8G8B8,
		D3DFMT_A16B16G16R16F,
		D3DFMT_A32B32G32R32F,
		D3DFMT_R16F,
		D3DFMT_DXT1,
		D3DFMT_DXT3,
		D3DFMT_DXT5,
		D3DFMT_D24S8,
		D3DFMT_D16,
		D3DFMT_L8,
		D3DFMT_R32F,
		D3DFMT_D24S8
	};

	return D3DFormatsArray[eFormat];
}

//-----------------------------------------------------------------------------
//	Name:		EnableAlphaTest
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::EnableAlphaTest(Bool	bState)
{
	if (bState != _bEnableAlphaTest)
	{
		_bEnableAlphaTest = bState;		

		_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, _bEnableAlphaTest);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestFunc
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetAlphaTestFunc(COMP_FUNC	CF)
{
	if (CF != _AlphaTestFunc)
	{
		_AlphaTestFunc = CF;
		_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCmpFonc[CF]);	
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAlphaTestRef
//	Object:		
//	05-11-03:	SBE - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SetAlphaTestRef(UInt8	Ref)
{
	if (Ref != _nAlphaTestRef)
	{
		_nAlphaTestRef = Ref;
		_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, Ref);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderWindow
//	Object:		
//	05-11-10:	ELE - Created
//-----------------------------------------------------------------------------
HWND	RENDER_CONTEXT_D3D9_PC::GetRenderWindow()
{
	return	(_hWindow);
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosX
//	Object:		
//	05-12-13:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_D3D9_PC::GetWindowPosX()
{
	if (_bFullScreen)
	{
		return	(0);
	}
	else
	{
		return	(_uiWindowPosX);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosY
//	Object:		
//	05-12-13:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_D3D9_PC::GetWindowPosY()
{
	if (_bFullScreen)
	{
		return	(0);
	}
	else
	{
		return	(_uiWindowPosY);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsDisplayWindowActive
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_D3D9_PC::IsDisplayWindowActive() const
{
	if (_hWindow)
	{
		return	((GetWindowLong(_hWindow, GWL_USERDATA) != 0));
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		HasDisplayInputFocus
//	Object:		
//	05-10-25:	ELE - Created
//-----------------------------------------------------------------------------
bool	RENDER_CONTEXT_D3D9_PC::HasDisplayInputFocus() const
{
	return	(GetFocus() == _hWindow);
}

//-----------------------------------------------------------------------------
//	Name:		SupportShaderModel
//	Object:		
//	05-11-23:	RMA - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_D3D9_PC::SupportShaderModel(SHADER_MODEL sm)
{
	switch (sm)
	{
	default:
		QDT_FAIL();
		return (false);

	case SM_D3D_VS_1_1:
		return (_Caps.VertexShaderVersion >= D3DVS_VERSION(1, 1));

	case SM_D3D_VS_2_0:
		return (_Caps.VertexShaderVersion >= D3DVS_VERSION(2, 0));

	case SM_D3D_VS_3_0:
		return (_Caps.VertexShaderVersion >= D3DVS_VERSION(3, 0));

	case SM_D3D_PS_1_1:
		return (_Caps.PixelShaderVersion >= D3DPS_VERSION(1, 1));
	
	case SM_D3D_PS_1_2:
		return (_Caps.PixelShaderVersion >= D3DPS_VERSION(1, 2));

	case SM_D3D_PS_1_3:
		return (_Caps.PixelShaderVersion >= D3DPS_VERSION(1, 3));

	case SM_D3D_PS_1_4:
		return (_Caps.PixelShaderVersion >= D3DPS_VERSION(1, 4));

	case SM_D3D_PS_2_0:
		return (_Caps.PixelShaderVersion >= D3DPS_VERSION(2, 0));

	case SM_D3D_PS_3_0:
		return (_Caps.PixelShaderVersion >= D3DPS_VERSION(3, 0));
	}
}

//-----------------------------------------------------------------------------
//	Name:		SupportFloatingPointBlending
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_D3D9_PC::SupportFloatingPointBlending()
{
	HRESULT hRes = _pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3DRTYPE_SURFACE, D3DFMT_A16B16G16R16F);
	
	if (hRes == D3D_OK)
	{
		return (true);
	}

	QDT_ASSERT(hRes == D3DERR_NOTAVAILABLE);

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		SaveStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::SaveStates()
{
	// This method is only implemented in OpenGL for now
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		RestoreStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::RestoreStates()
{
	// This method is only implemented in OpenGL for now
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		CleanStates
//	Object:		
//	06-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::CleanStates()
{
	// This method is only implemented in OpenGL for now
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexShaderFromCgProgram
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
I_VERTEX_SHADER *	RENDER_CONTEXT_D3D9_PC::CreateVertexShaderFromCgProgram(void *	pCgProgram)
{
	// Only for maya openGL!
	QDT_FAIL();

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		CreatePixelShaderFromCgProgram
//	Object:		
//	06-02-07:	FBO - Created
//-----------------------------------------------------------------------------
I_PIXEL_SHADER *	RENDER_CONTEXT_D3D9_PC::CreatePixelShaderFromCgProgram(void *	pCgProgram)
{
	// Only for maya openGL!
	QDT_FAIL();

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		CreateTextureFromID
//	Object:		
//	06-02-09:	FBO - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_D3D9_PC::CreateTextureFromID(void *	pID, 
														UInt32 uiWidth, 
														UInt32 uiHeight)
{
	// Only for maya openGL!
	QDT_FAIL();
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		CreateTextureFromID
//	Object:		
//	06-02-09:	FBO - Created
//-----------------------------------------------------------------------------
I_TEXTURE *	RENDER_CONTEXT_D3D9_PC::CreateCubeTextureFromID(void *	pID, 
															UInt32 uiEdgeLength)
{
	// Only for maya openGL!
	QDT_FAIL();
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		ClearRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT_D3D9_PC::ClearRenderStats()
{
	_RenderStats.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		GetRenderStats
//	Object:		
//	06-03-23:	RMA - Created
//-----------------------------------------------------------------------------
RENDER_STATS &	RENDER_CONTEXT_D3D9_PC::GetRenderStats()
{
	return (_RenderStats);
}

//-----------------------------------------------------------------------------
//	Name:		GetPlatformIntColor
//	Object:		
//	06-03-23:	VMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT_D3D9_PC::GetPlatformIntColor(const COLOR &	Color)
{
	return (Color.GetARGBValue());
}

//-----------------------------------------------------------------------------
//	Name:		SupportHardwareShadowMap
//	Object:		
//	06-03-20:	SBE - Created
//-----------------------------------------------------------------------------
Bool	RENDER_CONTEXT_D3D9_PC::SupportHardwareShadowMap()
{
	// toclean...
	return true;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
