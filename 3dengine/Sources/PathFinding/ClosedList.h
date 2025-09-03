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
//	The CLOSED_LIST class implements ...
//
//	02-04-08:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	CLOSED_LIST
{
public:

				CLOSED_LIST();
			   ~CLOSED_LIST();
	
	void		SetQuadtree(QUADTREE *pQuadtree);
	void		Clear();
	void		Insert(PATH_NODE & PathNode);

private:

	QUADTREE	*_pQuadtree;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
