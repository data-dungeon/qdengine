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
// CLASS: NODE
//
//
//	00-10-31	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(SceneGraph/Node/NodeChild)
#include	INCL_3DENGINE(SceneGraph/Node/NodeManager)
#include	INCL_3DENGINE(SceneGraph/Entities/Particles/ParticleEmitter)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/MeshGroup/MeshGroup)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Skeleton)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Bone/Bone)
#include	INCL_3DENGINE(SceneGraph/RayCaster/RayCastParameters)
#include	INCL_3DENGINE(Behavior/BehaviorManager)
#include	INCL_3DENGINE(Behavior/Controllers/Container/Container)
#include	INCL_3DENGINE(Behavior\Controllers\Entity Controllers\EntityControllerABC)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Hair/Hair)
#include	INCL_KCOM(CommunicatorType/communicatorContainerDescriptor)
#include	INCL_KCOM(Services/DatabaseService)
#include	INCL_KCOM(ComplexType/TypeVector)
#include	INCL_KCOM(ComplexType/TypeQuaternion)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(Catalog/Catalog)
#include	INCL_KCORE(Profiler/Profiler)
#include	INCL_KCORE(Stream/FileStream)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KPHYSICS(Physics/IPhysicJointFixed)
#include	INCL_KPHYSICS(Physics/IPhysicShapeCapsule)
#include	INCL_KPHYSICS(Physics/IPhysicController)
#include	INCL_KPHYSICS(Physics/IPhysicControllerManager)

#ifdef _DEBUG
	#include	"Node.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

const float		NODE::LIGHT_INTERPOLATION_DELAY = 500.0f; // 1s

