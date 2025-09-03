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
//	CLASS:	FILE_XBOX
//
//	03-05-05:	JPE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FILE_PC constructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_XBOX::FILE_XBOX()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		FILE_PC destructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_XBOX::~FILE_XBOX()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_XBOX::Open(	const char * const	pFileName,
							const F_MODE		eMode,
							const bool			bSharing)
{
	QDT_ASSERT( TestStatus(F_STATUS::FS_OPEN) == false );
	DWORD dwSharingMode;

	dwSharingMode = FILE_SHARE_READ;

	if ( TestStatus(F_STATUS::FS_OPEN) == false )
	{
		EnableStatus(F_STATUS::FS_OPEN);
		DWORD dwDesiredAccess = 0;
		
		switch (eMode)
		{
		case FM_READ : 
			dwDesiredAccess = GENERIC_READ;
			break;
		case FM_WRITE : 
			dwDesiredAccess = GENERIC_WRITE;
			break;
		case FM_READ_WRITE : 
			dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
			break;
		}
		
		if (bSharing)
		{
			dwSharingMode = FILE_SHARE_READ;
		}

		QDT_STRING		XBoxFileName("D:\\");
		XBoxFileName += pFileName;

		_Handle = CreateFile(XBoxFileName, dwDesiredAccess, dwSharingMode, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED|FILE_FLAG_NO_BUFFERING, NULL);

		if (_Handle != INVALID_HANDLE_VALUE)
		{
			SetSize(GetFileSize(_Handle, NULL));
			SetPosition(0);
			return ( true );
		}
		else
		{
			return ( false );
		}
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_XBOX::Close()
{
	QDT_ASSERT( TestStatus(F_STATUS::FS_OPEN) == true );

	if ( TestStatus(F_STATUS::FS_OPEN) == true )
	{
		int nErr;

		nErr = CloseHandle(_Handle);
		if ( nErr == 0 )
		{
			return ( false );
		}
		else
		{
			DisableStatus(F_STATUS::FS_OPEN);
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
bool	FILE_XBOX::IsExist(const char * const	pFileName) const
{
	QDT_STRING		XBoxFileName("D:\\");
	WIN32_FIND_DATA FindFile;

	XBoxFileName += pFileName;

	HANDLE			handle = FindFirstFile(XBoxFileName, &FindFile);
	
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
bool	FILE_XBOX::IsWriteProtected(const char * const	pFileName) const
{
	QDT_STRING		XBoxFileName("D:\\");
	WIN32_FIND_DATA FindFile;

	XBoxFileName += pFileName;

	HANDLE handle = FindFirstFile(XBoxFileName, &FindFile);
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
bool	FILE_XBOX::Seek(const F_SEEK	eInitialPosition,
						 const int		nOffset)
{
	// Gestion de l'asynchrone à faire
	
	QDT_ASSERT( ( nOffset % SECTOR_SIZE ) == 0 );

	int nSize = GetSize();
	int nCurrentPosition = GetCurrentPosition();
	
	switch (eInitialPosition)
	{
	case FS_BEGIN:
		
		QDT_ASSERT( nOffset >= 0 );
		//QDT_ASSERT( nOffset < nSize );
		
		SetPosition(nOffset);

		break;
	case FS_CURRENT:

		QDT_ASSERT( ( nCurrentPosition + nOffset ) < nSize );
		QDT_ASSERT( ( nCurrentPosition + nOffset ) >= 0 );

		SetPosition( nCurrentPosition + nOffset );
		
		break;
	case FS_END:
		
		QDT_ASSERT( nOffset <= 0 );
		QDT_ASSERT( nOffset >= ( -nSize ) );

		SetPosition( nSize + nOffset );
		
		break;
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_XBOX::Write(	void *		pBuffer,
							const int		nPosition,
							const int		nSize,
							const F_SYNC	eSync)

{
	// Gestion de l'asynchrone à faire
	BOOL bResult = FALSE;
	unsigned long nBytesWritten = 0;
	DWORD nLastError = 0;

	_Overlapped.hEvent = NULL;
	_Overlapped.Offset = nPosition;
	_Overlapped.OffsetHigh = 0;
    _Overlapped.Internal = 1;

	if ( Seek(FS_BEGIN, nPosition) == true )	
	{
		//SetFilePointer(_Handle, nPosition, NULL, FILE_BEGIN);
		bResult = WriteFile(_Handle, pBuffer, nSize, &nBytesWritten, &_Overlapped);

		if ( bResult == FALSE )
		{
			nLastError = GetLastError();

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
				QDT_Error("Invalid File Access: Probably writing read-only file (error code = %d)", nLastError );
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
//	Name:		Read
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_XBOX::Read(void *		pBuffer,
						  const int		nPosition,
						  const int		nSize,
						  const F_SYNC	eSync)

{
	// Gestion de l'asynchrone à faire
	BOOL bResult = FALSE;
	unsigned long nBytesRead = 0;
	DWORD nLastError = 0;

	QDT_ASSERT( nSize % SECTOR_SIZE == 0 );

	_Overlapped.hEvent = NULL;
	_Overlapped.Offset = nPosition;
	_Overlapped.OffsetHigh = 0;
    _Overlapped.Internal = 1;

	if ( Seek(FS_BEGIN, nPosition) == true )	
	{
		//SetFilePointer(_Handle, nPosition, NULL, FILE_BEGIN);
		bResult = ReadFile(_Handle, pBuffer, nSize, &nBytesRead, &_Overlapped);

		if ( bResult == FALSE )
		{
			nLastError = GetLastError();

			if ( nLastError == ERROR_HANDLE_EOF )
			{
				// Reached EoF
				bResult = TRUE;
			}
			else if ( nLastError == ERROR_IO_PENDING )
			{
				if ( eSync == FS_WAIT )
				{
					while( !IsIdle() );
					bResult = TRUE;
				}
			}
			else
			{
				QDT_Error("Invalid File Access: Probably reading write-only file (error code = %d)", nLastError );
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
bool	FILE_XBOX::Delete(const char * const	pFileName) const
{
	BOOL bError;

	bError = DeleteFile(pFileName);

	if ( bError == 0 )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_XBOX::Loop()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		GetSectorSize
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
int		FILE_XBOX::GetSectorSize() const
{
	return SECTOR_SIZE;
}

//-----------------------------------------------------------------------------
//	Name:		IsIdle
//	Object:		
//	02-02-27:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_XBOX::IsIdle() const
{
	return HasOverlappedIoCompleted(&_Overlapped);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
