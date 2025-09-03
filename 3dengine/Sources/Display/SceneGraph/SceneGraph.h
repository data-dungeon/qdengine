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
// CLASS: SCENE_GRAPH
//
//
//00-10-27 ELE Created: 
//*****************************************************************************

//=============================================================================
// Name: ROOT
//00-10-27 ELE Created: 
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_SCENEGRAPH_H__
#define __M3D_SCENEGRAPH_ENTITIES_SCENEGRAPH_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCOM(COM_INSTANCE)
DECL_3DENGINE(I_CULLING)
DECL_3DENGINE(NODE)
DECL_3DENGINE(MESH)
DECL_3DENGINE(LIGHT_ABC)
DECL_3DENGINE(SHADER_ABC)
DECL_3DENGINE(TRIGGER)
DECL_3DENGINE(AMBIENT_OCTREE)
DECL_3DENGINE(RAY_CAST_PARAMETERS)
DECL_KPHYSICS(I_PHYSIC_SCENE)

#include INCL_KCORE(Template/QDT_DList)
#include INCL_KCOM(CommunicatorType/ComInstance)
#include INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include INCL_KMATH(Math/SphericalHarmonics/SphericalHarmonics)
#include INCL_KMATH(Math/Color/Color)
#include INCL_KMATH(Math/Color/ColorFloat)
#include INCL_3DENGINE(Display/TransparentObject/TransparentObject)
#include INCL_3DENGINE(SceneGraph/Entities/PrimitiveSet/PrimitiveSet)
#include INCL_KPHYSICS(Physics/Enums)

namespace QDT
{
	namespace M3D
	{
		class SCENE_GRAPH
		{
		public:

			typedef enum PRIMITIVE_TYPE
			{
				BLOCK_BOUNDING_PLANES,
				BLOCK_BOUNDING_SPHERE,
				BLOCK_NB_PRIMITIVE_TYPES,
			};

			// Constructor & destructor
    										SCENE_GRAPH();
			virtual							~SCENE_GRAPH();	

			/** Accessors */
			_INLINE_ const COLOR_FLOAT &	GetAmbientColor() const;
			_INLINE_ const SPHERE &			GetBoundingSphere() const;
			_INLINE_ const AABB &			GetBoundingBox()const;
			_INLINE_ bool					IsFogEnable()const;
			_INLINE_ float					GetFogNear()const;
			_INLINE_ float					GetFogFar()const;
			_INLINE_ const COLOR &			GetFogColor()const;

			/** Modifiors */
			_INLINE_ void	SetAmbientColor(const COLOR_FLOAT & Color);
			_INLINE_ void	SetBoundingSphere(const SPHERE & Sphere);
			_INLINE_ void	SetBoundingBox(const AABB & Box);
			_INLINE_ void	SetFogEnable(bool b);
			_INLINE_ void	SetFogNear( float rval );
			_INLINE_ void	SetFogFar( float rval);
			_INLINE_ void	SetFogColor( const COLOR & C);

			virtual	void	InitParameters(float rFogDec);
			virtual	void	ResetParameters();

			// Hirarchy
			void	AddEntity(const COM_INSTANCE & EntityInst);
			void	AddEntity(const COM_INSTANCE & EntityInst, const COM_INSTANCE & ParentNodeInst);
			void	AddCatalog(const COM_INSTANCE & CatalogInst);
			void	RemoveEntity(const COM_INSTANCE & EntityInst);
			void	RemoveEntityKeepHierarchy(const COM_INSTANCE & EntityInst);
			void	RemoveCatalog(const COM_INSTANCE & CatalogInst);
			void	RemoveCatalogKeepHierarchy(const COM_INSTANCE & CatalogInst);

			// Light management
			_INLINE_ const QDT_DLIST<LIGHT_ABC *> &	GetLights() const;
			_INLINE_ const QDT_DLIST<LIGHT_ABC *> &	GetVisibleLights() const;
			_INLINE_ void							AddLight(LIGHT_ABC * pLight);
			_INLINE_ void							RemoveLight(LIGHT_ABC * pLight);

