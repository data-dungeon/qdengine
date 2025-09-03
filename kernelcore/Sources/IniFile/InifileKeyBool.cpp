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
//	CLASS:	INIFILE_KEY_BOOL
//
//	03-01-08:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IniFile/IniFileKeyBool)
#include	INCL_KCORE(IniFile/IniFile)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INIFILE_KEY_BOOL constructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_BOOL::INIFILE_KEY_BOOL()
{
}

//-----------------------------------------------------------------------------
//	Name:		~INIFILE_KEY_BOOL destructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_BOOL::~INIFILE_KEY_BOOL()
{
}

//-----------------------------------------------------------------------------
//	Name:		SetValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_BOOL::SetValue(const bool	bValue)
{
	_bValue = bValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_BOOL::GetValue() const
{
	return ( _bValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_BOOL::KEY_TYPE	INIFILE_KEY_BOOL::GetType()
{
	return ( KT_BOOL );
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_BOOL::ToString() const
{
	if ( _bValue == true )
	{
		return ( GetTrueString() );
	}
	else
	{
		return ( GetFalseString() );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_BOOL::IsValidSubclass() const
{
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		ReadFromFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_BOOL::ReadFromFileSubclass(const INI_FILE &	Inifile)
{
	_bValue = Inifile.GetBool( _sSection, _sKey );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		WriteToFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_BOOL::WriteToFileSubclass(INI_FILE &	Inifile) const
{
	if ( Inifile.WriteBool( _sSection, _sKey, _bValue ) != INI_FILE::NO_ERRORS )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetTrueString
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_BOOL::GetTrueString()
{
	return ( "true" );
}

//-----------------------------------------------------------------------------
//	Name:		GetFalseString
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_BOOL::GetFalseString()
{
	return ( "false" );
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
