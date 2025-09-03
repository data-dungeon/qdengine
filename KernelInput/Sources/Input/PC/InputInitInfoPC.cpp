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
//	CLASS:	INPUT_INIT_INFO_PC
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/PC/InputInitInfoPC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO_PC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PC::INPUT_INIT_INFO_PC()
:
_pDID8(NULL)
{
	
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO_PC constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PC::INPUT_INIT_INFO_PC(const INPUT_INIT_INFO_PC & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO_PC destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PC::~INPUT_INIT_INFO_PC()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PC &	INPUT_INIT_INFO_PC::operator=(const INPUT_INIT_INFO_PC & C)
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
void	INPUT_INIT_INFO_PC::Copy(const INPUT_INIT_INFO_PC & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		SetInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_INIT_INFO_PC::SetInputDevice(LPDIRECTINPUTDEVICE8	pDID8)
{
	_pDID8 = pDID8;
}

//-----------------------------------------------------------------------------
//	Name:		GetInputDevice
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8	INPUT_INIT_INFO_PC::GetInputDevice() const
{
	return	(_pDID8);
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
