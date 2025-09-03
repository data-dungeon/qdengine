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
// CLASS: RENDER_CONTEXT
//
//
//	05-07-24:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"RenderContext.h"

#if defined (PLATFORM_PC)
	#include	INCL_KRENDERER(Renderer\D3D9_PC\RenderContext_D3D9_PC)
	#include	INCL_KRENDERER(Renderer\OGL_PC\RenderContext_OGL_PC)
#elif defined (PLATFORM_PS3)
	#include	INCL_KRENDERER(Renderer\PSGL\RenderContext_PSGL)
#elif defined (PLATFORM_X360)
	#include	INCL_KRENDERER(Renderer\D3D9_X360\RenderContext_D3D9_X360)
#else
	#error
#endif

#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#include	INCL_KCORE(Core/IOutput)

#include INCL_KRENDERER(Renderer\ISwapChain)
#include INCL_KRENDERER(Renderer\IVertexBuffer)
#include INCL_KRENDERER(Renderer\IIndexBuffer)
#include INCL_KRENDERER(Renderer\ITexture)
#include INCL_KRENDERER(Renderer\IVertexDeclaration)
#include INCL_KRENDERER(Renderer\IVertexShader)
#include INCL_KRENDERER(Renderer\IPixelShader)
#include INCL_KRENDERER(Renderer\IRenderTarget)

#ifdef _DEBUG
	#include "RenderContext.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

Float64 RENDER_CONTEXT::_fTimeSwap = 0;
Float64 RENDER_CONTEXT::_fTimeFrame = 0;
Float64 RENDER_CONTEXT::_fFPS = 0;

I_RENDER_CONTEXT_IMPLEMENTOR * RENDER_CONTEXT::_pRenderContext = NULL;

#ifdef PLATFORM_PS3
	RENDER_CONTEXT::RENDER_CONTEXT_TYPE RENDER_CONTEXT::_nRenderContextType = RCT_PSGL;
#else
	RENDER_CONTEXT::RENDER_CONTEXT_TYPE RENDER_CONTEXT::_nRenderContextType = RCT_DIRECT3D9; //RCT_UNDEFINED;
#endif

MUTEX* RENDER_CONTEXT::_pMutex = NULL;

//-----------------------------------------------------------------------------
//	Name:		Init Destructor
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void RENDER_CONTEXT::Init(HWND hWnd, 
						  UInt32 uiWidth,
						  UInt32 uiHeight,
						  Bool bFullScreen,
						  Bool bCreateDepthBuffer)
{
	QDT_ASSERT(_nRenderContextType != RCT_UNDEFINED);

	_pMutex = new MUTEX(true);

	switch (_nRenderContextType)
	{
	default:
		QDT_FAIL();

	case RCT_DIRECT3D9:
		_pRenderContext = new RENDER_CONTEXT_D3D9_PC(hWnd, uiWidth, uiHeight, bFullScreen, bCreateDepthBuffer);
		break;

	case RCT_OPENGL:
		_pRenderContext = new RENDER_CONTEXT_OGL_PC(hWnd, uiWidth, uiHeight, bFullScreen, bCreateDepthBuffer);
		break;

	case RCT_OPENGL_FOR_MAYA:
		_pRenderContext = new RENDER_CONTEXT_OGL_PC(hWnd, uiWidth, uiHeight);
		break;
	}

	QDT_ASSERT(_pRenderContext);
}
#elif defined (PLATFORM_PS3)
void RENDER_CONTEXT::Init(DISPLAY_MODE DisplayMode)

{
	_pMutex = new MUTEX(true);
	_pRenderContext = new RENDER_CONTEXT_PSGL(DisplayMode);
	QDT_ASSERT(_pRenderContext);
}
#elif defined (PLATFORM_X360)
void RENDER_CONTEXT::Init(DISPLAY_MODE DisplayMode)

{
	_pMutex = new MUTEX(true);
	_pRenderContext = new RENDER_CONTEXT_D3D9_X360(DisplayMode);
	QDT_ASSERT(_pRenderContext);
}
#else
	#error
