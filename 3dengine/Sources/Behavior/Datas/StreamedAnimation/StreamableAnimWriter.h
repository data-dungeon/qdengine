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
//	CLASS:	STREAMABLE_ANIM_WRITER
//	The STREAMABLE_ANIM_WRITER class implements ...
//
//	03-05-28:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAMABLE_ANIM_WRITER_H__
#define __M3D_STREAMABLE_ANIM_WRITER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_TRACK)
DECL_3DENGINE(TRACK_DATA)
DECL_3DENGINE(ANIM_BLOCK)
DECL_3DENGINE(STREAM_TRACK_HEADER)

#include INCL_KCOM(Streaming/StreamablePartitionWriter)

namespace QDT
{
	namespace M3D
	{
		class	STREAMABLE_ANIM_WRITER : public STREAMABLE_PARTITION_WRITER
		{
		public:

			STREAMABLE_ANIM_WRITER(float rDuration, float rTimeDurationPerPartition, const COM_INSTANCE& Streamable, const COM_INSTANCE& Offset );
			virtual ~STREAMABLE_ANIM_WRITER();
			
			void					SetAnim( const COM_INSTANCE& Anim );
			const COM_INSTANCE&		GetAnim()const;
			void					AddStreamTrack( STREAM_TRACK *pST );
			STREAM_TRACK*			GetStreamTrack( int nNum );
			void					ResetTracks();
			float					GetFPS();
			
			//	overided methods from mother class
			virtual void	WriteSpecificData(MEM_STREAM * pS, int nNumPartition, float rDuration);
			virtual void	WriteSpecificHeaderInfo(MEM_STREAM * pS, int nNumPartition);
			virtual void	WriteFromBlocks(MEM_STREAM * pS, int nNumPartition, float rDuration);
			virtual void	WriterSpecificInit();
			
			void	FillStreamWithBlank(MEM_STREAM * pMemStream, float rDuration);

		protected:

			//	From animpc
			void				InitFromAnimPc();
			void				InitTrackHeader();
			void				InitAllTrackHeader();
			void				BuildAllTracks( float	rStartTime, float rEndTime );
			void				WriteAllTracks( MEM_STREAM *pS );
			void				BuildSingleTrack( int nNum, float rS, float rE );
			int					FillQuat( TRACK_DATA* pT, TRACK_DATA& T, float rS, float rE );
			int					FillTrans(  TRACK_DATA* pT, TRACK_DATA& T, float rS, float rE );
			int					FillAnimTop(  TRACK_DATA* pT, TRACK_DATA& T, float rS, float rE );
			void				FillQuatBuffer( STREAM_TRACK *pST, TRACK_DATA& T, int nNbrQuatKeys );
			void				FillTransBuffer( STREAM_TRACK *pST , TRACK_DATA& T, int nNbrTransKeys );
			
			TRACK_DATA*			GetTrackData( int nNum, float rS, float rE );
			
			//	From stream animation
			void				InitFromStreamAnimation();
			void				CopyTrackHeader();
			
			COM_INSTANCE				_AnimPc;
			ANIM_BLOCK*					_pCurrentBlock;
			int							_nNbrTracks;
			int							_nNbrFrames;
			STREAM_TRACK_HEADER*		_pTrackHeader;
			QDT_VECTOR<STREAM_TRACK*>	_vCurrentTrack;
			
		};
	}
}

#endif // __M3D_STREAMABLE_ANIM_WRITER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
