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
//	CLASS:	MESH_GROUP_HDR_REF
//
//	03-10-07:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_HDR_REF constructor
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP_HDR_REF::MESH_GROUP_HDR_REF()
:
_pNode(NULL),
_pMeshGroup(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_HDR_REF destructor
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP_HDR_REF::~MESH_GROUP_HDR_REF()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_HDR_REF constructor
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
void MESH_GROUP_HDR_REF::Clear()
{
	_vIndexes.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_HDR_REF constructor
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
void MESH_GROUP_HDR_REF::PushIndex(unsigned int nIndex)
{
	_vIndexes.PushTail(nIndex);
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_HDR_REF constructor
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int MESH_GROUP_HDR_REF::GetIndex(unsigned int i) const
{
	return (_vIndexes[i]);
}

//-----------------------------------------------------------------------------
//	Name:		MESH_GROUP_HDR_REF constructor
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int MESH_GROUP_HDR_REF::GetSize() const
{
	return (_vIndexes.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		SetNode
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
void MESH_GROUP_HDR_REF::SetNode(NODE * pN)
{
	_pNode = pN;
}

//-----------------------------------------------------------------------------
//	Name:		SetMeshGroup
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
void MESH_GROUP_HDR_REF::SetMeshGroup(MESH_GROUP *pG)
{
	_pMeshGroup = pG;
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
NODE * MESH_GROUP_HDR_REF::GetNode() const
{
	return (_pNode);
}

//-----------------------------------------------------------------------------
//	Name:		GetGroup
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
MESH_GROUP * MESH_GROUP_HDR_REF::GetMeshGroup() const
{
	return (_pMeshGroup);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	03-10-07:	RMA - Created
//-----------------------------------------------------------------------------
void MESH_GROUP_HDR_REF::Display()
{
	_pMeshGroup->DisplayHDR(*this);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
