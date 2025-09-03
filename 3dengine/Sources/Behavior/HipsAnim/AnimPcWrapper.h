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
//	CLASS:	ANIM_PC_WRAPPER
//	The ANIM_PC_WRAPPER class implements ...
//
//	01-10-18:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_PC_WRAPPER_H__
#define __M3D_ANIM_PC_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(ANIM_PC)
DECL_3DENGINE(TRACK_DATA_WRAPPER)

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_3DENGINE(Behavior/AnimWrapper/IAnimWrapper)

namespace QDT
{
	namespace M3D
	{
		class	ANIM_PC_WRAPPER : public IANIM_WRAPPER
		{
		public:

			ANIM_PC_WRAPPER();
			explicit			ANIM_PC_WRAPPER(const ANIM_PC_WRAPPER & C);
			virtual				~ANIM_PC_WRAPPER();
			ANIM_PC_WRAPPER&	operator=(const ANIM_PC_WRAPPER & C);
			void				Copy(const ANIM_PC_WRAPPER & C);
			
			virtual int					GetNbrFrames();
			virtual int					GetNbrTracks();
			virtual void				SetAnim( COM_INSTANCE& Anim );
			virtual COM_INSTANCE&		GetAnim();
			virtual ITRACK_WRAPPER*		GetTrack( int nNum );
			virtual void				ReadRotTrans( float rTime, QUATERNION_VECTOR *pQV );
			virtual void				ReleaseAnim();
			virtual void				UpdateCommunicator();
			

		protected:

			void					CreateAllTracks();
			ANIM_PC*				GetAnimPc();
			
			TRACK_DATA_WRAPPER		*_pTracks;
			int						_nNbrFrames;
			int						_nNbrTracks;

		private:
			
			COM_INSTANCE		_AnimPc;
			
		};
	}
}

#endif // #define __M3D_ANIM_PC_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
