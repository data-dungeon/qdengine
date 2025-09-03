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
//	CLASS:	FILE_X360
//
//	02-02-14:	AMA - Created 
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(File/X360/FileX360)
#include	INCL_KCORE(File/FilesManagerABC)

#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(OS/OS)

#define		QDT_INCLUDE_X360_XTL
#include	INCL_KCORE(Includes_X360/Includes_X360)

#define READ_SPEED		1.f * 1024.f*1024.f / 1000.f			// 1 Mo/s
//#define READ_SPEED		100.f * 1024.f*1024.f / 1000.f			// 100 Mo/s

//-----------------------------------------------------------------------------
//	Name:		FILE_PC constructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_X360::FILE_X360()
{
	_nReadSize = 0;
	_nReadPos = 0;
	_nCurPos = 0;
	_pReadBuffer = NULL;
	_pOverlapped=new OVERLAPPED;
	_rLastReadTime = 0.f;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_PC destructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_X360::~FILE_X360()
{
	delete(_pOverlapped);
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Open(const char * const	pFileName,
						  const F_MODE			eMode,
						  const bool			bSharing)
{
	QDT_ASSERT( TestStatus(FILE_ABC::FS_OPEN) == false );
	DWORD dwSharingMode = 0;
	DWORD dwFlagsAndAttributes = 0;
	DWORD dwCreationDisposition = 0;

	dwSharingMode = FILE_SHARE_READ;

	char			FullPathName[256];

	strcpy(FullPathName, HOME_DIR);
	strcat(FullPathName, pFileName);

	SetName(FullPathName);

	if ( TestStatus(FILE_ABC::FS_OPEN) == false )
	{
		EnableStatus(FILE_ABC::FS_OPEN);
		DWORD dwDesiredAccess = 0;
		
		switch (eMode)
		{
		case FM_READ : 
			dwDesiredAccess = GENERIC_READ;
			dwCreationDisposition = OPEN_EXISTING;
			dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED;//|FILE_FLAG_NO_BUFFERING;
			break;
		case FM_WRITE : 
			dwDesiredAccess = GENERIC_WRITE;
			dwCreationDisposition = CREATE_ALWAYS;
			dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED;
			break;
		case FM_READ_WRITE : 
			dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			dwCreationDisposition = OPEN_EXISTING;
			dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED;
			break;
		case FM_CREATE_READ_WRITE : 
			dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			dwCreationDisposition = CREATE_ALWAYS;
			dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED;
			break;
		}
		
		if (bSharing)
		{
			dwSharingMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		}

		_Handle = CreateFile(FullPathName, dwDesiredAccess, dwSharingMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);

		if (_Handle != INVALID_HANDLE_VALUE)
		{
			SetSize(GetFileSize(_Handle, NULL));
			SetPosition(0);
			return ( true );
		}
		else
		{
			QDT_STRING sMessage;
			sMessage.Format("Can't open File %s : error=%d", FullPathName, GetLastError() );
			QDT_Message(sMessage);
			return ( false );
		}
	}
	else
	{
		QDT_STRING sError;
		sError.Format("File % allready open:", FullPathName );
		QDT_Message(sError);
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Close()
{
	QDT_ASSERT( TestStatus(FILE_ABC::FS_OPEN) == true );

	if ( TestStatus(FILE_ABC::FS_OPEN) == true )
	{
		int nErr;

		nErr = CloseHandle(_Handle);
		if ( nErr == 0 )
		{
			return ( false );
		}
		else
		{
			DisableStatus(FILE_ABC::FS_OPEN);
			return ( true );
		}
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsExist
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::IsExist(const char * const	pFileName) const
{
	WIN32_FIND_DATA FindFile;
	char			FullPathName[256];

	strcpy(FullPathName, HOME_DIR);
	strcat(FullPathName, pFileName);

	HANDLE handle = FindFirstFile(FullPathName, &FindFile);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return ( false );
	}
	else
	{
		FindClose(handle);
		return ( true );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsWriteProtected
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::IsWriteProtected(const char * const	pFileName) const
{
	WIN32_FIND_DATA FindFile;
	char			FullPathName[256];

	strcpy(FullPathName, HOME_DIR);
	strcat(FullPathName, pFileName);
	HANDLE handle = FindFirstFile(FullPathName, &FindFile);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return ( false );
	}
	else
	{
		bool bResult;
		bResult = (FindFile.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
		FindClose(handle);
		return ( bResult );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Seek
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Seek(const F_SEEK	eInitialPosition,
						 const unsigned int		nOffset)
{
	//QDT_ASSERT( ( nOffset % SECTOR_SIZE ) == 0 );

	UInt32 nSize = GetSize();
	UInt32 nCurrentPosition = GetCurrentPosition();
	
	switch (eInitialPosition)
	{
	case FS_BEGIN:	
		QDT_ASSERT( nOffset >= 0 );
		SetPosition(nOffset);
		break;

	case FS_CURRENT:
		QDT_ASSERT( ( nCurrentPosition + nOffset ) < nSize );
		QDT_ASSERT( ( nCurrentPosition + nOffset ) >= 0 );
		SetPosition( nCurrentPosition + nOffset );	
		break;

	case FS_END:
		QDT_ASSERT( nOffset == 0 );
		SetPosition( nSize + nOffset );	
		break;
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	04-04-23:	CVI - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Read(void *				pBuffer,
						 const unsigned int	nPosition,
						 const int			nSize,
						 const F_SYNC		eSync)
{
	//QDT_ASSERT( nSize % SECTOR_SIZE == 0 );		//zog

	if ( FILES_MANAGER_ABC::Instance()->GetEnableEmulateCD() == false )
	{
		return ( ReadInternal(pBuffer, nPosition, nSize, eSync) );
	}

	_nReadSize   = nSize;
	_nReadPos    = nPosition;
	_pReadBuffer = (char*) pBuffer;
	_nCurPos     = 0;

	_rLastReadTime = (Float32)(TIME::Instance()->GetSystemTime());

	if ( eSync == FS_WAIT )
	{
		while( !IsIdle() );
	}

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::ReadInternal(void *		pBuffer,
						  const unsigned int		nPosition,
						  const int		nSize,
						  const F_SYNC	eSync)

{
	BOOL bResult = FALSE;
	unsigned long nBytesRead = 0;
	DWORD nLastError = 0;

	//QDT_ASSERT( nSize % SECTOR_SIZE == 0 );

	((OVERLAPPED*)_pOverlapped)->hEvent = NULL;
	((OVERLAPPED*)_pOverlapped)->Offset = nPosition;
	((OVERLAPPED*)_pOverlapped)->OffsetHigh = 0;

	if ( Seek(FS_BEGIN, nPosition) == true )	
	{
		//SetFilePointer(_Handle, nPosition, NULL, FILE_BEGIN);
		bool bPending;
		int nWatchDog = 2000;
		int	RequestReadOperation = true;
		do
		{
			bPending = false;
			nWatchDog--;
			if	(nWatchDog == 0)
			{
				QDT_Error("File %s is locked can't read", GetName().GetBuffer());
				nWatchDog = 2000;
			}

			if(RequestReadOperation)
			{
				bResult = ReadFile(_Handle, pBuffer, nSize, &nBytesRead, (OVERLAPPED*)_pOverlapped);
				RequestReadOperation = false;
				if(bResult == FALSE)
				{
					nLastError = ::GetLastError();
				}
			}

			if ( bResult == FALSE )
			{
				if ( nLastError == ERROR_HANDLE_EOF )
				{
					// Reached EoF
					bResult = TRUE;
				}
				else if ( nLastError == ERROR_IO_PENDING )
				{
					bResult = GetOverlappedResult(_Handle, (OVERLAPPED*)_pOverlapped, &nBytesRead, 0);

					if (!bResult) 
					{ 
						nLastError = ::GetLastError();
						// deal with the error code 
						switch ( nLastError )
						{ 
							case ERROR_HANDLE_EOF: 
							{ 
								// Reached EoF
								bResult = TRUE;
								break;
							} 

							case ERROR_IO_INCOMPLETE:
								bPending = true;
								break;
 
							default:
								RequestReadOperation = true;
								bPending = true;
								if ( eSync == FS_WAIT )
								{
									while( !IsIdle() );
									bResult = TRUE;
								}
								break;
						} 
					}
					else
					{
						// faire un loop si nBytesRead < ce qu'on a demander
						bResult = TRUE;
					}

				}
				else if( nLastError == ERROR_IO_INCOMPLETE)
				{
					Sleep(10);
					bPending = true;
					bResult = GetOverlappedResult(_Handle, (OVERLAPPED*)_pOverlapped, &nBytesRead, 0);
				}
				else
				{
					QDT_STRING sPath, sName;
					QDT_STRING::SplitPathFileName(GetName(), sPath, sName);

					QDT_STRING sMessage;
					OS::GetWinLastErrorMsg(sMessage);
					QDT_Error("FILE_X360::ReadInternal() %s : %s", sMessage.GetBuffer(), sName.GetBuffer() );

				}
			}
		} while( bPending );
	}

	if ( bResult )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Break
//	Object:		
//	04-06-30:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Break()
{
	if ( FILES_MANAGER_ABC::Instance()->GetEnableEmulateCD() )
	{
		_nReadSize = _nCurPos;
		return ( true );
	}
	else
	{
		if ( CancelIo(_Handle) == 0 )
		{
			return ( false );
		}

		return ( true );
	}
}


//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Write(void *		pBuffer,
						  const unsigned int		nPosition,
						  const int		nSize,
						  const F_SYNC	eSync)

{
	BOOL bResult = FALSE;
	unsigned long nBytesWritten = 0;
	DWORD nLastError = 0;

	((OVERLAPPED*)_pOverlapped)->hEvent = NULL;
	((OVERLAPPED*)_pOverlapped)->Offset = nPosition;
	((OVERLAPPED*)_pOverlapped)->OffsetHigh = 0;
    ((OVERLAPPED*)_pOverlapped)->Internal = 1;

	if ( Seek(FS_BEGIN, nPosition) == true )	
	{
		bResult = WriteFile(_Handle, pBuffer, nSize, &nBytesWritten, (OVERLAPPED*)_pOverlapped);

		if ( bResult == FALSE )
		{
			nLastError = ::GetLastError();

			if ( nLastError == ERROR_IO_PENDING )
			{
				if ( eSync == FS_WAIT )
				{
					while( !IsIdle() );
					bResult = TRUE;
				}
			}
			else
			{
				if ( nLastError == 112 )
				{
					QDT_STRING sMessage;
					sMessage = "There is not enough space on the disk. Retry?";
					if ( QDT_MessageBox(NULL, sMessage, "ERROR", QDT_YESNO ) == QDT_IDYES )
					{
						Write(pBuffer, nPosition, nSize, eSync);
					}
				}
				else
				{
					QDT_STRING sPath, sName;
					QDT_STRING::SplitPathFileName(GetName(), sPath, sName);
					QDT_STRING sMessage;
					OS::GetWinLastErrorMsg(sMessage);
					QDT_Error("FILE_X360::Write() %s : %s", sMessage.GetBuffer(), sName.GetBuffer() );
				}
			}
		}
	}
	
	if ( bResult )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
}
 
//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Delete(const char * const	pFileName) const
{
	BOOL bError;
	char			FullPathName[256];

	strcpy(FullPathName, HOME_DIR);
	strcat(FullPathName, pFileName);

	bError = DeleteFile(FullPathName);

	if ( bError == 0 ) //error
	{
		return ( false );
	}
	else
	{
		return ( true );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Rename
//	Object:		
//	03-12-05:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::Rename(const char * const	pFileName) const
{
	BOOL bError;

	QDT_STRING S;

	S.Format("%s.bak", pFileName);
	
	bError = CopyFile(pFileName, S.GetBuffer(), FALSE);

	if ( bError == 0 )
	{
		return ( false );
	}
	else
	{
		return ( true );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_X360::Loop()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		IsIdle
//	Object:		
//	02-02-27:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::IsIdle() const
{
	if ( FILES_MANAGER_ABC::Instance()->GetEnableEmulateCD() )
	{
		((FILE_X360*)this)->UpdateRead();

		return ( _nCurPos >= _nReadSize );
	}
	else
	{
		return HasOverlappedIoCompleted((OVERLAPPED*)_pOverlapped);
	}
}


//-----------------------------------------------------------------------------
//	Name:		GetSectorSize
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
int		FILE_X360::GetSectorSize() const
{
	return SECTOR_SIZE;
}

//-----------------------------------------------------------------------------
//	Name:		SetName
//	Object:		
//	03-09-10:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_X360::SetName(const QDT_STRING &	sName)
{
	_sName = sName;
}

//-----------------------------------------------------------------------------
//	Name:		GetName
//	Object:		
//	03-09-10:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	FILE_X360::GetName() const
{
	return	(_sName);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentReadPosition
//	Object:		
//	04-03-23:	VMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILE_X360::GetCurrentReadPosition() const
{
	return 0;
	// This commented code is obsolete. Return 0 as the PS3 version
	//if ( FILES_MANAGER_ABC::Instance()->GetEnableEmulateCD() )
	//{
	//	return (_nReadPos + _nCurPos);
	//}
	//else
	//{
	//	unsigned long nBytesRead = 0;
	//	GetOverlappedResult(_Handle, (OVERLAPPED*)_pOverlapped, &nBytesRead, FALSE);
	//	return (((OVERLAPPED*)_pOverlapped)->Offset + nBytesRead);
	//}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateRead
//	Object:		
//	04-03-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_X360::UpdateRead()
{
	QDT_ASSERT(FILES_MANAGER_ABC::Instance()->GetEnableEmulateCD());
	
	Float32 fCurTime;
	int nReadSize; 

	if ( _nCurPos >= _nReadSize )
	{
		return false;
	}

	fCurTime = (Float32)(TIME::Instance()->GetSystemTime());

	nReadSize = int( ( fCurTime - _rLastReadTime ) * READ_SPEED );

	nReadSize += 16 - ( nReadSize % 16 );

	if ( _nCurPos + nReadSize > _nReadSize )
	{
		nReadSize = _nReadSize - _nCurPos;
	}

	_rLastReadTime = fCurTime;
	
	if ( ReadInternal(_pReadBuffer + _nCurPos, _nReadPos + _nCurPos, nReadSize, FS_WAIT) )
	{
		_nCurPos     += nReadSize;

		return true;
	}
	
	return false;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

