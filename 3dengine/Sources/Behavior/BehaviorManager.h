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
//	CLASS:	CLOCK
//	The CLOCK class implements ...
//
//	02-02-27:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BEHAVIOR_MANAGER_H__
#define __M3D_BEHAVIOR_MANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(VECTOR)
DECL_KMATH(MATRIX)
DECL_KCOM(SINGLETON_DESCRIPTOR)
DECL_3DENGINE(CONTAINER)
DECL_3DENGINE(NODE)
DECL_3DENGINE(HIPS_ANIM_STRUCT)
DECL_3DENGINE(HIPS_ANIM)
DECL_3DENGINE(ANIM_STATUS)
DECL_3DENGINE(ANIM_NODE_STRUCT)
DECL_3DENGINE(TIME_ATTR_EX)
DECL_3DENGINE(ANIM_PC)
DECL_3DENGINE(COLOR_ANIM_DATA)
DECL_3DENGINE(NUMERIC_ANIM_DATA)
DECL_3DENGINE(TEXTURE_ANIM_DATA)

#include INCL_KCOM(CommunicatorType/Communicator)

namespace QDT
{
	namespace M3D
	{
		/** 
			[MBi - 2006.01.26] - 

				Act as a global animated nodes container.
				A better name would be ANIMATION_MANAGER.

		 */
		class	BEHAVIOR_MANAGER
		{
		public:

			////////////////////////////////////////////////////
			explicit					BEHAVIOR_MANAGER();
			~BEHAVIOR_MANAGER();
			////////////////////////////////////////////////////

			////////////////////////////////////////////////////
			enum
			{
				COMMAND_PLAY_FACE = CMD_USER_START,
				COMMAND_CREATE_HIPS_ANIM,
				CMD_INTERPOLATE_BLEND_SHAPE,
				CMD_BLINK_BLEND_SHAPE,
				CMD_PERIODIC_BLINK_BLEND_SHAPE,
				CMD_ANIMATE_BLEND_SHAPE,
				CMD_GET_BLEND_SHAPE_VALUE,

				COMMAND_CREATE_TARGET_ANIM,
				CMD_RESET_ANIM_POOL,
				CMD_LAST,
			};
			////////////////////////////////////////////////////

