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
//	CLASS:	INIFILE_KEY_STRING
//
//	03-01-08:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IniFile/IniFileKeyString)
#include	INCL_KCORE(IniFile/IniFile)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INIFILE_KEY_STRING constructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_STRING::INIFILE_KEY_STRING()
{
}

//-----------------------------------------------------------------------------
//	Name:		~INIFILE_KEY_STRING destructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_STRING::~INIFILE_KEY_STRING()
{
}

//-----------------------------------------------------------------------------
//	Name:		SetValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_STRING::SetValue(const QDT_STRING &	sValue)
{
	_sValue = sValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INIFILE_KEY_STRING::GetValue() const
{
		return ( _sValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_STRING::KEY_TYPE	INIFILE_KEY_STRING::GetType()
{
	return ( KT_STRING );
}
								
//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_STRING::ToString() const
{
	return ( _sValue );
}

//-----------------------------------------------------------------------------
//	Name:		IsValidSubclass
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_STRING::IsValidSubclass() const
{
	if ( _sValue.IsEmpty() == true )
	{
		return ( false );
	}
	else
	{
		return ( true );
	}
}
								
//-----------------------------------------------------------------------------
//	Name:		ReadFromFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_STRING::ReadFromFileSubclass(const INI_FILE &	Inifile)
{
	_sValue = Inifile.GetString( _sSection, _sKey );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		WriteToFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_STRING::WriteToFileSubclass(INI_FILE &	Inifile) const
{
	if ( Inifile.WriteString( _sSection, _sKey, _sValue ) != INI_FILE::NO_ERRORS )
	{
		return ( false );
	}

	return ( true );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
