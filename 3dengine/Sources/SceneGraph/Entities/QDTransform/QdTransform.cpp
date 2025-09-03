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
//	CLASS:	QD_TRANSFORM
//
//	01-11-28:	cvi - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/QdTransform/QdTransform)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QD_TRANSFORM constructor
//	Object:		
//	01-11-28:	cvi - Created
//-----------------------------------------------------------------------------
QD_TRANSFORM::QD_TRANSFORM()
:
ENTITY_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_QD_TRANSFORM)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		QD_TRANSFORM destructor
//	Object:		
//	01-11-28:	cvi - Created
//-----------------------------------------------------------------------------
QD_TRANSFORM::~QD_TRANSFORM()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instanciate
//	Object:		
//	06-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	QD_TRANSFORM::Instanciate(COM_INSTANCE &	Instance)
{
	QD_TRANSFORM *	pNewTransform;

	Instance.SetHandle(QD_TRANSFORM::GetGenericCommunicatorDescriptor()->Create());
	pNewTransform = static_cast<QD_TRANSFORM *>(Instance.GetCommunicator());

	InstanciateNode(pNewTransform);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateRec
//	Object:		
//	06-03-29:	ELE - Created
//-----------------------------------------------------------------------------
void	QD_TRANSFORM::InstanciateRec(COM_INSTANCE &	Instance)
{
	QD_TRANSFORM *	pNewTransform;

	Instance.SetHandle(QD_TRANSFORM::GetGenericCommunicatorDescriptor()->Create());
	pNewTransform = static_cast<QD_TRANSFORM *>(Instance.GetCommunicator());

	InstanciateNodeRec(pNewTransform);
}

//-----------------------------------------------------------------------------
//	Name:		TestVisibility
//	Object:		
//	04-06-21:	ELE - Created
//-----------------------------------------------------------------------------
bool	QD_TRANSFORM::TestVisibility(DISPLAY_PARAMETERS &	DisplayParameters)
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QD_TRANSFORM::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("TRANSFOR", 1 ,CM_ReadChunk1);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("TRANSFOR", 1 ,CM_WriteChunk1);
#endif	
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	QD_TRANSFORM::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_QD_TRANSFORM) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	QD_TRANSFORM::CreateGenericCommunicatorDescriptor()
{
	return( new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("QD_TRANSFORM"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_QD_TRANSFORM,
										QDT_COMMENT("7EB0FF5A-B0F6-714F-60C1-F0F554B55C61"), 
										sizeof(QD_TRANSFORM), 
										ComInit, 
										QDT_COMMENT(""),
										ENTITY_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QD_TRANSFORM::EM_Construct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	QD_TRANSFORM *pTrans;
	pTrans = static_cast<QD_TRANSFORM *>(pO);

	QDT_NEW_PLACED(pTrans, QD_TRANSFORM)();
	QDT_ASSERT(pTrans);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QD_TRANSFORM::EM_Destruct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	QD_TRANSFORM *pTrans;
	pTrans = static_cast<QD_TRANSFORM *>(pO);

	QDT_DELETE_PLACED(pTrans, QD_TRANSFORM);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	QD_TRANSFORM::CM_ReadChunk1(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	// Do nothing
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	QD_TRANSFORM::CM_WriteChunk1(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	QD_TRANSFORM *pTrans;
	pTrans = static_cast<QD_TRANSFORM *>(pCommunicator);

	Stream.DebugOutput("Locator :\t");
	Stream.DebugOutput(pTrans->GetName());
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("\r\n");
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
