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
//	CLASS:	ENTITY_CONTROLLER_FACTORY
//	The ENTITY_CONTROLLER_FACTORY class implements ...
//
//	01-12-18:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_ENTITY_CONTROLLER_FACTORY_H__
#define __M3D_ENTITY_CONTROLLER_FACTORY_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_3DENGINE(BLEND_SHAPE_CONTROLLER)
DECL_3DENGINE(NODE)

namespace QDT
{
	namespace M3D 
	{
		class	ENTITY_CONTROLLER_FACTORY
		{
		public:

			static ENTITY_CONTROLLER_FACTORY*	Instance();
			static void							Suicide();

			BLEND_SHAPE_CONTROLLER*				CreateBlendShapeController(int nNbrWeights) const;

			// Create a controller if there is no controller with the node
			void								CreateControllerIfNeed(int nBlendShapeID, NODE *pNode) const;

		private:

			ENTITY_CONTROLLER_FACTORY();
			~ENTITY_CONTROLLER_FACTORY();

			static ENTITY_CONTROLLER_FACTORY   *_pInstance;
		};
	}
}

#endif // __M3D_ENTITY_CONTROLLER_FACTORY_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
