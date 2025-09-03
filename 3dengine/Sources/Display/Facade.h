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
//	Category:	3DEngine\Sources\Display\
//
//	01-07-16:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_DISPLAY_FACADE_H__
	#define	__3DENGINE_SOURCES_DISPLAY_FACADE_H__

	#include	"../facade.h"

	#include	DEC_KRNMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron)
	#include	DEC_3DENGINE(Display\Viewport)
	
	#include	INC_KRNCORE(Tools)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes3D/Frustrum)
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes2D/Plane)
	#include	INC_3DENGINE(SceneGraph/Entities/Mesh)
	#include	INC_3DENGINE(SceneGraph/Entities/MeshGroup)
	#include	INC_3DENGINE(SceneGraph/Entities/Light)
	#include	INC_3DENGINE(SceneGraph/RayCaster)
#ifdef PLATFORM_PC
	#include	INC_KRNSERV(HardwareConfig)
#endif

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"Statistics\Statistics.h"
			#include    "DisplayParameters\DisplayParameters.h"
			#include	"TransparentObject\TransparentObject.h"
			#include	"HdrObject\HdrObject.h"
			#include	"SortedObject\SortedObject.h"
			#include	"DisplayManager.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_DISPLAY_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

