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
// CLASS: RADIAL_FIELD
//
//
//	01-03-13:	RMA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (RADIAL_FIELD)
//
//	The RADIAL_FIELD class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/ForceFields/RadialField/RadialField)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VORTEX_FIELD constructor
//	Object:		Constructor
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
RADIAL_FIELD::RADIAL_FIELD()
:
FORCE_FIELD_ABC	(GetGenericCommunicatorDescriptor(), ENTITY_RADIAL_FIELD)
{
//	Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VORTEX_FIELD destructor
//	Object:		Destructor
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
RADIAL_FIELD::~RADIAL_FIELD()
{
	// Do nothing
}
/*
//-----------------------------------------------------------------------------
//	Name:		RADIAL_FIELD::GetForceToApply
//	Object:		Get the force to apply to the physic body
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void RADIAL_FIELD::GetForceToApply(const PARTICLE &	Particle,
								   VECTOR &			Force) const
{
	Force = Particle.GetPosition() - _TransCenter;
	Force.SetLength(_rMagnitude);
}

//-----------------------------------------------------------------------------
//	Name:		RADIAL_FIELD::GetForceToApply
//	Object:		Get the force to apply to the physic body
//	04-10-27:	RMA - Created
//-----------------------------------------------------------------------------
void RADIAL_FIELD::GetForceToApply(PARTICLE_SPRITE_ARRAY * pArray, 
								   unsigned int i, 
								   VECTOR & Force) const
{
	VECTOR Position;
	pArray->GetPosition(i, Position);
	Force = Position - _TransCenter;
	Force.SetLength(_rMagnitude);
}
*/
//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	RADIAL_FIELD::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
#ifndef _MASTER_ONLY_DATABANK
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("RADIAL_F", 1, CM_ReadChunk);
#endif
#ifndef _MASTER
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("RADIAL_F", 1, CM_WriteChunk);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	RADIAL_FIELD::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RADIAL_FIELD) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	RADIAL_FIELD::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("RADIAL_FIELD"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_RADIAL_FIELD,
											QDT_COMMENT("2E92DB53-DA2C-3E4E-9FE4-C12CBB302392"),
											sizeof(RADIAL_FIELD),
											ComInit,
											QDT_COMMENT("Class for radial fields"),
											FORCE_FIELD_ABC::GetGenericCommunicatorDescriptor()));

}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	RADIAL_FIELD::EM_Construct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	RADIAL_FIELD	*pRadialField;
	pRadialField = static_cast<RADIAL_FIELD *>(pO);

	QDT_NEW_PLACED(pRadialField, RADIAL_FIELD)();
	QDT_ASSERT(pRadialField);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
void	RADIAL_FIELD::EM_Destruct(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	RADIAL_FIELD	*pRadialField;
	pRadialField = static_cast<RADIAL_FIELD *>(pO);

	QDT_DELETE_PLACED(pRadialField, RADIAL_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	RADIAL_FIELD::CM_ReadChunk(IO_STREAM &		Stream,
								   COMMUNICATOR *	pCommunicator)
{
	RADIAL_FIELD	*pRadialField;
	pRadialField = static_cast<RADIAL_FIELD *>(pCommunicator);

	pRadialField->ReadCommon(Stream);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk
//	Object:		
//	03-03-24:	ELE - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	RADIAL_FIELD::CM_WriteChunk(IO_STREAM &		Stream,
									COMMUNICATOR *	pCommunicator)
{
	RADIAL_FIELD	*pRadialField;
	pRadialField = static_cast<RADIAL_FIELD *>(pCommunicator);

	pRadialField->WriteCommon(Stream);
}
#endif

//=============================================================================
// CODE ENDS HERE
//=============================================================================
