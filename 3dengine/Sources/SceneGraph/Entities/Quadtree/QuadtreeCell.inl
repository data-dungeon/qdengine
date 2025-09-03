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
//	CLASS:	QUADTREE_CELL
//
//	01-07-23:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetState
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetState(STATE	eState)
{
	_eState = eState;
}

//-----------------------------------------------------------------------------
//	Name:		SetParentCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetParentCell(QUADTREE_CELL *	pCell)
{
	_pParentCell = pCell;
}

//-----------------------------------------------------------------------------
//	Name:		AddToClosedList
//	Object:		
//	01-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void	QUADTREE_CELL::AddToClosedList()
{
	_bInClosedList = true;
}

//-----------------------------------------------------------------------------
//	Name:		BelongToClosedList
//	Object:		
//	01-09-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	QUADTREE_CELL::BelongToClosedList()
{
	return (_bInClosedList);
}

//-----------------------------------------------------------------------------
//	Name:		SetNorthWestCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetNorthWestCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_NORTH_WEST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_NORTH_WEST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_NORTH_WEST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetNorthEastCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetNorthEastCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_NORTH_EAST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_NORTH_EAST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_NORTH_EAST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSouthWestCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetSouthWestCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_SOUTH_WEST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_SOUTH_WEST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_SOUTH_WEST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSouthEastCell
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::SetSouthEastCell(const QUADTREE_CELL * const pCell)
{
	if (pCell)
	{
		_pChildCell[QCC_SOUTH_EAST_CELL] = new	QUADTREE_CELL;
		*_pChildCell[QCC_SOUTH_EAST_CELL] = *pCell;
	}
	else
	{
		_pChildCell[QCC_SOUTH_EAST_CELL] = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetChildCell
//	Object:		
//	01-08-24:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL *	QUADTREE_CELL::GetChildCell(CARDINAL	eIndex) const
{
	return	(_pChildCell[eIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL* QUADTREE_CELL::operator[](unsigned int	nIndex) const
{
	QDT_ASSERT(nIndex < QCC_MAX);
	if	(nIndex >= QCC_MAX)
	{
		return	(NULL);
	}

	return	(GetChildCell(static_cast<CARDINAL>(nIndex)));
}

//-----------------------------------------------------------------------------
//	Name:		GetTriangle
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
const TRIANGLE2D& QUADTREE_CELL::GetTriangle(unsigned int	index) const
{
	QDT_ASSERT(index<2);
	return _Quad[index];
}

//-----------------------------------------------------------------------------
//	Name:		ComputeQuad
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
void QUADTREE_CELL::ComputeQuad()
{
	POINT2D P0(_rXMin, _rYMin);
	POINT2D P1(_rXMin, _rYMax);
	POINT2D P2(_rXMax, _rYMax);
	POINT2D P3(_rXMax, _rYMin);
	
	_Quad[0] = TRIANGLE2D(P0, P1, P2);
	_Quad[1] = TRIANGLE2D(P0, P2, P3);
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
float	QUADTREE_CELL::GetArea() const
{
	return (MATH::Abs(_Quad[0].GetArea()) + MATH::Abs(_Quad[1].GetArea()));
}

//-----------------------------------------------------------------------------
//	Name:		GetCardinal
//	Object:		
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::CARDINAL	QUADTREE_CELL::GetCardinal(const SIDE	eSide,
												   const bool	bFirst)
{
	switch(eSide)
	{
	default:
		QDT_FAIL();
	case	SIDE_SOUTH:
		if	(bFirst)
		{
			return	(QCC_SOUTH_EAST_CELL);
		}
		else
		{
			return	(QCC_SOUTH_WEST_CELL);
		}
		break;

	case	SIDE_WEST:
		if	(bFirst)
		{
			return	(QCC_SOUTH_WEST_CELL);
		}
		else
		{
			return	(QCC_NORTH_WEST_CELL);
		}
		break;

	case	SIDE_NORTH:
		if	(bFirst)
		{
			return	(QCC_NORTH_EAST_CELL);
		}
		else
		{
			return	(QCC_NORTH_WEST_CELL);
		}
		break;
	case	SIDE_EAST:
		if	(bFirst)
		{
			return	(QCC_SOUTH_EAST_CELL);
		}
		else
		{
			return	(QCC_NORTH_EAST_CELL);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetOppositeSide
//	Object:		
//	01-09-01:	ONA - Created
//-----------------------------------------------------------------------------
QUADTREE_CELL::SIDE	QUADTREE_CELL::GetOppositeSide(const SIDE	eSide)
{
	switch(eSide)
	{
	default:
		QDT_FAIL();
	case	SIDE_NORTH:
		return	(SIDE_SOUTH);
	case	SIDE_EAST:
		return	(SIDE_WEST);
	case	SIDE_SOUTH:
		return	(SIDE_NORTH);
	case	SIDE_WEST:
		return	(SIDE_EAST);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
