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
// CLASS: SKELETON
//
//
//	00-11-09	GHP - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION ( SKELETON )
//
//	The SKELETON class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Hair/Hair)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollData)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollActor)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollConstraint)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/MeshData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeBoxData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeSphereData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeCapsuleData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeTriangleData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Primitive)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)
#include	INCL_3DENGINE(SceneGraph/Material/ShaderABC)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Display/SortedObject/SortedObject)
#include	INCL_3DENGINE(Display/TransparentObject/TransparentObject)
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_KPHYSICS(Physics/IPhysicStaticActor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KPHYSICS(Physics/IPhysicShapeCapsule)
#include	INCL_KPHYSICS(Physics/IPhysicShapeBox)
#include	INCL_KPHYSICS(Physics/IPhysicShapeSphere)
#include	INCL_KPHYSICS(Physics/IPhysicShapeConvexMesh)
#include	INCL_KPHYSICS(Physics/IPhysicShapeMeshData)
#include	INCL_KPHYSICS(Physics/IPhysicMaterial)
#include	INCL_KPHYSICS(Physics/IPhysicJointCustom)
#include	INCL_KPHYSICS(Physics/PhysicShapeList)

#ifdef _DEBUG
#include	"Skeleton.inl"
#endif

#define _NEW_SKL_NODE

//=============================================================================
// CODE STARTS HERE
//=============================================================================


//-----------------------------------------------------------------------------
//	Name:		GetBodyPos
//	Object:		
//	06-03-13:	PDE - Created
//-----------------------------------------------------------------------------
void	SKELETON::GetBodyPos(VECTOR &	Pos)
{
	NODE_ARRAY	*pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
	Pos = pNodeArray->GetChild(0)->GetGlobalMatrix().GetTranslation();
}

//-----------------------------------------------------------------------------
// Name: SKELETON constructor
// Object: Default constructor
//00 - 11 - 09 GHP Created:
//-----------------------------------------------------------------------------
SKELETON::SKELETON()
:
MESH						(GetGenericCommunicatorDescriptor(), ENTITY_SKELETON),
_bNeedComputeBoundingBox	(false),
_pHair						(NULL),
_RagdollDataCI				(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_nNbrRagdollJoints			(0),
_pRagdollJoints				(NULL)
{	
	int i;

	for ( i = 0; i < NBR_JOINT_TYPES; ++i )
	{
		_SpecialSklJoints[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: SKELETON destructor
// Object: Default destructor
//00 - 11 - 09 GHP Created:
//-----------------------------------------------------------------------------
SKELETON::~SKELETON()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::Register()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->AddMesh(this);

		Init();

		if	(_RagdollDataCI.HasHandle() && _RagdollDataCI.IsLoaded())
		{
			UInt32			i, nNbrChildren;
			NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
			RAGDOLL_DATA *	pRagdollData = static_cast<RAGDOLL_DATA *>(_RagdollDataCI.GetCommunicator());

			nNbrChildren = pNodeArray->GetNbrChildren();
			for	(i = 0 ; i < nNbrChildren ; ++i)
			{
				NODE_CHILD *				pNodeChild = pNodeArray->GetChild(i);
				I_PHYSIC_DYNAMIC_ACTOR	*	pActor = pNodeChild->GetDynamicActor();

				if	(pActor)
				{
					I_PHYSIC_MATERIAL *	pPhysicMaterial = pNodeChild->GetPhysicMaterial();

					if	(pPhysicMaterial)
					{
						pSG->GetPhysicScene()->RegisterMaterial(pPhysicMaterial);

						PHYSIC_SHAPE_LIST * pLoop = pActor->GetShapeList();
						while (pLoop)
						{
							pLoop->GetShape()->SetMaterial(pPhysicMaterial);
							pLoop = pLoop->GetNext();
						}

						const RAGDOLL_ACTOR *	pRagdollActor = pRagdollData->GetActor(i);
						pPhysicMaterial->SetBounciness(pRagdollActor->GetBounciness());
						pPhysicMaterial->SetDynamicFriction(pRagdollActor->GetDynamicFriction());
						pPhysicMaterial->SetStaticFriction(pRagdollActor->GetStaticFriction());
					}

					pActor->SetGlobalMatrix(pNodeChild->GetGlobalMatrix());
					pSG->GetPhysicScene()->RegisterDynamicActor(pActor);
				}
			}

			for	(i = 0 ; i < _nNbrRagdollJoints ; ++i)
			{
				pSG->GetPhysicScene()->RegisterJoint(_pRagdollJoints[i]);
			}

			PHYSIC_MANAGER::Instance()->AddNodeWithPhysics(pNodeArray);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::Unregister()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		pSG->RemoveMesh(this);

		if	(_RagdollDataCI.HasHandle() && _RagdollDataCI.IsLoaded())
		{
			UInt32			i, nNbrChildren;
			NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
	
			nNbrChildren = pNodeArray->GetNbrChildren();
			for	(i = 0 ; i < nNbrChildren ; ++i)
			{
				NODE_CHILD *				pNodeChild = pNodeArray->GetChild(i);
				I_PHYSIC_DYNAMIC_ACTOR	*	pActor = pNodeChild->GetDynamicActor();

				if	(pActor)
				{
					I_PHYSIC_MATERIAL *	pPhysicMaterial = pNodeChild->GetPhysicMaterial();

					if	(pPhysicMaterial)
					{
						pSG->GetPhysicScene()->UnregisterMaterial(_pPhysicMaterial);
					}

					pSG->GetPhysicScene()->UnregisterDynamicActor(pActor);
				}
			}

			for	(i = 0 ; i < _nNbrRagdollJoints ; ++i)
			{
				pSG->GetPhysicScene()->UnregisterJoint(_pRagdollJoints[i]);
			}

			PHYSIC_MANAGER::Instance()->RemoveNodeWithPhysics(pNodeArray);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::Instanciate(COM_INSTANCE &	Instance)
{
	SKELETON *	pNewSkeleton;

	Instance.SetHandle(SKELETON::GetGenericCommunicatorDescriptor()->Create());
	pNewSkeleton = static_cast<SKELETON *>(Instance.GetCommunicator());

	InstanciateNode(pNewSkeleton);

	// Copy needed infos for instanciate
	pNewSkeleton->_fDisplayFlags = _fDisplayFlags;
	pNewSkeleton->_MeshDataCI = _MeshDataCI;
	pNewSkeleton->_RagdollDataCI = _RagdollDataCI;
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::InstanciateRec(COM_INSTANCE &	Instance)
{
	SKELETON *	pNewSkeleton;

	Instance.SetHandle(SKELETON::GetGenericCommunicatorDescriptor()->Create());
	pNewSkeleton = static_cast<SKELETON *>(Instance.GetCommunicator());

	InstanciateNodeRec(pNewSkeleton);

	// Copy needed infos for instanciate
	pNewSkeleton->_fDisplayFlags = _fDisplayFlags;
	pNewSkeleton->_MeshDataCI = _MeshDataCI;
	pNewSkeleton->_RagdollDataCI = _RagdollDataCI;
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::Init()
{
	if	(_fMeshFlags.IsSet(MESH_FLAG_INIT) == false)
	{
		MESH::Init();

		StoreJointIfSpecial(COM_INSTANCE(this), JT_ROOT);

		MATRIX	MatRef(MATRIX::IDENTITY);
		AddRefMatrix(MatRef);

		NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
		pNodeArray->UpdateAllMatrices(MatRef);

		UInt32	i, nNbrChildren;

		nNbrChildren = pNodeArray->GetNbrChildren();
		for (i = 0 ; i < nNbrChildren ; ++i)
		{
			NODE_CHILD *	pNodeChild = pNodeArray->GetChild(i);

			MatRef.InverseAffine(pNodeChild->GetGlobalMatrix());
			AddRefMatrix(MatRef);
			StoreSklChildIfSpecial(pNodeChild);
		}

		if	(_RagdollDataCI.HasHandle() && _RagdollDataCI.IsLoaded())
		{
			UInt32			i;
			RAGDOLL_DATA *	pRagdollData = static_cast<RAGDOLL_DATA *>(_RagdollDataCI.GetCommunicator());

			QDT_ASSERT(pRagdollData->GetNbrActors() == nNbrChildren);

			for	(i = 0 ; i < nNbrChildren ; ++i)
			{
				const RAGDOLL_ACTOR *	pRagdollActor = pRagdollData->GetActor(i);

				if	(pRagdollActor)
				{
					NODE_CHILD *	pNodeChild = pNodeArray->GetChild(i);

					pNodeChild->CreatePhysicMaterial();
					I_PHYSIC_DYNAMIC_ACTOR * pActor = pNodeChild->CreatePhysicDynamicActor();
					QDT_ASSERT(pActor);

					Float32	rMass = pRagdollActor->GetMass();
					if	(rMass > 0.0f)
					{
						pActor->SetMass(rMass);
					}
					else
					{
						pActor->SetDensity(pRagdollActor->GetDensity());
					}

					pActor->SetCenterOfMassLocalPosition(pRagdollActor->GetCenterOfMass());
					pActor->SetLinearDamping(pRagdollActor->GetLinearDamping());
					pActor->SetAngularDamping(pRagdollActor->GetAngularDamping());
					pActor->EnableKinematicMode(true);
					pActor->EnableGravity(pRagdollActor->IsInfluencedByGravity());
					pActor->SetSolverIterationCount(pRagdollActor->GetNbrSolverIterations());

					// Shapes
					UInt32	i, nNbrShapes = pRagdollActor->GetNbrShapesData();

					for	(i = 0 ; i < nNbrShapes ; ++i)
					{
						const PHYSIC_SHAPE_DATA *	pShapeData = pRagdollActor->GetShapeData(i);

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
				}
			}

			_nNbrRagdollJoints = pRagdollData->GetNbrConstraints();
			if	(_nNbrRagdollJoints)
			{
				_pRagdollJoints = new I_PHYSIC_JOINT_CUSTOM*[_nNbrRagdollJoints];

				for	(i = 0 ; i < _nNbrRagdollJoints ; ++i)
				{
					const RAGDOLL_CONSTRAINT &	pRagdollConstraint = pRagdollData->GetConstraint(i);

					I_PHYSIC_JOINT_CUSTOM *	pJoint = PHYSIC_CONTEXT::Instance()->CreateJointCustom();
					_pRagdollJoints[i] = pJoint;

 					NODE_CHILD *	pNodeChild1 = pNodeArray->GetChild(pRagdollConstraint.GetActor1Index());
					NODE_CHILD *	pNodeChild2 = pNodeArray->GetChild(pRagdollConstraint.GetActor2Index());

					pJoint->SetActor1(pNodeChild1->GetDynamicActor());
					pJoint->SetActor2(pNodeChild2->GetDynamicActor());
					pJoint->SetGlobalAnchor(pRagdollConstraint.GetGlobalAnchor());
					pJoint->SetGlobalAxis(pRagdollConstraint.GetGlobalAxis());
					pJoint->SetGlobalNormal(pRagdollConstraint.GetGlobalNormal());
					pJoint->EnableCollision(pRagdollConstraint.IsCollisionEnabled());
					pJoint->SetXMotionType(pRagdollConstraint.GetXMotionType());
					pJoint->SetYMotionType(pRagdollConstraint.GetYMotionType());
					pJoint->SetZMotionType(pRagdollConstraint.GetZMotionType());
					pJoint->SetSwing1MotionType(pRagdollConstraint.GetSwing1MotionType());
					pJoint->SetSwing2MotionType(pRagdollConstraint.GetSwing2MotionType());
					pJoint->SetTwistMotionType(pRagdollConstraint.GetTwistMotionType());
					pJoint->EnableProjection(pRagdollConstraint.IsProjectionEnabled());
					pJoint->SetProjectionDistance(pRagdollConstraint.GetProjectionDistance());
					pJoint->SetProjectionAngle(pRagdollConstraint.GetProjectionAngle());
					pJoint->SetLinearLimitValue(pRagdollConstraint.GetLinearLimitValue());
					pJoint->SetLinearLimitRestitution(pRagdollConstraint.GetLinearLimitRestitution());
					pJoint->SetLinearLimitSpring(pRagdollConstraint.GetLinearLimitSpring());
					pJoint->SetLinearLimitDamper(pRagdollConstraint.GetLinearLimitDamper());
					pJoint->SetSwing1LimitValue(pRagdollConstraint.GetSwing1LimitValue());
					pJoint->SetSwing1LimitRestitution(pRagdollConstraint.GetSwing1LimitRestitution());
					pJoint->SetSwing1LimitSpring(pRagdollConstraint.GetSwing1LimitSpring());
					pJoint->SetSwing1LimitDamper(pRagdollConstraint.GetSwing1LimitDamper());
					pJoint->SetSwing2LimitValue(pRagdollConstraint.GetSwing2LimitValue());
					pJoint->SetSwing2LimitRestitution(pRagdollConstraint.GetSwing2LimitRestitution());
					pJoint->SetSwing2LimitSpring(pRagdollConstraint.GetSwing2LimitSpring());
					pJoint->SetSwing2LimitDamper(pRagdollConstraint.GetSwing2LimitDamper());
					pJoint->SetTwistLowLimitValue(pRagdollConstraint.GetTwistLowLimitValue());
					pJoint->SetTwistLowLimitRestitution(pRagdollConstraint.GetTwistLowLimitRestitution());
					pJoint->SetTwistLowLimitSpring(pRagdollConstraint.GetTwistLowLimitSpring());
					pJoint->SetTwistLowLimitDamper(pRagdollConstraint.GetTwistLowLimitDamper());
					pJoint->SetTwistHighLimitValue(pRagdollConstraint.GetTwistHighLimitValue());
					pJoint->SetTwistHighLimitRestitution(pRagdollConstraint.GetTwistHighLimitRestitution());
					pJoint->SetTwistHighLimitSpring(pRagdollConstraint.GetTwistHighLimitSpring());
					pJoint->SetTwistHighLimitDamper(pRagdollConstraint.GetTwistHighLimitDamper());
//					pJoint->SetSwingDriveType(pRagdollConstraint.GetSwingDriveType());
//					pJoint->SetSwingDriveSpring(pRagdollConstraint.GetSwingDriveSpring());
//					pJoint->SetSwingDriveDamping(pRagdollConstraint.GetSwingDriveDamper());
//					pJoint->SetDriveOrientation(pRagdollConstraint.GetSwingDriveOrientation());
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	06-02-01:	FBO - Created
//-----------------------------------------------------------------------------
void	SKELETON::Clear()
{
	MESH::Clear();

	_vRefGlobalMatrices.Clear();
	memset(_SpecialSklJoints, 0, NBR_JOINT_TYPES * 4);

	delete	(_pHair);

	if	(_RagdollDataCI.HasHandle() && _RagdollDataCI.IsLoaded())
	{
		UInt32			i, nNbrChildren;
		NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());

		nNbrChildren = pNodeArray->GetNbrChildren();
		for	(i = 0 ; i < nNbrChildren ; ++i)
		{
			NODE_CHILD *	pNodeChild = pNodeArray->GetChild(i);

			pNodeChild->DeletePhysicMaterial();
			pNodeChild->DeletePhysicDynamicActor();
		}

		if	(_pRagdollJoints)
		{
			UInt32	i;

			for	(i = 0 ; i < _nNbrRagdollJoints ; ++i)
			{
				PHYSIC_CONTEXT::Instance()->ReleaseJoint(_pRagdollJoints[i]);
			}

			delete [] _pRagdollJoints;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComputeCameraDistance
//	Object: 
//	06-03-09:	RMA - Created
//-----------------------------------------------------------------------------
Float32	SKELETON::ComputeCameraDistance(UInt32 uiPrim)
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());
	const PRIMITIVE & Primitive = pMeshData->GetPrimitive(uiPrim);
	NODE_ARRAY *pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
	UInt32		j, nNbrJoints;
	VECTOR		Center(VECTOR::ZERO);
	VECTOR		CameraPos; 
	VECTOR		ViewDir;

	nNbrJoints = Primitive.GetNbrJoints();
	for	(j = 0 ; j < nNbrJoints ; ++j)
	{
		Center.Add(pNodeArray->GetChild(Primitive.GetJointIndex(j))->GetGlobalMatrix().GetTranslation());
	}
	Center /= nNbrJoints;

	DISPLAY_MANAGER::Instance()->GetActiveCamera()->GetNode()->GetGlobalPosition(CameraPos);
	ViewDir.Sub(Center, CameraPos);

	return (ViewDir.SquareLength());
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object: 
//	01-02-20:	RMA - Created
//-----------------------------------------------------------------------------
Bool SKELETON::TestVisibility(const CONE & Cone, 
							  UInt32 uiDisplayFlags)
{
	_bVisible = false;

	if	(_fDisplayFlags.GetFlags(uiDisplayFlags) ==  uiDisplayFlags && 
		_MeshDataCI.HasHandle() && 
		_MeshDataCI.IsLoaded() &&
		DISPLAY_MANAGER::Instance()->GetHideSkinClusters() == false)
	{
		NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
		UInt32			i, nNbrChildren = pNodeArray->GetNbrChildren();
		static float	rExtend = 15.0f;

		_GlobalBoundingBox.Clear();
		for (i = 0 ; i < nNbrChildren ; ++i)
		{		
			_GlobalBoundingBox.Grow(pNodeArray->GetChild(i)->GetGlobalMatrix().GetTranslation());
		}
		_GlobalBoundingBox.Extend(rExtend);

		SPHERE GlobalBoundingSphere(_GlobalBoundingBox.GetCenter(), _GlobalBoundingBox.GetRadius());

		_bVisible = GlobalBoundingSphere.Intersect(Cone);

		if	((_bVisible) && _MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
		{
			MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

			for	(i = 0 ; i < _nNbrPrimitives ; ++i)
			{
				QDT_FLAGS fDisplayFlags = pMeshData->GetDisplayFlags(i);

				if (fDisplayFlags.GetFlags(uiDisplayFlags) == uiDisplayFlags)
				{
					SendPrimitiveToDisplayList(i, true);
				}
			}
		}
	}

	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object: 
//	01-02-20:	RMA - Created
//-----------------------------------------------------------------------------
Bool SKELETON::TestVisibility(const FRUSTRUM & Frustum, 
							  UInt32 uiDisplayFlags)
{
	_bVisible = false;

	if	(_fDisplayFlags.GetFlags(uiDisplayFlags) ==  uiDisplayFlags && 
		_MeshDataCI.HasHandle() && 
		_MeshDataCI.IsLoaded() &&
		DISPLAY_MANAGER::Instance()->GetHideSkinClusters() == false)
	{
		NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());

		UInt32			i, nNbrChildren = pNodeArray->GetNbrChildren();
		static float	rExtend = 15.0f;

		_GlobalBoundingBox.Clear();
		for (i = 0 ; i < nNbrChildren ; ++i)
		{		
			_GlobalBoundingBox.Grow(pNodeArray->GetChild(i)->GetGlobalMatrix().GetTranslation());
		}
		_GlobalBoundingBox.Extend(rExtend);

		_bVisible = _GlobalBoundingBox.Intersect(Frustum.GetHexahedron());

		if	((_bVisible) && _MeshDataCI.HasHandle() && _MeshDataCI.IsLoaded())
		{
			MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

			for	(i = 0 ; i < _nNbrPrimitives ; ++i)
			{
				QDT_FLAGS fDisplayFlags = pMeshData->GetDisplayFlags(i);

				if (fDisplayFlags.GetFlags(uiDisplayFlags) == uiDisplayFlags)
				{
					SendPrimitiveToDisplayList(i);
				}
			}
		}
	}

	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::Display(UInt32	nPrimitive)
{
	MESH_DATA *	pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

	UInt32				j, nNbrJoints;
	NODE_ARRAY			*pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
	DISPLAY_MANAGER		*pDisplay = DISPLAY_MANAGER::Instance();
	CAMERA				*pCamera = pDisplay->GetActiveCamera();
	COM_INSTANCE		ciCurrentShader;
	QDT_VECTOR <VECTOR>	Points;
	SPHERE				GlobalSphere;

	VECTOR bodyPos;
	GetBodyPos(bodyPos);

	pDisplay->EnableSkinning(true);
	pDisplay->SetWorldMatrix(pNodeArray->GetGlobalMatrix());

	// Render primitive
	const PRIMITIVE & Primitive = pMeshData->GetPrimitive(nPrimitive);

	// Lights
	nNbrJoints = Primitive.GetNbrJoints();
	for	(j = 0 ; j < nNbrJoints ; ++j)
	{
		GlobalSphere.Grow(pNodeArray->GetChild(Primitive.GetJointIndex(j))->GetGlobalMatrix().GetTranslation());
	}
	FindLights(GlobalSphere);

	// Skinning matrices
	UInt32	nNbrWeights = 0;

	const MATRIX & invWorldMatrix = DISPLAY_MANAGER::Instance()->GetInvWorldMatrix();
	MATRIX	SkinningMatrix;

	pDisplay->SetNbrSkinningMatrices(nNbrJoints);
	for	(j = 0 ; j < nNbrJoints ; ++j)
	{
		UInt32	nJointIndex = Primitive.GetJointIndex(j);

		const MATRIX & Mat = pNodeArray->GetChild(nJointIndex)->GetGlobalMatrix();

		SkinningMatrix.Mul(_vRefGlobalMatrices[nJointIndex + 1], pNodeArray->GetChild(nJointIndex)->GetGlobalMatrix());
		SkinningMatrix.Mul(invWorldMatrix);
		pDisplay->SetSkinningMatrix(j, SkinningMatrix);

		++nNbrWeights;
	}

	for	(j = nNbrWeights ; j < 4 ; ++j)
	{
		pDisplay->SetSkinningMatrix(j, MATRIX::IDENTITY);
	}

	DrawPrimitive(nPrimitive, bodyPos);
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::Display(const QDT_VECTOR < UInt32 > &	vPrimitives)
{
	DISPLAY_MANAGER	*pDisplay = DISPLAY_MANAGER::Instance();

	if (pDisplay->GetDisplaySkeletons())
	{
		DisplaySkeleton();	
	}
	else
	{
		VECTOR bodyPos;
		GetBodyPos(bodyPos);

		MESH_DATA *		pMeshData = static_cast<MESH_DATA *>(_MeshDataCI.GetCommunicator());

		UInt32			i, j;
		NODE_ARRAY		*pNodeArray = static_cast<NODE_ARRAY *>(GetNode());

		CAMERA			*pCamera = pDisplay->GetActiveCamera();
		COM_INSTANCE	ciCurrentShader;
		UInt32			nNbrPrimitives = vPrimitives.GetSize();

		pDisplay->EnableSkinning(true);
		pDisplay->SetWorldMatrix(pNodeArray->GetGlobalMatrix());

		for	(i = 0 ; i < nNbrPrimitives ; ++i)
		{
			// Render primitive
			const PRIMITIVE & Primitive = pMeshData->GetPrimitive(vPrimitives[i]);

			// Lights
			SPHERE	GlobalSphere;
			UInt32	nNbrJoints = Primitive.GetNbrJoints();
			for	(j = 0 ; j < nNbrJoints ; ++j)
			{
				GlobalSphere.Grow(pNodeArray->GetChild(Primitive.GetJointIndex(j))->GetGlobalMatrix().GetTranslation());
			}
			FindLights(GlobalSphere);

			// Skinning matrices
			UInt32	nNbrWeights = 0;

			const MATRIX & invWorldMatrix = pDisplay->GetInvWorldMatrix();
			MATRIX	SkinningMatrix;

			pDisplay->SetNbrSkinningMatrices(nNbrJoints);
			for	(j = 0 ; j < nNbrJoints ; ++j)
			{
				UInt32	nJointIndex = Primitive.GetJointIndex(j);

				SkinningMatrix.Mul(_vRefGlobalMatrices[nJointIndex + 1], pNodeArray->GetChild(nJointIndex)->GetGlobalMatrix());
				SkinningMatrix.Mul(invWorldMatrix);
				pDisplay->SetSkinningMatrix(j, SkinningMatrix);

				++nNbrWeights;
			}

			for	(j = nNbrWeights ; j < 4 ; ++j)
			{
				pDisplay->SetSkinningMatrix(j, MATRIX::IDENTITY);
			}

			DrawPrimitive(vPrimitives[i], bodyPos);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayTriggerVolume
//	Object:		
//	03-01-17:	GGO - Created
//-----------------------------------------------------------------------------
void	SKELETON::DisplayTriggerVolume(const DISPLAY_PARAMETERS &	DisplayParameters)
{
	QDT_NOT_IMPLEMENTED();
	/*
	#ifndef _MASTER
	VECTOR		DirectionVector;
	SEGMENT		DirectionSegment;
	float		rSin, rCos, rAngle;
	AABB		Aabb;

	static const float	rDirLength = 100.0f;

	if ( DISPLAY_MANAGER::Instance()->DisplayTriggerVolumes() == true )
	{
	RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&MATRIX::IDENTITY);

	// Display the box.
	GetCharacterAabb( Aabb, DisplayParameters.GetNode() );
	Aabb.Display( COLOR_FLOAT( 1.0f, 0.0f, 1.0f, 0.0f ) );

	// Display the direction.
	rAngle = DisplayParameters.GetNode()->GetFrontDirAngle();
	MATH::GetCosinusAndSinus( rAngle, rCos, rSin );
	DirectionVector.SetXYZ( rDirLength * rSin, 0.0, rDirLength * rCos );
	DirectionSegment.SetPoint1( Aabb.GetCenter() );
	DirectionSegment.SetPoint2( Aabb.GetCenter() + DirectionVector );
	DirectionSegment.Display( COLOR_FLOAT( 1.0f, 0.0f, 1.0f, 0.0f ) );
	}
	#endif*/
}

//-----------------------------------------------------------------------------
// Name: SKELETON::AddRefMatrix
// Object: 
//01-01-24 CVI Created: 
//-----------------------------------------------------------------------------
void	SKELETON::AddRefMatrix(MATRIX& M)
{
	_vRefGlobalMatrices.PushTail(M);
}

//-----------------------------------------------------------------------------
// Name: SKELETON::GetInfluentObjectRefMatrix
// Object: 
//01-01-24 CVI Created: 
//-----------------------------------------------------------------------------
MATRIX&	SKELETON::GetInfluentObjectRefMatrix(int nIdxJoint)
{
	return (_vRefGlobalMatrices[nIdxJoint]);
}

//-----------------------------------------------------------------------------
//	Name:		StoreJointIfSpecial
//	Object:		Based on the static table declared both in the .h and filled 
//				in the constructor, joints INDEX_INTS ( from the dynamics ) 
//				are stored. this function is called from the life skeleton.
//				Aka when you instantiate a skeleton the table is filled
//	01-07-24:	GHP - Created
//-----------------------------------------------------------------------------
Bool	SKELETON::StoreJointIfSpecial(const COM_INSTANCE &	JointNodeInst,
									  JOINT_TYPE			Type)
{

	if (Type !=  JT_NONE)
	{
		_SpecialJoints[Type] = JointNodeInst; 
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		StoreSklChildIfSpecial
//	Object:		
//	03-07-18:	CVI - Created
//-----------------------------------------------------------------------------
Bool	SKELETON::StoreSklChildIfSpecial(NODE_CHILD *	pNodeChild)
{
	NODE_ARRAY * pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
	JOINT_TYPE	Type = pNodeChild->GetType();

	if	(Type != JT_NONE)
	{
		if	((Type == JT_HAIR) && pNodeArray->GetFlags().IsSet(NODE::RAGDOLL))
		{
			if	(_pHair == NULL)
			{
				_pHair = new HAIR;
			}
			_pHair->AddCell(pNodeChild);
		}
		else
		{
			if	((Type == JT_HEAD) && pNodeArray->GetFlags().IsSet(NODE::RAGDOLL))
			{
				if	(_pHair == NULL)
				{
					_pHair = new HAIR;
				}
				_pHair->SetHead(pNodeChild);

				// Create physic objects for HEAD
				I_PHYSIC_DYNAMIC_ACTOR * pDynamicActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();

				float	rRadius = 7.0f;
				float	rHeight;
				VECTOR	LocalPos(0.0f, 0.0f, 5.0f);
				VECTOR	FirstChildPos, Pos, V;

				Pos = pNodeChild->GetGlobalMatrix().GetTranslation();
				FirstChildPos = pNodeArray->GetChild(pNodeChild->GetChildIndex(0))->GetGlobalMatrix().GetTranslation();

				V.Sub(FirstChildPos, Pos);
				rHeight = V.Length();
				LocalPos.SetY(rHeight * 0.5f);

				I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
				pCapsule->SetRadius(rRadius);
				pCapsule->SetHeight(rHeight - 2.5f);
				pCapsule->SetLocalPosition(LocalPos);

				pDynamicActor->SetCollisionGroup(GROUP_JOINT);
				pDynamicActor->PushShape(pCapsule);
				pDynamicActor->SetMass(1000.0f);
				pDynamicActor->SetGlobalMatrix(pNodeChild->GetGlobalMatrix());
				pDynamicActor->SetLinearDamping(1.0f);
				pDynamicActor->SetAngularDamping(1.0f);
				_pSceneGraph->GetPhysicScene()->RegisterDynamicActor(pDynamicActor);

				pNodeChild->SetDynamicActor(pDynamicActor);
				pNodeChild->EnableKinematic(true);
			}

			_SpecialSklJoints[Type] = pNodeChild; 
			return (true);
		}
	}
	else
	{
		if	(pNodeArray->GetFlags().IsSet(NODE::RAGDOLL))
		{
			if	(pNodeChild->GetIndex() == 2)
			{
				// Create physic objects for Spine1
				I_PHYSIC_DYNAMIC_ACTOR * pDynamicActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();

				float		rRadius = 5.0f;
				float		rHeight;
				VECTOR		LocalPos(0.0f, 12.0f, 4.0f);
				VECTOR		FirstChildPos, Pos, V;
				QUATERNION	Rot, Rot2;
				MATRIX		M, M2;

				M.Identity();

				Pos = pNodeChild->GetGlobalMatrix().GetTranslation();
				FirstChildPos = pNodeArray->GetChild(pNodeChild->GetChildIndex(0))->GetGlobalMatrix().GetTranslation();

				V.Sub(FirstChildPos, Pos);
				rHeight = V.Length();

				Rot.SetVectRoll(VECTOR(1.0f, 0.0f, 0.0f), -MATH::PI() / 8.0f);

				M.SetQuaternion(Rot);
				M.SetTranslation(LocalPos);

				I_PHYSIC_SHAPE_BOX * pBox = PHYSIC_CONTEXT::Instance()->CreateShapeBox();
				pBox->SetDimensions(VECTOR(7.5f, (rHeight - (rRadius * 2.0f)) * 0.5f, rRadius));
				pBox->SetLocalMatrix(M);

				LocalPos = VECTOR(7.5f, LocalPos.GetY(), LocalPos.GetZ());
				M.SetTranslation(LocalPos);

				I_PHYSIC_SHAPE_CAPSULE * pCapsule1 = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
				pCapsule1->SetRadius(rRadius);
				pCapsule1->SetHeight(rHeight - (rRadius * 2.0f));
				pCapsule1->SetLocalMatrix(M);

				LocalPos = VECTOR(-7.5f, LocalPos.GetY(), LocalPos.GetZ());
				M.SetTranslation(LocalPos);

				I_PHYSIC_SHAPE_CAPSULE * pCapsule2 = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
				pCapsule2->SetRadius(rRadius);
				pCapsule2->SetHeight(rHeight - (rRadius * 2.0f));
				pCapsule2->SetLocalMatrix(M);

				LocalPos = VECTOR(0.0f, LocalPos.GetY() + ((rHeight - (rRadius * 2.0f)) * 0.5f), LocalPos.GetZ() - 2.0f);
				Rot2.SetVectRoll(VECTOR(0.0f, 0.0f, 1.0f), MATH::PI() * 0.5f);
				M.Identity();
				M.SetQuaternion(Rot);
				M.SetTranslation(LocalPos);
				M2.Identity();
				M2.SetQuaternion(Rot2);
				M2.Mul(M);

				I_PHYSIC_SHAPE_CAPSULE * pCapsule3 = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
				pCapsule3->SetRadius(rRadius);
				pCapsule3->SetHeight(15.0f);
				pCapsule3->SetLocalMatrix(M2);

				LocalPos = VECTOR(0.0f, LocalPos.GetY() - 2.0f, LocalPos.GetZ() - 5.5f);
				M.Identity();
				M.SetTranslation(LocalPos);

				I_PHYSIC_SHAPE_BOX * pBox1 = PHYSIC_CONTEXT::Instance()->CreateShapeBox();
				pBox1->SetDimensions(VECTOR(15.0f, 10.0f, rRadius));
				pBox1->SetLocalMatrix(M);

				pDynamicActor->SetCollisionGroup(GROUP_JOINT);
				pDynamicActor->PushShape(pBox);
				//pDynamicActor->PushShape(pBox1);
				pDynamicActor->PushShape(pCapsule1);
				pDynamicActor->PushShape(pCapsule2);
				pDynamicActor->PushShape(pCapsule3);
				pDynamicActor->SetMass(1000.0f);
				pDynamicActor->SetGlobalMatrix(pNodeChild->GetGlobalMatrix());
				pDynamicActor->SetLinearDamping(1.0f);
				pDynamicActor->SetAngularDamping(1.0f);
				_pSceneGraph->GetPhysicScene()->RegisterDynamicActor(pDynamicActor);

				pNodeChild->SetDynamicActor(pDynamicActor);
				pNodeChild->EnableKinematic(true);
			}

			if	(pNodeChild->GetIndex() == 25)
			{
				// Create physic objects for LeftShoulder
				I_PHYSIC_DYNAMIC_ACTOR * pDynamicActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();

				float		rRadius = 8.0f;
				float		rHeight;
				VECTOR		LocalPos(5.0f, -2.0f, -2.0f);
				VECTOR		FirstChildPos, Pos, V;
				QUATERNION	Rot;
				MATRIX		M;

				Pos = pNodeChild->GetGlobalMatrix().GetTranslation();
				FirstChildPos = pNodeArray->GetChild(pNodeChild->GetChildIndex(0))->GetGlobalMatrix().GetTranslation();

				V.Sub(FirstChildPos, Pos);
				rHeight = V.Length();

				Rot.SetVectRoll(VECTOR(0.0f, 0.0f, 1.0f), -MATH::PI() * 0.5f);

				M.SetQuaternion(Rot);
				M.SetTranslation(LocalPos);

				I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
				pCapsule->SetRadius(rRadius);
				pCapsule->SetHeight(rHeight);
				pCapsule->SetLocalMatrix(M);

				pDynamicActor->SetCollisionGroup(GROUP_JOINT);
				pDynamicActor->PushShape(pCapsule);
				pDynamicActor->SetMass(1000.0f);
				pDynamicActor->SetGlobalMatrix(pNodeChild->GetGlobalMatrix());
				pDynamicActor->SetLinearDamping(1.0f);
				pDynamicActor->SetAngularDamping(1.0f);
				_pSceneGraph->GetPhysicScene()->RegisterDynamicActor(pDynamicActor);

				pNodeChild->SetDynamicActor(pDynamicActor);
				pNodeChild->EnableKinematic(true);
			}

			if	(pNodeChild->GetIndex() == 3)
			{
				// Create physic objects for RightShoulder
				I_PHYSIC_DYNAMIC_ACTOR * pDynamicActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();

				float		rRadius = 8.0f;
				float		rHeight;
				VECTOR		LocalPos(-5.0f, -2.0f, -2.0f);
				VECTOR		FirstChildPos, Pos, V;
				QUATERNION	Rot;
				MATRIX		M;

				Pos = pNodeChild->GetGlobalMatrix().GetTranslation();
				FirstChildPos = pNodeArray->GetChild(pNodeChild->GetChildIndex(0))->GetGlobalMatrix().GetTranslation();

				V.Sub(FirstChildPos, Pos);
				rHeight = V.Length();

				Rot.SetVectRoll(VECTOR(0.0f, 0.0f, 1.0f), MATH::PI() * 0.5f);

				M.SetQuaternion(Rot);
				M.SetTranslation(LocalPos);

				I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
				pCapsule->SetRadius(rRadius);
				pCapsule->SetHeight(rHeight);
				pCapsule->SetLocalMatrix(M);

				pDynamicActor->SetCollisionGroup(GROUP_JOINT);
				pDynamicActor->PushShape(pCapsule);
				pDynamicActor->SetMass(1000.0f);
				pDynamicActor->SetGlobalMatrix(pNodeChild->GetGlobalMatrix());
				pDynamicActor->SetLinearDamping(1.0f);
				pDynamicActor->SetAngularDamping(1.0f);
				_pSceneGraph->GetPhysicScene()->RegisterDynamicActor(pDynamicActor);

				pNodeChild->SetDynamicActor(pDynamicActor);
				pNodeChild->EnableKinematic(true);
			}
		}
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetSpecialJoint
//	Object:		
//	02-03-01:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	SKELETON::GetSpecialJoint(JOINT::TYPE	SpecialJointType)
{
	//	This method is obsolete
	//	Use instead : int nIndex = GetSpecialSklChild( type )
	//					pSklNode->GetChild...( nIndex )
	QDT_FAIL();

	if (!_SpecialJoints[SpecialJointType].HasHandle())
	{
		QDT_Message(QDT_COMMENT("Couldn't find special joint"));
	}

	return (_SpecialJoints[SpecialJointType]);
}

//-----------------------------------------------------------------------------
//	Name:		GetSpecialNodeChild
//	Object:		
//	03-07-18:	CVI - Created
//-----------------------------------------------------------------------------
NODE_CHILD *	SKELETON::GetSpecialNodeChild(int	Type)
{
	if (Type < NBR_JOINT_TYPES)
	{
		return	(_SpecialSklJoints[Type]);
	}
	else
	{
		return	(NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSpecialBoneIndex
//	Object:		
//	06-01-06:	ELE - Created
//-----------------------------------------------------------------------------
int	SKELETON::GetSpecialBoneIndex(int	nType)
{
	NODE_CHILD *	pChild;

	pChild = GetSpecialNodeChild(nType);

	if	(pChild)
	{
		return	(pChild->GetIndex());
	}

	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		DisplaySkeleton
//	Object:		
//	06-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void	SKELETON::DisplaySkeleton()
{
#ifdef PLATFORM_PC
	DISPLAY_MANAGER * pDisplay = DISPLAY_MANAGER::Instance();

	const UInt32 JOINT_COLOR = 0xFFFFFF00;
	const UInt32 AXE_X_COLOR = 0xFFFF0000;
	const UInt32 AXE_Y_COLOR = 0xFF00FF00;
	const UInt32 AXE_Z_COLOR = 0xFF0000FF;
	const Float32 AXIS_SCALE = 2.0f;

	NODE_ARRAY * pNodeArray = static_cast<NODE_ARRAY *>(GetNode());
	CAMERA * pCamera = pDisplay->GetActiveCamera();

	const MATRIX &mViewMatrix = pCamera->GetViewMatrix();
	const MATRIX &mProjMatrix = pCamera->GetProjectionMatrix();

	UInt32 uiJoint, uiNbrJoints = pNodeArray->GetNbrChildren();
	const MATRIX & invWorldMatrix = pDisplay->GetInvWorldMatrix();

	DEBUG_VERTEX Vertices[4096]; // Max : 512 joints!!
	UInt32 uiCurrentLine = 0;

	for (uiJoint = 0 ; uiJoint < uiNbrJoints ; ++uiJoint)
	{
		Int32 iFatherJoint = pNodeArray->GetChild(uiJoint)->GetFatherIndex();

		const MATRIX & Matrix = pNodeArray->GetChild(uiJoint)->GetGlobalMatrix();
		const VECTOR & vJoint = pNodeArray->GetChild(uiJoint)->GetGlobalMatrix().GetTranslation();

		VECTOR vAxeX(VECTOR::UNIT_X);
		VECTOR vAxeY(VECTOR::UNIT_Y);
		VECTOR vAxeZ(VECTOR::UNIT_Z);

		Matrix.TransformDirection(vAxeX);
		Matrix.TransformDirection(vAxeY);
		Matrix.TransformDirection(vAxeZ);

		vAxeX.Mul(AXIS_SCALE);
		vAxeY.Mul(AXIS_SCALE);
		vAxeZ.Mul(AXIS_SCALE);

		vAxeX.Add(vJoint);
		vAxeY.Add(vJoint);
		vAxeZ.Add(vJoint);

		// Display axe X
		Vertices[uiCurrentLine]._rX = vJoint.GetX();
		Vertices[uiCurrentLine]._rY = vJoint.GetY();
		Vertices[uiCurrentLine]._rZ = vJoint.GetZ();
		Vertices[uiCurrentLine++]._nDiffuse = AXE_X_COLOR;

		Vertices[uiCurrentLine]._rX = vAxeX.GetX();
		Vertices[uiCurrentLine]._rY = vAxeX.GetY();
		Vertices[uiCurrentLine]._rZ = vAxeX.GetZ();
		Vertices[uiCurrentLine++]._nDiffuse = AXE_X_COLOR;

		// Display axe Y
		Vertices[uiCurrentLine]._rX = vJoint.GetX();
		Vertices[uiCurrentLine]._rY = vJoint.GetY();
		Vertices[uiCurrentLine]._rZ = vJoint.GetZ();
		Vertices[uiCurrentLine++]._nDiffuse = AXE_Y_COLOR;

		Vertices[uiCurrentLine]._rX = vAxeY.GetX();
		Vertices[uiCurrentLine]._rY = vAxeY.GetY();
		Vertices[uiCurrentLine]._rZ = vAxeY.GetZ();
		Vertices[uiCurrentLine++]._nDiffuse = AXE_Y_COLOR;

		// Display axe Z
		Vertices[uiCurrentLine]._rX = vJoint.GetX();
		Vertices[uiCurrentLine]._rY = vJoint.GetY();
		Vertices[uiCurrentLine]._rZ = vJoint.GetZ();
		Vertices[uiCurrentLine++]._nDiffuse = AXE_Z_COLOR;

		Vertices[uiCurrentLine]._rX = vAxeZ.GetX();
		Vertices[uiCurrentLine]._rY = vAxeZ.GetY();
		Vertices[uiCurrentLine]._rZ = vAxeZ.GetZ();
		Vertices[uiCurrentLine++]._nDiffuse = AXE_Z_COLOR;

		// Display relation between two joints!
		const VECTOR & vFatherJoint = pNodeArray->GetChild(iFatherJoint)->GetGlobalMatrix().GetTranslation();
		QDT_ASSERT(iFatherJoint < (Int32)uiNbrJoints);

		if (iFatherJoint != -1)
		{
			Vertices[uiCurrentLine]._rX = vJoint.GetX();
			Vertices[uiCurrentLine]._rY = vJoint.GetY();
			Vertices[uiCurrentLine]._rZ = vJoint.GetZ();
			Vertices[uiCurrentLine++]._nDiffuse = JOINT_COLOR;

			Vertices[uiCurrentLine]._rX = vFatherJoint.GetX();
			Vertices[uiCurrentLine]._rY = vFatherJoint.GetY();
			Vertices[uiCurrentLine]._rZ = vFatherJoint.GetZ();
			Vertices[uiCurrentLine++]._nDiffuse = JOINT_COLOR;
		}
	}	

	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
	RENDER_CONTEXT::DrawLines(uiCurrentLine, Vertices, mViewMatrix, mProjMatrix);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DisplayBoundingBoxes
//	Object:		
//	02-03-18:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::DisplayBoundingBoxes(DISPLAY_PARAMETERS &	DisplayParameters) const
{
	QDT_NOT_IMPLEMENTED();
	/*
	#ifndef _MASTER
	if (DISPLAY_MANAGER::Instance()->DisplayBoundingBoxes() ||
	DISPLAY_MANAGER::Instance()->DisplaySkeletonBoundingVolumes())
	{
	if (_SkinInst.HasHandle())
	{
	SKIN *pSkin = static_cast<SKIN *>(_SkinInst.GetCommunicator());
	SKIN_CLUSTER *pSC = NULL;
	MESH *pMesh = NULL;
	int nNbrSC = pSkin->GetNbrSkinClusters();
	int nEachSC;

	for (nEachSC = 0 ; nEachSC < nNbrSC ; nEachSC++)
	{
	pSC = pSkin->GetSkinCluster(nEachSC);
	QDT_ASSERT(pSC);

	pMesh = pSC->GetMesh();

	pMesh->DisplayBoundingBox(DisplayParameters);
	}
	}

	RENDER_MANAGER::Instance()->GetRenderContext()->SetWorldTransform0(&MATRIX::IDENTITY);
	_GlobalBoundingBox.Display(COLOR_FLOAT(1.0f, 0.0f, 1.0f, 0.0f));
	}
	#endif*/
}

//-----------------------------------------------------------------------------
//	Name:		GetCharacterObb
//	Object:		The OBB is build according the character position and is axis
//				aligned.
//	03-01-17:	GGO - Created
//-----------------------------------------------------------------------------
void	SKELETON::GetCharacterAabb(AABB &	Aabb)
{
	const float	CHARACTER_HEIGHT = 180.0f;
	float		rXMin, rXMax, rYMin, rYMax, rZMin, rZMax;
	Bool		bHasHead = GetSpecialNodeChild(JT_HEAD) != NULL;
	NODE		*pEntityNode = GetNode();

	if (bHasHead)
	{
		VECTOR	HeadPos;
		VECTOR GlobalPos;
		HeadPos = GetSpecialNodeChild(JT_HEAD)->GetGlobalMatrix().GetTranslation();
		pEntityNode->GetGlobalPosition(GlobalPos);

		rYMax = HeadPos.GetY() + 25.0f;
		rYMin = GlobalPos.GetY();

		if (rYMax < rYMin)
		{
			// Si le perso a la tête en bas
			MATH::Swap(rYMin, rYMax);
		}
	}
	else
	{
		rYMax = CHARACTER_HEIGHT;
		rYMin =  0.0f;
	}

	rXMin = -30.0f;
	rXMax =  30.0f;
	rZMin = -30.0f;
	rZMax =  30.0f;

	if ( pEntityNode != NULL )
	{
		VECTOR	GlobalPos;
		pEntityNode->GetGlobalPosition(GlobalPos);

		rXMin += GlobalPos.GetX();
		rXMax += GlobalPos.GetX();
		rZMin += GlobalPos.GetZ();
		rZMax += GlobalPos.GetZ();

		if (bHasHead == false)
		{
			rYMin += GlobalPos.GetY();
			rYMax += GlobalPos.GetY();
		}
	}

	QDT_ASSERT( rXMin <= rXMax );
	QDT_ASSERT( rYMin <= rYMax );
	QDT_ASSERT( rZMin <= rZMax );

	Aabb.SetXYZ( rXMin, rXMax, rYMin, rYMax, rZMin, rZMax );
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SKELETON", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("SKELETON", 2, CM_ReadChunk2);
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("SKELETON", 2, CM_WriteChunk2);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	SKELETON::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	SKELETON::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("SKELETON"),
		MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON,
		QDT_COMMENT("889F412C-4CA1-FAEF-2E01-CAD582352283"),
		sizeof(SKELETON),
		ComInit,
		QDT_COMMENT("Class for skeletons"),
		MESH::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::EM_Construct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	SKELETON	*pSkeleton;
	pSkeleton = static_cast<SKELETON *>(pO);

	QDT_NEW_PLACED(pSkeleton, SKELETON)();
	QDT_ASSERT(pSkeleton);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::EM_Destruct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	SKELETON	*pSkeleton;
	pSkeleton = static_cast<SKELETON *>(pO);

	QDT_DELETE_PLACED(pSkeleton, SKELETON);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::CM_ReadChunk1(IO_STREAM &		Stream,
							   COMMUNICATOR *	pCommunicator)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	SKELETON::CM_ReadChunk2(IO_STREAM &		Stream,
								COMMUNICATOR *	pCommunicator)
{
	SKELETON	*pSkeleton;
	pSkeleton = static_cast<SKELETON *>(pCommunicator);

	Stream >> pSkeleton->_RagdollDataCI;
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk2
//	Object:		
//	03-03-25:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	SKELETON::CM_WriteChunk2(IO_STREAM &		Stream,
								 COMMUNICATOR *	pCommunicator)
{
	SKELETON	*pSkeleton;
	pSkeleton = static_cast<SKELETON *>(pCommunicator);

	Stream.DebugOutput("Skeleton :\t");
	Stream.DebugOutput(pSkeleton->GetName());
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");

	Stream << pSkeleton->_RagdollDataCI;
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
