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
//	Category:	IAMAPPne\Sources\SceneGraph\Entities\Trigger\
//
//	02-03-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_TRIGGER_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_TRIGGER_FACADE_H__

	#include	"..\Facade.h"
	
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron)
	#include	INC_KRNMATH(Math/Geometry/BoundingVolume/Sphere)
	#include	DEC_KRNRENDERER(Renderer)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"Trigger.h"
		}
	}

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_TRIGGER_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
