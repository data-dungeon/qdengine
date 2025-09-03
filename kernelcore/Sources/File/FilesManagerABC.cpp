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
//	CLASS:	FILES_MANAGER_ABC
//
//	03-05-21:	VMA - Created 
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(File/FilesManagerABC)

#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(Tools/QDT_StringTokenizer)
#include	INCL_KCORE(OS/OS)

#include		INCL_KCORE(File/Remote/RemoteFile)
#if defined (PLATFORM_PC)
	#include	INCL_KCORE(File/PC/FilePCW98)
	#include	INCL_KCORE(File/PC/FilePCW2K)
	#define		QDT_INCLUDE_PC_WINDOWS
	#include	INCL_KCORE(Includes_PC/Includes_PC)
	#include <stdio.h>
#elif defined (PLATFORM_PS3)
	#include	INCL_KCORE(File/PS3/FilePS3Host)
	#define		QDT_INCLUDE_STDIO
	#include	INCL_KCORE(Includes/Includes)
#elif defined (PLATFORM_X360)
	#include	INCL_KCORE(File/X360/FileX360)
	#define		QDT_INCLUDE_X360_XTL
	#include	INCL_KCORE(Includes_X360/Includes_X360)
	#include <stdio.h>
#else
	#error PLATFORM UNIDENTIFIED, PLEASE CHECK
#endif

#ifndef _MASTER
#include	INCL_KCORE(Core/MessageLog)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

FILES_MANAGER_ABC *	FILES_MANAGER_ABC::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		FILES_MANAGER constructor
//	Object:		
//	02-02-19:	AMA - Created
//-----------------------------------------------------------------------------
FILES_MANAGER_ABC::FILES_MANAGER_ABC()
:
_bEnableEmulateCD	(false),
_bWinNT				(true),
_pRemoteFileService(0),
_bRemoteMode(false)
{  
	// Do Nothing...s
}

