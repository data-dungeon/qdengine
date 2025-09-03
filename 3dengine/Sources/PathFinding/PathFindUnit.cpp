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
//	CLASS:	PATH_FIND_UNIT
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

COMMUNICATOR_DESCRIPTOR		PATH_FIND_UNIT::_CD("m3d_path_unit", 
												99, 
												"DDFBC077-D0A0-E951-73B9-470EAPPO713F", 
												sizeof(PATH_FIND_UNIT), 
												NULL, 
												"Class for a path unit");

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-28:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PATH_FIND_UNIT::GetGenericCommunicatorDescriptor()
{
	return (&_CD);
}
	
//-----------------------------------------------------------------------------
//	Name:		PATH_FIND_UNIT constructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_UNIT::PATH_FIND_UNIT()
:
COMMUNICATOR(&_CD),
_piSector(NULL),
_piNode(NULL),
_piPath(NULL),
_bInMove(false)
{
}

//-----------------------------------------------------------------------------
//	Name:		PATH_FIND_UNIT constructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_UNIT::PATH_FIND_UNIT(const PATH_FIND_UNIT & PFU)
:
COMMUNICATOR(&_CD)
{
	Copy(PFU);
}

//-----------------------------------------------------------------------------
//	Name:		PATH_FIND_UNIT destructor
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_UNIT::~PATH_FIND_UNIT()
{
	if (IsInSector())
	{
		GetQuadtree()->UnregisterUnit(GetHandle());
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
PATH_FIND_UNIT &	PATH_FIND_UNIT::operator=(const PATH_FIND_UNIT & PFU)
{
	if	(this != &PFU)
	{
		Copy(PFU);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FIND_UNIT::Copy(const PATH_FIND_UNIT & PFU)
{
	COMMUNICATOR::Copy(PFU);

	_BoundingVolume = PFU._BoundingVolume;
	_bInMove		= PFU._bInMove;
	_piNode			= PFU._piNode;
	_piPath			= PFU._piPath;
	_piSector		= PFU._piSector;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentDirection
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
float	PATH_FIND_UNIT::GetCurrentDirection()
{
	QDT_NOT_IMPLEMENTED();
	return (0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GetDistanceToTarget
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
float	PATH_FIND_UNIT::GetDistanceToTarget()
{
	QDT_NOT_IMPLEMENTED();
	return (0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		GoTo
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FIND_UNIT::GoTo(POINT3D &	Target)
{
	if (!IsInSector())
	{
		FindSector();
	}

	QDT_ASSERT(IsInSector());

	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		FindSector
//	Object:		
//	02-07-11:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_FIND_UNIT::FindSector()
{
	// Must be called when we have lost the sector or at the begining
	QDT_ASSERT(!IsInSector());	

	// We find in which sector is the unit
	COM_INSTANCE *pInstSector = PATH_FIND_SOLVER::Instance()->FindUnitSector(GetHandle());
	QDT_ASSERT(pInstSector);

	if (!pInstSector) return;

	QUADTREE *pSector = static_cast<QUADTREE*>(pInstSector->GetCommunicator());
	QDT_ASSERT(pSector);

	// Register to the sector
	pSector->RegisterUnit(GetHandle());
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
