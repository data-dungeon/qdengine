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
//	CLASS:	STREAMABLE_BLEND_SHAPE_ANIM_WRITER
//	The STREAMABLE_BLEND_SHAPE_ANIM_WRITER class implements ...
//
//	03-06-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAMABLE_BLEND_SHAPE_ANIM_WRITER_H__
#define __M3D_STREAMABLE_BLEND_SHAPE_ANIM_WRITER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCOM(Streaming/StreamablePartitionWriter)

namespace QDT
{
	namespace M3D
	{
		class	STREAMABLE_BLEND_SHAPE_ANIM_WRITER : public STREAMABLE_PARTITION_WRITER
		{
		public:
			STREAMABLE_BLEND_SHAPE_ANIM_WRITER(float nDuration, float nTimeDurationPerPartition, const COM_INSTANCE& Streamable, const COM_INSTANCE& Offset);
			~STREAMABLE_BLEND_SHAPE_ANIM_WRITER();

			void					SetBlendShapeAnim(const COM_INSTANCE & Anim);
			const COM_INSTANCE &	GetBlendShapeAnim() const;

			virtual float	GetBlendingTime() const;
			virtual void	SetBlendingTime(float r);
			virtual void	SetBlendingWeights(float * prWeights);

			virtual void	WriteSpecificData(MEM_STREAM * pS, int nNumPartition, float rDuration );
			virtual	void	WriteSpecificHeaderInfo(MEM_STREAM * pS, int nNumPartition);
			virtual	void	WriterSpecificInit();

			void	FillStreamWithBlank(MEM_STREAM * pMemStream, float rDuration);

		protected:

			void			InitFromStreamable();
			void			InitFromBlendShapePc();

			COM_INSTANCE	_BlendShapePc;
			int				_nNbrWeights;
			float			_rFPS;

			float			_rBlendingTime;
			float			*_prBlendingWeights;

		private:
		};
	}
}

#endif // __M3D_STREAMABLE_BLEND_SHAPE_ANIM_WRITER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
