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
//	CLASS:	PATH_FINDING_MANAGER
//
//	02-07-11:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

PATH_FINDING_MANAGER*	PATH_FINDING_MANAGER::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		PATH_FINDING_MANAGER constructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FINDING_MANAGER::PATH_FINDING_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PATH_FINDING_MANAGER destructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FINDING_MANAGER::~PATH_FINDING_MANAGER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FINDING_MANAGER *	PATH_FINDING_MANAGER::Instance()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Suidice
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FINDING_MANAGER::Suicide()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		PushSector
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FINDING_MANAGER::PushSector(INDEX_INT	*pi)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		RemoveSector
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FINDING_MANAGER::RemoveSector(INDEX_INT	*pi)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