			// ambient octree
			_INLINE_ void						AddAmbientOctree(AMBIENT_OCTREE * pAmbientOctree);
			_INLINE_ UInt32						GetAmbientOctreesNb() const { return _lAmbientOctree.GetSize(); }
			_INLINE_ void						RemoveAmbientOctree(AMBIENT_OCTREE * pAmbientOctree);
			_INLINE_ void						ClearAmbientOctrees();

			// Meshes
			_INLINE_ const QDT_DLIST<MESH *> &	GetMeshes() const;
			_INLINE_ void						AddMesh(MESH * pMesh);
			_INLINE_ void						RemoveMesh(MESH * pMesh);

			// Displayable Debug Entities management
			_INLINE_ const QDT_DLIST<ENTITY_ABC *> &	GetDisplayableDebugEntities() const;
			_INLINE_ void								AddDisplayableDebugEntity(ENTITY_ABC * pEntity);
			_INLINE_ void								RemoveDisplayableDebugEntity(ENTITY_ABC * pEntity);

			_INLINE_ PRIMITIVE_SET*						GetTemporaryPrimitive(void);

			// Shaders
			void	AddShader(SHADER_ABC * pShader);

			// Transparency
			void	AddTransparentObject(const TRANSPARENT_OBJECT & TransObj);

			// Primitives
			void	AddPrimitive(PRIMITIVE_SET * pPrimitive);
			void	RemovePrimitive(PRIMITIVE_SET * pPrimitive);

			// Triggers
			void	AddTrigger(TRIGGER * pTrigger);
			void	RemoveTrigger(TRIGGER * pTrigger);
			
			// Visibility
			_INLINE_ void	ComputeLights(const FRUSTRUM & Frustum, QDT_DLIST<LIGHT_ABC *> & lLightsComputed);
			_INLINE_ void	ComputeMeshes(const CONE & BV, QDT_DLIST<MESH *> & lMeshesComputed);
			_INLINE_ void	ComputeMeshes(const SPHERE & BV, QDT_DLIST<MESH  *> & lMeshesComputed);
			_INLINE_ void	ComputeMeshes(const FRUSTRUM & Frustum, QDT_DLIST<MESH  *> & lMeshesComputed);


			// Display
					 void	Display();
					 void   Display(const CONE & Cone, UInt32 uiDisplayFlags);

			// RayCast
			void	RayCastAllEntities(RAY_CAST_PARAMETERS & RayCastParameters);

			// Physic
			_INLINE_ I_PHYSIC_SCENE *	GetPhysicScene() const;
			void						DisplayPhysics(const MATRIX & ViewMatrix, const MATRIX & ProjectionMatrix) const;
			void						DumpPhysicsToFile(OUTPUT_FILE_FORMAT FileFormat, const char * pFileName);

			bool						Grab(NODE * pNode);
			void						Ungrab(NODE * pNode);

			Bool						GetLocalAmbient(const VECTOR& Pos, SPHERICAL_HARMONICS& WorldSH);

		private:

			void								ComputeLightsVisible();
			void								ComputeShadowMaps();
			void								CleanShadowMaps();

			/** Ambient color of the zone */
			COLOR_FLOAT							_AmbientColor;
				
			//	Bounding box	
			AABB								_BBox;
			SPHERE								_BSphere;
			/*******/

			bool								_bFogEnable, _bOldFogEnable;	//	Enable fog
			float								_rFogNear, _rOldFogNear;		//	fog dist min 
			float								_rFogFar, _rOldFogFar;			//	fog dist max
			COLOR								_FogColor;						//	fog color
			COLOR								_OldFogColor;	

			QDT_VECTOR<SHADER_ABC *>			_vShaders;
			QDT_VECTOR<TRANSPARENT_OBJECT>		_vTransparentObjects;

			QDT_DLIST<PRIMITIVE_SET *>			_lPrimitives;
			QDT_DLIST<TRIGGER *>				_lTriggers;

			QDT_DLIST<AMBIENT_OCTREE *>			_lAmbientOctree;

			I_CULLING *							_pCulling;

			// Physic
			I_PHYSIC_SCENE *					_pPhysicScene;

			//tempo prims
			COM_INSTANCE						_TemporaryPrimitive;
		};
	}
}

#include	INCL_3DENGINE(Display/Culling/ICulling)

#ifndef _DEBUG
	#include	"SceneGraph.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_SCENEGRAPH_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