#endif

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::Release()
{
	delete	_pMutex;
	delete	_pRenderContext;
	_pRenderContext = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseSwapChain
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
#if defined (PLATFORM_PC)
void RENDER_CONTEXT::ReleaseSwapChain(I_SWAP_CHAIN * pSwapChain)
{
	delete pSwapChain;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ReleaseVertexBuffer
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleaseVertexBuffer(I_VERTEX_BUFFER * pVertexBuffer)
{
	delete pVertexBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseIndexBuffer
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleaseIndexBuffer(I_INDEX_BUFFER * pIndexBuffer)
{
	delete pIndexBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseTexture
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleaseTexture(I_TEXTURE * pTexture)
{
	delete pTexture;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseVertexDeclaration
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleaseVertexDeclaration(I_VERTEX_DECLARATION * pVertexDeclaration)
{
	delete pVertexDeclaration;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseVertexShader
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleaseVertexShader(I_VERTEX_SHADER * pVertexShader)
{
	delete pVertexShader;
}

//-----------------------------------------------------------------------------
//	Name:		ReleasePixelShader
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleasePixelShader(I_PIXEL_SHADER * pPixelShader)
{
	delete pPixelShader;
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseRenderTarget
//	Object:		
//	05-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void RENDER_CONTEXT::ReleaseRenderTarget(I_RENDER_TARGET * pRenderTarget)
{
	delete pRenderTarget;
}

//-----------------------------------------------------------------------------
//	Name:		SizeCompressed
//	Object:		
//	05-09-19:	RMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT::SizeCompressed(UInt32		nWidth,
									   UInt32		nHeight,
									   PIXEL_FORMAT	eFormat)
{
	return (((nWidth+3)/4)*((nHeight+3)/4)*((eFormat == PF_DXTC1) ? (8) : (16)));   
}

//-----------------------------------------------------------------------------
//	Name:		PrintDebugMessage
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
#define BUFFER_SIZE_DEBUG_MESSAGE 4092
void	RENDER_CONTEXT::PrintDebugMessageLog(
										  const Char *	Text,
										  ...			)
{
#if defined (PLATFORM_PC)
	/// [MBi - 2006.03.08] - My, my, my, is this going to be painful!
	/// So, there is no debug output in game at the moment.
	/// I need to find another way to deal with that... 
	/// Oh, this is going to get hacky now... Har! Har! 

	char Buffer[BUFFER_SIZE_DEBUG_MESSAGE] ="";

	va_list	Args;

	va_start(Args, Text);
	_vsnprintf(Buffer, BUFFER_SIZE_DEBUG_MESSAGE, Text, Args);
	va_end(Args);

	I_OUTPUT::Push(Buffer);

	if (OPTIONAL_FUNC_CONTAINER::_pOutputFunc) 
	{
		OPTIONAL_FUNC_CONTAINER::_pOutputFunc(Buffer);
	}

#endif
}
void	RENDER_CONTEXT::PrintDebugMessage(Float32		fXPos,
										  Float32		fYPos,
										  const Char *	Text,
										  ...			)
{
#if defined (PLATFORM_PS3)
	AUTO_LOCK	AL(_pMutex);
	va_list	Args;
	
	va_start(Args, Text);
	_pRenderContext->PrintDebugMessageV(fXPos, fYPos, Text, Args);
	va_end(Args);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		PrintConsoleMessage
//	Object:		
//	06-02-17:	VMA - Created
//-----------------------------------------------------------------------------
void	RENDER_CONTEXT::PrintConsoleMessage(const Char *	Text,
											...				)
{
#if defined (PLATFORM_PS3)
	AUTO_LOCK	AL(_pMutex);
	va_list	Args;
	
	va_start(Args, Text);
	_pRenderContext->PrintConsoleMessageV(Text, Args);
	va_end(Args);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetPlatformIntColor
//	Object:		
//	06-03-23:	VMA - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_CONTEXT::GetPlatformIntColor(const COLOR &	Color)
{
	return (_pRenderContext->GetPlatformIntColor(Color));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
