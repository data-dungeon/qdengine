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
//	CLASS:	DIRECTIONAL_LIGHT
//	The DIRECTIONAL_LIGHT class implements ...
//
//	01-07-03:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_LIGHT_DIRECTIONALLIGHT_H__
#define __M3D_SCENEGRAPH_ENTITIES_LIGHT_DIRECTIONALLIGHT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/Light/LightABC)

namespace QDT
{
	namespace M3D
	{
		class DIRECTIONAL_LIGHT : public LIGHT_ABC
		{
		public :

			DIRECTIONAL_LIGHT();
			virtual ~DIRECTIONAL_LIGHT();

			// Operators
			DIRECTIONAL_LIGHT &	operator=(const DIRECTIONAL_LIGHT & C);

			virtual void	Instanciate(COM_INSTANCE & Instance);
			virtual void	InstanciateRec(COM_INSTANCE & Instance);

			// Test if the light can lit the mesh
			virtual bool TestLighting(const SPHERE & BSphere) const;
			virtual bool TestVisibility(const FRUSTRUM & Frustum);

			// Derivation from LIGHT_ABC
			virtual void Display();
			virtual void UpdateGlobalPos();
			virtual void UpdateDepthRenderTarget(const VECTOR & CamPosition, SCENE_GRAPH * pSceneGraph);

			// Accessors
			void			GetColor(Float32 &r, Float32 &g, Float32 &b) const	{ r = _LightSource._rColorR; g = _LightSource._rColorG; b = _LightSource._rColorB; }
			Float32			GetIntensity() const								{ return (_LightSource._rIntensity); }
			Float32			GetDiffuse() const									{ return (_LightSource._rDiffuse); }
			Float32			GetSpecular() const									{ return (_LightSource._rSpecular); }

			// Mutators
			void			SetColor(Float32 r, Float32 g, Float32 b)			{ _LightSource._rColorR = r; _LightSource._rColorG = g; _LightSource._rColorB = b; }
			void			SetIntensity(Float32 i)								{ _LightSource._rIntensity = i; }
			void			SetDiffuse(Float32 d)								{ _LightSource._rDiffuse = d; }
			void			SetSpecular(Float32 s)								{ _LightSource._rSpecular = s; }

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		protected :

			void	Copy(const DIRECTIONAL_LIGHT & C);

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void		CM_ReadChunk7(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);	
			static void		CM_ReadChunk8(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);	
			static void		CM_ReadChunk9(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);	
			static void		CM_WriteChunk9(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_LIGHT_DIRECTIONALLIGHT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
