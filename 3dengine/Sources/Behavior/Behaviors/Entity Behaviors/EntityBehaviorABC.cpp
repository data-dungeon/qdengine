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
//	CLASS:	ENTITY_BEHAVIOR_ABC
//
//	01-12-12:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"EntityBehaviorABC.h"

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeAnimation)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeInterpolation)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeBlink)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapePeriodicBlink)
#include INCL_3DENGINE(SceneGraph/Node/Node)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ENTITY_BEHAVIOR_ABC constructor
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_BEHAVIOR_ABC::ENTITY_BEHAVIOR_ABC()
:
_rCurrentFrame	(0.0f),
_NodeInst		(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_bIsFinished	(false),
_rSpeed			(1.0f),
_rStartFrame	(0.0f),
_rEndFrame		(0.0f),
_nNbrLoops		(1)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_BEHAVIOR_ABC destructor
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_BEHAVIOR_ABC::~ENTITY_BEHAVIOR_ABC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeAnimation
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_BEHAVIOR_ABC::IsBlendShapeAnimation() const
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeInterpolation
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_BEHAVIOR_ABC::IsBlendShapeInterpolation() const
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeBlink
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_BEHAVIOR_ABC::IsBlendShapeBlink() const
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapePeriodicBlink
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_BEHAVIOR_ABC::IsBlendShapePeriodicBlink() const
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
NODE *	ENTITY_BEHAVIOR_ABC::GetNode() const
{
	NODE *pNode = static_cast<NODE *>(_NodeInst.GetCommunicator());
	QDT_ASSERT(pNode);

	return (pNode);
}

//-----------------------------------------------------------------------------
//	Name:		SetNodeInst
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_ABC::SetNodeInst(const COM_INSTANCE & NodeInst)
{
	_NodeInst = NodeInst;
}

//-----------------------------------------------------------------------------
//	Name:		SetFinished
//	Object:		
//	03-06-19:	AMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_ABC::SetFinished(bool	b)
{
	_bIsFinished = b;
}

//-----------------------------------------------------------------------------
//	Name:		IsFinished
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	ENTITY_BEHAVIOR_ABC::IsFinished() const
{
	return (_bIsFinished);
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_ABC::Update(float	rSpeedFactor)
{
	UpdateCurrentFrame(rSpeedFactor);
	UpdateData(rSpeedFactor);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateCurrentFrame
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_ABC::UpdateCurrentFrame(float	rSpeedFactor)
{
	_rCurrentFrame += rSpeedFactor * _rSpeed;
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_ABC::Reset()
{
	_rCurrentFrame = _rStartFrame;
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeAnimation
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIMATION *	ENTITY_BEHAVIOR_ABC::GetBlendShapeAnimation()
{
	QDT_ASSERT(IsBlendShapeAnimation());
	return (static_cast<BLEND_SHAPE_ANIMATION *>(this));
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeInterpolation
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_INTERPOLATION *	ENTITY_BEHAVIOR_ABC::GetBlendShapeInterpolation()
{
	QDT_ASSERT(IsBlendShapeInterpolation());
	return (static_cast<BLEND_SHAPE_INTERPOLATION *>(this));
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeBlink
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_BLINK *	ENTITY_BEHAVIOR_ABC::GetBlendShapeBlink()
{
	QDT_ASSERT(IsBlendShapeBlink());
	return (static_cast<BLEND_SHAPE_BLINK *>(this));
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapePeriodicBlink
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_PERIODIC_BLINK *	ENTITY_BEHAVIOR_ABC::GetBlendShapePeriodicBlink()
{
	QDT_ASSERT(IsBlendShapePeriodicBlink());
	return (static_cast<BLEND_SHAPE_PERIODIC_BLINK *>(this));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