			////////////////////////////////////////////////////
			/** this enum holds the flags that can be found on an animation*/ 
			enum ANIM_FLAG
			{
				ANIM_FINISHED			= 1 << 0  ,	//	1			Reach the last frame
				ANIM_LOOP				= 1 << 1  ,	//	2			Looping will last forever
				ANIM_SKELETON			= 1 << 2  ,	//	4			To mark when it s an animation on a skeleton
				ANIM_TIMES				= 1 << 3  ,	//	8			Times repetition
				ANIM_OK_TO_DELETE		= 1 << 4  ,	//	16			When this flag is u al the other should be down
				ANIM_TELEPORTABLE		= 1 << 5  ,	//	32			Animation allows teleportation
				ANIM_PREAMPTIVE			= 1 << 6  ,	//	64			Preamptive means it cuts current animation
				ANIM_ROTATION			= 1 << 7  ,	//	128			DO NOT SET MANUALLY Rotation are in this animation
				ANIM_TRANSLATION		= 1 << 8  ,	//	256			DO NOT SET MANUALLY Translation are in this animation
				ANIM_SCALING			= 1 << 9  , //	512			DO NOT SET MANUALLY Scaling are in this animation 
				ANIM_DONTMATCHSKELETON	= 1 << 10 ,	//	1024		Do NOT check Hierarchy structure
				ANIM_ABSOLUTE			= 1 << 11 ,	//	2048		Absolute caracter of animation
				ANIM_HIPSFRAME0			= 1 << 12 ,	//	4096		DO NOT SET MANUALLY For the HIPS to use ref data
				ANIM_TOPANIM			= 1 << 13 ,	//  8192		Animation Tops will be tested on the first item
				ANIM_BLENDING			= 1 << 14 ,	//	16384		Blending will occur with the preceding anim	
				ANIM_NOTELE_TRANS_X		= 1 << 15 ,	//	32768		Translation X will be IGNORED during teleportation
				ANIM_NOTELE_TRANS_Y		= 1 << 16 ,	//	65536		Translation X will be IGNORED during teleportation
				ANIM_NOTELE_TRANS_Z		= 1 << 17 ,	//	131072		Translation X will be IGNORED during teleportation
				ANIM_NOTELE_ROT_X		= 1 << 18 ,	//	262144		Rotation X will be IGNORED during teleportation
				ANIM_NOTELE_ROT_Y		= 1 << 19 ,	//	524288		Rotation Y will be IGNORED during teleportation
				ANIM_NOTELE_ROT_Z		= 1 << 20 ,	//  1048576		Rotation Z will be IGNORED during teleportation
				ANIM_OFFSET_PERCENT		= 1 << 21 ,	//  2097152		Offset Value must be interpreted as percentag
				ANIM_TO_MIX				= 1 << 22 ,	//	4194304		To be inserted in the firstsequence
				ANIM_ACCEL				= 1 << 23 ,	//	8388608		For the MoveTo to start with acceleration
				ANIM_DECCEL				= 1 << 24 ,	//	16777216	For the MoveTo to end with deceleration
				ANIM_PINGPONG			= 1 << 25 ,	//	33554432	Begin to end then end to begin and again
				ANIM_RELCOR				= 1 << 26 , //	67108864	To play rotation absolutely even in relative animation
				ANIM_REVERSE_AIM		= 1 << 27 ,	//	134217728	To reverse AimConstraints ( for characters ) 
			};
			////////////////////////////////////////////////////
			//=============================================================================
			// From COMMUNICATOR : 
			//=============================================================================
			static BEHAVIOR_MANAGER*	Instance();
			static void					Suicide();
			
			static void					EM_Construct(void * const pO, void * const pR, void * const pP);
			static void					EM_Destruct(void * const pO, void * const pR, void * const pP);

			//////////////////////////////////////////////////////////////////////
			bool						ApplyAnimationToNode(const COM_INSTANCE& NodeInst, const COM_INSTANCE& Anim, int& nIndex, CONTAINER *pMainContainer );	
			COM_INSTANCE				ApplyFreeCameraAnimationToNode	(const COM_INSTANCE & CameraInst);
			void						SetAnimationSpeed(const COM_INSTANCE & ChildInst, COM_INSTANCE &	Anim, Float32 rSpeed);
			void						RemoveAllAnimationFromNode( const COM_INSTANCE& ChildInst );
		
			COM_INSTANCE				CreateHipsAnim();
			void						ReleaseHipsAnims();
			void						AddFreeHipsAnim( const COM_INSTANCE& CI );
			
			void						ResetAnimPool();
			
			void						PlayAnim( const COM_INSTANCE& NodeInst, const COM_INSTANCE& Anim, bool bSeq );
			void						UpdateAnimation(Float32 rSpeedFactor);
			
			// Colors
			void						AddAnimatedColor(COLOR_ANIM_DATA * pColorAnimData);
			void						RemoveAnimatedColor(COLOR_ANIM_DATA * pColorAnimData);

			// Numeric attributes
			void						AddAnimatedNumericAttributes(NUMERIC_ANIM_DATA * pNumericAnimData);
			void						RemoveAnimatedNumericAttributes(NUMERIC_ANIM_DATA * pNumericAnimData);

			// Texture uv anim
			void						AddAnimatedMaterialTexture(TEXTURE_ANIM_DATA * pTextureAnimData);
			void						RemoveAnimatedMaterialTexture(TEXTURE_ANIM_DATA * pTextureAnimData);

