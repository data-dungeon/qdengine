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
//	CLASS:	HIPS_ANIM
//	The HIPS_ANIM class implements ...
//
//	01-10-18:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_HIPS_ANIM_H__
#define __M3D_HIPS_ANIM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/AnimNode/AnimNode)

namespace QDT
{
	namespace M3D
	{
		class	HIPS_ANIM : public ANIM_NODE
		{
		public:
			///////////////////////////////////////////////////////////////
			enum
			{
				CMD_SKL_INSTANCE	= CMD_ANIM_NODE_LAST, 
				CMD_GET_DATA_ANIM,
				CMD_SKL_INSTANCE_STREAM,
				CMD_LAST,
			};
			///////////////////////////////////////////////////////////////
			HIPS_ANIM();
			HIPS_ANIM( CLASS_DESCRIPTOR* const pCD );
			explicit			HIPS_ANIM(const HIPS_ANIM & C);
			virtual				~HIPS_ANIM();
			HIPS_ANIM&			operator=(const HIPS_ANIM & C);
			void				Copy(const HIPS_ANIM & C);
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			//	COMMUNICATOR
			///////////////////////////////////////////////////////////////
			static void		COMInit(CLASS_DESCRIPTOR * const pCD);
			static			COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static			COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			const QDT_STRING&	GetClassGUID() const;
			void				SetClassGUID(const QDT_STRING& sClassGUID);
			///////////////////////////////////////////////////////////////


		protected:

		private:
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
			///////////////////////////////////////////////////////////////

			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////
			//	COMMANDS
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////
			static void	EM_Instanciate(void * const pO, void * const pR, void * const pP);
			static void	EM_InstanciateStream(void * const pO, void * const pR, void * const pP);
			static void	EM_GetDataAnim(void * const pO, void * const pR, void * const pP);
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////
			QDT_STRING						_sClassGUID;
			///////////////////////////////////////////////////////////////
		};
	}
}

#endif //__M3D_HIPS_ANIM_H__

#ifndef _DEBUG
	#include "HipsAnim.inl"
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
