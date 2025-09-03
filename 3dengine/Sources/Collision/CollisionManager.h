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
//	CLASS:	COLLISION_MANAGER
//	The COLLISION_MANAGER class implements ...
//
//	03-09-01:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#error

#ifndef __M3D_COLLISION_COLLISIONMANAGER_H__
#define __M3D_COLLISION_COLLISIONMANAGER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCOM(SINGLETON_DESCRIPTOR)
DECL_KMATH(VECTOR)
DECL_KMATH(POINT2D)
DECL_KMATH(OBB)
DECL_KMATH(AABB)
DECL_KMATH(BOUNDING_VOLUME)
DECL_3DENGINE(NODE)
DECL_3DENGINE(COLLISION_MESH)
DECL_3DENGINE(COLLISION_ENTITY)
DECL_3DENGINE(COLLISION_NOTIFIER)

#include INCL_KCOM(ComBasis/ClassDescriptor)
#include INCL_KCOM(CommunicatorType/ComInstance)
#include INCL_3DENGINE(SceneGraph/Node/Node)
#include INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CollisionMesh)

#define NB_MAX_SIMULTANEOUS_DYNAMIC_COLS 10

namespace QDT
{
	namespace M3D
	{
		class COLLISION_MANAGER
		{
		public:

			enum CMCP_FLAGS
			{
				CMCPF_STATIC		=	0x00000001,
				CMCPF_DYNAMIC		=	0x00000002,
				CMCPF_USER_OBB		=	0x00000004,

				CMCPF_ALL			=	0xffffffff
			};

			enum
			{
				CMD_REGISTER_NODE = CMD_USER_START,
				CMD_UNREGISTER_NODE,
				CMD_REGISTER_MESH,
				CMD_UNREGISTER_MESH,
				CMD_ENABLE_PROCESSING,
				CMD_IS_PROCESSING,
				CMD_SET_WEIGHT,
				CMD_GET_WEIGHT,
				CMD_SET_COLLISION_MODE,
				CMD_GET_COLLISION_MODE,
				CMD_UNREGISTER_ALL_NODES,
				CMD_UNREGISTER_DYNAMIC_ONLY_NODES,
				CMD_SET_NODE_AS_GROUND_POINT,
				CMD_CORRECT_POSITION
			};

			class COLLISION_MESH_CONTAINER
			{
			public:
				inline								COLLISION_MESH_CONTAINER(const COM_INSTANCE &Inst)  
													{ 
														_CMeshInst	= Inst;
														_CMeshNodeInst = ((ENTITY_ABC *)(Inst.GetCommunicator()))->GetNodeInst();
														_pNext		= 0;
													}
				inline	void						SetNext(COLLISION_MESH_CONTAINER *pNext)	{ _pNext = pNext; }
				inline	COLLISION_MESH_CONTAINER	*GetNext(void)								{ return _pNext; }
				inline	COLLISION_MESH				*GetMesh(void)								{ return static_cast<COLLISION_MESH*>(_CMeshInst.GetCommunicator()); }
				inline	int							IsLoaded(void)								{ return _CMeshInst.IsLoaded(); }
				inline	const COM_INSTANCE			&GetMeshCI(void)							{ return _CMeshInst; }
				inline  const COM_INSTANCE			&GetMeshNodeCI(void)						{ return _CMeshNodeInst; }
				
			protected:
				COLLISION_MESH_CONTAINER *	_pNext;
				COM_INSTANCE				_CMeshInst;
				COM_INSTANCE				_CMeshNodeInst;
			};
											COLLISION_MANAGER();
											~COLLISION_MANAGER();

			// For singleton
			static	COLLISION_MANAGER		*Instance();
			static	void					Release();
			static	void					COM_Init(CLASS_DESCRIPTOR * const pCD);

			static	void					EM_Construct		(void * const pO, void * const pR, void * const pP);
			static	void					EM_Destruct			(void * const pO, void * const pR, void * const pP);
			static	void					EM_Loop				(void * const pO, void * const pR, void * const pP);

