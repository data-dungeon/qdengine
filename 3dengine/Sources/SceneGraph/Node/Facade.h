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
//	Category:	3DEngine\Sources\SceneGraph\Entities\Node
//
//	01-07-16:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_ENTITES_NODE_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_ENTITES_NODE_FACADE_H__

	#include	"..\Facade.h"

	#include	DEC_KRNCOM(Catalog)
	#include	DEC_KRNMATH(Math\Geometry\BoundingVolume\AABB)
	#include	DEC_3DENGINE(SceneGraph\Entities\Skeleton)
	
	#include	INC_KRNMATH(Math/Geometry/QuaternionVector)
	#include	INC_KRNMATH(Math/Geometry/Matrix)
	#include	INC_KRNPHYSICS(Physics)
	#include	INC_3DENGINE(SceneGraph\Entities)
	#include	INC_3DENGINE(SceneGraph\RayCaster)

	//using namespace QDT::KCORE;

	#include	"Node.h"

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"SklChild.h"
			#include	"LodHier.h"

			#include	"SklNode.h"
			#include	"NodeManager.h"
			#include	"NodeMapper.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_ENTITIES_NODE_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
