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
//	CLASS:	BLEND_SHAPE_ANIM_WRAPPER_ABC
//	The BLEND_SHAPE_ANIM_WRAPPER_ABC class implements ...
//
//	03-06-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BLEND_SHAPE_ANIM_WRAPPER_ABC_H__
#define __M3D_BLEND_SHAPE_ANIM_WRAPPER_ABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_HANDLE_BLEND_SHAPE_ANIM)

#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	BLEND_SHAPE_ANIM_WRAPPER_ABC
		{
		public:
			BLEND_SHAPE_ANIM_WRAPPER_ABC();
			virtual							~BLEND_SHAPE_ANIM_WRAPPER_ABC();

			virtual void					SetBlendShapeAnimInst(const COM_INSTANCE & BlendShapeAnimInst) = 0;
			virtual const COM_INSTANCE &	GetBlendShapeAnimInst() const = 0;

			virtual unsigned int			GetNbFrames() = 0;
			virtual float					GetFPS() = 0;
			virtual bool					GetBSWeightsAt(float rCurrentFrame, float * prWeights, unsigned int WeightCount, bool bInterpolate) = 0;

			virtual STREAM_HANDLE_BLEND_SHAPE_ANIM *	GetStreamHandle() = 0;
		};
	}
}

#endif // __M3D_BLEND_SHAPE_ANIM_WRAPPER_ABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
