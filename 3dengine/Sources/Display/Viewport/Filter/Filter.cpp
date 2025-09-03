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
//	CLASS:	FILTER
//
//	04-01-02:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Display/Viewport/Filter/Filter)
#include	INCL_3DENGINE(Display/Viewport/Filter/FilterLayer)
#include	INCL_3DENGINE(Display/Viewport/Filter/FilterScratch)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(CommunicatorType/CommunicatorContainerDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)

#ifdef _DEBUG
	#include	"Filter.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*									FILTER::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		FILTER constructor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER::FILTER()
:
COMMUNICATOR_CONTAINER(GetGenericCommunicatorDescriptor())
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		FILTER destructor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
FILTER::~FILTER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::Display(VIEWPORT *pViewport)
{
	unsigned int i;

	for (i = 0 ; i < GetNbrLayers() ; ++i)
	{
		GetLayer(i)->Display(pViewport);
	}

	for (i = 0 ; i < GetNbrScratches() ; ++i)
	{
		GetScratch(i)->Display(pViewport);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *		pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

	pMD = METHOD_DESCRIPTOR__Allocate("PushLayer", "");
	METHOD_DESCRIPTOR__AddParameter(pMD, "Layer", "", FILTER_LAYER::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_PUSH_LAYER, pMD, EM_PushLayer);

#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("FILTER  ", 0, CM_ReadChunk);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("FILTER  ", 0, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	FILTER::GetGenericCommunicatorDescriptor()
{
	return ( (COMMUNICATOR_CONTAINER_DESCRIPTOR *)COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FILTER) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_CONTAINER_DESCRIPTOR *	FILTER::CreateGenericCommunicatorDescriptor()
{
	return ( QDT_NEW COMMUNICATOR_CONTAINER_DESCRIPTOR(	QDT_COMMENT("FILTER"),
													MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_FILTER,
													QDT_COMMENT("3AB9223D-434E-C947-2BD7-16B44E792E15"),
													sizeof(FILTER),
													Init,
													QDT_COMMENT("The filter class."),
													COMMUNICATOR_CONTAINER::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::EM_Construct(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	FILTER *	pObject;
	pObject = static_cast<FILTER *>(pO);
	QDT_NEW_PLACED(pObject, FILTER)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-01-30:	VMA - Created
//-----------------------------------------------------------------------------
void	FILTER::EM_Construct_In_Memory(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::EM_Destruct(void * const	pO,
							void * const	pR,
							void * const	pP)
{
	FILTER *	pObject;
	pObject = static_cast<FILTER*> (pO);
	pObject->FILTER::~FILTER();
}

//-----------------------------------------------------------------------------
//	Name:		EM_PushLayer
//	Object:		
//	04-01-02:	RMA - Created
//-----------------------------------------------------------------------------
void	FILTER::EM_PushLayer(void * const	pO,
							 void * const	pR,
							 void * const	pP)
{
	COM_INSTANCE	*pciThis	= static_cast<COM_INSTANCE*>(pO);
	FILTER			*pThis		= static_cast<FILTER*>(pciThis->GetCommunicator());
	COM_INSTANCE	*pciLayer	= static_cast<COM_INSTANCE*>(pP);

	pThis->PushLayer(*pciLayer);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	FILTER::CM_ReadChunk(IO_STREAM &	Stream,
							 COMMUNICATOR *	pCommunicator)
{
	FILTER *pFilter = static_cast<FILTER *>(pCommunicator);

	unsigned int i;
	unsigned int nNbrLayers, nNbrScratches;
	COM_INSTANCE Inst;

	Stream >> nNbrLayers;
	for (i = 0 ; i < nNbrLayers ; ++i)
	{
		Stream >> Inst;
		pFilter->PushLayer(Inst);
	}

	Stream >> nNbrScratches;
	for (i = 0 ; i < nNbrScratches ; ++i)
	{
		Stream >> Inst;
		pFilter->PushScratch(Inst);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	04-01-12:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER	
void	FILTER::CM_WriteChunk(IO_STREAM &		Stream,
							  COMMUNICATOR *	pCommunicator)
{
	FILTER *pFilter = static_cast<FILTER *>(pCommunicator);

	unsigned int i;

	Stream.DebugOutput("Number of layers = ");
	Stream << pFilter->GetNbrLayers();
	Stream.DebugOutput("\r\n");

	for (i = 0 ; i < pFilter->GetNbrLayers() ; ++i)
	{
		Stream.DebugOutput("Layer : ");
		Stream << pFilter->GetLayerInst(i);
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("Number of scratches = ");
	Stream << pFilter->GetNbrScratches();
	Stream.DebugOutput("\r\n");

	for (i = 0 ; i < pFilter->GetNbrScratches() ; ++i)
	{
		Stream.DebugOutput("Scratch : ");
		Stream << pFilter->GetScratchInst(i);
		Stream.DebugOutput("\r\n");
	}
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