//-----------------------------------------------------------------------------
// Name: NODE constructor
// Object: Constructor
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
NODE::NODE()
:
COMMUNICATOR		(GetGenericCommunicatorDescriptor()),
_Flags				(UNTRANSFORMED | GLOBAL_MATRIX_VALID),
_ParentInst			(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_FirstChildInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_BrotherInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_pContainer 		(NULL),
_pController		(NULL),
_pDynamicActor		(NULL),
_pPhysicController	(NULL),
_nParentedIndex		(-1),
_bSklPhysicJoint	(false),
_pAttachJoint		(NULL),
_LMatrix			(MATRIX::IDENTITY),
_GMatrix			(MATRIX::IDENTITY)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		NODE constructor
//	Object:		
//	03-07-16:	CVI - Created
//-----------------------------------------------------------------------------
NODE::NODE(CLASS_DESCRIPTOR * const	pCD)
:
COMMUNICATOR		(pCD),
_Flags				(UNTRANSFORMED | GLOBAL_MATRIX_VALID),
_ParentInst			(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_FirstChildInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_BrotherInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_pContainer 		(NULL),
_pController		(NULL),
_pDynamicActor		(NULL),
_pPhysicController	(NULL),
_nParentedIndex		(-1),
_bSklPhysicJoint	(false),
_pAttachJoint		(NULL)
{
	_LMatrix.Identity();
	_GMatrix.Identity();
}

//-----------------------------------------------------------------------------
// Name: NODE destructor
// Object: Destructor
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
NODE::~NODE()
{
	RemoveContainer();

	if	(GetFlags().IsSet(PHYSIC_CONTROLLER) ||
		 GetFlags().IsSet(RAGDOLL))
	{
		PHYSIC_MANAGER::Instance()->RemoveNodeWithPhysics(this);

		if	(_pPhysicController)
		{
			PHYSIC_CONTEXT::Instance()->GetControllerManager()->ReleaseController(_pPhysicController);
			_pPhysicController = NULL;
		}
	}
	else
	{
		UnregisterDynamicActor(NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetGroundPoints
//	Object:		
//	03-11-06:	JPE - Created
//-----------------------------------------------------------------------------
VECTOR *NODE::GetGroundPoints( int & nOutNbPoints )
{
	static VECTOR	PosReturn;				// <-- Si quelqu'un a une meilleure idée, je suis preneur :o)
	GetGlobalPosition(PosReturn);
	nOutNbPoints = 1;
	return &PosReturn;
}
/*
//-----------------------------------------------------------------------------
//	Name:		BreakConnectionsRec
//	Object:		
//	03-04-23:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::BreakConnectionsRec()
{
	if ( IsEntityNode() == false )
	{
		while (_vChildInst.GetSize())
		{
			if (GetChildInst(0).HasHandle())
			{
				if	(GetChild(0)->GetParentNode() == NULL)
				{
					QDT_FAIL();
					_vChildInst.RemoveIndex(0);
				}
				else
				{
					GetChild(0)->BreakConnectionsRec();
				}
			}
		}
	}

	BreakConnections();
}

//-----------------------------------------------------------------------------
//	Name:		BuildNodeArrayRec
//	Object:		
//	03-12-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::BuildNodeArrayRec(QDT_VECTOR < COM_INSTANCE >	& vNodeArray)
{
	vNodeArray.PushTail(COM_INSTANCE(GetHandle()));

	unsigned int nChild;
	for (nChild = 0 ; nChild < GetNbrChildren() ; ++nChild)
	{
		GetChild(nChild)->BuildNodeArrayRec(vNodeArray);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BreakConnections
//	Object:		
//	03-04-23:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::BreakConnections()
{
	// RMA : This line makes a strange bug, i'll see why later
	//_EntityInst = COM_INSTANCE::GetNullComInstance();
		
	// RMA : DetachFromFather devrait éviter un bug!
	// Sinon, si on met le parent à null alors que le breakConnectionsRec part
	// d'un fils, on laisse une grave incohérence dans le graphe de scène
	DetachFromFather();
//	_ParentInst = COM_INSTANCE::GetNullComInstance();
//	_vChildInst.Clear();

	//	Added by CVI : 28/10/03
	// RemoveContainer();
}
*/
//-----------------------------------------------------------------------------
//	Name:		RemoveContainer
//	Object:		
//	02-12-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::RemoveContainer()
{
	if (_pController) 
	{
		BEHAVIOR_MANAGER::Instance()->RemoveNodeWithController(this);
		delete (_pController);
		_pController = NULL;

		if	(GetParentNode())
		{
			GetParentNode()->UnsetControllerOnChild();
		}
	}

	//	Added by CVI 28/10/03
	if ( _pContainer )
	{
		//	Purge all animations
		COM_INSTANCE ThisCI(this); // to avoid node destruction while cleaning animated node vector (decreasereference)
		PurgeAllAnimConnections();		
		delete _pContainer;
		_pContainer = NULL;
	}
	
}

//-----------------------------------------------------------------------------
//	Name:		NODE::UpdateLocalMatrix
//	Object:		This method update the local matrix of a node
//	01-02-05:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::UpdateLocalMatrix()
{
	MATRIX	InvContMat;
	MATRIX	InvParentMat;
	MATRIX	TempMat;

	if	(_ParentInst.HasHandle() && _ParentInst.IsLoaded())
	{
		InvParentMat.InverseAffine(GetParentGlobalMatrix());
	}
	else
	{
		InvParentMat.Identity();
	}

	if	(_pContainer)
	{
		InvContMat.InverseAffine(_pContainer->GetMatrix()) ;
	}
	else
	{
		InvContMat.Identity();
	}

	TempMat.MakeAffine();
	TempMat.MulAffine(_GMatrix, InvParentMat);
	_LMatrix.MulAffine(InvContMat, TempMat);
	QDT_ASSERT(_LMatrix.IsAffine());

	_Flags.Set(GLOBAL_MATRIX_VALID);

	if	(GetParentNode())
	{
		GetParentNode()->UnsetGlobalMatrixInvalidOnChild();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetIndexedGlobalMatrix
//	Object:		
//	03-09-23:	CVI - Created
//-----------------------------------------------------------------------------
const MATRIX &	NODE::GetIndexedGlobalMatrix(int	nNum) const
{
	return ( _GMatrix );
}

//-----------------------------------------------------------------------------
//	Name:		NODE::UpdateGlobalMatrix
//	Object:		This method update the global matrix of a node
//	01-02-05:	RMA - Created
//-----------------------------------------------------------------------------
void NODE::UpdateGlobalMatrix()
{
	CONTAINER * pContainer = GetContainer();

	if (_ParentInst.HasHandle() && _ParentInst.IsLoaded())
	{
		if	(pContainer)
		{
			pContainer->UpdateFromMainContainer();
			MATRIX Temp;
			Temp.MakeAffine();		
			Temp.MulAffine(_LMatrix, GetParentGlobalMatrix());
			_GMatrix.MulAffine(pContainer->GetMatrix(), Temp);
		}
		else
		{
			_GMatrix.MulAffine(_LMatrix, GetParentGlobalMatrix());
		}
	}
	else
	{	
		if (pContainer)
		{
			pContainer->UpdateFromMainContainer();
		 	_GMatrix.MulAffine(pContainer->GetMatrix(), _LMatrix);						
		}
		else
		{
			_GMatrix = _LMatrix;
		}
	}

	_Flags.Set(GLOBAL_MATRIX_VALID);
	_Flags.Unset(INSIDE_TREE_VALID);

	QDT_ASSERT(_GMatrix.IsAffine());
}

//-----------------------------------------------------------------------------
//	Name:		UpdateChildrenGlobalMatrix
//	Object:		
//	05-04-04:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UpdateChildrenGlobalMatrix()
{
	NODE *	pChild = GetFirstChild();
	while	(pChild)
	{
		pChild->UpdateGlobalMatrix();
		pChild->UpdateChildrenGlobalMatrix();

		pChild = pChild->GetBrother();
	}
}

/*
//-----------------------------------------------------------------------------
//	Name:		RegisterEntities
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::RegisterEntities()
{
	int	i, nNbrChildren;

	const COM_INSTANCE & SGNodeInst = GetSceneGraphNodeInst();
	
	if	(SGNodeInst.HasHandle())
	{
		NODE * pSGNode = static_cast<NODE *>(SGNodeInst.GetCommunicator());
		
		if	(_EntityInst.HasHandle())
		{
		switch (_EntityInst.GetType())
		{
		default:
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT :
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT :
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT :	
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					LIGHT_ABC	*pLight = static_cast<LIGHT_ABC *>(GetEntity());
				
					pSG->AddLight(this);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER :
			{	
				if (!DATABASE_SERVICE::Instance()->GetMayaExport())
				{
					QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
					PARTICLE_EMITTER *	pPE			= static_cast<PARTICLE_EMITTER *>(GetEntity());
					if (pSGNode->GetEntityInst().IsLoaded())
					{
						SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						pSG->AddParticleEmitter(this);
					}
					
					if ((DATABASE_SERVICE::Instance()->IsEnableDataBank() == false) ||
						(DATA_BANK::IsExporting()))
					{
						if (pPE->IsActive())
						{
							UpdateParticleEmitter(pPE->GetParticleShape()->GetLifeSpan());
						}
					}

					NODE_MANAGER::Instance()->AddNode(COM_INSTANCE(this), MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MESH :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddCollisionMeshNode(this);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_DATA:
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				SCENE_GRAPH * pSG = NULL;
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddMeshNode(this);
				}
			
				COM_INSTANCE	MeshInst = COM_INSTANCE(MESH::GetGenericCommunicatorDescriptor()->Create());
				MESH * pMesh = static_cast<MESH *>(MeshInst.GetCommunicator());

				pMesh->Init(GetEntityInst());

				SetEntityInst(MeshInst);

				// Physic : static actor
				I_PHYSIC_STATIC_ACTOR * pStaticActor = pMesh->GetStaticActor();
				if	(pStaticActor)
				{
					const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
					if (ciSGNode.HasHandle())
					{
						NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
						SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());

						pSG->GetPhysicScene()->RegisterStaticActor(pStaticActor);
//						pMeshGroup->SetPhysicMaterial(pSG->GetPhysicScene()->GetDefaultMaterial());
					}
				}
				else
				{
					// Physic : dynamic actor
					if	(GetFlags().IsSet(PHYSIC_CONTROLLER))
					{
						if	(_pPhysicController == NULL)
						{
							CreatePhysicController(PC_TYPE_BOX);
						}
					}
					else
					{
						if (_pDynamicActor == NULL)
						{
							CreatePhysicDynamicActor();
						}
					}
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH:
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE:
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SOFT_BODY:
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				SCENE_GRAPH * pSG = NULL;
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddMeshNode(this);
				}
				
				if (DATA_BANK::IsExporting() == false)
				{
					MESH	*pMesh = static_cast<MESH *>(GetEntity());
					
					if	(pMesh->IsSoftBody())
					{
						PHYSIC_MANAGER::Instance()->BuildCloth(COM_INSTANCE(this), 1);
					}

					if (!DATABASE_SERVICE::Instance()->GetMayaExport())
					{
						if	(_EntityInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE)
						{
							DISPLAY_MANAGER::Instance()->AddBlendShape(COM_INSTANCE(pMesh));
						}
					}

					if	(GetFlags().IsSet(PHYSIC_CONTROLLER))
					{
						if	(_pPhysicController == NULL)
						{
							CreatePhysicController(PC_TYPE_BOX);
						}
					}
					else
					{
						if (_pDynamicActor == NULL)
						{
							CreatePhysicDynamicActor();
						}
					}
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON :
			{	
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				SKELETON	*pSkeleton = static_cast<SKELETON *>(GetEntity());	
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					
					pSG->AddSkeletonNode(this);

					if	(GetFlags().IsSet(PHYSIC_CONTROLLER))
					{
						if	(_pPhysicController == NULL)
						{
							CreatePhysicController(PC_TYPE_BOX);

//							CreatePhysicDynamicActor();
//							_pDynamicActor->EnableKinematicMode(true);
						}
					}
				}
			}

			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_GROUP :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddMeshGroupNode(this);
				}

					const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
					if (ciSGNode.HasHandle())
					{
						NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
						SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						MESH_GROUP	*pMeshGroup = static_cast<MESH_GROUP *>(GetEntity());	
						if(pMeshGroup->GetStaticActor())
						{
							pSG->GetPhysicScene()->RegisterStaticActor(pMeshGroup->GetStaticActor());
	//						pMeshGroup->SetPhysicMaterial(pSG->GetPhysicScene()->GetDefaultMaterial());
						}
					}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_GLOW :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddGlowNode(this);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PRIMITIVE_SET :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddPrimitiveNode(this);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TRIGGER :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddTriggerNode(this);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_WAY_POINT :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddWaypointNode(this);
				}
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIFE_FIELD :
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_UNIFORM_FIELD :
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_VORTEX_FIELD :
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RADIAL_FIELD :
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PERTURBATION_FIELD :
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				if (pSGNode->GetEntityInst().IsLoaded())
				{
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->AddForceFieldNode(this);
				}
			}
			break;
		}
	}
	}

	if	(GetFlags().IsSet(PHYSIC_CONTROLLER))
	{
		if	(_pPhysicController)
		{
			const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
			if (ciSGNode.HasHandle())
			{
				NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
				SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
				PHYSIC_CONTEXT::Instance()->GetControllerManager()->RegisterController(pSG->GetPhysicScene(), _pPhysicController);
			}

			BEHAVIOR_MANAGER::Instance()->AddNodeWithPhysics(this);
		}
	}
	else
	{
		if	(GetFlags().IsSet(RAGDOLL))
		{
			BEHAVIOR_MANAGER::Instance()->AddNodeWithPhysics(this);
		}
		else
		{
			if (_pDynamicActor)
			{
				const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
				if (ciSGNode.HasHandle())
				{
					NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
					SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->GetPhysicScene()->RegisterDynamicActor(_pDynamicActor);
				}

				BEHAVIOR_MANAGER::Instance()->AddNodeWithPhysics(this);
			}
		}
	}
	
	nNbrChildren = GetNbrChildren();
	for	(i = 0 ; i < nNbrChildren ; ++i)
	{
		GetChild(i)->RegisterEntities();
	}
}

//-----------------------------------------------------------------------------
//	Name:		UnRegisterEntities
//	Object:		
//	03-08-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UnRegisterEntities()
{
	if (_EntityInst.HasHandle())
	{	
		const COM_INSTANCE & SGNodeInst = GetSceneGraphNodeInst();
		
		if (SGNodeInst.HasHandle())
		{
			NODE *pSGNode = static_cast<NODE *>(SGNodeInst.GetCommunicator());

			if	(_EntityInst.HasHandle())
			{
			switch (_EntityInst.GetType())
			{
			default:
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SPOT_LIGHT :
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_POINT_LIGHT :
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DIRECTIONAL_LIGHT :
				{
					if (_EntityInst.IsLoaded() && pSGNode->_EntityInst.IsLoaded())
					{
						SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());

						LIGHT_ABC	*pLight = static_cast<LIGHT_ABC *>(GetEntity());	
						pSG->RemoveLight(this);
					}
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER :
				{
					if (pSGNode->_EntityInst.IsLoaded())
					{
						SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						pSG->RemoveParticleEmitter(this);
					}
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_MESH :
				{
					if (pSGNode->_EntityInst.IsLoaded())
					{
						SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						pSG->RemoveCollisionMeshNode(this);
					}
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH:
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE:
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SOFT_BODY:
				{
					if (_EntityInst.IsLoaded())
					{
						MESH * pMesh = static_cast<MESH *>(GetEntity());			
						if	(pMesh->IsSoftBody())
						{
							PHYSIC_MANAGER::Instance()->RemoveCloth(GetEntityInst());
						}

						if	(_EntityInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE)
						{
							DISPLAY_MANAGER::Instance()->RemoveBlendShape(COM_INSTANCE(pMesh));
						}
					}

					if (pSGNode->_EntityInst.IsLoaded())
					{
						SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						pSG->RemoveMeshNode(this);
					}
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_DATA:
			{
				QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
				SCENE_GRAPH * pSG = NULL;		
			
				COM_INSTANCE	MeshInst = COM_INSTANCE(MESH::GetGenericCommunicatorDescriptor()->Create());
				MESH * pMesh = static_cast<MESH *>(MeshInst.GetCommunicator());

				// Physic : static actor
				I_PHYSIC_STATIC_ACTOR * pStaticActor = pMesh->GetStaticActor();
				if	(pStaticActor)
				{
					const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
					if (ciSGNode.HasHandle())
					{
						NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
						SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());

						pSG->GetPhysicScene()->UnregisterStaticActor(pStaticActor);
					}
				}

				if (pSGNode->GetEntityInst().IsLoaded())
				{
					pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->RemoveMeshNode(this);
				}
			}
			break;


			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_GROUP :
				if (pSGNode->_EntityInst.IsLoaded())
				{
					SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->RemoveMeshGroupNode(this);

						const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
						if (ciSGNode.HasHandle())
						{
							NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
							SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
							MESH_GROUP	*pMeshGroup = static_cast<MESH_GROUP *>(GetEntity());	
							if(pMeshGroup->GetStaticActor())
							{
								pSG->GetPhysicScene()->UnregisterStaticActor(pMeshGroup->GetStaticActor());
							}						
						}
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKELETON :
				{
					if (pSGNode->_EntityInst.IsLoaded())
					{
						SCENE_GRAPH	*pSG		= static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						SKELETON	*pSkeleton	= static_cast<SKELETON *>(GetEntity());

						pSG->RemoveSkeletonNode(this);
					}
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_GLOW :
				if (pSGNode->_EntityInst.IsLoaded())
				{
					SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->RemoveGlowNode(this);
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PRIMITIVE_SET :
				if (pSGNode->_EntityInst.IsLoaded())
				{
					SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->RemovePrimitiveNode(this);
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_TRIGGER :
				if (pSGNode->_EntityInst.IsLoaded())
				{
					SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->RemoveTriggerNode(this);
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_WAY_POINT :
				if (pSGNode->_EntityInst.IsLoaded())
				{
					SCENE_GRAPH *pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
					pSG->RemoveWaypointNode(this);
				}
				break;

			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIFE_FIELD :
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_UNIFORM_FIELD :
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_VORTEX_FIELD :
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RADIAL_FIELD :
			case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PERTURBATION_FIELD :
				{
					if (pSGNode->_EntityInst.IsLoaded())
					{
						QDT_ASSERT(pSGNode->_EntityInst.HasHandle());
						SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						pSG->RemoveForceFieldNode(this);
					}
				}
				break;
			}
		}
		}

		if (_EntityInst.HasHandle() && _EntityInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER)
		{
			NODE_MANAGER::Instance()->RemoveNode(COM_INSTANCE(this), MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER);
		}

		if	(GetFlags().IsSet(PHYSIC_CONTROLLER))
		{
			DeletePhysicController();
		}
		else
		{
			if	(GetFlags().IsSet(RAGDOLL))
			{
				BEHAVIOR_MANAGER::Instance()->RemoveNodeWithPhysics(this);
			}
			else
			{
				if (_pDynamicActor)
				{
					const COM_INSTANCE & ciSGNode = GetSceneGraphNodeInst();
					if (ciSGNode.HasHandle())
					{
						NODE * pSGNode = static_cast<NODE *>(ciSGNode.GetCommunicator());
						SCENE_GRAPH * pSG = static_cast<SCENE_GRAPH *>(pSGNode->GetEntity());
						pSG->GetPhysicScene()->UnregisterDynamicActor(_pDynamicActor);
					}

					BEHAVIOR_MANAGER::Instance()->RemoveNodeWithPhysics(this);
				}
			}
		}

		int	i, nNbrChildren;
		nNbrChildren = GetNbrChildren();
		for	(i = 0 ; i < nNbrChildren ; ++i)
		{
			GetChild(i)->UnRegisterEntities();
		}
	}
}

//-----------------------------------------------------------------------------
// Name: NODE::AttachChildKeepGlobalMat
// Object: 
//01-05-30 CVI Created: 
//-----------------------------------------------------------------------------
void	NODE::AttachChildKeepGlobalMat(const COM_INSTANCE	&ChildInst, int nIndex )
{
	QDT_ASSERT(ChildInst.HasHandle());
	if	(!ChildInst.HasHandle())
	{
		return;
	}

	NODE*	pChild = static_cast<NODE *>(ChildInst.GetCommunicator());
	MATRIX	InvParentMat;
	MATRIX	ResultMat;
	MATRIX	InvMatCont;
	MATRIX	ResultMat2;

	//	Get global matrix
	if ( nIndex >= 0 )
	{
		//	Indexed case
		MATRIX ParentMat = GetIndexedGlobalMatrix( nIndex );
		InvParentMat.InverseAffine(ParentMat);
	}
	else
	{
		InvParentMat.InverseAffine( _GMatrix );
	}

	//	Compute new local matrix of the child
	ResultMat.Mul(pChild->_GMatrix, InvParentMat);
	//	Now include container matrix in computation
	if (pChild->_pContainer)
	{
		//	There is a container, so substract its composant
		InvMatCont.InverseAffine(pChild->_pContainer->GetMatrix());
		ResultMat2.Mul( InvMatCont, ResultMat );
		pChild->SetLocalMatrix( ResultMat2 );
		pChild->_pContainer->ResetAnimMatrices();
	}
	else
	{
		//	No container
		pChild->SetLocalMatrix( ResultMat );
	}

	AttachChild(ChildInst);

	QDT_ASSERT(pChild->GetParentNodeInst().HasHandle());
}

//-----------------------------------------------------------------------------
//	Name:		AttachCatalog
//	Object:		Attach all the nodes of a catalog to a node
//	01-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::AttachCatalog(const COM_INSTANCE & Catalog)
{
	CATALOG * pCatalog = (CATALOG *) Catalog.GetCommunicator();

	int				i, nNbrObjects;
	COM_INSTANCE	ChildInst;
	NODE *			pChild;

	nNbrObjects = pCatalog->GetNbrCommunicator();
	for	(i = 0 ; i < nNbrObjects ; ++i)
	{
		if	(pCatalog->GetCommunicatorInstance(i).GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE)
		{
			ChildInst = pCatalog->GetCommunicatorInstance(i);
			pChild = static_cast<NODE *>(ChildInst.GetCommunicator());
			
			if	(!pChild->GetParentNodeInst().HasHandle())
			{
				AttachChild(ChildInst);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name:		NODE::DetachChild
// Object:		Detach a child of the node
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
void	NODE::DetachChild(const COM_INSTANCE &ChildInst)
{
	QDT_ASSERT(ChildInst.HasHandle());
	NODE *pChild = static_cast<NODE*>(ChildInst.GetCommunicator());

	if (!pChild->GetParentNodeInst().HasHandle() || 
		this != static_cast<NODE*>(pChild->GetParentNodeInst().GetCommunicator()))
	{
		return;
	}

//	pChild->UnRegisterEntities();

	if	(pChild->GetContainer() != NULL)
	{
		UnsetControllerOnChild();
	}
	
	_nParentedIndex = -1;
	QDT_VECTOR<COM_INSTANCE>::ITERATOR	it = _vChildInst.Find(_vChildInst.GetHead(), ChildInst);
	if (it != _vChildInst.GetTail())
	{
		_vChildInst.Remove(it);
	}

	pChild->SetParentNodeInst(COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION));
}

//-----------------------------------------------------------------------------
// Name: NODE::DetachChildKeepGlobalMat
// Object: 
//01-05-30 CVI Created: 
//-----------------------------------------------------------------------------
void	NODE::DetachChildKeepGlobalMat(const COM_INSTANCE &ChildInst)
{
	QDT_ASSERT(ChildInst.HasHandle());
	if	(!ChildInst.HasHandle())
	{
		return;
	}

	NODE *pChild = static_cast<NODE *>(ChildInst.GetCommunicator());
	pChild->SetLocalMatrix(pChild->_GMatrix);

#if 0
	if	(pChild->GetEntityType() == ENTITY_QD_TRANSFORM)
	{
		const COM_INSTANCE & NodeInst = pChild->GetChildInst(0);

		if	(NodeInst.HasHandle())
		{
			const COM_INSTANCE & EntityInst = static_cast<NODE *>(NodeInst.GetCommunicator())->GetEntityInst();
			if	(EntityInst.HasHandle())
			{
				ENTITY_ABC	*pEntity = static_cast<ENTITY_ABC *>(EntityInst.GetCommunicator());

				if	(pEntity->GetEntityType() == ENTITY_MESH)
				{
					MESH	*pMesh = static_cast<MESH *>(pEntity);

					if	(pMesh->IsSoftBody() && pMesh->IsFree())
					{
//						pMesh->SetFree(true);
					}
				}
			}
		}
	}
#end

	DetachChild(ChildInst);
}
*/
//-----------------------------------------------------------------------------
// Name:	NODE::AddChild
// Object:	Add a new child to a node
// Note:	The child is add to the last position of the children
// 00-11-13 JVA Created:
//-----------------------------------------------------------------------------
void	NODE::AddChild(const COM_INSTANCE &	NodeInst)
{
	QDT_ASSERT(NodeInst.HasHandle());
	QDT_ASSERT(NodeInst.GetCommunicator() != this);	// Can't attach on itself !!

	NODE *	pNodeToAttach = static_cast<NODE *>(NodeInst.GetCommunicator());
	NODE *	pOldParent = pNodeToAttach->GetParentNode();
	
	if (pOldParent == this)
	{
		return;
	}
	
	if (pOldParent)
	{
		pNodeToAttach->RemoveFromFather();
	}

	if	(_FirstChildInst.HasHandle())
	{
		NODE *	pChild = GetFirstChild();
		while	(pChild->GetBrotherInst().HasHandle())
		{
			pChild = pChild->GetBrother();
		}

		pChild->SetBrotherInst(NodeInst);
	}
	else
	{
		_FirstChildInst = NodeInst;
	}

	pNodeToAttach->SetParentNodeInst(COM_INSTANCE(GetHandle()));

	SetGlobalMatrixInvalidOnChild();

	if	(pNodeToAttach->GetContainer() != NULL)
	{
		SetContainerOnChild();
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddChild
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::AddChild(const COM_INSTANCE &	NodeInst,
					   int					nIndex)
{
	if	(NodeInst.HasHandle() == false)
	{
		return;
	}

	NODE *	pChild = static_cast<NODE *>(NodeInst.GetCommunicator());
	pChild->SetParentedIndex(nIndex);

	AddChild(NodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		AddChildKeepGlobalMatrix
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::AddChildKeepGlobalMatrix(const COM_INSTANCE &	NodeInst,
									   int					nIndex)
{
	if	(NodeInst.HasHandle() == false)
	{
		return;
	}

	NODE *	pChild = static_cast<NODE *>(NodeInst.GetCommunicator());
	MATRIX	InvParentMat;
	MATRIX	ResultMat;
	MATRIX	InvMatCont;
	MATRIX	ResultMat2;

	//	Get global matrix
	if (nIndex >= 0)
	{
		//	Indexed case
		MATRIX ParentMat = GetIndexedGlobalMatrix(nIndex);
		InvParentMat.InverseAffine(ParentMat);
	}
	else
	{
		InvParentMat.InverseAffine(_GMatrix);
	}
	pChild->SetParentedIndex(nIndex);

	//	Compute new local matrix of the child
	ResultMat.Mul(pChild->_GMatrix, InvParentMat);
	//	Now include container matrix in computation
	if (pChild->_pContainer)
	{
		//	There is a container, so substract its composant
		InvMatCont.InverseAffine(pChild->_pContainer->GetMatrix());
		ResultMat2.Mul(InvMatCont, ResultMat);
		pChild->SetLocalMatrix(ResultMat2);
		pChild->_pContainer->ResetAnimMatrices();
	}
	else
	{
		//	No container
		pChild->SetLocalMatrix(ResultMat);
	}

	AddChild(NodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveFromFather
//	Object:		
//	01-08-17:	VPI - Created
//-----------------------------------------------------------------------------
void	NODE::RemoveFromFather()
{
	if	(_ParentInst.HasHandle() == false)
	{
		return;
	}

	if	(GetContainer() != NULL)
	{
		UnsetControllerOnChild();
	}

	NODE *	pParentNode = GetParentNode();

	pParentNode->RemoveChild(COM_INSTANCE(GetHandle()));

	_ParentInst = COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveFromFatherKeepGlobalMat
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::RemoveFromFatherKeepGlobalMat()
{
	if	(_ParentInst.HasHandle() == false)
	{
		return;
	}

	if	(GetContainer() != NULL)
	{
		UnsetControllerOnChild();
	}

	NODE *	pParentNode = GetParentNode();

	pParentNode->RemoveChildKeepGlobalMat(COM_INSTANCE(GetHandle()));

	_ParentInst = COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveChild
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::RemoveChild(const COM_INSTANCE &	NodeInst)
{
	if	(_FirstChildInst == NodeInst)
	{
		NODE *	pNodeToRemove = static_cast<NODE *>(NodeInst.GetCommunicator());
		_FirstChildInst = pNodeToRemove->GetBrotherInst();
		pNodeToRemove->SetBrotherInst(COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION));
	}
	else
	{
		NODE *	pLeftBrother = GetFirstChild();
		NODE *	pChild = pLeftBrother->GetBrother();
		while	(pChild != NULL)
		{
			if	(NodeInst == COM_INSTANCE(pChild->GetHandle()))
			{
				pLeftBrother->SetBrotherInst(pChild->GetBrotherInst());
				pChild->SetBrotherInst(COM_INSTANCE(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION));
				break;
			}

			pLeftBrother = pChild;
			pChild = pChild->GetBrother();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveChildKeepGlobalMat
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::RemoveChildKeepGlobalMat(const COM_INSTANCE &	NodeInst)
{
	NODE *	pNodeToRemove = static_cast<NODE *>(NodeInst.GetCommunicator());
	pNodeToRemove->SetLocalMatrix(pNodeToRemove->_GMatrix);

	RemoveChild(NodeInst);
}

/*
//-----------------------------------------------------------------------------
//	Name:		DetachCatalog
//	Object:		Detach all the nodes of a catalog of a node
//	01-06-05:	ELE - Created
//  03-12-04:	RMA - Detach all children too
//-----------------------------------------------------------------------------
void	NODE::DetachCatalog(const COM_INSTANCE & Catalog)
{
	int				i, nNbrObjects;
	CATALOG *		pCatalog;
	COM_INSTANCE 	ComInstance;
	NODE *			pNode;

	QDT_ASSERT(Catalog.IsLoaded());

	if	(Catalog.IsLoaded())
	{
		pCatalog = static_cast<CATALOG *>(Catalog.GetCommunicator());
		nNbrObjects = pCatalog->GetNbrCommunicator();

		for	(i = 0 ; i < nNbrObjects ; ++i)
		{
			if	(pCatalog->GetCommunicatorInstance(i).GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE)
			{
				ComInstance = pCatalog->GetCommunicatorInstance(i);
				if	(!ComInstance.HasHandle())
				{
					continue;
				}

				pNode = static_cast<NODE*>(ComInstance.GetCommunicator());

				if	(pNode->GetParentNode() == this)
				{
					pNode->DetachFromFather();

					while (pNode->GetNbrChildren())
					{
						pNode->GetChild(0)->DetachFromFather();
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		BreakConnection
//	Object:		
//	04-01-29:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::BreakConnections(const COM_INSTANCE &	Catalog)
{
	QDT_ASSERT(Catalog.IsLoaded());
	
	if (Catalog.IsLoaded())
	{
		CATALOG * pCatalog = static_cast<CATALOG *>(Catalog.GetCommunicator());
		int nNbrObjects = pCatalog->GetNbrCommunicator();

		for	(int i = 0 ; i < nNbrObjects ; ++i)
		{
			if	(pCatalog->GetCommunicatorInstance(i).GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE)
			{
				COM_INSTANCE ci = pCatalog->GetCommunicatorInstance(i);
				if	(!ci.HasHandle())
				{
					continue;
				}

				NODE * pNode = static_cast<NODE*>(ci.GetCommunicator());

				if (pNode->GetParentNodeInst().HasHandle())
				{
					pNode->BreakConnectionsRec();
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name: NODE::RayCastTree
//	Object: Ray cast with a tree
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void NODE::RayCastTree(RAY_CAST_PARAMETERS & RayCastParameters)
{
	bool	bContinue = false;

	if	(!GetFlags().IsSet(NO_COLLISION))
	{
		if (RayCast(RayCastParameters, bContinue))
		{
			RayCastParameters.SetImpactNodeID(GetID());
		}
	}

	if	(bContinue)
	{
		unsigned int i, nNbrChildren = GetNbrChildren();
		for (i = 0 ; i < nNbrChildren ; ++i)
		{
			GetChild(i)->RayCastTree(RayCastParameters);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		NODE::RayCast
//	Object:		Raycast with the entity
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
bool NODE::RayCast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue)
{
	if (!_EntityInst.HasHandle())
	{
		return false;
	}

	COM_INSTANCE ThisComInstance(GetHandle());

	ENTITY_ABC *pEntity = GetEntity();

	// To avoid unlimited recursion using portals, we don't test the portal from
	// which we come from !!
	if (RayCastParameters.GetPortalInNodeID() == GetID())
	{
		bContinue = true;
		return (false);
	}

	bool bHit;

	// Update the global matrix of DisplayParameters
	RayCastParameters.SetGlobalMatrix(GetGlobalMatrix());
	RayCastParameters.SetCurrentNodeID(GetID());

	// Test the ray cast with the entity
	bHit = pEntity->RayCast(RayCastParameters, bContinue);

	if (bHit)
	{
		// So the node has been hit
		RayCastParameters.SetImpactNodeID(GetID());
	}

	return (bHit);
}
*/

//-----------------------------------------------------------------------------
//	Name:		NODE::LiveSkeleton
//	Object:		Make a alive copy of a skeleton.
//				You must be sure the skeleton is loaded before calling this
//				method
//	01-03-02:	RMA - Created
//	01-03-27	CVI - Updated
//  01-05-04	CVI - Updated 
//-----------------------------------------------------------------------------
void	NODE::LiveSkeleton(const COM_INSTANCE &	NodeInst,
						   I_PHYSIC_SCENE *		pScene,
						   UInt32				uiLastLockedLevel,
						   UInt32				uiFirstFreeLevel)
{}

//-----------------------------------------------------------------------------
//	Name:		LiveSkeleton
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE NODE::LiveSkeleton(I_PHYSIC_SCENE * pScene, UInt32 uiLastLockedLevel, UInt32 uiFirstFreeLevel)
{
	COM_INSTANCE NodeInst(NODE::GetGenericCommunicatorDescriptor()->Create());
	LiveSkeleton(NodeInst, pScene, uiLastLockedLevel, uiFirstFreeLevel);	
	return (NodeInst);	
}

//-----------------------------------------------------------------------------
//	Name:		CheckEmptyContainer
//	Object:		
//	01-05-17:	GHP - Created
//-----------------------------------------------------------------------------
bool NODE::CheckEmptyContainer()  
{
	CONTAINER::HasOrCreatesContainer(this);
	
	if ( _pContainer->IsEmpty() )
	{
		delete _pContainer ; 
		_pContainer = NULL ;

		if	(GetParentNode())
		{
			GetParentNode()->UnsetContainerOnChild();
		}

		return ( true ) ; 
	}
	return ( false ) ; 
}

//-----------------------------------------------------------------------------
//	Name:		IsPlaying
//	Object:		
//	01-07-04:	GHP - Created
//-----------------------------------------------------------------------------
bool NODE::IsPlaying() const
{
	/*if (GetEntityType() == ENTITY_SKELETON) 
	{
		return (GetChild(0)->IsPlaying()) ; 
	}
	else
	{	
		return(_pContainer && !_pContainer->IsEmpty());
	}*/

	//	CVI : 30/07/03
//	return(_pContainer && !_pContainer->IsEmpty());
	if ( !_pContainer )	return ( false );
	return ( (_pContainer->IsPlaying() ));
}

//-----------------------------------------------------------------------------
//	Name:		SetController
//	Object:		
//	01-07-05:	GHP - Created
//-----------------------------------------------------------------------------
void	NODE::SetController(ENTITY_CONTROLLER_ABC*	pController)
{
	if (_pController == NULL)
	{
		BEHAVIOR_MANAGER::Instance()->AddNodeWithController(this);
	}

	QDT_ASSERT(pController);
	_pController = pController;

	if	(_pController)
	{
		if	(GetParentNode())
		{
			GetParentNode()->SetControllerOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetController
//	Object:		
//	04-12-01:	ELE - Created
//-----------------------------------------------------------------------------
ENTITY_CONTROLLER_ABC *	NODE::GetController(int	nBSIdx) const
{
	return ( _pController ) ;
}

//-----------------------------------------------------------------------------
//	Name:		HasController
//	Object:		
//	04-12-01:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::HasController() const
{
	return	( _pController != NULL );
}
/*
//-----------------------------------------------------------------------------
//	Name:		CountJointChildren
//	Object:		
//	01-08-24:	GHP - Created
//-----------------------------------------------------------------------------
int	NODE::CountJointChildren()
{
	unsigned int i, nCount  = GetNbrChildren();
	unsigned int nNbrChilds = 0;

	for (i = 0 ; i < nCount ; ++i)
	{
		if (GetChild(i)->GetEntityType() == ENTITY_JOINT)
		{
			++nNbrChilds;
		}
	}

	return (nNbrChilds);
}
*/
//-----------------------------------------------------------------------------
//	Name:		CountTotalJointChildren
//	Object:		
//	03-07-17:	CVI - Created
//-----------------------------------------------------------------------------
int		NODE::CountTotalJointChildren()
{
	int		nNbrChildren = 0;
	
	NODE *	pChild = GetFirstChild();
	while	(pChild)
	{
		if (pChild->GetFlags().IsSet(JOINT_NODE))
		{
			nNbrChildren += pChild->CountTotalJointChildren();
			nNbrChildren++;	//	Count this child too
		}

		pChild = pChild->GetBrother();
	}

	return	(nNbrChildren);	
}

//-----------------------------------------------------------------------------
//	Name:		ResetPitch
//	Object:		
//	01-09-08:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::ResetPitch()
{
	if (!GetFlags().IsSet(GLOBAL_MATRIX_VALID)) 
	{
		UpdateGlobalMatrix();
	}

	_GMatrix.FilterPitchHeadRoll(false, true, true);
	UpdateLocalMatrix();
}

//-----------------------------------------------------------------------------
//	Name:		ResetHeading
//	Object:		
//	01-09-08:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::ResetHeading()
{
	if (!GetFlags().IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	_GMatrix.FilterPitchHeadRoll(true, false, true);
	UpdateLocalMatrix();
}

//-----------------------------------------------------------------------------
//	Name:		ResetRoll
//	Object:		
//	01-09-08:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::ResetRoll()
{
	if (!GetFlags().IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	_GMatrix.FilterPitchHeadRoll(true, true, false);
	UpdateLocalMatrix();
}

//-----------------------------------------------------------------------------
//	Name:		GetFrame
//	Object:		
//	01-09-24:	GHP - Created
//-----------------------------------------------------------------------------
void	NODE::GetFrame(float &rFrame)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		
//	01-10-05:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE::DumpTree(IO_STREAM &	Out)
{
	Dump(Out);

	NODE *	pFirstChild = GetFirstChild();
	if	(pFirstChild)
	{
		pFirstChild->DumpTree(Out);
	}

	NODE *	pBrother = GetBrother();
	if	(pBrother)
	{
		pBrother->DumpTree(Out);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		
//	01-10-05:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE::Dump(IO_STREAM &	Out)
{
	char	sBuffer[256];

	Out << "NODE :\n";

	sprintf(sBuffer, "m3d_node::%s\n", GetName().GetBuffer());
	Out << sBuffer;

	Out << "	Parent :\n";
	NODE *pParent = GetParentNode();

	if	(pParent)
	{
		sprintf(sBuffer, "        m3d_node::%s\n", pParent->GetName().GetBuffer());
		Out << sBuffer;
	}
	else
	{
		Out << "        none\n";
	}

	Out << "    Children :\n";

	NODE *	pChild = GetFirstChild();
	while	(pChild != NULL)
	{
		sprintf(sBuffer, "        m3d_node::%s", pChild->GetName().GetBuffer());
		Out << sBuffer;

		pChild = pChild->GetBrother();
	}

	if (_FirstChildInst.HasHandle() == false)
	{
		Out << "        none\n";
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		
//	01-10-09:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE::Dump(const char *	pFileName)
{
	char	CFileName[256];

	sprintf(CFileName, "%s.txt", pFileName); 
	FILE_STREAM	FSA(CFileName, FILE_STREAM::AT_CREATE);

	Dump(FSA);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		PlayAnim
//	Object:		
//	01-11-12:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::PlayAnim( const COM_INSTANCE& Anim )
{
	BEHAVIOR_MANAGER::Instance()->PlayAnim( COM_INSTANCE(GetHandle()), Anim, true );
} 

//-----------------------------------------------------------------------------
//	Name:		PushAnim
//	Object:		
//	01-11-16:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::PushAnim( const COM_INSTANCE& Anim )
{
	BEHAVIOR_MANAGER::Instance()->PlayAnim( COM_INSTANCE(GetHandle()), Anim, false );
}


//-----------------------------------------------------------------------------
//	Name:		GetActiveAnim
//	Object:		
//	01-12-05:	cvi - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE 	NODE::GetActiveAnim() const
{
	COM_INSTANCE ret(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION);

	if ( !_pContainer )
	{
		return ( ret );
	}

	ret = _pContainer->GetActiveAnim();
	return ( ret );
}

//-----------------------------------------------------------------------------
//	Name:		ResetContainerPos
//	Object:		
//	01-12-06:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::ResetContainerPos()
{	
	if ( _pContainer )
	{
		// Reset container position
		MATRIX & Mat = _pContainer->GetMatrix();
		Mat.SetPosition(VECTOR(0.0f, 0.0f, 0.0f));
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetContainerMatrix
//	Object:		
//	01-12-12:	cvi - Created
//-----------------------------------------------------------------------------
void	NODE::ResetContainerMatrix()
{
	
	
	if ( _pContainer )
	{
		//	Reset container position
		MATRIX Mat;
		Mat.Identity();
		_pContainer->SetMatrix( Mat );
	}
}

//-----------------------------------------------------------------------------
//	Name:		ResetAnimMatrix
//	Object:		
//	03-09-29:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::ResetAnimMatrix()
{
	if ( _pContainer )
	{
		_pContainer->ResetAnimMatrices();
	}
}


//-----------------------------------------------------------------------------
//	Name:		ResetChildMatrix
//	Object:		
//	01-12-12:	cvi - Created
//-----------------------------------------------------------------------------
int	NODE::ResetAllChildMatrix(NODE*	pNode,
							  void*	pData)
{
	if (!pNode)	return (1);
	pNode->ResetContainerMatrix();
	pNode->GetFlags().Unset(GLOBAL_MATRIX_VALID);

	if	(pNode->GetParentNode())
	{
		pNode->GetParentNode()->UnsetGlobalMatrixInvalidOnChild();
	}

	return (1);
}

//-----------------------------------------------------------------------------
//	Name:		GetBoxGlobalPosition
//	Object:		
//	02-01-14:	cvi - Created
//-----------------------------------------------------------------------------
void NODE::GetGlobalCenter(VECTOR &outCenter)
{
	outCenter = VECTOR::ZERO;

//	ENTITY_ABC	*pEntity = GetEntity();
//	if	(pEntity)
//	{
//		outCenter = pEntity->GetCenter();
		
		GetGlobalMatrix().TransformPosition(outCenter);
//	}
}

//-----------------------------------------------------------------------------
//	Name:		CheckSubGraphFatherLink
//	Object:		
//	02-01-28:	cvi - Created
//-----------------------------------------------------------------------------
int	 NODE::CheckSubGraphFatherLink(bool bCorrect)
{
	// Need update!!
	QDT_NOT_IMPLEMENTED();
	return (-1);


/*	int					i, n;
	NODE				*pN;
	NODE				*pFather;
	int					nLastFail = -1;
	int					nCount;
	int					nRes;

	///////////////////////////
	n = GetNbrChildren();
	if (n <= 0)
	{
		return (-1);
	}

	///////////////////////////
	nCount = -1;
	for (i = 0 ; i < n ; ++i)
	{
		nCount++;
		//	Get Node
		pN = GetChild(i);
		if ( !pN )
		{
			//	Not a valid index
			QDT_FAIL();
			DetachChild( GetChildInst(i) );
			return ( -1 );
		}
		/////////////////////////////////////////
		//	Recursive call
		nRes = pN->CheckSubGraphFatherLink( bCorrect );
		if ( nRes >= 0 )
		{
			if ( !bCorrect )
			{
				return ( nRes );
			}
		}
		/////////////////////////////////////////
		//	Get father node
		pFather = pN->GetParentNode();
		/////////////////////////////////////////
		if ( !pFather )
		{
			QDT_Error("Incoherent link with node %s",pN->GetName().GetBuffer() );
			if ( !bCorrect )
			{
				return ( nCount );
			}
			//	No father. So update father index of the node
			pN->_ParentInst = COM_INSTANCE(GetHandle());
			DetachChild( GetChildInst(i) );
			AttachChild( GetChildInst(i) , false);

			continue;
		}
		/////////////////////////////////////////
		if ( pFather != this )
		{
			QDT_Error("Incoherent link with node %s",pN->GetName().GetBuffer() );
			if ( !bCorrect )
			{
				return ( nCount );
			}
			//	Not same father
			nLastFail	= nCount;
			//	Detach node from this
			DetachChild( GetChildInst(i) );
			//	Set Father again
			pN->_ParentInst = COM_INSTANCE(pFather->GetHandle());
			//	Detach from that father to update father, as we dont' know if this father reconize it or not
			pN->DetachFromFather();
			//	Then attach it again to that father
			pFather->AttachChild( GetChildInst(i), false );
		}
	}

	return ( nLastFail );*/
	///////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		GetPitchHeadRoll
//	Object:		
//	02-04-04:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::GetPitchHeadRoll(QUATERNION &	QPitch,
							   QUATERNION &	QHead,
							   QUATERNION &	QRoll)
{
	_GMatrix.GetPitchHeadRoll( QPitch, QHead, QRoll );
}

//-----------------------------------------------------------------------------
//	Name:		GetQuaternion
//	Object:		
//	03-05-05:	JJA - Created
//-----------------------------------------------------------------------------
QUATERNION	NODE::GetQuaternion() const
{
	QUATERNION	Q;

	// Get Global Matrix Quaternion
	_GMatrix.GetQuaternion( Q );

	return	( Q );
}

//-----------------------------------------------------------------------------
//	Name:		SetQuaternion
//	Object:		
//	03-05-07:	FDE - Created
//-----------------------------------------------------------------------------
void	NODE::SetQuaternion(const QUATERNION&	Quat)
{
	_GMatrix.SetQuaternion( Quat );

	UpdateLocalMatrix() ;

	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);

	_Flags.Set(GLOBAL_MATRIX_VALID);

	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		GetPitchHeadRoll
//	Object:		
//	02-04-04:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::GetPitchHeadRoll(ANGLE3D &	Angle)
{
	float		rP, rH, rR;
	VECTOR		V;
	QUATERNION	QPitch, QHead, QRoll;

	///////////////////////////////////////////////////
	_GMatrix.GetPitchHeadRoll( QPitch, QHead, QRoll );
	///////////////////////////////////////////////////
	QPitch.GetVectRoll( V, rP );
	QHead.GetVectRoll( V, rH );
	QRoll.GetVectRoll( V, rR );
	///////////////////////////////////////////////////
	Angle.Set( rP, rH, rR );
	///////////////////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		UpdateContainer
//	Object:		
//	04-04-27:	RMA - Created
//  05-03-03:	RMA - Patched to remove a crash from a check point on the
//				murder
//-----------------------------------------------------------------------------
void	NODE::UpdateContainer(float rSpeedFactor)
{
	QDT_ASSERT(_pContainer);

	// RMA : I reset the holder node because it can be corrupted in some
	// situations. This is probably due to a node in two databank?
	// This seems to fix the problem however.
	_pContainer->SetHolderNode(this);
	_pContainer->Update(rSpeedFactor);
	if (!_pContainer->IsPlayingAnimation())
	{
		BEHAVIOR_MANAGER::Instance()->RemoveAnimatedNode(COM_INSTANCE(this));
	}
}	

//-----------------------------------------------------------------------------
//	Name:		UpdateBehavior
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::UpdateBehavior(float rSpeedFactor,
							 bool bForceUpdate)
{
	if	(GetFlags().IsSet(GLOBAL_MATRIX_VALID) == false)
	{
		UpdateGlobalMatrix();
	}

	if (_pContainer)
	{
		bool bNodeArray = GetHandle()->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY;

		VECTOR PreviousPos;
	
		if (bNodeArray)
		{
			static_cast<NODE_ARRAY *>(this)->GetChildGlobalPos(0, PreviousPos);
		}
		else
		{
			GetGlobalPosition(PreviousPos);
		}

		UpdateContainer(rSpeedFactor);
		
		SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateController
//	Object:		
//	04-01-29:	AMA - Created
//-----------------------------------------------------------------------------
void	NODE::UpdateController(float	rSpeedFactor)
{
	QDT_ASSERT(_pController);
	
	_pController->Update(rSpeedFactor);
	_pController->DeleteFinishedBehaviors();
}

//-----------------------------------------------------------------------------
//	Name:		ForceUpdate
//	Object:		
//	03-10-09:	VMA - Created
//-----------------------------------------------------------------------------
void	NODE::ForceUpdate(float rSpeedFactor)
{
	CONTAINER*	pContainer = GetContainer();

	if	( pContainer )
	{
		//	Test by CVI : 24/03/04:
		//	SklTracks were not computed, so force update behavior
//		pContainer->Update( 0.0f );
		UpdateBehavior(rSpeedFactor, true);
	}

	NODE *	pFirstChild = GetFirstChild();
	if	(pFirstChild)
	{
		pFirstChild->ForceUpdate(rSpeedFactor);
	}

	NODE *	pBrother = GetBrother();
	if	(pBrother)
	{
		pBrother->ForceUpdate(rSpeedFactor);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BuildAABB
//	Object:		
//	02-08-07:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::BuildAABB(AABB &	BoundingBox)
{
	//DAC: too much assert in IAM , QDT_NOT_IMPLEMENTED();
	QDT_Warning("NODE::BuildAABB() not implemented");
/*
	QDT_ASSERT(_EntityInst.HasHandle());

	switch (_EntityInst.GetType())
	{
	default:
		break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH:
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE:
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SOFT_BODY:
			{
				MESH *pMesh = static_cast<MESH*>(GetEntity());

				if (pMesh->GetBoundingBox())
				{
					BoundingBox.Grow(*static_cast<const AABB*>(pMesh->GetBoundingBox()));
				}
			}
			break;
		
		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH_GROUP :
			{
				MESH_GROUP *pMG = static_cast<MESH_GROUP*>(GetEntity());	
				BoundingBox.Grow(pMG->GetBoundingBox());
			}
			break;

		case MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PARTICLE_EMITTER :
			{
				PARTICLE_EMITTER *pPE = static_cast<PARTICLE_EMITTER*>(GetEntity());
				OBB Box = pPE->GetBoundingBox();
				Box.Transform(GetGlobalMatrix());
				BoundingBox.Grow(Box);				
			}
			break;
	}

	// Parse children recursively
	unsigned int i, nCount = GetNbrChildren();
	for (i = 0 ; i < nCount ; ++i)
	{
		GetChild(i)->BuildAABB(BoundingBox);
	}
	*/
}

//-----------------------------------------------------------------------------
//	Name: NODE::SetFlagsTree
//	Object: Set the flags for a node and all his children
//	01-02-06:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::SetFlagsTree(int nFlagsMask, int nFlagsSet)
{
	_Flags.SetFlags(nFlagsMask, nFlagsSet);

	NODE *	pFirstChild = GetFirstChild();
	if	(pFirstChild)
	{
		pFirstChild->SetFlagsTree(nFlagsMask, nFlagsSet);
	}

	NODE *	pBrother = GetBrother();
	if	(pBrother)
	{
		pBrother->SetFlagsTree(nFlagsMask, nFlagsSet);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFlagsUp
//	Object:		
//	04-01-27:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::SetFlagsUp(int	nFlagsMask,
						 int	nFlagsSet)
{
	_Flags.SetFlags(nFlagsMask, nFlagsSet);

	if (GetParentNodeInst().HasHandle())
	{
		GetParentNode()->SetFlagsUp(nFlagsMask, nFlagsSet);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsChildOf
//	Object:		
//	02-12-03:	RMA - Created
//-----------------------------------------------------------------------------
bool	NODE::IsChildOf(const COM_INSTANCE & NodeInst) const
{
	return (_ParentInst == NodeInst);
}
/*
//-----------------------------------------------------------------------------
//	Name:		NODE::GetSceneGraph
//	Object:		Get the scene graph of the node
//				See the method GetZoneNodeIndex for more informations, it's
//				very similary.
//	01-03-02:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE & NODE::GetSceneGraphNodeInst() const
{
	const NODE	*pNode = this;
	NODE		*pParentNode = GetParentNode();
	
	while	(pParentNode)
	{
		if (pParentNode->GetEntityType() == ENTITY_SCENE_GRAPH)
		{
			return (pNode->GetParentNodeInst());
		}
		
		pNode = pParentNode;
		pParentNode = pParentNode->GetParentNode();
	}

	return	(COM_INSTANCE::GetNullComInstance());
}
*/
//-----------------------------------------------------------------------------
//	Name:		MoveRightAround
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveRightAround(const VECTOR & TargetPos, float rValue, bool bHasRoll )
{
	MoveLeftAround( TargetPos, -rValue, bHasRoll );
}

//-----------------------------------------------------------------------------
//	Name:		MoveUpAround
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveUpAround(const VECTOR & TargetPos, float rValue, bool bHasRoll )
{
	VECTOR				Pos;
	VECTOR				RelPos;
	float				rA;
	QUATERNION			Q1, Q2, Q;
	VECTOR				V, VBase;
	QUATERNION_VECTOR	QV1, QV2, QVR, QTemp, QRot;
	QUATERNION_VECTOR	QLoc;
	VECTOR				V2;

	/////////////////////////////////
	Pos = _GMatrix.GetTranslation();
	RelPos.SetFromPoints(TargetPos, Pos);
	/////////////////////////////////
	Q1.Id();
	/////////////////////////////////
	//	Global Matrix for target pos
	QV1.SetQuat(Q1);
	QV1.SetVector(TargetPos);
	/////////////////////////////////
	//	Global Matrix of this node
	_GMatrix.GetQuaternion( Q );
	QV2.SetQuat( Q );
	QV2.SetVector( Pos );
	/////////////////////////////////
	//	Matrix of this node in target coordinates
	QTemp	= QV1;
	QTemp.Inverse();
	QLoc	= QTemp * QV2;
	/////////////////////////////////
	//	Rotation around Target Y-axis
	V.SetXYZ( 0.0f, 1.0f, 0.0f );
	VBase = Q * V;
	V = RelPos;
	V.Normalize();
	V2.Cross( VBase, V );
	if ( V2.IsNull() )
	{
		V2.SetXYZ( 0.0f, 1.0f, 0.0f );
	}
	else
	{
		V2.Normalize();
	}
	//
	rA = ( -rValue ) * ( 1.0f / 180.0f ) * MATH::PI();
	Q2.SetVectRoll( V2, rA );
	QRot.SetQuat( Q2 );
	/////////////////////////////////
	//	Include that rotation in targetPos
	//	matrix
	/////////////////////////////////
	QTemp = QV1 * QRot;
	/////////////////////////////////
	//	Now Compute This node Global 
	//	matrix
	QVR = QTemp * QLoc;
	/////////////////////////////////
	//	Now build matrix
	Q = QVR.GetQuat();
	Q.Normalize();
	_GMatrix.SetQuaternion( Q );
	_GMatrix.SetTranslation( QVR.GetVector() );
	/////////////////////////////////
	UpdateLocalMatrix();
	/////////////////////////////////
	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
	/////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		MoveAwayFrom
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveAwayFrom(const VECTOR & TargetPos, float rValue)
{
	VECTOR			P, PR;
	VECTOR			V;
	float			rDistance;
	float			rS;

	/////////////////////////////////////
	P = _GMatrix.GetTranslation();
	V.SetFromPoints( TargetPos, P );
	rDistance = V.Length();
	/////////////////////////////////////
	rS = rValue / rDistance;
	/////////////////////////////////////
	V.Mul( rS )	;
	P.Add( V );
	_GMatrix.SetPosition( P );
	UpdateLocalMatrix();
	/////////////////////////////////////
	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
	/////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		MoveLeftAround
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveLeftAround(const VECTOR & TargetPos, float rValue, bool bHasRoll )
{
	VECTOR				Pos;
	VECTOR				RelPos;
	float				rA;
	QUATERNION			Q1, Q2, Q;
	VECTOR				V, VBase;
	QUATERNION_VECTOR	QV1, QV2, QVR, QTemp, QRot;
	QUATERNION_VECTOR	QLoc;
	VECTOR				V2;


	/////////////////////////////////
	Pos = _GMatrix.GetTranslation();
	RelPos.SetFromPoints(  TargetPos, Pos );
	/////////////////////////////////
	Q1.Id();
	/////////////////////////////////
	//	Global Matrix for target pos
	QV1.SetQuat( Q1 );
	QV1.SetVector( TargetPos );
	/////////////////////////////////
	//	Global Matrix of this node
	_GMatrix.GetQuaternion( Q );
	QV2.SetQuat( Q );
	QV2.SetVector( Pos );
	/////////////////////////////////
	//	Matrix of this node in target coordinates
	QTemp	= QV1;
	QTemp.Inverse();
	QLoc	= QTemp * QV2;
	/////////////////////////////////
	//	Rotation around Target Y-axis
	V.SetXYZ( 1.0f, 0.0f, 0.0f );
	VBase = Q * V;
	V = RelPos;
	V.Normalize();
	V2.Cross( VBase, V );
	if ( V2.IsNull() || bHasRoll == false )
	{
		V2.SetXYZ( 0.0f, 1.0f, 0.0f );
	}
	else
	{
		V2.Normalize();
	}
	//
	rA = ( rValue ) * ( 1.0f / 180.0f ) * MATH::PI();
	Q2.SetVectRoll( V2, rA );
	QRot.SetQuat( Q2 );
	/////////////////////////////////
	//	Include that rotation in targetPos
	//	matrix
	/////////////////////////////////
	QTemp = QV1 * QRot;
	/////////////////////////////////
	//	Now Compute This node Global 
	//	matrix
	QVR = QTemp * QLoc;
	/////////////////////////////////
	//	Now build matrix
	Q = QVR.GetQuat();
	Q.Normalize();
	_GMatrix.SetQuaternion( Q );
	_GMatrix.SetTranslation( QVR.GetVector() );
	/////////////////////////////////
	UpdateLocalMatrix();
	/////////////////////////////////
	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
	/////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		LookAt 
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::LookAt(const VECTOR & TargetPos)
{
	// Update rotation
	_GMatrix.Target(TargetPos);
	UpdateLocalMatrix();
	
	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		MoveNearTo
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveNearTo(const VECTOR & TargetPos, float rValue)
{
	VECTOR			P, PR;
	VECTOR			V;
	float			rDistance;
	float			rS;

	/////////////////////////////////////
	P = _GMatrix.GetTranslation();
	V.Sub(TargetPos, P);
	rDistance = V.Length();
	/////////////////////////////////////
	rS = rValue / rDistance;
	if ( rS > 1.0f )
	{
		rS = 1.0f;
	}
	/////////////////////////////////////
	V.Mul( rS )	;
	P.Add( V );
	_GMatrix.SetPosition( P );
	UpdateLocalMatrix();
	/////////////////////////////////////
	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
	/////////////////////////////////////
}

//-----------------------------------------------------------------------------
//	Name:		MoveDownAround
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveDownAround(const VECTOR & TargetPos, float rValue, bool bHasRoll )
{
	MoveUpAround( TargetPos, -rValue, bHasRoll );
}

//-----------------------------------------------------------------------------
//	Name:		LookUp
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::LookUp(float	rAngle)
{
	VECTOR			AxeX(1.0f, 0.0f, 0.0f);
	MATRIX			Temp1;
	MATRIX			OldMat;
	VECTOR			CurrentPos;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	//	Get current Global Matrix
	OldMat		= _GMatrix;

	//	Get current global pos
	CurrentPos = _GMatrix.GetTranslation();

	//	Build temp1 matrix
	Temp1.Identity();
	Temp1.SetRotation(AxeX, rAngle);

	//	Reset global matrix;
	_GMatrix.Identity();
	//	Rotate OldMat of Temp1's rotation
	_GMatrix.Mul( Temp1, OldMat );
	//	Now set pos
	_GMatrix.SetPosition( CurrentPos );

	//	Update local matrix
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		LookDown
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::LookDown(float	rAngle)
{
	VECTOR			AxeX(1.0f, 0.0f, 0.0f);
	MATRIX			Temp1;
	MATRIX			OldMat;
	VECTOR			CurrentPos;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	//	Get current Global Matrix
	OldMat		= _GMatrix;

	//	Get current global pos
	CurrentPos = _GMatrix.GetTranslation();

	//	Build temp1 matrix
	Temp1.Identity();
	Temp1.SetRotation(AxeX, -rAngle);

	//	Reset global matrix;
	_GMatrix.Identity();
	//	Rotate OldMat of Temp1's rotation
	_GMatrix.Mul( Temp1, OldMat );

	//	Now set pos
	_GMatrix.SetPosition( CurrentPos );

	//	Update local matrix
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		TurnLeft
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::TurnLeft(float	rAngle)
{
	VECTOR			AxeY(0.0f, 1.0f, 0.0f);
	MATRIX			Temp1;
	MATRIX			OldMat;
	VECTOR			CurrentPos;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	//	Get current Global Matrix
	OldMat		= _GMatrix;

	//	Get current global pos
	CurrentPos = _GMatrix.GetTranslation();

	//	Build temp1 matrix
	Temp1.Identity();
	Temp1.SetRotation(AxeY, rAngle);

	//	Reset global matrix;
	_GMatrix.Identity();
	//	Rotate OldMat of Temp1's rotation
	_GMatrix.Mul( Temp1, OldMat );

	//	Now set pos
	_GMatrix.SetPosition( CurrentPos );

	//	Update local matrix
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		TurnRight
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::TurnRight(float	rAngle)
{
	VECTOR			AxeY(0.0f, 1.0f, 0.0f);
	MATRIX			Temp1;
	MATRIX			OldMat;
	VECTOR			CurrentPos;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	//	Get current Global Matrix
	OldMat		= _GMatrix;

	//	Get current global pos
	CurrentPos = _GMatrix.GetTranslation();

	//	Build temp1 matrix
	Temp1.Identity();
	Temp1.SetRotation(AxeY, -rAngle);

	//	Reset global matrix;
	_GMatrix.Identity();
	//	Rotate OldMat of Temp1's rotation
	_GMatrix.Mul( Temp1, OldMat );


	//	Now set pos
	_GMatrix.SetPosition( CurrentPos );

	//	Update local matrix
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		RollLeft
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::RollLeft(float	rAngle)
{
	VECTOR			AxeZ(0.0f, 0.0f, 1.0f);
	MATRIX			Temp1;
	MATRIX			OldMat;
	VECTOR			CurrentPos;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	//	Get current Global Matrix
	OldMat		= _GMatrix;

	//	Get current global pos
	CurrentPos = _GMatrix.GetTranslation();

	//	Build temp1 matrix
	Temp1.Identity();
	Temp1.SetRotation(AxeZ, rAngle);

	//	Reset global matrix;
	_GMatrix.Identity();
	//	Rotate OldMat of Temp1's rotation
	_GMatrix.Mul( Temp1, OldMat );


	//	Now set pos
	_GMatrix.SetPosition( CurrentPos );

	//	Update local matrix
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		RollRight
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::RollRight(float	rAngle)
{
	VECTOR			AxeZ(0.0f, 0.0f, 1.0f);
	MATRIX			Temp1;
	MATRIX			OldMat;
	VECTOR			CurrentPos;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	//	Get current Global Matrix
	OldMat		= _GMatrix;

	//	Get current global pos
	CurrentPos = _GMatrix.GetTranslation();

	//	Build temp1 matrix
	Temp1.Identity();
	Temp1.SetRotation(AxeZ, -rAngle);

	//	Reset global matrix;
	_GMatrix.Identity();
	//	Rotate OldMat of Temp1's rotation
	_GMatrix.Mul( Temp1, OldMat );

	//	Now set pos
	_GMatrix.SetPosition( CurrentPos );

	//	Update local matrix
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		TransformVector
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
void	NODE::TransformVector(const VECTOR & vInput, VECTOR & vTransformed)
{
	MATRIX GMatrix = GetGlobalMatrix();
	GMatrix.TransformDirection(vInput, vTransformed);
}

//-----------------------------------------------------------------------------
//	Name:		MoveForward
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveForward(float	rValue)
{
	if (rValue == 0.0f) return;

	VECTOR		AxeZ2;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	// Rotate Translation 
	_GMatrix.TransformDirection( VECTOR::UNIT_Z, AxeZ2 );

	VECTOR NewPosition;
	NewPosition = _GMatrix.GetTranslation();
	NewPosition -= AxeZ2 * rValue;
	_GMatrix.SetPosition(NewPosition);
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		MoveBackward
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveBackward(float	rValue)
{
	if (rValue == 0.0f) return;

	VECTOR AxeZ2;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	// Rotate Translation 
	_GMatrix.TransformDirection( VECTOR::UNIT_Z, AxeZ2 );

	// Update translation
	VECTOR NewPosition;
	NewPosition = _GMatrix.GetTranslation();
	NewPosition += AxeZ2 * rValue;
	_GMatrix.SetPosition(NewPosition);
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		StrafeLeft
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::StrafeLeft(float	rValue)
{
	if (rValue == 0.0f) return;

	VECTOR	AxeX(-1.0f, 0.0f, 0.0f);
	VECTOR	AxeX2;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	// Rotate Translation 
	_GMatrix.TransformDirection( AxeX, AxeX2 );
	
	// Update translation
	VECTOR NewPosition;
	NewPosition = _GMatrix.GetTranslation();
	NewPosition += AxeX2 * rValue;
	_GMatrix.SetPosition(NewPosition);
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		StrafeRight
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::StrafeRight(float	rValue)
{
	if (rValue == 0.0f) return;

	VECTOR	AxeX(-1.0f, 0.0f, 0.0f);
	VECTOR	AxeX2;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	// Rotate Translation 
	_GMatrix.TransformDirection( AxeX, AxeX2 );

	// Update translation
	VECTOR NewPosition;
	NewPosition = _GMatrix.GetTranslation();
	NewPosition -= AxeX2 * rValue;
	_GMatrix.SetPosition(NewPosition);
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		MoveUp
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveUp(float	rValue)
{
	if (rValue == 0.0f) return;

	VECTOR AxeY2;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	// Rotate Translation 
	_GMatrix.TransformDirection( VECTOR::UNIT_Y, AxeY2 );

	// Update translation
	VECTOR NewPosition;
	NewPosition = _GMatrix.GetTranslation();
	NewPosition += AxeY2 * rValue;
	_GMatrix.SetPosition(NewPosition);
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		MoveDown
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::MoveDown(float	rValue)
{
	if (rValue == 0.0f) return;

	VECTOR AxeY2;

	// Make sure global matrix is valid
	if (!_Flags.IsSet(GLOBAL_MATRIX_VALID)) UpdateGlobalMatrix();

	// Rotate Translation 
	_GMatrix.TransformDirection( VECTOR::UNIT_Y, AxeY2 );

	// Update translation
	VECTOR NewPosition;
	NewPosition = _GMatrix.GetTranslation();
	NewPosition -= AxeY2 * rValue;
	_GMatrix.SetPosition(NewPosition);
	UpdateLocalMatrix();

	// Update flags
	SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
	_Flags.Set(GLOBAL_MATRIX_VALID);
	SetGlobalMatrixInvalidOnChild();
}

//-----------------------------------------------------------------------------
//	Name:		SetContainerOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetContainerOnChild()
{
	_Flags.Set(CONTAINER_ON_CHILD, true);

	if (GetParentNodeInst().HasHandle())
	{
		if (!GetParentNode()->IsSetContainerOnChild())
		{
			GetParentNode()->SetContainerOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UnsetContainerOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UnsetContainerOnChild()
{
	bool bFound = false;

	NODE *	pChild = GetFirstChild();
	while	(pChild != NULL)
	{
		if	(pChild->GetContainer() || pChild->IsSetContainerOnChild())
		{
			bFound = true;
			break;
		}

		pChild = pChild->GetBrother();
	}

	if (!bFound)
	{
		_Flags.Set(CONTAINER_ON_CHILD, false);

		if (GetParentNodeInst().HasHandle())
		{
			GetParentNode()->UnsetContainerOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetControllerOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetControllerOnChild()
{
	_Flags.Set(CONTROLLER_ON_CHILD, true);

	if (GetParentNodeInst().HasHandle())
	{
		if (!GetParentNode()->IsSetControllerOnChild())
		{
			GetParentNode()->SetControllerOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UnsetControllerOnChild
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UnsetControllerOnChild()
{
	bool bFound = false;

	NODE *	pChild = GetFirstChild();
	while	(pChild != NULL)
	{
		if	(pChild->HasController() || pChild->IsSetControllerOnChild())
		{
			bFound = true;
			break;
		}

		pChild = pChild->GetBrother();
	}

	if (!bFound)
	{
		_Flags.Set(CONTROLLER_ON_CHILD, false);

		if (GetParentNodeInst().HasHandle())
		{
			GetParentNode()->UnsetControllerOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalMatrixInvalidOnChild
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetGlobalMatrixInvalidOnChild()
{
	SetGlobalMatrixInvalidOnFathers();

	SetGlobalMatrixInvalidOnChildren();
}

//-----------------------------------------------------------------------------
//	Name:		UnsetGlobalMatrixInvalidOnChild
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UnsetGlobalMatrixInvalidOnChild()
{
	bool bFound = false;

	NODE *	pChild = GetFirstChild();
	while	(pChild != NULL)
	{
		if	((pChild->GetFlags().IsSet(GLOBAL_MATRIX_VALID) == false) || pChild->IsSetGlobalMatrixInvalidOnChild())
		{
			bFound = true;
			break;
		}

		pChild = pChild->GetBrother();
	}

	if (!bFound)
	{
		_Flags.Set(GLOBAL_MATRIX_INVALID_ON_CHILD, false);

		if (GetParentNodeInst().HasHandle())
		{
			GetParentNode()->UnsetGlobalMatrixInvalidOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalMatrixInvalidOnFathers
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetGlobalMatrixInvalidOnFathers()
{
	_Flags.Set(GLOBAL_MATRIX_INVALID_ON_CHILD, true);

	if (GetParentNodeInst().HasHandle() && GetParentNodeInst().IsLoaded())
	{
		if (!GetParentNode()->IsSetGlobalMatrixInvalidOnChild())
		{
			GetParentNode()->SetGlobalMatrixInvalidOnChild();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalMatrixInvalidOnChildren
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetGlobalMatrixInvalidOnChildren()
{
	if	(_FirstChildInst.HasHandle())
	{
		if   (IsSetGlobalMatrixInvalidOnChild() == false)
		{
		    _Flags.Set(GLOBAL_MATRIX_INVALID_ON_CHILD, true);

			NODE *	pChild = GetFirstChild();
			while	(pChild != NULL)
			{
				pChild->SetGlobalMatrixInvalidOnChildren();
				pChild = pChild->GetBrother();
			}
	    }
    }
}

//-----------------------------------------------------------------------------
// Name: NODE::SetGlobalPosition
// Object: Set the global position
// 00 - 11 - 13 JVA Created:
// 04 - 02 - 09 JPE Added Force option to bypass the "1mm" check.
//					Collision system always need maximum precision !
// 04 - 08 - 12 JPE Replaced != test with EqualWithError to increase precision to 0.000001f
//-----------------------------------------------------------------------------
void NODE::SetGlobalPosition(const VECTOR & vPos)
{
	VECTOR PreviousPos;
	GetGlobalPosition(PreviousPos);

	if (!vPos.EqualWithError(PreviousPos, 0.000001f))
	{
		if (!_Flags.IsSet(GLOBAL_MATRIX_VALID))
		{
			UpdateGlobalMatrix();
		}
		
		_GMatrix.SetPosition(vPos);
		
		UpdateLocalMatrix();
		
		SetFlagsTree(GLOBAL_MATRIX_VALID, 0);
		
		_Flags.Set(GLOBAL_MATRIX_VALID);

		SetGlobalMatrixInvalidOnChild();

		if ((_pDynamicActor) && !GetFlags().IsSet(PHYSIC_CONTROLLER))
		{
			bool b = _pDynamicActor->IsKinematicModeEnabled();
			_pDynamicActor->EnableKinematicMode(true);
			_pDynamicActor->SetGlobalMatrix(GetGlobalMatrix());
			_pDynamicActor->EnableKinematicMode(b);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	03-03-18:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::Instanciate(NODE *	pNewNode)
{
	pNewNode->SetGlobalMatrix(GetGlobalMatrix());
	pNewNode->SetLocalMatrix(GetLocalMatrix());
	pNewNode->GetFlags().SetFlags(GLOBAL_MATRIX_VALID, 0);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::InstanciateRec(NODE *	pNewNode)
{
	pNewNode->SetGlobalMatrix(GetGlobalMatrix());
	pNewNode->SetLocalMatrix(GetLocalMatrix());
	pNewNode->GetFlags().SetFlags(GLOBAL_MATRIX_VALID, 0);

	pNewNode->_ParentInst = _ParentInst;
	pNewNode->_FirstChildInst = _FirstChildInst;
	pNewNode->_BrotherInst = _BrotherInst;
}

//-----------------------------------------------------------------------------
//	Name:		PostInstanciate
//	Object:		
//	06-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::PostInstanciate(const QDT_VECTOR < QDT_PAIR < COM_INSTANCE, COM_INSTANCE > > &	vInstanciatedNodes)
{
	int	i, nNbrInstanciatedNodes = vInstanciatedNodes.GetSize();

	if	(_ParentInst.HasHandle())
	{
		for	(i = 0 ; i < nNbrInstanciatedNodes ; ++i)
		{
			QDT_PAIR<COM_INSTANCE, COM_INSTANCE> Pair = vInstanciatedNodes[i];

			if	(Pair.GetSecond() == _ParentInst)
			{
				_ParentInst = Pair.GetFirst();
				break;
			}
		}
	}

	if	(_FirstChildInst.HasHandle())
	{
		for	(i = 0 ; i < nNbrInstanciatedNodes ; ++i)
		{
			QDT_PAIR<COM_INSTANCE, COM_INSTANCE> Pair = vInstanciatedNodes[i];

			if	(Pair.GetSecond() == _FirstChildInst)
			{
				_FirstChildInst = Pair.GetFirst();
				break;
			}
		}
	}

	
	if	(_BrotherInst.HasHandle())
	{
		for	(i = 0 ; i < nNbrInstanciatedNodes ; ++i)
		{
			QDT_PAIR<COM_INSTANCE, COM_INSTANCE> Pair = vInstanciatedNodes[i];

			if	(Pair.GetSecond() == _BrotherInst)
			{
				_BrotherInst = Pair.GetFirst();
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFrontDirAngle
//	Object:		
//	03-01-17:	GGO - Created
//-----------------------------------------------------------------------------
float	NODE::GetFrontDirAngle()
{
	QUATERNION	QuatTmp, QuatHead;
	VECTOR		TmpVector;
	float		rAngle;

	GetPitchHeadRoll( QuatTmp, QuatHead, QuatTmp );

	QuatHead.GetVectRoll( TmpVector, rAngle );

	return ( rAngle );
}

//-----------------------------------------------------------------------------
//	Name:		TransformVector
//	Object:		
//	03-08-01:	FDE - Created
//-----------------------------------------------------------------------------
void	NODE::TransformVector(VECTOR &	Vect)
{
	MATRIX GMatrix = GetGlobalMatrix();
	GMatrix.TransformDirection(Vect);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("NODE    ", 1 ,CM_ReadChunk);
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("NODE    ", 1 ,CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NODE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NODE::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("NODE"),								
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE,			
										QDT_COMMENT("30E614F8-EB5D-505B-18DC-C8E4CC29F846"),			
										sizeof(NODE),										
										ComInit,											
										QDT_COMMENT("Class for node. A node contain information such as relative position, global position. A node is connected to an entity (see ENTITY_ABC). Each node are connected with a parent node and children node to form a tree which is the base of the scene graph."),
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::EM_Construct(void * const	pO,
						   void * const	pR,
						   void * const	pP)
{
	NODE *pNode;
	pNode = static_cast<NODE *>(pO);

	QDT_NEW_PLACED(pNode, NODE)();
	QDT_ASSERT(pNode);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::EM_Destruct(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	NODE *pNode;
	pNode = static_cast<NODE *>(pO);


	QDT_DELETE_PLACED(pNode, NODE);
	QDT_ASSERT(pNode);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::CM_ReadChunk(IO_STREAM &		Stream,
						   COMMUNICATOR *	pCommunicator)
{
	NODE *pNode;
	pNode = static_cast<NODE *>(pCommunicator);

	COM_INSTANCE ParentInst, FirstChildInst, BrotherInst;
	
	Stream	>> pNode->_Flags				// Flags
			>> pNode->_LMatrix				// Local matrix
			>> pNode->_GMatrix				// Global matrix
			>> ParentInst					// Parent
			>> FirstChildInst				// First child
			>> BrotherInst;					// Brother
	
	pNode->_ParentInst = ParentInst;
	pNode->_FirstChildInst = FirstChildInst;
	pNode->_BrotherInst = BrotherInst;
	
	pNode->_Flags.Set(UNTRANSFORMED);
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-26:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE::CM_WriteChunk(IO_STREAM &	Stream,
							COMMUNICATOR *	pCommunicator)
{
	NODE *pNode;
	pNode = static_cast<NODE *>(pCommunicator);

	Stream.DebugOutput("Node :\t");
	Stream.DebugOutput(pNode->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Flags :\t");
	Stream	<< pNode->_Flags;		// Flags
	Stream.DebugOutput("\t");
	Stream.DebugOutputBinary(pNode->_Flags.AsInt());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Local matrix :\t");
	Stream.DebugOutput("\r\n");
	Stream	<< pNode->_LMatrix;		// Local matrix
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Global matrix :\t");
	Stream.DebugOutput("\r\n");
	Stream	<< pNode->_GMatrix;		// Global matrix
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Parent :\t");
	if (pNode->_ParentInst.HasHandle())
	{
		Stream.DebugOutput(pNode->_ParentInst.GetName());
		Stream.DebugOutput("\t");
	}
	Stream	<< pNode->_ParentInst;		// Parent node ID
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("First child :\t");
	if (pNode->_FirstChildInst.HasHandle())
	{
		Stream.DebugOutput(pNode->_FirstChildInst.GetName());
		Stream.DebugOutput("\t");
	}
	Stream	<< pNode->_FirstChildInst;		// First child node ID
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Brother :\t");
	if (pNode->_BrotherInst.HasHandle())
	{
		Stream.DebugOutput(pNode->_BrotherInst.GetName());
		Stream.DebugOutput("\t");
	}
	Stream	<< pNode->_BrotherInst;		// Brother node ID
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		AddContentToCommunicatorContainer
//	Object:		
//	04-01-29:	VMA - Created
//-----------------------------------------------------------------------------
void	NODE::AddContentToCommunicatorContainer(COMMUNICATOR_CONTAINER *	pContainer) const
{
	pContainer->PushComHandle( GetHandle() );

	NODE * pFirstChild = GetFirstChild();
	if	(pFirstChild)
	{
		pFirstChild->AddContentToCommunicatorContainer(pContainer);
	}

	NODE * pBrother = GetBrother();
	if	(pBrother)
	{
		pBrother->AddContentToCommunicatorContainer(pContainer);
	}
}

//-----------------------------------------------------------------------------
//	Name:		OnCloseObject
//	Object:		
//	03-10-28:	CVI - Created
//  03-12-04:	RMA - Removed BreakConnectionsRec because it seems to be useless
//-----------------------------------------------------------------------------
void	NODE::OnCloseObject()
{
	// BreakConnectionsRec();

	//	Verify that only caller may have a reference on this
	// QDT_ASSERT( GetHandle()->GetNbrReferences() <= 1 );
}

//-----------------------------------------------------------------------------
//	Name:		PlayTargetAnim
//	Object:		
//	03-11-17:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::PlayTargetAnim(const COM_INSTANCE &	TargetAnim)
{
	if ( !_pContainer )
	{
		_pContainer = new CONTAINER();
		_pContainer->SetHolderNode( this );
	}
	_pContainer->PlayTargetAnim( TargetAnim );
}

//-----------------------------------------------------------------------------
//	Name:		PlayPartAnim
//	Object:		
//	03-12-02:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::PlayPartAnim(const COM_INSTANCE &	PartAnim)
{
	if ( !_pContainer )
	{
		_pContainer = new CONTAINER();
		_pContainer->SetHolderNode( this );
	}
	_pContainer->PlayPartAnim( PartAnim );
}

//-----------------------------------------------------------------------------
//	Name:		ActiveAnimMix
//	Object:		
//	03-12-03:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::ActiveAnimMix(bool	bVal,
							bool	bBlend)
{
	if ( !_pContainer )	return;
	_pContainer->ProcessPartAnimActivation( bVal, bBlend );
}

//-----------------------------------------------------------------------------
//	Name:		GetPotentialNbrTracks
//	Object:		
//	04-01-19:	CVI - Created
//-----------------------------------------------------------------------------
int	NODE::GetPotentialNbrTracks() const
{
	//	one potential track only
	return ( 1 );
}

//-----------------------------------------------------------------------------
//	Name:		PurgeAllAnimConnections
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::PurgeAllAnimConnections()
{
	if ( !_pContainer )	return;

	_pContainer->DeleteAllAnimation();
	if (BEHAVIOR_MANAGER::Instance())
	{
		BEHAVIOR_MANAGER::Instance()->RemoveAnimatedNode(COM_INSTANCE(this));
	}
}

//-----------------------------------------------------------------------------
//	Name:		PurgeMainAnimConnection
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::PurgeMainAnimConnection()
{
	if ( !_pContainer )	return;

	_pContainer->DeleteMainAnimConnection();
}

//-----------------------------------------------------------------------------
//	Name:		PurgeTargetAnimConnection
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::PurgeTargetAnimConnection()
{
	if ( !_pContainer )	return;

	_pContainer->DeleteTargetConnection();
}

//-----------------------------------------------------------------------------
//	Name:		PurgeAnimMixConnection
//	Object:		
//	04-02-23:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::PurgeAnimMixConnection()
{
	if ( !_pContainer )	return;

	_pContainer->DeleteAnimMixConnection();
}

//-----------------------------------------------------------------------------
//	Name:		FootStep
//	Object:		
//	04-03-16:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::FootStep(int		nGroundPointIdx,
					   float	rGroundHeight)
{
	// Do nothing (only on NODE_ARRAY)
}

//-----------------------------------------------------------------------------
//	Name:		FindNode
//	Object:		I've build this method to avoid code duplication in exe with
//				the template method
//	04-03-19:	RMA - Created
//-----------------------------------------------------------------------------
QDT_VECTOR<NODE*>::ITERATOR	NODE::Find(const QDT_VECTOR <NODE *>	&v,
									   NODE *						pN)
{
	return (v.Find(v.GetHead(), pN));
}

//-----------------------------------------------------------------------------
//	Name:		FindInList
//	Object:		
//	04-07-20:	RMA - Created
//-----------------------------------------------------------------------------
QDT_DLIST < NODE * > ::ITERATOR	NODE::FindInList(const QDT_DLIST < NODE * > &	l,
												 NODE *							pN)
{
	return (l.Find(l.GetHead(), pN));
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetDistanceFromCamera
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
float	NODE::GetDistanceFromCamera(unsigned int nGeometry)
{
	QDT_ASSERT(_EntityInst.HasHandle());
	return (GetEntity()->GetDistanceFromCamera(nGeometry, this));
}

//-----------------------------------------------------------------------------
//	Name:		SaveState
//	Object:		
//	04-11-19:	RMA - Created
//-----------------------------------------------------------------------------
void NODE::SaveState(QDT_VECTOR<COM_INSTANCE> & vNodes, 
					 QDT_VECTOR<MATRIX> & vMatrices, 
					 QDT_VECTOR<COM_INSTANCE> & vParentNodes)
{
	if (_EntityInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_MESH ||
		_EntityInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_QD_TRANSFORM)
	{
		vNodes.PushTail(COM_INSTANCE(GetHandle()));
		vMatrices.PushTail(GetGlobalMatrix());
		vParentNodes.PushTail(GetParentNodeInst());
	}

	unsigned int i, nCount = GetNbrChildren();
	for (i = 0 ; i < nCount ; ++i)
	{
		GetChild(i)->SaveState(vNodes, vMatrices, vParentNodes);
	}
}
*/
//-----------------------------------------------------------------------------
//	Name:		UnregisterDynamicActor
//	Object:		
//	05-08-29:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::UnregisterDynamicActor(I_PHYSIC_SCENE *	pPhysicScene)
{
	if (_pDynamicActor)
	{
		bool	bUnregistered = true;

		if	(_bSklPhysicJoint)
		{
			bUnregistered = _pDynamicActor->Unregister(pPhysicScene);
		}

		if	(bUnregistered)
		{
			PHYSIC_CONTEXT::Instance()->ReleaseActor(_pDynamicActor);
			_pDynamicActor = NULL;

			PHYSIC_MANAGER::Instance()->RemoveNodeWithPhysics(this);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ApplyForce
//	Object:		
//	05-01-07:	RMA - Created
//-----------------------------------------------------------------------------
void NODE::ApplyForce(const VECTOR & ForceDir, 
					  float rForceStrength)
{
	if (_pDynamicActor)
	{
		_pDynamicActor->ApplyForce(ForceDir, rForceStrength);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ApplyTorque
//	Object:		
//	05-01-13:	RMA - Created
//-----------------------------------------------------------------------------
void NODE::ApplyTorque(const VECTOR & ForceDir, 
					   float rForceStrength)
{
	if (_pDynamicActor)
	{
		_pDynamicActor->ApplyTorque(ForceDir, rForceStrength);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableKinematic
//	Object:		
//	05-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::EnableKinematic(bool b)
{
	if (_pDynamicActor)
	{
		_pDynamicActor->EnableKinematicMode(b);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsKinematicEnabled
//	Object:		
//	05-02-03:	RMA - Created
//-----------------------------------------------------------------------------
bool	NODE::IsKinematicEnabled() const
{
	if (_pDynamicActor)
	{
		return (_pDynamicActor->IsKinematicModeEnabled());
	}

	// No physic actor, we can considere that this mode is enabled
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IsSklPhysicJoint
//	Object:		
//	05-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	NODE::IsSklPhysicJoint() const
{
	return	(_bSklPhysicJoint);
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearDamping
//	Object:		
//	05-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::SetLinearDamping(float rDamping)
{
	if (_pDynamicActor)
	{
		_pDynamicActor->SetLinearDamping(rDamping);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearDamping
//	Object:		
//	05-02-03:	RMA - Created
//-----------------------------------------------------------------------------
float	NODE::GetLinearDamping() const
	{
	if (_pDynamicActor)
		{
		return (_pDynamicActor->GetLinearDamping());
	}

	return (0.0f);
		}

//-----------------------------------------------------------------------------
//	Name:		SetAngularDampling
//	Object:		
//	05-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE::SetAngularDamping(float rDamping)
{
	if (_pDynamicActor)
		{
		_pDynamicActor->SetAngularDamping(rDamping);
		}
	}

//-----------------------------------------------------------------------------
//	Name:		GetAngularDampling
//	Object:		
//	05-02-03:	RMA - Created
//-----------------------------------------------------------------------------
float	NODE::GetAngularDamping() const
{
	if (_pDynamicActor)
	{
		return (_pDynamicActor->GetAngularDamping());
	}

	return (0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		InitPhysicJoint
// Object:		
//	05-07-08:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::InitPhysicJoint(NODE_CHILD *		pNodeChild,
							  I_PHYSIC_SCENE *	pPhysicScene)
{
	Int32	nJointIndex = pNodeChild->GetIndex();
	NODE	*pFather = NULL;

	if	(nJointIndex != 0)
	{
		pFather = static_cast<NODE *>(_ParentInst.GetCommunicator());
	}

	// If we are here, this node will not be added in the scene graph
	_bSklPhysicJoint = true;

	switch	(nJointIndex)
	{
		case	0:
		{
/*			CreatePhysicDynamicActor();

			float	rRadius;
			VECTOR	LocalPos(0.0f, 7.5f, 0.0f);
			VECTOR	FirstChildPos, Pos, V;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			V.Sub(FirstChildPos, Pos);
			rRadius = V.Length() * 0.5f + 1.5f;// + 10.0f;

			I_PHYSIC_SHAPE_SPHERE * pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();
			pSphere->SetRadius(rRadius);
			pSphere->SetLocalPosition(LocalPos);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pSphere);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->EnableKinematicMode(true);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);*/
		}
		break;

		case	1:
		case	2:
		case	3:
		{
/*			CreatePhysicDynamicActor();

			float	rRadius;
			VECTOR	LocalPos(0.0f, 0.0f, 0.0f);
			VECTOR	FirstChildPos, Pos, V;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			V.Sub(FirstChildPos, Pos);
//			if	(nJointIndex == 3)
//			{
//				rRadius = 1.0f;
//			}
//			else
			{
				rRadius = V.Length() * 0.8f;
			}
			LocalPos.SetY(rRadius);

			I_PHYSIC_SHAPE_SPHERE * pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();
			pSphere->SetRadius(rRadius);
			pSphere->SetLocalPosition(LocalPos);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pSphere);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			if	(nJointIndex == 1)
			{
				_pDynamicActor->EnableKinematicMode(true);

				I_PHYSIC_JOINT_SPHERICAL * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointSpherical();
				pPhysicJoint->SetActor1(pFather->GetDynamicActor());
				pPhysicJoint->SetActor2(_pDynamicActor);
				pPhysicJoint->SetGlobalAnchor(Pos);
				pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, -1.0f, 0.0f));
				pPhysicJoint->EnableCollision(false);
				pPhysicJoint->EnableTwistLimit(true);
				pPhysicJoint->SetTwistLimitLowValue(-0.1f * MATH::PI());
				pPhysicJoint->SetTwistLimitHighValue(0.1f * MATH::PI());
				pPhysicJoint->EnableSwingLimit(true);
				pPhysicJoint->SetSwingLimitValue(0.1f * MATH::PI());
				pPhysicScene->RegisterJoint(pPhysicJoint);
			}
			else
			{
				_pDynamicActor->EnableKinematicMode(true);

				I_PHYSIC_JOINT_FIXED * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointFixed();
				pPhysicJoint->SetActor1(pFather->GetDynamicActor());
				pPhysicJoint->SetActor2(_pDynamicActor);
				pPhysicJoint->SetGlobalAnchor(Pos);
				pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, -1.0f, 0.0f));
				pPhysicJoint->EnableCollision(false);
				pPhysicScene->RegisterJoint(pPhysicJoint);
			}*/
		}
		break;

		case	4:
		{
			CreatePhysicDynamicActor();

			float	rRadius = 10.0f;
			float	rHeight;
			VECTOR	LocalPos(0.0f, 0.0f, 3.0f);
			VECTOR	FirstChildPos, Pos, V;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			V.Sub(FirstChildPos, Pos);
			rHeight = V.Length();
			LocalPos.SetY(rHeight * 0.5f + 3.0f);

			I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
			pCapsule->SetRadius(rRadius);
			pCapsule->SetHeight(rHeight - 2.5f);
			pCapsule->SetLocalPosition(LocalPos);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pCapsule);
//			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetMass(1000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			_pDynamicActor->EnableKinematicMode(true);

			I_PHYSIC_JOINT_FIXED * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointFixed();
			pPhysicJoint->SetActor1(pFather->GetDynamicActor());
			pPhysicJoint->SetActor2(_pDynamicActor);
			pPhysicJoint->SetGlobalAnchor(Pos);
			pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, -1.0f, 0.0f));
			pPhysicJoint->EnableCollision(false);
			pPhysicScene->RegisterJoint(pPhysicJoint);
		}
		break;

		case	7:
		case	19:
		{
/*			CreatePhysicDynamicActor();

			float	rRadius;
			VECTOR	LocalPos(0.0f, 0.0f, 0.0f);
			VECTOR	FirstChildPos, Pos, V;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			V.Sub(FirstChildPos, Pos);
			rRadius = V.Length() * 0.5f;
			LocalPos = V * 0.5f;

			I_PHYSIC_SHAPE_SPHERE * pSphere = PHYSIC_CONTEXT::Instance()->CreateShapeSphere();
			pSphere->SetRadius(rRadius);
			pSphere->SetLocalPosition(LocalPos);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pSphere);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->EnableKinematicMode(false);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			I_PHYSIC_JOINT_REVOLUTE * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointRevolute();
			pPhysicJoint->SetActor1(pFather->GetDynamicActor());
			pPhysicJoint->SetActor2(_pDynamicActor);
			pPhysicJoint->SetGlobalAnchor(Pos);
			pPhysicJoint->EnableCollision(false);
			pPhysicJoint->EnableLimit(true);
			pPhysicJoint->SetLimitLowValue(-0.5f * MATH::PI());
			pPhysicJoint->SetLimitHighValue(-0.5f * MATH::PI());
			if	(nJointIndex == 7)
			{
				pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, 0.0f, -1.0f));
				pPhysicJoint->SetLimitLowValue(-MATH::PI());
				pPhysicJoint->SetLimitHighValue(-0.5f * MATH::PI());
			}
			else
			{
				pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, 0.0f, 1.0f));
				pPhysicJoint->SetLimitLowValue(-MATH::PI());
				pPhysicJoint->SetLimitHighValue(-0.5f * MATH::PI());
			}
			pPhysicScene->RegisterJoint(pPhysicJoint);*/

			pNodeChild->EnableKinematic(false);
		}
		break;

		case	8:
		case	10:
		case	20:
		case	21:
		{
/*			CreatePhysicDynamicActor();

			float		rRadius = 5.0f;
			float		rHeight;
			VECTOR		LocalPos;
			VECTOR		FirstChildPos, Pos, V;
			MATRIX		M;
			QUATERNION	Q;

			GetGlobalPosition(Pos);
			if	(nJointIndex == 8)
			{
				GetFirstChild()->GetBrother()->GetGlobalPosition(FirstChildPos);
			}
			else
			{
				GetFirstChild()->GetGlobalPosition(FirstChildPos);
			}

			V.Sub(FirstChildPos, Pos);
			rHeight = V.Length() - (rRadius * 2.0f);
			LocalPos = V * 0.5f;

			Q.SetVectRoll(VECTOR(0.0f, 0.0f, 1.0f), MATH::PI() * 0.5f);

			M.SetQuaternion(Q);
			M.SetTranslation(LocalPos);

			I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
			pCapsule->SetRadius(rRadius);
			pCapsule->SetHeight(rHeight);
			pCapsule->SetLocalMatrix(M);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pCapsule);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			if	((nJointIndex == 8) || (nJointIndex == 20))
			{
				_pDynamicActor->EnableKinematicMode(false);

				I_PHYSIC_JOINT_SPHERICAL * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointSpherical();
				pPhysicJoint->SetActor1(pFather->GetParentNode()->GetDynamicActor());
				pPhysicJoint->SetActor2(_pDynamicActor);
				pPhysicJoint->SetGlobalAnchor(Pos);
				if	(nJointIndex == 8)
				{
					pPhysicJoint->SetGlobalAxis(VECTOR(1.0f, 0.0f, 0.0f));
				}
				else
				{
					pPhysicJoint->SetGlobalAxis(VECTOR(-1.0f, 0.0f, 0.0f));
				}
				pPhysicJoint->EnableCollision(false);
				pPhysicJoint->EnableTwistLimit(true);
				pPhysicJoint->SetTwistLimitLowValue(0.0f);
				pPhysicJoint->SetTwistLimitHighValue(0.0f);
				pPhysicJoint->EnableSwingLimit(true);
				pPhysicJoint->SetSwingLimitValue(0.55f * MATH::PI());
				pPhysicScene->RegisterJoint(pPhysicJoint);

				VECTOR	LimitPoint = Pos;
				VECTOR	PlaneNormal;
				VECTOR	PlanePoint = Pos;
				
				LimitPoint.SetY(LimitPoint.GetY() + rRadius * 0.5f);
				PlanePoint.SetY(PlanePoint.GetY() + rRadius * 0.5f);
				if	(nJointIndex == 8)
				{
					LimitPoint.SetX(LimitPoint.GetX() - rHeight);
					PlaneNormal.SetXYZ(0.0f, -1.0f, 0.0f);
				}
				else
				{
					LimitPoint.SetX(LimitPoint.GetX() + rHeight);
					PlaneNormal.SetXYZ(0.0f, -1.0f, 0.0f);
				}

				pPhysicJoint->SetLimitPoint(LimitPoint);
				pPhysicJoint->AddLimitPlane(PlaneNormal, PlanePoint);
			}
			else
			{
				_pDynamicActor->EnableKinematicMode(false);

				I_PHYSIC_JOINT_REVOLUTE * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointRevolute();
				pPhysicJoint->SetActor1(pFather->GetDynamicActor());
				pPhysicJoint->SetActor2(_pDynamicActor);
				pPhysicJoint->SetGlobalAnchor(Pos);
				if	(nJointIndex == 10)
				{
					pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, -1.0f, 0.0f));
				}
				else
				{
					pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, 1.0f, 0.0f));
				}
				pPhysicJoint->EnableCollision(false);
				pPhysicJoint->EnableLimit(true);
				pPhysicJoint->SetLimitLowValue(0.0f);
				pPhysicJoint->SetLimitHighValue(0.8f * MATH::PI());
//				pPhysicJoint->EnableSpring(true);
//				pPhysicJoint->SetSpringValue(5000.0f);
//				pPhysicJoint->SetSpringDamper(50.0f);
//				pPhysicJoint->SetSpringTargetAngle(0.075f * MATH::PI());
				pPhysicScene->RegisterJoint(pPhysicJoint);
			}*/
		}
		break;

		case	31:
		case	32:
		case	35:
		case	36:
		{
/*			CreatePhysicDynamicActor();

			float	rRadius = 7.0f;
			float	rHeight;
			VECTOR	LocalPos;
			VECTOR	FirstChildPos, Pos, V;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			V.Sub(FirstChildPos, Pos);
			rHeight = V.Length() - (rRadius * 2.0f);
			LocalPos = V / 2.0f;

			I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
			pCapsule->SetRadius(rRadius);
			pCapsule->SetHeight(rHeight);
			pCapsule->SetLocalPosition(LocalPos);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pCapsule);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			if	((nJointIndex == 31) || (nJointIndex == 35))
			{
				_pDynamicActor->EnableKinematicMode(false);

				I_PHYSIC_JOINT_SPHERICAL * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointSpherical();
				pPhysicJoint->SetActor1(pFather->GetDynamicActor());
				pPhysicJoint->SetActor2(_pDynamicActor);
				pPhysicJoint->SetGlobalAnchor(Pos);
				pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, 1.0f, 0.0f));
				pPhysicJoint->EnableCollision(false);
				pPhysicJoint->EnableTwistLimit(true);
				pPhysicJoint->SetTwistLimitLowValue(-0.05f * MATH::PI());
				pPhysicJoint->SetTwistLimitHighValue(0.05f * MATH::PI());
				pPhysicJoint->EnableSwingLimit(true);
				pPhysicJoint->SetSwingLimitValue(0.35f * MATH::PI());
				pPhysicScene->RegisterJoint(pPhysicJoint);

				VECTOR	LimitPoint = Pos;
				VECTOR	PlaneNormal;
				VECTOR	PlanePoint = Pos;
				
				LimitPoint.SetY(LimitPoint.GetY() - rHeight);
				if	(nJointIndex == 31)
				{
					LimitPoint.SetX(LimitPoint.GetX() - rRadius);

					PlaneNormal.SetXYZ(-0.9f, -0.1f, 0.0f);
					PlaneNormal.Normalize();
					PlanePoint.SetX(PlanePoint.GetX() - rRadius);
				}
				else
				{
					LimitPoint.SetX(LimitPoint.GetX() + rRadius);

					PlaneNormal.SetXYZ(0.9f, -0.1f, 0.0f);
					PlaneNormal.Normalize();
					PlanePoint.SetX(PlanePoint.GetX() + rRadius);
				}

				pPhysicJoint->SetLimitPoint(LimitPoint);
				pPhysicJoint->AddLimitPlane(PlaneNormal, PlanePoint);
			}
			else
			{	
				_pDynamicActor->EnableKinematicMode(false);

				I_PHYSIC_JOINT_REVOLUTE * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointRevolute();
				pPhysicJoint->SetActor1(pFather->GetDynamicActor());
				pPhysicJoint->SetActor2(_pDynamicActor);
				pPhysicJoint->SetGlobalAnchor(Pos);
				pPhysicJoint->SetGlobalAxis(VECTOR(-1.0f, 0.0f, 0.0f));
				pPhysicJoint->EnableCollision(false);
				pPhysicJoint->EnableLimit(true);
				pPhysicJoint->SetLimitLowValue(0.0f);
				pPhysicJoint->SetLimitHighValue(0.9f * MATH::PI());
				pPhysicScene->RegisterJoint(pPhysicJoint);
			}*/
		}
		break;

		case	11:
		case	22:
		{
/*			CreatePhysicDynamicActor();

			float		rHeight, rRadius = 5.0f;
			VECTOR		LocalPos, FirstChildPos, Pos, V;
			MATRIX		M;
			QUATERNION	Q;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			if	(nJointIndex == 11)
			{
				LocalPos.SetXYZ(-5.0f, -2.0f, -1.5f);
			}
			else
			{
				LocalPos.SetXYZ(5.0f, -2.0f, -1.5f);
			}

			V.Sub(FirstChildPos, Pos);
			rHeight = V.Length() - (2.0f * rRadius) + 10.0f;
			LocalPos.SetX(LocalPos.GetX() + V.GetX() * 0.5f);
			LocalPos.SetY(LocalPos.GetY() + V.GetY() * 0.5f);
			LocalPos.SetZ(LocalPos.GetZ() + V.GetZ() * 0.5f);

			Q.SetVectRoll(VECTOR(0.0f, 0.0f, 1.0f), MATH::PI() * 0.5f);

			M.SetQuaternion(Q);
			M.SetTranslation(LocalPos);

			I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
			pCapsule->SetRadius(rRadius);
			pCapsule->SetHeight(rHeight);
			pCapsule->SetLocalMatrix(M);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pCapsule);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			_pDynamicActor->EnableKinematicMode(false);

			I_PHYSIC_JOINT_FIXED * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointFixed();
			pPhysicJoint->SetActor1(pFather->GetDynamicActor());
			pPhysicJoint->SetActor2(_pDynamicActor);
			pPhysicJoint->SetGlobalAnchor(Pos);
			pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, -1.0f, 0.0f));
			pPhysicJoint->EnableCollision(false);
			pPhysicScene->RegisterJoint(pPhysicJoint);*/
		}
		break;

		case	33:
		case	37:
		{
/*			CreatePhysicDynamicActor();

			float		rHeight, rRadius = 6.0f;
			VECTOR		LocalPos, FirstChildPos, Pos, V;
			MATRIX		M;
			QUATERNION	Q;

			GetGlobalPosition(Pos);
			GetFirstChild()->GetGlobalPosition(FirstChildPos);

			if	(nJointIndex == 33)
			{
				LocalPos.SetXYZ(-3.0f, 0.0f, 0.0f);
			}
			else
			{
				LocalPos.SetXYZ(3.0f, 0.0f, 0.0f);
			}

			V.Sub(FirstChildPos, Pos);
			rHeight = V.Length() + 10.0f;
			LocalPos.SetX(LocalPos.GetX() + V.GetX() * 0.5f);
			LocalPos.SetY(LocalPos.GetY() + V.GetY() * 0.5f);
			LocalPos.SetZ(LocalPos.GetZ() + V.GetZ() * 0.5f);

			Q.SetVectRoll(VECTOR(1.0f, 0.0f, 0.0f), MATH::PI() * 0.6f);

			M.SetQuaternion(Q);
			M.SetTranslation(LocalPos);

			I_PHYSIC_SHAPE_CAPSULE * pCapsule = PHYSIC_CONTEXT::Instance()->CreateShapeCapsule();
			pCapsule->SetRadius(rRadius);
			pCapsule->SetHeight(rHeight);
			pCapsule->SetLocalMatrix(M);

			_pDynamicActor->SetCollisionGroup(GROUP_JOINT);
			_pDynamicActor->PushShape(pCapsule);
			_pDynamicActor->SetDensity(2000.0f);
			_pDynamicActor->SetGlobalMatrix(_GMatrix);
			_pDynamicActor->SetLinearDamping(1.0f);
			_pDynamicActor->SetAngularDamping(1.0f);
			pPhysicScene->RegisterDynamicActor(_pDynamicActor);

			_pDynamicActor->EnableKinematicMode(false);

			I_PHYSIC_JOINT_FIXED * pPhysicJoint = PHYSIC_CONTEXT::Instance()->CreateJointFixed();
			pPhysicJoint->SetActor1(pFather->GetDynamicActor());
			pPhysicJoint->SetActor2(_pDynamicActor);
			pPhysicJoint->SetGlobalAnchor(Pos);
			pPhysicJoint->SetGlobalAxis(VECTOR(0.0f, -1.0f, 0.0f));
			pPhysicJoint->EnableCollision(false);
			pPhysicScene->RegisterJoint(pPhysicJoint);*/
		}
		break;

		default:
		break;
	}

	if	(_pDynamicActor)
	{
		_pDynamicActor->EnableKinematicMode(true);
	}
	else
	{
		pNodeChild->EnableKinematic(true);
	}

	pNodeChild->SetDynamicActor(_pDynamicActor);
	if	(_pDynamicActor)
	{
		_pDynamicActor->SetUserData(I_PHYSIC_ACTOR::DATA_TYPE_NODECHILD, pNodeChild);
		pNodeChild->EnableKinematic(_pDynamicActor->IsKinematicModeEnabled());

		PHYSIC_MANAGER::Instance()->AddNodeWithPhysics(this);
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreatePhysicDynamicActor
//	Object:		
//	05-04-20:	RMA - Created
//-----------------------------------------------------------------------------
I_PHYSIC_DYNAMIC_ACTOR *	NODE::CreatePhysicDynamicActor()
{
	_pDynamicActor = PHYSIC_CONTEXT::Instance()->CreateDynamicActor();
	_pDynamicActor->SetUserData(I_PHYSIC_ACTOR::DATA_TYPE_NODE, this);

	return (_pDynamicActor);
}

//-----------------------------------------------------------------------------
//	Name:		CreatePhysicController
//	Object:		
//	05-06-30:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_CONTROLLER *	NODE::CreatePhysicController(PHYSIC_CONTROLLER_TYPE	Type)
{
	_pPhysicController = PHYSIC_CONTEXT::Instance()->GetControllerManager()->CreateController(Type);
	_pPhysicController->SetUserData(this);

	return (_pPhysicController);
}

//-----------------------------------------------------------------------------
//	Name:		DeletePhysicController
//	Object:		
//	05-07-21:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::DeletePhysicController()
{
	if	(_pPhysicController)
	{
		PHYSIC_MANAGER::Instance()->RemoveNodeWithPhysics(this);

		PHYSIC_CONTEXT::Instance()->GetControllerManager()->ReleaseController(_pPhysicController);
		_pPhysicController = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name:		NODE::SetTestFatherVisibilityOnChild
// Object:		
// 05-04-11		RMA Created:
//-----------------------------------------------------------------------------
void	NODE::SetTestFatherVisibilityOnChild(bool b)
{
	QDT_NOT_IMPLEMENTED();
/*
	unsigned int i, nNbrChildren = _vChildInst.GetSize();
	for (i = 0 ; i < nNbrChildren ; ++i)
	{
		if (GetChildInst(i).HasHandle() && GetChildInst(i).IsLoaded())
		{
			NODE * pChild = GetChild(i);
			if (pChild->GetEntityInst().HasHandle() && pChild->GetEntityInst().IsLoaded())
			{
				if (pChild->GetEntityType() == ENTITY_MESH)
				{	
					MESH * pMesh = (MESH *)pChild->GetEntity();
					pMesh->SetTestFatherVisibility(b);
				}
			}

			pChild->SetTestFatherVisibilityOnChild(b);
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		UpdateInsideTree
//	Object:		
//	05-05-03:	CVI - Created
//-----------------------------------------------------------------------------
void	NODE::UpdateInsideTree()
{
	_Flags.Set(INSIDE_TREE_VALID);
}

//-----------------------------------------------------------------------------
//	Name:		Move
//	Object:		
//	05-09-02:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::Move()
{
	if	(_pDynamicActor && _pDynamicActor->IsKinematicModeEnabled())
	{
//		_pDynamicActor->MoveGlobalPosition(_GMatrix);
//		_pDynamicActor->SetGlobalMatrix(_GMatrix);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAttachJoint
//	Object:		
//	05-09-09:	ELE - Created
//-----------------------------------------------------------------------------
void	NODE::SetAttachJoint(I_PHYSIC_JOINT *	pJoint)
{
	_pAttachJoint = pJoint;
}

//-----------------------------------------------------------------------------
//	Name:		GetAttachJoint
//	Object:		
//	05-09-09:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_JOINT *	NODE::GetAttachJoint()
{
	return	(_pAttachJoint);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
