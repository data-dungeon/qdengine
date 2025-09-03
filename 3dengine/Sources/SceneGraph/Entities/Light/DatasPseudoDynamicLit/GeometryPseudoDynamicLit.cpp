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
//
//	02-06-28:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GEOMETRY_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_PSEUDO_DYNAMIC_LIT::GEOMETRY_PSEUDO_DYNAMIC_LIT()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GEOMETRY_PSEUDO_DYNAMIC_LIT constructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_PSEUDO_DYNAMIC_LIT::GEOMETRY_PSEUDO_DYNAMIC_LIT(const GEOMETRY_PSEUDO_DYNAMIC_LIT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		GEOMETRY_PSEUDO_DYNAMIC_LIT destructor
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_PSEUDO_DYNAMIC_LIT::~GEOMETRY_PSEUDO_DYNAMIC_LIT()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_PSEUDO_DYNAMIC_LIT &	GEOMETRY_PSEUDO_DYNAMIC_LIT::operator=(const GEOMETRY_PSEUDO_DYNAMIC_LIT & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-06-28:	RMA - Created
//-----------------------------------------------------------------------------
void	GEOMETRY_PSEUDO_DYNAMIC_LIT::Copy(const GEOMETRY_PSEUDO_DYNAMIC_LIT & C)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
