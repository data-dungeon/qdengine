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
//	CLASS:	ENGINE_TEXTURE_ABC
//
//	05-11-10:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KRENDERER(Renderer/RenderContext)
#include	INCL_3DENGINE(SceneGraph/Texture/EngineTextureABC)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_ABC Constructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_ABC::ENGINE_TEXTURE_ABC(COMMUNICATOR_DESCRIPTOR *pCD) : COMMUNICATOR(pCD)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENGINE_TEXTURE_ABC Destructor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
ENGINE_TEXTURE_ABC::~ENGINE_TEXTURE_ABC()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_ABC::GetGenericCommunicatorDescriptor()
{
	return (COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_ABC));
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR * ENGINE_TEXTURE_ABC::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("ENGINE_TEXTURE_ABC"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_ENGINE_TEXTURE_ABC,
										QDT_COMMENT("43044444-6DFA-F943-8BE3-26908EE85888"), 
										sizeof(ENGINE_TEXTURE_ABC), 
										ComInit, 
										"",
										COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_ABC::ComInit(CLASS_DESCRIPTOR * const pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_ABC::EM_Construct(void * const pO, 
									  void * const pR, 
									  void * const pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-11-10:	RMA - Created
//-----------------------------------------------------------------------------
void ENGINE_TEXTURE_ABC::EM_Destruct(void * const pO, 
									 void * const pR, 
									 void * const pP)
{
	// Abstract base class!
	QDT_FAIL();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
