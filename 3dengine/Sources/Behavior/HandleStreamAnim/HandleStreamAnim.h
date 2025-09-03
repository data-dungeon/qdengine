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
//	CLASS:	STREAM_HANDLE_ANIM
//	The STREAM_HANDLE_ANIM class implements ...
//
//	03-05-30:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_HANDLE_STREAM_ANIM_H__
#define __M3D_HANDLE_STREAM_ANIM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_ANIM_WRAPPER)
DECL_3DENGINE(ANIM_BLOCK)
DECL_3DENGINE(STREAM_TRACK)

#include INCL_KCOM(CommunicatorType/ComInstance)
#include INCL_KCORE(Streaming/StreamHandle)

namespace QDT
{
	namespace M3D
	{
		class	STREAM_HANDLE_ANIM : public STREAM_HANDLE
		{
		public:

			STREAM_HANDLE_ANIM();
			virtual ~STREAM_HANDLE_ANIM();
			
			STREAM_TRACK*		GetStreamTrack( int nNum, float rFrame );
			void				PrepareBlock( float rFrame, STREAM_ANIM_WRAPPER *pSAW  );
			void				SetAnim( const COM_INSTANCE& CI );
			
			/** 
				[MBi - 2006.01.31] - 

					Theses methods are defined as pure virtual by
					STREAM_HANDLE, but are obviously not needed 
					by STREAM_HANDLE_ANIM. Maybe should they be 
					removed from STREAM_HANDLE ?

			*/
			virtual void				Play();			//< Do not use.
			virtual void				Stop();			//< Do not use.
			virtual void				Pause();		//< Do not use.
			virtual void				Resume();		//< Do not use.
			virtual bool				IsPlaying();	//< Do not use.

		protected:

			ANIM_BLOCK*			GetCurrentAnimBlock();
			COM_INSTANCE		_Anim;

		};
	}
}

#endif // __M3D_HANDLE_STREAM_ANIM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
