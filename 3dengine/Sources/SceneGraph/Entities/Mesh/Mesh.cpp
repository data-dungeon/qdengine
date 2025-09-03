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
//	CLASS:	MESH
//
//	05-10-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Viewport/Viewport)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/MeshData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/StaticPhysicData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/DynamicPhysicData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeBoxData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeSphereData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeCapsuleData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeTriangleData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Primitive)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/FacesToSort)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Flow)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/FlowData)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureFlow)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(SceneGraph/RayCaster/RayCastParameters)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Display/SortedObject/SortedObject)
#include	INCL_3DENGINE(Display/TransparentObject/TransparentObject)
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KPHYSICS(Physics/IPhysicShapeMeshData)
#include	INCL_KPHYSICS(Physics/IPhysicShapeBox)
#include	INCL_KPHYSICS(Physics/IPhysicShapeSphere)
#include	INCL_KPHYSICS(Physics/IPhysicShapeCapsule)
#include	INCL_KPHYSICS(Physics/IPhysicShapeConvexMesh)
#include	INCL_KPHYSICS(Physics/IPhysicStaticActor)
#include	INCL_KPHYSICS(Physics/IPhysicRayCastHit)
#include	INCL_KPHYSICS(Physics/IPhysicMaterial)
#include	INCL_KPHYSICS(Physics/PhysicShapeList)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KRENDERER(Renderer/IVertexBuffer)
#include	INCL_KRENDERER(Renderer/IIndexBuffer)
#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

#ifdef _DEBUG
	#include	"Mesh.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		MESH constructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
MESH::MESH()
:
ENTITY_ABC					(GetGenericCommunicatorDescriptor(), ENTITY_MESH),
_nNbrPrimitives				(0),
_pGlobalBoundingBox			(NULL),
_pGlobalBoundingSphere		(NULL),
_pbVisible					(NULL),
_pStaticActor				(NULL),
_pPhysicShapeMeshData		(NULL),
_pPhysicMaterial			(NULL),
_pFlows						(NULL),
_numFlows					(0)
{
	SetVisible(true);
}

//-----------------------------------------------------------------------------
//	Name:		MESH constructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
MESH::MESH(COMMUNICATOR_DESCRIPTOR *	pCD,
		   ENTITY_TYPE					eEntityType)
:
ENTITY_ABC					(pCD, eEntityType),
_nNbrPrimitives				(0),
_pGlobalBoundingBox			(NULL),
_pGlobalBoundingSphere		(NULL),
_pbVisible					(NULL),
_pStaticActor				(NULL),
_pPhysicShapeMeshData		(NULL),
_pPhysicMaterial			(NULL),
_pFlows						(NULL),
_numFlows					(0)
{
	SetVisible(true);
}

