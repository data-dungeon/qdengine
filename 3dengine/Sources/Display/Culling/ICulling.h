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
//	CLASS:	ICULLING
//	The ICULLING class implements ...
//
//	06-01-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __DISPLAY_I_CULLING_H__
#define __DISPLAY_I_CULLING_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(FRUSTRUM)
DECL_KMATH(SPHERE)
DECL_KMATH(CONE)
DECL_3DENGINE(ENTITY_ABC)
DECL_3DENGINE(LIGHT_ABC)
DECL_3DENGINE(MESH)

#include	INCL_KCORE(Template/QDT_DList)

namespace QDT
{
	namespace M3D
	{
		class	I_CULLING
		{
		public:

			virtual ~I_CULLING() {};

			// Light management
			virtual const QDT_DLIST<LIGHT_ABC *> &	GetLights() const = 0;
			virtual const QDT_DLIST<LIGHT_ABC *> &	GetVisibleLights() const = 0;
			virtual void							AddLight(LIGHT_ABC * pLight) = 0;
			virtual void							RemoveLight(LIGHT_ABC * pLight) = 0;

			// Mesh management
			virtual const QDT_DLIST<MESH *> &	GetMeshes() const = 0;
			virtual void						AddMesh(MESH * pMesh) = 0;
			virtual void						RemoveMesh(MESH * pMesh) = 0;

			// Displayable Debug Entities management
			virtual const QDT_DLIST<ENTITY_ABC *> &	GetDisplayableDebugEntities() const = 0;
			virtual void							AddDisplayableDebugEntity(ENTITY_ABC * pEntity) = 0;
			virtual void							RemoveDisplayableDebugEntity(ENTITY_ABC * pEntity) = 0;

			// Visisbility
			virtual void	ComputeLights(const FRUSTRUM & Frustum, QDT_DLIST<LIGHT_ABC *> & lLightsComputed) = 0;
			virtual void	ComputeLightsVisible(const FRUSTRUM & Frustum) = 0;
			virtual void	ComputeMeshes(const CONE & Cone, QDT_DLIST<MESH  *> & lMeshesComputed) = 0;
			virtual void	ComputeMeshes(const SPHERE & Sphere, QDT_DLIST<MESH  *> & lMeshesComputed) = 0;
			virtual void	ComputeMeshes(const FRUSTRUM & Frustum, QDT_DLIST<MESH  *> & lMeshesComputed) = 0;
			virtual void	PrepareMeshesToDisplay(const FRUSTRUM & Frustum, UInt32 uiDisplayFlags) = 0;
			virtual void	PrepareMeshesToDisplay(const CONE & Cone, UInt32 uiDisplayFlags) = 0;
		};
	}
}

#endif // __DISPLAY_I_CULLING_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
