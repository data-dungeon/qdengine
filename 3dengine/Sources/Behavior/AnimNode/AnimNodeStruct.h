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
//	CLASS:	ANIM_NODE_STRUCT
//	The ANIM_NODE_STRUCT class implements ...
//
//	01-10-11:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ANIM_NODE_STRUCT_H__
#define __M3D_ANIM_NODE_STRUCT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(SCALE_ATTR)
DECL_3DENGINE(ANIM_LINK)
DECL_3DENGINE(NODE)
DECL_3DENGINE(STR_ANIM)

#include INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include INCL_KMATH(Math/Geometry/QuaternionVector/QuaternionVector)
#include INCL_3DENGINE(Behavior/AnimNode/ScaleAttr)
#include INCL_3DENGINE(Behavior/AnimNode/AnimStatus)
#include INCL_3DENGINE(Behavior/AnimNode/Animlink)
#include INCL_3DENGINE(Behavior/AnimTimer/TimeAttrEx)

namespace QDT
{
	namespace M3D
	{
		class	ANIM_NODE_STRUCT
		{
		public:

			enum BLENDING_STATE
			{
				NO_ACTIVATED = 0,
				BLENDING_TO,
				ACTIVATED,
				BLENDING_FROM,
			};
			
			ANIM_NODE_STRUCT();
			explicit				ANIM_NODE_STRUCT(const ANIM_NODE_STRUCT & C);
			virtual					~ANIM_NODE_STRUCT();
			ANIM_NODE_STRUCT&				operator=(const ANIM_NODE_STRUCT & C);
			void					Copy(const ANIM_NODE_STRUCT & C);
			
			void					Init();
			void					InitBeforeStart( ANIM_NODE *pPrev );
			void					InitBeforeUpdateStart( ANIM_NODE *pPrev );
			bool					IsFinished();
			void					SetFinished( bool bVal );

			virtual	void			StoreRotTrans( float rTime );
			virtual void			ReadRotation( float rTime, QUATERNION& Quat );
			virtual void			ReadTranslation( float rTime, VECTOR& Vect );
			virtual	void			ReadRotTrans( float rTime );
			virtual	void			InitAbsoluteTransfo();
			
			void					PauseAnim();
			void					Resume();
			void					Stop();
			void					RecStop();

			void					EnableDeletation( bool b = true );
			void					ActiveCountDown( float rVal = 10.0f );
			void					SetNbrBlendingFrames( float rVal );

			void					SetPartBlendingStatus( int nStatus );
			int						GetPartBlendingStatus()const;
			float					GetPartBlendCoeff()const;
			void					SetPartBlendCoeff( float rVal );

			_INLINE_ bool			IsTeleport()const;
			_INLINE_ int			GetNbrChildren()const;
			
			_INLINE_ float			GetNbrBlendingFrames()const;
			
			_INLINE_ void			SetChildIndex( int nIndex  );
			_INLINE_ int			GetChildIndex()const;
			
			_INLINE_ const	TIME_ATTR_EX&			GetTimeAttr()const;
			
			_INLINE_ const	ANIM_STATUS&			GetAnimStatus()const;
			_INLINE_ const	ANIM_LINK&				GetAnimLink()const;
			
			 NODE*					GetHolderNode()const;	
			_INLINE_ ANIM_LINK*		GetAnimLink();
			
			
			_INLINE_ bool			IsManualTimer()const;
			
			_INLINE_ float			GetCurrentFrame()const;
			_INLINE_ void			SetPartAnim( bool bVal );
			
			
			_INLINE_ void			SetTimeAttr( const TIME_ATTR_EX& TimeAttr );
			
			
			_INLINE_ void			SetContainer( CONTAINER *pC );
			_INLINE_ void			SetEndTime( float rVal );
			void					SetLoop( bool bVal );
			
			void					SetOffsetStart( float rVal );
			_INLINE_ void			SetManualTimer( bool b = true );
			_INLINE_ void			SetManualIncrement( float rVal );
			
			_INLINE_ ANIM_NODE*		GetAnimNode();
			_INLINE_ void			SetAnimNode( ANIM_NODE *pAN );

			_INLINE_ void			SetCurTime( float rVal );

			_INLINE_ bool			IsActive()const;
			void					Activate( bool b );

			_INLINE_ void			SetPartStartIndex( int nIndex );
			_INLINE_ int			GetPartStartIndex() const;

