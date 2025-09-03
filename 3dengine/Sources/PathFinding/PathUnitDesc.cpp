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
//	CLASS:	PATH_UNIT_DESC
//
//	02-04-08:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include	INC_3DENGINE(M3D)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

PATH_UNIT_DESC*	PATH_UNIT_DESC::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		PATH_UNIT_DESC constructor
//	Object:		
//	02-04-09:	RMA - Created
//-----------------------------------------------------------------------------
PATH_UNIT_DESC::PATH_UNIT_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_PATH_UNIT, 
	 "PATH_UNIT", 
	 false, 
	 NBR_CMDS, 
	 "Unit of the path finding system. This unit can move in the quadtree avoiding obstacles.")
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		~PATH_UNIT_DESC destructor
//	Object:		
//	02-04-09:	RMA - Created
//-----------------------------------------------------------------------------
PATH_UNIT_DESC::~PATH_UNIT_DESC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	02-04-09:	RMA - Created
//-----------------------------------------------------------------------------
DESC *	PATH_UNIT_DESC::Instance()
{
	if (_pInstance == NULL)
	{
		_pInstance = (new PATH_UNIT_DESC());
	}
	
	return (_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	02-04-09:	RMA - Created
//-----------------------------------------------------------------------------
void	PATH_UNIT_DESC::Suicide()
{
	QDT_ASSERT(_pInstance != NULL);
	
	if (_pInstance != NULL)
	{
		delete _pInstance;
		_pInstance = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReceiveCommandProc
//	Object:		
//	02-04-09:	RMA - Created
//-----------------------------------------------------------------------------
bool	PATH_UNIT_DESC::ReceiveCommandProc(void *				pObject,
										   COMMAND_ID			CommandID,
										   PARAMETER_STACK &	Stack) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		New
//	Object:		
//	02-04-09:	RMA - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	PATH_UNIT_DESC::New()
{
	_nNbObjects++;
	return (new PATH_UNIT);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
