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
//	CLASS:	STREAM_ANIMATION_OFFSET
//
//	03-04-02:	AMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamAnimationOffset.h"

#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*					STREAM_ANIMATION_OFFSET::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		STREAM_ANIMATION_OFFSET constructor
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
STREAM_ANIMATION_OFFSET::STREAM_ANIMATION_OFFSET()
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
STREAM_ANIMATION_OFFSET::~STREAM_ANIMATION_OFFSET()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION_OFFSET::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct,EM_Destruct);
	pCD->SetConstructorInMemory(EM_Construct_In_Memory);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	STREAM_ANIMATION_OFFSET::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAM_ANIMATION_OFFSET) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-28:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	STREAM_ANIMATION_OFFSET::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("STREAM_ANIMATION_OFFSET"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAM_ANIMATION_OFFSET,
											QDT_COMMENT("668A22CA-B5A7-163C-58F8-53514CA2179A"),
											sizeof(STREAM_ANIMATION_OFFSET), 
											ComInit,
											QDT_COMMENT("a Offset Table on a streamed Animation"),
											OFFSET_PARTITION::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-04-02:	AMA - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION_OFFSET::EM_Construct(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	STREAM_ANIMATION_OFFSET *		pObject;
	pObject = static_cast<STREAM_ANIMATION_OFFSET *>(pO);
	QDT_NEW_PLACED(pObject, STREAM_ANIMATION_OFFSET)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct_In_Memory
//	Object:		
//	04-03-01:	VMA - Created
//-----------------------------------------------------------------------------
void	STREAM_ANIMATION_OFFSET::EM_Construct_In_Memory(void * const	pO,
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
void	STREAM_ANIMATION_OFFSET::EM_Destruct(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	STREAM_ANIMATION_OFFSET *	pObject;
	pObject = static_cast<STREAM_ANIMATION_OFFSET *> (pO);
	QDT_DELETE_PLACED(pObject, STREAM_ANIMATION_OFFSET);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
