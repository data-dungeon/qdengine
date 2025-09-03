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
//	CLASS:	PERTURBATION_FIELD
//
//	03-10-07:	FPR - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/PerturbationField/PerturbationField)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PERTURBATION_FIELD constructor
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
PERTURBATION_FIELD::PERTURBATION_FIELD()
:
FORCE_FIELD_ABC(GetGenericCommunicatorDescriptor(), ENTITY_PERTURBATION_FIELD)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PERTURBATION_FIELD destructor
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
PERTURBATION_FIELD::~PERTURBATION_FIELD()
{
	// Do nothing
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetForceToApply
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
void	PERTURBATION_FIELD::GetForceToApply(const PARTICLE &	Particle,
											VECTOR &			Force) const
{
	Force.SetXYZ( MATH::Rand(2.0f)-1.0f, 0.0f, MATH::Rand(2.0f)-1.0f );
	//Why /2.0f ? Don't know, same code in Maya but needs half...
	Force.SetLength(_rMagnitude* 0.5f);

}

//-----------------------------------------------------------------------------
//	Name:		GetForceToApply
//	Object:		
//	04-10-27:	FPR - Created
//-----------------------------------------------------------------------------
void	PERTURBATION_FIELD::GetForceToApply(PARTICLE_SPRITE_ARRAY * pArray, 
											unsigned int i,
											VECTOR & Force) const
{
	Force.SetXYZ( MATH::Rand(2.0f)-1.0f, 0.0f, MATH::Rand(2.0f)-1.0f );
	//Why /2.0f ? Don't know, same code in Maya but needs half...
	Force.SetLength(_rMagnitude* 0.5f);
}
*/
//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
void	PERTURBATION_FIELD::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("PERTURBATION_F", 1, CM_ReadChunk);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("PERTURBATION_F", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PERTURBATION_FIELD::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PERTURBATION_FIELD) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PERTURBATION_FIELD::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("PERTURBATION_FIELD"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PERTURBATION_FIELD,
											QDT_COMMENT("ABFE226E-46BB-B300-92AC-763573F6F19E"),
											sizeof(PERTURBATION_FIELD),
											ComInit,
											QDT_COMMENT("Class for perturbation fields"),
											FORCE_FIELD_ABC::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
void	PERTURBATION_FIELD::EM_Construct(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	PERTURBATION_FIELD	*pPerturbField;
	pPerturbField = static_cast<PERTURBATION_FIELD *>(pO);

	QDT_NEW_PLACED(pPerturbField, PERTURBATION_FIELD)();
	QDT_ASSERT(pPerturbField);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
void	PERTURBATION_FIELD::EM_Destruct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	PERTURBATION_FIELD	*pPerturbField;
	pPerturbField = static_cast<PERTURBATION_FIELD *>(pO);

	QDT_DELETE_PLACED(pPerturbField, PERTURBATION_FIELD);
}


	
//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	PERTURBATION_FIELD::CM_ReadChunk(IO_STREAM &	Stream,
										 COMMUNICATOR *	pCommunicator)
{
	PERTURBATION_FIELD	*pPerturbField;
	pPerturbField = static_cast<PERTURBATION_FIELD *>(pCommunicator);

	pPerturbField->ReadCommon(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	PERTURBATION_FIELD::CM_WriteChunk(IO_STREAM &		Stream,
										  COMMUNICATOR *	pCommunicator)
{
	PERTURBATION_FIELD	*pPerturbField;
	pPerturbField = static_cast<PERTURBATION_FIELD *>(pCommunicator);

	pPerturbField->WriteCommon(Stream);
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
