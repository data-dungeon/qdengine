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
//	CLASS:	STREAM_TRACK_WRAPPER
//	The STREAM_TRACK_WRAPPER class implements ...
//
//	03-05-30:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_TRACK_WRAPPER_H__
#define __M3D_STREAM_TRACK_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_TRACK_HEADER)
DECL_3DENGINE(STREAM_ANIM_WRAPPER)

#include INCL_3DENGINE(Behavior/AnimWrapper/ITrackWrapper)
#include INCL_3DENGINE(Behavior/Datas/TrackIndexes)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_TRACK_WRAPPER : public ITRACK_WRAPPER
		{
		public:

			STREAM_TRACK_WRAPPER();
			virtual ~STREAM_TRACK_WRAPPER();
			
			virtual void			GetBaseRotation( QUATERNION& Q )const;
			virtual void			GetRotation( float rTime, QUATERNION& Q );
			virtual void			GetRotTrans( float rTime, QUATERNION_VECTOR & QV );
			virtual void			GetBaseTranslation( VECTOR& V )const;
			virtual void			GetTranslation( float rTime, VECTOR& V );
			virtual int				GetNbrRotKeys()const;
			virtual int				GetNbrTransKeys()const;
			virtual int				GetTrackNbrFrames()const;
			virtual int				GetTrackIndex()const;
			virtual int				GetAnimTops(const float rFrame, QDT_DLIST<int>& vEvents );
			virtual const ANIM_PC*	GetAnimPc()const;
			
			void					SetTrackIndex( int nTrackIndex );
			void					SetTrackHeader( STREAM_TRACK_HEADER *pT );
			void					SetAnimWrapper( STREAM_ANIM_WRAPPER *pSAW );
			const TRACK_INDEXES&	GetTrackIndexes()const;
			void					ResetTrackIndex() { _TrackIndex.Reset(); }

		protected:

			int							_nTrackIndex;
			STREAM_TRACK_HEADER			*_pTrack;
			TRACK_INDEXES				_TrackIndex;
			STREAM_ANIM_WRAPPER			*_pAnimWrapper;
		};
	}
}

#endif // __M3D_STREAM_TRACK_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
