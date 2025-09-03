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
//	CLASS:	PHYSIC_MANAGER
//
//	02-02-27:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Physic/PhysicManager)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(SceneGraph/Node/NodeArray)

#include	INCL_KCORE(Time/Time_ABC)

#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(BaseType/TypeBool)

#include 	INCL_KPHYSICS(Physics/PhysicContext)
#include 	INCL_KPHYSICS(Physics/IPhysicControllerManager)
#include 	INCL_KPHYSICS(Physics/IPhysicController)
	
//=============================================================================
// CODE STARTS HERE
//=============================================================================

// For singleton
PHYSIC_MANAGER*	PHYSIC_MANAGER::_pInstance; 

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_MANAGER constructor
//	Object:		
//	02-02-27:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_MANAGER::PHYSIC_MANAGER()
:
_bPause	(false)
{
	_pInstance = this;
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_MANAGER destructor
//	Object:		
//	02-02-27:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_MANAGER::~PHYSIC_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
// Name:		PHYSIC_MANAGER::Instance
// Object:		
// 01-01-24		ELE Created: 
//-----------------------------------------------------------------------------
PHYSIC_MANAGER* PHYSIC_MANAGER::Instance()
{
	return (static_cast<PHYSIC_MANAGER *>(GetSingletonDescriptor()->Instance()));
}

//-----------------------------------------------------------------------------
// Name:		BEHAVIOR_MANAGER::Suicide
// Object:		For singleton : suicide
// 01-01-24		ELE Created: 
//-----------------------------------------------------------------------------
void PHYSIC_MANAGER::Suicide()
{
	GetSingletonDescriptor()->Release();
}

//-----------------------------------------------------------------------------
//	Name:		SetPause
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::SetPause(bool	b)
{
	_bPause = b;
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	02-02-27:	ELE - Created
//-----------------------------------------------------------------------------
void PHYSIC_MANAGER::Loop()
{
	if	(!_bPause)
	{
		// Update physics controllers with animations results
		MoveNodesWithPhysics();

		// Physic update
		PHYSIC_CONTEXT::Instance()->UpdateTime(TIME::Instance()->GetDefaultClock()->GetDeltaFrameTimeInS());

		// Post updates controller
		PHYSIC_CONTEXT::Instance()->GetControllerManager()->UpdateControllers();

		// Report physics results in nodes
		UpdateNodesWithPhysics();
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddNodeWithPhysics
//	Object:		
//	06-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::AddNodeWithPhysics(NODE *	pNode)
{
	QDT_DLIST<NODE *>::ITERATOR it;
	it = _vNodesWithPhysics.Find(_vNodesWithPhysics.GetHead(), pNode);
	if (it == _vNodesWithPhysics.GetTail())
	{
		_vNodesWithPhysics.PushTail(pNode);
	}
}

//-----------------------------------------------------------------------------
//	Name:		RemoveNodeWithPhysics
//	Object:		
//	06-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::RemoveNodeWithPhysics(NODE *	pNode)
{
	QDT_DLIST<NODE *>::ITERATOR it;
	it = _vNodesWithPhysics.Find(_vNodesWithPhysics.GetHead(), pNode);
	if (it != _vNodesWithPhysics.GetTail())
	{
		_vNodesWithPhysics.Remove(it);
		return;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UnregisterNodeWithPhysics
//	Object:		
//	06-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::UnregisterNodeWithPhysics(I_PHYSIC_SCENE *	pPhysicScene)
{
	QDT_DLIST <NODE *>::ITERATOR itNext, it	= _vNodesWithPhysics.GetHead();
	QDT_DLIST <NODE *>::ITERATOR itTail		= _vNodesWithPhysics.GetTail();

	while (it != itTail)
	{
		itNext = it + 1;

		// This call can remove it from the list
		(*it)->UnregisterDynamicActor(pPhysicScene);

		it = itNext;
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateNodesWithPhysics
//	Object:		
//	06-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::UpdateNodesWithPhysics()
{
	QDT_DLIST <NODE *>::ITERATOR it	  = _vNodesWithPhysics.GetHead();
	QDT_DLIST <NODE *>::ITERATOR itTail = _vNodesWithPhysics.GetTail();
	while (it != itTail)
	{
		NODE * pNode = *it;

		if	(pNode->GetFlags().IsSet(NODE::PHYSIC_CONTROLLER))
		{
			I_PHYSIC_CONTROLLER *	pPhysicController = pNode->GetPhysicController();

			VECTOR ControllerPosition;
			pPhysicController->GetPosition(ControllerPosition);

			VECTOR	NodePos;
			pNode->GetGlobalPosition(NodePos);

			pNode->UpdateGlobalMatrix();
			pNode->SetGlobalPosition(ControllerPosition);
			pNode->UpdateOldPosition();

			if	(pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
			{
				static_cast<NODE_ARRAY *>(pNode)->RetrieveActorsPos();
			}
		}
		else
		{
			if	(pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
			{
				static_cast<NODE_ARRAY *>(pNode)->RetrieveActorsPos();
			}
			else
			{
				I_PHYSIC_DYNAMIC_ACTOR * pActor = pNode->GetDynamicActor();
				if (!pActor->IsSleeping() && !pNode->IsSklPhysicJoint())
				{
					if	(pActor->IsKinematicModeEnabled() == false)
					{
						MATRIX GlobalMatrix;
						pActor->GetGlobalMatrix(GlobalMatrix);
						pNode->SetGlobalMatrix(GlobalMatrix);
					}
					else
					{
						pActor->SetGlobalMatrix(pNode->GetGlobalMatrix());
					}
				}
			}
		}

		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		MoveNodesWithPhysics
//	Object:		
//	06-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::MoveNodesWithPhysics()
{
	QDT_DLIST <NODE *>::ITERATOR it	  = _vNodesWithPhysics.GetHead();
	QDT_DLIST <NODE *>::ITERATOR itTail = _vNodesWithPhysics.GetTail();
	
	while (it != itTail)
	{
		NODE * pNode = *it;

		if	(pNode->GetFlags().IsSet(NODE::PHYSIC_CONTROLLER))
		{
			VECTOR					Disp, NodePosition, NodeOldPosition;
			I_PHYSIC_CONTROLLER *	pPhysicController;

			pNode->GetGlobalPosition(NodePosition);
			pNode->GetOldPosition(NodeOldPosition);

			Disp.Sub(NodePosition, NodeOldPosition);

			pPhysicController = pNode->GetPhysicController();
			pPhysicController->Move(Disp);

			if	(pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
			{
				static_cast<NODE_ARRAY *>(pNode)->MoveActors();
			}
		}
		else
		{
			if	(pNode->GetType() == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_ARRAY)
			{
				static_cast<NODE_ARRAY *>(pNode)->MoveActors();
			}
			/*else // TO DO
			{
				pNode->Move();
			}*/
		}

		++it;
	}
}

//-----------------------------------------------------------------------------
//	Name:		COM_Init
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::COM_Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = METHOD_DESCRIPTOR::Allocate("SetPause", QDT_COMMENT("Pause all physics."), ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pMD->AddParameter(QDT_COMMENT("bValue"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddCommand(CMD_PM_PAUSE, pMD, CM_Pause);
}

//-----------------------------------------------------------------------------
//	Name:		GetSingletonDescriptor
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	PHYSIC_MANAGER::GetSingletonDescriptor()
{
	return (SINGLETON_DESCRIPTOR::FindSingDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_MANAGER));
}

//-----------------------------------------------------------------------------
//	Name:		CreateSingletonDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	PHYSIC_MANAGER::CreateSingletonDescriptor()
{
	return (new SINGLETON_DESCRIPTOR(	QDT_COMMENT("PHYSIC_MANAGER"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_MANAGER,
										QDT_COMMENT("A8FE2B6E-2BBB-0400-9FAC-4F35E8F6929E"),
										sizeof(PHYSIC_MANAGER),
										COM_Init,
										QDT_COMMENT("The physic manager singleton")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::EM_Construct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	PHYSIC_MANAGER *pPM = static_cast<PHYSIC_MANAGER *>(pO);
	QDT_NEW_PLACED(pPM, PHYSIC_MANAGER)();
	QDT_ASSERT(pPM);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-25:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_MANAGER::EM_Destruct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	PHYSIC_MANAGER *pPM = static_cast<PHYSIC_MANAGER *>(pO);

	QDT_DELETE_PLACED(pPM, PHYSIC_MANAGER);
}

//-----------------------------------------------------------------------------
//	Name:		CM_Pause
//	Object:		
//	03-11-12:	ELE - Created
//-----------------------------------------------------------------------------
int	PHYSIC_MANAGER::CM_Pause(COM_PARAM	P)
{
	COM_STACK Stack(P, GetSingletonDescriptor(), CMD_PM_PAUSE);

	bool bValue;
	if (Stack.GetParameter(0, bValue))
	{
		PHYSIC_MANAGER::Instance()->SetPause(bValue);
	}

	return (COM_STACK::NO_RET_VALUE);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
