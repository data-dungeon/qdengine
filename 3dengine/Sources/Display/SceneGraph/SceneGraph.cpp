//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: SCENE_GRAPH
//
//
//	00-11-13	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( ROOT )
//
//	The ROOT class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Display/Culling/RoughForce)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/Trigger/Trigger)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/SpotLight)
#include	INCL_3DENGINE(SceneGraph/Entities/Light/PointLight)
#include	INCL_3DENGINE(SceneGraph/Entities/AmbientOctree/AmbientOctree)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(Display/SortedObject/SortedObject)
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(Profiler/Profiler)
#include	INCL_KCOM(Catalog/Catalog)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Ray/Ray)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KPHYSICS(Physics/IPhysicShapeSphere)
#include	INCL_KPHYSICS(Physics/IPhysicJointFixed)

#ifdef _DEBUG
	#include	"SceneGraph.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: SCENE_GRAPH constructor
//	Object: Default constructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
SCENE_GRAPH::SCENE_GRAPH()
:
_AmbientColor		(0.0f, 0.0f, 0.0f, 0.0f),
_bFogEnable			(false),
_rFogNear			(0.1f),
_rFogFar			(10000.0f),
_FogColor			(255, 127, 127, 127)
{
	_pCulling = QDT_NEW ROUGH_FORCE;

	_pPhysicScene = PHYSIC_CONTEXT::Instance()->CreateScene();

	_TemporaryPrimitive.SetHandle(PRIMITIVE_SET::GetGenericCommunicatorDescriptor()->Create());
	AddEntity(_TemporaryPrimitive);
}

//-----------------------------------------------------------------------------
//	Name: Destructor
//	Object: Destructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
SCENE_GRAPH::~SCENE_GRAPH()
{
	RemoveEntity(_TemporaryPrimitive);

	delete	(_pCulling);

	// Physic
	PHYSIC_MANAGER::Instance()->UnregisterNodeWithPhysics(_pPhysicScene);
	PHYSIC_CONTEXT::Instance()->ReleaseScene(_pPhysicScene);
}

