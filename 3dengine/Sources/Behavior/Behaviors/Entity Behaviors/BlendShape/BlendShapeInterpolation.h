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
//	CLASS:	BLEND_SHAPE_INTERPOLATION
//	The BLEND_SHAPE_INTERPOLATION class implements ...
//
//	01-12-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BLEND_SHAPE_INTERPOLATION_H__
#define __M3D_BLEND_SHAPE_INTERPOLATION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(ENTITY_CONTROLLER_ABC)

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/EntityBehaviorABC)

namespace QDT
{
	namespace M3D
	{
		class	BLEND_SHAPE_INTERPOLATION : public ENTITY_BEHAVIOR_ABC
		{
		public:

			/** Constructor & destructor */
			BLEND_SHAPE_INTERPOLATION(int nTargetBlend, float rTargetWeight, float rFrameDelay);
			~BLEND_SHAPE_INTERPOLATION();

			/** Accessors */
			int					GetTargetBlend() const;

			/** Modifiors */
			void				SetTargetBlend(int nTargetBlend);
			void				SetTargetWeight(float rTargetWeight);
			void				SetDuration(float rDuration);

			/** Derivation from ENTITY_BEHAVIOR_ABC */
			virtual void		UpdateData(float rSpeedFactor);
			virtual void		UpdateController(ENTITY_CONTROLLER_ABC *pController) const;
			virtual bool		IsBlendShapeInterpolation() const;

		private:

			int					_nTargetBlend;
			float				_rWeight;
			float				_rDuration;
			float				_rTargetWeight;
		};
	}
}

#endif // __M3D_BLEND_SHAPE_INTERPOLATION_H__


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
