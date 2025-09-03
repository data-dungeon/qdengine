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
//	CLASS:	STR_ANIM
//	The STR_ANIM class implements ...
//
//	03-06-11:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STR_ANIM_H__
#define __M3D_STR_ANIM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/AnimGeometry/StrAnimBase)

namespace QDT
{
	namespace M3D
	{
		class	STR_ANIM
		{
		public:
			
			STR_ANIM();
			explicit		STR_ANIM(const STR_ANIM & C);
			~STR_ANIM();
			STR_ANIM&		operator=(const STR_ANIM & C);
			void			Copy(const STR_ANIM & C);

			_INLINE_	bool	MustNotifyCollideForTeleport()const;
			_INLINE_	void	AddNotifyCollideForTeleport( bool bVal );
			_INLINE_	void	ResetCollideNotification();

		public:

			STR_ANIM_BASE			_Own;
			QUATERNION_VECTOR		*_pChildrenOutput;
			int						_nNbrChildren;
			bool					_bNotifyCollideForTeleport;

		};
	}
}

#ifndef _DEBUG
	#include "StrAnim.inl"
#endif

#endif // __M3D_STR_ANIM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
