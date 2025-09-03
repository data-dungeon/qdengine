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
//	Category:	Maya PluginneGraph\Entities\MeshGroup\
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__MAYA_PLUGINNEGRAPH_ENTITIES_MESHGROUP_FACADE_H__
	#define	__MAYA_PLUGINNEGRAPH_ENTITIES_MESHGROUP_FACADE_H__

	#include	"..\Facade.h"

	#include	DEC_KRNPHYSICS(Physics)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"AABB_TreeCell.h"
			#include	"AABB_TreeCellCompressed.h"
			#include	"AABB_Tree.h"
			#include	"MeshGroupGeometry.h"
			#include	"MeshGroup.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__MAYA_PLUGINNEGRAPH_ENTITIES_MESHGROUP_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