			_INLINE_ void			SetPartEndTrack( int nIndex );
			_INLINE_ int			GetPartEndTrack() const;

		
			void					AddSequence( const COM_INSTANCE& CI );	
			void					SendToDeleteList();	
			bool					IsPlayingAnimRec( const COM_INSTANCE& CI )const ;
			
			void					SetAnimWeight( float rWeight );
			void					SetRotationWeight( float rWeight );
			void					SetTranslationWeight( float rWeight );
			void					SetStartTime( float rStart );
			void					SetSpeed( float rSpeed );
			void					InsertSequence( const COM_INSTANCE& CI );
			void					SetNbrLoops( int n );
			void					SetPingPong( bool bVal = true );
			void					SetAbsolute( bool bVal = true );
			void					SetTeleport( bool bVal = true );
			void					SetBlending( bool bVal	= true );
			void					SetPreemptive( bool bVal = true );
			void					RemoveSequence( const COM_INSTANCE& CI );
			
			void					PopAnim( const COM_INSTANCE& CI );
			void					ScaleTranslation( float rSX, float rSY, float rSZ );
			
			_INLINE_ void			SetAnimEndBlending( bool bVal );
			
			void					OnSequenceAdded( ANIM_NODE *pSeqAdded );
			void					OnRemovedAsNextSequence();
			
			bool					IsBlending()const;
			
			void					Process( float& rDelay, STR_ANIM *pStr );
			void					ApplyPartAnim( STR_ANIM *pStrPart, STR_ANIM *pStrDst );
			void					ActivePartAnim( bool bVal, bool bBlend );
			void					ActivePartBlending( bool bToGo );
			void					InitPartAnim( const COM_INSTANCE& SklInst );
			
			
		protected:

			_INLINE_ bool			HasBlending()const;
			void					ActiveNextSeq( bool b );
			void					OnNextPartAnimActivated( ANIM_NODE_STRUCT *pNext );
			void					ForceTerminate( bool bDeleteSeq );

			void					ApplyPartFirstTrack( STR_ANIM* pStrPart, STR_ANIM* pStrSrc, float rCoeff, bool bHasBlending );

			
			void					UpdateLastQV();
			void					UpdateAbsolute();
			void					UpdateCollideStatus( STR_ANIM *pStr );
				
			void					InitChildren( int nNbrChildren );
			
			void					ProcessTimer( float& rDelay );
			void					UpdatePartAnimTimer( float rDelay );
			void					AnimComputation();
			void					ProcessBlending( STR_ANIM *pSTR );
			void					ProcessNoBlending( STR_ANIM *pSTR );
			
			void					ExecTerminate( bool bDeleteSeq );
			void					ForceTerminateAllSeq();
			void					IncludeLoopsInTeleport( QUATERNION_VECTOR& QV );
			
			void					InitFirstFrame();
			virtual void			ExecDelete();
			
			_INLINE_		bool	IsFirstUpdate()const;
			_INLINE_		void	SetFirstUpdateSatus( bool bVal );
			
			QUATERNION_VECTOR		_LastQV;
			QUATERNION_VECTOR		_CurrentQV;
			QUATERNION_VECTOR		_Absolute;
			QUATERNION_VECTOR		_Teleport;

			
			TIME_ATTR_EX			_TimeAttr;
			SCALE_ATTR				_ScaleAttr;
			ANIM_STATUS				_AnimStatus;
			ANIM_LINK				_AnimLink;
		
			QUATERNION_VECTOR		*_pChildrenOwn;
			QUATERNION_VECTOR		*_pChildrenOutput;
			BASE_TIMER_ATTR			_PartBlendTimer;
			int						_nPartStartIndex;
			int						_nPartNbrTracks;
			
			int						_nPartEndIndex;
			int						_nNbrChildren;
			int						_nChildIndex;
			BLENDING_STATE			_PartBlending;
			ANIM_NODE				*_pAnimNode;
			bool					_bAnimDeleted;
			bool					_bFinished;
			bool					_bIsActive;
			
			bool					_bBlendingWithPrevSeq;
			bool					_bIsBlending;

			bool					_bBlendAtAnimEnd;
			bool					_bIsPartAnim;
			bool					_bFirstUpdate;

		};
	}
}

#ifndef _DEBUG
	#include "AnimNodeStruct.inl"
#endif

#endif // __M3D_ANIM_NODE_STRUCT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

