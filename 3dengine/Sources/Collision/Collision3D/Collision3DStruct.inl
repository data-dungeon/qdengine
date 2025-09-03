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
//	CLASS:	COLLISION_3D_STRUCT
//
//	05-04-29:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		COLLISION_3D_STRUCT constructor
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
COLLISION_3D_STRUCT::COLLISION_3D_STRUCT()
:
_NodeInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_OldPos		(VECTOR::ZERO),
_rRadius	(3.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COLLISION_3D_STRUCT destructor
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
COLLISION_3D_STRUCT::~COLLISION_3D_STRUCT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetNodeInst
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
void	COLLISION_3D_STRUCT::SetNodeInst(const COM_INSTANCE &	NodeInst)
{
	_NodeInst = NodeInst;

	NODE	*pNode = static_cast<NODE *>(_NodeInst.GetCommunicator());
	pNode->GetGlobalPosition(_OldPos);
}

//-----------------------------------------------------------------------------
//	Name:		SetOldPos
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
void	COLLISION_3D_STRUCT::SetOldPos(const VECTOR &	OldPos)
{
	_OldPos = OldPos;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingSphereRadius
//	Object:		
//	05-05-03:	ELE - Created
//-----------------------------------------------------------------------------
void	COLLISION_3D_STRUCT::SetBoundingSphereRadius(float	rRadius)
{
	_rRadius = rRadius;
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeInst
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	COLLISION_3D_STRUCT::GetNodeInst() const
{
	return	(_NodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetOldPos
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	COLLISION_3D_STRUCT::GetOldPos() const
{
	return	(_OldPos);
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	05-04-29:	ELE - Created
//-----------------------------------------------------------------------------
void	COLLISION_3D_STRUCT::Reset()
{
	_NodeInst = COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
	_OldPos = VECTOR::ZERO;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
