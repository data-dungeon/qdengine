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
//	CLASS:	TEXT_DATA_LIST_ABC
//
//	05-05-31:	VPI - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(TextDataFile/TextDataListAbc)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TEXT_DATA_LIST_ABC constructor
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
TEXT_DATA_LIST_ABC::TEXT_DATA_LIST_ABC()
{
	//Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		~TEXT_DATA_LIST_ABC destructor
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
TEXT_DATA_LIST_ABC::~TEXT_DATA_LIST_ABC()
{
	//Do Nothing
}

//-----------------------------------------------------------------------------
//	Name:		TEXT_DATA_LIST_ABC constructor
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
TEXT_DATA_LIST_ABC::TEXT_DATA_LIST_ABC(const TEXT_DATA_LIST_ABC &	C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
TEXT_DATA_LIST_ABC &	TEXT_DATA_LIST_ABC::operator=(const TEXT_DATA_LIST_ABC &	C)
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
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
void	TEXT_DATA_LIST_ABC::Copy(const TEXT_DATA_LIST_ABC &	C)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