			// Animated Nodes 
			void						UpdateAnimatedNodesChildren();
			void						AddAnimatedNode(const COM_INSTANCE & ciNode);
			void						RemoveAnimatedNode(const COM_INSTANCE & ciNode);

			// Controller Nodes
			void						AddNodeWithController(NODE * pNode);
			void						RemoveNodeWithController(NODE * pNode);

			static SINGLETON_DESCRIPTOR*	GetSingletonDescriptor();
			static SINGLETON_DESCRIPTOR*	CreateSingletonDescriptor();

			//////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////
			//	COMMANDS
			//////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////
			static void EM_FreeCamera(void * const pO, void * const pR, void * const pP);
			static void EM_ClearAnimation(void * const pO, void * const pR, void * const pP);
			static void	EM_DeleteAnimation(void * const pO, void * const pR, void * const pP);
			static void	EM_PurgeBehavior(void * const pO, void * const pR, void * const pP);
			static void	EM_PlayFace(void * const pO, void * const pR, void * const pP);
			static void	EM_CreateHipsAnim(void * const pO, void * const pR, void * const pP);
			static void	EM_InterpolateBlendShape(void * const pO, void * const pR, void * const pP);
			static void	EM_BlinkBlendShape(void * const pO, void * const pR, void * const pP);
			static void	EM_PeriodicBlinkBlendShape(void * const pO, void * const pR, void * const pP);
			static void	EM_AnimateBlendShape(void * const pO, void * const pR, void * const pP);
			static void	EM_GetBlendShapeValue(void * const pO, void * const pR, void * const pP);
			static void	EM_ResetAnimPool(void * const pO, void * const pR, void * const pP);
			//////////////////////////////////////////////////////////////////////

			void	DumpAnimatedNodes(const QDT_STRING & sPad, QDT_STRING & sDump) const;

		protected:
			
			CONTAINER*					ProcessHeadNodeApply( NODE* pN, ANIM_NODE_STRUCT *pAN, const COM_INSTANCE& Anim );
			
			/** 
				[MBi - 2006.01.26] - About the methods there:

					Moved here methods used only internally in order to clarify the public interface.

			 */
			/// Communicator.
			static void					COM_Init(CLASS_DESCRIPTOR * const pCD);

			/// Hips Anim management.
			/** 
				[MBi - 2006.01.26] - About hips anims management:

						As far as I understand the current implementation, HIPS_ANIM are preallocated in a pool.
						When a new animation is loaded, a preallocated HIPS_ANIM is retrieved from _sAnimationInsts.
						If the pool is full, more allocation are made.
						The pool is only flushed when the BEHAVIOR_MANAGER singleton is killed or when
					one of the many Reset function is called.

			 */
			COM_INSTANCE				QueryHipsAnim();
			void						ReserveHipsAnims( int n );

			/// Update methods.
			void						UpdateColors();
			void						UpdateNumericAttributes();
			void						UpdateMaterialTextureAnim();
			void						UpdateAnimatedNodes(Float32 rSpeedFactor);
			void						UpdateNodesWithController(Float32 rSpeedFactor);

		private:

			/////////////////////////////////////////////////////
			QDT_DLIST<COM_INSTANCE>			_vAnimatedNodes;
			QDT_DLIST<NODE *>				_vNodesWithController;
			QDT_DLIST<COLOR_ANIM_DATA *>	_vAnimatedColors;
			QDT_DLIST<NUMERIC_ANIM_DATA *>	_vAnimatedNumAttr;
			QDT_DLIST<TEXTURE_ANIM_DATA *>	_vAnimatedTexture;
			/////////////////////////////////////////////////////
			static BEHAVIOR_MANAGER	   *	_pInstance;
			/////////////////////////////////////////////////////
			QDT_DLIST<COM_INSTANCE>			_sAnimationInsts ;
			/////////////////////////////////////////////////////
		};
	}
}

#endif // __M3D_BEHAVIOR_MANAGER_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
