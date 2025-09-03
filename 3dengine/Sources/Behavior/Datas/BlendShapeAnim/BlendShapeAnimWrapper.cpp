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
//	CLASS:	BLEND_SHAPE_ANIM_WRAPPER
//
//	03-06-05:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"BlendShapeAnimWrapper.h"
#include	INCL_3DENGINE(Behavior/Datas/BlendShapeAnim/BlendShapePC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_ANIM_WRAPPER constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIM_WRAPPER::BLEND_SHAPE_ANIM_WRAPPER()
:
_BlendShapePcInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		BLEND_SHAPE_ANIM_WRAPPER destructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_ANIM_WRAPPER::~BLEND_SHAPE_ANIM_WRAPPER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendShapeAnimInst
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	BLEND_SHAPE_ANIM_WRAPPER::SetBlendShapeAnimInst(const COM_INSTANCE &	BlendShapeAnimInst)
{
	_BlendShapePcInst = BlendShapeAnimInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeAnimInst
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	BLEND_SHAPE_ANIM_WRAPPER::GetBlendShapeAnimInst() const
{
	return	(_BlendShapePcInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbFrames
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
unsigned int	BLEND_SHAPE_ANIM_WRAPPER::GetNbFrames()
{
	return	(GetBlendShapePc()->GetNbFrames());
}

//-----------------------------------------------------------------------------
//	Name:		GetFPS
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
float	BLEND_SHAPE_ANIM_WRAPPER::GetFPS()
{
	return	(GetBlendShapePc()->GetFPS());
}

//-----------------------------------------------------------------------------
//	Name:		GetBSWeightsAt
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
bool	BLEND_SHAPE_ANIM_WRAPPER::GetBSWeightsAt(float			rCurrentFrame,
												 float *		prWeights,
												 unsigned int	WeightCount,
												 bool			bInterpolate)
{
	return	(GetBlendShapePc()->GetBSWeightsAt(rCurrentFrame, prWeights, WeightCount, bInterpolate));
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapePc
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
BLEND_SHAPE_PC *	BLEND_SHAPE_ANIM_WRAPPER::GetBlendShapePc()
{
	if	(_BlendShapePcInst.HasHandle())
	{
		return	(static_cast<BLEND_SHAPE_PC *>(_BlendShapePcInst.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamHandle
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_BLEND_SHAPE_ANIM *	BLEND_SHAPE_ANIM_WRAPPER::GetStreamHandle()
{
	return	(NULL);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
