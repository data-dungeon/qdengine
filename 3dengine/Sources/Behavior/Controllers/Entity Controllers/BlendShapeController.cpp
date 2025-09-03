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
//	CLASS:	BLEND_SHAPE_CONTROLLER
//
//	01-12-18:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapeController.h"

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/EntityBehaviorABC)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeInterpolation)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapeBlink)
#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/BlendShape/BlendShapePeriodicBlink)

#define QDT_INCLUDE_MEMORY
#include INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_CONTROLLER constructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_CONTROLLER::BLEND_SHAPE_CONTROLLER(int nNbrWeights)
:
_nNbrWeights		(nNbrWeights),
_prWeights			(NULL),
_bBlending			(false),
_rBlendingTime		(0),
_rBlendingTimeRef	(0),
_prBlendingWeights	(NULL)
{
	_prWeights = new float [_nNbrWeights];

	int nEachWeight;
	for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
	{
		_prWeights[nEachWeight] = 0.0f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_CONTROLLER destructor
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_CONTROLLER::~BLEND_SHAPE_CONTROLLER()
{
	delete [] _prWeights;
	_prWeights = NULL;

	delete [] _prBlendingWeights;
	_prBlendingWeights = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_CONTROLLER::Update(float	rSpeedFactor)
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR it	= _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itEnd	= _dlBehaviors.GetTail();
	int nEachWeight;

	for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
	{
		_prWeights[nEachWeight] = 0.0f;
	}

	for(; it != itEnd ; ++it)
	{
		(*it)->Update(rSpeedFactor);
		(*it)->UpdateController(this);
	}

	if	(_bBlending)
	{
		if (_rBlendingTime > 0)
		{
			int		nEachWeight;
			float	ri;

			ri = (_rBlendingTimeRef - _rBlendingTime) / _rBlendingTimeRef;

			for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
			{
				// Don't blend if weight was not used by the previous animation (so script can use it)
				if	(_prBlendingWeights[nEachWeight] > 0.0f)
				{
					_prWeights[nEachWeight] = (ri * _prWeights[nEachWeight]) + ((1.0f - ri) * _prBlendingWeights[nEachWeight]);
				}
			}

			_rBlendingTime -= rSpeedFactor;
		}
		else
		{
			_bBlending = false;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetWeights
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
float *	BLEND_SHAPE_CONTROLLER::GetWeights()
{
	return (_prWeights);
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendingWeights
//	Object:		
//	04-03-03:	ELE - Created
//-----------------------------------------------------------------------------
float *	BLEND_SHAPE_CONTROLLER::GetBlendingWeights()
{
	return (_prBlendingWeights);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrWeights
//	Object:		
//	01-12-18:	RMA - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE_CONTROLLER::GetNbrWeights()
{
	return (_nNbrWeights);
}

//-----------------------------------------------------------------------------
//	Name:		GetWeight
//	Object:		
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
float	BLEND_SHAPE_CONTROLLER::GetWeight(int	nTarget)
{
	QDT_ASSERT(nTarget < _nNbrWeights);

	return (_prWeights[nTarget]);
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeController
//	Object:		
//	01-12-19:	RMA - Created
//-----------------------------------------------------------------------------
bool	BLEND_SHAPE_CONTROLLER::IsBlendShapeController() const
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		GetInterpolation
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_INTERPOLATION *	BLEND_SHAPE_CONTROLLER::GetInterpolation(int nTargetBlend)
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itEachBehavior = _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itTailBehavior = _dlBehaviors.GetTail();
	BLEND_SHAPE_INTERPOLATION	*pBSInterpolation;

	for(; itEachBehavior != itTailBehavior ; ++itEachBehavior)
	{
		if ((*itEachBehavior)->IsBlendShapeInterpolation())
		{
			pBSInterpolation = (*itEachBehavior)->GetBlendShapeInterpolation();

			if (pBSInterpolation->GetTargetBlend() == nTargetBlend)
			{
				return (pBSInterpolation);
			}
		}
	}
	
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetBlink
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_BLINK *	BLEND_SHAPE_CONTROLLER::GetBlink(int	nTargetBlend)
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itEachBehavior = _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itTailBehavior = _dlBehaviors.GetTail();
	BLEND_SHAPE_BLINK	*pBSBlink;

	for(; itEachBehavior != itTailBehavior ; ++itEachBehavior)
	{
		if ((*itEachBehavior)->IsBlendShapeBlink())
		{
			pBSBlink = (*itEachBehavior)->GetBlendShapeBlink();

			if (pBSBlink->GetTargetBlend() == nTargetBlend)
			{
				return (pBSBlink);
			}
		}
	}
	
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetBlink
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_PERIODIC_BLINK *	BLEND_SHAPE_CONTROLLER::GetPeriodicBlink(int	nTargetBlend)
{
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itEachBehavior = _dlBehaviors.GetHead();
	QDT_DLIST<ENTITY_BEHAVIOR_ABC*>::ITERATOR itTailBehavior = _dlBehaviors.GetTail();
	BLEND_SHAPE_PERIODIC_BLINK	*pBSBlink;

	for(; itEachBehavior != itTailBehavior ; ++itEachBehavior)
	{
		if ((*itEachBehavior)->IsBlendShapePeriodicBlink())
		{
			pBSBlink = (*itEachBehavior)->GetBlendShapePeriodicBlink();

			if (pBSBlink->GetTargetBlend() == nTargetBlend)
			{
				return (pBSBlink);
			}
		}
	}
	
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		SetBlending
//	Object:		
//	04-03-03:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_CONTROLLER::SetBlending(bool	b,
											float	rBlendingTime,
											float *	pBlendingWeights)
{
	_bBlending = b;

	if	(_bBlending)
	{
		_rBlendingTime = rBlendingTime;
		_rBlendingTimeRef = rBlendingTime + 1;

		if	(_prBlendingWeights == NULL)
		{
			_prBlendingWeights = new float [_nNbrWeights];
		}

		memcpy(_prBlendingWeights, pBlendingWeights, _nNbrWeights * sizeof(float));
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
