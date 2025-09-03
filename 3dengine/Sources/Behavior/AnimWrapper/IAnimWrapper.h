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
//	CLASS:	IANIM_WRAPPER
//	The IANIM_WRAPPER class implements ...
//
//	01-10-18:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_IANIM_WRAPPER_H__
#define __M3D_IANIM_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(ITRACK_WRAPPER)
DECL_KCOM(COM_INSTANCE)
DECL_KMATH(VECTOR)
DECL_KMATH(QUATERNION_VECTOR)

namespace QDT
{
	namespace M3D
	{
		class	IANIM_WRAPPER
		{
		public:
			
			IANIM_WRAPPER() {}
			virtual ~IANIM_WRAPPER() {}
			
			virtual	int					GetNbrFrames()						= 0;
			virtual	int					GetNbrTracks()						= 0;
			virtual	void				SetAnim( COM_INSTANCE& Anim )		= 0;
			virtual	COM_INSTANCE&		GetAnim()							= 0;
			virtual ITRACK_WRAPPER*		GetTrack( int nNum )				= 0;
			virtual void				ReadRotTrans( float rTime, QUATERNION_VECTOR *pQV ) = 0;
			virtual	void				ReleaseAnim()						= 0;
			virtual void				UpdateCommunicator()				= 0;
			
		};
	}
}

#endif //__M3D_IANIM_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
