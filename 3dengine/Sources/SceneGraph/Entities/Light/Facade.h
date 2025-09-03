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
//	Category:	3DEngine\Sources\SceneGraph\Entities\Light
//
//	01-07-16:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_ENTITES_LIGHT_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_ENTITES_LIGHT_FACADE_H__

	#include	"..\Facade.h"
	
	#include	INC_KRNMATH(Math/Geometry/Shapes/Shapes3D/Cone)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"LightABC.h"
			#include	"SpotLight.h"
			#include	"PointLight.h"
			#include	"DirectionalLight.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_LIGHT_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
