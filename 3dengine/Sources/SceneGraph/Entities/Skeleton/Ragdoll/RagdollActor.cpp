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
//	CLASS:	RAGDOLL_ACTOR
//
//	06-03-20:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollActor)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeBoxData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeSphereData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeCapsuleData)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/PhysicData/PhysicShapeTriangleData)

#ifdef _DEBUG
	#include	"RagdollActor.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RAGDOLL_ACTOR constructor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
RAGDOLL_ACTOR::RAGDOLL_ACTOR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAGDOLL_ACTOR destructor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
RAGDOLL_ACTOR::~RAGDOLL_ACTOR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ReadChunk
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::ReadChunk(IO_STREAM &	Stream)
{
	Stream >> _rDensity;
	Stream >> _rMass;
	Stream >> _CenterOfMass;
	Stream >> _LinearDamping;
	Stream >> _AngularDamping;
	Stream >> _bGravity;
	Stream >> _nNbrSolverIterations;

	UInt32	nNbrShapesData;
	Stream >> nNbrShapesData;
	SetNbrShapesData(nNbrShapesData);
	
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
		SetShapeData(i, pShapeData);
	}

	Stream >> _rBounciness;
	Stream >> _rStaticFriction;
	Stream >> _rDynamicFriction;
}

//-----------------------------------------------------------------------------
//	Name:		WriteChunk
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::WriteChunk(IO_STREAM &	Stream)
{
	Stream.DebugOutput("Density :");
	Stream << _rDensity;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Mass :");
	Stream << _rMass;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Center of mass :");
	Stream << _CenterOfMass;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear damping :");
	Stream << _LinearDamping;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Angular damping :");
	Stream << _AngularDamping;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Gravity :");
	Stream << _bGravity;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of solver iterations :");
	Stream << _nNbrSolverIterations;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Number of shapes :");
	Stream << _nNbrShapesData;
	Stream.DebugOutput("\r\n");

	for	(UInt32 i = 0 ; i < _nNbrShapesData ; ++i)
	{
		Stream << _pShapesData[i]->GetType();
		_pShapesData[i]->WriteChunk(Stream);
	}

	Stream.DebugOutput("Bounciness :");
	Stream << _rBounciness;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Static friction :");
	Stream << _rStaticFriction;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Dynamic friction :");
	Stream << _rDynamicFriction;
	Stream.DebugOutput("\r\n");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
