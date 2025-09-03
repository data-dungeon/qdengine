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
//	CLASS:	STREAM_HANDLE
//	The STREAM_HANDLE class implements ...
//
//	03-05-15:	AMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_STREAMING_STREAMHANDLE_H__
#define __KCORE_STREAMING_STREAMHANDLE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(STREAM_BLOCK)
#include INCL_KCORE(Template/QDT_Vector)

namespace	QDT
{
	namespace KCORE
	{
		class	STREAM_HANDLE
		{
		public:

			STREAM_HANDLE();
			virtual ~STREAM_HANDLE();

			void	SetCurrentPosition(int nPosition);
			void	IncCurrentPosition();
			int		GetCurrentPosition();
			bool	IsPaused();
			void	SetPause(bool bPause);

			virtual void	Play() = 0;
			virtual void	Stop() = 0;
			virtual void	Pause() = 0;
			virtual void	Resume() = 0;
			virtual bool	IsPlaying() = 0;

			void			SetCurrentBlock(STREAM_BLOCK * pCurrentBlock);
			STREAM_BLOCK *	GetCurrentBlock();

			float			GetTheCurrentTime();
			void			SetCurrentTime(float rCurrentTime);

			bool			GetIsManaged();
			void			SetIsManaged(bool bIsManaged);

			void			PushStreamHandle(STREAM_HANDLE * pStreamHandle);
			int				GetNbrStreamHandle();
			STREAM_HANDLE *	GetStreamHandle(int nIndex);

		protected:

			float	_rCurrentTime;

		private:

			STREAM_BLOCK *		_pCurrentBlock;

			int		_nCurrentID;
			bool	_bPaused;
			bool	_bIsManaged;

			QDT_VECTOR<STREAM_HANDLE *>		_vStreamHandle;
		};
	}
}

#endif //__KCORE_STREAMING_STREAMHANDLE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
