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
// CLASS: UNIFORM_FIELD
//
//
//	01-03-12:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (UNIFORM_FIELD)
//
//	The UNIFORM_FIELD class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/UniformField/UniformField)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		UNIFORM_FIELD constructor
//	Object:		Constructor
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
UNIFORM_FIELD::UNIFORM_FIELD()
:
FORCE_FIELD_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_UNIFORM_FIELD)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		UNIFORM_FIELD destructor
//	Object:		Destructor
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
UNIFORM_FIELD::~UNIFORM_FIELD()
{
	// Do nothing
}
/*
//-----------------------------------------------------------------------------
//	Name:		UNIFORM_FIELD::GetForceToApply
//	Object:		Get the force to apply to the physic body
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void UNIFORM_FIELD::GetForceToApply(const PARTICLE &	Particle,
									VECTOR &			Force) const
{
	Force = _Direction * _rMagnitude;
}

//-----------------------------------------------------------------------------
//	Name:		UNIFORM_FIELD::GetForceToApply
//	Object:		Get the force to apply to the physic body
//	04-10-27:	RMA - Created
//-----------------------------------------------------------------------------
void UNIFORM_FIELD::GetForceToApply(PARTICLE_SPRITE_ARRAY * pArray, 
									unsigned int i, 
									VECTOR & Force) const
{
	Force = _Direction * _rMagnitude;
}
*/
//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	UNIFORM_FIELD::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("UNIF_F", 1, CM_ReadChunk);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("UNIF_F", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	UNIFORM_FIELD::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_SKIN) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	UNIFORM_FIELD::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("UNIFORM_FIELD"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_UNIFORM_FIELD,
											QDT_COMMENT("8574EE0C-DBE1-0B8E-4102-433384FC568C"),
											sizeof(UNIFORM_FIELD),
											ComInit,
											QDT_COMMENT("Class for uniform fields"),
											FORCE_FIELD_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	UNIFORM_FIELD::EM_Construct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	UNIFORM_FIELD	*pUniformField;
	pUniformField = static_cast<UNIFORM_FIELD *>(pO);

	QDT_NEW_PLACED(pUniformField, UNIFORM_FIELD)();
	QDT_ASSERT(pUniformField);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	UNIFORM_FIELD::EM_Destruct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	UNIFORM_FIELD	*pUniformField;
	pUniformField = static_cast<UNIFORM_FIELD *>(pO);

	QDT_DELETE_PLACED(pUniformField, UNIFORM_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	UNIFORM_FIELD::CM_ReadChunk(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	UNIFORM_FIELD	*pUniformField;
	pUniformField = static_cast<UNIFORM_FIELD *>(pCommunicator);

	pUniformField->ReadCommon(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	UNIFORM_FIELD::CM_WriteChunk(IO_STREAM &	Stream,
									 COMMUNICATOR *	pCommunicator)
{
	UNIFORM_FIELD	*pUniformField;
	pUniformField = static_cast<UNIFORM_FIELD *>(pCommunicator);

	pUniformField->WriteCommon(Stream);
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
