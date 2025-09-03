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
//	CLASS:	ENTITY_CONTROLLER_ABC
//
//	01-12-18:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"EntityControllerABC.h"

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeAnimation)
#include INCL_3DENGINE(Behavior/Controllers/Entity Controllers/BlendShapeController)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENTITY_CONTROLLER_ABC constructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_CONTROLLER_ABC::ENTITY_CONTROLLER_ABC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_CONTROLLER_ABC destructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_CONTROLLER_ABC::~ENTITY_CONTROLLER_ABC()
{
	DeleteBehaviors();
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_CONTROLLER_ABC::IsEmpty() const
{
	if (_dlBehaviors.IsEmpty())
	{
		return (true);
	}

	return (false); 
}

//-----------------------------------------------------------------------------
//	Name:		AddBehavior
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_CONTROLLER_ABC::AddBehavior(ENTITY_BEHAVIOR_ABC *	pBehavior)
{
	if	(pBehavior->IsBlendShapeAnimation())
	{
		_dlBehaviors.InsertTail(pBehavior);
		return;
	}

	_dlBehaviors.InsertHead(pBehavior);
}

//-----------------------------------------------------------------------------
//	Name:		DeleteBehaviors
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_CONTROLLER_ABC::DeleteBehaviors()
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC *>::ITERATOR itEach = _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC *>::ITERATOR itTail = _dlBehaviors.GetTail();

	while (itEach != itTail)
	{
		delete (*itEach);
		++itEach;
	}

	_dlBehaviors.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		DeleteFinishedBehaviors
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_CONTROLLER_ABC::DeleteFinishedBehaviors()
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itEach	= _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itTail	= _dlBehaviors.GetTail();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itPrev;
	
	for(; itEach != itTail ;)
	{
		itPrev = itEach;

		++itEach;
		
		if ((*itPrev)->IsFinished())
		{ 
			_dlBehaviors.Remove(itPrev);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		DeleteBehavior
//	Object:		
//	03-05-12:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_CONTROLLER_ABC::DeleteBehavior(ENTITY_BEHAVIOR_ABC *	pBehavior)
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itEach	= _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itTail	= _dlBehaviors.GetTail();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itPrev;
	
	for(; itEach != itTail ;)
	{
		itPrev = itEach;

		++itEach;
		
		if ((*itPrev) == pBehavior)
		{ 
			_dlBehaviors.Remove(itPrev);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeController
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_CONTROLLER_ABC::IsBlendShapeController() const
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeController
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_CONTROLLER *	ENTITY_CONTROLLER_ABC::GetBlendShapeController()
{
	QDT_ASSERT(IsBlendShapeController());
	return (static_cast<BLEND_SHAPE_CONTROLLER *>(this));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
