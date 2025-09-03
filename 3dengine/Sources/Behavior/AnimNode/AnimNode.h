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
//	CLASS:	ANIM_NODE
//	The ANIM_NODE class implements ...
//
//	01-10-11:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_NODE_H__
#define __M3D_ANIM_NODE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(ANIM_NODE_STRUCT)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		class	ANIM_NODE : public COMMUNICATOR
		{
		public:

			enum
			{
				SET_ANIM_WEIGHT = CMD_COM_USER_START,
				SET_ROTATION_WEIGHT,
				SET_TRANSLATION_WEIGHT,
				SET_START_TIME,
				SET_END_TIME,
				SET_SPEED,
				ADD_SEQUENCE,
				INSERT_SEQUENCE,
				SET_LOOP,
				SET_NBR_LOOPS,
				SET_PING_PONG,
				SET_ABSOLUTE,
				SET_TELEPORT,
				SET_BLENDING,
				SET_PREEMPTIVE,
				REMOVE_SEQUENCE,
				POP_ANIM,
				SCALE_TRANSLATION,
				IS_PLAYING,
				PAUSE,
				RESUME,
				STOP,
				GET_ANIM_CHILD,
				GET_CHILD_INDEX,
				SET_OFFSET_START,
				SET_MANUAL_TIMER,
				INCREMENT_TIMER,
				IS_MANUAL_TIMER,
				SET_CUR_TIME,
				ENABLE_DELETATION,
				ACTIVE_COUNT_DOWN,
				SET_NBR_BLENDING_FRAMES,
				CMD_ANIM_NODE_LAST,
			};
			
			ANIM_NODE();
			ANIM_NODE( CLASS_DESCRIPTOR* const pCD );
			explicit							ANIM_NODE(const ANIM_NODE & C);
			virtual								~ANIM_NODE();
			ANIM_NODE&							operator=(const ANIM_NODE & C);
			void								Copy(const ANIM_NODE & C);
			
			static ANIM_NODE_STRUCT*			GetStructFromInstance( const COM_INSTANCE& CI );
			_INLINE_ ANIM_NODE_STRUCT*			GetAnimStruct() const;
			
			Float32								GetCurrentFrame() const;
			void								SetSpeed( Float32 rSpeed );

			//	COMMUNICATOR
			static void		COMInit(CLASS_DESCRIPTOR * const pCD);
			static			COMMUNICATOR_DESCRIPTOR*	GetGenericCommunicatorDescriptor();
			static			COMMUNICATOR_DESCRIPTOR*	CreateGenericCommunicatorDescriptor();
			const QDT_STRING&	GetClassGUID() const;
			void				SetClassGUID(const QDT_STRING& sClassGUID);

			///	COMMANDS
			static void	EM_SetAnimWeight(void * const pO, void * const pR, void * const pP);
			static void	EM_SetRotationWeight(void * const pO, void * const pR, void * const pP);
			static void	EM_SetTranslationWeight(void * const pO, void * const pR, void * const pP);
			static void	EM_SetStartTime(void * const pO, void * const pR, void * const pP);
			static void	EM_SetEndTime(void * const pO, void * const pR, void * const pP);
			static void	EM_SetSpeed(void * const pO, void * const pR, void * const pP);
			static void	EM_AddSequence(void * const pO, void * const pR, void * const pP);
			static void	EM_InsertSequence(void * const pO, void * const pR, void * const pP);
			static void	EM_SetLoop(void * const pO, void * const pR, void * const pP);
			static void	EM_SetNbrLoops(void * const pO, void * const pR, void * const pP);
			static void	EM_SetPingPong(void * const pO, void * const pR, void * const pP);
			static void	EM_SetAbsolute(void * const pO, void * const pR, void * const pP);
			static void	EM_SetTeleport(void * const pO, void * const pR, void * const pP);
			static void	EM_SetBlending(void * const pO, void * const pR, void * const pP);
			static void	EM_SetPreemptive(void * const pO, void * const pR, void * const pP);
			static void	EM_RemoveSequence(void * const pO, void * const pR, void * const pP);
			static void	EM_PopAnim(void * const pO, void * const pR, void * const pP);
			static void	EM_ScaleTranslation(void * const pO, void * const pR, void * const pP);
			static void	EM_GetBlender(void * const pO, void * const pR, void * const pP);
			static void	EM_IsPlaying(void * const pO, void * const pR, void * const pP);
			static void	EM_Pause(void * const pO, void * const pR, void * const pP);
			static void	EM_Resume(void * const pO, void * const pR, void * const pP);
			static void	EM_Stop(void * const pO, void * const pR, void * const pP);
			static void	EM_GetAnimChild(void * const pO, void * const pR, void * const pP);
			static void	EM_GetChildIndex(void * const pO, void * const pR, void * const pP);
			static void	EM_SetOffsetStart(void * const pO, void * const pR, void * const pP);
			static void	EM_SetManualTimer(void * const pO, void * const pR, void * const pP);
			static void	EM_IncrementTimer(void * const pO, void * const pR, void * const pP);
			static void	EM_IsManualTimer(void * const pO, void * const pR, void * const pP);
			static void	EM_SetCurTime(void * const pO, void * const pR, void * const pP);
			static void	EM_EnableDeletation(void * const pO, void * const pR, void * const pP);
			static void	EM_ActiveCountDown(void * const pO, void * const pR, void * const pP);
			static void	EM_SetNbrBlendingFrames(void * const pO, void * const pR, void * const pP);


		protected:

			_INLINE_ void						SetAnimStruct( ANIM_NODE_STRUCT *pS );
			
			ANIM_NODE_STRUCT		*_pStruct;

		private:
			
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
			
			QDT_STRING	_sClassGUID;
			
		};
	}
}

#ifndef _DEBUG
	#include "AnimNode.inl"
#endif

#endif // __M3D_ANIM_NODE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
