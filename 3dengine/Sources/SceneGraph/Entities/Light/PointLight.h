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
//	CLASS:	POINT_LIGHT
//	The POINT_LIGHT class implements ...
//
//	01-07-03:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_LIGHT_POINTLIGHT_H__
#define __M3D_SCENEGRAPH_ENTITIES_LIGHT_POINTLIGHT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/Light/LightABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)

namespace QDT
{
	namespace M3D
	{
		class POINT_LIGHT : public LIGHT_ABC
		{
		public :

			POINT_LIGHT();
			virtual ~POINT_LIGHT();

			// Operators
			POINT_LIGHT &	operator=(const POINT_LIGHT & C);

			virtual void	Instanciate(COM_INSTANCE & Instance);
			virtual void	InstanciateRec(COM_INSTANCE & Instance);

			// Test if the light can lit the mesh
			virtual bool TestLighting(const SPHERE & BSphere) const;
			virtual void UpdateGlobalPos();

			virtual bool TestVisibility(const FRUSTRUM & Frustum);

			// Accessors
			void			GetColor(Float32 &r, Float32 &g, Float32 &b) const	{ r = _LightSource._rColorR; g = _LightSource._rColorG; b = _LightSource._rColorB; }
			Float32			GetIntensity() const								{ return (_LightSource._rIntensity); }
			Float32			GetAmbient() const									{ return (_LightSource._rAmbient); }
			Float32			GetDiffuse() const									{ return (_LightSource._rDiffuse); }
			Float32			GetSpecular() const									{ return (_LightSource._rSpecular); }
			Float32			GetRadius() const									{ return (_BoundingSphere.GetRadius()); }
			const SPHERE &	GetGlobalBoundingSphere() const						{ return (_GlobalBoundingSphere); }

			// Mutators
			void			SetColor(Float32 r, Float32 g, Float32 b)			{ _LightSource._rColorR = r; _LightSource._rColorG = g; _LightSource._rColorB = b; }
			void			SetIntensity(Float32 i);
			void			SetAmbient(Float32 a)								{ _LightSource._rAmbient = a; }
			void			SetDiffuse(Float32 d)								{ _LightSource._rDiffuse = d; }
			void			SetSpecular(Float32 s)								{ _LightSource._rSpecular = s; }
			void			SetRadius(Float32 r);
			void			SetAttenuationPower(Float32 power)					{ _LightSource._rAttenuationPower = power; }

			// Derivation from LIGHT_ABC
			virtual void Display();
			virtual void UpdateDepthRenderTarget(const VECTOR & CamPosition, SCENE_GRAPH * pSceneGraph);			

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		protected :

			void	Copy(const POINT_LIGHT & C);

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void		CM_ReadChunk7(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk8(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk9(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk10(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk11(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk11(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			SPHERE	_BoundingSphere;
			SPHERE	_GlobalBoundingSphere;
		};
	}
}

#ifndef _DEBUG
	#include "PointLight.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_LIGHT_POINTLIGHT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
