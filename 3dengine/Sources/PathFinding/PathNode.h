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
//	The PATH_NODE class implements ...
//
//	01-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_NODE
{
public:

	/** Constructor & destructor */
								PATH_NODE();
								PATH_NODE(QUADTREE_CELL *pCell, const POINT2D & SrcPos, const POINT2D & DstPos);
								PATH_NODE(QUADTREE_CELL *pCell, PATH_NODE *PrevPathNode, const POINT2D & DstPos);
							   ~PATH_NODE();

	/** Operators */
	PATH_NODE &					operator = (const PATH_NODE & PathNode);				
	bool						operator == (const PATH_NODE & PathNode) const;
	bool						operator < (const PATH_NODE & PathNode) const;
	bool						operator > (const PATH_NODE & PathNode) const;

	/** Accessors & Modifiors */
	_INLINE_ const POINT2D &	GetPosition() const;
	_INLINE_ void				SetPosition(const POINT2D & Pos);
	_INLINE_ float				GetCost() const;
	_INLINE_ void				SetCost(float rCost);
	_INLINE_ float				GetEstimate() const;
	_INLINE_ void				SetEstimate(float rEstimate);
	_INLINE_ QUADTREE_CELL*		GetCell();
	_INLINE_ void				SetCell(QUADTREE_CELL* pCell);
	_INLINE_ PATH_NODE*			GetPreviousPathNode();
	_INLINE_ void				SetPreviousPathNode(PATH_NODE *pPrevPNode);

private:

	POINT2D						_Position;		// Position of the way point
	QUADTREE_CELL			   *_pQuadCell;		// Cell of the quadtree
	float						_rCost;			// Squared distance from the source
	float						_rEstimate;		// Squared distance estimated to the destination
	PATH_NODE				   *_pPrevPathNode;	// The PATH_NODE from which we came
};


class	PATH_NODE_PTR
{
public :

	PATH_NODE_PTR(PATH_NODE *Ptr) {_pPathNode = Ptr; }

	bool	operator <  (const PATH_NODE_PTR & PathNodePtr) const { return (*_pPathNode <  *(PathNodePtr._pPathNode)); }
	bool	operator >  (const PATH_NODE_PTR & PathNodePtr) const { return (*_pPathNode >  *(PathNodePtr._pPathNode)); }
	bool	operator == (const PATH_NODE_PTR & PathNodePtr) const { return (*_pPathNode == *(PathNodePtr._pPathNode)); }

	PATH_NODE *_pPathNode;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
