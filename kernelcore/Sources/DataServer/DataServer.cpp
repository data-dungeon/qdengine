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
//	CLASS:	DATA_SERVER
//
//	05-02-16:	AMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(DataServer/DataServer)

#include	INCL_KCORE(IniFile/IniFile)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

const char * DATA_SERVER::DEFAULT_INI = "DataServer.ini";

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-02-16:	AMA - Created
//-----------------------------------------------------------------------------
void	DATA_SERVER::Init(const char* pIniFile)
{
	int			nIndex = 0;
	QDT_STRING	sIniFile(DEFAULT_INI);
	QDT_STRING	sSection;
	QDT_STRING	sNameKey, sCurrentNameKey;
	QDT_STRING	sExportKey, sCurrentExportKey;
	QDT_STRING	sName;
	bool		bExport;
	INI_FILE	DatabaseIniFile;

	sSection	= "DataServer";
	sNameKey	= "Name";
	sExportKey	= "Export";

	if(pIniFile)
	{
		sIniFile = 	pIniFile;
	}

	if ( DatabaseIniFile.OpenFile(sIniFile.GetBuffer()) )
	{
		sCurrentNameKey.Format("%s%d", sNameKey.GetBuffer(), nIndex);
		sCurrentExportKey.Format("%s%d", sExportKey.GetBuffer(), nIndex);

		sName	= DatabaseIniFile.GetString(sSection, sCurrentNameKey);
		bExport = DatabaseIniFile.GetBool(sSection, sCurrentExportKey);
		
		while ( ( sName.IsEmpty() == false ) && ( sName.IsEmpty() == false ) )
		{
			SERVER_INFO ServerInfo;

			ServerInfo.sName		= sName;
			ServerInfo.bExport	= bExport;
			_vDatabase.PushTail(ServerInfo);

			++nIndex;

			sCurrentNameKey.Format("%s%d", sNameKey.GetBuffer(), nIndex);
			sCurrentExportKey.Format("%s%d", sExportKey.GetBuffer(), nIndex);

			sName	= DatabaseIniFile.GetString(sSection, sCurrentNameKey);
			bExport = DatabaseIniFile.GetBool(sSection, sCurrentExportKey);
		}

		DatabaseIniFile.CloseFile();
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrDatabase
//	Object:		
//	05-02-16:	AMA - Created
//-----------------------------------------------------------------------------
int	DATA_SERVER::GetNbrDatabase()
{
	return (_vDatabase.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetDatabaseName
//	Object:		
//	05-02-16:	AMA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	DATA_SERVER::GetDatabaseName(int	nIndex)
{
	return (_vDatabase[nIndex].sName);
}

//-----------------------------------------------------------------------------
//	Name:		IsExportDatabase
//	Object:		
//	05-02-16:	AMA - Created
//-----------------------------------------------------------------------------
bool	DATA_SERVER::IsExportDatabase(int	nIndex)
{
	return (_vDatabase[nIndex].bExport);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
