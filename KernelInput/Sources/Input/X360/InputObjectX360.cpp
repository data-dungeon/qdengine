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
//	CLASS:	INPUT_OBJECT
//
//	02-06-26:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KINPUT(Input/X360/InputObjectX360)

#define QDT_INCLUDE_STRING
#include INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

int INPUT_OBJECT::_nObjTypeNum[INPUT_OBJECT_ABC::MAX_OBJ_TYPE];

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT constructor
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT::INPUT_OBJECT()
:
_nFlags(0), 
_nState(0)
{

}

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT constructor
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT::INPUT_OBJECT(const INPUT_OBJECT & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INPUT_OBJECT destructor
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT::~INPUT_OBJECT()
{

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
INPUT_OBJECT &	INPUT_OBJECT::operator=(const INPUT_OBJECT & C)
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
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::Copy(const INPUT_OBJECT & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		NewDevice
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::NewDevice() 
{
	memset(_nObjTypeNum, 0, INPUT_OBJECT_ABC::MAX_OBJ_TYPE * sizeof(UInt32));
}

//-----------------------------------------------------------------------------
//	Name:		SetState
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::SetState(Float32 nState)
{
	_nState = nState;
}

//-----------------------------------------------------------------------------
//	Name:		GetState
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
Float32	INPUT_OBJECT::GetState() const
{
	return _nState;
}

//-----------------------------------------------------------------------------
//	Name:		SetReadHandle
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::SetReadHandle(UInt32	nRH)
{
	_nReadHandle = nRH;
}

//-----------------------------------------------------------------------------
//	Name:		GetReadHandle
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
UInt32	INPUT_OBJECT::GetReadHandle() const 
{
	return _nReadHandle;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
