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
//	CLASS:	GENERIC_TRACK
//
//	02-06-12:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GENERIC_TRACK constructor
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
GENERIC_TRACK::GENERIC_TRACK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GENERIC_TRACK constructor
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
GENERIC_TRACK::GENERIC_TRACK(const GENERIC_TRACK &	Track)
{
	Copy(Track);
}

//-----------------------------------------------------------------------------
//	Name:		~GENERIC_TRACK destructor
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
GENERIC_TRACK::~GENERIC_TRACK()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
GENERIC_TRACK &	GENERIC_TRACK::operator = (const GENERIC_TRACK & Track) 
{
	if (this != &Track)
	{
		Copy(Track);
	}

	return (*this);
}
	
//-----------------------------------------------------------------------------
//	Name:		PushKey
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
void	GENERIC_TRACK::PushKey(const GENERIC_KEY & Key)
{
#ifdef _DEBUG
	if (GetNbrKeys() > 0)
	{
		QDT_ASSERT(_vKeys[GetNbrKeys() -1].GetTime() < Key.GetTime());
	}
#endif

	_vKeys.PushTail(Key);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrKey
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	GENERIC_TRACK::GetNbrKeys()	const
{
	return (_vKeys.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
T	GENERIC_TRACK::GetValue(float	rTime) const
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-06-12:	RMA - Created
//-----------------------------------------------------------------------------
void	GENERIC_TRACK::Copy(const GENERIC_TRACK & Track) const
{
	_vKeys = Track._vKeys;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
