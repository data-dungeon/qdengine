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
//	CLASS:	SPOT_LIGHT
//	The SPOT_LIGHT class implements ...
//
//	01-07-03:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_LIGHT_SPOTLIGHT_H__
#define __M3D_SCENEGRAPH_ENTITIES_LIGHT_SPOTLIGHT_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(SceneGraph/Entities/Light/LightABC)
#include	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Cone/Cone)

namespace QDT
{
	namespace M3D
	{
		class SPOT_LIGHT : public LIGHT_ABC
		{
		public:
			enum
			{
				 CMD_SPOTLIGHT_SETCOLOR = CMD_LIGHTABC_LAST,
				 CMD_SPOTLIGHT_SETOUTERANGLE,
				 CMD_SPOTLIGHT_SETINNERANGLE,
				 CMD_SPOTLIGHT_SETINTENSITY,
				 CMD_SPOTLIGHT_SETAMBIENT,
				 CMD_SPOTLIGHT_SETDIFFUSE,
				 CMD_SPOTLIGHT_SETSPECULAR,
				 CMD_SPOTLIGHT_SETRADIUS,
				 CMD_SPOTLIGHT_SETATTENUATIONPOWER,
				 CMD_SPOTLIGHT_SETSHADOWSCALE,

				 CMD_SPOTLIGHT_LAST,
			};

			SPOT_LIGHT();
			virtual ~SPOT_LIGHT();

			// Operators
			SPOT_LIGHT &	operator=(const SPOT_LIGHT & C);

			virtual void	Instanciate(COM_INSTANCE & Instance);
			virtual void	InstanciateRec(COM_INSTANCE & Instance);

			// Test if the light can lit the mesh
			virtual bool TestLighting(const SPHERE & BSphere) const;
			virtual void UpdateGlobalPos();

			virtual bool TestVisibility(const FRUSTRUM & Frustum);

			// Accessors
			Float32		GetOuterAngle()	const								{ return (_LightSource._rSpotOuterAngle); }
			Float32		GetInnerAngle()	const								{ return (_LightSource._rSpotInnerAngle); }	
			void		GetColor(Float32 &r, Float32 &g, Float32 &b) const	{ r = _LightSource._rColorR; g = _LightSource._rColorG; b = _LightSource._rColorB; }
			Float32		GetIntensity() const								{ return (_LightSource._rIntensity); }
			Float32		GetAmbient() const									{ return (_LightSource._rAmbient); }
			Float32		GetDiffuse() const									{ return (_LightSource._rDiffuse); }
			Float32		GetSpecular() const									{ return (_LightSource._rSpecular); }
			Float32		GetRadius() const									{ return (_BoundingCone.GetRadius()); }
			const CONE& GetGlobalBoundingCone() const						{ return (_GlobalBoundingCone); }
			Float32		GetShadowScale() const								{ return (_fShadowScale); }

			// Modifiors
			void		SetOuterAngle(Float32 rAngle)						{ _LightSource._rSpotOuterAngle = rAngle; _BoundingCone.SetAngle(_LightSource._rSpotOuterAngle); }
			void		SetInnerAngle(Float32 rAngle)						{ _LightSource._rSpotInnerAngle = rAngle; }
			void		SetColor(Float32 r, Float32 g, Float32 b)			{ _LightSource._rColorR = r; _LightSource._rColorG = g; _LightSource._rColorB = b; }
			void		SetIntensity(Float32 i)								{ _LightSource._rIntensity = i; }
			void		SetAmbient(Float32 a)								{ _LightSource._rAmbient = a; }
			void		SetDiffuse(Float32 d)								{ _LightSource._rDiffuse = d; }
			void		SetSpecular(Float32 s)								{ _LightSource._rSpecular = s; }
			void		SetRadius(Float32 r);
			void		SetAttenuationPower(Float32 power)					{ _LightSource._rAttenuationPower = power; }
			void		SetShadowScale(Float32 scale)						{ _fShadowScale = scale; }

			// Derivation from LIGHT_ABC
			virtual void	Display();
			virtual void	UpdateDepthRenderTarget(const VECTOR & CamPosition, SCENE_GRAPH * pSceneGraph);
			
			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			void	Copy(const SPOT_LIGHT & C);

			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static void		CM_ReadChunk7(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk8(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk9(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk10(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk11(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk12(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_ReadChunk13(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk13(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			//lua compatible methods
			static	int		CM_SetColor				(COM_PARAM P);
			static	int		CM_SetOuterAngle		(COM_PARAM P);
			static	int		CM_SetInnerAngle		(COM_PARAM P);
			static	int		CM_SetIntensity			(COM_PARAM P);
			static	int		CM_SetAmbient			(COM_PARAM P);
			static	int		CM_SetDiffuse			(COM_PARAM P);
			static	int		CM_SetSpecular			(COM_PARAM P);
			static	int		CM_SetRadius			(COM_PARAM P);
			static	int		CM_SetAttenuationPower	(COM_PARAM P);
			static	int		CM_SetShadowScale		(COM_PARAM P);

			CONE		_BoundingCone;
			CONE		_GlobalBoundingCone;
			Float32		_fShadowScale;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_LIGHT_SPOTLIGHT_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
