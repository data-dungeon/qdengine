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
//	CLASS:	SCENE_GRRAPH
//
//	05-09-15:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetAmbientColor
//	Object:		Get the ambient color
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
const COLOR_FLOAT & SCENE_GRAPH::GetAmbientColor() const
{
	return (_AmbientColor);
}

//-----------------------------------------------------------------------------
//	Name:		SetAmbientColor
//	Object:		Set the ambient color
//	01-03-07:	RMA - Created
//-----------------------------------------------------------------------------
void SCENE_GRAPH::SetAmbientColor(const COLOR_FLOAT & Color)
{
	_AmbientColor = Color;
}

//-----------------------------------------------------------------------------
//	Name:		SCENE_GRAPH::SetBoundingSphere
//	Object:		Set the bounding sphere
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void SCENE_GRAPH::SetBoundingSphere(const SPHERE & Sphere)
{
	_BSphere = Sphere;
}

//-----------------------------------------------------------------------------
//	Name:		SCENE_GRAPH::GetBoundingSphere
//	Object: Get the bounding sphere
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
const SPHERE & SCENE_GRAPH::GetBoundingSphere()const
{
	return (_BSphere) ;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogEnable
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::SetFogEnable(bool	b)
{
	_bFogEnable = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogNear
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::SetFogNear(float	rval)
{
	_rFogNear = rval;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogFar
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::SetFogFar(float	rval)
{
	_rFogFar = rval;
}

//-----------------------------------------------------------------------------
//	Name:		SetFogColor
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::SetFogColor(const COLOR &	C)
{
	_FogColor = C;
}

//-----------------------------------------------------------------------------
//	Name:		IsFogEnable
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
bool	SCENE_GRAPH::IsFogEnable()const
{
	return ( _bFogEnable);
}

//-----------------------------------------------------------------------------
//	Name:		GetFogNear
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
float	SCENE_GRAPH::GetFogNear()const
{
	return ( _rFogNear );
}

//-----------------------------------------------------------------------------
//	Name:		GetFogFar
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
float	SCENE_GRAPH::GetFogFar()const
{
	return ( _rFogFar );
}

//-----------------------------------------------------------------------------
//	Name:		GetFogColor
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
const COLOR &	SCENE_GRAPH::GetFogColor()const
{
	return ( _FogColor );
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingBox
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
const AABB &	SCENE_GRAPH::GetBoundingBox()const
{
	return ( _BBox);
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	01-07-27:	cvi - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::SetBoundingBox(const AABB &	Box)
{
	_BBox = AABB(Box);
}

//-----------------------------------------------------------------------------
//	Name:		GetLights
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST<LIGHT_ABC *> &	SCENE_GRAPH::GetLights() const
{
	return	(_pCulling->GetLights());
}

//-----------------------------------------------------------------------------
//	Name:		GetVisibleLights
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST < LIGHT_ABC * > &	SCENE_GRAPH::GetVisibleLights() const
{
	return	(_pCulling->GetVisibleLights());
}

//-----------------------------------------------------------------------------
//	Name:		AddLight
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddLight(LIGHT_ABC * pLight)
{
	_pCulling->AddLight(pLight);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveLight
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveLight(LIGHT_ABC *	pLight)
{
	_pCulling->RemoveLight(pLight);
}

//-----------------------------------------------------------------------------
//	Name:		GetMeshNodes
//	Object:		
//	04-06-01:	FPR - Created
//-----------------------------------------------------------------------------
const QDT_DLIST<MESH *> &	SCENE_GRAPH::GetMeshes() const
{
	return	(_pCulling->GetMeshes());
}

//-----------------------------------------------------------------------------
//	Name:		AddMesh
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddMesh(MESH *	pMesh)
{
	_pCulling->AddMesh(pMesh);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveMesh
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveMesh(MESH *	pMesh)
{
	_pCulling->RemoveMesh(pMesh);
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayableDebugEntities
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST < ENTITY_ABC * > &	SCENE_GRAPH::GetDisplayableDebugEntities() const
{
	return _pCulling->GetDisplayableDebugEntities();
}

//-----------------------------------------------------------------------------
//	Name:		AddDisplayableDebugEntity
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddDisplayableDebugEntity(ENTITY_ABC *	pEntity)
{
	_pCulling->AddDisplayableDebugEntity(pEntity);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveDisplayableDebugEntity
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveDisplayableDebugEntity(ENTITY_ABC *	pEntity)
{
	_pCulling->RemoveDisplayableDebugEntity(pEntity);
}

//-----------------------------------------------------------------------------
//	Name:		GetPhysicScene
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
I_PHYSIC_SCENE * SCENE_GRAPH::GetPhysicScene() const
{
	return (_pPhysicScene);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeLights
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ComputeLights(const FRUSTRUM &				Frustum,
								   QDT_DLIST < LIGHT_ABC * > &	lLightsComputed)
{
	_pCulling->ComputeLights(Frustum, lLightsComputed);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeMeshes
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ComputeMeshes(const CONE &				Cone,
									 QDT_DLIST < MESH * > &	lMeshesComputed)
{
	_pCulling->ComputeMeshes(Cone, lMeshesComputed);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeMeshes
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ComputeMeshes(const SPHERE &			Sphere,
									 QDT_DLIST < MESH * > &	lMeshesComputed)
{
	_pCulling->ComputeMeshes(Sphere, lMeshesComputed);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeMeshes
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ComputeMeshes(const FRUSTRUM &			Frustum,
									 QDT_DLIST < MESH * > &	lMeshesComputed)
{
	_pCulling->ComputeMeshes(Frustum, lMeshesComputed);
}

//-----------------------------------------------------------------------------
//	Name:		AddAmbientOctree
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddAmbientOctree(AMBIENT_OCTREE *	pAmbientOctree)
{
	if (_lAmbientOctree.Find(_lAmbientOctree.GetHead(), pAmbientOctree) == _lAmbientOctree.GetTail())
	{
		_lAmbientOctree.PushTail(pAmbientOctree);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAmbientOctree
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveAmbientOctree(AMBIENT_OCTREE *	pAmbientOctree)
{
	_lAmbientOctree.Remove(pAmbientOctree);
}

//-----------------------------------------------------------------------------
//	Name:		ClearAmbientOctrees
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ClearAmbientOctrees()
{
	// TODO: Revoir le cleanage de cette liste
	_lAmbientOctree.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		ClearAmbientOctrees
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
PRIMITIVE_SET* SCENE_GRAPH::GetTemporaryPrimitive(void)
{
	return static_cast<PRIMITIVE_SET*>(_TemporaryPrimitive.GetCommunicator());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
