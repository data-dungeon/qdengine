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
//	CLASS:	NODE_CHILD
//
//	06-01-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_TYPE	NODE_CHILD::GetType() const
{
	return	(_nType);
}

//-----------------------------------------------------------------------------
//	Name:		GetIndex
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
Int32	NODE_CHILD::GetIndex() const
{
	return	(_iIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetFatherIndex
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
Int32	NODE_CHILD::GetFatherIndex() const
{
	return	(_iFatherIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildIndex
//	Object:		
//	06-02-01:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	NODE_CHILD::GetChildIndex(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _uiNbrChildren);
	return	(_pChildren[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalMatrix
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
MATRIX &	NODE_CHILD::GetLocalMatrix()
{
	return	(_LMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetLocalMatrix
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE_CHILD::GetLocalMatrix() const
{
	return	(_LMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimMatrix
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
MATRIX &	NODE_CHILD::GetAnimMatrix()
{
	return	(_AnimMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetAnimMatrix
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE_CHILD::GetAnimMatrix() const
{
	return	(_AnimMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalMatrix
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE_CHILD::GetGlobalMatrix() const
{
	return	(_GMatrix);
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeArray
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
NODE_ARRAY *	NODE_CHILD::GetNodeArray() const
{
	return	(_pNodeArray);
}

//-----------------------------------------------------------------------------
//	Name:		GetDynamicActor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_DYNAMIC_ACTOR *	NODE_CHILD::GetDynamicActor()
{
	return	(_pDynamicActor);
}

//-----------------------------------------------------------------------------
//	Name:		GetPhysicMaterial
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_MATERIAL *	NODE_CHILD::GetPhysicMaterial()
{
	return	(_pPhysicMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		GetHairCell
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL *	NODE_CHILD::GetHairCell()
{
	return	(_pHairCell);
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetType(JOINT_TYPE	nType)
{
	_nType = nType;
}

//-----------------------------------------------------------------------------
//	Name:		SetLocalMatrix
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetLocalMatrix(const MATRIX &	LocalMatrix)
{
	_LMatrix = LocalMatrix;
}

//-----------------------------------------------------------------------------
//	Name:		SetIndex
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetIndex(Int32	iIndex)
{
	_iIndex = iIndex;
}

//-----------------------------------------------------------------------------
//	Name:		SetFatherIndex
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetFatherIndex(Int32	iIndex)
{
	_iFatherIndex = iIndex;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrChildren
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetNbrChildren(UInt32	nNbrChildren)
{
	_uiNbrChildren = nNbrChildren;
}

//-----------------------------------------------------------------------------
//	Name:		SetChildren
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetChildren(UInt32 *	pChildren)
{
	_pChildren = pChildren;
}

//-----------------------------------------------------------------------------
//	Name:		SetNodeArray
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetNodeArray(NODE_ARRAY *	pNodeArray)
{
	_pNodeArray = pNodeArray;
}

//-----------------------------------------------------------------------------
//	Name:		SetDynamicActor
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetDynamicActor(I_PHYSIC_DYNAMIC_ACTOR *	pActor)
{
	_pDynamicActor = pActor;
}

//-----------------------------------------------------------------------------
//	Name:		SetHairCell
//	Object:		
//	06-01-13:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE_CHILD::SetHairCell(HAIR_CELL *	pHairCell)
{
	_pHairCell = pHairCell;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
