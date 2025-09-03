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
//	CLASS:	INPUT_INIT_INFO_PS3
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/PS3/InputInitInfoPS3)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO_PS3 constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PS3::INPUT_INIT_INFO_PS3()
:
_nPort(0),
_nSlot(0)
{
	
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO_PS3 constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PS3::INPUT_INIT_INFO_PS3(const INPUT_INIT_INFO_PS3 & C)
:
INPUT_INIT_INFO(C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO_PS3 destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PS3::~INPUT_INIT_INFO_PS3()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO_PS3 &	INPUT_INIT_INFO_PS3::operator=(const INPUT_INIT_INFO_PS3 & C)
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
void	INPUT_INIT_INFO_PS3::Copy(const INPUT_INIT_INFO_PS3 & C)
{
	_nPort = C._nPort;
	_nSlot = C._nSlot;
}

//-----------------------------------------------------------------------------
//	Name:		SetPort
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_INIT_INFO_PS3::SetPort(const int	nPort)
{
	_nPort = nPort;
}

//-----------------------------------------------------------------------------
//	Name:		GetPort
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_INIT_INFO_PS3::GetPort() const
{
	return	(_nPort);
}


//-----------------------------------------------------------------------------
//	Name:		SetSlot
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_INIT_INFO_PS3::SetSlot(const int	nSlot)
{
	_nSlot = nSlot;
}

//-----------------------------------------------------------------------------
//	Name:		GetSlot
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_INIT_INFO_PS3::GetSlot() const
{
	return	(_nSlot);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
