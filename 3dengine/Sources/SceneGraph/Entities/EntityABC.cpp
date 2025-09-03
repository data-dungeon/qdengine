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
// CLASS: ENTITY_ABC
//
//
//	01-02-08:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)

#ifdef _DEBUG
	#include "EntityABC.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENTITY_ABC constructor
//	Object:		
//	02-12-03:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_ABC::ENTITY_ABC(COMMUNICATOR_DESCRIPTOR *	pCD,
					   ENTITY_TYPE					eEntityType)
:
COMMUNICATOR	(pCD),
_bVisible		(false),
_NodeInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_pSceneGraph	(NULL),
_eEntityType	(eEntityType)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_ABC constructor
//	Object:		
//	02-12-03:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_ABC::ENTITY_ABC(	COMMUNICATOR_DESCRIPTOR *	pCD,
						ENTITY_TYPE					eEntityType,
						COM_HANDLE *				pCH)
:
COMMUNICATOR	(pCD, pCH),
_bVisible		(false),
_NodeInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_pSceneGraph	(NULL),
_eEntityType	(eEntityType)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~ENTITY_ABC destructor
//	Object:		
//	02-12-03:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_ABC::~ENTITY_ABC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	06-01-12:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Copy(const ENTITY_ABC &	C)
{
	_NodeInst = C._NodeInst;
	_bVisible = C._bVisible;
	_pSceneGraph = C._pSceneGraph;
}

//-----------------------------------------------------------------------------
//	Name:		AttachChild
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::AttachChild(const COM_INSTANCE &	ChildInst)
{
	if	(ChildInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(ChildInst.GetCommunicator());

		pEntity->SetSceneGraph(_pSceneGraph);
		pEntity->Register();

		GetNode()->AddChild(pEntity->GetNode());
	}
}

//-----------------------------------------------------------------------------
//	Name:		AttachChild
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::AttachChild(const COM_INSTANCE &	ChildInst,
								int						nIndex)
{
	if	(ChildInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(ChildInst.GetCommunicator());

		pEntity->SetSceneGraph(_pSceneGraph);
		pEntity->Register();

		GetNode()->AddChild(pEntity->GetNode(), nIndex);
	}
}

