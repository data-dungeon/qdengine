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
//	CLASS:	TOP_ANIM
//
//	02-01-14:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TOP_ANIM constructor
//	Object:		
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
TOP_ANIM::TOP_ANIM()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		TOP_ANIM constructor
//	Object:		
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
TOP_ANIM::TOP_ANIM(const TOP_ANIM & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		TOP_ANIM destructor
//	Object:		
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
TOP_ANIM::~TOP_ANIM()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
TOP_ANIM &	TOP_ANIM::operator=(const TOP_ANIM & C)
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
//	02-01-14:	RMA - Created
//-----------------------------------------------------------------------------
void	TOP_ANIM::Copy(const TOP_ANIM & C)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
