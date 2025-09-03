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
//	The STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER class implements ...
//
//	03-06-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER_H__
#define __M3D_STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAMABLE_BLEND_SHAPE_ANIM)

#include INCL_3DENGINE(Behavior/Datas/StreamableBlendShapeAnim/StreamHandleBlendShapeAnim)
#include INCL_3DENGINE(Behavior/Datas/BlendShapeAnim/BlendShapeAnimWrapper)

namespace QDT
{
	namespace M3D
	{
		class	STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER : public BLEND_SHAPE_ANIM_WRAPPER_ABC
		{
		public:
			STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER();
			virtual							~STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER();

			virtual void					SetBlendShapeAnimInst(const COM_INSTANCE & BlendShapeAnimInst);
			virtual const COM_INSTANCE &	GetBlendShapeAnimInst() const;

			virtual unsigned int			GetNbFrames();
			virtual float					GetFPS();
			virtual bool					GetBSWeightsAt(float rCurrentFrame, float * prWeights, unsigned int WeightCount, bool bInterpolate);

			virtual STREAM_HANDLE_BLEND_SHAPE_ANIM *	GetStreamHandle();

		protected:

			void							InitStreamHandle();

			STREAMABLE_BLEND_SHAPE_ANIM *	GetStreamableBlendShapeAnim();

		private:

			COM_INSTANCE					_StreamableBlendShapeAnimInst;

			STREAM_HANDLE_BLEND_SHAPE_ANIM	_StreamHandle;
		};
	}
}

#endif // __M3D_STREAMABLE_BLEND_SHAPE_ANIM_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
