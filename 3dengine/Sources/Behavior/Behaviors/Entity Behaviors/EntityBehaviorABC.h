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
//	CLASS:	ENTITY_BEHAVIOR_ABC
//	The ENTITY_BEHAVIOR_ABC class implements ...
//
//	01-12-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ENTITY_BEHAVIOR_ABC__
#define __M3D_ENTITY_BEHAVIOR_ABC__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(NODE)
DECL_3DENGINE(ENTITY_CONTROLLER_ABC)
DECL_3DENGINE(BLEND_SHAPE_ANIMATION)
DECL_3DENGINE(BLEND_SHAPE_INTERPOLATION)
DECL_3DENGINE(BLEND_SHAPE_BLINK)
DECL_3DENGINE(BLEND_SHAPE_PERIODIC_BLINK)

#include INCL_KCOM(CommunicatorType/ComInstance)

namespace QDT
{
	namespace M3D 
	{
		class	ENTITY_BEHAVIOR_ABC
		{
		public:

			/** Constructor & destructor */
			ENTITY_BEHAVIOR_ABC();
			virtual					   ~ENTITY_BEHAVIOR_ABC();

			/** Accessors & modifiors */
			NODE*						GetNode() const;
			void						SetNodeInst(const COM_INSTANCE & NodeInst);

			/** Update the behavior */
			void						Update(float rSpeedFactor);
			virtual void				UpdateData(float rSpeedFactor) = 0;

			/** Update the controller of the behavior */
			virtual void				UpdateController(ENTITY_CONTROLLER_ABC *pController) const = 0;

			/** Is the behavior finished ? */
			void						SetFinished(bool b);
			bool						IsFinished() const;

			/** Reset the behavior current frame */
			void						Reset();

			_INLINE_ void				SetSpeed(float rSpeed)	{ _rSpeed = rSpeed; }
			_INLINE_ void				SetLoop(int nNbrLoops)	{ _nNbrLoops = nNbrLoops - 1; }

			/** Is of a certain type ? */
			virtual bool				IsBlendShapeAnimation() const;
			virtual bool				IsBlendShapeInterpolation() const;
			virtual bool				IsBlendShapeBlink() const;
			virtual bool				IsBlendShapePeriodicBlink() const;

			/** Specialize */
			BLEND_SHAPE_ANIMATION*		GetBlendShapeAnimation();
			BLEND_SHAPE_INTERPOLATION*	GetBlendShapeInterpolation();
			BLEND_SHAPE_BLINK*			GetBlendShapeBlink();
			BLEND_SHAPE_PERIODIC_BLINK*	GetBlendShapePeriodicBlink();

		protected:

			void						UpdateCurrentFrame(float rSpeedFactor);

			float						_rCurrentFrame;

			COM_INSTANCE				_NodeInst;
			bool						_bIsFinished;

			float						_rSpeed;
			float						_rStartFrame;
			float						_rEndFrame;
			int							_nNbrLoops;
		};
	}
}

#endif // #define __M3D_ENTITY_BEHAVIOR_ABC__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
