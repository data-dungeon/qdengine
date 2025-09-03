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
//	CLASS:	FILE_PS3_HOST_STD
//
//	02-02-14:	AMA - Created
//*****************************************************************************
#include	"Root.h"
#include	INCL_KCORE(File/PS3/FilePS3HostStd)
#include	INCL_KCORE(Time/Time_ABC)

#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)

#define READ_SPEED		1.f * 1024.f*1024.f / 1000.f			// 1 Mo/s
//#define READ_SPEED		100.f * 1024.f*1024.f / 1000.f			// 100 Mo/s

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FILE_PS3_HOST_STD constructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_PS3_HOST_STD::FILE_PS3_HOST_STD()
{
	_nReadSize = 0;
	_nReadPos = 0;
	_nCurPos = 0;
	_pReadBuffer = NULL;
	_rLastReadTime = 0.f;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_PS3_HOST_STD destructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_PS3_HOST_STD::~FILE_PS3_HOST_STD()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Open(const char * const	pFileName,
						  const F_MODE			eMode,
						  const bool			bSharing)
{
	QDT_ASSERT( TestStatus(FS_OPEN) == false );

	QDT_STRING sFlags;

	_sName = QDT_STRING(HOME_DIR) + pFileName;

	if ( TestStatus(FS_OPEN) == false )
	{
		EnableStatus(FS_OPEN);
		
		switch (eMode)
		{
		case FM_READ :
			sFlags = "rb";
			break;
		case FM_WRITE : 
			sFlags = "wb";
			break;
		case FM_READ_WRITE : 
			sFlags = "r+b";
			break;
		case FM_CREATE_READ_WRITE : 
			sFlags = "w+b";
			break;
		}

		_pFile = fopen(_sName.GetBuffer(), sFlags);

		if (_pFile != NULL)
		{
			fseek((FILE*)_pFile, 0, SEEK_END);
			int nSize = ftell((FILE*)_pFile);
			fseek((FILE*)_pFile, 0, SEEK_SET);

			SetSize(nSize);
			SetPosition(0);
			return ( true );
		}
		else
		{
			/*QDT_STRING sError, sMessage;
			OS::GetWinLastErrorMsg(sError);
			sMessage.Format("Can't open File %s : %s", pFileName, sError.GetBuffer() );
			QDT_Message(sMessage);*/
			return ( false );
		}
	}
	else
	{
		QDT_STRING sError;
		sError.Format("File % allready open:", pFileName );
		QDT_Message(sError);
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Close()
{
	QDT_ASSERT( TestStatus(FS_OPEN) );

	if ( TestStatus(FS_OPEN) )
	{
		int nErr;

		nErr = fclose((FILE*)_pFile);
		_pFile = NULL;
		if ( nErr == 0 )
		{
			DisableStatus(FS_OPEN);
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
//	Name:		IsExist
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::IsExist(const char * const	pFileName) const
{
	QDT_STRING sName = QDT_STRING(HOME_DIR) + pFileName;

	void * pF = fopen(sName.GetBuffer(), "rb");
	
	fclose((FILE*)pF);

	return (pF != NULL);
}

//-----------------------------------------------------------------------------
//	Name:		IsWriteProtected
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::IsWriteProtected(const char * const	pFileName) const
{
	return(false);
}

//-----------------------------------------------------------------------------
//	Name:		Seek
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Seek(const F_SEEK	eInitialPosition,
						 const unsigned int		nOffset)
{
	//QDT_ASSERT( ( nOffset % SECTOR_SIZE ) == 0 );

	UInt32 nSize = GetSize();
	unsigned int nCurrentPosition = GetCurrentPosition();
	
	switch (eInitialPosition)
	{
	case FS_BEGIN:	
		QDT_ASSERT( nOffset >= 0 );
		SetPosition(nOffset);

		break;
	case FS_CURRENT:
		QDT_ASSERT(( nCurrentPosition + nOffset ) < nSize );
		QDT_ASSERT(( nCurrentPosition + nOffset ) >= 0 );
		SetPosition( nCurrentPosition + nOffset );
		
		break;
	case FS_END:	
		QDT_ASSERT(nOffset == 0);
		SetPosition(nSize + nOffset);
		
		break;
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	04-04-23:	CVI - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Read(void *				pBuffer,
						 const unsigned int	nPosition,
						 const int			nSize,
						 const F_SYNC		eSync)
{
	QDT_ASSERT( nSize % SECTOR_SIZE == 0 );

//	if ( eSync == FS_WAIT )
	{
		int nError;
		nError = fseek((FILE*)_pFile, nPosition, SEEK_SET);
		QDT_ASSERT(nError == 0);
		int nRead = fread(pBuffer, sizeof(char), nSize, (FILE*)_pFile);
		if	(nRead == 0)
		{
			ferror((FILE*)_pFile);
		}
		return ( nRead > 0 );
	}
	/*else
	{
		_nReadSize   = nSize;
		_nReadPos    = nPosition;
		_pReadBuffer = (char*) pBuffer;
		_nCurPos     = 0;

		_rLastReadTime = TIME::Instance()->GetSystemTime();
	}*/

	return true;
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::ReadInternal(void *				pBuffer,
								  const unsigned int	nPosition,
								  const int				nSize,
								  const F_SYNC			eSync)

{
	int nRead = 0;

	if ( Seek(FS_BEGIN, nPosition) == true )	
	{
		int nError;
		nError = fseek((FILE*)_pFile, nPosition, SEEK_SET);
		QDT_ASSERT(nError == 0);
		//QDT_Message("ReadInternal %s : Pos = %d, Size = %d", _sName.GetBuffer(), nPosition, nSize);
		nRead = fread(pBuffer, sizeof(char), nSize, (FILE*)_pFile);
		if	(nRead == 0)
		{
			ferror((FILE*)_pFile);
		}
	}

	return ( nRead > 0 );
}

//-----------------------------------------------------------------------------
//	Name:		Break
//	Object:		
//	04-06-30:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Break()
{
	_nReadSize = _nCurPos;
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Write(void *		pBuffer,
						  const unsigned int		nPosition,
						  const int		nSize,
						  const F_SYNC	eSync)

{
	fseek((FILE*)_pFile, nPosition, SEEK_SET);
	int nWrite = fwrite(pBuffer, nSize, 1, (FILE*)_pFile);
	return ( nWrite > 0 );
}
 
//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	02-02-20:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Delete(const char * const	pFileName) const
{
	QDT_NOT_IMPLEMENTED();
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		Rename
//	Object:		
//	03-12-05:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::Rename(const char * const	pFileName) const
{
	QDT_NOT_IMPLEMENTED();
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_PS3_HOST_STD::Loop()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		IsIdle
//	Object:		
//	02-02-27:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::IsIdle() const
{
	((FILE_PS3_HOST_STD*)this)->UpdateRead();
	return ( _nCurPos >= _nReadSize );
}

//-----------------------------------------------------------------------------
//	Name:		GetSectorSize
//	Object:		
//	03-05-22:	VMA - Created
//-----------------------------------------------------------------------------
int		FILE_PS3_HOST_STD::GetSectorSize() const
{
	return (SECTOR_SIZE);
}

//-----------------------------------------------------------------------------
//	Name:		SetName
//	Object:		
//	03-09-10:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_PS3_HOST_STD::SetName(const QDT_STRING &	sName)
{
	_sName = sName;
}

//-----------------------------------------------------------------------------
//	Name:		GetName
//	Object:		
//	03-09-10:	VPI - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	FILE_PS3_HOST_STD::GetName() const
{
	return	(_sName);
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentReadPosition
//	Object:		
//	04-03-23:	VMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILE_PS3_HOST_STD::GetCurrentReadPosition() const
{
	return (_nReadPos + _nCurPos);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateRead
//	Object:		
//	04-03-12:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_PS3_HOST_STD::UpdateRead()
{
	float fCurTime;
	int nReadSize; 

	if ( _nCurPos >= _nReadSize )
	{
		return false;
	}

	fCurTime = TIME::Instance()->GetSystemTime();

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

