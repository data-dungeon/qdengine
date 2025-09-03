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
// CLASS: PORTAL
//
//
//	01-02-09:	RMA - Created
//*****************************************************************************

#include INC_KRNCORE(Math/Geometry/BoundingVolume/OBB)

//-----------------------------------------------------------------------------
//	Name:		SetFlags
//	Object:		
//	01-07-18:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetFlags(int	nFlags)
{
	_nFlags = nFlags;
}

//-----------------------------------------------------------------------------
//	Name:	GetFlags
//	Object: Get a flag mask
//	01-04-11:	ELE - Created
//-----------------------------------------------------------------------------
int PORTAL_ABC::GetFlags(int nFlagsMask) const
{
	return (_nFlags & nFlagsMask) ;
}

//-----------------------------------------------------------------------------
//	Name:	SetFlags
//	Object: Set a flag mask
//	01-04-11:	ELE - Created
//-----------------------------------------------------------------------------
void PORTAL_ABC::SetFlags(int nFlagsMask, int nFlagsSet)
{
	_nFlags &= ~nFlagsMask ;
	_nFlags |=  nFlagsSet ;
}

//-----------------------------------------------------------------------------
//	Name:		SetOnlyDynamicsReflexions
//	Object:		
//	04-04-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetOnlyDynamicsReflexions(bool b)
{
	if	(b)
	{
		SetFlags(ONLY_DYNAMIC, ONLY_DYNAMIC);
	}
	else
	{
		SetFlags(ONLY_DYNAMIC, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsOnlyDynamicsReflexionsActivated
//	Object:		
//	04-04-07:	ELE - Created
//-----------------------------------------------------------------------------
bool	PORTAL_ABC::IsOnlyDynamicsReflexionsActivated() const
{
	return	(GetFlags(ONLY_DYNAMIC) > 0);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrGeometries
//	Object:		
//	01-07-18:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetNbrGeometries(int	nNbrGeometries)
{
	_nNbrGeometries = nNbrGeometries;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrGeometries
//	Object:		
//	03-02-19:	RMA - Created
//-----------------------------------------------------------------------------
int	PORTAL_ABC::GetNbrGeometries() const
{
	return (_nNbrGeometries);
}

//-----------------------------------------------------------------------------
//	Name:		GetGeometry
//	Object:		
//	03-02-19:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	PORTAL_ABC::GetGeometry(int i) const
{
	return (_pGeometriesArray[i]);
}

//-----------------------------------------------------------------------------
//	Name:		GetGeometries
//	Object:		
//	03-02-19:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC **	PORTAL_ABC::GetGeometries() const
{
	return (_pGeometriesArray);
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometries
//	Object:		
//	01-07-18:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetGeometries(GEOMETRY_ABC * *	pGeometries)
{
	_pGeometriesArray = pGeometries;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrVertices
//	Object:		
//	01-08-03:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetNbrVertices(int	nNbrVertices)
{
	_nNbrVertices = nNbrVertices;
}

//-----------------------------------------------------------------------------
//	Name:		SetSortedVertices
//	Object:		
//	01-08-03:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetSortedVertices(VECTOR *	pSortedVertices)
{
	_pSortedVerticesArray = pSortedVertices;
}

//-----------------------------------------------------------------------------
//	Name: PORTAL_ABC::GetFarClippingDistance
//	Object: Get the far clipping distance of the portal
//	01-03-30:	ELE - Created
//-----------------------------------------------------------------------------
float PORTAL_ABC::GetFarClippingDistance() const
{
	return (_rFarClippingDistance) ;
}

//-----------------------------------------------------------------------------
//	Name: PORTAL_ABC::SetFarClippingDistance
//	Object: Set the far clipping distance of the portal
//	01-03-30:	ELE - Created
//-----------------------------------------------------------------------------
void PORTAL_ABC::SetFarClippingDistance(float rValue)
{
	_rFarClippingDistance = rValue ;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrCrossingMax
//	Object:		Get the maximum of crossing threw the portal
//	01-05-30:	ELE - Created
//-----------------------------------------------------------------------------
int	PORTAL_ABC::GetNbrCrossingMax() const
{
	return	(_nNbrCrossingMax);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrCrossingMax
//	Object:		Set the maximum of crossing threw the portal
//	01-04-02:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetNbrCrossingMax(int nNbrCrossingMax) 
{
	_nNbrCrossingMax = nNbrCrossingMax;
}

//-----------------------------------------------------------------------------
//	Name:		GetPortalOut
//	Object:		Get the portal out
//	01-04-09:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_PORTAL_ZONE*	PORTAL_ABC::GetPortalZone()
{
	if	(_nNbrCrossing < _nNbrCrossingMax)
	{
		return	(&(_pPortalZones[_nNbrCrossing]));
	}
	else
	{
		return	(NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetTwinPortalNodeIndex
//	Object:		Get the twin portal node
//	01-03-30:	ELE - Created
//-----------------------------------------------------------------------------
COM_INSTANCE PORTAL_ABC::GetTwinPortalNodeInst() const
{
	return	(COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION));
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingVolume
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME *	PORTAL_ABC::GetBoundingBox() const
{
	return (_pBoundingBox);
}

//-----------------------------------------------------------------------------
//	Name:		GetOBR
//	Object:		
//	01-10-09:	ELE - Created
//-----------------------------------------------------------------------------
OBR *	PORTAL_ABC::GetOBR() const
{
	return	(_pOBR);
}

//-----------------------------------------------------------------------------
//	Name:		GetPlane
//	Object:		
//	01-08-09:	ELE - Created
//-----------------------------------------------------------------------------
const PLANE &	PORTAL_ABC::GetPlane() const
{
	return	(_Plane);
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	01-09-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::Reset()
{
	_nNbrCrossing = 0;
}

//-----------------------------------------------------------------------------
//	Name:		AddCrossing
//	Object:		
//	03-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::AddCrossing()
{
	_nNbrCrossing++;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	02-12-04:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetBoundingBox(const BOUNDING_VOLUME *	pBV)
{
	if (_pBoundingBox) delete _pBoundingBox;
	_pBoundingBox = BV_FACTORY::Instance()->Create(pBV);
}

//-----------------------------------------------------------------------------
//	Name:		GetPriority
//	Object:		
//	03-05-23:	RMA - Created
//-----------------------------------------------------------------------------
int	PORTAL_ABC::GetPriority() const
{
	return (_nPriority);
}

//-----------------------------------------------------------------------------
//	Name:		SetPriority
//	Object:		
//	03-05-23:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetPriority(int	n)
{
	_nPriority = n;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterials
//	Object:		
//	03-10-16:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetMaterials(COM_INSTANCE *	pciMaterials)
{
	_pGeometriesMaterialInst = pciMaterials;
}

//-----------------------------------------------------------------------------
//	Name:		SetAABBTree
//	Object:		
//	04-05-17:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetAABBTree(AABB_TREE *	pTree)
{
	_pAABBTree = pTree;
}

//-----------------------------------------------------------------------------
//	Name:		SetSharedVBIndices
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetSharedVBIndices(unsigned int *	pIndices)
{
	QDT_ASSERT(_pSharedVBIndex == NULL);
	_pSharedVBIndex = pIndices;
}

//-----------------------------------------------------------------------------
//	Name:		SetMeshGroup
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetMeshGroup(const COM_INSTANCE &	MeshGroup)
{
	_MeshGroup = MeshGroup;
}

//-----------------------------------------------------------------------------
//	Name:		SetSharedVBIndex
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
void	PORTAL_ABC::SetSharedVBIndex(int	nGeometry,
									 unsigned int	nIndex)
{
	QDT_ASSERT(_pSharedVBIndex);
	QDT_ASSERT(nGeometry < _nNbrGeometries);
	_pSharedVBIndex[nGeometry] = nIndex;
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================