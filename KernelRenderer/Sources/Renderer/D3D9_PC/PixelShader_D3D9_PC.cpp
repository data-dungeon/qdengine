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
// CLASS: PIXEL_SHADER_D3D9_PC
//
//
//	05-09-27:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"PixelShader_D3D9_PC.h"
#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)
#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PIXEL_SHADER_D3D9_PC Constructor
//	Object:		
//	05-09-27:	RMA - Created
//-----------------------------------------------------------------------------	
PIXEL_SHADER_D3D9_PC::PIXEL_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pRenderContext, 
										   const QDT_STRING & sFileName)
:
_pRenderContext(pRenderContext),
_pD3DPixelShader(NULL)
{	
	// open file
	FILE *fp;
	QDT_STRING sExtendedFileName = sFileName + "_D3D_3_0";
	fp = fopen(sExtendedFileName.GetBuffer(), "rb");

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		UInt32 nSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		DWORD * pBuffer = new DWORD [nSize];

		fread(pBuffer, 4, nSize, fp);
			
		QDT_ASSERT(_pRenderContext);

		HRESULT hRes = _pRenderContext->Device()->CreatePixelShader(pBuffer, &_pD3DPixelShader);
		QDT_ASSERT(hRes == D3D_OK);
		
		fclose(fp);
	}
	else
	{
		QDT_Error("Can't find pixel shader %s on disk!", sExtendedFileName);
	}
}

//-----------------------------------------------------------------------------
//	Name:		PIXEL_SHADER_D3D9_PC Constructor
//	Object:		
//	05-09-27:	RMA - Created
//-----------------------------------------------------------------------------	
PIXEL_SHADER_D3D9_PC::PIXEL_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pRenderContext, 
										   UInt8 * pShader)
:
_pRenderContext(pRenderContext),
_pD3DPixelShader(NULL)
{
	QDT_ASSERT(_pRenderContext);

	HRESULT hRes = _pRenderContext->Device()->CreatePixelShader((DWORD*)pShader, &_pD3DPixelShader);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		PIXEL_SHADER_D3D9_PC Destructor
//	Object:		
//	05-09-27:	RMA - Created
//-----------------------------------------------------------------------------	
PIXEL_SHADER_D3D9_PC::~PIXEL_SHADER_D3D9_PC()
{
	if (_pD3DPixelShader)
	{
		UInt32 nRefCount = _pD3DPixelShader->Release();
		QDT_ASSERT(nRefCount == 0);
		_pD3DPixelShader = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DPixelShader
//	Object:		
//	05-09-27:	RMA - Created
//-----------------------------------------------------------------------------	
IDirect3DPixelShader9 * PIXEL_SHADER_D3D9_PC::GetD3DPixelShader()
{
	return (_pD3DPixelShader);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================