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
//	CLASS:	TRAGET_ANIM_STRUCT
//	The TRAGET_ANIM_STRUCT class implements ...
//
//	01-12-18:	cvi - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_TARGET_ANIM_STRUCT_H__
#define __M3D_TARGET_ANIM_STRUCT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/TargetAnim/TargetTrack)
#include INCL_3DENGINE(Behavior/AnimNode/AnimNodeStruct)

namespace QDT
{
	namespace M3D 
	{
		class	TARGET_ANIM_STRUCT : public ANIM_NODE_STRUCT
		{
		public:
#define MAX_TARGET_TRACKS	5
			enum TARGET_ENUM
			{
				NO_TYPE		= 0,
				TYPE_DIR	= 1<<0,
				TYPE_POS	= 1<<1,
			};


			TARGET_ANIM_STRUCT();
			virtual ~TARGET_ANIM_STRUCT();

			virtual void		StoreRotTrans( float rTime );
			void				ApplyTarget( QUATERNION_VECTOR *pQV );
			void				InitTargetTracks( const COM_INSTANCE& SklInst);
			void				SetNbrTargetTracks( int nNbrTarget )	{ _nNbrTargetTracks = nNbrTarget; }
			
			void				ActivateTarget( bool bVal, bool bBlend = true );
			void				SetTrackBoneType( int nNum, int nType );
			void				SetMaxSpeed( int nNum, const VECTOR& V );
			void				SetRange( int nNum, const VECTOR& V );
			void				SetAcceleration( int nNum, const VECTOR& V );
			void				SetInductionWeight( int nNum, float rW );
			void				SetInducted( int nNum, bool bVal );
			void				SetTargetPoint( const VECTOR& P );
			
			void				SetCurrentDelay( float rDelay );
			
			void				LockTargetNode( const COM_INSTANCE& NodeInst, int nIndex = -1 );
			void				UnlockTargetNode();
			void				InitFromPrevious( TARGET_ANIM_STRUCT	*pPrevTS );

			const MATRIX&		GetGlobalMatrix( int nNumObj );

			const MATRIX&		GetContainerMatrix( int nNumObj );
			void				AddInductionToParent( int nParentIndex, const QUATERNION& Q );
			TARGET_TRACK*		GetTargetTrack( int nNum );

		protected:

			void				ResetTrackParameters();
			void				ActiveBlendingTimer( bool bToGo );

			void				ResetInduction();
			void				UpdateTargetNodePosition();
			void				UpdateTargetSklNodePosition();

			TARGET_TRACK			_TargetTrack[MAX_TARGET_TRACKS];
			BASE_TIMER_ATTR			_BlendingTimer;
			VECTOR					_TargetPoint;
			COM_INSTANCE			_TargetNodeInst;
			int						_nNbrTargetTracks;
			float					_rCurrentDelay;
			int						_nTargetIndex;
			bool					_bBlendTarget;
			bool					_bTargetIsActive;
			bool					_bGoingToTarget;

		};
	}
}

#ifndef _DEBUG
	#include "TargetAnimStruct.inl"
#endif

#endif // __M3D_TARGET_ANIM_STRUCT_H__


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
