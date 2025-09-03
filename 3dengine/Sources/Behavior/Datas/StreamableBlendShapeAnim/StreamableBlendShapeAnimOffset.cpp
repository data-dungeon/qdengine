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
//	CLASS:	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET
//
//	03-04-02:	AMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamableBlendShapeAnimOffset.h"

#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*					STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM_OFFSET constructor
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::STREAMABLE_BLEND_SHAPE_ANIM_OFFSET()
:
OFFSET_PARTITION(GetGenericCommunicatorDescriptor())
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		SOUND_OFFSET destructor
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::~STREAMABLE_BLEND_SHAPE_ANIM_OFFSET()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct,EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAMABLE_BLEND_SHAPE_ANIM_OFFSET));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-10-07:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("STREAMABLE_BLEND_SHAPE_ANIM_OFFSET"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAMABLE_BLEND_SHAPE_ANIM_OFFSET,
										QDT_COMMENT("0105EFFA-F837-D02D-2647-7A03ACABFB21"),
										sizeof(STREAMABLE_BLEND_SHAPE_ANIM_OFFSET), 
										ComInit,
										QDT_COMMENT("a Offset Table on a streamed blend shape Animation"),
										OFFSET_PARTITION::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::EM_Construct(void * const	pO,
														 void * const	pR,
														 void * const	pP)
{
	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET *		pObject;
	pObject = static_cast<STREAMABLE_BLEND_SHAPE_ANIM_OFFSET *>(pO);
	QDT_NEW_PLACED(pObject, STREAMABLE_BLEND_SHAPE_ANIM_OFFSET)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-03-01:	VMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::EM_Construct_In_Memory(void * const	pO,
																   void * const	pR,
																   void * const	pP)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET::EM_Destruct(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	STREAMABLE_BLEND_SHAPE_ANIM_OFFSET *	pObject;
	pObject = static_cast<STREAMABLE_BLEND_SHAPE_ANIM_OFFSET *> (pO);
	QDT_DELETE_PLACED(pObject, STREAMABLE_BLEND_SHAPE_ANIM_OFFSET);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
