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
//	CLASS:	ENTITY_CONTROLLER_ABC
//	The ENTITY_CONTROLLER_ABC class implements ...
//
//	01-12-18:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ENTITY_CONTROLLER_ABC_H__
#define __M3D_ENTITY_CONTROLLER_ABC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(BLEND_SHAPE_CONTROLLER)
DECL_3DENGINE(ENTITY_BEHAVIOR_ABC)

#include INCL_KCORE(Template/QDT_DList)

namespace QDT
{
	namespace M3D 
	{
		class	ENTITY_CONTROLLER_ABC
		{
		public:
			ENTITY_CONTROLLER_ABC();
			virtual							   ~ENTITY_CONTROLLER_ABC();

			// Update all behaviors 
			virtual void						Update(float rSpeedFactor) = 0;

			bool								IsEmpty() const; 
			void								AddBehavior(ENTITY_BEHAVIOR_ABC *pBehavior);
			void								DeleteBehaviors();
			void								DeleteFinishedBehaviors();
			void								DeleteBehavior(ENTITY_BEHAVIOR_ABC *pBehavior);

			virtual bool						IsBlendShapeController() const;

			BLEND_SHAPE_CONTROLLER*				GetBlendShapeController();

		protected:

			QDT_DLIST<ENTITY_BEHAVIOR_ABC*>		_dlBehaviors;
		};
	}
}

#endif // #ifndef __M3D_ENTITY_CONTROLLER_ABC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
