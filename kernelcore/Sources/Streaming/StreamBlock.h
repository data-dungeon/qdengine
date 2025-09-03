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
//	CLASS:	STREAM_BLOCK
//	The STREAM_BLOCK class implements ...
//
//	03-05-19:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_STREAMING_STREAMBLOCK_H__
#define __KCORE_STREAMING_STREAMBLOCK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(MEM_STREAM)

namespace	QDT
{
	namespace KCORE
	{
		class	STREAM_BLOCK
		{
		public:
			
			STREAM_BLOCK();
			STREAM_BLOCK(int nID);
			STREAM_BLOCK(int nID, float rStartTime, float rEndTime, const char * pBuffer, int nSize);
			virtual ~STREAM_BLOCK();

			int			GetID();
			void		SetID(int nID);
			void		IncreaseReference();
			void		DecreaseReference();
			int			GetNbrReference();
			float		GetStartTime();
			float		GetEndTime();
			
			void			SetBufferPadding(int nOffset);
			const char  *	GetBuffer();
			int				GetBufferSize();
			virtual void	FillStream(MEM_STREAM * pMemStream, float rStartTime, float rEndTime, float rDuration, bool bFillWhithBlank, bool bFillAtBegin, bool bCompressed) = 0;

			virtual void	SetBlendingTime(float r);
			virtual float	GetLastBlendingTime() const;
			virtual float *	GetLastFrameWeights();

		private:

			int			_nID;
			int			_nNbrRef;
			float		_rStartTime;
			float		_rEndTime;

			const char *_pBuffer;
			int			_nSize;
		};
	}
}

#endif //__KCORE_STREAMING_STREAMBLOCK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
