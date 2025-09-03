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
//	CLASS:	CONSTRAINT
//
//	06-03-13:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Constraint/Constraint)
#include	INCL_3DENGINE(SceneGraph/Entities/Constraint/ConstraintData)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KPHYSICS(Physics/IPhysicJointCustom)

#ifdef _DEBUG
	#include	"Constraint.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CONSTRAINT constructor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
CONSTRAINT::CONSTRAINT()
:
ENTITY_ABC		(GetGenericCommunicatorDescriptor(), ENTITY_CONSTRAINT),
_pJointCustom	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CONSTRAINT destructor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
CONSTRAINT::~CONSTRAINT()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::Register()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		Init();

		if	(_pJointCustom)
		{
			pSG->GetPhysicScene()->RegisterJoint(_pJointCustom);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::Unregister()
{
	SCENE_GRAPH * pSG = GetSceneGraph();

	if	(pSG)
	{
		if	(_pJointCustom)
		{
			pSG->GetPhysicScene()->UnregisterJoint(_pJointCustom);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::Instanciate(COM_INSTANCE &	Instance)
{
	CONSTRAINT *	pNewConstraint;

	Instance.SetHandle(CONSTRAINT::GetGenericCommunicatorDescriptor()->Create());
	pNewConstraint = static_cast<CONSTRAINT *>(Instance.GetCommunicator());

	// Copy needed infos for instanciate
	pNewConstraint->InitConstraintData(_ConstraintDataCI);
	pNewConstraint->_Actor1CI = _Actor1CI;
	pNewConstraint->_Actor2CI = _Actor2CI;
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::InstanciateRec(COM_INSTANCE &	Instance)
{
	Instanciate(Instance);
}

//-----------------------------------------------------------------------------
//	Name:		PostInstanciate
//	Object:		
//	06-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::PostInstanciate(const QDT_VECTOR < QDT_PAIR < COM_INSTANCE, COM_INSTANCE > > &	vInstanciatedMeshes)
{
	UInt32				i, nNbrInstanciatedMeshes = vInstanciatedMeshes.GetSize();
	CONSTRAINT_DATA *	pConstraintData = static_cast<CONSTRAINT_DATA *>(_ConstraintDataCI.GetCommunicator());

	_Actor1CI = pConstraintData->GetActor1CI();
	if	(_Actor1CI.HasHandle())
	{
		for	(i = 0 ; i < nNbrInstanciatedMeshes ; ++i)
		{
			QDT_PAIR<COM_INSTANCE, COM_INSTANCE> Pair = vInstanciatedMeshes[i];

			if	(Pair.GetSecond() == _Actor1CI)
			{
				_Actor1CI = Pair.GetFirst();
				break;
			}
		}
	}

	_Actor2CI = pConstraintData->GetActor2CI();
	if	(_Actor2CI.HasHandle())
	{
		for	(i = 0 ; i < nNbrInstanciatedMeshes ; ++i)
		{
			QDT_PAIR<COM_INSTANCE, COM_INSTANCE> Pair = vInstanciatedMeshes[i];

			if	(Pair.GetSecond() == _Actor2CI)
			{
				_Actor2CI = Pair.GetFirst();
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::Init()
{
	if	((_nFlags.IsSet(CONSTRAINT_INIT) == false) && _ConstraintDataCI.HasHandle() && _ConstraintDataCI.IsLoaded())
	{			
		I_PHYSIC_DYNAMIC_ACTOR *	pActor1;
		I_PHYSIC_DYNAMIC_ACTOR *	pActor2;
		CONSTRAINT_DATA *			pConstraintData = static_cast<CONSTRAINT_DATA *>(_ConstraintDataCI.GetCommunicator());

		Clear();

		if	(_Actor1CI.HasHandle() == false)
		{
			_Actor1CI = pConstraintData->GetActor1CI();
		}

		if	(_Actor1CI.HasHandle() && _Actor1CI.IsLoaded())
		{
			ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(_Actor1CI.GetCommunicator());
			Int32			iChildIndex1 = pConstraintData->GetChildIndex1();
			NODE *			pNode = pEntity->GetNode();
			
			if	((iChildIndex1 >= 0) && (pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY))
			{
				NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(pNode);
				
				pActor1 = pNodeArray->GetChild(iChildIndex1)->GetDynamicActor();
			}
			else
			{
				pActor1 = pNode->GetDynamicActor();
			}
		}
		else
		{
			_pJointCustom->SetActor1(NULL);
		}

		if	(_Actor2CI.HasHandle() == false)
		{
			_Actor2CI = pConstraintData->GetActor2CI();
		}

		if	(_Actor2CI.HasHandle() && _Actor2CI.IsLoaded())
		{
			ENTITY_ABC *				pEntity = static_cast<ENTITY_ABC *>(_Actor2CI.GetCommunicator());
			Int32						iChildIndex2 = pConstraintData->GetChildIndex2();
			NODE *						pNode = pEntity->GetNode();

			if	((iChildIndex2 >= 0) && (pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY))
			{
				NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(pNode);
				
				pActor2 = pNodeArray->GetChild(iChildIndex2)->GetDynamicActor();
			}
			else
			{
				pActor2 = pNode->GetDynamicActor();
			}
		}
		else
		{
			_pJointCustom->SetActor2(NULL);
		}

		if	(pActor1 || pActor2)
		{
			_pJointCustom = PHYSIC_CONTEXT::Instance()->CreateJointCustom();
			_pJointCustom->SetActor1(pActor1);
			_pJointCustom->SetActor2(pActor2);
			_pJointCustom->SetGlobalAnchor(pConstraintData->GetGlobalAnchor());
			_pJointCustom->SetGlobalAxis(pConstraintData->GetGlobalAxis());
			_pJointCustom->EnableCollision(pConstraintData->IsCollisionEnabled());
			_pJointCustom->SetXMotionType(pConstraintData->GetXMotionType());
			_pJointCustom->SetYMotionType(pConstraintData->GetYMotionType());
			_pJointCustom->SetZMotionType(pConstraintData->GetZMotionType());
			_pJointCustom->SetSwing1MotionType(pConstraintData->GetSwing1MotionType());
			_pJointCustom->SetSwing2MotionType(pConstraintData->GetSwing2MotionType());
			_pJointCustom->SetTwistMotionType(pConstraintData->GetTwistMotionType());
			_pJointCustom->EnableProjection(pConstraintData->IsProjectionEnabled());
			_pJointCustom->SetProjectionDistance(pConstraintData->GetProjectionDistance());
			_pJointCustom->SetProjectionAngle(pConstraintData->GetProjectionAngle());
			_pJointCustom->SetLinearLimitValue(pConstraintData->GetLinearLimitValue());
			_pJointCustom->SetLinearLimitRestitution(pConstraintData->GetLinearLimitRestitution());
			_pJointCustom->SetLinearLimitSpring(pConstraintData->GetLinearLimitSpring());
			_pJointCustom->SetLinearLimitDamper(pConstraintData->GetLinearLimitDamper());
			_pJointCustom->SetSwing1LimitValue(pConstraintData->GetSwing1LimitValue());
			_pJointCustom->SetSwing1LimitRestitution(pConstraintData->GetSwing1LimitRestitution());
			_pJointCustom->SetSwing1LimitSpring(pConstraintData->GetSwing1LimitSpring());
			_pJointCustom->SetSwing1LimitDamper(pConstraintData->GetSwing1LimitDamper());
			_pJointCustom->SetSwing2LimitValue(pConstraintData->GetSwing2LimitValue());
			_pJointCustom->SetSwing2LimitRestitution(pConstraintData->GetSwing2LimitRestitution());
			_pJointCustom->SetSwing2LimitSpring(pConstraintData->GetSwing2LimitSpring());
			_pJointCustom->SetSwing2LimitDamper(pConstraintData->GetSwing2LimitDamper());
			_pJointCustom->SetTwistLowLimitValue(pConstraintData->GetTwistLowLimitValue());
			_pJointCustom->SetTwistLowLimitRestitution(pConstraintData->GetTwistLowLimitRestitution());
			_pJointCustom->SetTwistLowLimitSpring(pConstraintData->GetTwistLowLimitSpring());
			_pJointCustom->SetTwistLowLimitDamper(pConstraintData->GetTwistLowLimitDamper());
			_pJointCustom->SetTwistHighLimitValue(pConstraintData->GetTwistHighLimitValue());
			_pJointCustom->SetTwistHighLimitRestitution(pConstraintData->GetTwistHighLimitRestitution());
			_pJointCustom->SetTwistHighLimitSpring(pConstraintData->GetTwistHighLimitSpring());
			_pJointCustom->SetTwistHighLimitDamper(pConstraintData->GetTwistHighLimitDamper());
			//_pJointCustom->SetSwingDriveType(pConstraintData->GetSwingDriveType());
			//_pJointCustom->SetSwingDriveSpring(pConstraintData->GetSwingDriveSpring());
			//_pJointCustom->SetSwingDriveDamping(pConstraintData->GetSwingDriveDamper());
			//_pJointCustom->SetDriveOrientation(pConstraintData->GetSwingDriveOrientation());
		}

		_nFlags.Set(CONSTRAINT_INIT, true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::Clear()
{
	if	(_pJointCustom)
	{
		PHYSIC_CONTEXT::Instance()->ReleaseJoint(_pJointCustom);
		_pJointCustom = NULL;
	}

	_nFlags.Set(CONSTRAINT_INIT, false);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("CONSDATA", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("CONSDATA", 2, CM_ReadChunk2);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("CONSDATA", 2, CM_WriteChunk2);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CONSTRAINT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CONSTRAINT::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("CONSTRAINT"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT,
											QDT_COMMENT("356CAEC5-A7D6-FD50-EADE-483EEDFA3334"),
											sizeof(CONSTRAINT),
											ComInit,
											QDT_COMMENT("Class for constraint"),
											ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	CONSTRAINT	*pConstraint;
	pConstraint = static_cast<CONSTRAINT *>(pO);

	QDT_NEW_PLACED(pConstraint, CONSTRAINT)();
	QDT_ASSERT(pConstraint);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	CONSTRAINT	*pConstraint;
	pConstraint = static_cast<CONSTRAINT *>(pO);

	QDT_DELETE_PLACED(pConstraint, CONSTRAINT);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::CM_ReadChunk1(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	CONSTRAINT	*pConstraint;
	pConstraint = static_cast<CONSTRAINT *>(pCommunicator);

	Stream >> pConstraint->_NodeInst;

	Stream >> pConstraint->_ConstraintDataCI;
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk2
//	Object:		
//	06-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::CM_ReadChunk2(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	CONSTRAINT	*pConstraint;
	pConstraint = static_cast<CONSTRAINT *>(pCommunicator);

	Stream >> pConstraint->_ConstraintDataCI;
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk2
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT::CM_WriteChunk2(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	CONSTRAINT	*pConstraint;
	pConstraint = static_cast<CONSTRAINT *>(pCommunicator);

	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("\r\n");
	Stream.DebugOutput("CONSTRAINT :");
	Stream.DebugOutput(pConstraint->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("    Constraint data : ");
	Stream << pConstraint->_ConstraintDataCI;
	Stream.DebugOutput("\r\n");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
