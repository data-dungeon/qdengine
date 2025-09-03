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
//	CLASS:	CLOSED_LIST
//
//	02-04-08:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#ifndef _DEBUG
	#include	"inline.h"
	#include	INL_3DENGINE(Scenegraph\Entities\Quadtree)	
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CLOSED_LIST constructor
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
CLOSED_LIST::CLOSED_LIST()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~CLOSED_LIST destructor
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
CLOSED_LIST::~CLOSED_LIST()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetQuadtree
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOSED_LIST::SetQuadtree(QUADTREE *pQuadtree)
{
	_pQuadtree = pQuadtree;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOSED_LIST::Clear()
{
	_pQuadtree->GetRootCell()->ResetClosedList();
}

//-----------------------------------------------------------------------------
//	Name:		Insert
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	CLOSED_LIST::Insert(PATH_NODE & PathNode)
{
	PathNode.GetCell()->AddToClosedList();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
