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
//	CLASS:	BLEND_SHAPE_INTERPOLATION
//
//	01-12-12:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapeInterpolation.h"

#include	INCL_3DENGINE(Behavior/Controllers/Entity Controllers/BlendShapeController)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_INTERPOLATION constructor
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_INTERPOLATION::BLEND_SHAPE_INTERPOLATION(int	nTargetBlend, 
													 float	rTargetWeight,
													 float	rDuration)
:
_nTargetBlend	(nTargetBlend),
_rWeight		(0.0f),
_rDuration		(rDuration),	
_rTargetWeight	(rTargetWeight)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_INTERPOLATION destructor
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_INTERPOLATION::~BLEND_SHAPE_INTERPOLATION()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_INTERPOLATION::UpdateData(float	rSpeedFactor)
{
	if (_rCurrentFrame >= _rDuration)
	{
		_rWeight = _rTargetWeight;
	}
	else
	{
		// Linear interpolation
		_rWeight = _rWeight + rSpeedFactor * (_rTargetWeight - _rWeight) 
															/ 
										(_rDuration - _rCurrentFrame + rSpeedFactor);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeInterpolation
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	BLEND_SHAPE_INTERPOLATION::IsBlendShapeInterpolation() const
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateController
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_INTERPOLATION::UpdateController(ENTITY_CONTROLLER_ABC *	pController) const
{
	BLEND_SHAPE_CONTROLLER *pBSController = pController->GetBlendShapeController();

	float *prWeights = pBSController->GetWeights();

	if	(_nTargetBlend < pBSController->GetNbrWeights())
	{
		prWeights[_nTargetBlend] += _rWeight;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetBlend
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_INTERPOLATION::SetTargetBlend(int	nTargetBlend)
{
	_nTargetBlend = nTargetBlend;
}

//-----------------------------------------------------------------------------
//	Name:		SetTargetWeight
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_INTERPOLATION::SetTargetWeight(float	rTargetWeight)
{
	_rTargetWeight = rTargetWeight;
}

//-----------------------------------------------------------------------------
//	Name:		SetFrameDelay
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_INTERPOLATION::SetDuration(float	rDuration)
{
	_rDuration = rDuration;
}

//-----------------------------------------------------------------------------
//	Name:		GetTargetBlend
//	Object:		
//	01-12-13:	RMA - Created
//-----------------------------------------------------------------------------
int	BLEND_SHAPE_INTERPOLATION::GetTargetBlend() const
{
	return (_nTargetBlend);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
