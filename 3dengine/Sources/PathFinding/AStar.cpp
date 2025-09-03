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
//	CLASS:	ASTAR
//
//	02-04-08:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_3DENGINE(SceneGraph\Entities\Path)

#include	INC_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment2D)
#include	INC_KRNCORE(Math/Geometry/Distance)

#ifndef _DEBUG
	#include	"Inline.h"
	
	#include	INL_KRNCORE(Math/Geometry/Distance)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point2D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes2D/AABR2D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Segment2D)

	#include	INL_3DENGINE(Scenegraph/Entities/Quadtree)	
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		ASTAR constructor
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
ASTAR::ASTAR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ASTAR destructor
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
ASTAR::~ASTAR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SetQuadtree
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	ASTAR::SetQuadtree(QUADTREE *	pQuadtree)
{
	_pQuadtree = pQuadtree;
	_ClosedList.SetQuadtree(pQuadtree);
}

//-----------------------------------------------------------------------------
//	Name:		SetSource
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	ASTAR::SetSource(const POINT2D &	Src)
{
	_Source = Src;
}

//-----------------------------------------------------------------------------
//	Name:		SetDestination
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	ASTAR::SetDestination(const POINT2D &	Dst)
{
	_Destination = Dst;
}

//-----------------------------------------------------------------------------
//	Name:		SetRadius
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	ASTAR::SetRadius(float	rRadius)
{
	_rRadius = rRadius;
}

//-----------------------------------------------------------------------------
//	Name:		SetPath
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
void	ASTAR::SetPath(PATH *	pPath)
{
	_pPath = pPath;
}

