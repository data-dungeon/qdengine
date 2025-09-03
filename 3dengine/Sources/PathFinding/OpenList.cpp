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
//	CLASS:	OPEN_LIST
//
//	02-04-08:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		OPEN_LIST constructor
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
OPEN_LIST::OPEN_LIST()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		OPEN_LIST destructor
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
OPEN_LIST::~OPEN_LIST()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	OPEN_LIST::Clear()
{
	_nNbrPathNode = 0;
	_mhOpenList.Reset();
}

//-----------------------------------------------------------------------------
//	Name:		Insert
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE *	OPEN_LIST::Insert(const PATH_NODE &	Node)
{
	QDT_ASSERT(_nNbrPathNode + 1 < MAX_PATH_NODE);
	
	_tPathNode[_nNbrPathNode] = Node;
	_mhOpenList.Insert(PATH_NODE_PTR(&(_tPathNode[_nNbrPathNode])));
	++_nNbrPathNode;

	return (&_tPathNode[_nNbrPathNode]);
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
int		OPEN_LIST::GetSize()
{
	return (_mhOpenList.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		RemoveMin
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE*	OPEN_LIST::RemoveMin()
{
	return (_mhOpenList.RemoveMin()._pPathNode);
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	OPEN_LIST::IsEmpty()
{
	return (_mhOpenList.GetSize() == 0);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
