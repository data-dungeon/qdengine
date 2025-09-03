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
//	CLASS:	ENTITY_CONTROLLER_FACTORY
//
//	01-12-18:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"EntityControllerFactory.h"

#include	INCL_3DENGINE(Behavior/Controllers/Entity Controllers/BlendShapeController)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

ENTITY_CONTROLLER_FACTORY   * ENTITY_CONTROLLER_FACTORY::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		ENTITY_CONTROLLER_FACTORY constructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_CONTROLLER_FACTORY::ENTITY_CONTROLLER_FACTORY()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_CONTROLLER_FACTORY destructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_CONTROLLER_FACTORY::~ENTITY_CONTROLLER_FACTORY()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_CONTROLLER_FACTORY *	ENTITY_CONTROLLER_FACTORY::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new ENTITY_CONTROLLER_FACTORY;
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_CONTROLLER_FACTORY::Suicide()
{
	if (_pInstance)
	{
		delete (_pInstance);
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateControllerIfNeed
//	Object:		
//	04-12-01:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_CONTROLLER_FACTORY::CreateControllerIfNeed(int		nBlendShapeID,
														  NODE *	pNode) const
{
	QDT_NOT_IMPLEMENTED();

}

//-----------------------------------------------------------------------------
//	Name:		CreateBlendShapeController
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_CONTROLLER *	ENTITY_CONTROLLER_FACTORY::CreateBlendShapeController(int	nNbrWeights) const
{
	return (new BLEND_SHAPE_CONTROLLER(nNbrWeights));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
