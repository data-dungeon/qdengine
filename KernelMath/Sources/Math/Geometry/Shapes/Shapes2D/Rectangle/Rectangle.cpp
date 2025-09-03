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
//	CLASS:	RECTANGLE
//
//	01-10-22:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Rectangle/Rectangle)

#ifdef _DEBUG
#include	"Rectangle.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
RECTANGLE &	RECTANGLE::operator=(const RECTANGLE & C)
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
//	01-10-22:	GGO - Created
//-----------------------------------------------------------------------------
void	RECTANGLE::Copy(const RECTANGLE & C)
{
	_nLeft		= C._nLeft;
	_nRight		= C._nRight;
	_nTop		= C._nTop;
	_nBottom	= C._nBottom;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
