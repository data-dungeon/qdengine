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
// CLASS: INDEX_BUFFER_D3D9_PC
//
//
//	05-09-02:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"VertexShader_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

#include INCL_KCORE(Tools/QdtString)

#define QDT_INCLUDE_STDIO
#include INCL_KCORE(Includes/Includes)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_D3D9_PC Constructor
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_SHADER_D3D9_PC::VERTEX_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pRenderContext, 
											 const QDT_STRING & sFileName)
:
_pRenderContext(pRenderContext),
_pD3DVertexShader(NULL)
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
			
		HRESULT hRes = _pRenderContext->Device()->CreateVertexShader(pBuffer, &_pD3DVertexShader);
		QDT_ASSERT(hRes == D3D_OK);

		fclose(fp);
	}
	else
	{
		QDT_Error("Can't find vertex shader %s on disk!", sExtendedFileName);
	}
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_D3D9_PC Constructor
//	Object:		
//	05-11-16:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_SHADER_D3D9_PC::VERTEX_SHADER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pRenderContext, 
											 UInt8 * pShader)
:
_pRenderContext(pRenderContext),
_pD3DVertexShader(NULL)
{
	QDT_ASSERT(_pRenderContext);
			
	HRESULT hRes = _pRenderContext->Device()->CreateVertexShader((DWORD*)pShader, &_pD3DVertexShader);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_D3D9_PC Destructor
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_SHADER_D3D9_PC::~VERTEX_SHADER_D3D9_PC()
{
	if (_pD3DVertexShader)
	{
		UInt32 nRefCount = _pD3DVertexShader->Release();
		QDT_ASSERT(nRefCount == 0);
		_pD3DVertexShader = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_SHADER_D3D9_PC Constructor
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
IDirect3DVertexShader9 * VERTEX_SHADER_D3D9_PC::GetD3DVertexShader()
{
	return (_pD3DVertexShader);
}

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
