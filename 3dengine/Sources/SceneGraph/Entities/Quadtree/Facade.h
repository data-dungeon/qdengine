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
//	Category:	Runtimee\Sources\SceneGraph\Entities\Obsolete\
//
//	04-03-04:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__RUNTIMEE_SOURCES_SCENEGRAPH_ENTITIES_OBSOLETE_FACADE_H__
	#define	__RUNTIMEE_SOURCES_SCENEGRAPH_ENTITIES_OBSOLETE_FACADE_H__

	#include	"..\Facade.h"
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes2D/AABR2D)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Triangle2D)

	namespace	QDT
	{
		namespace	M3D
		{
			#include "Quadtree.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__RUNTIMEE_SOURCES_SCENEGRAPH_ENTITIES_OBSOLETE_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
