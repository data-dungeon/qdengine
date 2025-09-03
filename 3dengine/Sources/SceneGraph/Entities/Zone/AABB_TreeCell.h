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
//	CLASS:	AABB_TREE_CELL
//	The AABB_TREE_CELL class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	AABB_TREE_CELL
{
public:

					 AABB_TREE_CELL();
					~AABB_TREE_CELL();

private:

	// Binary tree (for now)
	AABB_TREE_CELL * _pLeftChild;
	AABB_TREE_CELL * _pRightChild;

	// Indexed triangle list
	int				 _nNbrIndexes;
	short *			 _pIndexes;

};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
