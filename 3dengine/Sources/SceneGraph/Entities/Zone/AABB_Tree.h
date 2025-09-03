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
//	CLASS:	AABB_TREE
//	The AABB_TREE class implements ...
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	AABB_TREE
{
public:

				AABB_TREE();
			   ~AABB_TREE();

private:

	int					_nNbrPoints;
	POINT3D *			_pPoints;
	AABB_TREE_CELL *	_pRoot;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
