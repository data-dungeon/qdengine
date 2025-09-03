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
//	CLASS:	ITRACK_WRAPPER
//	The ITRACK_WRAPPER class implements ...
//
//	01-10-17:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ITRACK_WRAPPER_H__
#define __M3D_ITRACK_WRAPPER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(QUATERNION)
DECL_KMATH(VECTOR)
DECL_KMATH(QUATERNION_VECTOR)
DECL_3DENGINE(ANIM_PC)

#include INCL_KCORE(Template/QDT_DList)

namespace QDT
{
	namespace M3D
	{
		class	ITRACK_WRAPPER
		{
		public:

			ITRACK_WRAPPER() {}
			virtual ~ITRACK_WRAPPER() {}

			virtual	void			GetBaseRotation( QUATERNION& Q )const = 0;
			virtual	void			GetRotation( Float32 rTime, QUATERNION& Q ) = 0;
			virtual	void			GetBaseTranslation( VECTOR& V )const = 0;
			virtual	void			GetTranslation( Float32 rTime, VECTOR& V) = 0;
			virtual	int				GetNbrRotKeys() const										= 0;
			virtual	int				GetNbrTransKeys()const										= 0;
			virtual	int				GetTrackNbrFrames()const									= 0;
			virtual	int				GetTrackIndex()const										= 0;
			virtual	int				GetAnimTops(const Float32 rFrame, QDT_DLIST<int>& vEvents )	= 0;
			virtual	const ANIM_PC*	GetAnimPc()const											= 0;
			virtual	void			GetRotTrans( Float32 rTime, QUATERNION_VECTOR & QV)			= 0;
		};
	}
}

#endif // __M3D_ITRACK_WRAPPER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
