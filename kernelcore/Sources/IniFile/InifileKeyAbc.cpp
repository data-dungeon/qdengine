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
//	CLASS:	INIFILE_KEY_ABC
//
//	03-01-08:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IniFile/InifileKeyAbc)
#include	INCL_KCORE(IniFile/IniFile)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INIFILE_KEY_ABC constructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_ABC::INIFILE_KEY_ABC()
{
}

//-----------------------------------------------------------------------------
//	Name:		~INIFILE_KEY_ABC destructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_ABC::~INIFILE_KEY_ABC()
{
}

//-----------------------------------------------------------------------------
//	Name:		SetSectionName
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_ABC::SetSectionName(const QDT_STRING &	sName)
{
	QDT_ASSERT( sName.IsEmpty() == false );

	_sSection = sName;
}

//-----------------------------------------------------------------------------
//	Name:		GetSectionName
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INIFILE_KEY_ABC::GetSectionName() const
{
	return ( _sSection );
}

//-----------------------------------------------------------------------------
//	Name:		SetKeyName
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_ABC::SetKeyName(const QDT_STRING &	sName)
{
	QDT_ASSERT( sName.IsEmpty() == false );

	_sKey = sName;
}

//-----------------------------------------------------------------------------
//	Name:		GetKeyName
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INIFILE_KEY_ABC::GetKeyName() const
{
	return ( _sKey );
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_ABC::IsValid() const
{
	if ( ( _sSection.IsEmpty() == true ) || ( _sKey.IsEmpty() == true ) )
	{
		return ( false );
	}

	return ( IsValidSubclass() );
}

//-----------------------------------------------------------------------------
//	Name:		ReadFromFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_ABC::ReadFromFile(const INI_FILE &	Inifile)
{
	if ( Inifile.DoesSectionAndKeyExist( _sSection, _sKey ) == false )
	{
		return ( false );
	}

	return ( ReadFromFileSubclass( Inifile ) );
}

//-----------------------------------------------------------------------------
//	Name:		WriteToFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_ABC::WriteToFile(INI_FILE &	Inifile) const
{
	if ( IsValid() == false )
	{
		return ( false );
	}

	return ( WriteToFileSubclass( Inifile ) );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
