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
//	CLASS:	BLEND_SHAPE_CONTROLLER
//	The BLEND_SHAPE_CONTROLLER class implements ...
//
//	01-12-18:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_BLEND_SHAPE_CONTROLLER_H__
#define __M3D_BLEND_SHAPE_CONTROLLER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(BLEND_SHAPE_INTERPOLATION)
DECL_3DENGINE(BLEND_SHAPE_BLINK)	
DECL_3DENGINE(BLEND_SHAPE_PERIODIC_BLINK)	

#include INCL_3DENGINE(Behavior/Controllers/Entity Controllers/EntityControllerABC)

namespace QDT
{
	namespace M3D 
	{
		class	BLEND_SHAPE_CONTROLLER : public ENTITY_CONTROLLER_ABC
		{
		public:

			/** Constructor & destructor */
			BLEND_SHAPE_CONTROLLER(int nNbrWeights);
			~BLEND_SHAPE_CONTROLLER();

			/** Derivation from ENTITY_CONTROLLER_ABC */
			virtual void				Update(float rSpeedFactor);
			virtual bool				IsBlendShapeController() const;

			/** Accessors */
			float*						GetWeights();
			float*						GetBlendingWeights();
			int							GetNbrWeights();
			float						GetWeight(int nTarget);
			BLEND_SHAPE_INTERPOLATION*	GetInterpolation(int nTarget);
			BLEND_SHAPE_BLINK*			GetBlink(int nTarget);
			BLEND_SHAPE_PERIODIC_BLINK*	GetPeriodicBlink(int nTarget);

			void						SetBlending(bool b, float rBlendingTime, float * pBlendingWeights);
			bool						IsBlending() const	{ return (_bBlending); }

		private:

			int							_nNbrWeights;
			float					   *_prWeights;

			bool						_bBlending;
			float						_rBlendingTime;
			float						_rBlendingTimeRef;
			float						*_prBlendingWeights;
		};
	}
}

#endif // __M3D_BLEND_SHAPE_CONTROLLER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
