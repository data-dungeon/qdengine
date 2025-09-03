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
//	CLASS:	INIFILE_KEY_INT
//
//	03-01-08:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IniFile/IniFileKeyInt)
#include	INCL_KCORE(IniFile/IniFile)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INIFILE_KEY_INT constructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_INT::INIFILE_KEY_INT()
:
_nValue	( -1 )
{
}

//-----------------------------------------------------------------------------
//	Name:		~INIFILE_KEY_INT destructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_INT::~INIFILE_KEY_INT()
{
}

//-----------------------------------------------------------------------------
//	Name:		SetValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_INT::SetValue(const int	nValue)
{
	_nValue = nValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
int	INIFILE_KEY_INT::GetValue() const
{
	return ( _nValue );
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_INT::KEY_TYPE	INIFILE_KEY_INT::GetType()
{
	return ( KT_INT );
}

						
//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_INT::ToString() const
{
	QDT_STRING	sValue;

	sValue.Format( "%d", _nValue );

	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		IsValidSubclass
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_INT::IsValidSubclass() const
{
	if ( _nValue < 0 )
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
bool	INIFILE_KEY_INT::ReadFromFileSubclass(const INI_FILE &	Inifile)
{
	_nValue = Inifile.GetInt( _sSection, _sKey );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		WriteToFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_INT::WriteToFileSubclass(INI_FILE &	Inifile) const
{
	if ( Inifile.WriteInt( _sSection, _sKey, _nValue ) != INI_FILE::NO_ERRORS )
	{
		return ( false );
	}

	return ( true );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
