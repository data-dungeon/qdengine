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
//	CLASS:	ZONE_DISPLAY_GEOMETRY
//	The ZONE_DISPLAY_GEOMETRY class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESHGROUPGEOMETRY_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESHGROUPGEOMETRY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(DISPLAY_PARAMETERS)
DECL_3DENGINE(NODE)

#include	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include	INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D
	{
		class	MESH_GROUP_GEOMETRY
		{
		public:

			enum
			{
				MGF_DOUBLE_SIDED				= 1 << 0,
				MGF_DO_CLIPPING					= 1 << 1,
				MGF_DO_SHADOW_CLIPPING			= 1 << 2,
				MGF_NEED_LIGHTS_ON_PRELIT		= 1 << 3,
				MGF_NEED_LIGHTS_ON_DYNAMICLIT	= 1 << 4,
				MGF_CULLING_ON_SHADOWING		= 1 << 5,
				MGF_ENTITY_FOG					= 1 << 6,
			};

											MESH_GROUP_GEOMETRY();
										~MESH_GROUP_GEOMETRY();

			void							Display(unsigned int nIndex, DISPLAY_PARAMETERS & DisplayParameters);
			void							DisplaySorted(DISPLAY_PARAMETERS & DisplayParameters);
			void							DisplayHDR();
			void							DisplayGeometry(DISPLAY_PARAMETERS & DisplayParameters);

		//	_INLINE_ GEOMETRY_ABC *			GetGeometry() const;
			_INLINE_ const COM_INSTANCE&	GetMaterial() const;
			_INLINE_ const AABB&			GetBoundingBox() const;
			_INLINE_ const SPHERE			GetBoundingSphere() const;
			_INLINE_ unsigned int			GetSharedVBIndex() const;
			_INLINE_ bool					NeedLightsOnPrelit() const;
			_INLINE_ bool					NeedLightsOnDynamicLit() const;
			_INLINE_ bool					DoClipping() const;
			_INLINE_ bool					DoShadowCulling() const;
			_INLINE_ bool					DoShadowClipping() const;
			_INLINE_ bool					IsDoubleSided() const;
			_INLINE_ bool					GetEntityFog() const;

					void					SetMaterial(const COM_INSTANCE & ciMaterial);
		//	_INLINE_ void					SetGeometry(GEOMETRY_ABC * pGeomtry);
			_INLINE_ void					SetBoundingBox(const AABB& BBox);
			_INLINE_ void					SetBoundingSphere(const SPHERE & BSphere);
			_INLINE_ void					SetNeedLightsOnPrelit(bool b);
			_INLINE_ void					SetNeedLightsOnDynamicLit(bool b);
			_INLINE_ void					SetCullingOnShadowing(bool b);
			_INLINE_ void					SetDoubleSided(bool b);
			_INLINE_ void					SetSharedVBIndex(unsigned int n);
		//	_INLINE_ void					SetSharedVB(GEOMETRY_ABC * pSharedVB);
			_INLINE_ void					SetEntityFog(bool b);

			bool							TestShadowVisibility(DISPLAY_PARAMETERS &	DisplayParameters);
			float							GetDistanceFromCamera();

		private:

		friend class MESH_GROUP;

			bool							IsVisible(DISPLAY_PARAMETERS & DisplayParameters);
			void							FindLights(DISPLAY_PARAMETERS &DisplayParameters, QDT_VECTOR<NODE*> & vLightNodes);
			void							DisplayGeometryNoMaterial(DISPLAY_PARAMETERS &DisplayParameters);
			void							SetLights(DISPLAY_PARAMETERS &DisplayParameters);

			AABB							_BBox;
			SPHERE							_BSphere;

		//	GEOMETRY_ABC				   *_pGeometry;
			COM_INSTANCE					_ciMaterial;

			unsigned int					_nSharedVertexBufferIndex;

			QDT_FLAGS						_Flags;
		};
	}
}

#ifndef _DEBUG
	#include	"MeshGroupGeometry.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_MESHGROUPGEOMETRY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
