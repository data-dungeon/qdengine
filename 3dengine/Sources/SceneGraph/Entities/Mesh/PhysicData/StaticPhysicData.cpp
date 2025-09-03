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
//	CLASS:	STATIC_PHYSIC_DATA
//
//	06-02-16:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/StaticPhysicData)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include	"StaticPhysicData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STATIC_PHYSIC_DATA constructor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
STATIC_PHYSIC_DATA::STATIC_PHYSIC_DATA()
:
COMMUNICATOR		(GetGenericCommunicatorDescriptor()),
_GeometricDataSize	(0),
_pGeometricData		(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STATIC_PHYSIC_DATA destructor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
STATIC_PHYSIC_DATA::~STATIC_PHYSIC_DATA()
{
	delete [] (Char*) _pGeometricData;
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("STPHDATA", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("STPHDATA", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	STATIC_PHYSIC_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STATIC_PHYSIC_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	STATIC_PHYSIC_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("STATIC_PHYSIC_DATA"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STATIC_PHYSIC_DATA,
											QDT_COMMENT("92B23E52-830F-4E04-D1A0-8959CA8A3DE2"),
											sizeof(STATIC_PHYSIC_DATA),
											ComInit,
											QDT_COMMENT("Class for static physic data exported from maya"),
											COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::EM_Construct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	STATIC_PHYSIC_DATA	*pStaticPhysicData;
	pStaticPhysicData = static_cast<STATIC_PHYSIC_DATA *>(pO);

	QDT_NEW_PLACED(pStaticPhysicData, STATIC_PHYSIC_DATA)();
	QDT_ASSERT(pStaticPhysicData);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::EM_Destruct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	STATIC_PHYSIC_DATA	*pStaticPhysicData;
	pStaticPhysicData = static_cast<STATIC_PHYSIC_DATA *>(pO);

	QDT_DELETE_PLACED(pStaticPhysicData, STATIC_PHYSIC_DATA);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::CM_ReadChunk1(IO_STREAM &		Stream,
										  COMMUNICATOR *	pCommunicator)
{
	STATIC_PHYSIC_DATA	*pStaticPhysicData;
	pStaticPhysicData = static_cast<STATIC_PHYSIC_DATA *>(pCommunicator);

	Stream >> pStaticPhysicData->_GeometricDataSize;

	pStaticPhysicData->_pGeometricData = static_cast<void *>(new UInt8[pStaticPhysicData->_GeometricDataSize]);
	Stream.GetBufferFast(pStaticPhysicData->_pGeometricData, pStaticPhysicData->_GeometricDataSize);
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	STATIC_PHYSIC_DATA::CM_WriteChunk1(IO_STREAM &		Stream,
										   COMMUNICATOR *	pCommunicator)
{
	STATIC_PHYSIC_DATA	*pStaticPhysicData;
	pStaticPhysicData = static_cast<STATIC_PHYSIC_DATA *>(pCommunicator);

	Stream << pStaticPhysicData->_GeometricDataSize;
	Stream.PutBufferFast(pStaticPhysicData->_pGeometricData, pStaticPhysicData->_GeometricDataSize);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
