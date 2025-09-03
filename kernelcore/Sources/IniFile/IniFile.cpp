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
//	CLASS:	INI_FILE
//
//	02-01-21:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IniFile/IniFile)

#include	INCL_KCORE(File/FilesManagerABC)
#include	INCL_KCORE(Stream/FileTextStream)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INI_FILE constructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
INI_FILE::INI_FILE()
:
_nCurrentSection(-1),
_nCurrentKey(-1)
{
}

//-----------------------------------------------------------------------------
//	Name:		INI_FILE constructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
INI_FILE::INI_FILE(const QDT_STRING &	strFileName)
:
_nCurrentSection(-1),
_nCurrentKey(-1)
{
	OpenFile(strFileName);
}


//-----------------------------------------------------------------------------
//	Name:		INI_FILE constructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
INI_FILE::INI_FILE(const INI_FILE & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		INI_FILE destructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
INI_FILE::~INI_FILE()
{
	CloseFile();
	ClearAllSections();
	SetIsGood(false);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
INI_FILE &	INI_FILE::operator=(const INI_FILE & C)
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
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
void	INI_FILE::Copy(const INI_FILE & C)
{

	_strFileName		= C._strFileName;
	_bIsGood			= C._bIsGood;
	_bHasChanged		= C._bHasChanged;
	_nCurrentSection	= C._nCurrentSection;
	_sCurrentSectionName= C._sCurrentSectionName;
	_nCurrentKey		= C._nCurrentKey;
}

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	02-01-22:	DAC - Created
//-----------------------------------------------------------------------------
void	INI_FILE::Reset()
{
	_strFileName.Empty();
	_bIsGood = false;
	_bHasChanged = false;
	ClearAllSections();
}


//-----------------------------------------------------------------------------
//	Name:		OpenFile
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::OpenFile(const QDT_STRING &	strFileName)
{
	//if file exists, ok 
	if	(FILES_MANAGER_ABC::Instance()->IsFileExist(strFileName.GetBuffer()))
	{
		_strFileName = strFileName;
		ReadIniFileSection(_strFileName);
		SetIsGood(true);
		
		return	(true);
	}
#ifdef PLATFORM_PC
	else //if not , create it 
	{
		QDT_STRING	sPath;
		QDT_STRING	sFile;
		bool		bSplitOk;
		
		bSplitOk = QDT_STRING::SplitPathFileName(strFileName,sPath, sFile);
		if	(bSplitOk)
		{
			// Create the directory if it does not already exist.
			if ( GetFileAttributes( sPath ) == 0xFFFFFFFF )
			{
				if ( CreateDirectory( sPath, NULL ) == FALSE )
				{
					QDT_STRING sErrorMsg;
					sErrorMsg.Format( "Failed creating the directory '%s', can't create file.", strFileName.GetBuffer() );
					QDT::KCORE::QDT_MessageBox(NULL,sErrorMsg, "ERROR", QDT::KCORE::QDT_ICONSTOP );
					
					SetIsGood(false);
					return	(false);
				}
			}
		}

		FILE_TEXT_STREAM fStream;
		if	(fStream.Open(strFileName.GetBuffer(), FILE_STREAM::AT_CREATE))
		{
			fStream.Close();
			_strFileName = strFileName;
			ReadIniFileSection(_strFileName);
			SetIsGood(true);
			return	(true);
		}
	}
#endif

	//well let's face it ! we failed !
	SetIsGood(false);
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		OpenFile
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::OpenFile(IO_STREAM &	Stream)
{
	ReadIniFileSection(Stream);
	SetIsGood(true);
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		CloseFile
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::CloseFile()
{
	return (WriteIniFileSection(GetFileName()));
}

//-----------------------------------------------------------------------------
//	Name:		WriteFile
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::WriteFile(IO_STREAM &	Stream)
{
	return (WriteIniFileSection(Stream));
}

//-----------------------------------------------------------------------------
//	Name:		PrepareForMemoryUsage
//	Object:		
//	04-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	INI_FILE::PrepareForMemoryUsage(const QDT_STRING & sRootName)
{
	Reset();
	SetIsGood(true);
}


//-----------------------------------------------------------------------------
//	Name:		EraseFile
//	Object:		
//	02-01-23:	DAC - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::EraseFile()
{
	if	(GetFileName().IsEmpty())
	{
		return (false);
	}

	ClearAllSections();
	_bHasChanged = false;

	FILE_TEXT_STREAM fStream;
	if	(fStream.Open(GetFileName().GetBuffer(), FILE_STREAM::AT_CREATE))
	{
		SetIsGood(true);
		fStream.Close();return	(true);
	}

	SetIsGood(false);
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		DoesSectionAndKeyExist
//	Object:		
//	02-05-29:	GGO - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::DoesSectionAndKeyExist(const QDT_STRING &	strSection,
										 const QDT_STRING &	strKey) const
{
	QDT_STRING	strReturn;

	strReturn = GetString(strSection , strKey, DEFAULT_EXISTS_SECTION_STR);

	return	(strReturn != DEFAULT_EXISTS_SECTION_STR);
}

//-----------------------------------------------------------------------------
//	Name:		GetString
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
QDT_STRING	INI_FILE::GetString(const QDT_STRING &	strSection,
								const QDT_STRING &	strKey,
								const QDT_STRING &	strDefault) const
{
	QDT_ENSURE(IsGood());

	QDT_ASSERT(strSection.IsEmpty() == false);
	QDT_ASSERT(strKey.IsEmpty() == false);

	int			nRetCode;
	QDT_STRING	sBuffer;
	int			nSize;
	bool		bOk;
	
	bOk		= false;
	nSize	= DEFAULT_BUFFER_SIZE;

	while	(bOk == false)
	{
#if (defined(PLATFORM_PS2) || defined(PLATFORM_XBOX))
		QDT::KRN::QDT_Error(QDT_COMMENT("INI_FILE::GetString on Console !!"));
#else
		nRetCode	= GetMemPrivateProfileString( strSection, strKey, strDefault, sBuffer, nSize, GetFileName() );
#endif
		bOk			= true;

		if	(nRetCode == nSize - 2)	//Microsoft doc (see return val)
		{
			bOk = false;
		}
	}
	
	return	(sBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetInt
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
int	INI_FILE::GetInt(const QDT_STRING &	strSection,
					 const QDT_STRING &	strKey,
					 const int			nDefault) const
{
	QDT_ENSURE(IsGood());

	QDT_STRING strVal;
	QDT_STRING sDefault;
	sDefault += nDefault;
	int			nVal;

	strVal = GetString( strSection, strKey, sDefault );
	
	nVal = strVal.Atoi();

	return	(nVal);
}

//-----------------------------------------------------------------------------
//	Name:		GetBool
//	Object:		
//	02-10-16:	GGO - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::GetBool(const QDT_STRING &	strSection,
						  const QDT_STRING &	strKey,
						  const bool			bDefault) const
{
	int	nVal;

	nVal = GetInt( strSection, strKey, (int) bDefault );

	if ( nVal == 0 )
	{
		return ( false );
	}
	else
	{
		QDT_ASSERT( nVal == 1 );

		return ( true );
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFloat
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
float	INI_FILE::GetFloat(const QDT_STRING &	strSection,
						   const QDT_STRING &	strKey,
						   const float			rDefault) const
{
	QDT_ENSURE(IsGood());

	QDT_STRING	strVal;
	QDT_STRING	sDefault;
	sDefault += rDefault;
	float		rVal;

	strVal = GetString( strSection, strKey, sDefault);
	
	rVal = strVal.Atof();

	return	(rVal);
}

//-----------------------------------------------------------------------------
//	Name:		WriteString
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_DATA_FILE_ERRORS	INI_FILE::WriteString(const QDT_STRING &	strSection,
																  const QDT_STRING &	strKey,
																  const QDT_STRING &	strVal)
{
	QDT_ENSURE(IsGood());

	QDT_ASSERT(strSection.IsEmpty() == false);
	QDT_ASSERT(strKey.IsEmpty() == false);
	
	int 	nRet;

#if (defined(PLATFORM_PS2) || defined(PLATFORM_XBOX))
	QDT::KRN::QDT_Error(QDT_COMMENT("INI_FILE::WriteString on Console !!"));
#else
	nRet = (int) WriteMemPrivateProfileString(strSection, strKey, strVal, GetFileName());
#endif

	if	(nRet == 0)
	{
		return	(ERROR_WRITE);
	}

	return	(NO_ERRORS);
}

//-----------------------------------------------------------------------------
//	Name:		WriteInt
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_DATA_FILE_ERRORS	INI_FILE::WriteInt(const QDT_STRING &	strSection,
															   const QDT_STRING &	strKey,
															   const int			nVal)
{
	QDT_ENSURE(IsGood());

	QDT_ASSERT(strSection.IsEmpty() == false);
	QDT_ASSERT(strKey.IsEmpty() == false);

	QDT_STRING	strVal;

	strVal.ValueOf(nVal);

	return	(WriteString(strSection, strKey, strVal));
}

//-----------------------------------------------------------------------------
//	Name:		WriteBool
//	Object:		
//	02-10-16:	GGO - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_DATA_FILE_ERRORS	INI_FILE::WriteBool(const QDT_STRING &	strSection,
																const QDT_STRING &	strKey,
																const bool			bVal)
{
	if ( bVal == true )
	{
		return ( WriteInt( strSection, strKey, 1 ) );
	}
	else
	{
		return ( WriteInt( strSection, strKey, 0 ) );
	}
}

//-----------------------------------------------------------------------------
//	Name:		WriteFloat
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_DATA_FILE_ERRORS	INI_FILE::WriteFloat(const QDT_STRING &	strSection,
																 const QDT_STRING &	strKey,
																 const float		rVal)
{
	QDT_ENSURE(IsGood());

	QDT_ASSERT(strSection.IsEmpty() == false);
	QDT_ASSERT(strKey.IsEmpty() == false);

	QDT_STRING	strVal;

	strVal.ValueOf(rVal);

	return	(WriteString(strSection, strKey, strVal));
}

//-----------------------------------------------------------------------------
//	Name:		PushEmptyLine
//	Object:		
//	02-01-23:	DAC - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::PushEmptyLine()
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		ClearAllSections
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
void	INI_FILE::ClearAllSections()
{
	int i, nMax;
	nMax = _vSection.GetSize();
	for (i = 0; i < nMax; ++i)
	{
		_vSection[i].vKey.Clear();
	}
	
	_vSection.Clear();
	_nCurrentSection	= -1;
	_sCurrentSectionName.Empty();
	_nCurrentKey		= -1;
	_bHasChanged		= false;
}

//-----------------------------------------------------------------------------
//	Name:		ReadIniFileSection
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::ReadIniFileSection(const QDT_STRING &	strIniFileName)
{
	FILE_TEXT_STREAM Stream;
	bool bRes = false;
	Stream.Open(strIniFileName, IO_STREAM::READ);

	bRes = ReadIniFileSection(Stream);

	Stream.Close();
	return (bRes);
}

//-----------------------------------------------------------------------------
//	Name:		ReadIniFileSection
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::ReadIniFileSection(IO_STREAM &	Stream)
{
	const int				nBuffSize = 16*1024;
	QDT_STRING				sLine, sErrorMsg;
	char					Buff[nBuffSize];
	QDT_VECTOR<QDT_STRING>	InvalidLines;

	while (Stream.IsEof() == false)
	{
		Stream.GetLine(Buff, nBuffSize, '\n');
		if(Buff[0] != '\0')
		{
			sLine = Buff;
			sLine.TrimRight();
			sLine.TrimLeft();

			if (sLine.IsEmpty() == false)
			{
				if (sLine[0] == '[')
				{
					sLine = sLine.ExtractMid(1, sLine.GetSize()-2);
					sLine.TrimRight();
					sLine.TrimLeft();
					NewSection(sLine);
				}
				else
				{
					if (_nCurrentSection == -1)
					{
						InvalidLines.PushTail( sLine );
					}
					else
					{
						if ( NewKey(_nCurrentSection, sLine) == -1 )
						{
							if ( sLine.GetSize() > 64 ) // Don't display too long lines.
							{
								sLine = sLine.EraseRight( sLine.GetSize() - 64 );
								sLine += "...";
							}
							InvalidLines.PushTail( sLine );
						}
					}
				}
			}
		}
	}

	// Display error message if something wrong happened.
	if ( InvalidLines.IsEmpty() == false )
	{
#ifndef _MASTER
		int	nIndex, nSize;

		sErrorMsg = "Error in 'INI_FILE::ReadIniFileSection', could not create key from the following lines :\r\n";

		nSize = InvalidLines.GetSize();
		for ( nIndex = 0; nIndex < nSize; ++nIndex )
		{
			sErrorMsg += "- ";
			sErrorMsg += InvalidLines[ nIndex ];
			sErrorMsg +="\r\n";

			if ( nIndex >= 10 ) // Don't display too many invalid lines.
			{
				sErrorMsg += "- ...\r\n";
				break;
			}
		}

		QDT::KCORE::QDT_Error( sErrorMsg.GetBuffer() );
#endif
		return ( false );
	}

	return(true);
}

//-----------------------------------------------------------------------------
//	Name:		WriteIniFileSection
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::WriteIniFileSection(const QDT_STRING &	strIniFileName) const
{
	FILE_TEXT_STREAM Stream;
	int nNbrSections;
	
	nNbrSections = GetNbrSections();

	if ((nNbrSections == 0) || (HasChanged() == false) || (GetFileName().IsEmpty()))
	{
		//do nothing if no sections -> call EraseIniFile() to clear the ini on disk
		return (false);
	}

#ifndef _MASTER
	QDT_STRING Str;
	Str.Format("Saving %s", strIniFileName.GetBuffer());
	QDT::KCORE::QDT_Message(Str);
#endif

	if (Stream.Open(strIniFileName, IO_STREAM::WRITE) == false)
	{
		QDT::KCORE::QDT_Error(QDT_COMMENT("Can't open Inifile to save"));
		return (false);
	}

	WriteIniFileSection(Stream);

	Stream.Close();
	_bHasChanged = false;
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		WriteIniFileSection
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::WriteIniFileSection(IO_STREAM &	Stream) const
{
	QDT_STRING sLine;
	int i, nNbrSections;
	int j, nNbrKeys;
	
	nNbrSections = GetNbrSections();

	for (i = 0; i < nNbrSections; ++i)
	{
		Stream.PutString("");
		sLine.Format("[%s]", GetSectionName(i).GetBuffer());
		Stream.PutString(sLine.GetBuffer());

		nNbrKeys = GetNbrKeys(i);
		for (j = 0; j < nNbrKeys; ++j)
		{
			sLine.Format("%s=%s", GetKeyName(i, j).GetBuffer(), GetKeyValue(i, j).GetBuffer());
			Stream.PutString(sLine.GetBuffer());
		}
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		NewSection
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::NewSection(const QDT_STRING &	sLine)
{
	REG_SECTION Section;
	Section.vKey.Clear();
	if (sLine.IsEmpty())
	{
		QDT::KCORE::QDT_Error(QDT_COMMENT("Empty Section Name in Inifile"));
		return (-1);
	}
	Section.sSectionName = sLine;
	_vSection.PushTail(Section);
	_nCurrentSection = _vSection.GetSize() - 1;
	_sCurrentSectionName = sLine;
	_nCurrentKey = -1;
	return (_nCurrentSection);
}

//-----------------------------------------------------------------------------
//	Name:		NewKey
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::NewKey(const int			nSection,
					 const QDT_STRING &	sLine)
{
	REG_KEY Key;
	QDT_STRING sKeyName, sKeyValue;
	int nPos = sLine.Find('=', 0);
	if (nPos == -1)
	{
		QDT::KCORE::QDT_Warning(QDT_COMMENT("Bad Key Format in Inifile"));
		return (-1);
	}

	sKeyName = sLine.ExtractMid(0, nPos);
	sKeyValue = sLine.ExtractMid(nPos+1, sLine.GetSize() - nPos - 1);
	sKeyName.TrimRight();
	sKeyName.TrimLeft();
	sKeyValue.TrimRight();
	sKeyValue.TrimLeft();
	if (sKeyName.IsEmpty())
	{
		QDT::KCORE::QDT_Error(QDT_COMMENT("Empty Key Name in Inifile"));
		return(-1);
	}

	Key.sKeyName = sKeyName;
	Key.sKeyValue = sKeyValue;
	if (nSection < 0)
	{
		QDT::KCORE::QDT_Error(QDT_COMMENT("No Current Section in Inifile"));
		return(-1);
	}

	_vSection[nSection].vKey.PushTail(Key);
	_nCurrentKey = _vSection[nSection].vKey.GetSize() - 1;
	return (_vSection[nSection].vKey.GetSize()-1);
}

//-----------------------------------------------------------------------------
//	Name:		ChangeKeyValue
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
void	INI_FILE::ChangeKeyValue(int				nSectionIndex,
								 int				nKeyIndex,
								 const QDT_STRING &	sKeyValue)
{
	QDT_ASSERT ( (nSectionIndex != -1) && (nKeyIndex != -1));
	_vSection[nSectionIndex].vKey[nKeyIndex].sKeyValue = sKeyValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetMemPrivateProfileString
//	Object:		The return value is the number of characters copied to the buffer, not including the terminating null character. 
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::GetMemPrivateProfileString(const QDT_STRING &	strSection,
										 const QDT_STRING &	strKey,
										 const QDT_STRING &	strDefault,
										 QDT_STRING &		sDestBuffer,
										 const int			nSize,		//only for prototype compatibility.
										 const QDT_STRING &	strIniFileName) const
{
	int nSectionIndex;
	int nKeyIndex;
	QDT_STRING Str;

	QDT_ASSERT (strIniFileName == GetFileName());

	nSectionIndex = FindSection(strSection);
	if (nSectionIndex >= 0)
	{
		_sCurrentSectionName = strSection;
		_nCurrentSection = nSectionIndex;

		nKeyIndex = FindKey(nSectionIndex, strKey);
		if (nKeyIndex >= 0)
		{
			sDestBuffer = GetKeyValue(nSectionIndex, nKeyIndex);
			return(sDestBuffer.GetSize());
		}
	}
	
	sDestBuffer = strDefault;
	return(sDestBuffer.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		WriteMemPrivateProfileString
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::WriteMemPrivateProfileString(const QDT_STRING &	sSectionName,
											   const QDT_STRING &	sKeyName,
											   const QDT_STRING &	sString,
											   const QDT_STRING &	sFileName)
{
	int nSectionIndex;
	int nKeyIndex;
	QDT_STRING Str;

	SetHasChanged(true);
	Str = sKeyName + "=" + sString;

	if (sFileName != GetFileName())
	{
		QDT_FAIL();
	}

	nSectionIndex = FindSection(sSectionName);

	if (nSectionIndex >= 0)
	{
		_nCurrentSection = nSectionIndex;
		_sCurrentSectionName = sSectionName;
		nKeyIndex = FindKey(nSectionIndex, sKeyName);
		if (nKeyIndex >= 0)
		{
			ChangeKeyValue(nSectionIndex, nKeyIndex , sString);
			return(true);
		}
		//No Key found
		NewKey(nSectionIndex, Str);
		return(true);
	}
	//no section found
	nSectionIndex = NewSection(sSectionName);
	NewKey(nSectionIndex, Str);
	return(true);
}

//-----------------------------------------------------------------------------
//	Name:		DoesSectionExist
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::DoesSectionExist(const QDT_STRING &	sSectionName) const
{
	return (FindSection(sSectionName) != -1);
}

//-----------------------------------------------------------------------------
//	Name:		FindSection
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::FindSection(const QDT_STRING &	sSectionName) const
{
	int i, nNbrSections;
	nNbrSections = GetNbrSections();

	if (_sCurrentSectionName.CompareNoCase(sSectionName))
	{
		return (_nCurrentSection);
	}
	else if ((_nCurrentSection+1 < nNbrSections) && (GetSectionName(_nCurrentSection+1).CompareNoCase(sSectionName)))
	{
		return (_nCurrentSection+1);
	}
	else if ((_nCurrentSection > 0) && (GetSectionName(_nCurrentSection-1).CompareNoCase(sSectionName)))
	{
		return (_nCurrentSection-1);
	}

	//can't find it around _nCurrentSection, so full search. ( maybe set up a Hash Table later )

	for (i = 0; i < nNbrSections; ++i)
	{
		if (sSectionName.CompareNoCase(GetSectionName(i)))
		{
			return (i);
		}
	}
	return (-1);
}

//-----------------------------------------------------------------------------
//	Name:		FindKey
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::FindKey(const int				nSectionIndex,
					  const QDT_STRING &	sKeyName) const
{
	QDT_ASSERT(nSectionIndex != -1);
	int i, nNbrKey;
	nNbrKey = GetNbrKeys(nSectionIndex);
	for (i = 0; i < nNbrKey; ++i)
	{
		if (GetKeyName(nSectionIndex, i).CompareNoCase(sKeyName))
		{
			return (i);
		}
	}
	return (-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrSections
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::GetNbrSections() const
{
	return(_vSection.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrKeys
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
int	INI_FILE::GetNbrKeys(const int	nSectionIndex) const
{
	return (_vSection[nSectionIndex].vKey.GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		GetSectionName
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INI_FILE::GetSectionName(const int	nSectionIndex) const
{
	return (_vSection[nSectionIndex].sSectionName);
}

//-----------------------------------------------------------------------------
//	Name:		GetKeyName
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INI_FILE::GetKeyName(const int	nSectionIndex,
										 const int	nKeyIndex) const
{
	return (_vSection[nSectionIndex].vKey[nKeyIndex].sKeyName);
}

//-----------------------------------------------------------------------------
//	Name:		GetKeyValue
//	Object:		
//	04-03-22:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	INI_FILE::GetKeyValue(const int	nSectionIndex,
										  const int	nKeyIndex) const
{
	return (_vSection[nSectionIndex].vKey[nKeyIndex].sKeyValue);
}

//-----------------------------------------------------------------------------
//	Name:		ResetToTop
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::ResetToTop()
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		PushSection
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::PushSection(const QDT_STRING &	sName)
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		PopSection
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::PopSection()
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		PushElement
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::PushElement(const QDT_STRING &	sName,
							  const QDT_STRING &	sValue)
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		PushText
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
bool	INI_FILE::PushText(const QDT_STRING &	sText)
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetSectionList
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
TEXT_DATA_LIST_ABC *	INI_FILE::GetSectionList(const QDT_STRING &	sName)
{
	QDT_NOT_IMPLEMENTED();
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		ChangeRelativeSearch
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
void	INI_FILE::ChangeRelativeSearch(const QDT_STRING &	sName)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		ResetRelativeSearch
//	Object:		
//	05-06-16:	VPI - Created
//-----------------------------------------------------------------------------
void	INI_FILE::ResetRelativeSearch()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetFileType
//	Object:		
//	06-06-13:	CAU - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_FILE_TYPE	INI_FILE::GetFileType() const
{
	return TFT_INI;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
