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
//	CLASS:	ENTITY_BEHAVIOR_FACTORY
//	The ENTITY_BEHAVIOR_FACTORY is a factory which manage creation & 
//	application of all entity behaviors.
//
//	01-12-18:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ENTITY_BEHAVIOR_FACTORY_H__
#define __M3D_ENTITY_BEHAVIOR_FACTORY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(BLEND_SHAPE_ANIMATION)
DECL_3DENGINE(BLEND_SHAPE_BLINK)
DECL_3DENGINE(BLEND_SHAPE_PERIODIC_BLINK)
DECL_3DENGINE(BLEND_SHAPE_INTERPOLATION)

#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D 
	{
		class	ENTITY_BEHAVIOR_FACTORY
		{
		public:

			// Singleton
			static ENTITY_BEHAVIOR_FACTORY*	Instance();
			static void						Suicide();

			// Blend Shape
			BLEND_SHAPE_ANIMATION*			CreateBlendShapeAnimation(int nNbrWeights, const COM_INSTANCE & BSPCInst) const;
			BLEND_SHAPE_BLINK*				CreateBlendShapeBlink(int nTargetBlend, float rTargetWeight, float rDuration) const;
			BLEND_SHAPE_PERIODIC_BLINK*		CreateBlendShapePeriodicBlink(int nTargetBlend, float rTargetWeight, float rDuration, float rPeriod, float rRandom) const;
			BLEND_SHAPE_INTERPOLATION*		CreateBlendShapeInterpolation(int nTargetBlend, float rTargetWeight, float rDuration) const;

			BLEND_SHAPE_ANIMATION*			ApplyBlendShapeAnimation(const COM_INSTANCE & NodeInst, const COM_INSTANCE & BSPCInst, bool bBlending, float rBlendingTime, float rSpeed, float rStartFrame, float rEndFrame, int nNbrLoops, int nBlendShapeID) const;
			void							ApplyBlendShapeBlink(const COM_INSTANCE & NodeInst, int nTargetBlend, float rTargetWeight, float rDuration, int nBlendShapeID) const;
			void							ApplyBlendShapePeriodicBlink(const COM_INSTANCE & NodeInst, int nTargetBlend, float rTargetWeight, float rDuration, float rPeriod, float rRandom, int nBlendShapeID) const;
			void							ApplyBlendShapeInterpolation(const COM_INSTANCE & NodeInst, int nTargetBlend, float rTargetWeight, float rDuration, int nBlendShapeID) const;

			void							DeleteBlendShapeAnimation(const COM_INSTANCE & NodeInst, BLEND_SHAPE_ANIMATION * pBSAnim, int nBlendShapeID) const;

			float							GetBlendShapeValue(const COM_INSTANCE & NodeInst, int nTargetBlend, int nBlendShapeID) const;

		private:

			ENTITY_BEHAVIOR_FACTORY();
			~ENTITY_BEHAVIOR_FACTORY();

			static ENTITY_BEHAVIOR_FACTORY *_pInstance;
		};
	}
}

#endif // __M3D_ENTITY_BEHAVIOR_FACTORY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
