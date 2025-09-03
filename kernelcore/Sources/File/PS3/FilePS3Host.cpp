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
//	CLASS:	FILE_PS3_HOST
//
//	05-11-15:	VMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(File/PS3/FilePS3Host)

#include <cell/fs/cell_fs_file_api.h>
#include <sys/paths.h>
#include <string.h>

char FILE_PS3_HOST::_homePath[256] = SYS_APP_HOME;

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

static CellFsErrno snErrno = CELL_FS_SUCCEEDED;

void PrintFSError(const Char * pCode, CellFsErrno err)
{
	snErrno = err;

	QDT_Message("In %s", pCode);

	switch (err)
	{
		default:
			QDT_Error("Error not documented");
			break;          

		case CELL_FS_EFAULT:
			QDT_Error("NULL pointer is specified in the buf");
			break;        

		case CELL_FS_ENOENT:
			QDT_Message("File system corresponding to path is not mounted");
			QDT_Message("or The file specified by the path does not exist");
			QDT_Error("or The specified file does not exist and the CELL_FS_O_CREAT is not specified in the flags");
			break;        

		case CELL_FS_ENOSPC:
			QDT_Error("When the CELL_FS_O_CREAT is specified in the flags and the file does not exist, there is no area to create a new file");
			break;        

		case CELL_FS_ERANGE:
			QDT_Error("Stream reached to the end point");
			break;        

		case CELL_FS_EROFS:
			QDT_Error("File system is read only");
			break;         

		case CELL_FS_EBADF:
			QDT_Error("fs is invalid or not opened for read");
			break;         

		case CELL_FS_EBUSY:
			QDT_Message("File system is being mounted with lsp");
			QDT_Message("or it exceeds the maximum number of file system that system can handle");
			QDT_Error(" or it has already been mounted");
			break;         

		case CELL_FS_EEXIST:
			QDT_Error("When the specified file exists, the CELL_FS_O_CREATE and the CELL_FS_O_EXCL are specified");
			break;        

		case CELL_FS_EINVAL:
			QDT_Message("Argument is invalid or the mount point has already been used");
			QDT_Error("or the options are invalid or the mount_type is invalid");
			break;        

		case CELL_FS_EIO:
			QDT_Error("I/O error has occured");
			break;           

		case CELL_FS_EISDIR:
			QDT_Error("The specified path is a directory");
			break;        

		case CELL_FS_EMFILE:
			QDT_Error("The size of file descriptor exceeds the maximum size to be opened");
			break;        

		case CELL_FS_ENOMEM:
			QDT_Error("Memory is insufficient or a file specified by the path does not exist");
			break;        

		case CELL_FS_ENOTDIR:
			QDT_Message("The specified path is not a directory");
			QDT_Error("or components in the path contains something other than a directory");
			break;       

		case CELL_FS_ENXIO:
			QDT_Error("A file is not locked by the cellFsLsnLock()");
			break;         

		case CELL_FS_EPERM:
			QDT_Error("The fd is opened by the CELL_FS_OWRONLY");
			break;         

		case CELL_FS_EXDEV:
			QDT_Error("The mount point by the from is different from the one by the to");
			break;         

		case CELL_FS_ENAMETOOLONG:
			QDT_Error("Components in the path exceeds the maximum length");
			break;  

		case CELL_FS_ENOSYS:
			QDT_Error("Connection to file system server process failed");
			break;        

		case CELL_FS_ENOTEMPTY:
			QDT_Error("Directory is not empty");
			break;     

		case CELL_FS_ENOTSUP:
			QDT_Error("This API is not supported or the specified file system is not supported");
			break;       

		case CELL_FS_EFSSPECIFIC:
			QDT_Error("File system specific error has occured");
			break;   
	}
}

#ifdef _DEBUG
#define FILE_PS3_HOST_CHECK_ERROR(e) snErrno = e; if (snErrno != CELL_FS_SUCCEEDED) PrintFSError(#e, snErrno);
#else
	#define FILE_PS3_HOST_CHECK_ERROR(e) snErrno = e
#endif

//-----------------------------------------------------------------------------
//	Name:		FILE_PS3_HOST constructor
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
FILE_PS3_HOST::FILE_PS3_HOST()
{
}

//-----------------------------------------------------------------------------
//	Name:		FILE_PS3_HOST destructor
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
FILE_PS3_HOST::~FILE_PS3_HOST()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetSectorSize
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
int	FILE_PS3_HOST::GetSectorSize() const
{
	return SECTOR_SIZE;
}

