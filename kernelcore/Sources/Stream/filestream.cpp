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
// CLASS: FILE_STREAM
//
//
//00-12-11 FKE Created: 
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KCORE(Stream/filestream)

#include	INCL_KCORE(File/FilesManagerABC)
#include	INCL_KCORE(Core/Endian)

#define		QDT_INCLUDE_STRING
#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM constructor
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
FILE_STREAM::FILE_STREAM()
:
_nSize(0),
_nOffset(0),
_nBufferSize(DEFAULT_CACHE_SIZE),
_pcBuffer(NULL),
_pcAllocBuffer(NULL),
_nFlags(0),
_pcFilename(NULL),
_bValid(false),
_pFile(NULL)

{
	//	Do nothing...
}
	
//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM constructor
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
FILE_STREAM::FILE_STREAM(const char *	pFilename,
					     ACCESS_TYPE	AccessType,
						 const bool		bSharing)
:
_nSize(0),
_nOffset(0),
_nBufferSize(16384),
_pcBuffer(NULL),
_pcAllocBuffer(NULL),
_nFlags(0),
_pcFilename(NULL),
_bValid(false),
_pFile(NULL)
{
	QDT_FLAGS Flags;
	Flags.Set(FILES_MANAGER_ABC::FO_FILE_SHARE, bSharing );
	if ( Open(pFilename, AccessType, DEFAULT_CACHE_SIZE, CM_FILLCACHE, Flags) == false )
	{
		QDT_Error(QDT_COMMENT("FILE_STREAM::FILE_STREAM, Can't open file %s, AccessMode %d"), pFilename, AccessType);
	}
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM destructor
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
FILE_STREAM::~FILE_STREAM()
{
	Close();
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM Open
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILE_STREAM::Open(const Int8 *	pFilename,
						  Int32			nAccessType,
						  Int32			nCacheSize,
						  CACHE_MODE	CacheMode,
							const QDT_FLAGS&	rOptionFlags)
{
	FILES_MANAGER_ABC::FM_ERROR Error;

	//////////////////////////////////////////////////////////
	// [VMA] Tests optis chargement
	nCacheSize = DEFAULT_CACHE_SIZE;

	if ( nCacheSize % 2048 != 0 )
	{
		_nBufferSize = ( ( nCacheSize / 2048 ) + 1 ) * 2048;
	}
	else
	{
		_nBufferSize = nCacheSize;
	}

	//	Make the mask to optimize
	_nBufferSizeMask = _nBufferSize - 1;

	///////////////////////////////////////////////////////////

	_pcFilename = new char [strlen(pFilename) + 1];
	strcpy(_pcFilename, pFilename);

	// Allocate a 64 bytes aligned buffer
	_pcAllocBuffer = new char [_nBufferSize + 64];
	
	IntPtr Value = (IntPtr)(_pcAllocBuffer) % 64;

	_pcBuffer = (Char*)_pcAllocBuffer + 64 - Value ;
	_nSize = 0;

	switch (nAccessType & (~AT_TEXT))
	{
	default:
		QDT_FAIL();

		case	AT_READ_ONLY:
		_nFlags = READ;
		_pFile = FILES_MANAGER_ABC::Instance()->Open( pFilename, FILE_ABC::FM_READ, Error, rOptionFlags);
		_nOffset = 0;

		if	(_pFile == NULL)
		{
			SetValid(false);
			return	(false);
		}
		
		_nSize = _pFile->GetSize();
		_pFile->Seek( FILE_ABC::FS_BEGIN, 0 );

		if ( CacheMode == CM_FILLCACHE )
		{
			FillBuffer();
		}

		break;
			case	AT_CREATE:
		//	on est en écriture
		_nFlags = READ|WRITE;
		_pFile = FILES_MANAGER_ABC::Instance()->Open( pFilename, FILE_ABC::FM_CREATE_READ_WRITE, Error, rOptionFlags);
		_nOffset = 0;

		if	(_pFile == NULL)
		{
			SetValid(false);
			return	(false);
		}

		break;

	case	AT_MODIFY:
		_nFlags = READ|WRITE;
		_pFile = FILES_MANAGER_ABC::Instance()->Open( pFilename, FILE_ABC::FM_READ_WRITE, Error, rOptionFlags);
		_nOffset = 0;

		if	(_pFile == NULL)
		{
			SetValid(false);
			return	(false);
		}

		_nSize = _pFile->GetSize();
		_pFile->Seek( FILE_ABC::FS_BEGIN, 0 );

		if ( CacheMode == CM_FILLCACHE )
		{
			FillBuffer();
		}

		break;
	}


	// old code
	//case	AT_READ_ONLY:
	//	_nFlags = READ;
	//	_pFile = FILES_MANAGER_ABC::Instance()->Open( pFilename, FILE_ABC::FM_READ, Error, bSharing);
	//	_nOffset = 0;

	//	if	(_pFile == NULL)
	//	{
	//		SetValid(false);
	//		return	(false);
	//	}
	//	
	//	_nSize = _pFile->GetSize();
	//	_pFile->Seek( FILE_ABC::FS_BEGIN, 0 );

	//	if ( CacheMode == CM_FILLCACHE )
	//	{
	//		FillBuffer();
	//	}

	//	break;
	//case	AT_CREATE:
	//	//	on est en écriture
	//	_nFlags = READ|WRITE;
	//	_pFile = FILES_MANAGER_ABC::Instance()->Open( pFilename, FILE_ABC::FM_CREATE_READ_WRITE, Error, bSharing);
	//	_nOffset = 0;

	//	if	(_pFile == NULL)
	//	{
	//		SetValid(false);
	//		return	(false);
	//	}

	//	break;

	//case	AT_MODIFY:
	//	_nFlags = READ|WRITE;
	//	_pFile = FILES_MANAGER_ABC::Instance()->Open( pFilename, FILE_ABC::FM_READ_WRITE, Error, bSharing);
	//	_nOffset = 0;

	//	if	(_pFile == NULL)
	//	{
	//		SetValid(false);
	//		return	(false);
	//	}

	//	_nSize = _pFile->GetSize();
	//	_pFile->Seek( FILE_ABC::FS_BEGIN, 0 );

	//	if ( CacheMode == CM_FILLCACHE )
	//	{
	//		FillBuffer();
	//	}

	//	break;
	//}

	QDT_ASSERT( _nBufferSize % _pFile->GetSectorSize() == 0 );

	SetValid(true);
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM Close
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::Close()
{
	CloseFile();

	if(_pcAllocBuffer)
	{
		delete []_pcAllocBuffer;
		_pcAllocBuffer = NULL;
	}

	_pcBuffer = NULL;

	if(_pcFilename)
	{
		delete []_pcFilename;
		_pcFilename = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Flush
//	Object:		
//	02-05-17:	DAC - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::Flush()
{
	//write to HD
	FlushFile();

	//reinit
	FillBuffer();
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM FillBuffer
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::FillBuffer()
{
	UInt32 offset = _nOffset & (~_nBufferSizeMask);
	
	if (offset + _nBufferSize < _nSize)
	{
		_pFile->Read( _pcBuffer, offset, _nBufferSize, FILE_ABC::FS_WAIT );
	}
	else
	{
		UInt32 nModuloSize;
		_pFile->GetModuloPositionAndSize( offset, _nSize - offset, offset, nModuloSize );

		_pFile->Read( _pcBuffer, offset, nModuloSize, FILE_ABC::FS_WAIT );
	}	
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM FlushFile
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::FlushFile()
{
	if (_nSize == 0)
		return;

	UInt32 offset = _nOffset & (~_nBufferSizeMask);
	if (offset + _nBufferSize < _nSize)
	{
		_pFile->Write( _pcBuffer, offset, _nBufferSize, FILE_ABC::FS_WAIT );
	}
	else
	{
		_pFile->Write( _pcBuffer, offset, _nSize - offset, FILE_ABC::FS_WAIT );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Restart
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILE_STREAM::Restart()
{
	if (_nFlags & READ)
	{
		_pFile->Seek( FILE_ABC::FS_BEGIN, 0 );
		_nOffset = 0;
		return (true);
	}
	else
	{
		return (false);
	}
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM SetChars
//	Object:		on considère l'octet comme la base de tout, ce qui nous évite le problème d'alignement...
//				voir si ça rame, dans ce cas on fera du cas par cas...
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SetChars(const char * const	pC,
							  int			Size)
{
	const char *	pCurrentC = pC;

	QDT_ASSERT(_nFlags & WRITE);

	if	(Size >= 8)
	{
		UInt32	nMaxWritableSize;
		UInt32	nSizeToWrite, nCurrentSizeToWrite;

		nSizeToWrite = Size;

		while(nSizeToWrite > 0)
		{
			nMaxWritableSize = _nBufferSize - (_nOffset & _nBufferSizeMask);

			//	Find the max writable size to copy in one QDT_MEMCOPY in the ring buffer
			if	(nSizeToWrite > nMaxWritableSize)
			{
				nCurrentSizeToWrite = nMaxWritableSize;
			}
			else
			{
				nCurrentSizeToWrite = nSizeToWrite;
			}

			//	Copy a part of the buffer
			QDT_MEMCOPY(_pcBuffer + (_nOffset & _nBufferSizeMask), pCurrentC, nCurrentSizeToWrite);

			//	Move the file stream position
			_nOffset += nCurrentSizeToWrite;
			//	Move the buffer position
			pCurrentC += nCurrentSizeToWrite;

			//	Substract the size to copy
			nSizeToWrite -= nCurrentSizeToWrite;

			if (_nOffset > _nSize)
			{
				_nSize = _nOffset;
			}
			if ((_nOffset & _nBufferSizeMask) == 0)
			{
				if	(_pFile)
				{
					_pFile->Write( _pcBuffer, _nOffset - _nBufferSize, _nBufferSize, FILE_ABC::FS_WAIT );
				}
				if (_nOffset < _nSize)
				{
					FillBuffer();
				}
			}
		}
	}
	else
	{
		while(Size--)
		{
			*(_pcBuffer + (_nOffset & _nBufferSizeMask)) = *pCurrentC++;
			_nOffset++;
			if (_nOffset > _nSize)
			{
				_nSize = _nOffset;
			}
			if ((_nOffset & _nBufferSizeMask) == 0)
			{
				if	(_pFile)
				{
					_pFile->Write( _pcBuffer, _nOffset - _nBufferSize, _nBufferSize, FILE_ABC::FS_WAIT );
				}
				if (_nOffset < _nSize)
				{
					FillBuffer();
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetChars
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::GetChars(char *	pC,
							  int		Size)
{
	QDT_ASSERT(_nFlags & READ);

	if ((_nOffset & _nBufferSizeMask) == 0 && _nOffset != 0)
	{
		FillBuffer();
	}

	while(Size--)
	{
		if	(_nOffset > _nSize)
		{
			_nOffset = _nSize;
			return;
		}

		*pC++ = *(_pcBuffer + (_nOffset & _nBufferSizeMask));
		_nOffset++;

		if ( ((_nOffset & _nBufferSizeMask) == 0) && (_nOffset != _nSize) )
		{
			FillBuffer();
		}
	}

	if	(_nOffset > _nSize)
	{
		_nOffset = _nSize;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM IsEof
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILE_STREAM::IsEof() const
{
	return(_nOffset == _nSize);
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	02-05-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	FILE_STREAM::IsValid() const
{
	return	(_bValid);
}

//-----------------------------------------------------------------------------
//	Name:		SetValid
//	Object:		
//	02-05-21:	DAC - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SetValid(const bool	bFlag)
{
	_bValid = bFlag;
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		pour les booléens, vu qu'on ne connait pas la taille sur tous les compilos (normalisé?) on le transforme en char
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const bool	b)
{
	char n;
	n = (b == true);
	SetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Int8	n)
{
	SetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Int32	n)
{
	Int32 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(Int32));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Int64	n)
{
	Int64 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(Int64));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Int16	n)
{
	Int16 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(Int16));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const UInt8	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(unsigned char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const UInt32	n)
{
	UInt32 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(UInt32));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const UInt64	n)
{
	UInt64 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(UInt64));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const UInt16	n)
{
	UInt16 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(UInt16));
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Float32	n)
{
	Float32 fVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&fVal), sizeof(Float32));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Float64	n)
{
	Float64 fVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&fVal), sizeof(Float64));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const Int8 * const	pC)
{
	char			c;
	const char *	pCurrentC = pC;

	do
	{
		c = *pCurrentC++;
		SetChars(&c, sizeof(char));
	}
	while	(c);

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & FILE_STREAM::operator <<(IO_STREAM	&S)
{
	char tmpbuffer;
	for (int i=S.GetSize(); i>0; --i)
	{
		S>>tmpbuffer;
		operator<<(tmpbuffer);
	}
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamFast
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::GetStreamFast(IO_STREAM &	S,
								   int			nSize)
{
	QDT_ASSERT(nSize <= (Int32)S.GetSize());
	char tmpbuffer;
	for (int i = nSize; i > 0; --i)
	{
		S>>tmpbuffer;
		operator<<(tmpbuffer);
	}
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(Int32 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(Int32));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(Int64 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(Int64));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(Int16 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(Int16));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(Int8 &	n)
{
	GetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(UInt32 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(UInt32));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(UInt64 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(UInt64));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(UInt16 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(UInt16));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(unsigned char &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(unsigned char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(bool &	b)
{
	char n;
	GetChars(reinterpret_cast<char *> (&n), sizeof(char));
	b = (n == 1);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(Float32 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(Float32));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(Float64 &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(Float64));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(char *	pC)
{
	int i = 0;
	char c = 0;
	GetChars(&c, sizeof(char));
	while (c != 0)
	{
		*(pC + i) = c;
		i++;
		GetChars(&c, sizeof(char));
	}
	*(pC + i) = c;
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & FILE_STREAM::operator >>(IO_STREAM	&S)
{
	char tmpbuffer;
	for (int i=GetSize(); i>0; --i)
	{
		operator>>(tmpbuffer);
		S<<tmpbuffer;
	}
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		GetLine
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::GetLine(char * const	pBuffer,
							const int		nSizeBuffer,
							const char		cDelimiter)
{
	char *	pC = pBuffer;
	char	c;
	int		nSize = 0;

	for(; nSize < nSizeBuffer - 1 ; ++nSize)
	{
		if	(IsEof())
		{
			break;
		}
		GetChars(&c, 1);
		if (c == cDelimiter)
		{
			break;
		}
		if (c == 0)
		{
			break;
		}
		*pC++ = c;
	}
	*pC = '\0';
}


#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::DebugOutput(const char *	pTxt)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::DebugOutput(unsigned char	Char)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::DebugOutput(int	Int)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutputBinary
//	Object:		
//	02-02-14:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::DebugOutputBinary(int	Int)
{
	// Do nothing
}
#endif

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator +
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator+(const UInt32	s)
{
	QDT_ASSERT(_nFlags & READ);
	if (_nFlags & WRITE)
	{
		FlushFile();
	}
	_nOffset += s;
	QDT_ASSERT(_nOffset <= _nSize);
	FillBuffer();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator =
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator-(const UInt32	s)
{
	QDT_ASSERT(_nFlags & READ);	
	if (_nFlags & WRITE)
	{
		FlushFile();
	}
	_nOffset -= s;
	QDT_ASSERT(_nOffset >= 0);
	FillBuffer();
	return *this;	
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator =
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator=(const UInt32	s)
{
	QDT_ASSERT(_nFlags & READ);	
	if (_nFlags & WRITE)
	{
		FlushFile();
	}
	_nOffset = s;
	QDT_ASSERT(_nOffset <= _nSize);
	FillBuffer();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator ==
//	Object:		recherche d'une chaine de caractère...renvoi la position...
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
int	FILE_STREAM::operator==(const char *	pc)
{
	QDT_ASSERT(_nFlags & READ);
	int i = _nOffset, pos = _nOffset, p = 0, l;
	char c = 0;
	l = strlen(pc);
	for(;;)
	{
		if (_nOffset == _nSize)
		{
			operator  = (pos);
			return -1;
		}
		GetChars(&c, 1);
		if (*(pc + p) == c)
		{
			p++;
			if (p == l)
			{
				break;
			}
		}
		else
		{
			p = 0;
		}
		i++;

	}
	operator = (pos);
	return i;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetString
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::GetString(char *	p,
							   int		s)
{
	int i = s - 1;
	char c = 0;
	while (i--)
	{
		GetChars(&c, 1);
		if (c == 0xd)
		{
			GetChars(&c, 1);
			break;
		}
		if (c == 0)
		{
			break;
		}
		*p = c;
		p++;
	}
	*p = 0;
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	01-12-11:	VPI - Created
//-----------------------------------------------------------------------------
UInt32	FILE_STREAM::GetBuffer(void *	pBuffer,
										int		nSize)
{
	UInt32 nPreviousOffset;
	UInt32 nNewOffset;

	nPreviousOffset = _nOffset;
	nNewOffset		= _nOffset + nSize;

	if(nNewOffset > _nSize)
		nSize = _nSize - _nOffset;

	GetChars((char*)pBuffer, nSize);

	return	(_nOffset - nPreviousOffset);
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferFast
//	Object:		
//	03-12-08:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::GetBufferFast(void *	pBuffer,
								  int		nSize)
{
	GetChars((char*)pBuffer, nSize);
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM PutString
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::PutString(const char *	p)
{
	char c;
	while (*p != 0)
	{
		SetChars(p, 1);
		p++;
	}
	c = 0xd;
	SetChars(&c, 1);
	c = 0xa;
	SetChars(&c, 1);
}

//-----------------------------------------------------------------------------
//	Name:		PutBuffer
//	Object:		
//	01-12-11:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::PutBuffer(const void * pBuffer,
							   int			nSize)
{
	SetChars((const char* const)pBuffer, nSize);
}

//-----------------------------------------------------------------------------
//	Name:		PutBufferFast
//	Object:		
//	03-11-17:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::PutBufferFast(const void *	pBuffer,
								   int			nSize)
{
	SetChars((const char* const)pBuffer, nSize);
}

//-----------------------------------------------------------------------------
//	Name:		ZeroBufferFast
//	Object:		
//	03-11-17:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::ZeroBufferFast(int			nSize)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetSize
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
UInt32	FILE_STREAM::GetSize() const
{
	return _nSize;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetPos
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
UInt32	FILE_STREAM::GetPos() const
{
	return _nOffset;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM CloseFile
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::CloseFile()
{
	if (_pFile != NULL)
	{
		if (_nFlags & WRITE)
		{
			FlushFile();
		}
		FILES_MANAGER_ABC::Instance()->Close(_pFile);
		_pFile = NULL;
	}

	SetValid(false);
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM SwitchWriteToRead
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SwitchWriteToRead()
{
	QDT_ASSERT((_nFlags & WRITE));
	Close();
	Open(_pcFilename, AT_READ_ONLY);
	QDT_ASSERT(_pFile != NULL);
	_nFlags &= ~WRITE;
	_nFlags |= READ;
}

//-----------------------------------------------------------------------------
//	Name:		SwitchReadToWrite
//	Object:		
//	03-10-28:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SwitchReadToWrite()
{
	QDT_ASSERT((_nFlags & READ));
	Close();
	Open(_pcFilename, AT_MODIFY);
	QDT_ASSERT(_pFile != NULL);
	_nFlags &= ~READ;
	_nFlags |= WRITE;
}

//-----------------------------------------------------------------------------
//	Name:		AddPadding
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::AddPadding(int	nPadding)
{
	while( _nOffset % nPadding )
	{
		*this << '-';
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddRelocationLink
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::AddRelocationLink( UInt32 nPointerOffset, UInt32 nDataOffset )
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		ApplyRelocation
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::ApplyRelocation()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstRelocatedPointerOffset
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
int		FILE_STREAM::GetFirstRelocatedPointerOffset()
{
	QDT_NOT_IMPLEMENTED();

	return ( 0 );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbRelocatedLongJumps
//	Object:		
//	03-11-25:	VMA - Created
//-----------------------------------------------------------------------------
int	FILE_STREAM::GetNbRelocatedLongJumps()
{
	QDT_NOT_IMPLEMENTED();

	return (0);
}

//-----------------------------------------------------------------------------
//	Name:		WriteLongJumpRelocationTable
//	Object:		
//	03-11-25:	VMA - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::WriteLongJumpRelocationTable(IO_STREAM &	Stream)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetFileName
//	Object:		
//	04-07-26:	JJA - Created
//-----------------------------------------------------------------------------
const char *	FILE_STREAM::GetFileName() const
{
	return	( _pcFilename );
}

//-----------------------------------------------------------------------------
//	Name:		GetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
int	FILE_STREAM::GetRealocSize() const
{
	return(IO_STREAM::BASE_SIZE);
}

//-----------------------------------------------------------------------------
//	Name:		SetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SetRealocSize(const int	nRealocSize)
{
	//Do Nothing
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