//-----------------------------------------------------------------------------
//	Name:		AttachChildKeepGlobalMatrix
//	Object:		
//	06-01-16:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::AttachChildKeepGlobalMatrix(const COM_INSTANCE &	ChildInst,
												int						nIndex)
{
	if	(ChildInst.HasHandle())
	{
		ENTITY_ABC *	pEntity = static_cast<ENTITY_ABC *>(ChildInst.GetCommunicator());

		pEntity->SetSceneGraph(_pSceneGraph);
		pEntity->Register();

		GetNode()->AddChildKeepGlobalMatrix(pEntity->GetNode(), nIndex);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Detach
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Detach()
{
	Unregister();
	_pSceneGraph = NULL;

	NODE* pNode = GetNode();
	if (pNode != NULL)
		pNode->RemoveFromFather();
}

//-----------------------------------------------------------------------------
//	Name:		DetachKeepHierarchy
//	Object:		
//	06-03-20:	DAC - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::DetachKeepHierarchy()
{
	Unregister();
	_pSceneGraph = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		DetachKeepGlobalMat
//	Object:		
//	06-01-11:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::DetachKeepGlobalMat()
{
	Unregister();
	_pSceneGraph = NULL;

	GetNode()->RemoveFromFatherKeepGlobalMat();
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateNode
//	Object:		
//	06-01-09:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::InstanciateNode(ENTITY_ABC * pInstance)
{
	COM_INSTANCE	NewNodeInst;

	if	(_NodeInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
	{
		NODE_ARRAY *	pNewNodeArray;
		NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());

		NewNodeInst.SetHandle(NODE_ARRAY::GetGenericCommunicatorDescriptor()->Create());
		pNewNodeArray = static_cast<NODE_ARRAY *>(NewNodeInst.GetCommunicator());

		pNodeArray->Instanciate(pNewNodeArray);
	}
	else
	{
		NODE *	pNewNode;
		NODE *	pNode = GetNode();

		NewNodeInst.SetHandle(NODE::GetGenericCommunicatorDescriptor()->Create());
		pNewNode = static_cast<NODE *>(NewNodeInst.GetCommunicator());

		pNode->Instanciate(pNewNode);
	}

	pInstance->SetNodeInst(NewNodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateNodeRec
//	Object:		
//	06-01-18:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::InstanciateNodeRec(ENTITY_ABC *	pInstance)
{
	COM_INSTANCE	NewNodeInst;

	if	(_NodeInst.GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
	{
		NODE_ARRAY *	pNewNodeArray;
		NODE_ARRAY *	pNodeArray = static_cast<NODE_ARRAY *>(GetNode());

		NewNodeInst.SetHandle(NODE_ARRAY::GetGenericCommunicatorDescriptor()->Create());
		pNewNodeArray = static_cast<NODE_ARRAY *>(NewNodeInst.GetCommunicator());

		pNodeArray->InstanciateRec(pNewNodeArray);
	}
	else
	{
		NODE *	pNewNode;
		NODE *	pNode = GetNode();

		NewNodeInst.SetHandle(NODE::GetGenericCommunicatorDescriptor()->Create());
		pNewNode = static_cast<NODE *>(NewNodeInst.GetCommunicator());

		pNode->InstanciateRec(pNewNode);
	}

	pInstance->SetNodeInst(NewNodeInst);
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_ABC::Display
//	Object:		Display an entity
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Display()
{
	// Default display for all the entities

	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-01-25:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Display(const QDT_VECTOR<UInt32> &	vPrimitives)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	06-01-26:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Display(UInt32	nPrimitive)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_ABC::RayCast
//	Object:		Test the intersection of the ray & the entity
//				RayCastParameters contain all the parameters needed for raycast
//				as the ray, the nearest impact...
//				If the 
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
bool ENTITY_ABC::RayCast(RAY_CAST_PARAMETERS & RayCastParameters)
{
	// Default raycast for all the entities

	// Do nothing

	return (false); 
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_ABC::UpdateParticleEmitter
//	Object:		Update the particle system
//	01-03-08:	RMA - Created
//-----------------------------------------------------------------------------
void ENTITY_ABC::UpdateParticleEmitter(unsigned int nDeltaTime, const MATRIX & Matrix, PARTICLE_SYSTEM & ParticleSystem)
{
	// Default update particle system for all entities

	// Do nothing

	return;
}

//-----------------------------------------------------------------------------
//	Name:		FindShadowMap
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::FindShadowMap(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return;
}

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		Write entity info in a stream
//	01-10-05:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Dump(IO_STREAM &	Out)
{
#ifndef _MASTER
	char	sBuffer[256];

	sprintf(sBuffer, "        ENTITY_ABC::%s\n", GetName().GetBuffer());
	Out << sBuffer;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetBoxGlobalPos
//	Object:		
//	02-01-14:	cvi - Created
//-----------------------------------------------------------------------------
VECTOR	ENTITY_ABC::GetCenter() const
{
	return (VECTOR::ZERO);
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	02-03-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_ABC::IsInside(NODE *	pTriggerNode)
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Update()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ToUpdate
//	Object:		
//	03-06-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	ENTITY_ABC::ToUpdate() const
{
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		HasJustBeenCreated
//	Object:		Called just after creation
//	02-04-05:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::HasJustBeenCreated()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	03-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	ENTITY_ABC::TestVisibility(const FRUSTRUM & Frustum)
{
	_bVisible = false;
	return	(_bVisible);
}

//-----------------------------------------------------------------------------
//	Name:		ClearShadowMapFlags
//	Object:		
//	03-03-21:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::ClearShadowMapFlags()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		InitParameters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::InitParameters(float rFogDec)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ResetParameters
//	Object:		
//	04-01-14:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::ResetParameters()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk( "ENTITY  ", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("ENTITY  ", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ENTITY_ABC::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENTITY_ABC));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-10-06:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	ENTITY_ABC::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("ENTITY_ABC"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENTITY_ABC,
										QDT_COMMENT("43053959-6DFA-F943-8BE3-26908EE51CD1"), 
										sizeof(ENTITY_ABC), 
										ComInit, 
										QDT_COMMENT("Entity is the base class for all objects such as cameras, lights, meshes..."),
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::CM_ReadChunk1(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	ENTITY_ABC *	pEntity;
	pEntity = static_cast<ENTITY_ABC *>(pCommunicator);

	Stream >> pEntity->_NodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	06-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::CM_WriteChunk1(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	ENTITY_ABC *	pEntity;
	pEntity = static_cast<ENTITY_ABC *>(pCommunicator);

	Stream.DebugOutput("    Node : ");
	Stream << pEntity->_NodeInst;
	Stream.DebugOutput("\r\n");
}

//-----------------------------------------------------------------------------
//	Name:		InterpolateLighting
//	Object:		
//	03-11-04:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::InterpolateLighting(float	rTime)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetCollisionVolume
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
const BOUNDING_VOLUME *	ENTITY_ABC::GetCollisionVolume() const
{
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetDistanceFromCamera
//	Object:		
//	04-07-23:	RMA - Created
//-----------------------------------------------------------------------------
float	ENTITY_ABC::GetDistanceFromCamera(unsigned int	nGeometry,
										  NODE *		pNode)
{
	return (0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		Register
//	Object:		
//	06-01-24:	VMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Register()
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Unregister
//	Object:		
//	06-01-24:	VMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Unregister()
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-01-24:	VMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::Instanciate(COM_INSTANCE &	Instance)
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-01-24:	VMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_ABC::InstanciateRec(COM_INSTANCE &	Instance)
{
	//Do nothing
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
