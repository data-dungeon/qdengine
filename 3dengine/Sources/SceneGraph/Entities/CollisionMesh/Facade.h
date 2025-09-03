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
//	Category:	Maya PluginneGraph\Entities\CollisionMesh\
//
//	03-08-07:	JPE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__MAYA_PLUGINNEGRAPH_ENTITIES_COLLISIONMESH_FACADE_H__
	#define	__MAYA_PLUGINNEGRAPH_ENTITIES_COLLISIONMESH_FACADE_H__

	#include	"..\Facade.h"

	#define		QDT_INCLUDE_MEMORY
	#include	INC_KRNCORE(Includes)

	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Plane)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	INC_KRNMATH(Math/Geometry/Vector2)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"CM_Edge.h"
			#include	"CM_Face.h"
			#include	"CM_Sector.h"
			#include	"CollisionMesh.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__MAYA_PLUGINNEGRAPH_ENTITIES_COLLISIONMESH_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
