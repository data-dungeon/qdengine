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
//	CLASS:	MESH_GROUP
//	The MESH_GROUP class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(MESH_GROUP_GEOMETRY)
DECL_KPHYSICS(I_PHYSIC_STATIC_ACTOR)
DECL_KPHYSICS(I_PHYSIC_SHAPE_MESH_DATA)
DECL_KPHYSICS(I_PHYSIC_MATERIAL)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)

namespace QDT
{
	namespace M3D
	{
		class	MESH_GROUP : public ENTITY_ABC
		{
		public:

			// Constructor & destructor
											MESH_GROUP();
										~MESH_GROUP();

			// Accessors
			_INLINE_ unsigned int			GetNbrGeometries();
			_INLINE_ unsigned int			GetNbrSharedVertexBuffers();
		//	_INLINE_ const AABB_TREE &			GetAABBTree() const;
		//	_INLINE_ AABB_TREE &				GetAABBTree();
			_INLINE_ const AABB	&			GetBoundingBox() const;
		//	_INLINE_ GEOMETRY_ABC *			GetGeometry(int nNumGeometry) const;
			_INLINE_ MESH_GROUP_GEOMETRY *	GetMeshGroupGeometry(int nNumGeometry) const;
		//	_INLINE_ GEOMETRY_ABC *			GetSharedVB(unsigned int i) const; 
			_INLINE_ I_PHYSIC_STATIC_ACTOR *	GetStaticActor() const;
			_INLINE_ void						SetPhysicMaterial(I_PHYSIC_MATERIAL * pMaterial);
			_INLINE_ bool					IsMeshGroupGeometryVisible(int nNumGeometry) const;

			// Modifiors
			void							SetNbrGeometries(unsigned int nCount);
			void							SetNbrSharedVertexBuffers(unsigned int nCount);
			_INLINE_ void					SetBoundingBox(const AABB & Box);
		//	_INLINE_ void					SetSharedVertexBuffer(unsigned int i, GEOMETRY_ABC * pGeometry);
		//	_INLINE_ void					SetGeometry(unsigned int i, GEOMETRY_ABC * pGeometry);
			_INLINE_ void					SetMaterial(unsigned int i, const COM_INSTANCE & ciMat);
			_INLINE_ void					SetBoundingBox(unsigned int i, const AABB & Box);
			_INLINE_ void					SetBoundingSphere(unsigned int i, const SPHERE & Sphere);
			_INLINE_ void					SetDoubleSided(unsigned int i, bool b);
			_INLINE_ void					SetCullingOnShadowing(unsigned int i, bool b);
			_INLINE_ void					SetSharedVBIndex(unsigned int nGeometry, unsigned int nIndex);

			void							SetNbrCollisionVertices(unsigned int nNbrCollisionVertices);
			void							SetNbrCollisionIndices(unsigned int nNbrCollisionIndices);
			void							SetCollisionVertices(float * pVertices);
			void							SetCollisionIndices(unsigned int * pIndices);

			// Derivation from ENTITY_ABC
			void							Display(DISPLAY_PARAMETERS & DisplayParameters);
			void							DisplayTrans(DISPLAY_PARAMETERS & DisplayParameters, unsigned int nGeometryIndex, unsigned int nSkinClusterIndex);
			void							DisplayHdrObject(DISPLAY_PARAMETERS & DisplayParameters, unsigned int nGeometryIndex, unsigned int nSkinClusterIndex);
			void							DisplaySorted(DISPLAY_PARAMETERS & DisplayParameters, unsigned int nGeometry);
		//	bool							RayCast(RAY_CAST_PARAMETERS & RayCastParameters, bool & bContinue);
			bool							TestVisibility(DISPLAY_PARAMETERS & DisplayParameters);
			float							GetDistanceFromCamera(unsigned int nGeometry, NODE * pNode);
			bool							TestGeometriesVisibility(DISPLAY_PARAMETERS &	DisplayParameters);

			// Derivation from 
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

		private:

			void							CreateGeometries();

			static	void					EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void					EM_Destruct(void * const pO, void * const pR, void * const pP);
		#ifndef _MASTER_ONLY_DATABANK
			static	void					CM_ReadChunk0(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void					CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void					CM_ReadChunk2(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void					CM_ReadChunk3(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void					CM_ReadChunk4(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static	void					CM_ReadChunk5(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static  void					CM_ReadChunk6(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif
		#ifndef _MASTER	
			static	void					CM_WriteChunk6(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		#endif

			unsigned int					_nNbrSharedVertexBuffers;
		//	GEOMETRY_ABC **					_pSharedVertexBuffers;

			unsigned int					_nNbrGeometries;
			MESH_GROUP_GEOMETRY	*			_pGeometries;

			bool *							_pbVisibleGeometries;
			unsigned int *					_nSharedVertexBufferIndex;

			//AABB_TREE						_Tree;

			// Buffer used to build collision data for the physic engine
			// (this replace the AABB tree)
			AABB							_BoundingBox;
			unsigned int					_nNbrCollisionVertices;
			unsigned int					_nNbrCollisionIndices;
			float *							_pCollisionVertices;
			unsigned int *					_pCollisionIndices;

			I_PHYSIC_STATIC_ACTOR *			_pStaticActor;
			I_PHYSIC_SHAPE_MESH_DATA *		_pMeshData;
		};
	}
}

#ifndef _DEBUG
	#include	"MeshGroup.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUP_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
