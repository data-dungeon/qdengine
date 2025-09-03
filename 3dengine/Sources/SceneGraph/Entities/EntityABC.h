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
// CLASS: ENTITY_ABC
//
//
//	01-02-08:	RMA - Created
//*****************************************************************************


//=============================================================================
// Name: ENTITY_ABC
//	01-02-08:	RMA - Created
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_ENTITYABC_H__
#define __M3D_SCENEGRAPH_ENTITIES_ENTITYABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(DISPLAY_PARAMETERS)
DECL_3DENGINE(RAY_CAST_PARAMETERS)
DECL_3DENGINE(NODE)
DECL_3DENGINE(SCENE_GRAPH)
DECL_3DENGINE(PARTICLE_SYSTEM)
DECL_KMATH(MATRIX)
DECL_KMATH(FRUSTRUM)
DECL_KMATH(BOUNDING_VOLUME)

#include INCL_KCOM(CommunicatorType/Communicator)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace QDT
{
	namespace M3D
	{
		enum ENTITY_TYPE
		{
			ENTITY_NONE,

			ENTITY_CAMERA,
			ENTITY_JOINT,
			ENTITY_SKELETON,
			ENTITY_SOUND,
			ENTITY_TRIGGER,
			ENTITY_SCENE_GRAPH,
			ENTITY_PARTICLE_EMITTER,
			ENTITY_QD_TRANSFORM,
			ENTITY_QUADTREE,
			ENTITY_PATH,
			ENTITY_GLOW,
			ENTITY_INTERFACE_3D,
			ENTITY_PRIMITIVE_SET,

			ENTITY_MESH,
			ENTITY_BLEND_SHAPE,
			ENTITY_SOFT_BODY,

			ENTITY_POINT_LIGHT,
			ENTITY_DIRECTIONAL_LIGHT,
			ENTITY_SPOT_LIGHT,
			
			ENTITY_UNIFORM_FIELD,
			ENTITY_RADIAL_FIELD,
			ENTITY_VORTEX_FIELD,
			ENTITY_LIFE_FIELD,
			
			ENTITY_NAVIGATION_MESH,
			ENTITY_WAY_POINT,
			ENTITY_MESH_GROUP,
			ENTITY_COLLISION_MESH,

			ENTITY_PERTURBATION_FIELD,

			ENTITY_AMBIENT_OCTREE,

			ENTITY_CONSTRAINT,

			ENTITY_MAX = 0xFFFFFFFF
		};

		class ENTITY_ABC : public COMMUNICATOR
		{
		public :

			enum
			{
				CMD_ENTITY_LAST = CMD_COM_USER_START,
			};


											ENTITY_ABC(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType);
											ENTITY_ABC(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType, COM_HANDLE * pCH);
			virtual						   ~ENTITY_ABC();

			_INLINE_ void					SetNodeInst(const COM_INSTANCE & NodeInst);
			_INLINE_ const COM_INSTANCE &	GetNodeInst() const;
			_INLINE_ NODE *					GetNode();
			_INLINE_ const NODE *			GetNode() const;

			_INLINE_ void					SetSceneGraph(SCENE_GRAPH * pSceneGraph);
			_INLINE_ SCENE_GRAPH *			GetSceneGraph();

			virtual void					Register();
			virtual void					Unregister();

			virtual void					Instanciate(COM_INSTANCE & Instance);
			virtual void					InstanciateRec(COM_INSTANCE & Instance);

			void	AttachChild(const COM_INSTANCE & ChildInst);
			void	AttachChild(const COM_INSTANCE & ChildInst, int nIndex);
			void	AttachChildKeepGlobalMatrix(const COM_INSTANCE & ChildInst, int nIndex);
			void	Detach();
			void	DetachKeepHierarchy();
			void	DetachKeepGlobalMat();

			virtual void					Display();
			virtual void					Display(UInt32 nPrimitive);
			virtual void					Display(const QDT_VECTOR<UInt32> & vPrimitives);
			virtual void					FindShadowMap(DISPLAY_PARAMETERS & DisplayParameters);
			virtual float					GetDistanceFromCamera(unsigned int nGeometry, NODE * pNode);
			virtual	bool					RayCast(RAY_CAST_PARAMETERS & RayCastParameters);
			virtual VECTOR					GetCenter() const;
			virtual void					Dump(IO_STREAM &Out);
			virtual void					UpdateParticleEmitter(unsigned int nDeltaTime, const MATRIX & GlobalMatrix, PARTICLE_SYSTEM & ParticleSystem);
			virtual bool					IsInside(NODE *pTriggerNode);
			virtual void					Update();
			virtual bool					ToUpdate() const;
			virtual bool					TestVisibility(const FRUSTRUM & Frustum);
			virtual void					ClearShadowMapFlags();
			virtual void					HasJustBeenCreated();
			virtual	void					InitParameters(float rFogDec);
			virtual	void					ResetParameters();
			virtual	void					InterpolateLighting(float rTime);
			virtual	const BOUNDING_VOLUME*	GetCollisionVolume() const;

			_INLINE_ ENTITY_TYPE			GetEntityType() const	{ return (_eEntityType); }

			_INLINE_ bool					IsMesh() const				{ return (_eEntityType == ENTITY_MESH); }
			_INLINE_ bool					IsLight() const				{ return ((_eEntityType == ENTITY_POINT_LIGHT) || (_eEntityType == ENTITY_DIRECTIONAL_LIGHT) || (_eEntityType == ENTITY_SPOT_LIGHT)); }	
			_INLINE_ bool					IsForceField() const		{ return ((_eEntityType == ENTITY_UNIFORM_FIELD) || (_eEntityType == ENTITY_RADIAL_FIELD) || (_eEntityType == ENTITY_VORTEX_FIELD) || (_eEntityType == ENTITY_LIFE_FIELD) || (_eEntityType == ENTITY_PERTURBATION_FIELD)); }
			_INLINE_ bool					IsSoftBody() const			{ return (_eEntityType == ENTITY_SOFT_BODY); }
			_INLINE_ bool					IsParticleEmitter() const	{ return (_eEntityType == ENTITY_PARTICLE_EMITTER); }

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

		protected :

			void							Copy(const ENTITY_ABC & C);

			void							InstanciateNode(ENTITY_ABC * pInstance);
			void							InstanciateNodeRec(ENTITY_ABC * pInstance);

			COM_INSTANCE					_NodeInst;

			bool							_bVisible;
			SCENE_GRAPH	*					_pSceneGraph;

		private :

			static	void					EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void					EM_Destruct(void * const pO, void * const pR, void * const pP);

			static void						CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void						CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			ENTITY_TYPE						_eEntityType;
		};
	}
}

#include INCL_3DENGINE(SceneGraph/Node/Node)

#ifndef _DEBUG
	#include "EntityABC.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_ENTITYABC_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
