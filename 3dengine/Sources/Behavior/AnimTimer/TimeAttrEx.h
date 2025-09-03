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
//	CLASS:	TIME_ATTR_EX
//	The TIME_ATTR_EX class implements ...
//
//	03-06-11:	CVI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TIME_ATTR_EX_H__
#define __M3D_TIME_ATTR_EX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Tools/QdtFlags)
#include INCL_3DENGINE(Behavior/AnimTimer/BaseTimerAttr)

namespace QDT
{
	namespace M3D
	{
		class	TIME_ATTR_EX
		{
		public:

			enum	UPDATE_MODE
			{
				PREV_SEQ_BLEND = 0,
				OWN,
				NEXT_SEQ_BLEND,
			};

			enum	TIMER_STATUS
			{
				NO_ONE					= 0,
				STATUS_PREV_SEQ_BLEND	= 1,
				STATUS_OWN_PROCESS		= 2,
				STATUS_NEXT_SEQ_BLEND	= 4,
			};
			
			TIME_ATTR_EX();
			TIME_ATTR_EX( const TIME_ATTR_EX& Timer );
			~TIME_ATTR_EX();
			void	Copy( const TIME_ATTR_EX& Timer );
			const TIME_ATTR_EX&	operator=( const TIME_ATTR_EX& Timer );

			void					Init();
			bool					UpdateTime( float& rDelay, bool& bBlendPrevSeq );
			
			_INLINE_	float				GetCurTime()const;
			_INLINE_	float				GetStartTime()const;
			_INLINE_	float				GetEndTime()const;	
			_INLINE_	float				GetSpeed()const;	
			_INLINE_	bool				HasLooped()const;
			_INLINE_	bool				IsFinished()const;
			_INLINE_	int					GetNbrLoopsDone()const;	
			_INLINE_	bool				IsManualIncrement()const;
			_INLINE_	void				SetFrameIncrement( float rVal );
			_INLINE_	void				SetCurTime(float rVal );
			_INLINE_	void				SetStartTime( float rVal );
			_INLINE_	void				SetEndTime( float rVal );
			_INLINE_	void				SetNbrRepeat( int nVal );
			_INLINE_	void				SetSpeed( float rVal );
			_INLINE_	void				SetPaused( bool bVal );	
			_INLINE_	void				SetLastTime( float rVal );
			_INLINE_	void				SetOffsetStart( float rVal );	
			_INLINE_	void				SetManualIncrement( bool b = true );
			_INLINE_	float				GetBlendingTimeLong()const;
			_INLINE_	float				GetTimeToGo()const;
			_INLINE_	void				ActiveBlending();
			_INLINE_	void				SetNbrBlendingFrames( float rVal );	
			_INLINE_	float				GetCurrentDeltaTime()const;
			_INLINE_	void				SetInfinite( bool bVal );
			_INLINE_	void				SetLoop( bool bVal );
			_INLINE_	float				GetCurrentBlendingFrame()const;
			_INLINE_	bool				IsBlendingActived()const;
			bool							BlendingIsFinished()const { return ( _BlendingTimer.IsFinished() ); }
			
		protected:

			_INLINE_	float				GetFrameIncrement()const;
			
			void		UpdateInfinite( float& rDelay );
			void		BasicUpdate( float& rDelay , bool& bBlend );
			void		UpdateOwn( float& rDelay );
			void		UpdateTimer( float& rDelay );
			void		UpdateTimerPos( float& rDelay );
			void		UpdateTimerNeg( float& rDelay );

			
			BASE_TIMER_ATTR		_BlendingTimer;
			float				_rStartTime;
			float				_rEndTime;
			float				_rSpeed;
			float				_rCurrentTime;
			float				_rLastTime;
			float				_rTimeToGo;
			float				_rOffsetStart;
			int					_nNbrLoops;
			int					_nNbrLoopsDone;
			bool				_bHasLoop;
			bool				_bIsManualTimer;
			bool				_bIsInfinite;
			bool				_bIsLoop;
			bool				_bIsFinished;
			bool				_bIsPaused;
			bool				_bBlendingForceActived;
			float				_rFrameInc;
			float				_rCurrentDeltaTime;
			
		};
	}
}

#ifndef _DEBUG
	#include "TimeAttrEx.inl"
#endif

#endif // __M3D_TIME_ATTR_EX_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
