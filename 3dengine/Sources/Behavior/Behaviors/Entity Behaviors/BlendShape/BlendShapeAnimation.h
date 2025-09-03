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
//	CLASS:	BLEND_SHAPE_ANIMATION
//	The BLEND_SHAPE_ANIMATION class implements ...
//
//	01-12-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BLEND_SHAPE_ANIMATION_H__
#define __M3D_BLEND_SHAPE_ANIMATION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(STREAM_HANDLE_BLEND_SHAPE_ANIM)
DECL_3DENGINE(BLEND_SHAPE_ANIM_WRAPPER_ABC)

#include INCL_3DENGINE(Behavior/Behaviors/Entity Behaviors/EntityBehaviorABC)

namespace QDT
{
	namespace M3D
	{
		class	BLEND_SHAPE_ANIMATION : public ENTITY_BEHAVIOR_ABC
		{
		public:

			/** Constructor & destructor */
			BLEND_SHAPE_ANIMATION(int nNbrWeights, const COM_INSTANCE & BSPCInst);
			~BLEND_SHAPE_ANIMATION();

			/** Derivation from ENTITY_BEHAVIOR_ABC */
			virtual void		UpdateData(float rSpeedFactor);
			virtual void		UpdateController(ENTITY_CONTROLLER_ABC *pController) const;
			virtual bool		IsBlendShapeAnimation() const;

			STREAM_HANDLE_BLEND_SHAPE_ANIM *	GetStreamHandle();

			void				SetStartFrame(float rStartFrame);
			void				SetEndFrame(float rEndFrame);
			void				SetBlending(bool b, float rBlendingTime, float * pBlendingWeights);

			void				SetManaged(bool b)			{ _bManaged = b; }
			void				SetInterpolation(bool b)	{ _bInterpolation = b; }

		private:

			BLEND_SHAPE_ANIM_WRAPPER_ABC	*_pBlendShapeAnimWrapper;
			int								_nNbrWeights;
			float							*_prWeights;

			bool							_bManaged;

			bool							_bInterpolation;

			bool							_bBlending, _bSaveBlending;
			float							_rBlendingTime;
			float							*_prBlendingWeights;
		};
	}
}

#endif //__M3D_BLEND_SHAPE_ANIMATION_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
