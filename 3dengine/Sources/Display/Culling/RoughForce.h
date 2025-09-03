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
//	CLASS:	ROUGH_FORCE
//	The ROUGH_FORCE class implements ...
//
//	06-01-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_ROUGH_FORCE_H__
#define __DISPLAY_ROUGH_FORCE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_3DENGINE(Display/Culling/ICulling)

DECL_3DENGINE(ENTITY_ABC)

namespace QDT
{
	namespace M3D
	{
		class	ROUGH_FORCE : public I_CULLING
		{
		public:
			ROUGH_FORCE();
			~ROUGH_FORCE();

			// Light management
			const QDT_DLIST<LIGHT_ABC *> &	GetLights() const;
			const QDT_DLIST<LIGHT_ABC *> &	GetVisibleLights() const;
			void							AddLight(LIGHT_ABC * pLight);
			void							RemoveLight(LIGHT_ABC * pLight);

			// Mesh management
			const QDT_DLIST<MESH *> &	GetMeshes() const;
			void						AddMesh(MESH * pMesh);
			void						RemoveMesh(MESH * pMesh);

			// Displayable Debug Entities management
			const QDT_DLIST<ENTITY_ABC *> &	GetDisplayableDebugEntities() const;
			void							AddDisplayableDebugEntity(ENTITY_ABC * pEntity);
			void							RemoveDisplayableDebugEntity(ENTITY_ABC * pEntity);

			// Visibility
			void	ComputeLights(const FRUSTRUM & Frustum, QDT_DLIST<LIGHT_ABC *> & lLightsComputed);
			void	ComputeLightsVisible(const FRUSTRUM & Frustum);
			void	ComputeMeshes(const CONE & BV, QDT_DLIST<MESH *> & lMeshesComputed);
			void	ComputeMeshes(const SPHERE & BV, QDT_DLIST<MESH *> & lMeshesComputed);
			void	ComputeMeshes(const FRUSTRUM & Frustum, QDT_DLIST<MESH *> & lMeshesComputed);
			void	PrepareMeshesToDisplay(const FRUSTRUM & Frustum, UInt32 uiDisplayFlags);
			void	PrepareMeshesToDisplay(const CONE & Cone, UInt32 uiDisplayFlags);

		private:

			QDT_DLIST<LIGHT_ABC *>		_lLights;
			QDT_DLIST<LIGHT_ABC *>		_lVisibleLights;

			QDT_DLIST<MESH *>		_lMeshes;
			QDT_DLIST<ENTITY_ABC *>		_lDisplayableDebugEntities;
		};
	}
}

#endif // __DISPLAY_ROUGH_FORCE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
