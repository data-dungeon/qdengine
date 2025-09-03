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
//	CLASS:	PATH_FIND_SOLVER
//
//	02-07-11:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_3DENGINE(SceneGraph\Node)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

PATH_FIND_SOLVER*	PATH_FIND_SOLVER::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		PATH_FIND_SOLVER constructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_SOLVER::PATH_FIND_SOLVER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		PATH_FIND_SOLVER destructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_SOLVER::~PATH_FIND_SOLVER()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_SOLVER *	PATH_FIND_SOLVER::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = new PATH_FIND_SOLVER;
	}

	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FIND_SOLVER::Release()
{
	if (_pInstance)
	{
		delete _pInstance;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindUnitSector
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
COM_INSTANCE *	PATH_FIND_SOLVER::FindUnitSector(COM_HANDLE *pUnitHandle)
{
	// We get the list of all sectors
	const QDT_DLIST<COM_INSTANCE> &dlSectors = NODE_MANAGER::Instance()->GetQuadtrees();

	// Make sure we have sectors loaded
	if (dlSectors.IsEmpty()) return (NULL);

	QDT_DLIST<COM_INSTANCE>::ITERATOR itSector	= dlSectors.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR itTail	= dlSectors.GetTail();
	QUADTREE	*pSector = NULL;

	for(; itSector != itTail ; ++itSector)
	{
		if (pSector->Contain(pUnitHandle))
		{
			return (&(*itSector));
		}
	}

	return (NULL);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
