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
//	Category:	Maya PluginhFinding\
//
//	01-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_PATHFINDING_FACADE_H__
	#define	__MAYA __3DENGINE_SOURCES_PATHFINDING_FACADE_H__

	#include	"..\Facade.h"

	#include	INC_KRNCOM(OldCom/Communication)
	#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
	#include	INC_KRNCOM(CommunicatorType)	

	#include	DEC_3DENGINE(SceneGraph/Entities/Path)
	#include	INC_3DENGINE(SceneGraph/Entities/Quadtree)

	#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes2D/Circle)

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"PathNode.h"
			#include	"ClosedList.h"
			#include	"OpenList.h"
			#include	"AStar.h"
			#include	"PathFinder.h"
			#include	"PathFindUnit.h"
			#include	"PathFindSolver.h"
		}
	}

#endif	// __3DENGINE_SOURCES_PATHFINDING_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
