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
//	CLASS:	LIGHT_ABC
//	The LIGHT_ABC class implements ...
//
//	01-07-03:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_LIGHT_LIGHTABC_H__
#define __M3D_SCENEGRAPH_ENTITIES_LIGHT_LIGHTABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KMATH(SPHERE)
DECL_KMATH(MATRIX)
DECL_3DENGINE(MESH)
DECL_3DENGINE(SHADOW_RENDERTARGETS)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		enum LIGHT_TYPE
		{
			LIGHT_UNDEFINED = 0,
			LIGHT_POINT,
			LIGHT_SPOT,
			LIGHT_DIRECTIONAL,
			LIGHT_POINT_SHADOW,
			LIGHT_SPOT_SHADOW, // Attention a l'ordre !!
			LIGHT_DIRECTIONAL_SHADOW,
			LIGHT_POINT_HARDWARE_SHADOW,
			LIGHT_SPOT_HARDWARE_SHADOW, 
			LIGHT_DIRECTIONAL_HARDWARE_SHADOW,
		};
		enum SHADOW_BLUR_MODE
		{
			SBM_NONE = 0, 
			SBM_LOW,
			SBM_MEDIUM,
			SBM_HIGH
		};

		struct S_SHADOW_ATTRIBUTES
		{
			// Matrices
			MATRIX					_mView;	
			MATRIX					_mProj;

			// only used for the shadow map generation :
			// precomputed array (export?) of static meshes lighted
			QDT_VECTOR<MESH *>		_lStaticLightedMeshes;
			
			// dynamically computed list of dynamic meshes lighted
			QDT_DLIST<MESH *>		_lLightedMeshes;

			// copie VRAM (ou RAM ?) de la depth render target de ts les objets statiques
			// valable uniquement quand la light est statique
			I_RENDER_TARGET *		_pDepthRenderTargetOfStaticMeshes;
			SHADOW_RENDERTARGETS *	_pAssociatedDepthRenderTargets;

			// Pixel and vertex shaders
			I_PIXEL_SHADER *		_pShadowPassPS;
			I_VERTEX_SHADER *		_pShadowPassVS;			
		};

		class S_LIGHT_SOURCE
		{
		public:
			LIGHT_TYPE				_eType;
			VECTOR					_vPos;
			VECTOR					_vDir;
			Float32					_rColorR, _rColorG, _rColorB;
			Float32					_rIntensity;
			Float32					_rAmbient;
			Float32					_rDiffuse;
			Float32					_rSpecular;
			Float32					_rInvSquareRadius;
			Float32					_rAttenuationPower;
			Float32					_rSpotInnerAngle;
			Float32					_rSpotOuterAngle;
			Float32					_rBias;
			Float32					_rShadowR, _rShadowG, _rShadowB;			
			SHADOW_BLUR_MODE		_nShadowBlurMode;
			Float32					_rShadowBlurRadius;
			S_SHADOW_ATTRIBUTES *	_pShadowAttributes; // dynamic
		};

		class LIGHT_ABC : public ENTITY_ABC
		{
		public :
			enum
			{
				CMD_LIGHTABC_SETSHADOWBLURMODE = CMD_ENTITY_LAST,
				CMD_LIGHTABC_SETSHADOWBLURRADIUS,
				CMD_LIGHTABC_SETSHADOWCOLOR,
				CMD_LIGHTABC_SETBIAS,
				CMD_LIGHTABC_SETCASTSHADOW,

				CMD_LIGHTABC_LAST
			};

			LIGHT_ABC(COMMUNICATOR_DESCRIPTOR *pCD, ENTITY_TYPE eEntityType);
			virtual ~LIGHT_ABC();

			// Derivation from ENTITY_ABC
			virtual void	Register();
			virtual void	Unregister();
			virtual void	Display();

			virtual void UpdateGlobalPos() = 0;
			virtual void UpdateDepthRenderTarget(const VECTOR & CamPosition, SCENE_GRAPH * pSceneGraph) = 0;

			// Test if the light can lit the mesh
			virtual bool TestLighting(const SPHERE & BSphere) const = 0;

			// Switch light ON or OFF
			void Switch(Bool b);
			Bool GetSwitch() const;

			// Cast shadow ON or OFF
			void SetCastShadow(Bool b) { _bCastShadow = b; } // reserved to the export pipeline
			void CastShadow(Bool b);
			Bool GetCastShadow() const { return _bCastShadow; }

			// Bias
			void SetBias(Float32 f) { _LightSource._rBias = f; }
			Float32 GetBias() const	{ return (_LightSource._rBias); }

			// Shadow color
			void SetShadowColor(Float32 r, Float32 g, Float32 b) { _LightSource._rShadowR = r; _LightSource._rShadowG = g;  _LightSource._rShadowB = b; }
			void GetShadowColor(Float32 &r, Float32 &g, Float32 &b) { r = _LightSource._rShadowR; g = _LightSource._rShadowG; b = _LightSource._rShadowB; }

			void				SetShadowBlurMode(SHADOW_BLUR_MODE Mode) { _LightSource._nShadowBlurMode = Mode; }
			SHADOW_BLUR_MODE	GetShadowBlurMode() const { return _LightSource._nShadowBlurMode; }
			void				SetShadowBlurRadius(Float32 BlurRadius) { _LightSource._rShadowBlurRadius = BlurRadius; }
			Float32				GetShadowBlurRadius() const { return _LightSource._rShadowBlurRadius; }

			void CleanShadowRenderTargets();
			QDT_DLIST<MESH *> &	GetLightedMeshes();

			const S_LIGHT_SOURCE & GetLightSource() const { return (_LightSource); }
			LIGHT_TYPE GetType() const { return _LightSource._eType; }
			
			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

		protected :

			void	Copy(const LIGHT_ABC & C);

			// Communication
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);

			//lua compatible methods
			static	int		CM_SetShadowBlurMode	(COM_PARAM P);
			static	int		CM_SetShadowBlurRadius	(COM_PARAM P);
			static	int		CM_SetShadowColor		(COM_PARAM P);
			static	int		CM_SetBias				(COM_PARAM P);
			static	int		CM_SetCastShadow		(COM_PARAM P);

			static const Float32 MAX_RANGE_EPSILON;

			S_LIGHT_SOURCE		_LightSource;
			Bool				_bSwitch;
			Bool				_bCastShadow;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_LIGHT_LIGHTABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


