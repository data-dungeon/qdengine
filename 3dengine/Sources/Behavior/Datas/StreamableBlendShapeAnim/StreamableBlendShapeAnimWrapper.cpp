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
//	CLASS:	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER
//
//	03-06-05:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	"StreamableBlendShapeAnimWrapper.h"

#include	INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamableBlendShapeAnim)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER constructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER()
:
_StreamableBlendShapeAnimInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER destructor
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::~STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetBlendShapeAnimInst
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::SetBlendShapeAnimInst(const COM_INSTANCE &	BlendShapeAnimInst)
{
	_StreamableBlendShapeAnimInst = BlendShapeAnimInst;

	InitStreamHandle();
}

//-----------------------------------------------------------------------------
//	Name:		GetBlendShapeAnimInst
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::GetBlendShapeAnimInst() const
{
	return	(_StreamableBlendShapeAnimInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbFrames
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
unsigned int	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::GetNbFrames()
{
	return	(GetStreamableBlendShapeAnim()->GetNbrFrames());
}

//-----------------------------------------------------------------------------
//	Name:		GetFPS
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
float	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::GetFPS()
{
	return	(GetStreamableBlendShapeAnim()->GetFPS());
}

//-----------------------------------------------------------------------------
//	Name:		GetBSWeightsAt
//	Object:		
//	03-06-05:	ELE - Created
//-----------------------------------------------------------------------------
bool	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::GetBSWeightsAt(float			rCurrentFrame,
															float *			prWeights,
															unsigned int	WeightCount,
															bool			bInterpolate)
{
	return	(_StreamHandle.GetBSWeightsAt(rCurrentFrame, prWeights, WeightCount, bInterpolate));
}

//-----------------------------------------------------------------------------
//	Name:		InitStreamHandle
//	Object:		
//	03-06-11:	ELE - Created
//-----------------------------------------------------------------------------
void	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::InitStreamHandle()
{
	_StreamHandle.SetBlendShapeAnimInst(_StreamableBlendShapeAnimInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamableBlendShapeAnim
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
STREAMABLE_BLEND_SHAPE_ANIM *	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::GetStreamableBlendShapeAnim()
{
	if	(_StreamableBlendShapeAnimInst.HasHandle())
	{
		return	(static_cast<STREAMABLE_BLEND_SHAPE_ANIM *>(_StreamableBlendShapeAnimInst.GetCommunicator()));
	}

	return	(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamHandle
//	Object:		
//	03-06-13:	ELE - Created
//-----------------------------------------------------------------------------
STREAM_HANDLE_BLEND_SHAPE_ANIM *	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER::GetStreamHandle()
{
	return	(&_StreamHandle);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