			static	void					EM_SetWeight		(void * const pO, void * const pR, void * const pP);
			static	void					EM_GetWeight		(void * const pO, void * const pR, void * const pP);

			static	void					EM_SetCollisionMode	(void * const pO, void * const pR, void * const pP);
			static	void					EM_GetCollisionMode	(void * const pO, void * const pR, void * const pP);

			static	void					EM_RegisterNode		(void * const pO, void * const pR, void * const pP);
			static	void					EM_UnRegisterNode	(void * const pO, void * const pR, void * const pP);

			static	void					EM_RegisterMesh		(void * const pO, void * const pR, void * const pP);
			static	void					EM_UnRegisterMesh	(void * const pO, void * const pR, void * const pP);

			static	void					EM_EnableProcessing	(void * const pO, void * const pR, void * const pP);
			static	void					EM_IsProcessing		(void * const pO, void * const pR, void * const pP);

			static	void					EM_UnRegisterAllDynamicNodes	(void * const pO, void * const pR, void * const pP);
			static	void					EM_UnRegisterAllNodes			(void * const pO, void * const pR, void * const pP);

			static	void					EM_IsPositionValid	(void * const pO, void * const pR, void * const pP);

			static	void					EM_SetNodeAsGroundPoint (void * const pO, void * const pR, void * const pP);

			static	void					EM_CorrectPosition	(void * const pO, void * const pR, void * const pP);

			static	SINGLETON_DESCRIPTOR	*GetSingletonDescriptor();
			static  SINGLETON_DESCRIPTOR	*CreateSingletonDescriptor();

			// Insertion of entities
					
					int						InsertNode(const COM_INSTANCE &NodeCI);
					int						InsertNode(const COM_INSTANCE &NodeCI, const COM_INSTANCE &NodeCIBounding, int bIgnoreValidityTest);
					int						InsertNode(COLLISION_ENTITY *pEntity, COM_INSTANCE *pNodeCIBounding=0, int bIgnoreValidityTest=false);
					void					InsertMesh(const COM_INSTANCE &ComMeshCI);
					int						InsertMesh(COLLISION_MESH_CONTAINER	*pColMeshContainer);
					
					void					RemoveNode(const COM_INSTANCE &ComNodeCI);
					void					RemoveNode(COLLISION_ENTITY *pEntity, int bFromDestructor=false, int bDestructEntity=true);
					void					RemoveMesh(const COM_INSTANCE &ComMeshCI);
					void					RemoveMesh(COLLISION_MESH_CONTAINER	*pColMeshContainer);
					void					RemoveMesh(COLLISION_MESH	*pColMesh);

					void					ReplaceNode(COLLISION_ENTITY *pEntity, COLLISION_ENTITY *pNewEntity);

					void					UnRegisterAllDynamicNodes(void);
					void					UnRegisterAllNodes(void);

			// Activation of entities
					void					SetActivity(NODE *pNode, int bOnOff);
					void					SetActivity(const COM_INSTANCE &NodeCI, int bOnOff);
					bool					GetActivity(NODE *pNode);
					bool					GetActivity(const COM_INSTANCE &NodeCI);
					void					EnableNodeOnGround(NODE *pNode, int bOnOff);
					void					EnableNodeOnGround(const COM_INSTANCE &NodeCI, int bOnOff);

			// Teleport
					void					ObjectHasBeenTeleported(COM_INSTANCE TeleportedNode);
					void					ObjectHasBeenTeleported(NODE *pNode);

			// Position validity test
					int						IsPositionValid(NODE *pNode, int bTryToReplace=false);
					int						IsPositionValid(VECTOR &Point, int bTryToReplace=false);

			// Enable Processing
					bool					IsCollisionEnable();
					void					EnableProcessing(int OnOff);
					void					SetGravityProcessing(NODE *pNode, int bOnOff);
					void					SetGravityProcessing(const COM_INSTANCE &NodeCI, int bOnOff);

