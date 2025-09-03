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
// CLASS: VERTEX_DECLARATION_D3D9_PC
//
//
//	05-08-24:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"VertexDeclaration_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_D3D9_PC Constructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_DECLARATION_D3D9_PC::VERTEX_DECLARATION_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pContext, 
													   S_VERTEX_ELEMENT * pElements,
													   UInt32 numElements)
:
_pRenderContext(pContext),
_pDeclaration(NULL)
{
	QDT_ASSERT(_pRenderContext);
	QDT_ASSERT(numElements < MAX_NUM_ELEMENTS);

	_nNumElements = numElements;
	
	memcpy(&_aElements[0], pElements, _nNumElements * sizeof(S_VERTEX_ELEMENT));

	static D3DDECLTYPE D3DDeclTypeArray[7] = 
	{
		D3DDECLTYPE_FLOAT1,
		D3DDECLTYPE_FLOAT2,
		D3DDECLTYPE_FLOAT3,
		D3DDECLTYPE_FLOAT4,
		D3DDECLTYPE_D3DCOLOR,
		D3DDECLTYPE_FLOAT16_2,
		D3DDECLTYPE_FLOAT16_4,
	};

	D3DVERTEXELEMENT9 D3DElements[MAX_NUM_ELEMENTS+1];
	for (UInt32 i = 0 ; i < _nNumElements ; ++i)
	{
		D3DElements[i].Stream = pElements[i]._nStream;
		D3DElements[i].Offset = pElements[i]._nOffset;
		D3DElements[i].Type = D3DDeclTypeArray[pElements[i]._nType];
		D3DElements[i].Method = D3DDECLMETHOD_DEFAULT;

		switch (pElements[i]._nUsage)
		{
		default:
			QDT_FAIL();

		case DU_POSITION:
			D3DElements[i].Usage = D3DDECLUSAGE_POSITION;
			D3DElements[i].UsageIndex = 0;
			break;

		case DU_COLOR:
			D3DElements[i].Usage = D3DDECLUSAGE_COLOR;
			D3DElements[i].UsageIndex = 0;
			break;

		case DU_NORMAL:
			D3DElements[i].Usage = D3DDECLUSAGE_NORMAL;
			D3DElements[i].UsageIndex = 0;
			break;

		case DU_BLENDWEIGHT:
			D3DElements[i].Usage = D3DDECLUSAGE_BLENDWEIGHT;
			D3DElements[i].UsageIndex = 0;
			break;

		case DU_BLENDINDICES:
			D3DElements[i].Usage = D3DDECLUSAGE_BLENDINDICES;
			D3DElements[i].UsageIndex = 0;
			break;

		case DU_TEXCOORD0:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 0;
			break;

		case DU_TEXCOORD1:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 1;
			break;

		case DU_TEXCOORD2:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 2;
			break;

		case DU_TEXCOORD3:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 3;
			break;

		case DU_TEXCOORD4:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 4;
			break;

		case DU_TEXCOORD5:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 5;
			break;

		case DU_TEXCOORD6:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 6;
			break;

		case DU_TEXCOORD7:
			D3DElements[i].Usage = D3DDECLUSAGE_TEXCOORD;
			D3DElements[i].UsageIndex = 7;
			break;
		}	

		
	}

	D3DElements[i].Stream = 0xFF;
	D3DElements[i].Offset = 0;
	D3DElements[i].Type = D3DDECLTYPE_UNUSED;
	D3DElements[i].Method = 0;
	D3DElements[i].Usage = 0;
	D3DElements[i].UsageIndex = 0;

	HRESULT hRes = _pRenderContext->Device()->CreateVertexDeclaration(&D3DElements[0], &_pDeclaration);
	QDT_ASSERT(hRes == D3D_OK);
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_D3D9_PC Destructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_DECLARATION_D3D9_PC::~VERTEX_DECLARATION_D3D9_PC()
{
	if (_pDeclaration)
	{
		UInt32 nRefCount = _pDeclaration->Release();
		QDT_ASSERT(nRefCount == 0);
		_pDeclaration = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDeclaration
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void VERTEX_DECLARATION_D3D9_PC::GetDeclaration(S_VERTEX_ELEMENT* & pElements, 
												UInt32& nNumElements)
{
	pElements = &_aElements[0];
	nNumElements = _nNumElements;
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DVertexDeclaration
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
IDirect3DVertexDeclaration9 * VERTEX_DECLARATION_D3D9_PC::GetD3DVertexDeclaration() const 
{ 
	return _pDeclaration; 
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================