//-----------------------------------------------------------------------------
//	Name:		Search
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	ASTAR::Search()
{
	QUADTREE_CELL	*pSrcCell = _pQuadtree->FindCellByPos(_Source);
	QUADTREE_CELL	*pDstCell = _pQuadtree->FindCellByPos(_Destination);
	
	if (TestDirectWay())
	{
		_pPath->AddWayPoint(_Destination);
		_pPath->AddWayPoint(_Source);
		return (true);
	}

	// If destination is not valid, we find another destination near this one
	if (_pQuadtree->IntersectWithCircle(_Destination, _rRadius))
	{
		_Destination	= FindNearestValidPos();
		pDstCell		= _pQuadtree->FindCellByPos(_Destination);
	}

	PATH_NODE		*pNearestPathNode = NULL;
	float			 rNearestDistance = DISTANCE2D::DistancePointPoint(_Source, _Destination);
	bool			 bPathFound	= false;
	PATH_NODE		*pCurPNode	= NULL;				// Current path node
	QUADTREE_CELL   *pCurCell	= NULL;				// Current quadtree cell
	int				 nEachSide;
	int				 nEachNeighbours;
	int				 nNbrNeighbours;

	_OpenList.Clear();
	_ClosedList.Clear();

	_OpenList.Insert(PATH_NODE(pSrcCell, _Source, _Destination));

	while (bPathFound == false)
	{
		if (_OpenList.IsEmpty()) break;
		
		// We scan all the node of the OPEN list until we found the destination quad cell
		pCurPNode	= _OpenList.RemoveMin();
		pCurCell	= pCurPNode->GetCell();

		// The PATH_NODE is removed from OPEN list & put in the CLOSED list
		_ClosedList.Insert(*pCurPNode);

		// Look all the neighbour of the PATH_NODE
		for(nEachSide = 0 ; nEachSide < AABR2D::SIDE_MAX ; ++nEachSide)
		{
			nNbrNeighbours = pCurCell->GetNbrNeightbours(static_cast<AABR2D::SIDE>(nEachSide));

			for (nEachNeighbours = 0 ; nEachNeighbours < nNbrNeighbours ; ++nEachNeighbours)
			{
				QUADTREE_CELL *pNeighbourCell = pCurCell->GetNeightbour(static_cast<AABR2D::SIDE>(nEachSide), nEachNeighbours);

				if (pNeighbourCell->GetState() == QUADTREE_CELL::QCS_EMPTY)
				{
					if (!pNeighbourCell->BelongToClosedList())
					{
						if (pNeighbourCell == pDstCell)
						{
							POINT2D		Org = pCurPNode->GetPosition();
							POINT2D		End = _Destination;
							SEGMENT2D	Way(Org, End);

							if (!_pQuadtree->IntersectWithSegment(Way, _rRadius))
							{	
								bPathFound = true;
								break;
							}
						}
						else
						{	
							POINT2D Org = pCurPNode->GetPosition();
							POINT2D End(pNeighbourCell->GetXCenter(), pNeighbourCell->GetYCenter());
							SEGMENT2D Way(Org, End);
							if (!_pQuadtree->IntersectWithSegment(Way, _rRadius))
							{
								PATH_NODE *pPN = _OpenList.Insert(PATH_NODE(pNeighbourCell, pCurPNode, _Destination));
							
								if (rNearestDistance > pPN->GetEstimate())
								{
									rNearestDistance =  pPN->GetEstimate();
									pNearestPathNode =  pPN;
								}
							}
						}
					}
				}
			}
			if (bPathFound == true)	break;		
		}			
	}

	// Build path
	if (bPathFound == true)
	{
		_pPath->AddWayPoint(_Destination);

		while (pCurPNode)
		{
			_pPath->AddWayPoint(pCurPNode->GetPosition());
			pCurPNode = pCurPNode->GetPreviousPathNode();		
		}
	}
	else
	{
		if (pNearestPathNode == NULL)	return (false);

		// The path send us to the nearest position from the target
		while (pNearestPathNode)
		{
			_pPath->AddWayPoint(pNearestPathNode->GetPosition());
			pNearestPathNode = pNearestPathNode->GetPreviousPathNode();		
		}
	}

	_OpenList.Clear();
	_ClosedList.Clear();
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestDirectWay
//	Object:		
//	02-04-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	ASTAR::TestDirectWay()
{
	SEGMENT2D Way(_Source, _Destination);	

	return (!_pQuadtree->IntersectWithSegment(Way, _rRadius));
}

//-----------------------------------------------------------------------------
//	Name:		FindNearestValidPos
//	Object:		This algortihm give the best position used for destination
//				when destination isn't valid
//	02-03-12:	RMA - Created
//-----------------------------------------------------------------------------
POINT2D 	ASTAR::FindNearestValidPos()
{
	QUADTREE_CELL			   *pPosCell		= _pQuadtree->FindCellByPos(_Destination);
	QUADTREE_CELL			   *pTestCell		= NULL;
	QUADTREE_CELL			   *pNeighbourCell	= NULL;

	QDT_VECTOR<QUADTREE_CELL*>	OpenList;
	POINT2D						CandidatePos;

	POINT2D						BestPos(0.0f, 0.0f);
	float						rBestDistance	= FLT_MAX;
	float						rDistance;
	int							nNbrNeighbours;
	int							nEachNeighbour;
	int							nEachSide;
	bool						bFound = false;

	_ClosedList.Clear();

	// We initialize the open list with the cell which contain the pos
	OpenList.PushTail(pPosCell);
	
	// While there is something in the open list
	while (!OpenList.IsEmpty())
	{
		// We get the last elt of the open list
		pTestCell = *OpenList.GetLast();
		OpenList.PopTail();
		
		// For each side
		for (nEachSide = 0 ; nEachSide < AABR2D::SIDE_MAX ; ++nEachSide)
		{
			// For each neighbour
			nNbrNeighbours = pTestCell->GetNbrNeightbours(static_cast<AABR2D::SIDE>(nEachSide));
			
			for (nEachNeighbour = 0 ; nEachNeighbour < nNbrNeighbours ; ++nEachNeighbour)
			{
				pNeighbourCell = pTestCell->GetNeightbour(static_cast<AABR2D::SIDE>(nEachSide), nEachNeighbour);
				
				// If the neighbour is not in the closed list
				if (!pNeighbourCell->BelongToClosedList())
				{
					// We Add it to the closed list
					pNeighbourCell->AddToClosedList();
						
					// If the distance from the pos to this point is better than rBestDistance
					CandidatePos.SetXY(pNeighbourCell->GetXCenter(), pNeighbourCell->GetYCenter());
					rDistance = DISTANCE2D::DistanceSquarePointPoint(_Destination, CandidatePos);
					if (rDistance < rBestDistance)
					{
						// We add it to the open list
						OpenList.PushTail(pNeighbourCell);

						// If this point is valid
						if (_pQuadtree->IntersectWithCircle(CandidatePos, _rRadius))
						{
							// We have a new best distance
							rBestDistance	= rDistance;
							BestPos			= CandidatePos;
							bFound			= true;
						}
					}
				}
			}
		}
	}

	QDT_ASSERT(bFound == true);

	/////////////////////////
	// DICHOTOMIE
	/////////////////////////

	const float rPrecision = 10.0f;	// 10 cm
	POINT2D		WrongPos = _Destination;
	POINT2D		Middle;

	rDistance = DISTANCE2D::DistancePointPoint(BestPos, _Destination);

	while (rDistance > rPrecision)
	{
		rDistance  /= 2.0f;
		Middle		= (BestPos + WrongPos) / 2;

		if (_pQuadtree->IntersectWithCircle(Middle, _rRadius))
		{
			BestPos = Middle;
		}
		else
		{
			WrongPos = Middle;
		}
	}

	return (BestPos);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
