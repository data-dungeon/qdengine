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
//	CLASS:	ROUGH_FORCE
//
//	06-01-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Culling/RoughForce)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/LightABC)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ROUGH_FORCE constructor
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
ROUGH_FORCE::ROUGH_FORCE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ROUGH_FORCE destructor
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
ROUGH_FORCE::~ROUGH_FORCE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetLights
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST < LIGHT_ABC * > &	ROUGH_FORCE::GetLights() const
{
	return	(_lLights);
}

//-----------------------------------------------------------------------------
//	Name:		GetVisibleLights
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST < LIGHT_ABC * > &	ROUGH_FORCE::GetVisibleLights() const
{
	return	(_lVisibleLights);
}

//-----------------------------------------------------------------------------
//	Name:		AddLight
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::AddLight(LIGHT_ABC *	pLight)
{
	if (_lLights.Find(_lLights.GetHead(), pLight) == _lLights.GetTail())
	{
		_lLights.PushTail(pLight);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveLight
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::RemoveLight(LIGHT_ABC *	pLight)
{
	QDT_DLIST<LIGHT_ABC *>::ITERATOR	It = _lLights.Find(_lLights.GetHead(), pLight);
	if (It != _lLights.GetTail())
	{
		_lLights.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetMeshes
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST < MESH * > &	ROUGH_FORCE::GetMeshes() const
{
	return	(_lMeshes);
}

//-----------------------------------------------------------------------------
//	Name:		AddMesh
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::AddMesh(MESH *	pMesh)
{
	if (_lMeshes.Find(_lMeshes.GetHead(), pMesh) == _lMeshes.GetTail())
	{
		_lMeshes.PushTail(pMesh);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveMesh
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::RemoveMesh(MESH *	pMesh)
{
	QDT_DLIST<MESH *>::ITERATOR	It = _lMeshes.Find(_lMeshes.GetHead(), pMesh);
	if	(It != _lMeshes.GetTail())
	{
		_lMeshes.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDisplayableDebugEntities
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
const QDT_DLIST < ENTITY_ABC * > &	ROUGH_FORCE::GetDisplayableDebugEntities() const
{
	return	(_lDisplayableDebugEntities);
}

//-----------------------------------------------------------------------------
//	Name:		AddDisplayableDebugEntity
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::AddDisplayableDebugEntity(ENTITY_ABC *	pEntity)
{
	if (_lDisplayableDebugEntities.Find(_lDisplayableDebugEntities.GetHead(), pEntity) == _lDisplayableDebugEntities.GetTail())
	{
		_lDisplayableDebugEntities.PushTail(pEntity);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveDisplayableDebugEntity
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::RemoveDisplayableDebugEntity(ENTITY_ABC *	pEntity)
{
	QDT_DLIST<ENTITY_ABC *>::ITERATOR	It = _lDisplayableDebugEntities.Find(_lDisplayableDebugEntities.GetHead(), pEntity);
	if	(It != _lDisplayableDebugEntities.GetTail())
	{
		_lDisplayableDebugEntities.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeLights
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::ComputeLights(const FRUSTRUM &				Frustum,
								   QDT_DLIST < LIGHT_ABC * > &	lLightsComputed)
{
	QDT_DLIST<LIGHT_ABC *>::ITERATOR ItLight = _lLights.GetHead();
	QDT_DLIST<LIGHT_ABC *>::ITERATOR ItLightEnd = _lLights.GetTail();

	while (ItLight != ItLightEnd)
	{
		LIGHT_ABC *	pLight = *ItLight;

		pLight->UpdateGlobalPos();

		if	(pLight->TestVisibility(Frustum))
		{
			lLightsComputed.PushTail(pLight);
		}

		++ItLight;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeLightsVisible
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::ComputeLightsVisible(const FRUSTRUM & Frustum)
{
	_lVisibleLights.Clear();
	ComputeLights(Frustum, _lVisibleLights);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeMeshes
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::ComputeMeshes(const CONE & Cone,
								   QDT_DLIST <MESH *> &	lMeshesComputed)
{
	QDT_DLIST<MESH *>::ITERATOR ItMesh = _lMeshes.GetHead();
	QDT_DLIST<MESH *>::ITERATOR ItMeshEnd = _lMeshes.GetTail();

	// uniquement mesh pr le moment
	while (ItMesh != ItMeshEnd)
	{
		MESH * pMesh = (MESH *) (*ItMesh);
		NODE * pNode = pMesh->GetNode();

		SPHERE GlobalSphere = pNode->GetLocalBoundingSphere();
		GlobalSphere.Transform(pNode->GetGlobalMatrix());

		if (GlobalSphere.Intersect(Cone))
		{
			// todo Intersection OBB CONE ou  OBB OBB en construisant OBB a partir du CONE
//			OBB GlobalBBox = pNode->GetLocalBoundingBox();
//			GlobalBBox.Transform(pNode->GetGlobalMatrix());
//
//			if (GlobalBBox.Intersect(Cone))
			{
				lMeshesComputed.PushTail(pMesh);
			}
		}

		++ItMesh;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeMeshes
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::ComputeMeshes(const SPHERE & Sphere,
								   QDT_DLIST <MESH *> &	lMeshesComputed)
{
	QDT_DLIST<MESH *>::ITERATOR ItMesh = _lMeshes.GetHead();
	QDT_DLIST<MESH *>::ITERATOR ItMeshEnd = _lMeshes.GetTail();

	// uniquement mesh pr le moment
	while (ItMesh != ItMeshEnd)
	{
		MESH * pMesh = (MESH *) (*ItMesh);
		NODE * pNode = pMesh->GetNode();

		SPHERE GlobalSphere = pNode->GetLocalBoundingSphere();
		GlobalSphere.Transform(pNode->GetGlobalMatrix());

		if (GlobalSphere.Intersect(Sphere))
		{
//			OBB GlobalBBox = pNode->GetLocalBoundingBox();
//			GlobalBBox.Transform(pNode->GetGlobalMatrix());
//
//			if (GlobalBBox.Intersect(Sphere))
			{
				lMeshesComputed.PushTail(pMesh);
			}
		}

		++ItMesh;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeMeshes
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::ComputeMeshes(const FRUSTRUM & Frustum,
									 QDT_DLIST < MESH * > &	lMeshesComputed)
{
	QDT_DLIST<MESH *>::ITERATOR ItMesh = _lMeshes.GetHead();
	QDT_DLIST<MESH *>::ITERATOR ItMeshEnd = _lMeshes.GetTail();

	// uniquement mesh pr le moment
	while (ItMesh != ItMeshEnd)
	{
		MESH * pMesh = (MESH *) (*ItMesh);
		NODE * pNode = pMesh->GetNode();

		OBB GlobalBBox = pNode->GetLocalBoundingBox();
		GlobalBBox.Transform(pNode->GetGlobalMatrix());

		if (GlobalBBox.Intersect(Frustum.GetHexahedron()))
		{
			lMeshesComputed.PushTail(pMesh);
		}

		++ItMesh;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PrepareMeshesToDisplay
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::PrepareMeshesToDisplay(const FRUSTRUM & Frustum, 
											UInt32 uiDisplayFlags)
{
	QDT_DLIST<MESH *>::ITERATOR ItMesh = _lMeshes.GetHead();
	QDT_DLIST<MESH *>::ITERATOR ItMeshEnd = _lMeshes.GetTail();

	while (ItMesh != ItMeshEnd)
	{
		MESH * pMesh = (MESH *) (*ItMesh);
		pMesh->TestVisibility(Frustum, uiDisplayFlags);
		++ItMesh;
	}
}

//-----------------------------------------------------------------------------
//	Name:		PrepareMeshesToDisplay
//	Object:		
//	06-02-09:	SBE - Created
//-----------------------------------------------------------------------------
void	ROUGH_FORCE::PrepareMeshesToDisplay(const CONE & Cone, 
											UInt32 uiDisplayFlags)
{
	QDT_DLIST<MESH *>::ITERATOR ItMesh = _lMeshes.GetHead();
	QDT_DLIST<MESH *>::ITERATOR ItMeshEnd = _lMeshes.GetTail();

	while (ItMesh != ItMeshEnd)
	{
		MESH * pMesh = (MESH *) (*ItMesh);
		pMesh->TestVisibility(Cone, uiDisplayFlags);
		++ItMesh;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
