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
// CLASS: VERTEX_BUFFER_D3D9_PC
//
//
//	05-08-31:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"VertexBuffer_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_D3D9_PC Constructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_BUFFER_D3D9_PC::VERTEX_BUFFER_D3D9_PC(RENDER_CONTEXT_D3D9_PC *	pRenderContext, 
											 UInt32						vertexCount, 
											 UInt32						vertexStride,
											 Bool						bDynamic)
:
_pRenderContext(pRenderContext),
_pD3DVertexBuffer(NULL),
_nVertexCount(vertexCount),
_nVertexStride(vertexStride),
_bLocked(false),
_bDynamic(bDynamic)
{
	QDT_ASSERT(_pRenderContext);
	CreateVertexBuffer();
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_D3D9_PC Destructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_BUFFER_D3D9_PC::~VERTEX_BUFFER_D3D9_PC()
{
	QDT_ASSERT(_bLocked == false);
	ReleaseVertexBuffer();
}

//-----------------------------------------------------------------------------
//	Name:		CreateVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void VERTEX_BUFFER_D3D9_PC::CreateVertexBuffer()
{
	HRESULT hRes;

	if (_bDynamic)
	{
		hRes = _pRenderContext->Device()->CreateVertexBuffer(
			_nVertexStride * _nVertexCount, 
			D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
			0,
			D3DPOOL_DEFAULT,
			&_pD3DVertexBuffer,
			NULL);
	}
	else
	{
		hRes = _pRenderContext->Device()->CreateVertexBuffer(
			_nVertexStride * _nVertexCount, 
			D3DUSAGE_WRITEONLY, 
			0,
			D3DPOOL_MANAGED,
			&_pD3DVertexBuffer,
			NULL);
	}
	
	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(_pD3DVertexBuffer);
}
	
//-----------------------------------------------------------------------------
//	Name:		ReleaseVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void VERTEX_BUFFER_D3D9_PC::ReleaseVertexBuffer()
{
	if (_pD3DVertexBuffer)
	{
		_pD3DVertexBuffer->Release();
		_pD3DVertexBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void* VERTEX_BUFFER_D3D9_PC::Lock()
{
	QDT_ASSERT(_pD3DVertexBuffer);
	QDT_ASSERT(_bLocked == false);

	HRESULT	hRes;
	void* pBuffer;
	if (_bDynamic)
	{
		hRes = _pD3DVertexBuffer->Lock(0, _nVertexCount * _nVertexStride, &pBuffer, D3DLOCK_DISCARD);
	}
	else
	{
		hRes = _pD3DVertexBuffer->Lock(0, _nVertexCount * _nVertexStride, &pBuffer, 0);
	}

	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(pBuffer);

	_bLocked = true;

	return pBuffer;
}
	
//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void VERTEX_BUFFER_D3D9_PC::Unlock()
{
	QDT_ASSERT(_pD3DVertexBuffer);
	QDT_ASSERT(_bLocked == true);

	HRESULT		hRes;
	hRes = _pD3DVertexBuffer->Unlock();
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = false;
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexCount
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 VERTEX_BUFFER_D3D9_PC::GetVertexCount() const
{
	return (_nVertexCount);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexStride
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 VERTEX_BUFFER_D3D9_PC::GetVertexStride() const
{
	return (_nVertexStride);
}

//-----------------------------------------------------------------------------
//	Name:		IsDynamic
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
Bool VERTEX_BUFFER_D3D9_PC::IsDynamic() const
{
	return (_bDynamic);
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
Bool VERTEX_BUFFER_D3D9_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DVertexBuffer
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
IDirect3DVertexBuffer9 * VERTEX_BUFFER_D3D9_PC::GetD3DVertexBuffer() const
{
	return (_pD3DVertexBuffer);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================