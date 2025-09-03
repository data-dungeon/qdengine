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
//	CLASS:	MESH
//	The MESH class implements ...
//
//	05-10-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_MESH_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_MESH_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(PRIMITIVE)
DECL_KMATH(OBB)
DECL_KMATH(SPHERE)
DECL_KMATH(FRUSTRUM)
DECL_KPHYSICS(I_PHYSIC_STATIC_ACTOR)
DECL_KPHYSICS(I_PHYSIC_SHAPE_MESH_DATA)
DECL_KPHYSICS(I_PHYSIC_MATERIAL)
DECL_KRENDERER(I_VERTEX_DECLARATION)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_INDEX_BUFFER)
DECL_3DENGINE(ENGINE_TEXTURE_FLOW)
DECL_3DENGINE(FLOW)
DECL_3DENGINE(FLOW_DATA)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		class	MESH : public ENTITY_ABC
		{
		public:
						MESH();
			explicit	MESH(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType);
			virtual		~MESH();

			/** For flags usage */
			enum
			{
				MESH_FLAG_INIT	= 1 << 0,
				MESH_ALL_FLAGS	= 0xFFFFFFFF,
			};

			enum
			{
				DISPLAY_FLAG_VISIBLE			= 1 << 0,
				DISPLAY_FLAG_CAST_SHADOWS		= 1 << 1,
				DISPLAY_FLAG_RECEIVE_SHADOWS	= 1 << 2,
				DISPLAY_ALL_FLAGS				= 0xFFFFFFFF,
			};

			// From ENTITY_ABC
			void	Register();
			void	Unregister();
			void	Instanciate(COM_INSTANCE & Instance);
			void	InstanciateRec(COM_INSTANCE & Instance);

			// Initialization
			virtual void	Init();
			virtual void	Clear();
			void			UpdateLocalBoundingVolumes();

			// Accessors
			_INLINE_ Bool						GetMeshFlags(UInt32 nFlagsMask) const;
			_INLINE_ void						SetMeshFlags(UInt32 nFlagsMask, Bool b);
			_INLINE_ Bool						GetVisible() const;
			_INLINE_ void						SetVisible(Bool b);
			_INLINE_ Bool						GetCastShadows() const;
			_INLINE_ void						SetCastShadows(Bool b); 
			_INLINE_ Bool						GetReceiveShadows() const;// obso a virer !
			_INLINE_ void						SetReceiveShadows(Bool b); 
			_INLINE_ UInt32						GetNbrVertexBuffers() const;
			_INLINE_ UInt32						GetNbrIndexBuffers() const;
			_INLINE_ UInt32						GetNbrPrimitives() const;
			_INLINE_ OBB *						GetGlobalBoundingBoxes();
			_INLINE_ OBB *						GetGlobalBoundingBox(UInt32 nIndex);
			_INLINE_ I_PHYSIC_STATIC_ACTOR *	GetStaticActor();
			_INLINE_ void						SetMeshData(const COM_INSTANCE & MeshDataInst);
			_INLINE_ void						InitMeshData(const COM_INSTANCE & MeshDataInst);
			_INLINE_ void						InitPhysicData(const COM_INSTANCE & PhysicDataInst);

			// Display
			virtual Bool	TestVisibility(const FRUSTRUM & Frustum, UInt32 uiDisplayFlags);
			virtual Bool	TestVisibility(const CONE & Cone, UInt32 uiDisplayFlags);
			virtual void	Display(UInt32 nPrimitive);
			virtual void	Display(const QDT_VECTOR<UInt32> & vPrimitives);

			// RayCast
			virtual Bool	RayCast(RAY_CAST_PARAMETERS & RayCastParameters);

			// Compute camera distance
			virtual Float32	ComputeCameraDistance(UInt32 uiPrim);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

			FLOW* GetFlow(ENGINE_TEXTURE_FLOW *	flowTex, FLOW_DATA* flowData);
			void UpdateFlows();

		protected:

			// Communication
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void	CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);	
			static void	CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			void	FindLights(const SPHERE & PrimitiveSphere);
			void	SendPrimitiveToDisplayList(UInt32 uiPrim, Bool bForceTransparentObject = false);
			void    DrawPrimitive(UInt32 uiPrim, const VECTOR& shCenter);

			I_INDEX_BUFFER * SortFaces(UInt32 uiIndex);	

			QDT_FLAGS	_fMeshFlags;
			QDT_FLAGS	_fDisplayFlags;

			// Dynamic datas
			// Primitives
			UInt32		_nNbrPrimitives;
			OBB *		_pGlobalBoundingBox;
			SPHERE *	_pGlobalBoundingSphere;
			Bool *		_pbVisible;
			
			FLOW**		_pFlows;
			UInt32		_numFlows;

			// Static datas
			COM_INSTANCE _MeshDataCI;

			// Physic datas
			COM_INSTANCE _PhysicDataCI;

			// Physic dynamic stuff
			I_PHYSIC_STATIC_ACTOR *		_pStaticActor;
			I_PHYSIC_SHAPE_MESH_DATA *	_pPhysicShapeMeshData;
			I_PHYSIC_MATERIAL *			_pPhysicMaterial;
		};
	}
}

#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/MeshData)

#ifndef _DEBUG
	#include "Mesh.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_MESH_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
