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
//	CLASS:	QUADTREE
//
//	01-07-23:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetNbrCells
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
int	QUADTREE::GetNbrCells() const
{
	return (_nNbrCells);
}

//-----------------------------------------------------------------------------
//	Name:		GetDepth
//	Object:		
//	01-07-23:	RMA - Created
//-----------------------------------------------------------------------------
int	QUADTREE::GetDepth() const
{
	return (_nDepth);
}

//-----------------------------------------------------------------------------
//	Name:		SetRootCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetRootCell(const QUADTREE_CELL * const pRootCell)
{
	if (pRootCell)
	{
		_pRootCell = new QUADTREE_CELL;
		*_pRootCell = *pRootCell;
	}
	else
		_pRootCell = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrCells
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetNbrCells(int	nNbrCells)
{
	_nNbrCells = nNbrCells;
}

//-----------------------------------------------------------------------------
//	Name:		SetDepth
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetDepth(int	nDepth)
{
	_nDepth = nDepth;
}

//-----------------------------------------------------------------------------
//	Name:		SetHeight
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::SetHeight(float	rHeight)
{
	_rHeight = rHeight;
}

//-----------------------------------------------------------------------------
//	Name:		GetRootCell
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE::GetRootCell() const
{
	return	(_pRootCell);
}

//-----------------------------------------------------------------------------
//	Name:		FindByPos
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE::FindCellByPos(const POINT2D &	Pos)
{
	return (_pRootCell->FindByPos(Pos));
}

//-----------------------------------------------------------------------------
//	Name:		ComputeNeighbours
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE::ComputeNeighbours()
{
	_pRootCell->UpdateNeightbour();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
