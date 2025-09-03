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
//	CLASS:	STREAM_HANDLE_BLEND_SHAPE_ANIM
//	The STREAM_HANDLE_BLEND_SHAPE_ANIM class implements ...
//
//	03-06-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_HANDLE_BLEND_SHAPE_ANIM_H__
#define __M3D_STREAM_HANDLE_BLEND_SHAPE_ANIM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Streaming/StreamHandle)
#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_HANDLE_BLEND_SHAPE_ANIM  : public STREAM_HANDLE
		{
		public:
			STREAM_HANDLE_BLEND_SHAPE_ANIM();
			~STREAM_HANDLE_BLEND_SHAPE_ANIM();

			void			SetBlendShapeAnimInst(const COM_INSTANCE & BlendShapeAnimInst);

			virtual void	Play();
			virtual void	Stop();
			virtual void	Pause();
			virtual void	Resume();
			virtual bool	IsPlaying();

			bool			GetBSWeightsAt(float rCurrentFrame, float * prWeights, unsigned int WeightCount, bool bInterpolate);

		private:

			bool			GetBSWeights(float rCurrentFrame, float * prWeights, unsigned int WeightCount, bool bInterpolate);
			bool			GetManagedBSWeights(float * prWeights, unsigned int WeightCount, bool bInterpolate);

			COM_INSTANCE	_StreamableBlendShapeAnimInst;
		};
	}
}

#endif // __M3D_STREAM_HANDLE_BLEND_SHAPE_ANIM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
