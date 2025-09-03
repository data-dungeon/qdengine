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
//	Category:	3DEngine\Sources\SceneGraph\Entities\Skeleton\
//
//	02-01-30:	DAC - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_SKELETON_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_SKELETON_FACADE_H__

	#include	"..\Facade.h"

	#include	DEC_3DENGINE(SceneGraph/Entities/Mesh/SoftBody)

	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes1D/Segment)
	#include	INC_KRNMATH(Math/Geometry/Matrix)

	#include	INC_3DENGINE(SceneGraph/Entities/Joint)
	#include	INC_3DENGINE(SceneGraph/Entities/Light)

	namespace QDT
	{
		namespace M3D
		{
			#include	"Bone\Bone.h"
			#include	"Skeleton.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_SKELETON_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
