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
//	CLASS:	BLEND_SHAPE_ANIMATION
//
//	01-12-12:	RMA - Created
//*****************************************************************************

#include "Root.h"
#include "BlendShapeAnimation.h"

#include INCL_3DENGINE(Behavior/Datas/BlendShapeAnim/BlendShapeAnimWrapper)
#include INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnimWrapper)
#include INCL_3DENGINE(Behavior/Controllers/Entity Controllers/BlendShapeController)
#include INCL_3DENGINE(M3D/Moteur3DModule)
#define	 QDT_INCLUDE_MEMORY
#include INCL_KCORE(Includes/Includes)


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_ANIMATION constructor
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIMATION::BLEND_SHAPE_ANIMATION(int					nNbrWeights, 
											 const COM_INSTANCE &	BSPCInst)
:
_nNbrWeights			(nNbrWeights),
_prWeights				(NULL),
_pBlendShapeAnimWrapper	(NULL),
_bManaged				(false),
_bInterpolation			(true),
_bBlending				(false),
_rBlendingTime			(0),
_prBlendingWeights		(NULL)
{
	_prWeights = new float [_nNbrWeights];

	int nEachWeight;
	for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
	{
		_prWeights[nEachWeight] = 0.0f;
	}

	switch	(BSPCInst.GetType())
	{
	default:
		QDT_FAIL();
		break;

	case	MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_BLEND_SHAPE_PC:
		_pBlendShapeAnimWrapper = new BLEND_SHAPE_ANIM_WRAPPER;
		break;

	case	MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_STREAMABLE_BLEND_SHAPE_ANIM:
		_pBlendShapeAnimWrapper = new STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER;
		break;
	}

	_pBlendShapeAnimWrapper->SetBlendShapeAnimInst(BSPCInst);

	_rEndFrame = _pBlendShapeAnimWrapper->GetNbFrames();
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_ANIMATION destructor
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIMATION::~BLEND_SHAPE_ANIMATION()
{
	if (_prWeights) delete [] _prWeights;
	_prWeights	= NULL;

	if (_prBlendingWeights) delete [] _prBlendingWeights;
	_prBlendingWeights	= NULL;

	delete _pBlendShapeAnimWrapper;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIMATION::UpdateData(float	rSpeedFactor)
{
	QDT_ASSERT(_pBlendShapeAnimWrapper);

	if	(_bManaged)
	{
		_pBlendShapeAnimWrapper->GetBSWeightsAt(_rCurrentFrame, _prWeights, _nNbrWeights, _bInterpolation);
	}
	else
	{
		if (_rCurrentFrame > _rEndFrame - 1)
		{
			if	(_nNbrLoops >= 0)
			{
				if	(_nNbrLoops)
				{
					Reset();
					_bSaveBlending = _bBlending;
					_bBlending = false;
					_nNbrLoops--;
				}
				else
				{
					_bIsFinished = true;
					_bBlending = _bSaveBlending;
				}
			}
			else
			{
				Reset();
				_bBlending = false;
			}
		}
		else
		{
			_pBlendShapeAnimWrapper->GetBSWeightsAt(_rCurrentFrame, _prWeights, _nNbrWeights, _bInterpolation);
		}
	}

	if	(_bBlending)
	{
		if (_rCurrentFrame < _rBlendingTime)
		{
			int		nEachWeight;
			float	ri;

			ri = (_rBlendingTime - _rCurrentFrame) / _rBlendingTime;

			for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
			{
				// Don't blend if weight is not used by the animation (so script can use it)
				if	(_prWeights[nEachWeight] > 0.0f)
				{
					_prWeights[nEachWeight] = ((1.0f - ri) * _prWeights[nEachWeight]) + (ri * _prBlendingWeights[nEachWeight]);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateController
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIMATION::UpdateController(ENTITY_CONTROLLER_ABC *	pController) const
{
	BLEND_SHAPE_CONTROLLER *pBSController = pController->GetBlendShapeController();

	if	(!_bIsFinished)
	{
		float nNbrControllerWeights = pBSController->GetNbrWeights();
		float *prWeights = pBSController->GetWeights();

		int nEachWeight;

		for (nEachWeight = 0 ; nEachWeight < _nNbrWeights ; ++nEachWeight)
		{
			// Animation is priority over others blending effects (blink, script, ...)
			if	(_prWeights[nEachWeight] > 0.0f)
			{
				if	(nEachWeight < nNbrControllerWeights)
				{
					prWeights[nEachWeight] = _prWeights[nEachWeight];
				}
			}
		}

		return;
	}

	pBSController->SetBlending(_bBlending, _rBlendingTime, _prWeights);
}

//-----------------------------------------------------------------------------
//	Name:		IsBlendShapeAnimation
//	Object:		
//	01-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	BLEND_SHAPE_ANIMATION::IsBlendShapeAnimation() const
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamHandle
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_BLEND_SHAPE_ANIM *	BLEND_SHAPE_ANIMATION::GetStreamHandle()
{
	return	(_pBlendShapeAnimWrapper->GetStreamHandle());
}

//-----------------------------------------------------------------------------
//	Name:		SetStartFrame
//	Object:		
//	04-10-14:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIMATION::SetStartFrame(float rStartFrame)
{
	_rStartFrame = rStartFrame;

	if	( (_rStartFrame <= 0.0f) ||
		  (_rStartFrame > _pBlendShapeAnimWrapper->GetNbFrames()) )
	{
		_rStartFrame = 0.0f;
	}
	
	_rCurrentFrame = _rStartFrame;
}

//-----------------------------------------------------------------------------
//	Name:		SetEndFrame
//	Object:		
//	04-10-14:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIMATION::SetEndFrame(float rEndFrame)	
{
	_rEndFrame = rEndFrame;

	if	( (_rEndFrame <= 0.0f) ||
		  (_rEndFrame > _pBlendShapeAnimWrapper->GetNbFrames()) )
	{
		_rEndFrame = _pBlendShapeAnimWrapper->GetNbFrames();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBlending
//	Object:		
//	04-03-03:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIMATION::SetBlending(bool		b,
										   float	rBlendingTime,
										   float	*pBlendingWeights)
{
	_bBlending = b;
	_rBlendingTime = rBlendingTime;

	if	(_prBlendingWeights == NULL)
	{
		_prBlendingWeights = new float [_nNbrWeights];
	}

	memcpy(_prBlendingWeights, pBlendingWeights, _nNbrWeights * sizeof(float));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
