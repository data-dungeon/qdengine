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
//	Category:	IAMAPPne\Sources\SceneGraph\Lightsets\
//
//	02-10-17:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_LIGHTSETS_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_LIGHTSETS_FACADE_H__

	#include	"..\Facade.h"

	#include	DEC_3DENGINE(SceneGraph\Entities\Mesh)
	#include	INC_KRNCORE(Display\GeometryABC)
	#include	INC_KRNCORE(Math\Color)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"GeometryLightTrack.h"
			#include	"MaterialLightTrack.h"
			#include	"MeshLightTrack.h"
			#include	"LightTrack.h"		
			#include	"SeparateLightset.h"
			#include	"LightsetManager.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_LIGHTSETS_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
