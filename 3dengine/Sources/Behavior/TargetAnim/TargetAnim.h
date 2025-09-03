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
//	CLASS:	TARGET_ANIM
//	The TARGET_ANIM class implements ...
//
//	01-12-18:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TARGET_ANIM_H__
#define __M3D_TARGET_ANIM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/AnimNode/AnimNode)

namespace QDT
{
	namespace M3D 
	{
		class	TARGET_ANIM : public ANIM_NODE
		{
		public:

			TARGET_ANIM();
			TARGET_ANIM( CLASS_DESCRIPTOR* const pCD );
			explicit			TARGET_ANIM(const TARGET_ANIM & C);
			virtual				~TARGET_ANIM();

			TARGET_ANIM&		operator=(const TARGET_ANIM & C);
			void				Copy(const TARGET_ANIM & C);

			static void		COMInit(CLASS_DESCRIPTOR * const pCD);
			static			COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static			COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			const QDT_STRING&	GetClassGUID() const;
			void				SetClassGUID(const QDT_STRING& sClassGUID);


		private:

			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
			
			QDT_STRING						_sClassGUID;
		};
	}
}

#ifndef _DEBUG
	#include "TargetAnim.inl"
#endif

#endif // __M3D_TARGET_ANIM_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
