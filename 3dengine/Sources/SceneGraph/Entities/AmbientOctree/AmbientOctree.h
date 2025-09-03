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
//	CLASS:	AMBIENT_OCTREE
//	AMBIENT_OCTREE class implements a octree filled with ambient lighting data
//	(i.e. spherical harmonics) used to improve ambient lighting of dynamic
//	object.
//
//	06-03-13:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_AMBIENT_OCTREE_AMBIENT_OCTREE_H__
#define __M3D_SCENEGRAPH_ENTITIES_AMBIENT_OCTREE_AMBIENT_OCTREE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(IRRADIANCE_VOLUME)
DECL_KMATH(SPHERICAL_HARMONICS)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)

namespace QDT
{
	namespace M3D
	{
		class AMBIENT_OCTREE : public ENTITY_ABC
		{
		public :

			AMBIENT_OCTREE();
			~AMBIENT_OCTREE();

			// Derivation from ENTITY_ABC
			virtual void	Register();
			virtual void	Unregister();

			void SetIrradianceVolume(IRRADIANCE_VOLUME * pIrrVol);

			void SampleIrradiance(const VECTOR &Pos, SPHERICAL_HARMONICS & sh);
		
			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

		protected :

			// Communication
			static void	EM_Construct(void * const pO, void * const pR, void * const pP);
			static void	EM_Destruct(void * const pO, void * const pR, void * const pP);	
			static void	CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void	CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		
			IRRADIANCE_VOLUME * _pIrradianceVolume;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_AMBIENT_OCTREE_AMBIENT_OCTREE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


