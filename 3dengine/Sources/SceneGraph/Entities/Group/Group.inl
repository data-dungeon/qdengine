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
// FILE: Group.inl
//
//
//	01-02-08:	RMA - Created
//*****************************************************************************

#include INC_KRNCORE(Math/Geometry/BoundingVolume/OBB)
#include INC_3DENGINE(SceneGraph/Entities/Mesh)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetFlags
//	Object: Get a flag mask
//	01-08-09:	ELE - Created
//-----------------------------------------------------------------------------
int GROUP::GetFlags(int nFlagsMask) const
{
	return (_nFlags & nFlagsMask) ;
}

//-----------------------------------------------------------------------------
//	Name:		SetFlags
//	Object: Set a flag mask
//	01-08-09:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::SetFlags(int nFlagsMask,
						int nFlagsSet)
{
	_nFlags &= ~nFlagsMask ;
	_nFlags |=  nFlagsSet ;
}

//-----------------------------------------------------------------------------
//	Name:		CastShadows
//	Object:		
//	03-03-21:	ELE - Created
//-----------------------------------------------------------------------------
bool	GROUP::CastShadows() const
{
	if	(_MeshInst.HasHandle())
	{
		return	(static_cast<MESH *>(_MeshInst.GetCommunicator())->GetFlags(MESH::CAST_SHADOWS) > 0);
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	03-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::SetBoundingBox(const BOUNDING_VOLUME *	pBB)
{
	if (_pBoundingBox) delete _pBoundingBox;
	_pBoundingBox = BV_FACTORY::Instance()->Create(pBB);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingSphere
//	Object:		
//	03-01-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::SetBoundingSphere(const BOUNDING_VOLUME *	pBS)
{
	if (_pBoundingSphere) delete _pBoundingSphere;
	_pBoundingSphere = BV_FACTORY::Instance()->Create(pBS);
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingBox
//	Object:		
//	03-01-16:	ELE - Created
//-----------------------------------------------------------------------------
const BOUNDING_VOLUME *	GROUP::GetBoundingBox() const
{
	return (_pBoundingBox);
}

//-----------------------------------------------------------------------------
//	Name:		SetGroupType
//	Object:		
//	03-01-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::SetGroupType(int	nType)
{
	_nGroupType = nType;
}

//-----------------------------------------------------------------------------
//	Name:		SetMeshInst
//	Object:		
//	03-01-27:	ELE - Created
//-----------------------------------------------------------------------------
void	GROUP::SetMeshInst(const COM_INSTANCE & MeshInst)
{
	_MeshInst = MeshInst;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================