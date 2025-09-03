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
//	CLASS:	TEXT_DATA_FILE_ABC
//
//	05-05-31:	VPI - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(TextDataFile/TextDataFileABC)
#include	INCL_KCORE(File/FilesManagerABC)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

#define		QDT_INCLUDE_X360_XTL
#include	INCL_KCORE(Includes_X360/Includes_X360)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//unique string that can be used as a key value in an existing INI file
const char*	TEXT_DATA_FILE_ABC::DEFAULT_EXISTS_SECTION_STR = "4882C322-BCB5-4f2e-BDAF-2FD45FB1322B";

//-----------------------------------------------------------------------------
//	Name:		TEXT_DATA_FILE_ABC constructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_DATA_FILE_ABC()
:
_bIsGood(false),
_bHasChanged(false)
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TEXT_DATA_FILE_ABC constructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::TEXT_DATA_FILE_ABC(const QDT_STRING &	strFileName)
:
_bIsGood(false),
_bHasChanged(false)
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		TEXT_DATA_FILE_ABC destructor
//	Object:		
//	02-01-21:	DAC - Created
//-----------------------------------------------------------------------------
TEXT_DATA_FILE_ABC::~TEXT_DATA_FILE_ABC()
{
	//Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetFileName
//	Object:		
//	02-01-21:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	TEXT_DATA_FILE_ABC::GetFileName() const
{
	return	(_strFileName);
}

//-----------------------------------------------------------------------------
//	Name:		SetFileName
//	Object:		
//	05-06-13:	VPI - Created
//-----------------------------------------------------------------------------
void	TEXT_DATA_FILE_ABC::SetFileName(const QDT_STRING &	sName)
{
	_strFileName = sName;
}

//-----------------------------------------------------------------------------
//	Name:		IsGood
//	Object:		
//	02-01-21:	VPI - Created
//-----------------------------------------------------------------------------
bool	TEXT_DATA_FILE_ABC::IsGood() const
{
	return	(_bIsGood);
}

//-----------------------------------------------------------------------------
//	Name:		SetIsGood
//	Object:		
//	02-01-21:	VPI - Created
//-----------------------------------------------------------------------------
void	TEXT_DATA_FILE_ABC::SetIsGood(const bool	bFlag)
{
	_bIsGood = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		HasChanged
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
bool	TEXT_DATA_FILE_ABC::HasChanged() const
{
	return (_bHasChanged);
}

//-----------------------------------------------------------------------------
//	Name:		SetHasChanged
//	Object:		
//	04-03-23:	VPI - Created
//-----------------------------------------------------------------------------
void	TEXT_DATA_FILE_ABC::SetHasChanged(const bool	bFlag) const
{
	_bHasChanged = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		IsFileReadOnly
//	Object:		
//	02-03-21:	VPI - Created
//-----------------------------------------------------------------------------
bool	TEXT_DATA_FILE_ABC::IsFileReadOnly() const
{
	if	(_strFileName.IsEmpty())
	{
		return	(false);
	}

#if defined(PLATFORM_PS3)
	QDT::KCORE::QDT_Error(QDT_COMMENT("XML_SECTION_FILE::IsFileReadOnly on Console !!"));
	return false;
#else
	return	((GetFileAttributes(_strFileName.GetBuffer()) & FILE_ATTRIBUTE_READONLY) != 0);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		BackupFile
//	Object:		Create a copy of the ini file on the hardrive
//	03-03-07:	IAM - Created
//-----------------------------------------------------------------------------
bool	TEXT_DATA_FILE_ABC::BackupFile() const
{
	QDT_ASSERT(IsGood());

#if defined(PLATFORM_PS3)
	QDT::KCORE::QDT_Error(QDT_COMMENT("XML_SECTION_FILE::BackupFile() on Console !!"));
	return (false);
#else
	
	QDT_STRING sDestFileName, sSourceFileName, sMsg;
	
	// Check the size
	int nSize = FILES_MANAGER_ABC::Instance()->GetFileSize(GetFileName().GetBuffer());
	
	//	Original file has a size of 0 ..it is screwed, don't erase the bak file !!
	if	(nSize == 0)
	{
		return	(true);
	}

	sSourceFileName	= GetFileName();
	sDestFileName	= GetFileName();
	sDestFileName	+= ".bak";

	SetFileAttributes(sDestFileName, FILE_ATTRIBUTE_NORMAL);

	if ( ::CopyFile( sSourceFileName, sDestFileName, FALSE ) == FALSE )
	{
		return ( false );
	}
	else
	{
		return ( true );
	}
	
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetLastErrorReason
//	Object:		
//	05-06-08:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING 	TEXT_DATA_FILE_ABC::GetLastErrorReason() const
{
	return (_sLastErrorReason);
}

//-----------------------------------------------------------------------------
//	Name:		SetLastErrorReason
//	Object:		
//	05-06-08:	VPI - Created
//-----------------------------------------------------------------------------
void	TEXT_DATA_FILE_ABC::SetLastErrorReason(const QDT_STRING &	sReason)
{
	_sLastErrorReason = sReason;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
