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
// CLASS: SKELETON
//
//
//00-11-09 GHP Created: 
//*****************************************************************************

//=============================================================================
// Name: SKELETON
//00-11-09 GHP Created: 
//=============================================================================

#ifndef __SCENEGRAPH_ENTITIES_SKELETON_H__
#define __SCENEGRAPH_ENTITIES_SKELETON_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KPHYSICS(I_PHYSIC_JOINT_CUSTOM)
DECL_3DENGINE(HAIR)
DECL_3DENGINE(NODE_CHILD)

#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/Mesh)
#include	INCL_3DENGINE(SceneGraph/Entities/Joint/Joint)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)

#define MAX_LIGHT_COUNT 4

namespace QDT
{
	namespace M3D
	{
		class SKELETON : public MESH
		{	
		public:
			
						SKELETON();
						~SKELETON();

			// From ENTITY_ABC
			void	Register();
			void	Unregister();
			void	Instanciate(COM_INSTANCE & Instance);
			void	InstanciateRec(COM_INSTANCE & Instance);

			// Initialization
			virtual void	Init();
			virtual void	Clear();

			// Accessors
			const COM_INSTANCE &	GetSpecialJoint(JOINT::TYPE SpecialJointType);

			// Display
			virtual Bool	TestVisibility(const FRUSTRUM & Frustum, UInt32 uiDisplayFlags);
			virtual Bool	TestVisibility(const CONE & Cone, UInt32 uiDisplayFlags);
			virtual void	Display(UInt32 nPrimitive);
			virtual void	Display(const QDT_VECTOR<UInt32> & vPrimitives);

			// Compute camera distance
			virtual Float32	ComputeCameraDistance(UInt32 uiPrim);

			/** Dynamic Stuffs */
			MATRIX&					GetInfluentObjectRefMatrix(int nIdxJoint);
			Bool					StoreJointIfSpecial(const COM_INSTANCE & JointNodeInst, JOINT_TYPE JointType);
			void					AddRefMatrix( MATRIX& M);

			/** Ray cast */
		//	Bool						RayCast(RAY_CAST_PARAMETERS & RayCastParameters, Bool & bContinue);

			void						GetCharacterAabb(AABB & Aabb);

			_INLINE_ void				SetNeedComputeBoundingBox(Bool b)		{_bNeedComputeBoundingBox = b;}
			
			// Hair
			_INLINE_ HAIR *				GetHair();

			// Ragdoll
			_INLINE_ void	InitRagdollData(const COM_INSTANCE & RagdollDataCI);

			//	Compatibiliy with NODE_ARRAY
			Bool						StoreSklChildIfSpecial(NODE_CHILD * pNodeChild);
			NODE_CHILD *				GetSpecialNodeChild(int Type);
			int							GetSpecialBoneIndex(int nType);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		protected:

			void						GetBodyPos(VECTOR & Pos);

			void						DisplaySkeleton();
			void						DisplayBoundingBoxes(DISPLAY_PARAMETERS & DisplayParameters) const;
			void						DisplayTriggerVolume(const DISPLAY_PARAMETERS & DisplayParameters);

			// Dynamic Stuffs :
			AABB						_GlobalBoundingBox;
			Bool						_bNeedComputeBoundingBox;

			QDT_VECTOR<MATRIX>			_vRefGlobalMatrices;
		 
			COM_INSTANCE			    _SpecialJoints[NBR_JOINT_TYPES];
			NODE_CHILD *				_SpecialSklJoints[NBR_JOINT_TYPES];

			// Hair
			HAIR *	_pHair;

			// Ragdoll
			COM_INSTANCE				_RagdollDataCI;
			UInt32						_nNbrRagdollJoints;
			I_PHYSIC_JOINT_CUSTOM **	_pRagdollJoints;

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunk
			static void		CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#ifndef _MASTER	
			static void		CM_WriteChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		};
	}
}

#ifndef _DEBUG
	#include	"Skeleton.inl"
#endif

#endif // __SCENEGRAPH_ENTITIES_SKELETON_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