//-----------------------------------------------------------------------------
//	Name:		MESH destructor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
MESH::~MESH()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Register()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->AddMesh(this);

		Init();

		// Physic : static actor
		if	(_pStaticActor)
		{
			pSG->GetPhysicScene()->RegisterStaticActor(_pStaticActor);
		}
		else
		{
			NODE	*pNode = GetNode();

			if	(pNode)
			{
				I_PHYSIC_DYNAMIC_ACTOR	*pActor = pNode->GetDynamicActor();
				if	(pActor)
				{
					if	(_pPhysicMaterial)
					{
						pSG->GetPhysicScene()->RegisterMaterial(_pPhysicMaterial);

						PHYSIC_SHAPE_LIST * pLoop = pActor->GetShapeList();
						while (pLoop)
						{
							pLoop->GetShape()->SetMaterial(_pPhysicMaterial);
							pLoop = pLoop->GetNext();
						}

						DYNAMIC_PHYSIC_DATA *	pPhysicData = static_cast<DYNAMIC_PHYSIC_DATA *>(_PhysicDataCI.GetCommunicator());
						_pPhysicMaterial->SetBounciness(pPhysicData->GetBounciness());
						_pPhysicMaterial->SetDynamicFriction(pPhysicData->GetDynamicFriction());
						_pPhysicMaterial->SetStaticFriction(pPhysicData->GetStaticFriction());
					}

					pActor->SetGlobalMatrix(pNode->GetGlobalMatrix());
					pSG->GetPhysicScene()->RegisterDynamicActor(pActor);
					PHYSIC_MANAGER::Instance()->AddNodeWithPhysics(pNode);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-05:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Unregister()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->RemoveMesh(this);

		// Physic : static actor
		if	(_pStaticActor)
		{
			pSG->GetPhysicScene()->UnregisterStaticActor(_pStaticActor);
		}
		else
		{
			NODE	*pNode = GetNode();

			if	(pNode)
			{
				I_PHYSIC_DYNAMIC_ACTOR	*pActor = pNode->GetDynamicActor();
				if	(pActor)
				{
					if	(_pPhysicMaterial)
					{
						pSG->GetPhysicScene()->UnregisterMaterial(_pPhysicMaterial);
					}

					pSG->GetPhysicScene()->UnregisterDynamicActor(pActor);
					PHYSIC_MANAGER::Instance()->RemoveNodeWithPhysics(pNode);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Instanciate(COM_INSTANCE &	Instance)
{
	MESH *	pNewMesh;

	Instance.SetHandle(MESH::GetGenericCommunicatorDescriptor()->Create());
	pNewMesh = static_cast<MESH *>(Instance.GetCommunicator());

	// Copy needed infos for instanciate
	pNewMesh->_fDisplayFlags = _fDisplayFlags;
	pNewMesh->SetMeshData(_MeshDataCI);
	pNewMesh->InitPhysicData(_PhysicDataCI);

	InstanciateNode(pNewMesh);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::InstanciateRec(COM_INSTANCE &	Instance)
{
	MESH *	pNewMesh;

	Instance.SetHandle(MESH::GetGenericCommunicatorDescriptor()->Create());
	pNewMesh = static_cast<MESH *>(Instance.GetCommunicator());

	// Copy needed infos for instanciate
	pNewMesh->_fDisplayFlags = _fDisplayFlags;
	pNewMesh->SetMeshData(_MeshDataCI);
	pNewMesh->InitPhysicData(_PhysicDataCI);

	InstanciateNodeRec(pNewMesh);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-11-03:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Init()
{
	if	((_fMeshFlags.IsSet(MESH_FLAG_INIT) == false) && _MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
	{
		MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

		Clear();

		pMeshData->Init();

		// Primitives
		_nNbrPrimitives = pMeshData->GetNbrPrimitives();
		if	(_nNbrPrimitives > 0)
		{
			_pGlobalBoundingBox = new OBB[_nNbrPrimitives];
			_pGlobalBoundingSphere = new SPHERE[_nNbrPrimitives];
			_pbVisible = new Bool[_nNbrPrimitives];
			memset(_pbVisible, 0, _nNbrPrimitives * sizeof(Bool));
		}

		// Physic
		if	(_PhysicDataCI.HasHandle() && _PhysicDataCI.IsLoaded())
		{
			if	(_PhysicDataCI.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STATIC_PHYSIC_DATA)
			{
				// Static physic
				STATIC_PHYSIC_DATA *	pPhysicData = static_cast<STATIC_PHYSIC_DATA *>(_PhysicDataCI.GetCommunicator());

				_pStaticActor = PHYSIC_CONTEXT::Instance()->CreateStaticActor();
				_pPhysicShapeMeshData = PHYSIC_CONTEXT::Instance()->CreateShapeMeshData();

				if (_pStaticActor && _pPhysicShapeMeshData)
				{
					_pPhysicShapeMeshData->GenerateDatas(pPhysicData->GetGeometricData());
					_pStaticActor->PushShape(_pPhysicShapeMeshData);
				}
			}
			else
			{
				// Dynamic physic
				DYNAMIC_PHYSIC_DATA *	pPhysicData = static_cast<DYNAMIC_PHYSIC_DATA *>(_PhysicDataCI.GetCommunicator());
				NODE *					pNode = GetNode();

				I_PHYSIC_DYNAMIC_ACTOR * pActor = pNode->CreatePhysicDynamicActor();
				QDT_ASSERT(pActor);

				Float32	rMass = pPhysicData->GetMass();
				if	(rMass > 0.0f)
				{
					pActor->SetMass(rMass);
				}
				else
				{
					pActor->SetDensity(pPhysicData->GetDensity());
				}

				pActor->SetCenterOfMassLocalPosition(pPhysicData->GetCenterOfMass());
				pActor->SetLinearDamping(pPhysicData->GetLinearDamping());
				pActor->SetAngularDamping(pPhysicData->GetAngularDamping());
				pActor->EnableKinematicMode(pPhysicData->IsKinematic());
				pActor->EnableGravity(pPhysicData->IsInfluencedByGravity());
				pActor->SetSolverIterationCount(pPhysicData->GetNbrSolverIterations());

				// Shapes
				UInt32	i, nNbrShapes = pPhysicData->GetNbrShapesData();

				for	(i = 0 ; i < nNbrShapes ; ++i)
				{
					const PHYSIC_SHAPE_DATA *	pShapeData = pPhysicData->GetShapeData(i);

					switch	(pShapeData->GetType())
					{
						case	PHYSIC_SHAPE_DATA::PSDT_BOX:
							{
								const PHYSIC_SHAPE_BOX_DATA *	pBoxData = static_cast<const PHYSIC_SHAPE_BOX_DATA *>(pShapeData);
								I_PHYSIC_SHAPE_BOX *			pBox = PHYSIC_CONTEXT::Instance()->CreateShapeBox();

								pBox->SetDimensions(pBoxData->GetDimensions());
								pBox->SetLocalMatrix(pBoxData->GetLocalMatrix());

								pActor->PushShape(pBox);
							}
							break;

						case	PHYSIC_SHAPE_DATA::PSDT_SPHERE:
							{
								const PHYSIC_SHAPE_SPHERE_DATA *	pSphereData = static_cast<const PHYSIC_SHAPE_SPHERE_DATA *>(pShapeData);
								I_PHYSIC_SHAPE_SPHERE *				pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();

								pSphere->SetRadius(pSphereData->GetRadius());
								pSphere->SetLocalMatrix(pSphereData->GetLocalMatrix());

								pActor->PushShape(pSphere);
							}
							break;

						case	PHYSIC_SHAPE_DATA::PSDT_CAPSULE:
							{
								const PHYSIC_SHAPE_CAPSULE_DATA *	pCapsuleData = static_cast<const PHYSIC_SHAPE_CAPSULE_DATA *>(pShapeData);
								I_PHYSIC_SHAPE_CAPSULE *			pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();

								pCapsule->SetRadius(pCapsuleData->GetRadius());
								pCapsule->SetHeight(pCapsuleData->GetHeight());
								pCapsule->SetLocalMatrix(pCapsuleData->GetLocalMatrix());

								pActor->PushShape(pCapsule);
							}
							break;

						case	PHYSIC_SHAPE_DATA::PSDT_TRIANGLE:
							{
								const PHYSIC_SHAPE_TRIANGLE_DATA *	pTriangleData = static_cast<const PHYSIC_SHAPE_TRIANGLE_DATA *>(pShapeData);

								switch	(pTriangleData->GetType())
								{
									case	PHYSIC_SHAPE_TRIANGLE_DATA::PSTT_CONVEX:
										{
											I_PHYSIC_SHAPE_CONVEX_MESH *	pConvexMesh = PHYSIC_CONTEXT::Instance()->CreateShapeConvexMesh();

											pConvexMesh->GenerateDatas(pTriangleData->GetGeometricData());
											pConvexMesh->SetLocalMatrix(pTriangleData->GetLocalMatrix());

											pActor->PushShape(pConvexMesh);
										}
										break;

									case	PHYSIC_SHAPE_TRIANGLE_DATA::PSTT_NONCONVEX:
										{
											I_PHYSIC_SHAPE_MESH_DATA *	pTriangleMesh = PHYSIC_CONTEXT::Instance()->CreateShapeMeshData();

											pTriangleMesh->GenerateDatas(pTriangleData->GetGeometricData());
											pTriangleMesh->SetLocalMatrix(pTriangleData->GetLocalMatrix());

											pActor->PushShape(pTriangleMesh);
										}
										break;
								}
							}
							break;
					}
				}

				// Material
				_pPhysicMaterial = PHYSIC_CONTEXT::Instance()->CreateMaterial();
			}
		}

		_fMeshFlags.Set(MESH_FLAG_INIT, true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Clear()
{
	if	(_MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
	{
		MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());
		pMeshData->Clear();
	}

	// Primitives
	if	(_nNbrPrimitives > 0)
	{
		delete [] _pGlobalBoundingBox;
		delete [] _pGlobalBoundingSphere;
		delete [] _pbVisible;

		_pGlobalBoundingBox = NULL;
		_pGlobalBoundingSphere = NULL;
		_pbVisible = NULL;
	}
	_nNbrPrimitives = 0;

	// Physic
	if	(_pStaticActor)
	{
		// Don't need to release shape, it's done by releasing actor
		PHYSIC_CONTEXT::Instance()->ReleaseActor(_pStaticActor);

		_pStaticActor = NULL;
		_pPhysicShapeMeshData = NULL;
	}

	_fMeshFlags.Set(MESH_FLAG_INIT, false);
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	06-03-08:	RMA - Created
//-----------------------------------------------------------------------------
Bool	MESH::TestVisibility(const CONE & Cone, 
							 UInt32 uiDisplayFlags)
{
	UInt32	i;
	NODE *	pNode = GetNode();

	_bVisible = false;

	if	(_fDisplayFlags.GetFlags(uiDisplayFlags) ==  uiDisplayFlags && 
		 _MeshDataCI.HasHandle() && 
		 _MeshDataCI.IsLoaded())
	{
		SPHERE GlobalBoundingSphere(pNode->GetLocalBoundingSphere());
		GlobalBoundingSphere.Transform(pNode->GetGlobalMatrix());

		// RMA : This volume isn't computed in maya so I'm removing this test
		// The bounding sphere should be set in entity abc (local & global...)
		// if (GlobalBoundingSphere.Intersect(Cone))
		{
			if	(_MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
			{
				MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

				for	(i = 0 ; i < _nNbrPrimitives ; ++i)
				{
					QDT_FLAGS fDisplayFlags = pMeshData->GetDisplayFlags(i);

					if (fDisplayFlags.GetFlags(uiDisplayFlags) == uiDisplayFlags)
					{
						_pGlobalBoundingSphere[i] = pMeshData->GetLocalBoundingSphere(i);
						_pGlobalBoundingSphere[i].Transform(pNode->GetGlobalMatrix());

						_pbVisible[i] = _pGlobalBoundingSphere[i].Intersect(Cone);

						if	(_pbVisible[i])
						{
							_bVisible = true;
							SendPrimitiveToDisplayList(i, true);
						}
					}
				}
			}
		}
	}

	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
Bool	MESH::TestVisibility(const FRUSTRUM & Frustum, 
							 UInt32 uiDisplayFlags)
{
	UInt32	i;
	NODE *	pNode = GetNode();

	_bVisible = false;

	if	(_fDisplayFlags.GetFlags(uiDisplayFlags) ==  uiDisplayFlags && 
		 _MeshDataCI.HasHandle() && 
		 _MeshDataCI.IsLoaded())
	{
		SPHERE GlobalBoundingSphere(pNode->GetLocalBoundingSphere());
		GlobalBoundingSphere.Transform(pNode->GetGlobalMatrix());

		// RMA : This volume isn't computed in maya so I'm removing this test
		// The bounding sphere should be set in entity abc (local & global...)
		// if (GlobalBoundingSphere.Intersect(Frustum.GetHexahedron()))
		{
			if	(_MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
			{
				MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

				for	(i = 0 ; i < _nNbrPrimitives ; ++i)
				{
					QDT_FLAGS fDisplayFlags = pMeshData->GetDisplayFlags(i);

					if (fDisplayFlags.GetFlags(uiDisplayFlags) == uiDisplayFlags)
					{
						_pGlobalBoundingBox[i] = pMeshData->GetLocalBoundingBox(i);
						_pGlobalBoundingBox[i].Transform(pNode->GetGlobalMatrix());

						_pbVisible[i] = _pGlobalBoundingBox[i].Intersect(Frustum.GetHexahedron());

						if (_pbVisible[i])
						{
							_bVisible = true;
							SendPrimitiveToDisplayList(i);
						}
					}
				}
			}
		}
	}

	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeCameraDistance
//	Object:		
//	06-03-09:	RMA - FindLights
//-----------------------------------------------------------------------------
Float32	MESH::ComputeCameraDistance(UInt32 uiPrim)
{
	const VECTOR &	Center = _pGlobalBoundingBox[uiPrim].GetCenter();
	VECTOR			CameraPos, ViewDir;

	DISPLAY_MANAGER::Instance()->GetActiveCamera()->GetNode()->GetGlobalPosition(CameraPos);
	ViewDir.Sub(Center, CameraPos);

	return (ViewDir.SquareLength());
}

//-----------------------------------------------------------------------------
//	Name:		SendPrimitiveToDisplayList
//	Object:		
//	06-03-09:	RMA - FindLights
//-----------------------------------------------------------------------------
void MESH::SendPrimitiveToDisplayList(UInt32 uiPrim, Bool bForceTransparentObject)
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());
	const PRIMITIVE & Primitive = pMeshData->GetPrimitive(uiPrim);
	SHADER_ABC * pShader = static_cast<SHADER_ABC *>(Primitive.GetShader().GetCommunicator());

	if	(pShader->IsTransparent() && (bForceTransparentObject == false))
	{
		TRANSPARENT_OBJECT	Obj;

		Obj.SetPriority(pShader->GetTransparencyPriority());
		Obj.SetCameraDistance(ComputeCameraDistance(uiPrim));
		Obj.SetEntity(this);
		Obj.SetPrimitive(uiPrim);

		_pSceneGraph->AddTransparentObject(Obj);
	}
	else
	{
		QDT_VECTOR<SORTED_OBJECT> &	vObjects = pShader->GetObjectsToDisplay();
		UInt32						j, nNbrObjects = vObjects.GetSize();
		Bool						bFound = false;

		if	(nNbrObjects > 0)
		{
			for	(j = 0 ; j < nNbrObjects ; ++j)
			{
				SORTED_OBJECT & Obj = vObjects[j];
				if	(Obj.GetEntity() == this)
				{
					Obj.AddPrimitive(uiPrim);

					bFound = true;
					break;
				}
			}
		}
		else
		{
			_pSceneGraph->AddShader(pShader);
		}

		if	(!bFound)
		{
			SORTED_OBJECT Obj(this, uiPrim);
			vObjects.PushTail(Obj);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindLights
//	Object:		
//	05-11-21:	ELE - FindLights
//-----------------------------------------------------------------------------
void MESH::FindLights(const SPHERE &	PrimitiveSphere)
{
	VIEWPORT * pViewport = DISPLAY_MANAGER::Instance()->GetCurrentViewport();
	
	DISPLAY_MANAGER::Instance()->ClearLights();
	DISPLAY_MANAGER::Instance()->SetAmbientColor(COLOR_FLOAT(0.0f, 0.0f, 0.0f, 0.0f));

	if (pViewport->IsLightingEnabled())
	{
		if (pViewport->IsDefaultLightingEnabled())
		{
			CAMERA * pCamera = DISPLAY_MANAGER::Instance()->GetActiveCamera();
			NODE * pNode = pCamera->GetNode();

			VECTOR vCameraDir(VECTOR::UNIT_Z);
			pNode->GetGlobalMatrix().TransformDirection(vCameraDir);

			MATRIX mInverse;
			mInverse.InverseAffine(GetNode()->GetGlobalMatrix());
			mInverse.TransformDirection(vCameraDir);

			S_LIGHT_SOURCE	LightSource;
			LightSource._eType = LIGHT_DIRECTIONAL;
			LightSource._vDir = vCameraDir;
			LightSource._rColorR = 1.0f;
			LightSource._rColorG = 1.0f;
			LightSource._rColorB = 1.0f;
			LightSource._rIntensity = 1.0f;
			LightSource._rDiffuse = 1.0f;
			LightSource._rSpecular = 1.0f;

			DISPLAY_MANAGER::Instance()->PushLight(LightSource);
		}
		else
		{
			SCENE_GRAPH * pSG = GetSceneGraph();

			if	(pSG)
			{
				const QDT_DLIST<LIGHT_ABC *> & vLights = pSG->GetVisibleLights();

				MATRIX mInverse;
				mInverse.InverseAffine(GetNode()->GetGlobalMatrix());

				DISPLAY_MANAGER::Instance()->ClearLights();
				DISPLAY_MANAGER::Instance()->SetAmbientColor(pSG->GetAmbientColor());

				QDT_DLIST<LIGHT_ABC *>::ITERATOR It = vLights.GetHead();
				QDT_DLIST<LIGHT_ABC *>::ITERATOR ItEnd = vLights.GetTail();
				while	(It != ItEnd)
				{
					LIGHT_ABC *	pLight = *It;

					if	(pLight->GetSwitch() && pLight->TestLighting(PrimitiveSphere))
					{
						S_LIGHT_SOURCE	LightSource = pLight->GetLightSource();

						mInverse.TransformDirection(LightSource._vDir);
						mInverse.TransformPosition(LightSource._vPos);

						LightSource._vDir.Normalize();

						DISPLAY_MANAGER::Instance()->PushLight(LightSource);
					}

					++It;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SortFaces
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
I_INDEX_BUFFER * MESH::SortFaces(UInt32	uiIndex)
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

	MATRIX mCameraToWorld = DISPLAY_MANAGER::Instance()->GetCameraNode()->GetGlobalMatrix();
	MATRIX mWorldToObject = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();
	MATRIX mCameraToObject;

	mCameraToObject.Mul(mCameraToWorld, mWorldToObject);

	// Need to get eye dir in object space to sort!
	PLANE vEyePlane(0.0f, 0.0f, -1.0f, 0.0f);
	vEyePlane.Transform(mCameraToObject);

	return (pMeshData->GetFacesToSort(uiIndex).Sort(vEyePlane));
}	

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Display(UInt32 nPrimitive)
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

	DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();

	NODE	*pNode = GetNode();
	CAMERA	*pCamera = pDisplay->GetActiveCamera();

	pDisplay->SetCurrentMesh(this);
	pDisplay->SetCurrentMeshData(_MeshDataCI);
	pDisplay->EnableSkinning(false);
	pDisplay->SetWorldMatrix(pNode->GetGlobalMatrix());

	_pGlobalBoundingSphere[nPrimitive] = pMeshData->GetLocalBoundingSphere(nPrimitive);
	_pGlobalBoundingSphere[nPrimitive].Transform(pNode->GetGlobalMatrix());
	FindLights(_pGlobalBoundingSphere[nPrimitive]);

	DrawPrimitive(nPrimitive, _pGlobalBoundingBox->GetCenter());
}

//-----------------------------------------------------------------------------
//	Name:		DrawPrimitive
//	Object:		
//	06-03-13:	RMA - Created
//-----------------------------------------------------------------------------
void    MESH::DrawPrimitive(UInt32 uiPrim,
							const VECTOR& shCenter)
{
	UInt32	nNbrTriangles;
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());
	const PRIMITIVE & Primitive = pMeshData->GetPrimitive(uiPrim);
	DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();
	QDT_FLAGS fDisplayFlags = pMeshData->GetDisplayFlags(uiPrim);
	Bool bShadowReceiverEnabled = fDisplayFlags.IsSet(DISPLAY_FLAG_RECEIVE_SHADOWS);

	RENDER_CONTEXT::SetVertexDeclaration(pMeshData->GetVertexDecl(Primitive.GetVertexBufferIdx()));
	RENDER_CONTEXT::SetVertexBuffer(0, pMeshData->GetVertexBuffer(Primitive.GetVertexBufferIdx()));

	if (Primitive.GetPerPolygoneSorting())
	{
		RENDER_CONTEXT::SetIndexBuffer(SortFaces(Primitive.GetIndexBufferIdx()));
	}
	else
	{
		RENDER_CONTEXT::SetIndexBuffer(pMeshData->GetIndexBuffer(Primitive.GetIndexBufferIdx()));
	}

	switch	(Primitive.GetType())
	{
	case PT_TRIANGLE_LIST:
		nNbrTriangles = Primitive.GetNbrIndices() / 3;
		break;

	case PT_TRIANGLE_STRIP:
		nNbrTriangles = Primitive.GetNbrIndices() - 2;
		break;
	}

	SHADER_ABC * pShader = NULL;

	if (pDisplay->IsShaderReplacementEnabled())
	{
		pShader = static_cast<SHADER_ABC *>(pDisplay->GetReplacementShader().GetCommunicator());
	}
	else
	{
		pShader = static_cast<SHADER_ABC *>(Primitive.GetShader().GetCommunicator());
	}

	pShader->SetSupportShadowing(bShadowReceiverEnabled);
	if	(pShader->IsTransparent() && 
		 pShader->GetDoubleSided() &&
		 pShader->GetDoubleRendering() && 
		 Primitive.GetPerPolygoneSorting() == false)
	{
		RENDER_CONTEXT::SetCullMode(CULL_COUNTERCLOCKWISE);

		UInt32 uiPass, uiNbrPasses = pShader->GetNbrPasses();
		for (uiPass = 0 ; uiPass < uiNbrPasses ; ++uiPass)
		{
			pShader->SetPass(uiPass, shCenter);
			RENDER_CONTEXT::DrawIndexedPrimitive(Primitive.GetType(),
				Primitive.GetFirstVertex(),
				Primitive.GetNbrVertices(),
				Primitive.GetFirstIndex(),
				nNbrTriangles);
			pShader->Restore();
		}

		RENDER_CONTEXT::SetCullMode(CULL_CLOCKWISE);

		uiNbrPasses = pShader->GetNbrPasses();
		for (uiPass = 0 ; uiPass < uiNbrPasses ; ++uiPass)
		{
			pShader->SetPass(uiPass, shCenter);
			RENDER_CONTEXT::DrawIndexedPrimitive(Primitive.GetType(),
				Primitive.GetFirstVertex(),
				Primitive.GetNbrVertices(),
				Primitive.GetFirstIndex(),
				nNbrTriangles);
			pShader->Restore();
		}
	}
	else
	{
		if (pShader->GetDoubleSided())
		{
			RENDER_CONTEXT::SetCullMode(CULL_NONE);
		}
		else
		{
			RENDER_CONTEXT::SetCullMode(CULL_CLOCKWISE);
		}

		UInt32 uiPass, uiNbrPasses = pShader->GetNbrPasses();
		for (uiPass = 0 ; uiPass < uiNbrPasses ; ++uiPass)
		{
			if (Primitive.GetPerPolygoneSorting() && uiPass != uiNbrPasses-1)
			{
				RENDER_CONTEXT::EnableDepthWrite(false);
			}
			else
			{
				RENDER_CONTEXT::EnableDepthWrite(true);
			}

			pShader->SetPass(uiPass, shCenter);
			RENDER_CONTEXT::DrawIndexedPrimitive(Primitive.GetType(),
				Primitive.GetFirstVertex(),
				Primitive.GetNbrVertices(),
				Primitive.GetFirstIndex(),
				nNbrTriangles);
			pShader->Restore();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::Display(const QDT_VECTOR < UInt32 > &	vPrimitives)
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());
	DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();

	UInt32	i, nNbrPrimitives;
	NODE	*pNode = GetNode();
	CAMERA	*pCamera = pDisplay->GetActiveCamera();

	pDisplay->SetCurrentMesh(this);
	pDisplay->SetCurrentMeshData(_MeshDataCI);
	pDisplay->EnableSkinning(false);
	pDisplay->SetWorldMatrix(pNode->GetGlobalMatrix());
	
	nNbrPrimitives = vPrimitives.GetSize();
	for	(i = 0 ; i < nNbrPrimitives ; ++i)
	{
		const PRIMITIVE & Primitive = pMeshData->GetPrimitive(vPrimitives[i]);

		_pGlobalBoundingSphere[vPrimitives[i]] = pMeshData->GetLocalBoundingSphere(vPrimitives[i]);
		_pGlobalBoundingSphere[vPrimitives[i]].Transform(pNode->GetGlobalMatrix());
		FindLights(_pGlobalBoundingSphere[vPrimitives[i]]);

		DrawPrimitive(vPrimitives[i], _pGlobalBoundingBox->GetCenter());
	}
}

//-----------------------------------------------------------------------------
//	Name:		RayCast
//	Object:		
//	05-12-13:	ELE - Created
//-----------------------------------------------------------------------------
Bool	MESH::RayCast(RAY_CAST_PARAMETERS &	RayCastParameters)
{
	if	(_pPhysicShapeMeshData == NULL)
	{
		return	(false);
	}

	I_PHYSIC_RAYCAST_HIT *	pRaycastHit = PHYSIC_CONTEXT::Instance()->CreateRaycastHit();
	Bool	bTouched = _pPhysicShapeMeshData->Raycast(RayCastParameters.GetRay(), QDT_FLOAT32_MAX, 0xffffffff, pRaycastHit, false);

	if	(bTouched)
	{
		float	rImpactDistance = pRaycastHit->GetImpactDistance();
		if	(rImpactDistance > RayCastParameters.GetImpactDistance())
		{
			delete	(pRaycastHit);
			return	(false);
		}

		VECTOR	ImpactNormal;
		pRaycastHit->GetImpactNormal(ImpactNormal);

		RayCastParameters.SetImpactDistance(rImpactDistance);
		RayCastParameters.SetImpactNormal(ImpactNormal);

		RayCastParameters.SetImpactNodeID(GetNode()->GetID());

		delete	(pRaycastHit);
		return	(true);
	}

	delete	(pRaycastHit);
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESH", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESH", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESH", 3, CM_ReadChunk3);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("MESH", 4, CM_ReadChunk4);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("MESH", 4, CM_WriteChunk4);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MESH::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	MESH::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("MESH"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH,
											QDT_COMMENT("C6AF089C-F170-9B21-218A-F3DC7C405500"),
											sizeof(MESH),
											ComInit,
											QDT_COMMENT("Class for meshes"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::EM_Construct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pO);

	QDT_NEW_PLACED(pMesh, MESH)();
	QDT_ASSERT(pMesh);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-10-27:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::EM_Destruct(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pO);

	QDT_DELETE_PLACED(pMesh, MESH);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::CM_ReadChunk1(IO_STREAM &		Stream,
						    COMMUNICATOR *	pCommunicator)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pCommunicator);

	Stream >> pMesh->_NodeInst;

	Stream >> pMesh->_MeshDataCI;
	pMesh->SetVisible(true);
	pMesh->SetCastShadows(true);
	pMesh->SetReceiveShadows(true);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	06-02-20:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::CM_ReadChunk2(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pCommunicator);

	Stream >> pMesh->_NodeInst;

	Stream >> pMesh->_MeshDataCI;
	Stream >> pMesh->_PhysicDataCI;
	pMesh->SetVisible(true);
	pMesh->SetCastShadows(true);
	pMesh->SetReceiveShadows(true);

	pMesh->UpdateLocalBoundingVolumes();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk3
//	Object:		
//	06-02-20:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::CM_ReadChunk3(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pCommunicator);

	Stream >> pMesh->_NodeInst;

	Stream >> pMesh->_MeshDataCI;
	Stream >> pMesh->_PhysicDataCI;
	Stream >> pMesh->_fDisplayFlags;
	
	pMesh->UpdateLocalBoundingVolumes();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk4
//	Object:		
//	06-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::CM_ReadChunk4(IO_STREAM &		Stream,
							COMMUNICATOR *	pCommunicator)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pCommunicator);

	Stream >> pMesh->_MeshDataCI;
	Stream >> pMesh->_PhysicDataCI;
	Stream >> pMesh->_fDisplayFlags;
	
	pMesh->UpdateLocalBoundingVolumes();
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk4
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	MESH::CM_WriteChunk4(IO_STREAM &		Stream,
							 COMMUNICATOR *		pCommunicator)
{
	MESH	*pMesh;
	pMesh = static_cast<MESH *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("MESH :");
	Stream.DebugOutput(pMesh->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Meshdata : ");
	Stream << pMesh->_MeshDataCI;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Physicdata : ");
	Stream << pMesh->_PhysicDataCI;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("	Display flags : ");
	Stream << pMesh->_fDisplayFlags;
	Stream.DebugOutput("\r\n");
}

//-----------------------------------------------------------------------------
//	Name:		UpdateLocalBoundingVolumes
//	Object:		
//	06-02-10:	SBE - Created
//-----------------------------------------------------------------------------
void	MESH::UpdateLocalBoundingVolumes()
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());
	UInt32 nNbrPrimitives = pMeshData->GetNbrPrimitives();
	QDT_ASSERT(nNbrPrimitives > 0);

	OBB LocalBoundingBox(pMeshData->GetLocalBoundingBox(0));
	SPHERE LocalBoundingSphere(pMeshData->GetLocalBoundingSphere(0));

	for (UInt32 iPrimitive = 1; iPrimitive < nNbrPrimitives; iPrimitive++)
	{
		// OBB
		VECTOR Points[8];
		
		pMeshData->GetLocalBoundingBox(iPrimitive).GetPoints(Points);
		
		for (UInt32 iPoint = 0; iPoint < 8; iPoint++)
		{
			LocalBoundingBox.Grow(Points[iPoint]);
		}

		// SPHERE
		LocalBoundingSphere.Grow(pMeshData->GetLocalBoundingSphere(iPrimitive));
	}

	NODE * pNode = GetNode();
	pNode->SetLocalBoundingBox(LocalBoundingBox);
	pNode->SetLocalBoundingSphere(LocalBoundingSphere);
}



//-----------------------------------------------------------------------------
//	Name:		GetFlow
//	Object:		
//	06-03-29:	PDE - Created
//-----------------------------------------------------------------------------
FLOW *	MESH::GetFlow(ENGINE_TEXTURE_FLOW *	flowTex, FLOW_DATA* flowData)
{
	QDT_ASSERT(flowTex != NULL);

	UInt32 uvSet = flowTex->GetUVSet();
	if (uvSet <= _numFlows + 1)
	{
		UInt32 newNum = uvSet + 1;
		_pFlows = (FLOW**)realloc(_pFlows, newNum * sizeof(FLOW*));

		for (UInt32 i = _numFlows; i < newNum; i++)
		{
			_pFlows[i] = NULL;
		}
		_numFlows = newNum;
	}

	if (!_pFlows[uvSet])
	{
		_pFlows[uvSet] = new FLOW(flowTex);
	}

	FLOW * res = _pFlows[uvSet];
	res->SetFlowData(flowData);
	return res;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateFlows
//	Object:		
//	06-03-30:	FBO - Created
//-----------------------------------------------------------------------------
void	MESH::UpdateFlows()
{
	for (UInt32 i = 0; i < _numFlows; i++)
	{
		if (_pFlows[i])
			_pFlows[i]->Update();
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
