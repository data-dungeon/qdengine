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
//	CLASS:	LIFE_FIELD
//
//	02-01-24:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/LifeField/LifeField)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef	_DEBUG
	#include	"LifeField.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		LIFE_FIELD constructor
//	Object:		Constructor
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
LIFE_FIELD::LIFE_FIELD()
:
FORCE_FIELD_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_LIFE_FIELD)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		LIFE_FIELD destructor
//	Object:		Destructor
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
LIFE_FIELD::~LIFE_FIELD()
{
	// Do nothing
}
/*
//-----------------------------------------------------------------------------
//	Name:		LIFE_FIELD::GetForceToApply
//	Object:		Get the force to apply to the physic body
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void LIFE_FIELD::GetForceToApply(const PARTICLE &	Particle,
								 VECTOR &			Force) const
{
	Force = VECTOR::ZERO;
}

//-----------------------------------------------------------------------------
//	Name:		LIFE_FIELD::GetForceToApply
//	Object:		Get the force to apply to the physic body
//	04-10-27:	RMA - Created
//-----------------------------------------------------------------------------
void	LIFE_FIELD::GetForceToApply(PARTICLE_SPRITE_ARRAY * pArray, 
									unsigned int i, 
									VECTOR & Force) const
{
	Force = VECTOR::ZERO;
}
*/
//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	LIFE_FIELD::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("LIFE_F", 1, CM_ReadChunk);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("LIFE_F", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	LIFE_FIELD::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIFE_FIELD) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	LIFE_FIELD::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("LIFE_FIELD"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_LIFE_FIELD,
											QDT_COMMENT("D1C83A52-634F-3085-A05E-BA5E5FBB0590"),
											sizeof(LIFE_FIELD),
											ComInit,
											QDT_COMMENT("Class for life fields"),
											FORCE_FIELD_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	LIFE_FIELD::EM_Construct(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	LIFE_FIELD	*pLifeField;
	pLifeField = static_cast<LIFE_FIELD *>(pO);

	QDT_NEW_PLACED(pLifeField, LIFE_FIELD)();
	QDT_ASSERT(pLifeField);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	LIFE_FIELD::EM_Destruct(void * const	pO,
								void * const	pR,
								void * const	pP)
{
	LIFE_FIELD	*pLifeField;
	pLifeField = static_cast<LIFE_FIELD *>(pO);

	QDT_DELETE_PLACED(pLifeField, LIFE_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	LIFE_FIELD::CM_ReadChunk(IO_STREAM &	Stream,
								 COMMUNICATOR *	pCommunicator)
{
	LIFE_FIELD	*pLifeField;
	pLifeField = static_cast<LIFE_FIELD *>(pCommunicator);

	pLifeField->ReadCommon(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	LIFE_FIELD::CM_WriteChunk(IO_STREAM &		Stream,
								  COMMUNICATOR *	pCommunicator)
{
	LIFE_FIELD	*pLifeField;
	pLifeField = static_cast<LIFE_FIELD *>(pCommunicator);

	Stream.DebugOutput("LIFE_FIELD :\t");
	Stream.DebugOutput(pLifeField->GetName());
	Stream.DebugOutput("\r\n");

	pLifeField->WriteCommon(Stream);
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