//-----------------------------------------------------------------------------
//	Name:		AddEntity
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddEntity(const COM_INSTANCE &	EntityInst)
{
	if	(EntityInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(EntityInst.GetCommunicator());

		pEntity->SetSceneGraph(this);
		pEntity->Register();
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddEntity
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddEntity(const COM_INSTANCE &	EntityInst,
							   const COM_INSTANCE &	ParentNodeInst)
{
	if	(EntityInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(EntityInst.GetCommunicator());

		pEntity->SetSceneGraph(this);
		pEntity->Register();

		if	(ParentNodeInst.HasHandle())
		{
			NODE *	pParentNode = static_cast<NODE *>(ParentNodeInst.GetCommunicator());

			pParentNode->AddChild(pEntity->GetNode());
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddCatalog
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddCatalog(const COM_INSTANCE &	CatalogInst)
{
	if	(CatalogInst.HasHandle())
	{
		CATALOG * pCatalog = static_cast<CATALOG *>(CatalogInst.GetCommunicator());

		int	i, nNbrObjects;

		nNbrObjects = pCatalog->GetNbrCommunicator();
		for	(i = 0 ; i < nNbrObjects ; ++i)
		{
			int	nType = pCatalog->GetCommunicatorInstance(i).GetType();
			if	( (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH)        ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON)    ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT)  ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT)  ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT) ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT) ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_AMBIENT_OCTREE))
			{
				AddEntity(pCatalog->GetCommunicatorInstance(i));
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveEntity
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveEntity(const COM_INSTANCE &	EntityInst)
{
	if	(EntityInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(EntityInst.GetCommunicator());

		pEntity->Unregister();
		pEntity->SetSceneGraph(NULL);

		pEntity->Detach();
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveEntityKeepHierarchy
//	Object:		
//	06-03-20:	DAC - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveEntityKeepHierarchy(const COM_INSTANCE &	EntityInst)
{
	if	(EntityInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(EntityInst.GetCommunicator());

		pEntity->Unregister();
		pEntity->SetSceneGraph(NULL);

		pEntity->DetachKeepHierarchy();
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveCatalog
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveCatalog(const COM_INSTANCE &	CatalogInst)
{
	if	(CatalogInst.HasHandle())
	{
		CATALOG * pCatalog = static_cast<CATALOG *>(CatalogInst.GetCommunicator());

		int	i, nNbrObjects;

		nNbrObjects = pCatalog->GetNbrCommunicator();
		for	(i = 0 ; i < nNbrObjects ; ++i)
		{
			int	nType = pCatalog->GetCommunicatorInstance(i).GetType();
			if	( (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH)        ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON)    ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT)  ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT)  ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT) ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT) )
			{
				RemoveEntity(pCatalog->GetCommunicatorInstance(i));
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveCatalogKeepHierarchy
//	Object:		
//	06-03-20:	DAC - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveCatalogKeepHierarchy(const COM_INSTANCE &	CatalogInst)
{
	if	(CatalogInst.HasHandle())
	{
		CATALOG * pCatalog = static_cast<CATALOG *>(CatalogInst.GetCommunicator());

		int	i, nNbrObjects;

		nNbrObjects = pCatalog->GetNbrCommunicator();
		for	(i = 0 ; i < nNbrObjects ; ++i)
		{
			int	nType = pCatalog->GetCommunicatorInstance(i).GetType();
			if	( (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH)        ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON)    ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT)  ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT)  ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT) ||
				  (nType == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT) )
			{
				RemoveEntityKeepHierarchy(pCatalog->GetCommunicatorInstance(i));
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		InitParameters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::InitParameters(float rFogDec)
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	//	Set Ambient color
	_OldDynamicAmbientColor = pContext->GetGlobalAmbientLight();

	pContext->SetGlobalAmbientLight(_StaticAmbientColor);

	//	Fog
	_bOldFogEnable = pContext->IsFogEnabled();
	if (IsFogEnable() && DISPLAY_MANAGER::Instance()->IsFogEnabled())		
	{
		pContext->EnableFog(true);
	}
	else
	{
		pContext->EnableFog(false);
	}

	_rOldFogNear = pContext->GetFogStart();
	_rOldFogFar = pContext->GetFogEnd();
	_OldFogColor = pContext->GetFogColor();

	// J'ai retire le rFogDec, car de l'exterieur du diner j'ai mis
	// le même fog dans le diner pour eviter de voir super bien le diner
	pContext->SetFogStart(GetFogNear());// + rFogDec);
	pContext->SetFogEnd(GetFogFar());// + rFogDec);
	pContext->SetFogColor(GetFogColor());*/
}

//-----------------------------------------------------------------------------
//	Name:		ResetParameters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ResetParameters()
{
	QDT_NOT_IMPLEMENTED();
/*
	RENDER_CONTEXT	*pContext = RENDER_MANAGER::Instance()->GetRenderContext();

	//	Set Ambient color
	pContext->SetGlobalAmbientLight(_OldDynamicAmbientColor);

	//	Fog
	pContext->EnableFog(_bOldFogEnable);
	pContext->SetFogStart(_rOldFogNear);
	pContext->SetFogEnd(_rOldFogFar);
	pContext->SetFogColor(_OldFogColor);*/
}

//-----------------------------------------------------------------------------
//	Name:		AddShader
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddShader(SHADER_ABC *	pShader)
{
	_vShaders.PushTail(pShader);
}

//-----------------------------------------------------------------------------
//	Name:		AddTransparentObject
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddTransparentObject(const TRANSPARENT_OBJECT &	TransObj)
{
	_vTransparentObjects.PushTail(TransObj);
}

//-----------------------------------------------------------------------------
//	Name:		AddPrimitive
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddPrimitive(PRIMITIVE_SET *	pPrimitive)
{
	if (_lPrimitives.Find(_lPrimitives.GetHead(), pPrimitive) == _lPrimitives.GetTail())
	{
		_lPrimitives.PushTail(pPrimitive);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemovePrimitive
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemovePrimitive(PRIMITIVE_SET *	pPrimitive)
{
	QDT_DLIST <PRIMITIVE_SET *>::ITERATOR	It = _lPrimitives.Find(_lPrimitives.GetHead(), pPrimitive);

	if (It != _lPrimitives.GetTail())
	{
		_lPrimitives.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddTrigger
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::AddTrigger(TRIGGER *	pTrigger)
{
	if (_lTriggers.Find(_lTriggers.GetHead(), pTrigger) == _lTriggers.GetTail())
	{
		_lTriggers.PushTail(pTrigger);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveTrigger
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RemoveTrigger(TRIGGER *	pTrigger)
{
	QDT_DLIST <TRIGGER *>::ITERATOR	It = _lTriggers.Find(_lTriggers.GetHead(), pTrigger);

	if (It != _lTriggers.GetTail())
	{
		_lTriggers.Remove(It);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeVisibleLights
//	Object:		
//	06-02-22:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ComputeLightsVisible()
{
	const FRUSTRUM & Frustum = DISPLAY_MANAGER::Instance()->GetDisplayParameters().GetFrustrum();
	_pCulling->ComputeLightsVisible(Frustum);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeShadowMaps
//	Object:		
//	06-02-22:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::ComputeShadowMaps()
{
	// on recupere la liste de ttes les lights pouvant eclairer la vue de la camera
	// => intersection entre Frustum camera et bounding volume de la light
	NODE * pCameraNode = DISPLAY_MANAGER::Instance()->GetActiveCamera()->GetNode();

	// pr chq light du SG (visible et castant une ombre) on recupere la liste de ts les meshes eclaires par la light
	// todo a terme : ne parser que les objets dyn et avoir une liste preconstruite pr les meshes statiques ds light
	QDT_DLIST<LIGHT_ABC *>::ITERATOR ItLight = GetVisibleLights().GetHead();
	QDT_DLIST<LIGHT_ABC *>::ITERATOR ItLightEnd = GetVisibleLights().GetTail();
	while (ItLight != ItLightEnd)
	{
		LIGHT_ABC *	pLight = *ItLight;

		// todo? specialiser le pSG->ComputeLights pr recuperer uniquement des lights castant une ombre
		// => eviter de parser 90% des lighs ne castant pas d'ombre
		if (pLight->GetCastShadow()) 
		{
			switch (pLight->GetType())
			{
			default:
				QDT_FAIL(); // should never happened
				/*
				case LIGHT_DIRECTIONAL_SHADOW:
				{
				pSG->ComputeMeshes(CameraFrustum, pLight->GetLightedMeshes());
				}
				break;*/

			case LIGHT_POINT_SHADOW:					
				{
					SPHERE BoundingSphere = ((POINT_LIGHT *)pLight)->GetGlobalBoundingSphere();
					ComputeMeshes(BoundingSphere, pLight->GetLightedMeshes());
				}
				break;

			case LIGHT_SPOT_SHADOW:
			case LIGHT_SPOT_HARDWARE_SHADOW:
				{
					CONE BoundingCone = ((SPOT_LIGHT *)pLight)->GetGlobalBoundingCone();
					ComputeMeshes(BoundingCone, pLight->GetLightedMeshes());
				}
				break;
			}

			// alloc render target en fonction de distance light BV camera
			// rendu ds render target de la liste de mesh stockee ds la light
			pLight->UpdateDepthRenderTarget(pCameraNode->GetGlobalMatrix().GetTranslation(), this);

			pLight->GetLightedMeshes().Clear();
		}

		++ItLight;
	}

}

//-----------------------------------------------------------------------------
//	Name:		CleanShadowMaps
//	Object:		
//	06-02-22:	RMA - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::CleanShadowMaps()
{
	// Clean Shadow Render Targets associated to each light casting shadows
	QDT_DLIST<LIGHT_ABC *>::ITERATOR ItLight = GetVisibleLights().GetHead();
	QDT_DLIST<LIGHT_ABC *>::ITERATOR ItLightEnd = GetVisibleLights().GetTail();
	while (ItLight != ItLightEnd)
	{
		LIGHT_ABC *	pLight = *ItLight;

		if (pLight->GetCastShadow()) 
		{
			pLight->CleanShadowRenderTargets();
		}
		++ItLight;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-03-08:	RMA - Created : Special version used to display shadows
//-----------------------------------------------------------------------------
void   SCENE_GRAPH::Display(const CONE & Cone, 
							UInt32 uiDisplayFlags)
{
	_pCulling->PrepareMeshesToDisplay(Cone, uiDisplayFlags);

	UInt32	i, nNbrShaders = _vShaders.GetSize();

	for	(i = 0 ; i < nNbrShaders ; ++i)
	{
		QDT_VECTOR<SORTED_OBJECT> &	vObjectsToDisplay = _vShaders[i]->GetObjectsToDisplay();

		UInt32	j, nNbrObjects = vObjectsToDisplay.GetSize();
		for	(j = 0 ; j < nNbrObjects ; ++j)
		{
			vObjectsToDisplay[j].Display();
		}

		vObjectsToDisplay.Clear();
	}

	_vShaders.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	04-07-21:	RMA - Created
//  05-04-27:   RMA - Modified : For optimization purpose, filtering is set
//				to bilinear for filters
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::Display()
{
	BEGIN_PROFILE(PROFILE_SCENEGRAPH_DISPLAY);

	ComputeLightsVisible();

	if ( (DISPLAY_MANAGER::Instance()->IsDynamicShadowsEnabled()) &&
		 (DISPLAY_MANAGER::Instance()->DISPLAY_MANAGER::Instance()->GetCurrentViewport()->AreShadowsEnabled()) )
	{
		ComputeShadowMaps();
	}

	_pCulling->PrepareMeshesToDisplay(DISPLAY_MANAGER::Instance()->GetDisplayParameters().GetFrustrum(), MESH::DISPLAY_FLAG_VISIBLE);

	UInt32	i, nNbrShaders = _vShaders.GetSize();

	for	(i = 0 ; i < nNbrShaders ; ++i)
	{
		QDT_VECTOR<SORTED_OBJECT> &	vObjectsToDisplay = _vShaders[i]->GetObjectsToDisplay();

		UInt32	j, nNbrObjects = vObjectsToDisplay.GetSize();
		for	(j = 0 ; j < nNbrObjects ; ++j)
		{
			vObjectsToDisplay[j].Display();
		}

		vObjectsToDisplay.Clear();
	}

	_vShaders.Clear();

	// Transparency
	_vTransparentObjects.Sort();

	UInt32	nNbrTransparentObjects = _vTransparentObjects.GetSize();

	for	(i = 0 ; i < nNbrTransparentObjects ; ++i)
	{
		_vTransparentObjects[i].Display();
	}

	_vTransparentObjects.Clear();

	// Debug entities
/*
	if (DISPLAY_MANAGER::Instance()->GetDisplayLights() == true)
	{
		unsigned int i, nCount;

		nCount = _vLights.GetSize();
		for (i = 0 ; i < nCount ; ++i)
		{
			_vLights[i]->Display();
		}
	}
*/
	if (DISPLAY_MANAGER::Instance()->GetDisplayPrimitives() == true)
	{
		QDT_DLIST<PRIMITIVE_SET *>::ITERATOR ItPrimitive = _lPrimitives.GetHead();
		QDT_DLIST<PRIMITIVE_SET *>::ITERATOR ItPrimitiveEnd = _lPrimitives.GetTail();

		while (ItPrimitive != ItPrimitiveEnd)
		{
			PRIMITIVE_SET* pPrim=*ItPrimitive;
			pPrim->Display();
			ItPrimitive++;
		}
	}
	//clear temporary primitive
	GetTemporaryPrimitive()->Clear();

	if (DISPLAY_MANAGER::Instance()->GetDisplayTriggerVolumes() == true)
	{
		QDT_DLIST<TRIGGER *>::ITERATOR ItTrigger = _lTriggers.GetHead();
		QDT_DLIST<TRIGGER *>::ITERATOR ItTriggerEnd = _lTriggers.GetTail();

		while (ItTrigger != ItTriggerEnd)
		{
			(*ItTrigger)->Display();
			ItTrigger++;
		}
	}

	if ( (DISPLAY_MANAGER::Instance()->IsDynamicShadowsEnabled()) &&
		 (DISPLAY_MANAGER::Instance()->DISPLAY_MANAGER::Instance()->GetCurrentViewport()->AreShadowsEnabled()) )
	{
		CleanShadowMaps();
	}

	END_PROFILE(PROFILE_SCENEGRAPH_DISPLAY);
}

//-----------------------------------------------------------------------------
//	Name:		RayCastAllEntities
//	Object:		
//	06-01-17:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::RayCastAllEntities(RAY_CAST_PARAMETERS &	RayCastParameters)
{
	// Only meshes for now, think to add other entity type
	const QDT_DLIST<MESH *> &	lMeshes = _pCulling->GetMeshes();
	QDT_DLIST<MESH *>::ITERATOR ItMesh = lMeshes.GetHead();
	QDT_DLIST<MESH *>::ITERATOR ItMeshEnd = lMeshes.GetTail();

	while (ItMesh != ItMeshEnd)
	{
		(*ItMesh)->RayCast(RayCastParameters);
		ItMesh++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayPhysics
//	Object:		
//	05-01-21:	RMA - Created
//-----------------------------------------------------------------------------
void SCENE_GRAPH::DisplayPhysics(const MATRIX & ViewMatrix,
								 const MATRIX & ProjectionMatrix) const
{
	if (GetPhysicScene())
	{
		GetPhysicScene()->DebugDisplay(ViewMatrix, ProjectionMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		DumpPhysicsToFile
//	Object:		
//	06-03-16:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::DumpPhysicsToFile(OUTPUT_FILE_FORMAT	FileFormat,
									   const char *			pFileName)
{
	if (GetPhysicScene())
	{
		GetPhysicScene()->Dump(FileFormat, pFileName);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Grab
//	Object:		
//	05-09-01:	ELE - Created
//-----------------------------------------------------------------------------
bool	SCENE_GRAPH::Grab(NODE *	pNode)
{
	if	(_pPhysicScene)
	{
		RAY			Ray;
		QUATERNION	Q;
		MATRIX		M;
		VECTOR		Position;
		VECTOR		Direction(0.0f, 0.0f, -1.0f);

		pNode->GetGlobalPosition(Position);
		Q = pNode->GetQuaternion();

		M.SetQuaternion(Q);
		M.TransformDirection(Direction);
		Direction.Normalize();
		
		Ray.SetOrigin(Position);
		Ray.SetDirection(Direction);

		I_PHYSIC_RAYCAST_HIT	*pRaycastHit = PHYSIC_CONTEXT::Instance()->CreateRaycastHit();
		I_PHYSIC_ACTOR			*pRayCastedActor = _pPhysicScene->RaycastClosestShape(Ray, PSST_DYNAMIC_SHAPES, pRaycastHit);

		if	(pRayCastedActor)
		{
			// Actor
			I_PHYSIC_DYNAMIC_ACTOR	*pActor = pNode->GetDynamicActor();

			if	(pActor == NULL)
			{
				pActor = pNode->CreatePhysicDynamicActor();
			}

			VECTOR	LocalPos(0.0f, 0.0f, 0.0f);
			VECTOR	FirstChildPos, Pos, V;

			I_PHYSIC_SHAPE_SPHERE * pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();
			pSphere->SetRadius(1.0f);
			pSphere->SetLocalPosition(VECTOR(0.0f, 0.0f, 0.0f));

			pActor->PushShape(pSphere);
			pActor->SetMass(2000.0f);
			pActor->SetGlobalMatrix(pNode->GetGlobalMatrix());
			pActor->SetLinearDamping(1.0f);
			pActor->SetAngularDamping(1.0f);
			
			_pPhysicScene->RegisterDynamicActor(pActor);

			pActor->EnableKinematicMode(true);

			// Joint
			I_PHYSIC_JOINT	*pJoint = pNode->GetAttachJoint();

			if	(pJoint == NULL)
			{
				pJoint = PHYSIC_CONTEXT::Instance()->CreateJointFixed();
				pNode->SetAttachJoint(pJoint);
			}

			VECTOR	JointGlobalAnchor;
			pNode->GetGlobalPosition(JointGlobalAnchor);

			pJoint->SetActor1(pActor);
			pJoint->SetActor2(pRayCastedActor);
			pJoint->SetGlobalAnchor(JointGlobalAnchor);
			pJoint->SetGlobalAxis(Direction);
			pJoint->EnableCollision(false);

			_pPhysicScene->RegisterJoint(pJoint);

			PHYSIC_MANAGER::Instance()->AddNodeWithPhysics(pNode);

			return	(true);
		}
	}

	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		Ungrab
//	Object:		
//	05-09-02:	ELE - Created
//-----------------------------------------------------------------------------
void	SCENE_GRAPH::Ungrab(NODE *	pNode)
{
	if	(_pPhysicScene && pNode)
	{
		// Actor
		I_PHYSIC_DYNAMIC_ACTOR	*pActor = pNode->GetDynamicActor();

		if	(pActor)
		{
			_pPhysicScene->UnregisterDynamicActor(pActor);
		}

		// Joint
		I_PHYSIC_JOINT	*pJoint = pNode->GetAttachJoint();

		if	(pJoint)
		{
			_pPhysicScene->UnregisterJoint(pJoint);
		}

		PHYSIC_MANAGER::Instance()->RemoveNodeWithPhysics(pNode);
	}
}


//-----------------------------------------------------------------------------
//	Name:		GetLocalAmbient
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
Bool	SCENE_GRAPH::GetLocalAmbient(const VECTOR &				Pos,
									 SPHERICAL_HARMONICS &	WorldSH)
{
	if (_lAmbientOctree.GetSize() == 0)
	{
		WorldSH.Init();
		WorldSH.AddConstant(1.0f, 1.0f, 1.0f);
		return (false);
	}
	else
	{
		//Here we'll find which ambient octree we want to sample
		//For now we pick the first one
		
		AMBIENT_OCTREE* pAmbOctree = *_lAmbientOctree.GetHead();
		pAmbOctree->SampleIrradiance(Pos, WorldSH);
		return (true);
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
