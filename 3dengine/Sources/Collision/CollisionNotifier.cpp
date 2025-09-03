
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
//	CLASS:	SPEECH ELEMENT
//
//	04-01-05:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_3DENGINE(Collision/CollisionNotifier)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_NOTIFIER::COLLISION_NOTIFIER()
:
COMMUNICATOR(GetGenericCommunicatorDescriptor())
{
}


//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
COLLISION_NOTIFIER::COLLISION_NOTIFIER(CLASS_DESCRIPTOR * const pCD, COM_HANDLE * pHandle)
:
COMMUNICATOR( pCD, pHandle )
{
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *COLLISION_NOTIFIER::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_NOTIFIER) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	COLLISION_NOTIFIER::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("COLLISION_NOTIFIER"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_COLLISION_NOTIFIER, 
										QDT_COMMENT("0E49944E-38E8-4472-BBDC-E1BE92369FAA"),
										sizeof(COLLISION_NOTIFIER), 
										EM_Init, 
										QDT_COMMENT("Collision Notification mechanism")));
}

/*
//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_NOTIFIER::GenerateStaticCollisionEvent( NODE *pNode, int nbOccurences )
{
	struct PARAMS
	{
		NODE	*_pNode;
		int		_nOccurences;
	};

	PARAMS	Params = { pNode, nbOccurences };

	GetComInstance().CallMethod( CMD_STATIC_COLLISION_EVENT, &Params, NULL );
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_NOTIFIER::GenerateGroundEvent( NODE *pNode )
{
	GetComInstance().CallMethod( CMD_GROUND_EVENT, &pNode, NULL );
}

*/

//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
void COLLISION_NOTIFIER::EM_Init(CLASS_DESCRIPTOR * const pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_NOTIFIER::EM_Construct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	QDT_FAIL();
	//COLLISION_NOTIFIER *	pColElm;
	//pColElm = static_cast<COLLISION_NOTIFIER*>(pO);
	//QDT_NEW_PLACED(pColElm, COLLISION_NOTIFIER)();
}

//-----------------------------------------------------------------------------
//	Name:
//	Object:
//	04-01-05:	JPE - Created
//-----------------------------------------------------------------------------
void	COLLISION_NOTIFIER::EM_Destruct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	QDT_FAIL();
	//COLLISION_NOTIFIER *	pColElm;
	//pColElm = (COLLISION_NOTIFIER *) pO;
	//QDT_DELETE_PLACED(pColElm, COLLISION_NOTIFIER);
}


