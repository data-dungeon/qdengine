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
//	CLASS:	DYNAMIC_PHYSIC_DATA
//
//	06-02-16:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/DynamicPhysicData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeBoxData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeSphereData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeCapsuleData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeTriangleData)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include	"DynamicPhysicData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DYNAMIC_PHYSIC_DATA constructor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
DYNAMIC_PHYSIC_DATA::DYNAMIC_PHYSIC_DATA()
:
COMMUNICATOR	(GetGenericCommunicatorDescriptor()),
_rDensity		(0.0f),
_rMass			(0.0f),
_CenterOfMass	(0.0f, 0.0f, 0.0f),
_LinearDamping	(0.0f),
_AngularDamping	(0.0f),
_bKinematic		(false),
_bGravity		(true),
_nNbrShapesData	(0),
_pShapesData	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DYNAMIC_PHYSIC_DATA destructor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
DYNAMIC_PHYSIC_DATA::~DYNAMIC_PHYSIC_DATA()
{
	for	(UInt32 i = 0 ; i < _nNbrShapesData ; ++i)
	{
		delete	(_pShapesData[i]);
	}
	delete [] _pShapesData;
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("DYPHDATA", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("DYPHDATA", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	DYNAMIC_PHYSIC_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DYNAMIC_PHYSIC_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	DYNAMIC_PHYSIC_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("DYNAMIC_PHYSIC_DATA"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_DYNAMIC_PHYSIC_DATA,
											QDT_COMMENT("691D2711-84D2-14BB-197B-DDC8493D4DC9"),
											sizeof(DYNAMIC_PHYSIC_DATA),
											ComInit,
											QDT_COMMENT("Class for dynamic physic data exported from maya"),
											COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::EM_Construct(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	DYNAMIC_PHYSIC_DATA	*pDynamicPhysicData;
	pDynamicPhysicData = static_cast<DYNAMIC_PHYSIC_DATA *>(pO);

	QDT_NEW_PLACED(pDynamicPhysicData, DYNAMIC_PHYSIC_DATA)();
	QDT_ASSERT(pDynamicPhysicData);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::EM_Destruct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	DYNAMIC_PHYSIC_DATA	*pDynamicPhysicData;
	pDynamicPhysicData = static_cast<DYNAMIC_PHYSIC_DATA *>(pO);

	QDT_DELETE_PLACED(pDynamicPhysicData, DYNAMIC_PHYSIC_DATA);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::CM_ReadChunk1(IO_STREAM &		Stream,
										   COMMUNICATOR *	pCommunicator)
{
	DYNAMIC_PHYSIC_DATA	*pDynamicPhysicData;
	pDynamicPhysicData = static_cast<DYNAMIC_PHYSIC_DATA *>(pCommunicator);

	Stream >> pDynamicPhysicData->_rDensity;
	Stream >> pDynamicPhysicData->_rMass;
	Stream >> pDynamicPhysicData->_CenterOfMass;
	Stream >> pDynamicPhysicData->_LinearDamping;
	Stream >> pDynamicPhysicData->_AngularDamping;
	Stream >> pDynamicPhysicData->_bKinematic;
	Stream >> pDynamicPhysicData->_bGravity;
	Stream >> pDynamicPhysicData->_nNbrSolverIterations;

	UInt32	nNbrShapesData;
	Stream >> nNbrShapesData;
	pDynamicPhysicData->SetNbrShapesData(nNbrShapesData);
	
	for	(UInt32 i = 0 ; i < nNbrShapesData ; ++i)
	{
		UInt32										Type;
		PHYSIC_SHAPE_DATA::PHYSIC_SHAPE_DATA_TYPE	DataType;
		PHYSIC_SHAPE_DATA *							pShapeData;

		Stream >> Type;
		DataType = static_cast<PHYSIC_SHAPE_DATA::PHYSIC_SHAPE_DATA_TYPE>(Type);

		switch	(DataType)
		{
			case	PHYSIC_SHAPE_DATA::PSDT_BOX:
				pShapeData = static_cast<PHYSIC_SHAPE_DATA *>(new PHYSIC_SHAPE_BOX_DATA());
				break;

			case	PHYSIC_SHAPE_DATA::PSDT_SPHERE:
				pShapeData = static_cast<PHYSIC_SHAPE_DATA *>(new PHYSIC_SHAPE_SPHERE_DATA());
				break;

			case	PHYSIC_SHAPE_DATA::PSDT_CAPSULE:
				pShapeData = static_cast<PHYSIC_SHAPE_DATA *>(new PHYSIC_SHAPE_CAPSULE_DATA());
				break;

			case	PHYSIC_SHAPE_DATA::PSDT_TRIANGLE:
				pShapeData = static_cast<PHYSIC_SHAPE_DATA *>(new PHYSIC_SHAPE_TRIANGLE_DATA());
				break;
		}

		pShapeData->ReadChunk(Stream);
		pDynamicPhysicData->SetShapeData(i, pShapeData);
	}

	Stream >> pDynamicPhysicData->_rBounciness;
	Stream >> pDynamicPhysicData->_rStaticFriction;
	Stream >> pDynamicPhysicData->_rDynamicFriction;
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::CM_WriteChunk1(IO_STREAM &		Stream,
											COMMUNICATOR *	pCommunicator)
{
	DYNAMIC_PHYSIC_DATA	*pDynamicPhysicData;
	pDynamicPhysicData = static_cast<DYNAMIC_PHYSIC_DATA *>(pCommunicator);

	Stream.DebugOutput("Density :");
	Stream << pDynamicPhysicData->_rDensity;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Mass :");
	Stream << pDynamicPhysicData->_rMass;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Center of mass :");
	Stream << pDynamicPhysicData->_CenterOfMass;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear damping :");
	Stream << pDynamicPhysicData->_LinearDamping;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Angular damping :");
	Stream << pDynamicPhysicData->_AngularDamping;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Kinematic :");
	Stream << pDynamicPhysicData->_bKinematic;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Gravity :");
	Stream << pDynamicPhysicData->_bGravity;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of solver iterations :");
	Stream << pDynamicPhysicData->_nNbrSolverIterations;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of shapes :");
	Stream << pDynamicPhysicData->_nNbrShapesData;
	Stream.DebugOutput("\r\n");

	for	(UInt32 i = 0 ; i < pDynamicPhysicData->_nNbrShapesData ; ++i)
	{
		Stream << pDynamicPhysicData->_pShapesData[i]->GetType();
		pDynamicPhysicData->_pShapesData[i]->WriteChunk(Stream);
	}

	Stream.DebugOutput("Bounciness :");
	Stream << pDynamicPhysicData->_rBounciness;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Static friction :");
	Stream << pDynamicPhysicData->_rStaticFriction;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Dynamic friction :");
	Stream << pDynamicPhysicData->_rDynamicFriction;
	Stream.DebugOutput("\r\n");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
