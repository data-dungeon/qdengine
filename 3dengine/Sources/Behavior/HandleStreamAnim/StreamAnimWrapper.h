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
//	CLASS:	STREAM_ANIM_WRAPPER
//	The STREAM_ANIM_WRAPPER class implements ...
//
//	03-05-30:	CVI - Created
//*****************************************************************************


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STREAM_ANIM_WRAPPER_H__
#define __M3D_STREAM_ANIM_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_TRACK)
DECL_3DENGINE(STREAM_TRACK_WRAPPER)

#include INCL_3DENGINE(Behavior/HandleStreamAnim/HandleStreamAnim)
#include INCL_3DENGINE(Behavior/AnimWrapper/IAnimWrapper)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_ANIM_WRAPPER : public IANIM_WRAPPER
		{
		public:

			STREAM_ANIM_WRAPPER();
			virtual ~STREAM_ANIM_WRAPPER();
	
			virtual int					GetNbrFrames();
			virtual int					GetNbrTracks();

			virtual void				SetAnim( COM_INSTANCE& Anim );
			virtual COM_INSTANCE&		GetAnim();
					
			virtual ITRACK_WRAPPER*		GetTrack( int nNum );
			virtual void				ReadRotTrans( float rTime, QUATERNION_VECTOR *pQV );
			virtual void				ReleaseAnim();
			virtual void				UpdateCommunicator();
			
			STREAM_TRACK*				GetStreamTrack( int nNum, float rFrame );	
			STREAM_HANDLE*				GetStreamHandle();
			void						ResetTracks();
			
		protected:

			void						CreateAllTracks();
			void						InitHandleStream();
		
			STREAM_TRACK_WRAPPER		*_pTracks;
			int							_nNbrTracks;
			COM_INSTANCE				_Anim;
			int							_nNbrFrames;
			STREAM_HANDLE_ANIM			_HandleStream;

		};
	}
}

#endif // __M3D_STREAM_ANIM_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
