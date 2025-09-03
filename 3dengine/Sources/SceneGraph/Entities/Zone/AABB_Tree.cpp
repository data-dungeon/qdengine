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
//	CLASS:	AABB_TREE
//
//	03-09-01:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		AABB_TREE constructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE::AABB_TREE()
:
_nNbrPoints(0),
_pPoints(NULL),
_pRoot(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		AABB_TREE destructor
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
AABB_TREE::~AABB_TREE()
{
	// Do nothing
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
