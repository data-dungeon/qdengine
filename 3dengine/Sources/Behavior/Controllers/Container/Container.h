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
// CLASS: CONTAINER
//
//
//	01-01-19:	GHP - Created
//*****************************************************************************

//=============================================================================
// Name: CONTAINER
//	01-01-19:	GHP - Created
//=============================================================================

#ifndef __M3D_CONTAINER_H__
#define __M3D_CONTAINER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KCORE(Template/QDT_PAIR)
#include INCL_KCOM(CommunicatorType/ComInstance)
#include INCL_3DENGINE(Behavior/AnimGeometry/StrAnim)

DECL_3DENGINE(ANIM_NODE)
DECL_3DENGINE(ANIM_NODE_STRUCT)
DECL_3DENGINE(NODE)

namespace QDT
{
	namespace M3D
	{
		/** 
			[MBi - 2006.01.26] - 

				Act as an animation instance container for the node.
				A better name would be ANIMATION_CONTAINER.

		 */
		class	CONTAINER
		{
		public:
			struct	TARGET_STRUCT
			{
				COM_INSTANCE	_TargetAnimInst;
				STR_ANIM		_StrAnim;
				bool			_bIsActive;
			};

			struct	PART_ANIM_STRUCT
			{
				COM_INSTANCE	_PartAnimInst;
				STR_ANIM		_StrAnim;
				bool			_bIsActive;
			};
			
			CONTAINER();
			CONTAINER( const CONTAINER& C );
			virtual							~CONTAINER();
			void							Copy(const CONTAINER& C);
			CONTAINER&						operator=( const CONTAINER& C );
			
			_INLINE_    MATRIX &			GetMatrix();
			_INLINE_	const MATRIX &		GetMatrix() const;
			
			_INLINE_	NODE*				GetHolderNode() const;
						ANIM_NODE*			GetAnimNode();
						ANIM_NODE_STRUCT*	GetAnimNodeStruct();
						ANIM_NODE_STRUCT*	GetPartAnimStruct();
			_INLINE_	void				ResetTeleport();
			_INLINE_    bool				IsPlayingAnimation() const;
			
			_INLINE_	void				SetMatrix( MATRIX& Mat );
			_INLINE_	void				SetHolderNode(NODE *pN );
			_INLINE_	void				SetAnimNode( const COM_INSTANCE& Anim );
			_INLINE_	void				SetIndex( int nIndex );
			_INLINE_	int					GetIndex()const;
			_INLINE_	void				SetMainContainer( CONTAINER *pContainer );
			STR_ANIM*						GetStrAnim() { return &_StrAnim; }
			
			void							SetNbrChildren( int nNbrChildren );
			void							PlayTargetAnim( const COM_INSTANCE& TargetAnim );
			void							ActivateTargetAnim( bool bVal );
			void							PlayPartAnim( const COM_INSTANCE& PartAnim );
			void							ActivatePartAnim( bool bVal );
			void							ProcessPartAnimActivation( bool bVal, bool bBlend );
			void							Update( float rDelay );
			static bool						HasOrCreatesContainer(NODE * const	pN);
				
			void							DeleteAllAnimation();
			void							SetSpeed( COM_INSTANCE& Anim, float rSpeed )  ;	
			bool							IsEmpty() ; 
			bool							IsPlaying();
			
			void							ShiftAnim();
			void							OnAnimRemoved( ANIM_NODE *pAN );
			void							SetActiveAnim( const COM_INSTANCE& Anim );
			const	COM_INSTANCE&			GetActiveAnim()const;
			void							GetAnimMatrix( int nIndex, MATRIX& Mat );
			void							ResetAnimMatrices();
			void							SaveBlendingStart( ANIM_NODE_STRUCT *pANS );
			void							SaveBlendingStop();
			void							PrepareStrBeforeUpdate(ANIM_NODE_STRUCT *pAN );
			void							UpdateFromMainContainer();	

			void							DeleteMainAnimConnection();
			void							DeleteTargetConnection();
			void							DeleteAnimMixConnection();

		protected:
			
			void							AfterUpdate();
			void							UpdateTarget( float rDelay );
			void							UpdatePartAnim( float rDelay );
			void							ShiftPartAnim();

		private:
			
			MATRIX						_LMatrix;
			TARGET_STRUCT				_Target;
			PART_ANIM_STRUCT			_PartAnim;
			COM_INSTANCE				_FirstAnimNode;
			COM_INSTANCE				_LastFirstAnimNode;
			
			NODE						*_pHolderNode;
			bool						_bMustTeleport;
			bool						_bNewAnimAdded;
		
			STR_ANIM					_StrAnim;
			STR_ANIM					_SaveStrAnim;
			COM_INSTANCE				_ActiveAnim;		
			QUATERNION_VECTOR			_Teleport;			//< Teleport is used to do motion extraction. (a little bit hacky...) 
			int							_nIndex;
			CONTAINER					*_pMainContainer;
			QUATERNION_VECTOR			*_pSaveQVArray;
			bool						_bSaveBlending;
			int							_nNbrChildren;		//< This seems to be the number of bones in the hierarchy == size of _pSaveQVArray
			
		};
	}
}

#ifndef _DEBUG
	#include "Container.inl"
#endif

#endif // __M3D_CONTAINER_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