//-----------------------------------------------------------------------------
//	Name:		IsIdle
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::IsIdle() const
{
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Open(const char * const	pFileName,
						   const F_MODE			eMode,
						   const bool			bSharing)
{
	UInt32			nFlags;
	UInt64			nSize;
	char			FullPathName[256];

	strcpy(FullPathName, _homePath);

	switch (eMode)
	{
		case FM_READ : 
			nFlags = CELL_FS_O_RDONLY;
			break;
		case FM_WRITE : 
			nFlags = CELL_FS_O_WRONLY | CELL_FS_O_CREAT;
			break;
		case FM_READ_WRITE : 
			nFlags = CELL_FS_O_RDWR;
			break;
		case FM_CREATE_READ_WRITE : 
			nFlags = CELL_FS_O_RDWR | CELL_FS_O_CREAT;
			break;
	}

	strcat(FullPathName, "/");
	strcat(FullPathName, pFileName);
			
	FILE_PS3_HOST_CHECK_ERROR(cellFsOpen( FullPathName, nFlags, &_nHandle, NULL, 0 ));

	if ( snErrno != CELL_FS_SUCCEEDED )
	{
		QDT_Warning(QDT_COMMENT("[FILE_PS3_HOST::Open] Cannot open file %s"), FullPathName );
		return (false);
	}

	FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, 0, CELL_FS_SEEK_END, &nSize));
	SetSize( nSize );
	FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, 0, CELL_FS_SEEK_SET, &nSize));

	return (true);

}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Close()
{
	FILE_PS3_HOST_CHECK_ERROR(cellFsClose(_nHandle));
}

//-----------------------------------------------------------------------------
//	Name:		IsExist
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::IsExist(const char * const	pFileName) const
{
	char			FullPathName[256];
	Int32			nHandle;
	CellFsErrno		nErrno;

	strcpy(FullPathName, _homePath);
	strcat(FullPathName, "/");
	strcat(FullPathName, pFileName);
	
	nErrno = cellFsOpen( FullPathName, CELL_FS_O_RDONLY, &nHandle, NULL, 0 );

	cellFsClose(nHandle);

	return ( nErrno == CELL_FS_SUCCEEDED );
}

//-----------------------------------------------------------------------------
//	Name:		IsWriteProtected
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::IsWriteProtected(const char * const	pFileName) const
{
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Seek
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Seek(const F_SEEK			eInitialPosition,
						    const unsigned int		nOffset)
{
	UInt64 nPos;

	switch (eInitialPosition)
	{
		case FS_BEGIN:
			FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, nOffset, CELL_FS_SEEK_SET, &nPos ));
			break;

		case FS_CURRENT:
			FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, nOffset, CELL_FS_SEEK_CUR, &nPos));
			break;

		case FS_END:
			FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, nOffset, CELL_FS_SEEK_END, &nPos));
			break;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Read(void *				pBuffer,
						   const unsigned int	nPosition,
						   const int			nSize,
						   const F_SYNC			eSync)
{
	UInt64	nPos, nRead;

	_Mutex.Lock();

	FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, nPosition, CELL_FS_SEEK_SET, &nPos));

	FILE_PS3_HOST_CHECK_ERROR(cellFsRead( _nHandle, pBuffer, nSize, &nRead ));

	_Mutex.Unlock();

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Write(void *				pBuffer,
							const unsigned int	nPosition,
							const int			nSize,
							const F_SYNC		eSync)
{
	UInt64	nPos, nWritten;

	_Mutex.Lock();

	FILE_PS3_HOST_CHECK_ERROR(cellFsLseek(_nHandle, nPosition, CELL_FS_SEEK_SET, &nPos));

	FILE_PS3_HOST_CHECK_ERROR(cellFsWrite( _nHandle, pBuffer, nSize, &nWritten ));

	_Mutex.Unlock();

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Delete(const char * const	pFileName) const
{
	char			FullPathName[256];

	strcpy(FullPathName, _homePath);
	strcat(FullPathName, pFileName);

	FILE_PS3_HOST_CHECK_ERROR(cellFsUnlink(FullPathName));
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_PS3_HOST::Loop()
{
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentReadPosition
//	Object:		Obsolete
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILE_PS3_HOST::GetCurrentReadPosition() const
{
	return (0);
}

//-----------------------------------------------------------------------------
//	Name:		Break
//	Object:		Obsolete
//	05-11-15:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST::Break()
{
	QDT_NOT_IMPLEMENTED();
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		SetHomePath
//	Object:		
//	06-05-09:	VHA - Created
//-----------------------------------------------------------------------------
void	FILE_PS3_HOST::SetHomePath(const char *	newHome)
{
	QDT_ASSERT(strlen(newHome) < 256);

	strcpy(_homePath, newHome);
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
