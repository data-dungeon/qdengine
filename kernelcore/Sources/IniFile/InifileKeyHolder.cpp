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
//	CLASS:	INIFILE_KEY_HOLDER
//
//	03-01-08:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IniFile/IniFileKeyHolder)

#include	INCL_KCORE(IniFile/IniFileKeyBool)
#include	INCL_KCORE(IniFile/IniFileKeyInt)
#include	INCL_KCORE(IniFile/IniFileKeyString)
#include	INCL_KCORE(IniFile/IniFile)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INIFILE_KEY_HOLDER constructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_HOLDER::INIFILE_KEY_HOLDER()
{
}

//-----------------------------------------------------------------------------
//	Name:		~INIFILE_KEY_HOLDER destructor
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_HOLDER::~INIFILE_KEY_HOLDER()
{
	Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::Clear()
{
	_Keys.ClearAll();
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::Copy(const INIFILE_KEY_HOLDER &	C)
{
	int						nNbrKeys, nKeyIndex;
	INIFILE_KEY_ABC *		pKey;
	INIFILE_KEY_BOOL *		pKeyBool;
	INIFILE_KEY_INT *		pKeyInt;
	INIFILE_KEY_STRING *	pKeyString;


	Clear();

	nNbrKeys = C.GetNbrKeys();
	for ( nKeyIndex = 0; nKeyIndex < nNbrKeys; ++nKeyIndex )
	{
		pKey = C.GetKey( nKeyIndex );

		switch ( pKey->GetType() )
		{
		case INIFILE_KEY_ABC::KT_BOOL:
			pKeyBool = static_cast<INIFILE_KEY_BOOL*>( pKey );
			AddBoolKey( pKeyBool->GetSectionName(), 
						pKeyBool->GetKeyName(), 
						pKeyBool->GetValue() );
			break;

		case INIFILE_KEY_ABC::KT_INT:
			pKeyInt = static_cast<INIFILE_KEY_INT*>( pKey );
			AddIntKey( pKeyInt->GetSectionName(), 
					   pKeyInt->GetKeyName(), 
					   pKeyInt->GetValue() );
			break;

		case INIFILE_KEY_ABC::KT_STRING:
			pKeyString = static_cast<INIFILE_KEY_STRING*>( pKey );
			AddStringKey( pKeyString->GetSectionName(), 
						  pKeyString->GetKeyName(), 
						  pKeyString->GetValue() );
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_HOLDER &	INIFILE_KEY_HOLDER::operator=(const INIFILE_KEY_HOLDER &	C)
{
	Copy( C );

	return ( *this );
}

//-----------------------------------------------------------------------------
//	Name:		AddBoolKey
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::AddBoolKey(const QDT_STRING &	sSection,
									   const QDT_STRING &	sKey,
									   const bool			bValue)
{
	INIFILE_KEY_BOOL *	pKey;

	pKey = new INIFILE_KEY_BOOL();

	pKey->SetSectionName( sSection );
	pKey->SetKeyName( sKey );
	pKey->SetValue( bValue );

	_Keys.PushTail( pKey );
}

//-----------------------------------------------------------------------------
//	Name:		AddIntKey
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::AddIntKey(const QDT_STRING &	sSection,
									  const QDT_STRING &	sKey,
									  const int				nValue)
{
	INIFILE_KEY_INT *	pKey;

	pKey = new INIFILE_KEY_INT();

	pKey->SetSectionName( sSection );
	pKey->SetKeyName( sKey );
	pKey->SetValue( nValue );

	_Keys.PushTail( pKey );
}

//-----------------------------------------------------------------------------
//	Name:		AddStringKey
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::AddStringKey(const QDT_STRING &	sSection,
										 const QDT_STRING &	sKey,
										 const QDT_STRING &	sValue)
{
	INIFILE_KEY_STRING *	pKey;

	pKey = new INIFILE_KEY_STRING();

	pKey->SetSectionName( sSection );
	pKey->SetKeyName( sKey );
	pKey->SetValue( sValue );

	_Keys.PushTail( pKey );	
}

//-----------------------------------------------------------------------------
//	Name:		GetBoolKeyValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::GetBoolKeyValue(const QDT_STRING &	sSection,
											const QDT_STRING &	sKey,
											bool &				bValue) const
{
	INIFILE_KEY_BOOL *	pKey;

	if ( IsKeyBool( sSection, sKey ) == false )
	{
		return ( false );
	}

	pKey = static_cast<INIFILE_KEY_BOOL*>( FindKey( sSection, sKey ) );
	QDT_ASSERT( pKey );

	bValue = pKey->GetValue();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetIntKeyValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::GetIntKeyValue(const QDT_STRING &	sSection,
										   const QDT_STRING &	sKey,
										   int &				nValue) const
{
	INIFILE_KEY_INT *	pKey;

	if ( IsKeyInt( sSection, sKey ) == false )
	{
		return ( false );
	}

	pKey = static_cast<INIFILE_KEY_INT*>( FindKey( sSection, sKey ) );
	QDT_ASSERT( pKey );

	nValue = pKey->GetValue();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetBoolKeyValue
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::GetStringKeyValue(const QDT_STRING &	sSection,
											  const QDT_STRING &	sKey,
											  QDT_STRING &			sValue) const
{
	INIFILE_KEY_STRING *	pKey;

	if ( IsKeyString( sSection, sKey ) == false )
	{
		return ( false );
	}

	pKey = static_cast<INIFILE_KEY_STRING*>( FindKey( sSection, sKey ) );
	QDT_ASSERT( pKey );

	sValue = pKey->GetValue();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		SetBoolKey
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::SetBoolKey(const QDT_STRING &	sSection,
									   const QDT_STRING &	sKey,
									   const bool			bValue)
{
	INIFILE_KEY_BOOL *	pKey;

	if ( IsKeyBool( sSection, sKey ) == false )
	{
		QDT_FAIL();
		return;
	}

	pKey = static_cast<INIFILE_KEY_BOOL*>( FindKey( sSection, sKey ) );
	QDT_ASSERT( pKey );

	pKey->SetValue( bValue );	
}

//-----------------------------------------------------------------------------
//	Name:		SetIntKey
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::SetIntKey(const QDT_STRING &	sSection,
									  const QDT_STRING &	sKey,
									  const int				nValue)
{
	INIFILE_KEY_INT *	pKey;

	if ( IsKeyInt( sSection, sKey ) == false )
	{
		QDT_FAIL();
		return;
	}

	pKey = static_cast<INIFILE_KEY_INT*>( FindKey( sSection, sKey ) );
	QDT_ASSERT( pKey );

	pKey->SetValue( nValue );	
}

//-----------------------------------------------------------------------------
//	Name:		SetStringKey
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	INIFILE_KEY_HOLDER::SetStringKey(const QDT_STRING &	sSection,
										 const QDT_STRING &	sKey,
										 const QDT_STRING &	sValue)
{
	INIFILE_KEY_STRING *	pKey;

	if ( IsKeyString( sSection, sKey ) == false )
	{
		QDT_FAIL();
		return;
	}

	pKey = static_cast<INIFILE_KEY_STRING*>( FindKey( sSection, sKey ) );
	QDT_ASSERT( pKey );

	pKey->SetValue( sValue );	
}

//-----------------------------------------------------------------------------
//	Name:		HasKey
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::HasKey(const QDT_STRING &	sSection,
								   const QDT_STRING &	sKey) const
{
	if ( FindKey( sSection, sKey ) == NULL )
	{
		return ( false );
	}
	else
	{
		return ( true );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsKeyBool
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::IsKeyBool(const QDT_STRING &	sSection,
									  const QDT_STRING &	sKey) const
{
	INIFILE_KEY_ABC *	pKey;

	pKey = FindKey( sSection, sKey );
	if ( pKey == NULL )
	{
		return ( false );
	}

	if ( pKey->GetType() != INIFILE_KEY_ABC::KT_BOOL )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		IsKeyInt
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::IsKeyInt(const QDT_STRING &	sSection,
									 const QDT_STRING &	sKey) const
{
	INIFILE_KEY_ABC *	pKey;

	pKey = FindKey( sSection, sKey );
	if ( pKey == NULL )
	{
		return ( false );
	}

	if ( pKey->GetType() != INIFILE_KEY_ABC::KT_INT )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		IsKeyString
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::IsKeyString(const QDT_STRING &	sSection,
										const QDT_STRING &	sKey) const
{
	INIFILE_KEY_ABC *	pKey;

	pKey = FindKey( sSection, sKey );
	if ( pKey == NULL )
	{
		return ( false );
	}

	if ( pKey->GetType() != INIFILE_KEY_ABC::KT_STRING )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrKeys
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
int	INIFILE_KEY_HOLDER::GetNbrKeys() const
{
	int	nNbrKeys;

	nNbrKeys = _Keys.GetSize();

	return ( nNbrKeys );
}

//-----------------------------------------------------------------------------
//	Name:		GetKey
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_ABC *	INIFILE_KEY_HOLDER::GetKey(const int	nKeyIndex) const
{
	QDT_ASSERT( ( nKeyIndex >= 0 ) && ( nKeyIndex < GetNbrKeys() ) );

	INIFILE_KEY_ABC *	pKey;

	pKey = _Keys[ nKeyIndex ];
	QDT_ASSERT( pKey );

	return ( pKey );
}

//-----------------------------------------------------------------------------
//	Name:		FindKey
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
INIFILE_KEY_ABC *	INIFILE_KEY_HOLDER::FindKey(const QDT_STRING &	sSection,
												const QDT_STRING &	sKey) const
{
	INIFILE_KEY_ABC *	pKey;
	int					nNbrKeys, nKeyIndex;

	nNbrKeys = GetNbrKeys();
	for ( nKeyIndex = 0; nKeyIndex < nNbrKeys; ++nKeyIndex )
	{
		pKey = GetKey( nKeyIndex );
		if ( ( pKey->GetSectionName() == sSection ) && ( pKey->GetKeyName() == sKey ) )
		{
			return ( pKey );
		}
	}

	return ( NULL );
}

//-----------------------------------------------------------------------------
//	Name:		GetKeyName
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INIFILE_KEY_HOLDER::GetKeyName(const int	nKeyIndex) const
{
	INIFILE_KEY_ABC *	pKey;

	pKey = GetKey( nKeyIndex );
	QDT_ASSERT( pKey )

	return ( pKey->GetKeyName() );
}

//-----------------------------------------------------------------------------
//	Name:		GetKeyValueString
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_HOLDER::GetKeyValueString(const int	nKeyIndex) const
{
	INIFILE_KEY_ABC *	pKey;
	QDT_STRING			sValue;


	pKey = GetKey( nKeyIndex );
	QDT_ASSERT( pKey )

	sValue = pKey->ToString();

	return ( sValue );
}

//-----------------------------------------------------------------------------
//	Name:		ReadFromFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::ReadFromFile(const QDT_STRING &	sFileName)
{
	INIFILE_KEY_ABC *	pKey;
	int					nNbrKeys, nKeyIndex;
	INI_FILE			IniFile;
	bool				bOk;


	if ( IniFile.OpenFile( sFileName ) == false )
	{
		return ( false );
	}

	bOk			= true;
	nNbrKeys	= GetNbrKeys();
	for ( nKeyIndex = 0; nKeyIndex < nNbrKeys; ++nKeyIndex )
	{
		pKey = GetKey( nKeyIndex );
		if ( pKey->ReadFromFile( IniFile ) == false )
		{
			bOk = false;	
		}
	}

	return ( bOk );
}

//-----------------------------------------------------------------------------
//	Name:		SaveToFile
//	Object:		
//	03-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	INIFILE_KEY_HOLDER::SaveToFile(const QDT_STRING &	sFileName) const
{
	INIFILE_KEY_ABC *	pKey;
	int					nNbrKeys, nKeyIndex;
	INI_FILE			IniFile;
	bool				bOk;


	if ( IniFile.OpenFile( sFileName ) == false )
	{
		return ( false );
	}

	bOk			= true;
	nNbrKeys	= GetNbrKeys();
	for ( nKeyIndex = 0; nKeyIndex < nNbrKeys; ++nKeyIndex )
	{
		pKey = GetKey( nKeyIndex );
		if ( pKey->WriteToFile( IniFile ) == false )
		{
			bOk = false;	
		}
	}

	return ( bOk );
}

//-----------------------------------------------------------------------------
//	Name:		GetTrueString
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_HOLDER::GetTrueString()
{
	return ( INIFILE_KEY_BOOL::GetTrueString() );
}

//-----------------------------------------------------------------------------
//	Name:		GetFalseString
//	Object:		
//	03-01-09:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING	INIFILE_KEY_HOLDER::GetFalseString()
{
	return ( INIFILE_KEY_BOOL::GetFalseString() );
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
