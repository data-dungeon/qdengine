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
//	CLASS:	PATH_NODE
//
//	01-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetPosition
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
const POINT2D &	PATH_NODE::GetPosition() const
{
	return (_Position);
}

//-----------------------------------------------------------------------------
//	Name:		SetPosition
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_NODE::SetPosition(const POINT2D &	Pos)
{
	_Position = Pos;
}

//-----------------------------------------------------------------------------
//	Name:		GetCost
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
float	PATH_NODE::GetCost() const
{
	return (_rCost);
}

//-----------------------------------------------------------------------------
//	Name:		SetCost
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_NODE::SetCost(float	rCost)
{
	_rCost = rCost;
}

//-----------------------------------------------------------------------------
//	Name:		GetEstimate
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
float	PATH_NODE::GetEstimate() const
{
	return (_rEstimate);
}

//-----------------------------------------------------------------------------
//	Name:		SetEstimate
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_NODE::SetEstimate(float	rEstimate)
{
	_rEstimate = rEstimate;
}

//-----------------------------------------------------------------------------
//	Name:		GetCell
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	PATH_NODE::GetCell()
{
	return (_pQuadCell);
}

//-----------------------------------------------------------------------------
//	Name:		SetCell
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_NODE::SetCell(QUADTREE_CELL *	pCell)
{
	_pQuadCell = pCell;
}

//-----------------------------------------------------------------------------
//	Name:		GetPreviousPathNode
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE *	PATH_NODE::GetPreviousPathNode()
{
	return (_pPrevPathNode);
}

//-----------------------------------------------------------------------------
//	Name:		SetPreviousPathNode
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_NODE::SetPreviousPathNode(PATH_NODE *	pPrevPNode)
{
	_pPrevPathNode = pPrevPNode;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
