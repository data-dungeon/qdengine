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
//	CLASS:	NODE_MAPPER
//
//	04-05-13:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Node/NodeMapper)
#include	INCL_3DENGINE(SceneGraph/Node/Node)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void * NODE_MAPPER::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		NODE_MAPPER constructor
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
NODE_MAPPER::NODE_MAPPER()
:
_nNbrNodes(0),
_pNodes(NULL),
_pEntities(NULL),
COMMUNICATOR(GetGenericCommunicatorDescriptor(), NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		NODE_MAPPER constructor
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
NODE_MAPPER::NODE_MAPPER(COM_HANDLE * pHandle)
:
_nNbrNodes(0),
_pNodes(NULL),
_pEntities(NULL),
COMMUNICATOR(GetGenericCommunicatorDescriptor(), pHandle)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		NODE_MAPPER destructor
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
NODE_MAPPER::~NODE_MAPPER()
{
	delete [] _pNodes;
	delete [] _pEntities;
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE &	NODE_MAPPER::GetNode(unsigned int	i) const
{
	QDT_ASSERT(i < _nNbrNodes);
	return (_pNodes[i]);
}

//-----------------------------------------------------------------------------
//	Name:		GetEntity
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE &	NODE_MAPPER::GetEntity(unsigned int	i) const
{
	QDT_ASSERT(i < _nNbrNodes);
	return (_pEntities[i]);
}

//-----------------------------------------------------------------------------
//	Name:		AllocateMemory
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	NODE_MAPPER::AllocateMemory(unsigned int	i)
{
	_nNbrNodes = i;
	_pNodes = new COM_INSTANCE[_nNbrNodes];
	_pEntities = new COM_INSTANCE[_nNbrNodes];
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetNode
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE_MAPPER::SetNode(unsigned int			i,
							 const COM_INSTANCE &	Node)
{
	QDT_ASSERT(i < _nNbrNodes);
	_pNodes[i] = Node;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetEntity
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	NODE_MAPPER::SetEntity(unsigned int			i,
							   const COM_INSTANCE &	Node)
{
	QDT_ASSERT(i < _nNbrNodes);
	_pEntities[i] = Node;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NODE_MAPPER::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_MAPPER));
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::Init(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);

#ifndef _MASTER_ONLY_DATABANK
	pCD->AddReadChunk("NDMAPPER", 0, CM_ReadChunkMatInfos);
#endif

#ifndef _MASTER
	pCD->AddWriteChunk("NDMAPPER", 0, CM_WriteChunkMatInfos);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-10-12:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	NODE_MAPPER::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("NODE_MAPPER"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_NODE_MAPPER, 
										QDT_COMMENT("7EB27A52-CF0F-6A04-7DA0-8559D68A19E2"),
										sizeof(NODE_MAPPER), Init,
										QDT_COMMENT("The NODE_MAPPER class")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::EM_Construct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	NODE_MAPPER *	pObject;
	COM_HANDLE * pHandle;

	pObject = static_cast<NODE_MAPPER *>(pO);
	pHandle = static_cast<COM_HANDLE*>(pP);

	QDT_NEW_PLACED(pObject, NODE_MAPPER)(pHandle);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::EM_Construct_In_Memory(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::EM_Destruct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	NODE_MAPPER *	pObject;
	pObject = static_cast<NODE_MAPPER*> (pO);
	pObject->NODE_MAPPER::~NODE_MAPPER();
}

#ifndef _MASTER_ONLY_DATABANK
//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunkMatInfos
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::CM_ReadChunkMatInfos(IO_STREAM &		Stream,
										  COMMUNICATOR *	pCommunicator)
{
	NODE_MAPPER * pMapper = static_cast<NODE_MAPPER *>(pCommunicator);
	Stream >> pMapper->_nNbrNodes;
	pMapper->AllocateMemory(pMapper->_nNbrNodes);

	unsigned int i;
	for (i = 0 ; i < pMapper->_nNbrNodes ; ++i)
	{
		Stream >> pMapper->_pNodes[i];
		Stream >> pMapper->_pEntities[i];
	}

	pMapper->ApplyMap();
}
#endif

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunkMatInfos
//	Object:		
//	04-05-13:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::CM_WriteChunkMatInfos(IO_STREAM &		Stream,
										   COMMUNICATOR *	pCommunicator)
{
	NODE_MAPPER * pMapper = static_cast<NODE_MAPPER *>(pCommunicator);

	Stream.DebugOutput("Number of nodes : ");
	Stream << pMapper->_nNbrNodes; 
	Stream.DebugOutput("\r\n");

	unsigned int i;
	for (i = 0 ; i < pMapper->_nNbrNodes ; ++i)
	{
		Stream.DebugOutput("Node :\t");
		if (pMapper->_pNodes[i].HasHandle())
		{
			Stream.DebugOutput(pMapper->_pNodes[i].GetName());
			Stream.DebugOutput("\t");
		}

		Stream << pMapper->_pNodes[i];
		Stream.DebugOutput("\r\n");
		
		Stream.DebugOutput("Entity :\t");
		if (pMapper->_pEntities[i].HasHandle())
		{
			Stream.DebugOutput(pMapper->_pEntities[i].GetName());
			Stream.DebugOutput("\t");
		}
		
		Stream << pMapper->_pEntities[i];
		Stream.DebugOutput("\r\n");
	}

	Stream.DebugOutput("\r\n");
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ApplyMap
//	Object:		
//	04-05-14:	RMA - Created
//-----------------------------------------------------------------------------
void	NODE_MAPPER::ApplyMap()
{
	QDT_NOT_IMPLEMENTED();
/*
	unsigned int i;
	for (i = 0 ; i < _nNbrNodes ; ++i)
	{
		NODE * pNode = static_cast<NODE*>(_pNodes[i].GetCommunicator());
		pNode->SetEntityInst(_pEntities[i].GetCommunicator());
	}
*/
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
