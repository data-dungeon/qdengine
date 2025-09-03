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
//	CLASS:	PRIMITIVE
//
//	02-12-19:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE constructor
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
PRIMITIVE::PRIMITIVE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE constructor
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
PRIMITIVE::PRIMITIVE(const PRIMITIVE & C)
:
_Color	(C._Color),
_vPoints(C._vPoints),
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PRIMITIVE destructor
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
PRIMITIVE::~PRIMITIVE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-12-19:	RMA - Created
//-----------------------------------------------------------------------------
PRIMITIVE &	PRIMITIVE::operator=(const PRIMITIVE & C)
{
	if	(this != &C)
	{
		_Color	 = C._Color;
		_vPoints = C._vPoints;
	}

	return	(*this);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
