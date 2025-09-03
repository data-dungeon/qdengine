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
//	The ASTAR class implements ...
//
//	02-04-08:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	ASTAR
{
public:

				ASTAR();
			   ~ASTAR();

	void		SetQuadtree(QUADTREE *pQuadtree);
	void		SetSource(const POINT2D &Src);
	void		SetDestination(const POINT2D &Dst);
	void		SetRadius(float rRadius);
	void		SetPath(PATH *pPath);

	bool		Search();

private:

	POINT2D 	FindNearestValidPos();
	bool		TestDirectWay();

	QUADTREE   *_pQuadtree;

	OPEN_LIST	_OpenList;
	CLOSED_LIST _ClosedList;
	
	POINT2D		_Source;
	POINT2D		_Destination;
	float		_rRadius;
	PATH	   *_pPath;
	
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
