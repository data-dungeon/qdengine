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
//	CLASS:	MESH_GROUP
//
//	03-09-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	INC_KRNPHYSICS(Physics)

//-----------------------------------------------------------------------------
//	Name:		GetNbrGeometries
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	MESH_GROUP::GetNbrGeometries()
{
	return (_nNbrGeometries);
}

//-----------------------------------------------------------------------------
//	Name:		GetAABB
//	Object:		
//	03-09-05:	RMA - Created
//-----------------------------------------------------------------------------
const AABB &	MESH_GROUP::GetBoundingBox() const
{
	return (_BoundingBox);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrSharedVertexBuffers
//	Object:		
//	04-06-09:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	MESH_GROUP::GetNbrSharedVertexBuffers()
{
	return (_nNbrSharedVertexBuffers);
}
/*
//-----------------------------------------------------------------------------
//	Name:		SetSharedVertexBuffer
//	Object:		
//	04-06-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetSharedVertexBuffer(unsigned int i, GEOMETRY_ABC * pGeometry)
{
	_pSharedVertexBuffers[i] = pGeometry;
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometry
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetGeometry(unsigned int	i,
								GEOMETRY_ABC *	pGeometry)
{
	QDT_ASSERT(i < _nNbrGeometries);
	_pGeometries[i].SetGeometry(pGeometry);
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetMaterial
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetMaterial(unsigned int			i,
								const COM_INSTANCE &	ciMat)
{
	QDT_NOT_IMPLEMENTED();

//	QDT_ASSERT(i < _nNbrGeometries);
//	_pGeometries[i].SetMaterial(ciMat);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	05-02-16:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetBoundingBox(const AABB & Box)
{
	_BoundingBox = Box;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetBoundingBox(unsigned int	i,
								   const AABB &	Box)
{
	QDT_NOT_IMPLEMENTED();

//	QDT_ASSERT(i < _nNbrGeometries);
//	_pGeometries[i].SetBoundingBox(Box);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingSphere
//	Object:		
//	03-09-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetBoundingSphere(unsigned int		i,
									  const SPHERE &	Sphere)
{
	QDT_NOT_IMPLEMENTED();

//	QDT_ASSERT(i < _nNbrGeometries);
//	_pGeometries[i].SetBoundingSphere(Sphere);
}

//-----------------------------------------------------------------------------
//	Name:		SetDoubleSided
//	Object:		
//	03-09-03:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetDoubleSided(unsigned int	i,
								   bool			b)
{
	QDT_NOT_IMPLEMENTED();

//	QDT_ASSERT(i < _nNbrGeometries);
//	_pGeometries[i].SetDoubleSided(b);
}

//-----------------------------------------------------------------------------
//	Name:		SetCullingOnShadowing
//	Object:		
//	03-10-28:	FPR - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetCullingOnShadowing(unsigned int	i,
										  bool			b)
{
	QDT_NOT_IMPLEMENTED();

//	QDT_ASSERT(i < _nNbrGeometries);
//	_pGeometries[i].SetCullingOnShadowing(b);
}


//-----------------------------------------------------------------------------
//	Name:		GetAABBTree
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
/*const AABB_TREE &	MESH_GROUP::GetAABBTree() const
{
	return (_Tree);
}*/

//-----------------------------------------------------------------------------
//	Name:		GetAABBTree
//	Object:		
//	03-09-04:	RMA - Created
//-----------------------------------------------------------------------------
/*AABB_TREE &	MESH_GROUP::GetAABBTree()
{
	return (_Tree);
}*/
/*
//-----------------------------------------------------------------------------
//	Name:		GetGeometry
//	Object:		
//	03-10-27:	FPR - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	MESH_GROUP::GetGeometry(int	nNumGeometry) const
{
	return (_pGeometries[nNumGeometry].GetGeometry());
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetMeshGroupGeometry
//	Object:		
//	03-10-28:	SBE - Created
//-----------------------------------------------------------------------------
MESH_GROUP_GEOMETRY *	MESH_GROUP::GetMeshGroupGeometry(int	nNumGeometry) const
{	
	QDT_NOT_IMPLEMENTED();
	return	(NULL);

//	return (&_pGeometries[nNumGeometry]);
}

//-----------------------------------------------------------------------------
//	Name:		IsMeshGroupGeometryVisible
//	Object:		
//	03-10-28:	SBE - Created
//-----------------------------------------------------------------------------
bool	MESH_GROUP::IsMeshGroupGeometryVisible(int	nNumGeometry) const
{
	return (_pbVisibleGeometries[nNumGeometry]);
}

//-----------------------------------------------------------------------------
//	Name:		SetSharedVBIndex
//	Object:		
//	04-06-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetSharedVBIndex(unsigned int	nGeometry,
									 unsigned int	nIndex)
{
	QDT_NOT_IMPLEMENTED();

//	_pGeometries[nGeometry].SetSharedVBIndex(nIndex);
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetSharedVB
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	MESH_GROUP::GetSharedVB(unsigned int i) const
{
	QDT_ASSERT(_pSharedVertexBuffers);
	QDT_ASSERT(i < _nNbrSharedVertexBuffers);
	return (_pSharedVertexBuffers[i]);
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetStaticActor
//	Object:		
//	04-06-10:	RMA - Created
//-----------------------------------------------------------------------------
I_PHYSIC_STATIC_ACTOR *	MESH_GROUP::GetStaticActor() const
{
	return (_pStaticActor);
}

//-----------------------------------------------------------------------------
//	Name:		SetPhysicMaterial
//	Object:		
//	05-06-15:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH_GROUP::SetPhysicMaterial(I_PHYSIC_MATERIAL *	pMaterial)
{
	if	(_pMeshData)
	{
		_pMeshData->SetMaterial(pMaterial);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
