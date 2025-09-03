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
//	CLASS:	MESH_DATA
//
//	05-11-03:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetNbrVertexBuffers
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrVertexBuffers() const
{
	return	(_uiNbrVertexBuffers);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrVertices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrVertices(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_puiNbrVertices[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexSize
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetVertexSize(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_puiVertexSizes[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void *	MESH_DATA::GetVertices(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_pVertices[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrVertexElements
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrVertexElements(UInt32 nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_puiNbrVertexElement[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexElements
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
S_VERTEX_ELEMENT *	MESH_DATA::GetVertexElements(UInt32 nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_pVertexElement[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndexBuffers
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrIndexBuffers() const
{
	return	(_uiNbrIndexBuffers);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrIndices(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrIndexBuffers);
	return	(_puiNbrIndices[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetIndices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt16 *	MESH_DATA::GetIndices(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrIndexBuffers);
	return	(_pIndices[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPrimitives
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrPrimitives() const
{
	return	(_uiNbrPrimitives);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexDecl
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
I_VERTEX_DECLARATION *	MESH_DATA::GetVertexDecl(UInt32	nIndex)
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_pVertexDecl[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexBuffer
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
I_VERTEX_BUFFER *	MESH_DATA::GetVertexBuffer(UInt32	nIndex)
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	return	(_pVertexBuffer[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetIndexBuffer
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
I_INDEX_BUFFER *	MESH_DATA::GetIndexBuffer(UInt32	nIndex)
{
	QDT_ASSERT(nIndex < _uiNbrIndexBuffers);
	return	(_pIndexBuffer[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrVertexBuffers
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetNbrVertexBuffers(UInt32	uiNbrVertexBuffers)
{
	if	(uiNbrVertexBuffers)
	{
		_uiNbrVertexBuffers = uiNbrVertexBuffers;

		_puiNbrVertices = new UInt32[_uiNbrVertexBuffers];
		_puiVertexSizes = new UInt32[_uiNbrVertexBuffers];
		_pVertices = (void **)new UInt8*[_uiNbrVertexBuffers];
		
		_puiNbrVertexElement = new UInt32[_uiNbrVertexBuffers];
		_pVertexElement = new S_VERTEX_ELEMENT*[_uiNbrVertexBuffers];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetVertices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetVertices(UInt32	nIndex,
							   UInt32	uiNbrVertices,
							   UInt32	uiVertexSize,
							   void *	pVertices)
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	_puiNbrVertices[nIndex] = uiNbrVertices;
	_puiVertexSizes[nIndex] = uiVertexSize;
	_pVertices[nIndex] = pVertices;
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexElements
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetVertexElements(UInt32				nIndex,
									 UInt32				uiNbrVertexElements,
									 S_VERTEX_ELEMENT *	pVertexElements)
{
	QDT_ASSERT(nIndex < _uiNbrVertexBuffers);
	_puiNbrVertexElement[nIndex] = uiNbrVertexElements;
	_pVertexElement[nIndex] = pVertexElements;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrIndexBuffers
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetNbrIndexBuffers(UInt32	uiNbrIndexBuffers)
{
	if	(uiNbrIndexBuffers)
	{
		_uiNbrIndexBuffers = uiNbrIndexBuffers;

		_puiNbrIndices = new UInt32[_uiNbrIndexBuffers];
		_pIndices = new UInt16*[_uiNbrIndexBuffers];
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetIndices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_DATA::SetIndices(UInt32	nIndex,
							  UInt32	uiNbrIndices,
							  UInt16 *	pIndices)
{
	QDT_ASSERT(nIndex < _uiNbrIndexBuffers);
	_puiNbrIndices[nIndex] = uiNbrIndices;
	_pIndices[nIndex] = pIndices;
}

//-----------------------------------------------------------------------------
//	Name:		SetIndices
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
QDT_FLAGS MESH_DATA::GetDisplayFlags(UInt32 nIndex) const
{
	return (_pfDisplayFlags[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrFacesToSort
//	Object:		
//	06-03-16:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 MESH_DATA::GetNbrFacesToSort() const
{
	return (_uiNbrFacesToSort);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrFlowData
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
UInt32	MESH_DATA::GetNbrFlowData() const
{
	return (_iFlowDataNb);
}

//-----------------------------------------------------------------------------
//	Name:		GetFlowData
//	Object:		
//	06-03-29:	FBO - Created
//-----------------------------------------------------------------------------
FLOW_DATA *	MESH_DATA::GetFlowData(UInt32	iFlowDataIndex) const
{
	QDT_ASSERT(iFlowDataIndex < _iFlowDataNb);
	
	return _pFlowDataArray[iFlowDataIndex];
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
