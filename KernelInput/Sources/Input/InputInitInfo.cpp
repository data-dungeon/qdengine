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
//	CLASS:	INPUT_INIT_INFO
//
//	02-06-24:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/InputInitInfo)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO::INPUT_INIT_INFO()
:
_strDeviceName("unknown")
{
	
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO constructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO::INPUT_INIT_INFO(const INPUT_INIT_INFO & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_INIT_INFO destructor
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO::~INPUT_INIT_INFO()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_INIT_INFO &	INPUT_INIT_INFO::operator=(const INPUT_INIT_INFO & C)
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
void	INPUT_INIT_INFO::Copy(const INPUT_INIT_INFO & C)
{
	_strDeviceName = C._strDeviceName;
}

//-----------------------------------------------------------------------------
//	Name:		SetDeviceName
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_INIT_INFO::SetDeviceName(const QDT_STRING &	strDeviceName)
{
	_strDeviceName = strDeviceName;
}

//-----------------------------------------------------------------------------
//	Name:		GetDeviceName
//	Object:		
//	02-06-24:	DAC - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INPUT_INIT_INFO::GetDeviceName() const
{
	return	(_strDeviceName);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
