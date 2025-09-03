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
//	CLASS:	IDEVICE_PARAM
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include 	INCL_KINPUT(Input/IDeviceParam)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IDEVICE_PARAM constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
IDEVICE_PARAM::IDEVICE_PARAM()
{

}

//-----------------------------------------------------------------------------
//	Name:		IDEVICE_PARAM constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
IDEVICE_PARAM::IDEVICE_PARAM(const IDEVICE_PARAM & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		IDEVICE_PARAM destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
IDEVICE_PARAM::~IDEVICE_PARAM()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
IDEVICE_PARAM &	IDEVICE_PARAM::operator=(const IDEVICE_PARAM & C)
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
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	IDEVICE_PARAM::Copy(const IDEVICE_PARAM & C)
{
	QDT_NOT_IMPLEMENTED();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
