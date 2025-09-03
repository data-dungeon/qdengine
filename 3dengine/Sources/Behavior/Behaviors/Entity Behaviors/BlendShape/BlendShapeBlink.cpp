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
//	CLASS:	BLEND_SHAPE_BLINK
//
//	01-12-17:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapeBlink.h"

#include	INCL_3DENGINE(Behavior/Controllers/Entity Controllers/BlendShapeController)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_BLINK constructor
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_BLINK::BLEND_SHAPE_BLINK(int	nTargetBlend, 
									 float	rTargetWeight, 
									 float	rDuration)
:
_nTargetBlend	(nTargetBlend),
_rWeight		(0.0f),
_rDuration		(rDuration / 2.0f),
_rTargetWeight	(rTargetWeight)
{
	// Do nothing	
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_BLINK destructor
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_BLINK::~BLEND_SHAPE_BLINK()
{
	// Do  nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetTargetBlend
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE_BLINK::GetTargetBlend() const
{
	return (_nTargetBlend);
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetBlend
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_BLINK::SetTargetBlend(int	nTargetBlend)
{
	_nTargetBlend = nTargetBlend;
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetWeight
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_BLINK::SetTargetWeight(float	rTargetWeight)
{
	_rTargetWeight = rTargetWeight;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrameDelay
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_BLINK::SetDuration(float	rDuration)
{
	_rDuration = rDuration;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateData
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_BLINK::UpdateData(float	rSpeedFactor)
{
	if (_rCurrentFrame > (_rDuration * 2))
	{
		_rWeight = 0.0f;
	}
	else
	{
		if (_rCurrentFrame > _rDuration)
		{
			// Linear interpolation
			_rWeight = _rWeight + rSpeedFactor * (0.0f - _rWeight) 
										/ 
						((_rDuration * 2) - _rCurrentFrame + rSpeedFactor);
		}
		else	// _rCurrentFrame < _rNextBlink
		{
			// Linear interpolation
			_rWeight = _rWeight + rSpeedFactor * (_rTargetWeight - _rWeight) 
										/ 
						(_rDuration - _rCurrentFrame + rSpeedFactor);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateController
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_BLINK::UpdateController(ENTITY_CONTROLLER_ABC *	pController) const
{
	BLEND_SHAPE_CONTROLLER *pBSController = pController->GetBlendShapeController();

	float *prWeights = pBSController->GetWeights();

	if	(_nTargetBlend < pBSController->GetNbrWeights())
	{
		prWeights[_nTargetBlend] += _rWeight;
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeInterpolation
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
bool	BLEND_SHAPE_BLINK::IsBlendShapeBlink() const
{
	return (true);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
