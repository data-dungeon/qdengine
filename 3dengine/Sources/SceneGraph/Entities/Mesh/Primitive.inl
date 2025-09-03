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
//	CLASS:	PRIMITIVE
//
//	05-10-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	05-11-07:	ELE - Created
//-----------------------------------------------------------------------------
PRIMITIVE_TYPE	PRIMITIVE::GetType() const
{
	return	(_Type);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexBufferIdx
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetVertexBufferIdx() const
{
	return	(_nVertexBufferIdx);
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstVertex
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetFirstVertex() const
{
	return	(_nFirstVertex);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrVertices
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetNbrVertices() const
{
	return	(_nNbrVertices);
}

//-----------------------------------------------------------------------------
//	Name:		GetIndexBufferIdx
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetIndexBufferIdx() const
{
	return	(_nIndexBufferIdx);
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstIndex
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetFirstIndex() const
{
	return	(_nFirstIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndices
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetNbrIndices() const
{
	return	(_nNbrIndices);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrJoints
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetNbrJoints() const
{
	return	(_nNbrJoints);
}

//-----------------------------------------------------------------------------
//	Name:		GetJointIndex
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	PRIMITIVE::GetJointIndex(UInt32	nIndex) const
{
	return	(_pJointIndices[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetShader
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE & PRIMITIVE::GetShader() const
{
	return	(_ciShader);
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	05-11-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetType(PRIMITIVE_TYPE	Type)
{
	_Type = Type;
}

//-----------------------------------------------------------------------------
//	Name:		SetVertexBufferIdx
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetVertexBufferIdx(UInt32	nVertexBufferIdx)
{
	_nVertexBufferIdx = nVertexBufferIdx;
}

//-----------------------------------------------------------------------------
//	Name:		SetFirstVertex
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetFirstVertex(UInt32	nFirstVertex)
{
	_nFirstVertex = nFirstVertex;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrVertices
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetNbrVertices(UInt32	nNbrVertices)
{
	_nNbrVertices = nNbrVertices;
}

//-----------------------------------------------------------------------------
//	Name:		SetIndexBufferIdx
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetIndexBufferIdx(UInt32	nIndexBufferIdx)
{
	_nIndexBufferIdx = nIndexBufferIdx;
}

//-----------------------------------------------------------------------------
//	Name:		SetFirstIndex
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetFirstIndex(UInt32	nFirstIndex)
{
	_nFirstIndex = nFirstIndex;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrIndices
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetNbrIndices(UInt32	nNbrIndices)
{
	_nNbrIndices = nNbrIndices;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrJoints
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetNbrJoints(UInt32	nNbrJoints)
{
	_nNbrJoints = nNbrJoints;
}

//-----------------------------------------------------------------------------
//	Name:		SetJointIndices
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetJointIndices(UInt32 *	pJointIndices)
{
	_pJointIndices = pJointIndices;
}

//-----------------------------------------------------------------------------
//	Name:		SetShader
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetShader(const COM_INSTANCE & ciShader)
{
	_ciShader = ciShader;
}

//-----------------------------------------------------------------------------
//	Name:		GetPerPolygoneSorting
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
Bool	PRIMITIVE::GetPerPolygoneSorting() const
{
	return (_bPerPolygoneSorting);
}

//-----------------------------------------------------------------------------
//	Name:		SetPerPolygoneSorting
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	PRIMITIVE::SetPerPolygoneSorting(Bool	b)
{
	_bPerPolygoneSorting = b;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
