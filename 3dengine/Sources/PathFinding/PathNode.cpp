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

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include	"Include.h"
#include	INC_3DENGINE(SceneGraph\Entities\Quadtree)

#include	INC_KRNCORE(Math/Geometry/Distance)

#ifdef _DEBUG
	#include	"PathNode.inl"
#else
	#include	"Inline.h"

	#include	INL_KRNCORE(Math/Geometry/Distance)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/AABR2D)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PATH_NODE constructor
//	Object:		
//	01-10-02:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE::PATH_NODE()
:
_pQuadCell		(NULL),
_rCost			(0.0f),
_rEstimate		(0.0f),
_pPrevPathNode	(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PATH_NODE constructor
//	Object:		Constructor for the first PATH_NODE
//	Note:		SrcPos is inside pCell
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE::PATH_NODE(QUADTREE_CELL *pCell,
					 const POINT2D &SrcPos,
					 const POINT2D &DstPos)
:
_pQuadCell		(pCell),
_Position		(SrcPos),
_rCost			(0.0f),
_pPrevPathNode	(NULL)
{
	_rEstimate = DISTANCE2D::DistancePointPoint(_Position, DstPos);
}

//-----------------------------------------------------------------------------
//	Name:		PATH_NODE constructor
//	Object:		Constructor for the second PATH_NODE & the nexts
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE::PATH_NODE(QUADTREE_CELL *pCell, 
					 PATH_NODE	   *pPrevPathNode,
					 const POINT2D &Dest)
:
_pQuadCell		(pCell),
_Position		(pCell->GetXCenter(), pCell->GetYCenter()),
_pPrevPathNode	(pPrevPathNode)
{
	_rEstimate = DISTANCE2D::DistancePointPoint(_Position, Dest);
	_rCost = _pPrevPathNode->GetCost() + DISTANCE2D::DistancePointPoint(_pPrevPathNode->GetPosition(), _Position);
}

//-----------------------------------------------------------------------------
//	Name:		~PATH_NODE destructor
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE::~PATH_NODE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
bool	PATH_NODE::operator == (const PATH_NODE &	PathNode) const
{
	if ((PathNode._rCost + PathNode._rEstimate) == (_rCost + _rEstimate))
	{
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
bool PATH_NODE::operator <	(const PATH_NODE &	PathNode) const
{
	if ((_rCost + _rEstimate) < (PathNode._rCost + PathNode._rEstimate))
	{
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-09-24:	RMA - Created
//-----------------------------------------------------------------------------
bool PATH_NODE::operator > (const PATH_NODE &	PathNode) const
{
	if ((_rCost + _rEstimate) > (PathNode._rCost + PathNode._rEstimate))
	{
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-01:	RMA - Created
//-----------------------------------------------------------------------------
PATH_NODE &	PATH_NODE::operator = (const PATH_NODE & PathNode)
{
	_Position		= PathNode._Position;		
	_pQuadCell		= PathNode._pQuadCell;
	_rCost			= PathNode._rCost;
	_rEstimate		= PathNode._rEstimate;
	_pPrevPathNode	= PathNode._pPrevPathNode;

	return (*this);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
