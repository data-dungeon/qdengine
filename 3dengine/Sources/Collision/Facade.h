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
//	File	:	Facade.h
//
//	03-09-01:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_COLLISION_FACADE_H__
	#define	__3DENGINE_SOURCES_COLLISION_FACADE_H__

	#include	"..\Facade.h"

	#include	INC_KRNCOM(CommunicatorType)
	#include	INC_KRNCOM(ComBasis)

	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/Cylinder)
	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/OBB)

	#include	INC_3DENGINE(SceneGraph/Node)
	#include	INC_3DENGINE(SceneGraph/Entities/CollisionMesh)

	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/Cylinder)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"CollisionNotifier.h"
			#include	"CollisionEntity.h"
			#include	"CollisionManager.h"
		}
	}

#endif	// __3DENGINE_SOURCES_PATHFINDING_FACADE_H__


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
