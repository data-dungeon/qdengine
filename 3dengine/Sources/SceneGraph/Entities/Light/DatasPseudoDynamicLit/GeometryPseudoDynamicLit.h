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
//	CLASS:	GEOMETRY_PSEUDO_DYNAMIC_LIT
//	The GEOMETRY_PSEUDO_DYNAMIC_LIT class implements ...
//
//	02-06-28:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	GEOMETRY_PSEUDO_DYNAMIC_LIT
{
public:

	GEOMETRY_PSEUDO_DYNAMIC_LIT();
	GEOMETRY_PSEUDO_DYNAMIC_LIT(const GEOMETRY_PSEUDO_DYNAMIC_LIT & C);
	~GEOMETRY_PSEUDO_DYNAMIC_LIT();
	//	REMEMBER To make the destructor virtual if 
	//	you create a subclass, or if you have one

	GEOMETRY_PSEUDO_DYNAMIC_LIT &	operator=(const GEOMETRY_PSEUDO_DYNAMIC_LIT & C);
	void	Copy(const GEOMETRY_PSEUDO_DYNAMIC_LIT & C);

private:
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
