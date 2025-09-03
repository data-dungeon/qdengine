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
//	CLASS:	MEMORYCARD_MANAGER
//
//	03-11-06:	VMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

#include INC_KRNCORE(Stream)

#ifdef PLATFORM_PC
	#include INC_KRNCORE(File/PC)
	#undef	 DeleteFile		// conflit avec windows.h
#endif

#ifdef PLATFORM_XBOX
	#include INC_KRNCORE(File/XBox)
	#undef	 DeleteFile		// conflit avec windows.h
	#define	 BLOCK_SIZE 16384
#endif

#ifdef PLATFORM_PS2
	#include INC_KRNCORE(File/PS2)
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#if defined (PLATFORM_PC) || defined(PLATFORM_XBOX)
	static HANDLE	_hFindRes; //for finding saves
#endif

//-----------------------------------------------------------------------------
//	Name:		FILES_MANAGER constructor
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
MEMORYCARD_MANAGER::MEMORYCARD_MANAGER()
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		FILES_MANAGER destructor
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
MEMORYCARD_MANAGER::~MEMORYCARD_MANAGER()
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		CreateSaveFileName
//	Object:		
//	04-12-20:	JPE - Created
//-----------------------------------------------------------------------------
#ifdef PLATFORM_XBOX
void	MEMORYCARD_MANAGER::CreateSaveFileName(	char *pBuffer, 
												const int nBufferSize, 
												const char * const pFileName )
{
	char				TmpFile[MAX_PATH];
	static const char	Name[] = { 'M',0,'A',0,'I',0,'N', 0, 0, 0, 0 };
	int					Error = XCreateSaveGame(	"U:\\", 
													(LPCWSTR)Name, 
													OPEN_ALWAYS, 
													0, 
													TmpFile, 
													MAX_PATH);

	if(strlen(TmpFile)+strlen(pFileName)+1 < nBufferSize)
		sprintf(pBuffer, "%s%s", TmpFile, pFileName);
	else
		pBuffer[0] = 0;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetFreeSize
//	Object:		
//	05-04-01:	JPE - Created
//-----------------------------------------------------------------------------
int	MEMORYCARD_MANAGER::GetFreeSize(void)
{
#if defined (PLATFORM_XBOX)
    ULARGE_INTEGER lFreeBytesAvailable;
    ULARGE_INTEGER lTotalNumberOfBytes;
    ULARGE_INTEGER lTotalNumberOfFreeBytes;

	BOOL bSuccess = GetDiskFreeSpaceEx( "U:\\",
                                        &lFreeBytesAvailable,
                                        &lTotalNumberOfBytes,
                                        &lTotalNumberOfFreeBytes );

    if( !bSuccess )
        return 0;

    ULONGLONG qwTotalBytes;
    ULONGLONG qwUsedBytes;
    ULONGLONG qwFreeBytes;

    qwTotalBytes = lTotalNumberOfBytes.QuadPart;
    qwFreeBytes  = lFreeBytesAvailable.QuadPart;
    qwUsedBytes  = qwTotalBytes - qwFreeBytes;

    DWORD dwBlockSize   = BLOCK_SIZE;
    DWORD dwTotalBlocks = DWORD( qwTotalBytes / dwBlockSize );
    DWORD dwUsedBlocks  = DWORD( qwUsedBytes / dwBlockSize );
    DWORD dwFreeBlocks  = dwTotalBlocks - dwUsedBlocks;

	return dwFreeBlocks;
#elif defined (PLATFORM_PS2)
	return ( FILE_PS2_MEMORYCARD::GetFreeSpace() );
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		ExternalAppFreeSpace
//	Object:		
//	05-04-04:	JPE - Created
//-----------------------------------------------------------------------------
void	MEMORYCARD_MANAGER::ExternalAppFreeSpace(int nbBlocksNeeded)
{
#if defined(PLATFORM_XBOX)
	LD_LAUNCH_DASHBOARD	LauchInfo;
	LauchInfo.dwContext		= 0;
	LauchInfo.dwReason		= XLD_LAUNCH_DASHBOARD_MEMORY;
	LauchInfo.dwParameter1	= 'U';
	LauchInfo.dwParameter2	= nbBlocksNeeded;
	XLaunchNewImage(NULL, (LAUNCH_DATA*)&LauchInfo);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		ComputeSignature
//	Object:		
//	04-12-20:	JPE - Created
//-----------------------------------------------------------------------------
char *	MEMORYCARD_MANAGER::ComputeSignature(MEM_STREAM &		inBuffer,
											  unsigned int		nSize,
											  unsigned int &	nSigSize)
{
	QDT_ASSERT (nSize <= inBuffer.GetSize())

	QDT_CRC_32		CRC32;

	nSigSize	= sizeof(unsigned int);

	char *pCRC = new char[nSigSize];
	CRC32.BufferCrc32( (const void*)inBuffer.GetBuffer(), nSize, *((unsigned int*)pCRC) );

	return (char*)pCRC;
}

//-----------------------------------------------------------------------------
//	Name:		GetSignatureSize
//	Object:		
//	05-02-17:	VPI - Created
//-----------------------------------------------------------------------------
unsigned int	MEMORYCARD_MANAGER::GetSignatureSize()
{
	unsigned int nSigSize = 0;
	nSigSize	= sizeof(unsigned int);
	return (nSigSize);
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
FILE_ABC *	MEMORYCARD_MANAGER::OpenFile(const char * const		pFileName,
										 const FILE_ABC::F_MODE	eMode,
										 MC_ERROR &				eError)
{
	eError = MC_NO_ERROR;

	FILE_ABC * pFile;
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}

	if ( pFile->Open(pFileName, eMode, false) == true )
	{
		return (pFile);
	}
	else
	{
		eError = MC_OPEN_FAILED;
		delete pFile;
		return ( NULL );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
MEMORYCARD_MANAGER::MC_ERROR	MEMORYCARD_MANAGER::CloseFile(FILE_ABC *	pFile)
{
	MC_ERROR eError = MC_NO_ERROR;
	
	QDT_ASSERT( pFile );
	QDT_ASSERT( pFile->IsIdle() );
	
	if ( pFile->Close() == true )
	{
		delete pFile;
		return ( eError );
	}
	else
	{
		eError = MC_CLOSE_FAILED;
		delete pFile; //delete quand même ?
		return ( eError );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
MEMORYCARD_MANAGER::MC_ERROR	MEMORYCARD_MANAGER::DeleteFile(const char * const	pFileName)
{
	FILE_ABC * pFile;
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}

	MC_ERROR		eError = MC_NO_ERROR;

	if ( pFile->IsExist(pFileName) == true )
	{
		if ( pFile->Delete(pFileName) == false )
		{
			eError = MC_FILE_NOT_FOUND;
		}
	}
	else
	{
		eError = MC_FILE_NOT_FOUND;
	}
	delete pFile;

	return ( eError );
}

//-----------------------------------------------------------------------------
//	Name:		IsFileExist
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::IsFileExist(const char * const	pFileName,
										MC_ERROR &			eError)
{
	bool			bResult;
	
	eError	= MC_NO_ERROR;

	FILE_ABC * pFile;
	if ( FILES_MANAGER_ABC::GetWinNT() )
	{
		pFile = new FILE_PC_W2K;
	}
	else
	{
		pFile = new FILE_PC_W98;
	}
	
	bResult = pFile->IsExist(pFileName);
	delete pFile;

	return ( bResult );
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstFileName
//	Object:		
//	04-06-09:	VPI - Created
//-----------------------------------------------------------------------------
QDT_STRING	MEMORYCARD_MANAGER::GetFirstFileName(const QDT_STRING &	sFindStr,
												 MC_ERROR &			eError)
{
	QDT_STRING sName;
	eError = MC_NO_ERROR;

	WIN32_FIND_DATA	FindData;
	_hFindRes = ::FindFirstFile(sFindStr, &FindData);
	if	(_hFindRes != INVALID_HANDLE_VALUE)
	{
		sName = FindData.cFileName;
	}

	return (sName);
}

//-----------------------------------------------------------------------------
//	Name:		GetNextFileName
//	Object:		
//	04-06-09:	VPI - Created
//-----------------------------------------------------------------------------
QDT_STRING	MEMORYCARD_MANAGER::GetNextFileName(const QDT_STRING &	sFindStr,
												MC_ERROR &			eError)
{
	QDT_STRING sName;
	eError = MC_NO_ERROR;

#if defined(PLATFORM_PC) || defined(PLATFORM_XBOX)
	WIN32_FIND_DATA	FindData;
	BOOL bOK = FindNextFile(_hFindRes, &FindData);
	if (bOK && (_hFindRes != INVALID_HANDLE_VALUE))
	{
		sName = FindData.cFileName;
	}
#elif defined(PLATFORM_PS2)
	sName = FILE_PS2_MEMORYCARD::GetNextFileName();	

	while ((sName.IsEmpty() == false) && (sName.Find(sFindStr) == -1))
	{
		sName = FILE_PS2_MEMORYCARD::GetNextFileName();
	}
	if (sName.Find(sFindStr) == -1)
	{
		return ("");
	}
#endif

	return (sName);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::Init(MC_ERROR & eError)
{
	eError = MC_NO_ERROR;
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		FormatMemoryCard
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::FormatMemoryCard( MC_ERROR & eError )
{
	eError = MC_NO_ERROR;
#if defined(PLATFORM_PC)
	return (true);
#elif defined(PLATFORM_XBOX)
	return (true);
#elif defined(PLATFORM_PS2)
	return (FILE_PS2_MEMORYCARD::Format( eError ));
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsMemoryCardReady
//	Object:		
//	03-11-06:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::IsMemoryCardReady( int nNeededSpace, MC_ERROR & eError )
{
	eError = MC_NO_ERROR;
#if defined(PLATFORM_PC)
	return (true);
#elif defined(PLATFORM_XBOX)
	return (true);
#elif defined(PLATFORM_PS2)
	return (FILE_PS2_MEMORYCARD::IsReady( nNeededSpace, eError ));
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsMemoryCardPrepared
//	Object:		
//	03-11-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::IsMemoryCardPrepared(MC_ERROR & eError)
{
	eError = MC_NO_ERROR;
#if defined(PLATFORM_PC)
	return (true);
#elif defined(PLATFORM_XBOX)
	return (true);
#elif defined(PLATFORM_PS2)
	return (FILE_PS2_MEMORYCARD::IsPrepared());
#endif	
}

//-----------------------------------------------------------------------------
//	Name:		PrepareMemoryCard
//	Object:		
//	03-11-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::PrepareMemoryCard( MC_ERROR &	eError )
{
	eError = MC_NO_ERROR;
#if defined(PLATFORM_PC)
	return (true);
#elif defined(PLATFORM_XBOX)
	return (true);
#elif defined(PLATFORM_PS2)
	return (FILE_PS2_MEMORYCARD::Prepare(eError));
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CleanMemoryCard
//	Object:		
//	03-11-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	MEMORYCARD_MANAGER::CleanMemoryCard(MC_ERROR &	eError)
{
	eError = MC_NO_ERROR;
#if defined(PLATFORM_PC)
	return (true);
#elif defined(PLATFORM_XBOX)
	return (true);
#elif defined(PLATFORM_PS2)
	return (FILE_PS2_MEMORYCARD::Clean(eError));
#endif
}
//=============================================================================
//	CODE ENDS HERE
//=============================================================================

