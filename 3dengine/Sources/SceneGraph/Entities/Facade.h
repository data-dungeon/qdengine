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
//	Category:	3DEngine\Sources\SceneGraph\Entities
//
//	01-07-16:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_FACADE_H__

	#include	"..\Facade.h"

	#include	DEC_KRNMATH(Math/Geometry/BoundingVolume/OBB)
	#include	DEC_KRNMATH(Math/Geometry/BoundingVolume/Cylinder)
	#include	DEC_KRNMATH(Math/Geometry/VerticalRay)
	#include	DEC_KRNMATH(Math/Geometry/BoundingVolume)

	#include	INC_KRNMATH(Math/Color)
	#include	INC_KRNMATH(Math/Geometry/More)
	#include	INC_KRNMATH(Math/Geometry/Matrix)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/AABB)
	#include	INC_KRNMATH(Math/Geometry/BoundingVolume)
	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/Sphere)

	#include	INC_3DENGINE(M3D)

	#include	DEC_3DENGINE(SceneGraph/Entities/Particles)
	#include	DEC_3DENGINE(SceneGraph/Node)
	#include	DEC_3DENGINE(SceneGraph/RayCaster)
	#include	DEC_3DENGINE(Display)

	#include	"EntityABC.h"

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"Camera\Camera.h"
			#include	"QdTransform\QdTransform.h"
			#include	"Glow\Glow.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
