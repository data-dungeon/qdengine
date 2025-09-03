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
//	CLASS:	PHYSIC_JOINT
//
//	05-02-02:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJoint)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJointRevolute)
#include	INCL_3DENGINE(Display/SceneGraph/SceneGraph)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicJoint)
#include	INCL_KPHYSICS(Physics/IPhysicScene)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT Constructor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT::PHYSIC_JOINT()
:
COMMUNICATOR(CreateGenericCommunicatorDescriptor()),
_pJoint(NULL),
_pSceneGraph(NULL)
{
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT Constructor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT::PHYSIC_JOINT(COMMUNICATOR_DESCRIPTOR * pCD)
:
COMMUNICATOR(pCD),
_pJoint(NULL),
_pSceneGraph(NULL)
{
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT Destructor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT::~PHYSIC_JOINT()
{
	UnregisterJoint();
	PHYSIC_CONTEXT::Instance()->ReleaseJoint(_pJoint);
	_pJoint = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SetNode1
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::SetNode1(const COM_INSTANCE & ciNode1)
{
	QDT_ASSERT(_pJoint);
	QDT_ASSERT(ciNode1.HasHandle());
	NODE * pNode = static_cast<NODE *>(ciNode1.GetCommunicator());
	I_PHYSIC_ACTOR * pPhysicActor = pNode->GetDynamicActor();
	if (!pPhysicActor)
	{
		QDT_Error("The node1 of the joint %s is not a dynamic actor! This can't work!");
		return;
	}

	_pJoint->SetActor1(pPhysicActor);
}

//-----------------------------------------------------------------------------
//	Name:		SetNode2
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::SetNode2(const COM_INSTANCE & ciNode2)
{
	QDT_ASSERT(_pJoint);
	QDT_ASSERT(ciNode2.HasHandle());
	NODE * pNode = static_cast<NODE *>(ciNode2.GetCommunicator());
	I_PHYSIC_ACTOR * pPhysicActor = pNode->GetDynamicActor();
	if (!pPhysicActor)
	{
		QDT_Error("The node1 of the joint %s is not a dynamic actor! This can't work!");
		return;
	}

	_pJoint->SetActor2(pPhysicActor);
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAnchor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::SetGlobalAnchor(const VECTOR & Pos)
{
	QDT_ASSERT(_pJoint);
	_pJoint->SetGlobalAnchor(Pos);
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAxis
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::SetGlobalAxis(const VECTOR & Axis)
{
	QDT_ASSERT(_pJoint);
	_pJoint->SetGlobalAxis(Axis);
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAxis
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::EnableCollision(bool bState)
{
	QDT_ASSERT(_pJoint);
	_pJoint->EnableCollision(bState);
}

//-----------------------------------------------------------------------------
//	Name:		RegisterJoint
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::RegisterJoint(SCENE_GRAPH * pSceneGraph)
{
	QDT_ASSERT(pSceneGraph);
	_pSceneGraph = pSceneGraph;
	I_PHYSIC_SCENE * pPhysicScene = pSceneGraph->GetPhysicScene();
	QDT_ASSERT(pPhysicScene);
	pPhysicScene->RegisterJoint(_pJoint);
}

//-----------------------------------------------------------------------------
//	Name:		UnregisterJoint
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void PHYSIC_JOINT::UnregisterJoint()
{
	if (_pSceneGraph)
	{
		I_PHYSIC_SCENE * pPhysicScene = _pSceneGraph->GetPhysicScene();
		QDT_ASSERT(pPhysicScene);
		pPhysicScene->UnregisterJoint(_pJoint);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::ComInit(CLASS_DESCRIPTOR * const pCD)
{
	METHOD_DESCRIPTOR *pMD;
	pCD->SetConstructors(EM_Construct, EM_Destruct);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetNode1"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("Node1"), QDT_COMMENT("Node1"), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_NODE1, pMD, EM_SetNode1);
	
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetNode2"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("Node1"), QDT_COMMENT("Node2"), NODE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_NODE2, pMD, EM_SetNode2);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetGlobalAnchor"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_GLOBAL_ANCHOR, pMD, EM_SetGlobalAnchor);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetGlobalAxis"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rY"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pMD->AddParameter(QDT_COMMENT("rX"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_GLOBAL_AXIS, pMD, EM_SetGlobalAxis);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableCollision"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bState"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_COLLISION, pMD, EM_EnableCollision);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR* PHYSIC_JOINT::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_JOINT) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PHYSIC_JOINT::CreateGenericCommunicatorDescriptor()
{
	return (new	COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PHYSIC_JOINT"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_JOINT,
										QDT_COMMENT("30E614F8-EB5D-505B-38DC-C8E8CC29E626"),
										sizeof(PHYSIC_JOINT_REVOLUTE),
										ComInit,
										QDT_COMMENT("Class for physic joint. A joint is a constraint applied between two NODE."),
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_Construct(void * const pO,
								   void * const pR, 
								   void * const pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_Destruct(void * const pO, 
								  void * const pR, 
								  void * const pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNode1
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_SetNode1(void * const pO, 
								  void * const pR, 
								  void * const pP)
{
	COM_INSTANCE 	*pParameter		= static_cast<COM_INSTANCE *>(pP);
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT	*pJoint			= static_cast<PHYSIC_JOINT *>(pComInstance->GetCommunicator());
	
	pJoint->SetNode1(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetNode2
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_SetNode2(void * const pO, 
								  void * const pR, 
								  void * const pP)
{
	COM_INSTANCE 	*pParameter		= static_cast<COM_INSTANCE *>(pP);
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT	*pJoint			= static_cast<PHYSIC_JOINT *>(pComInstance->GetCommunicator());
	
	pJoint->SetNode2(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetGlobalAnchor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_SetGlobalAnchor(void * const pO, 
										 void * const pR, 
										 void * const pP)
{
	struct PARAMETERS
	{
		float _rX;
		float _rY;
		float _rZ;
	};

	PARAMETERS 		*pParameter		= static_cast<PARAMETERS *>(pP);
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT	*pJoint			= static_cast<PHYSIC_JOINT *>(pComInstance->GetCommunicator());

	pJoint->SetGlobalAnchor(VECTOR(pParameter->_rX, pParameter->_rY, pParameter->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetGlobalAxis
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_SetGlobalAxis(void * const pO, 
									   void * const pR,
									   void * const pP)
{
	struct PARAMETERS
	{
		float _rX;
		float _rY;
		float _rZ;
	};

	PARAMETERS 		*pParameter		= static_cast<PARAMETERS *>(pP);
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT	*pJoint			= static_cast<PHYSIC_JOINT *>(pComInstance->GetCommunicator());

	pJoint->SetGlobalAxis(VECTOR(pParameter->_rX, pParameter->_rY, pParameter->_rZ));
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableCollision
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT::EM_EnableCollision(void * const pO, 
										 void * const pR, 
										 void * const pP)
{
	bool	 		*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE	*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT	*pJoint			= static_cast<PHYSIC_JOINT *>(pComInstance->GetCommunicator());

	pJoint->EnableCollision(*pParameter);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
