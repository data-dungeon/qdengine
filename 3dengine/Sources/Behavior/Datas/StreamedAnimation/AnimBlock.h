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
//	CLASS:	ANIM_BLOCK
//	The ANIM_BLOCK class implements ...
//
//	03-05-28:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_BLOCK_H__
#define __M3D_ANIM_BLOCK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_TRACK)

#include INCL_KCORE(Streaming/StreamBlock)

namespace QDT
{
	namespace M3D
	{
		class	ANIM_BLOCK : public STREAM_BLOCK
		{
		public:

			////////////////////////////////////////////
			ANIM_BLOCK( int nID );
			ANIM_BLOCK( int nID, float rStartTime, float rEndTime, const char *pBuffer, int nSize );
			~ANIM_BLOCK();
			////////////////////////////////////////////

			////////////////////////////////////////////
			_INLINE_	void		SetNbrTracks( int n );
			_INLINE_	int			GetNbrTracks()const;
			_INLINE_	void		SetTracks( STREAM_TRACK *pST );
			////////////////////////////////////////////

			////////////////////////////////////////////
			void				InitTracks( const char *pBuffer );
			STREAM_TRACK*		GetTrack( int nNum );
			////////////////////////////////////////////

			////////////////////////////////////////////
			MEM_STREAM *		GetBuffer();
			int					GetBufferSize();
			////////////////////////////////////////////

			////////////////////////////////////////////
			void	FillStream(MEM_STREAM * pMemStream, float rStartTime, float rEndTime, float rDuration, bool bFillWhithBlank, bool bFillAtBegin, bool bCompressed);
			////////////////////////////////////////////


		protected:

			////////////////////////////////////////////
			int				_nNbrTracks;
			STREAM_TRACK	*_pTracks;
			////////////////////////////////////////////

		private:

			////////////////////////////////////////////
			////////////////////////////////////////////
		};
	}
}

#ifndef _DEBUG
	#include "AnimBlock.inl"
#endif

#endif //__M3D_ANIM_BLOCK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
