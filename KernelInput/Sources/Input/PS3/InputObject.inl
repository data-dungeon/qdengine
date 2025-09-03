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
//	05-03-17:	SBE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetState
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
void	INPUT_OBJECT::SetState(Float32	nState)
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
void	INPUT_OBJECT::SetReadHandle(int	nRH)
{
	_nReadHandle = nRH;
}

//-----------------------------------------------------------------------------
//	Name:		GetReadHandle
//	Object:		
//	02-06-26:	DAC - Created
//-----------------------------------------------------------------------------
int	INPUT_OBJECT::GetReadHandle() const 
{
	return _nReadHandle;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