//-----------------------------------------------------------------------------
//	Name:		FILES_MANAGER destructor
//	Object:		
//	02-02-19:	AMA - Created
//-----------------------------------------------------------------------------
FILES_MANAGER_ABC::~FILES_MANAGER_ABC()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		
//	05-10-17:	AMA - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::Create()
{
	QDT_ASSERT ( _pInstance == NULL );
	_pInstance = new FILES_MANAGER_ABC;
	_pInstance->Init();
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	05-10-17:	AMA - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::Release()
{
#ifndef _MASTER
	MESSAGE_LOG::Release();
#endif
	QDT_ASSERT ( _pInstance );
	delete _pInstance;
	_pInstance = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	05-10-17:	AMA - Created
//-----------------------------------------------------------------------------
FILES_MANAGER_ABC *	FILES_MANAGER_ABC::Instance()
{
	QDT_ASSERT ( _pInstance );
	return(_pInstance);
}

//-----------------------------------------------------------------------------
//	Name:		Exist
//	Object:		
//	06-02-23:	MBI - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::Exist()
{
	return (_pInstance != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		SetInstance
//	Object:		
//	05-10-19:	AMA - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::SetInstance(FILES_MANAGER_ABC *	pInstance)
{
	_pInstance = pInstance;
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	05-04-21:	AMA - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::Init()
{
#ifdef PLATFORM_PC
	OSVERSIONINFO VersionInformation;
	VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if ( GetVersionEx(&VersionInformation) == 0 )
	{
		QDT_STRING sMessage;
		OS::GetWinLastErrorMsg(sMessage);
		QDT_Error(sMessage.GetBuffer());
	}

	if ( VersionInformation.dwPlatformId < VER_PLATFORM_WIN32_NT )
	{
		QDT_Message("FILES_MANAGER_ABC::Init() : WINDOWS 98");
		_bWinNT = false;
	}
	else
	{
		QDT_Message("FILES_MANAGER_ABC::Init() : WINDOWS NT");
		_bWinNT = true;
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:	Open a file (can be a remote one)	
//	03-05-21:	VMA - Created
//-----------------------------------------------------------------------------
FILE_ABC *	FILES_MANAGER_ABC::Open(const char * const		pFileName,
																		const FILE_ABC::F_MODE	eMode,
																		FM_ERROR &				eError,
																		const QDT_FLAGS&	rFlags)		
{
	FILE_ABC * pFile;
	eError = FM_FILE_NO_ERROR;
	char szName[255];	// file to open

	if (GetRemoteMode())
	{
		const QDT_FLAGS* pFlagsHack = &(rFlags);
		(const_cast<QDT_FLAGS*> (pFlagsHack))->Set(FO_FILE_REMOTE);
	}

#ifdef PLATFORM_PS3
	#ifdef ADD_HOME_PS3
		sprintf(szName,"D:/HOME_PS3/%s",pFileName);
	#else 
		sprintf(szName,"%s",pFileName);
	#endif
#else		// PC
	sprintf(szName,"%s",pFileName);
#endif 
	
	// Remote file.
	if (rFlags.IsSet(FO_FILE_REMOTE) )
	{
		QDT_ASSERT(_pRemoteFileService);
		Int32 nDesiredAccess = 0;
		if ( ! _pRemoteFileService->GotALink() )
		{
			return NULL;
		}
		// no test on error, we are on remote
		pFile = new REMOTE_FILE();
	}
	// Local File.
	else  
	{
	#if defined (PLATFORM_PC)
		if ( FILES_MANAGER_ABC::GetWinNT() )
		{
			pFile = new FILE_PC_W2K;
		}
		else
		{
			pFile = new FILE_PC_W98;
		}
	#elif defined (PLATFORM_PS3)
		pFile = new FILE_PS3_HOST;
	#elif defined (PLATFORM_X360)
		pFile = new FILE_X360;
	#endif
	}

	const bool				bShare = rFlags.IsSet(FO_FILE_SHARE) ? true : false;

	// Open 
	bool bOK = pFile->Open(szName, eMode, bShare);
		
	if (  ! rFlags.IsSet(FO_FILE_REMOTE) ) 
	{
		// Not remote so we check only the return of the OPEN.
		if (! bOK)
		{
			eError = FM_OPEN_FAILED;
			delete pFile;
			return  NULL ;
		}
	}
	else
	{
		// Remote, the return flag is not enough, Open check some prerequesites but we have 
		// to wait until really open or time out.
		if (! bOK)
		{
			// prerequesites failed.
			eError = FM_OPEN_FAILED;
			delete pFile;
			return NULL;
		}
		// wait until service has done it's job (or timeout)
		while ( ! pFile->IsIdle() )
		{
		}
		// maybe the file is no longer idle but opening failed.
		if (pFile->GetLastError() != FM_FILE_NO_ERROR)
		{
			eError = FM_OPEN_FAILED;
			delete pFile;
			return NULL;
		}
	}

	return (pFile);
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	03-05-21:	VMA - Created
//-----------------------------------------------------------------------------
FILES_MANAGER_ABC::FM_ERROR	FILES_MANAGER_ABC::Close(FILE_ABC *	pFile)
{
	FM_ERROR eError = FM_FILE_NO_ERROR;
	
	QDT_ASSERT( pFile );

	//QDT_ASSERT( pFile->IsIdle() );
	while ( ! pFile->IsIdle() )
	{
		//	Wait end current loading
	}
	
	if ( pFile->Close() )
	{
		delete pFile;
		return ( eError );
	}
	else
	{
		eError = FM_CLOSE_FAILED;
		delete pFile;				//delete quand même ?
		return ( eError );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	03-05-21:	VMA - Created
//-----------------------------------------------------------------------------
FILES_MANAGER_ABC::FM_ERROR	FILES_MANAGER_ABC::Delete(const char * const	pFileName)
{
	FILE_ABC * pFile;
#if defined(PLATFORM_PC)
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}
#elif defined (PLATFORM_PS3)
	pFile = new FILE_PS3_HOST;
#elif defined (PLATFORM_X360)
	pFile = new FILE_X360;
#endif

	FM_ERROR		eError = FM_FILE_NO_ERROR;
	if ( pFile->IsExist(pFileName) == true )
	{
		if ( pFile->Delete(pFileName) == false )
		{
			eError = FM_FILE_NOT_FOUND;
		}
	}
	else
	{
		eError = FM_FILE_NOT_FOUND;
	}
	delete pFile;

	return ( eError );
}

//-----------------------------------------------------------------------------
//	Name:		Rename
//	Object:		
//	03-12-05:	AMA - Created
//-----------------------------------------------------------------------------
FILES_MANAGER_ABC::FM_ERROR	FILES_MANAGER_ABC::Rename(const char * const	pFileName)
{
#if defined(PLATFORM_PC)
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		FILE_PC_W2K * pFile = new FILE_PC_W2K;
		FM_ERROR		eError = FM_FILE_NO_ERROR;
		if ( pFile->IsExist(pFileName) == true )
		{
			if ( pFile->Rename(pFileName) == false )
			{
				eError = FM_FILE_NOT_FOUND;
			}
		}
		else
		{
			eError = FM_FILE_NOT_FOUND;
		}

		return ( eError );
	}
	else
	{
		FILE_PC_W98 * pFile = new FILE_PC_W98;
		FM_ERROR		eError = FM_FILE_NO_ERROR;
		if ( pFile->IsExist(pFileName) == true )
		{
			if ( pFile->Rename(pFileName) == false )
			{
				eError = FM_FILE_NOT_FOUND;
			}
		}
		else
		{
			eError = FM_FILE_NOT_FOUND;
		}

		return ( eError );
	}
#else
#ifndef _MASTER
	QDT_STRING S;
	S.Format("You can't rename file %s on this plateform.", pFileName);
	
	QDT_Error(S);
#endif
	return FM_FILE_NO_ERROR;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsFileExist
//	Object:		
//	03-05-21:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::IsFileExist(const char * const	pFileName)
{
	bool bResult;
	FILE_ABC * pFile;
	if (GetRemoteMode())
	{
		pFile = new REMOTE_FILE();
	}
	else
	{
#if defined(PLATFORM_PC)
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}
#elif defined (PLATFORM_PS3)
		pFile = new FILE_PS3_HOST;
#elif defined (PLATFORM_X360)
		pFile = new FILE_X360;
#endif
	}
	
	bResult = pFile->IsExist(pFileName);
	delete pFile;
	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		IsFileWriteProtected
//	Object:		
//	03-05-21:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::IsFileWriteProtected(const char * const	pFileName)
{
	bool			bResult;
	FILE_ABC * pFile;
#if defined(PLATFORM_PC)
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}
#elif defined (PLATFORM_PS3)
	pFile = new FILE_PS3_HOST;
#elif defined (PLATFORM_X360)
	pFile = new FILE_X360;
#endif
	bResult = pFile->IsWriteProtected(pFileName);
	delete pFile;
	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		DeepCopy
//	Object:		copy all files and directories from one root directory to a dest directory
//	03-06-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::DeepCopy(const QDT_STRING &	sSrcDir,
									const QDT_STRING &	sDestDir,
									const bool			bInteractive)
{
#if defined(PLATFORM_PC)
	QDT_ASSERT(sSrcDir.IsEmpty() == false);
	QDT_ASSERT(sDestDir.IsEmpty() == false);

	WIN32_FIND_DATA	FindData;
	HANDLE			hRes;
	QDT_STRING		sErrorMsg;
	QDT_STRING		sSrcFile, sDestFile;
	QDT_STRING		sFileName;
	BOOL			bOK = TRUE;
	QDT_STRING		sSrcDirectory;
	QDT_STRING		sDestDirectory;

	sSrcDirectory	= sSrcDir;
	sDestDirectory	= sDestDir;
	
	//	Append ending backslash if none found
	if	(sSrcDir[sSrcDir.GetSize()-1] != '\\')
	{
		sSrcDirectory = sSrcDir + "\\";
	}
	
	if	(sDestDir[sDestDir.GetSize()-1] != '\\')
	{
		sDestDirectory = sDestDir + "\\";
	}

	// Create the directory if it does not already exist.
	if ( GetFileAttributes( sDestDirectory ) == 0xFFFFFFFF )
	{
		if ( CreateDirectory( sDestDirectory, NULL ) == FALSE )
		{
			if	(bInteractive)
			{
				sErrorMsg.Format( "Failed creating the directory '%s', can't copy file(s).", sDestDirectory.GetBuffer() );
				MessageBox( NULL, sErrorMsg, "ERROR", MB_ICONSTOP );
			}
			return	(false);
		}
	}

	hRes = ::FindFirstFile(sSrcDirectory + "*", &FindData);
	if	(hRes == INVALID_HANDLE_VALUE)
	{		
		if	(bInteractive)
		{
			sErrorMsg.Format("The directory %s containing the files is missing", sSrcDirectory.GetBuffer());
			MessageBox(NULL, sErrorMsg.GetBuffer(), "Error copying files",MB_ICONSTOP);
		}

		::FindClose(hRes);
		return	(false);
	}
	

	while	(bOK && (hRes != INVALID_HANDLE_VALUE))
	{
		sFileName = FindData.cFileName;

		if	((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{	
			sDestFile = sDestDirectory + sFileName;
			sSrcFile = sSrcDirectory + sFileName;

			//	Remove Readonly status
			SetFileAttributes(sSrcFile, FILE_ATTRIBUTE_NORMAL);
			SetFileAttributes(sDestFile, FILE_ATTRIBUTE_NORMAL);
			
			// Copy file.
			if ( ::CopyFile( sSrcFile, sDestFile, FALSE ) == FALSE )
			{
				if	(bInteractive)
				{
					sErrorMsg.Format("Failed copying the file %s", sSrcFile);
					MessageBox(NULL, sErrorMsg.GetBuffer(), "Error copying files", MB_ICONSTOP);
				}
				::FindClose(hRes);
				return	(false);
			}
		}
		else
		{
			if	(sFileName != "." && sFileName != "..")
			{
				QDT_STRING sNewSrcDir	= sSrcDirectory + sFileName;
				QDT_STRING sNewDestDir	= sDestDirectory + sFileName;

				if	(DeepCopy(sNewSrcDir, sNewDestDir) == false)
				{
					::FindClose(hRes);
					return	(false);
				}
			}
		}

	
		bOK = FindNextFile(hRes, &FindData);
	}

	::FindClose(hRes);
	return	(true);
#else
	QDT_NOT_IMPLEMENTED();
	return	(false);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CopyFile
//	Object:		
//	06-05-23:	ALO - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::FileCopy(const QDT_STRING &	sSrcName,
									const QDT_STRING &	sDestName)
{
#if defined(PLATFORM_PC)
	QDT_ASSERT(sSrcName.IsEmpty() == false);
	QDT_ASSERT(sDestName.IsEmpty() == false);

	WIN32_FIND_DATA	FindData;
	HANDLE			hRes;
	QDT_STRING		sErrorMsg;
	BOOL			bOK = TRUE;

	hRes = ::FindFirstFile(sSrcName, &FindData);
	if	(hRes == INVALID_HANDLE_VALUE)
	{		
		sErrorMsg.Format("The file %s is missing", sSrcName.GetBuffer());
		MessageBox(NULL, sErrorMsg.GetBuffer(), "Error copying file", MB_ICONSTOP);

		::FindClose(hRes);
		return	(false);
	}

	// Copy file.
	if ( ::CopyFile( sSrcName, sDestName, FALSE ) == FALSE )
	{		
		sErrorMsg.Format("Failed copying the file %s", sSrcName);
		MessageBox(NULL, sErrorMsg.GetBuffer(), "Error copying file", MB_ICONSTOP);

		::FindClose(hRes);
		return	(false);
	}

	::FindClose(hRes);
	return	(true);
#else
	QDT_NOT_IMPLEMENTED();
	return	(false);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		DeleteDir
//	Object:		
//	04-02-23:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::DeleteDir(const QDT_STRING &	sSrcDir,
									 const QDT_STRING &	sExtMask,
									 const bool			bWarn,
									 const bool			bDelSrcDir)
{
#if defined(PLATFORM_PC)
	bool bRecWarn = bWarn;

	if	(bWarn)
	{
		QDT_ASSERT(sSrcDir.IsEmpty() == false);
		if (sExtMask.IsEmpty() || (sExtMask == "*.*"))
		{
			QDT_STRING sError;
			sError.Format("You are about to delete all files in dir %s. Do you want to abort ?", sSrcDir.GetBuffer());
			if (MessageBox(NULL, sError.GetBuffer(), "WARNING: before erasing directory", MB_ICONSTOP | MB_YESNO) == IDYES)
			{
				return (false);
			}
			else
			{
				bRecWarn = false;
			}
		}
	}

	WIN32_FIND_DATA	FindData;
	HANDLE			hRes;
	QDT_STRING		sErrorMsg;
	QDT_STRING		sSrcFile;
	QDT_STRING		sFileName;
	BOOL			bOK = TRUE;
	QDT_STRING		sSrcDirectory;

	sSrcDirectory	= sSrcDir;
	
	//	Append ending backslash if none found
	if	(sSrcDir[sSrcDir.GetSize()-1] != '\\')
	{
		sSrcDirectory = sSrcDir + "\\";
	}
	
	hRes = ::FindFirstFile(sSrcDirectory + sExtMask, &FindData);
	if	(hRes == INVALID_HANDLE_VALUE)
	{
//		sErrorMsg.Format("The directory %s is missing", sSrcDirectory.GetBuffer());
//		MessageBox(NULL, sErrorMsg.GetBuffer(), "Error cleaning files", MB_ICONSTOP);
		return	(false);
	}

	while	(bOK && (hRes != INVALID_HANDLE_VALUE))
	{
		sFileName = FindData.cFileName;

		if	((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{	
			sSrcFile = sSrcDirectory + sFileName;

			//	Remove Readonly status
			SetFileAttributes(sSrcFile, FILE_ATTRIBUTE_NORMAL);
			
			// Delete file.
			if ( ::DeleteFile(sSrcFile) == FALSE )
			{
//				sErrorMsg.Format("Failed deleting the file %s", sSrcFile);
//				MessageBox(NULL, "Error deleting files", sErrorMsg.GetBuffer(), MB_ICONSTOP);
				//return	(false);
//				continue;
			}
		}
		else
		{
			if	(sFileName != "." && sFileName != "..")
			{
				QDT_STRING sNewSrcDir = sSrcDirectory + sFileName;

				if	(DeleteDir(sNewSrcDir, sExtMask, bRecWarn) == false)
				{
					::FindClose(hRes);
					return	(false);
				}
			}
		}

		bOK = FindNextFile(hRes, &FindData);
	}

	::FindClose(hRes);

	if ( bDelSrcDir )
	{
		if ( ::RemoveDirectory(sSrcDir) == 0 )
		{		
			return	(false);
		}
	}

	return	(true);
#else
	QDT_NOT_IMPLEMENTED();
	return	(false);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetFileSize
//	Object:		returns the size of a file
//	03-08-19:	DAC - Created
//-----------------------------------------------------------------------------
int	FILES_MANAGER_ABC::GetFileSize(const char * const pFileName)
{
	int		nSize = 0;

#ifdef PLATFORM_PC
	QDT_ASSERT(pFileName);

	// Check the size
	HANDLE	hFile = NULL;

	hFile = CreateFile( pFileName, 
						GENERIC_READ, 
						FILE_SHARE_READ | FILE_SHARE_WRITE, 
						NULL, 
						OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL, 
						NULL);
	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		nSize = ::GetFileSize(hFile, NULL);
		::CloseHandle(hFile);
	}
#else
	QDT_NOT_IMPLEMENTED();
#endif

	return	(nSize);
}


//-----------------------------------------------------------------------------
//	Name:		GetModuloPositionAndSize
//	Object:		
//	04-04-29:	DAC - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::GetModuloPositionAndSize(UInt32		nPosition,
													UInt32		nSize,
													UInt32 &	nModuloPosition,
													UInt32 &	nModuloSize)
{
	FILE_ABC * pFile;
#if defined(PLATFORM_PC)
	
	if ( _bWinNT )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}
#elif defined (PLATFORM_PS3)
	pFile = new FILE_PS3_HOST;
#elif defined (PLATFORM_X360)
	pFile = new FILE_X360;
#endif
	
	pFile->GetModuloPositionAndSize(nPosition, nSize, nModuloPosition, nModuloSize);
	delete pFile;
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableEmulateCD
//	Object:		
//	04-05-03:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::GetEnableEmulateCD()
{
	return ( _bEnableEmulateCD );
}

//-----------------------------------------------------------------------------
//	Name:		SetEnableEmulateCD
//	Object:		
//	04-05-03:	AMA - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::SetEnableEmulateCD(bool	bVal)
{
	_bEnableEmulateCD = bVal;
}

//-----------------------------------------------------------------------------
//	Name:		IsFileReadOnly
//	Object:		
//	04-09-29:	DAC - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::IsFileReadOnly(const char * const	pFileName)
{
#if defined(PLATFORM_PC)
	DWORD	Attributes;
	
	Attributes = GetFileAttributes( pFileName );
	QDT_ASSERT( Attributes != 0xFFFFFFFF );
  
	if ( Attributes & FILE_ATTRIBUTE_READONLY )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
#else
	QDT_NOT_IMPLEMENTED();
	return	(true); // assume it is readonly on console platforms
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsDirectoryExist
//	Object:		
//	04-12-21:	DAC - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::IsDirectoryExist(const char * const	pDirName)
{
#ifdef PLATFORM_PC
	// 
	if ( GetFileAttributes( pDirName ) == 0xFFFFFFFF )
	{
		return	(false);
	}
	else
	{
		return	(true);
	}
#else
	return	(false);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CreateDir
//	Object:		
//	04-12-21:	DAC - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::CreateDir(const char * const	pDirName)
{
#ifdef PLATFORM_PC
	
	DWORD					FileAttributes;
	QDT_STRING_TOKENIZER	Tokenizer;
	int						i, nNbrTokens;
	QDT_STRING				sCurSubDir;
	QDT_STRING				sCurDir;

	QDT_STRING sPath = pDirName;
	
	sPath.TrimLeft();
	sPath.TrimRight();
	
	// Computer name in path
	if	(sPath[0] == '\\' && sPath[1] == '\\')
	{
		// remove "\\"
		sPath = sPath.EraseLeft(2);
		sCurDir += "\\\\";

		// find next "\"
		int nPos = sPath.Find("\\");
		QDT_ASSERT(nPos != -1);
		
		// and finish removing computer name
		sCurDir += sPath.ExtractLeft(nPos + 1);
		sPath = sPath.EraseLeft(nPos + 1);
	}

	Tokenizer.Tokenize( sPath, "\\");
	
	nNbrTokens = Tokenizer.GetNbrTokens();
	for	(i = 0; i < nNbrTokens; ++i)
	{
		sCurSubDir = Tokenizer.GetToken(i);
		
		// merge subdir and try to create it
		sCurDir += sCurSubDir;
		sCurDir += "\\";
		
		if	(sCurSubDir.Find(":") == -1)	// if not E: D: C: ,test and create if needed
		{
			// if sub dir doesn't exist, create it.
			FileAttributes = ::GetFileAttributes( sCurDir );
			if ( ( FileAttributes == 0xFFFFFFFF ) ||					// Directory doesn't exist.
					( (FileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 ) )	// Filename is not a directory.
			{
				if ( ::CreateDirectory( sCurDir, NULL ) == FALSE )
				{
					return (false);
				}
			}
		}
	}
	
	return	(true);
	
#else
	return	(false);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetWinNT
//	Object:		
//	05-04-21:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILES_MANAGER_ABC::GetWinNT()
{
	return ( _bWinNT );
}

//-----------------------------------------------------------------------------
//	Name:		GetWinNT
//	Object:		
//	06-04-28:	GRI - Created
//-----------------------------------------------------------------------------
void	FILES_MANAGER_ABC::SetRemoteMode(bool bRemoteMode)
{
	if (bRemoteMode && !_pRemoteFileService)
	{
		QDT_Message("! Set to remote but service not ok");
		return;
	}
	_bRemoteMode = bRemoteMode;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
