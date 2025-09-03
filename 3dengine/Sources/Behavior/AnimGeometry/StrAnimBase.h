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
//	CLASS:	STR_ANIM_BASE
//	The STR_ANIM_BASE class implements ...
//
//	03-06-11:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_STR_ANIM_BASE_H__
#define __M3D_STR_ANIM_BASE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/AnimGeometry/BiQuatVect)

namespace QDT
{
	namespace M3D
	{
		class	STR_ANIM_BASE
		{
		public:

			STR_ANIM_BASE();
			STR_ANIM_BASE(const STR_ANIM_BASE & C);
			~STR_ANIM_BASE();
			STR_ANIM_BASE&			operator=(const STR_ANIM_BASE & C);
			void					Copy(const STR_ANIM_BASE & C);

			_INLINE_ STR_ANIM_BASE		operator *	(const STR_ANIM_BASE &SAB) const;		
			_INLINE_ STR_ANIM_BASE&		operator *=	(const STR_ANIM_BASE &SAB) ;		
			void						Blend(const STR_ANIM_BASE &SAB, float rCoeff );
			void						Reset();

		public:

			BI_QUAT_VECT			_AbsRel;
			QUATERNION_VECTOR		_Teleport;
			bool					_bAbsolute;
			bool					_bTeleport;
		};
	}
}

#ifndef _DEBUG
	#include "StrAnimBase.inl"
#endif

#endif // #define __M3D_STR_ANIM_BASE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
