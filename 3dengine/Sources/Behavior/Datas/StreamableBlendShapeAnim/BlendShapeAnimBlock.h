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
//	CLASS:	BLEND_SHAPE_ANIM_BLOCK
//	The BLEND_SHAPE_ANIM_BLOCK class implements ...
//
//	03-06-05:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BLEND_SHAPE_ANIM_BLOCK_H__
#define __M3D_BLEND_SHAPE_ANIM_BLOCK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Streaming/StreamBlock)

namespace QDT
{
	namespace M3D
	{
		class	BLEND_SHAPE_ANIM_BLOCK : public STREAM_BLOCK
		{
		public:
			BLEND_SHAPE_ANIM_BLOCK(int nID, float rStartTime, float rEndTime, const char * pBuffer, int nSize);
			virtual					~BLEND_SHAPE_ANIM_BLOCK();

			float					GetFPS()const			{ return (_rFPS); }
			int						GetNbrWeights()const	{ return (_nNbrWeights); }	
			float *					GetWeights()const		{ return (_pWeights); }

			void					SetFPS( float rVal )	{ _rFPS = rVal; }
			void					SetNbrWeights( int n )	{ _nNbrWeights = n; }

			void					InitFromStream(const char * pBuffer);

			virtual MEM_STREAM *	GetBuffer();
			virtual int				GetBufferSize();
			virtual void			FillStream(MEM_STREAM * pMemStream, float rStartTime, float rEndTime, float rDuration, bool bFillWhithBlank, bool bFillAtBegin, bool bCompressed);

			virtual void			SetBlendingTime(float r);
			virtual float			GetLastBlendingTime() const;
			virtual float *			GetLastFrameWeights();

		protected:

			float					_rFPS;
			int						_nNbrWeights;
			float					*_pWeights;

			float					_rBlendingTime;
			float					*_pLastFrameWeights;
		};
	}
}

#endif // __M3D_BLEND_SHAPE_ANIM_BLOCK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
