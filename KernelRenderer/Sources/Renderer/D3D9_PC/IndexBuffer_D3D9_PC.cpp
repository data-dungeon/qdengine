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
#include	"IndexBuffer_D3D9_PC.h"
#include	INCL_KRENDERER(Renderer/D3D9_PC/RenderContext_D3D9_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_D3D9_PC Constructor
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
INDEX_BUFFER_D3D9_PC::INDEX_BUFFER_D3D9_PC(RENDER_CONTEXT_D3D9_PC * pContext,
										   UInt32 nNbrIndices)
:
_pRenderContext(pContext),
_pD3DIndexBuffer(NULL),
_nNbrIndices(nNbrIndices),
_bLocked(false),
_bDynamic(false)
{
	QDT_ASSERT(_pRenderContext->Device());

	HRESULT hRes;
	hRes = _pRenderContext->Device()->CreateIndexBuffer(_nNbrIndices * 2, 
														D3DUSAGE_WRITEONLY, 
														D3DFMT_INDEX16, 
														D3DPOOL_MANAGED, 
														&_pD3DIndexBuffer, 
														NULL);
	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(_pD3DIndexBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		INDEX_BUFFER_D3D9_PC Destructor
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
INDEX_BUFFER_D3D9_PC::~INDEX_BUFFER_D3D9_PC()
{
	if (_pD3DIndexBuffer)
	{
		UInt32 nRefCount = _pD3DIndexBuffer->Release();
		QDT_ASSERT(nRefCount == 0);
		_pD3DIndexBuffer = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
void* INDEX_BUFFER_D3D9_PC::Lock()
{
	QDT_ASSERT(_pD3DIndexBuffer);
	QDT_ASSERT(_bLocked == false);

	HRESULT		hRes;
	void	   *pIndices;
	hRes = _pD3DIndexBuffer->Lock(0, _nNbrIndices * sizeof(UInt16), &pIndices, 0);
	QDT_ASSERT(hRes == D3D_OK);
	QDT_ASSERT(pIndices);

	_bLocked = true;

	return (pIndices);
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
void INDEX_BUFFER_D3D9_PC::Unlock()
{
	QDT_ASSERT(_pD3DIndexBuffer);
	QDT_ASSERT(_bLocked == true);

	HRESULT		hRes;
	hRes = _pD3DIndexBuffer->Unlock();
	QDT_ASSERT(hRes == D3D_OK);

	_bLocked = false;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndices
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 INDEX_BUFFER_D3D9_PC::GetNbrIndices() const
{
	return (_nNbrIndices);
}

//-----------------------------------------------------------------------------
//	Name:		IsDynamic
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
Bool INDEX_BUFFER_D3D9_PC::IsDynamic() const
{
	return (_bDynamic);
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
Bool INDEX_BUFFER_D3D9_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetD3DIndexBuffer
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------	
IDirect3DIndexBuffer9 * INDEX_BUFFER_D3D9_PC::GetD3DIndexBuffer() const
{
	return (_pD3DIndexBuffer);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================