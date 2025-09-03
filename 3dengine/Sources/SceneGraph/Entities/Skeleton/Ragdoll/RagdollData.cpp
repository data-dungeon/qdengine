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
//	CLASS:	RAGDOLL_DATA
//
//	06-03-20:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollData)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollActor)
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollConstraint)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include	"RagdollData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RAGDOLL_DATA constructor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
RAGDOLL_DATA::RAGDOLL_DATA()
:
COMMUNICATOR		(GetGenericCommunicatorDescriptor()),
_uiNbrActors		(0),
_pActors			(NULL),
_uiNbrConstraints	(0),
_pConstraints		(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAGDOLL_DATA destructor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
RAGDOLL_DATA::~RAGDOLL_DATA()
{
	if	(_pActors)
	{
		UInt32	i;

		for	(i = 0 ; i < _uiNbrActors ; ++i)
		{
			delete	(_pActors[i]);
		}
		delete [] _pActors;
	}

	if	(_pConstraints)
	{
		delete [] _pConstraints;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("RAGDATA ", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("RAGDATA ", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	RAGDOLL_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RAGDOLL_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	RAGDOLL_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("RAGDOLL_DATA"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RAGDOLL_DATA,
											QDT_COMMENT("55F9D07D-388E-E987-CB17-38F4E7B96255"),
											sizeof(RAGDOLL_DATA),
											ComInit,
											QDT_COMMENT("Class for ragdoll data exported from maya"),
											COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::EM_Construct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	RAGDOLL_DATA	*pRagdollData;
	pRagdollData = static_cast<RAGDOLL_DATA *>(pO);

	QDT_NEW_PLACED(pRagdollData, RAGDOLL_DATA)();
	QDT_ASSERT(pRagdollData);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::EM_Destruct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	RAGDOLL_DATA	*pRagdollData;
	pRagdollData = static_cast<RAGDOLL_DATA *>(pO);

	QDT_DELETE_PLACED(pRagdollData, RAGDOLL_DATA);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::CM_ReadChunk1(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	UInt32	i;
	bool	bActor;

	RAGDOLL_DATA	*pRagdollData;
	pRagdollData = static_cast<RAGDOLL_DATA *>(pCommunicator);

	// Actors
	Stream >> pRagdollData->_uiNbrActors;
	if	(pRagdollData->_uiNbrActors > 0)
	{
		pRagdollData->_pActors = new RAGDOLL_ACTOR*[pRagdollData->_uiNbrActors];

		for	(i = 0 ; i < pRagdollData->_uiNbrActors ; ++i)
		{
			Stream >> bActor;

			if	(bActor)
			{
				pRagdollData->_pActors[i] = new RAGDOLL_ACTOR();
				pRagdollData->_pActors[i]->ReadChunk(Stream);
			}
			else
			{
				pRagdollData->_pActors[i] = NULL;
			}
		}
	}

	// Constraints
	Stream >> pRagdollData->_uiNbrConstraints;
	if	(pRagdollData->_uiNbrConstraints > 0)
	{
		pRagdollData->_pConstraints = new RAGDOLL_CONSTRAINT[pRagdollData->_uiNbrConstraints];

		for	(i = 0 ; i < pRagdollData->_uiNbrConstraints ; ++i)
		{
			pRagdollData->_pConstraints[i].ReadChunk(Stream);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_DATA::CM_WriteChunk1(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	UInt32	i;

	RAGDOLL_DATA	*pRagdollData;
	pRagdollData = static_cast<RAGDOLL_DATA *>(pCommunicator);

	Stream.DebugOutput("Ragdoll data :");
	Stream.DebugOutput(pRagdollData->GetName());
	Stream.DebugOutput("\r\n");

	// Actors
	Stream.DebugOutput("Number of actors :");
	Stream << pRagdollData->_uiNbrActors;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < pRagdollData->_uiNbrActors ; ++i)
	{
		if	(pRagdollData->_pActors[i])
		{
			Stream << true;
			Stream.DebugOutput("Actor ");
			Stream.DebugOutput(static_cast<Int32>(i));
			Stream.DebugOutput(" :\r\n");
			pRagdollData->_pActors[i]->WriteChunk(Stream);
			Stream.DebugOutput("\r\n");
		}
		else
		{
			Stream << false;
		}
	}

	// Constraints
	Stream.DebugOutput("Number of constraints :");
	Stream << pRagdollData->_uiNbrConstraints;
	Stream.DebugOutput("\r\n");

	for	(i = 0 ; i < pRagdollData->_uiNbrConstraints ; ++i)
	{
		Stream.DebugOutput("Constraint ");
		Stream.DebugOutput(static_cast<Int32>(i));
		Stream.DebugOutput(" :\r\n");
		pRagdollData->_pConstraints[i].WriteChunk(Stream);
		Stream.DebugOutput("\r\n");
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
