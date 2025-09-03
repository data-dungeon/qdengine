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
//	CLASS:	BLEND_SHAPE_PERIODIC_BLINK
//
//	01-12-17:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapePeriodicBlink.h"

#include	INCL_KMATH(Math/Math)
#include	INCL_3DENGINE(Behavior/Controllers/Entity Controllers/BlendShapeController)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_PERIODIC_BLINK constructor
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_PERIODIC_BLINK::BLEND_SHAPE_PERIODIC_BLINK(int		nTargetBlend, 
													   float	rTargetWeight, 
													   float	rDuration, 
													   float	rPeriod, 
													   float	rRandom)
:
_nTargetBlend	(nTargetBlend),
_rWeight		(0.0f),
_rDuration		(rDuration / 2.0f),
_rTargetWeight	(rTargetWeight),
_rPeriod		(rPeriod),
_rRandom		(rRandom),
_rNextBlink		(0.0f)
{
	// Do nothing	
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_PERIODIC_BLINK destructor
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_PERIODIC_BLINK::~BLEND_SHAPE_PERIODIC_BLINK()
{
	// Do  nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetTargetBlend
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE_PERIODIC_BLINK::GetTargetBlend() const
{
	return (_nTargetBlend);
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetBlend
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::SetTargetBlend(int	nTargetBlend)
{
	_nTargetBlend = nTargetBlend;
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetWeight
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::SetTargetWeight(float	rTargetWeight)
{
	_rTargetWeight = rTargetWeight;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrameDelay
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::SetDuration(float	rDuration)
{
	_rDuration = rDuration;
}

//-----------------------------------------------------------------------------
//	Name:		SetFramePeriod
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::SetPeriod(float	rPeriod)
{
	_rPeriod = rPeriod;
}

//-----------------------------------------------------------------------------
//	Name:		SetTimeRandom
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::SetRandom(float	rRandom)
{
	_rRandom = rRandom;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateData
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::UpdateData(float	rSpeedFactor)
{
	if (_rCurrentFrame > _rNextBlink + _rDuration)
	{
		ComputeNextBlink();
	}

	if ((_rCurrentFrame > _rNextBlink + _rDuration)
							||
		(_rCurrentFrame < _rNextBlink - _rDuration))
	{
		_rWeight = 0.0f;
	}
	else
	{
		if (_rCurrentFrame > _rNextBlink)
		{
			// Linear interpolation
			_rWeight = _rWeight + rSpeedFactor * (0.0f - _rWeight) 
										/ 
						(_rNextBlink + _rDuration - _rCurrentFrame + rSpeedFactor);
		}
		else	// _rCurrentFrame < _rNextBlink
		{
			// Linear interpolation
			_rWeight = _rWeight + rSpeedFactor * (_rTargetWeight - _rWeight) 
										/ 
						(_rNextBlink - _rCurrentFrame + rSpeedFactor);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateController
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::UpdateController(ENTITY_CONTROLLER_ABC *pController) const
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
bool	BLEND_SHAPE_PERIODIC_BLINK::IsBlendShapePeriodicBlink() const
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeNextBlink
//	Object:		
//	01-12-17:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_PERIODIC_BLINK::ComputeNextBlink()
{
	_rNextBlink += _rPeriod + MATH::Rand(1.0f) * _rRandom;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
