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
//	CLASS:	ENTITY_BEHAVIOR_FACTORY
//
//	01-12-18:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"EntityBehaviorFactory.h"

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeAnimation)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeInterpolation)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeBlink)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapePeriodicBlink)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

ENTITY_BEHAVIOR_FACTORY* ENTITY_BEHAVIOR_FACTORY::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		ENTITY_BEHAVIOR_FACTORY constructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_BEHAVIOR_FACTORY::ENTITY_BEHAVIOR_FACTORY()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ENTITY_BEHAVIOR_FACTORY destructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_BEHAVIOR_FACTORY::~ENTITY_BEHAVIOR_FACTORY()
{
	// DO nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
ENTITY_BEHAVIOR_FACTORY *	ENTITY_BEHAVIOR_FACTORY::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new ENTITY_BEHAVIOR_FACTORY;
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_FACTORY::Suicide()
{
	if (_pInstance)
	{
		delete _pInstance;
	}
}

//-----------------------------------------------------------------------------
//	Name:		CreateBlendShapeAnimation
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIMATION *	ENTITY_BEHAVIOR_FACTORY::CreateBlendShapeAnimation(int					nNbrWeights,
																		   const COM_INSTANCE & BSPCInst) const
{
	return (new BLEND_SHAPE_ANIMATION(nNbrWeights, BSPCInst));
}

//-----------------------------------------------------------------------------
//	Name:		CreateBlendShapeBlink
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_BLINK *	ENTITY_BEHAVIOR_FACTORY::CreateBlendShapeBlink(int		nTargetBlend,
																   float	rTargetWeight,
																   float	rFrameDelay) const
{
	return (new BLEND_SHAPE_BLINK(nTargetBlend,
								  rTargetWeight,
								  rFrameDelay));
}

//-----------------------------------------------------------------------------
//	Name:		CreateBlendShapePeriodicBlink
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_PERIODIC_BLINK *	ENTITY_BEHAVIOR_FACTORY::CreateBlendShapePeriodicBlink(int		nTargetBlend,
																					   float	rTargetWeight,
																					   float	rFrameDelay,
																					   float	rFramePeriod,
																					   float	rRandom) const
{
	return (new BLEND_SHAPE_PERIODIC_BLINK(nTargetBlend,
										   rTargetWeight,
										   rFrameDelay,
										   rFramePeriod,
										   rRandom));
}

//-----------------------------------------------------------------------------
//	Name:		CreateBlendShapeInterpolation
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_INTERPOLATION *	ENTITY_BEHAVIOR_FACTORY::CreateBlendShapeInterpolation(int		nTargetBlend,
																				   float	rTargetWeight,
																				   float	rFrameDelay) const
{
	return (new BLEND_SHAPE_INTERPOLATION(nTargetBlend,
										  rTargetWeight,
										  rFrameDelay));
}

//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShapeAnimation
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIMATION*	ENTITY_BEHAVIOR_FACTORY::ApplyBlendShapeAnimation(const COM_INSTANCE & NodeInst,
																		  const COM_INSTANCE & BSPCInst,
																		  bool	bBlending,
																		  float rBlendingTime,
																		  float rSpeed,
																		  float	rStartFrame,
																		  float	rEndFrame,
																		  int	nNbrLoops,
																		  int	nBlendShapeID) const
{
	QDT_NOT_IMPLEMENTED();
	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShapeBlink
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_FACTORY::ApplyBlendShapeBlink(const COM_INSTANCE &	NodeInst,
													  int					nTargetBlend,
													  float					rTargetWeight,
													  float					rDuration, // in FPS
													  int					nBlendShapeID) const
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShapePeriodicBlink
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_FACTORY::ApplyBlendShapePeriodicBlink(const COM_INSTANCE &	NodeInst,
															  int					nTargetBlend,
															  float					rTargetWeight,
															  float					rDuration,			// in FPS
															  float					rPeriod,			// in FPS
															  float					rRandom,			// in FPS
															  int					nBlendShapeID) const
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		ApplyBlendShapeInterpolation
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_FACTORY::ApplyBlendShapeInterpolation(const COM_INSTANCE &	NodeInst,
															  int					nTargetBlend,
															  float					rTargetWeight,
															  float					rDuration,			// in FPS
															  int					nBlendShapeID) const
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		DeleteBlendShapeAnimation
//	Object:		
//	03-05-12:	ELE - Created
//-----------------------------------------------------------------------------
void	ENTITY_BEHAVIOR_FACTORY::DeleteBlendShapeAnimation(const COM_INSTANCE &		NodeInst,
														   BLEND_SHAPE_ANIMATION *	pBSAnim,
														   int						nBlendShapeID) const
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeValue
//	Object:		
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
float	ENTITY_BEHAVIOR_FACTORY::GetBlendShapeValue(const COM_INSTANCE &	NodeInst,
													int						nTargetBlend,
													int						nBlendShapeID) const
{
	QDT_NOT_IMPLEMENTED();
	return (0.0f);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