			// Callback
					void					SetNotifier(const NODE *pNode, const COLLISION_NOTIFIER *pNotifier);
					void					UnsetNotifier(const NODE *pNode);

			// Entities Weight & mode
					void					SetEntityWeight(const COM_INSTANCE &NodeCI, int nWeight);
					int						GetEntityWeight(const COM_INSTANCE &NodeCI);
					void					SetEntityCollisionMode(const COM_INSTANCE &NodeCI, int nColMode);
					int						GetEntityCollisionMode(const COM_INSTANCE &NodeCI);
					void					SetEntityDebugInfo(const COM_INSTANCE &NodeCI, const QDT_STRING & sDebugInfo);
					const QDT_STRING &		GetEntityDebugInfo(const COM_INSTANCE &NodeCI) const;

			// Misc
					void					ReleaseLists(void);
					int						GetGroundAltitude(VECTOR &SourcePos, float &fAltitude);
					int						GetNodeYLockOnGround(COLLISION_ENTITY *pEntity, float &fAltitude);
					int						GetNodeYLockOnGround(const COM_INSTANCE &NodeCI, float &fAltitude);

			// Camera related methods
					int						CorrectPosition(VECTOR &TargetP, VECTOR &Position, VECTOR &CW, VECTOR &CCW, OBB *pObb, int nbObb, float rMinResizeAllowed=1e33, int nFlag = CMCPF_ALL);
					void					ExcludeCameraSectors(AABB *	pAABB);
					void					ResetCameraSectors(void);

		protected:
			// Camera related methods
					int						CPStaticLoop(VECTOR &Source, VECTOR &CW, VECTOR &CCW, int &bMerged, float rMinResizeAllowed);
					int						CPDynamicLoop(VECTOR &Source, VECTOR &CW, VECTOR &CCW, int &bMerged, OBB *pObb, int nbObb, int Flag, float rMinResizeAllowed);
					int						CPDynamicObject(COLLISION_ENTITY *pObj, VECTOR &Source, VECTOR &Dest, float fSens, BOUNDING_VOLUME *pBV=0, float rMinResizeAllowed=1e33, bool bTryToResize=true);

					void					Loop(void);
					void					StaticLoop(void);
					int						ProcessEntity(COLLISION_ENTITY *pEntity);
					int						ProcessDynamicEntity(COLLISION_ENTITY *pEntity, int EntityID);
					int						ProcessBetween(COLLISION_ENTITY *pSource, COLLISION_ENTITY *pAgainst);
					void					DynamicLoop(void);
					int						ShallIProcess(COLLISION_MESH_CONTAINER *pContainer);
					inline	void			CorrectOnEdge( POINT2D &vSource2, POINT2D &vNorm12, float &Source2SqLen, float &SourceDestLen, float &SourceDestSqLen, VECTOR &Source, VECTOR &Dest );

					COLLISION_ENTITY		*GetEntity(const NODE *pNode);

					bool					TestIntersection(const POINT2D & Source, POINT2D & Segment, const float & rDist1, const float & rDist3, const POINT2D & vDir1, const POINT2D & vDir3, const POINT2D & Center);
					int						ProcessInside(OBB & Obb, VECTOR &Source, VECTOR &Dest, float fSens, BOUNDING_VOLUME *pBV=0);

		protected:
					int							_nbEvents;
					COLLISION_ENTITY			*_FirstCollider[NB_MAX_SIMULTANEOUS_DYNAMIC_COLS];
					COLLISION_ENTITY			*_CollideWith[NB_MAX_SIMULTANEOUS_DYNAMIC_COLS];

					COM_INSTANCE				_NotifierInst;
					int							_IsActive;
					COLLISION_ENTITY			*_pFirstEntity;
					COLLISION_MESH_CONTAINER	*_pFirstMesh;
		};
	}
}

#endif // __M3D_COLLISION_COLLISIONMANAGER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
