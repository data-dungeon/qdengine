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
//	CLASS:	BLEND_SHAPE_PERIODIC_BLINK
//	The BLEND_SHAPE_PERIODIC_BLINK class implements a blink every frame period.
//	You can add a random composant to the period.
//
//	01-12-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BLEND_SHAPE_PERIODIC_BLINK_H__
#define __M3D_BLEND_SHAPE_PERIODIC_BLINK_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/EntityBehaviorABC)

namespace QDT
{
	namespace M3D
	{
		class	BLEND_SHAPE_PERIODIC_BLINK : public ENTITY_BEHAVIOR_ABC
		{
		public:

			/** Constructor & destructor */
			BLEND_SHAPE_PERIODIC_BLINK(int nTargetBlend, float rTargetWeight, float rDuration, float rPeriod, float rRandom);
			~BLEND_SHAPE_PERIODIC_BLINK();

			/** Accessors */
			int					GetTargetBlend() const;

			/** Modifiors */
			void				SetTargetBlend(int nTargetBlend);
			void				SetTargetWeight(float rTargetWeight);
			void				SetDuration(float rDuration);
			void				SetPeriod(float rPeriod);
			void				SetRandom(float rRandom);

			/** Derivation from ENTITY_BEHAVIOR_ABC */
			virtual void		UpdateData(float rSpeedFactor);
			virtual void		UpdateController(ENTITY_CONTROLLER_ABC *pController) const;
			virtual bool		IsBlendShapePeriodicBlink() const;

		private:

			void				ComputeNextBlink();

			int					_nTargetBlend;
			float				_rWeight;
			float				_rDuration;
			float				_rTargetWeight;
			float				_rPeriod;
			float				_rRandom;
			float				_rNextBlink;	// In frame per second
		};
	}
}

#endif // __M3D_BLEND_SHAPE_PERIODIC_BLINK_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
