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
// CLASS: MEM_STREAM
//
//
//00-12-11 FKE Created: 
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (MEM_STREAM)
//
//	The MEM_STREAM class implements ...
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Stream/MemStream)
#include	INCL_KCORE(Core/Endian)

#define		QDT_INCLUDE_STRING
#define		QDT_INCLUDE_STDLIB
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#ifndef _MASTER
	MEM_STREAM *	MEM_STREAM::_RawStreams[MAX_MEMORY_TYPE][MAX_LOD_TYPE];
#endif

// NHO : Commente pour verifier son utilite....
//MEM_POOL_HEADER * MEM_STREAM::_pMemPool = NULL;

//-----------------------------------------------------------------------------
//	Name:		MEM_STREAM constructor
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
MEM_STREAM::MEM_STREAM(char *pBuf,
					   int Size)
:
_nSize(Size),
_nBufferSize(Size),
_nOffset(0),
_pcBuffer(pBuf),
_nFlags(READ|EXTERNAL_BUFFER),
_nRealocSize(BASE_SIZE)
{
	if	(_pcBuffer == NULL)
	{
		_pcBuffer = AllocateBuffer(Size);
	}
}

//-----------------------------------------------------------------------------
//	Name:		MEM_STREAM constructor
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
MEM_STREAM::MEM_STREAM()
:
_nSize(0),
_nBufferSize(0),
_nOffset(0),
_pcBuffer(NULL),
_nFlags(WRITE),
_nRealocSize(BASE_SIZE)
{
	_pcBuffer = AllocateBuffer(_nRealocSize);
	QDT_ASSERT(_pcBuffer != NULL);
	_nBufferSize = _nRealocSize;
}

//-----------------------------------------------------------------------------
//	Name:		MEM_STREAM constructor
//	Object:		
//	02-11-08:	DAC - Created
//-----------------------------------------------------------------------------
MEM_STREAM::MEM_STREAM(int	nBaseSize)
:
_nSize(0),
_nBufferSize(0),
_nOffset(0),
_pcBuffer(NULL),
_nFlags(WRITE),
_nRealocSize(BASE_SIZE)
{
	if (nBaseSize != 0)
	{
		_pcBuffer = AllocateBuffer(nBaseSize);
		QDT_ASSERT(_pcBuffer != NULL);
		_nBufferSize = nBaseSize;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FreeBuffer
//	Object:		Optimize MEM_STREAM with a size of 32	
//	04-12-15:	RMA - Created
//-----------------------------------------------------------------------------
char * MEM_STREAM::AllocateBuffer(int nSize)
{
	return (new char[nSize]);
}

//-----------------------------------------------------------------------------
//	Name:		FreeBuffer
//	Object:		
//	04-12-15:	RMA - Created
//-----------------------------------------------------------------------------
void MEM_STREAM::FreeBuffer(char * pBuffer, int nSize)
{
	if(pBuffer)
	{
		delete [] pBuffer;
	}
}

//-----------------------------------------------------------------------------
//	Name:		~MEM_STREAM destructor
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
MEM_STREAM::~MEM_STREAM()
{
	if (!(_nFlags & EXTERNAL_BUFFER))
	{
		FreeBuffer(_pcBuffer, _nBufferSize);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Reserve
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::Reserve(UInt32	nSize)
{
	if (_nBufferSize < nSize)
	{
		if	(_pcBuffer == NULL)
		{
			_pcBuffer = AllocateBuffer(nSize);
			_nBufferSize = nSize;
			Clear();
		}
		else
		{
			FreeBuffer(_pcBuffer, _nBufferSize);
			_pcBuffer = AllocateBuffer(nSize);
			_nBufferSize = nSize;
			Clear();
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetRealocSize
//	Object:		
//	04-03-05:	VPI - Created
//-----------------------------------------------------------------------------
int	MEM_STREAM::GetRealocSize() const
{
	return(_nRealocSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetRealocSize
//	Object:		
//	04-03-05:	VPI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SetRealocSize(const int	nRealocSize)
{
	_nRealocSize = nRealocSize;
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::Clear()
{
	_nFlags = WRITE,
	_nSize = 0;
	_nOffset = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Restart
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
bool	MEM_STREAM::Restart()
{
	if (_nFlags & READ)
	{
		_nOffset = 0;
		return (true);
	}
	else
	{
		return (false);
	}
}


//-----------------------------------------------------------------------------
//	Name:		SetChars
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SetChars(const char *	pC,
							 int			Size)
{
	QDT_ASSERT(_nFlags & WRITE);

#ifdef PLATFORM_PC
	PutBufferFast(pC, Size);
#else
	while	(Size--)
	{
		if	(_nSize == _nBufferSize)
		{
			//	On réalloue le buffer...
			char *	p = AllocateBuffer(_nBufferSize + _nRealocSize);
			QDT_MEMCOPY(p, _pcBuffer, _nBufferSize);
			FreeBuffer(_pcBuffer, _nBufferSize);
			_pcBuffer = p;
			_nBufferSize += _nRealocSize;
		}
		
		*(_pcBuffer + _nOffset) = *pC++;
		_nOffset++;
		if (_nOffset > _nSize)
		{
			_nSize = _nOffset;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetChars
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::GetChars(char *	pC,
							 int	Size)
{
#ifdef PLATFORM_PC
	GetBufferFast(pC, Size);
#else
	QDT_ASSERT(_nFlags & READ);

	while	(Size > 0)
	{
		QDT_ASSERT(_nOffset < _nSize); // we try to read after stream size (because _nSize is the first empty char of the stream
		*pC++ = *(_pcBuffer + _nOffset);
		_nOffset++;

		if	(_nOffset > _nSize)
		{
			//we are at the end of the stream, so break copy
			_nOffset = _nSize;
			return;
		}
		--Size;
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetFromFile
//	Object:		
//	04-01-19:	VPI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::GetFromFile(FILE_ABC *			pFile,
								UInt32				nSize,
								FILE_ABC::F_SYNC	Sync,
								UInt32				nPos)
{
//	QDT_ASSERT (nSize <= _nBufferSize);
	QDT_ASSERT( nPos % pFile->GetSectorSize() == 0 );

	SwitchWriteToRead();

	UInt32	nModuloSize;
	UInt32	nModuloPos;
	pFile->GetModuloPositionAndSize(nPos, nSize, nModuloPos, nModuloSize );

	//realloc buffer for multiple of 512 for File Read
	if (_nBufferSize < nModuloSize)
	{
		if (_pcBuffer != NULL)
		{
			FreeBuffer(_pcBuffer, _nBufferSize);
			_pcBuffer = NULL;
		}
		_pcBuffer = AllocateBuffer(nModuloSize);
		QDT_ASSERT(_pcBuffer != NULL);
		_nBufferSize = nModuloSize;
	}
	//read fast from File
	pFile->Read(GetBuffer(), nModuloPos, nModuloSize, Sync);
	_nSize = nSize;
}
//-----------------------------------------------------------------------------
//	Name:		PutToFile
//	Object:		
//	04-03-01:	VPI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::PutToFile(FILE_ABC *		pFile,
							  FILE_ABC::F_SYNC	Sync,
							  UInt32		nPos)
{
	QDT_ASSERT( nPos % pFile->GetSectorSize() == 0 );

	SwitchWriteToRead();
	pFile->Write(GetBuffer(), nPos, GetSize(), Sync);
}

//-----------------------------------------------------------------------------
//	Name:		InstanciateStream
//	Object:		
//	xx-xx-xx:	CVI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::InstanciateStream( const MEM_STREAM& Stream )
{
	if (!(_nFlags & EXTERNAL_BUFFER))
	{
		FreeBuffer(_pcBuffer, _nBufferSize);
	}

	_nSize = Stream.GetSize();
	_nBufferSize = Stream.GetBufferSize();
	_nOffset = 0;

	_nFlags = READ|EXTERNAL_BUFFER;
	_pcBuffer = Stream._pcBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const bool	b)
{
	char n;
	n = (b == true);
	SetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Int8	n)
{
	SetChars(&n, sizeof(Int8));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Int32	n)
{
	Int32 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(Int32));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Int64	n)
{
	Int64 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(Int64));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Int16	n)
{
	Int16 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(Int16));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const UInt8	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(UInt8));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const UInt32	n)
{
	UInt32 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(UInt32));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const UInt64	n)
{
	UInt64 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(UInt64));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const UInt16	n)
{
	UInt16 nVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&nVal), sizeof(UInt16));
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Float32	n)
{
	Float32 fVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&fVal), sizeof(Float32));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Float64	n)
{
	Float64 fVal = ENDIAN::Instance()->Convert(n);
	SetChars(reinterpret_cast <const char *> (&fVal), sizeof(Float64));
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator<<(const Int8 * const	pC)
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
//	03-12-08:	VMA - now uses fast copy
//-----------------------------------------------------------------------------
IO_STREAM & MEM_STREAM::operator << (IO_STREAM	&S)
{
	QDT_ASSERT(_nFlags & WRITE);

	int nSize = S.GetSize() - S.GetPos();
	
	if	(_nSize + nSize > _nBufferSize)
	{
		//	On réalloue le buffer...
		int nNewSize = _nRealocSize * ( ( ( _nSize + nSize ) + ( _nRealocSize - 1 ) ) / _nRealocSize );
		char *	p = AllocateBuffer(nNewSize);
		QDT_MEMCOPY(p, _pcBuffer, _nSize);
		FreeBuffer(_pcBuffer, _nBufferSize);
		_pcBuffer = p;
		_nBufferSize = nNewSize;
	}
		
	S.GetBufferFast(_pcBuffer + _nOffset, nSize);
	
	_nSize += nSize;
	_nOffset += nSize;
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		GetStreamFast
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::GetStreamFast(IO_STREAM &	S,
								  int			nSize)
{
	QDT_ASSERT(_nFlags & WRITE);

	int nStreamSize = S.GetSize() - S.GetPos();
	QDT_ASSERT (nSize <= nStreamSize);
	
	if	(_nSize + nSize > _nBufferSize)
	{
		//	On réalloue le buffer...
		int nNewSize = _nRealocSize * ( ( ( _nSize + nSize ) + ( _nRealocSize - 1 ) ) / _nRealocSize );
		char *	p = AllocateBuffer(nNewSize);
		QDT_MEMCOPY(p, _pcBuffer, _nSize);
		FreeBuffer(_pcBuffer, _nBufferSize);
		_pcBuffer = p;
		_nBufferSize = nNewSize;
	}
		
	S.GetBufferFast(_pcBuffer + _nOffset, nSize);
	
	_nSize += nSize;
	_nOffset += nSize;
}


//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Int32 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(Int32));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Int64 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(Int64));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Int16 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(Int16));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Int8 &	n)
{
	GetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(UInt32 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(UInt32));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(UInt64 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(UInt64));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(UInt16 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(UInt16));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(UInt8 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(UInt8));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Bool &	b)
{
	char n;
	GetChars(&n, sizeof(char));
	b = (n == 1);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Float32 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(Float32));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(Float64 &	n)
{
	GetChars(reinterpret_cast <char *> (&n), sizeof(Float64));
	n = ENDIAN::Instance()->Convert(n);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator>>(char *	pC)
{
	char	c;

	do
	{
		GetChars(&c, sizeof(char));
		*pC++ = c;
	}
	while	(c);

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & MEM_STREAM::operator >> (IO_STREAM	&S)
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
//	Name:		operator+
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator+(const UInt32	s)
{
	QDT_ASSERT(_nFlags & READ);	
	_nOffset += s;
	QDT_ASSERT(_nOffset <= _nSize);
	return *this;	
}

//-----------------------------------------------------------------------------
//	Name:		operator-
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator-(const UInt32	s)
{
	QDT_ASSERT(_nFlags & READ);	
	_nOffset -= s;
	QDT_ASSERT(_nOffset >= 0);
	return *this;	
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
IO_STREAM &	MEM_STREAM::operator=(const UInt32	s)
{
//	QDT_ASSERT(_nFlags & READ);	
	_nOffset = s;
	QDT_ASSERT(_nOffset <= _nSize);
	return *this;		
}


//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
int	MEM_STREAM::operator==(const char *	pc)
{
	QDT_ASSERT(_nFlags & READ);
	UInt32 i = _nOffset, pos = _nOffset, p = 0, l;
	char c = 0;
	l = strlen(pc);
	for(;;)
	{
		if (_nOffset == _nSize)
		{
			operator = (pos);
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
//	Name:		GetString
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::GetString(char *	p,
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
//	Name:		PutString
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::PutString(const char *	p)
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
//	Name:		GetSize
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
UInt32	MEM_STREAM::GetSize() const
{
	return _nSize;
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferSize
//	Object:		
//	02-05-14:	DAC - Created
//-----------------------------------------------------------------------------
UInt32	MEM_STREAM::GetBufferSize() const
{
	return	(_nBufferSize);
}


//-----------------------------------------------------------------------------
//	Name:		GetPos
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
UInt32	MEM_STREAM::GetPos() const
{
	return _nOffset;
}


//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void *	MEM_STREAM::GetBuffer()
{
//	QDT_ASSERT(_nFlags & READ);
	return (static_cast <char *> (_pcBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	05-04-14:	GGO - Created
//-----------------------------------------------------------------------------
const void *	MEM_STREAM::GetBuffer() const
{
	return (static_cast <char *> (_pcBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentReadPtr
//	Object:		
//	05-01-07:	DAC - Created
//-----------------------------------------------------------------------------
void *	MEM_STREAM::GetCurrentReadPtr()
{
	return	(_pcBuffer + _nOffset);
}


//-----------------------------------------------------------------------------
//	Name:		SwitchWriteToRead
//	Object:		
//	xx-xx-xx:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SwitchWriteToRead()
{
//	QDT_ASSERT(_nFlags & WRITE);
	_nFlags &= ~WRITE;
	_nFlags |= READ; 
}

//-----------------------------------------------------------------------------
//	Name:		SwitchReadToWrite
//	Object:		
//	02-11-08:	DAC - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SwitchReadToWrite()
{
//	QDT_ASSERT(_nFlags & READ);
	_nFlags &= ~READ;
	_nFlags |= WRITE;
}


//-----------------------------------------------------------------------------
//	Name:		MEM_STREAM IsEof
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
bool	MEM_STREAM::IsEof() const
{
	return(_nOffset == _nSize);
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	02-05-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	MEM_STREAM::IsValid() const
{
	return	((_pcBuffer != NULL) && (_nBufferSize > 0));
}


//-----------------------------------------------------------------------------
//	Name:		SetStatus
//	Object:		
//	02-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SetStatus(int	nStatus)
{
	_nFlags = nStatus;
}

//-----------------------------------------------------------------------------
//	Name:		GetStatus
//	Object:		
//	04-03-04:	VPI - Created
//-----------------------------------------------------------------------------
int	MEM_STREAM::GetStatus() const
{
	return (_nFlags);
}

//-----------------------------------------------------------------------------
//	Name:		IsWrite
//	Object:		
//	02-05-28:	CVI - Created
//-----------------------------------------------------------------------------
bool	MEM_STREAM::IsWrite()
{
	return ( (_nFlags&WRITE) == WRITE );
}

//-----------------------------------------------------------------------------
//	Name:		IsExternalBuffer
//	Object:		
//	04-01-08:	VPI - Created
//-----------------------------------------------------------------------------
bool	MEM_STREAM::IsExternalBuffer() const
{
	return ( (_nFlags&EXTERNAL_BUFFER) == EXTERNAL_BUFFER );
}

//-----------------------------------------------------------------------------
//	Name:		SetReadStatus
//	Object:		
//	02-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SetReadStatus(bool	b)
{
	if ( !b )
	{
		SetWriteStatus();
		return;
	}
	_nFlags &= ~WRITE;
	_nFlags |= READ;
}

//-----------------------------------------------------------------------------
//	Name:		SetWriteStatus
//	Object:		
//	02-05-28:	CVI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::SetWriteStatus(bool	b)
{
	if ( !b )
	{
		SetReadStatus();
		return;
	}
	_nFlags &= ~READ;
	_nFlags |= WRITE;
	
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	02-10-17:	CVI - Created
//-----------------------------------------------------------------------------
UInt32	MEM_STREAM::GetBuffer(void *	pBuffer,
						  int		nSize)
{
	int	nPreviousOffset = _nOffset;
	QDT_ASSERT( nSize >= 0 );
	if (nSize != 0)
	{
		GetBufferFast(pBuffer, nSize);
	}
	return	(_nOffset - nPreviousOffset);
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferFast
//	Object:		
//	03-12-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::GetBufferFast(void *	pBuffer,
								  int		nSize)
{
	if (nSize != 0)
	{
		if ( _nOffset + nSize > _nSize )
		{
			nSize = _nSize - _nOffset;
		}

		QDT_MEMCOPY( pBuffer, _pcBuffer + _nOffset, nSize);
		_nOffset += nSize;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetLine
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::GetLine(char * const	pBuffer,
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

//-----------------------------------------------------------------------------
//	Name:		PutBuffer
//	Object:		
//	02-10-17:	CVI - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::PutBuffer(const void *	pBuffer,
							  int			nSize)
{
	char		*p;

	QDT_ASSERT( nSize >= 0 );
	if (nSize != 0)
	{
		p = (char*)	pBuffer;

		while( nSize-- )
		{
			SetChars( p, 1 );
			p++;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		PutBufferFast
//	Object:		
//	03-06-30:	AMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::PutBufferFast(const void *	pBuffer,
								  int			nSize)
{
	QDT_ASSERT(_nFlags & WRITE);

	if (nSize != 0)
	{
		//Because we can override datas
		if	(_nOffset + nSize > _nBufferSize)
		{
			//	On réalloue le buffer...
			// because of the test above it can be a pb... but the comportement will change if I use _nOffset instead of _nSize....
			// maybe later.
			UInt32 nNewSize = _nRealocSize * ( ( ( _nSize + nSize ) + ( _nRealocSize - 1 ) ) / _nRealocSize );
			char *	p = AllocateBuffer(nNewSize);
			QDT_MEMCOPY(p, _pcBuffer, _nSize);
			FreeBuffer(_pcBuffer, _nBufferSize);
			_pcBuffer = p;
			_nBufferSize = nNewSize;
		}
		
		QDT_MEMCOPY(_pcBuffer + _nOffset, pBuffer, nSize);
		_nOffset += nSize;
		if (_nOffset > _nSize)
		{
			_nSize = _nOffset;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		ZeroBufferFast
//	Object:		
//	03-11-17:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::ZeroBufferFast(int		nSize)
{
	QDT_ASSERT(_nFlags & WRITE);
	
	if	(_nSize + nSize > _nBufferSize)
	{
		//	On réalloue le buffer...
		int nNewSize = _nRealocSize * ( ( ( _nSize + nSize ) + ( _nRealocSize - 1 ) ) / _nRealocSize );
		char *	p = AllocateBuffer(nNewSize);
		QDT_MEMCOPY(p, _pcBuffer, _nSize);
		FreeBuffer(_pcBuffer, _nBufferSize);
		_pcBuffer = p;
		_nBufferSize = nNewSize;
	}

	memset(_pcBuffer + _nOffset, 0, nSize);
	_nSize += nSize;
	_nOffset += nSize;
}

//-----------------------------------------------------------------------------
//	Name:		AddPadding
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::AddPadding(int	nPadding)
{
	while( _nOffset % nPadding )
	{
		*this << '-';
	}
}

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		AddRelocationLink
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::AddRelocationLink( UInt32 nPointerOffset, UInt32 nDataOffset )
{
	QDT_ENSURE( nPointerOffset >= 0 && nPointerOffset <= _nSize );
	QDT_ENSURE( nPointerOffset >= 0 && nDataOffset <= _nSize );
	QDT_ENSURE( nPointerOffset % 4 == 0 );
	QDT_ENSURE( nDataOffset % 4 == 0 );

	RelocationData	RData = { nPointerOffset, nDataOffset };
	
	RelocationDataList.PushTail( RData);
}

//-----------------------------------------------------------------------------
//	Name:		AddLongRelocationLink
//	Object:		
//	04-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::AddLongRelocationLink(UInt32	nPointerOffset,
										  UInt32	nDataOffset)
{
	QDT_ENSURE( nPointerOffset % 4 == 0 );
	QDT_ENSURE( nDataOffset % 4 == 0 );

	RelocationData	RData = { nPointerOffset, nDataOffset };
	
	LongJumpRelocationDataList.PushTail( RData);
}
//-----------------------------------------------------------------------------
//	Name:		CompareRelocationData
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
#if ( (defined(_MSC_VER)) && (_MSC_VER >= 1300) )
int __cdecl CompareRelocationData(const void *elem1, const void *elem2 )
#else
int CompareRelocationData(const void *elem1, const void *elem2 )
#endif
{
	MEM_STREAM::RelocationData* RData1 = (MEM_STREAM::RelocationData*) elem1;
	MEM_STREAM::RelocationData* RData2 = (MEM_STREAM::RelocationData*) elem2;

	if ( RData1->nPointerOffset >= RData2->nPointerOffset )
		return 1;
	else
		return -1;
}

//-----------------------------------------------------------------------------
//	Name:		ApplyRelocation
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::ApplyRelocation()
{
	int nSize = RelocationDataList.GetSize(), nIdx, nLastPointerOffset = 0, nDataOffset, nPointerOffset;

	if (nSize == 0)
	{
		return;
	}

	qsort( &RelocationDataList[0], nSize, sizeof(RelocationData), CompareRelocationData );

	for ( nIdx = 0 ; nIdx < nSize ; nIdx++ )
	{
		nPointerOffset = RelocationDataList[nIdx].nPointerOffset;
		nDataOffset	   = RelocationDataList[nIdx].nDataOffset;

		QDT_ENSURE( (nPointerOffset - nLastPointerOffset) > 0 );

		SwitchWriteToRead();
		*this = nPointerOffset;
		SwitchReadToWrite();

		if (	(nDataOffset - nPointerOffset) < 0 
			 || (nDataOffset - nPointerOffset) >= 262144 )
		{
			if ( nIdx !=  nSize - 1 )
			{
				RelocationDataList[nIdx].nPointerOffset = RelocationDataList[nIdx+1].nPointerOffset - RelocationDataList[nIdx].nPointerOffset;
			}
			else
			{
				RelocationDataList[nIdx].nPointerOffset = 0;
			}

			RelocationDataList[nIdx].nDataOffset = nDataOffset - nPointerOffset;

			LongJumpRelocationDataList.PushTail( RelocationDataList[nIdx] );
			RelocationDataList[nIdx].nDataOffset = -1;

			*this << (short)( LongJumpRelocationDataList.GetSize() - 1 );
			*this << (short)( -1 );
		}
		else
		{		
			*this << (short)( ( nDataOffset - nPointerOffset ) >> 2 );
			*this << (short)( 0 );
		}

		if ( nLastPointerOffset && RelocationDataList[nIdx-1].nDataOffset != -1 )
		{
			if ( (nPointerOffset - nLastPointerOffset) < 262144 )
			{
				SwitchWriteToRead();
				*this = ( nLastPointerOffset + 2 );
				SwitchReadToWrite();

				*this << (short)( ( nPointerOffset - nLastPointerOffset ) >> 2 );
			}
			else
			{
				SwitchWriteToRead();
				*this = ( nLastPointerOffset );
				SwitchReadToWrite();

				RelocationDataList[nIdx-1].nDataOffset -= RelocationDataList[nIdx-1].nPointerOffset;
				RelocationDataList[nIdx-1].nPointerOffset = RelocationDataList[nIdx].nPointerOffset - RelocationDataList[nIdx-1].nPointerOffset;

				LongJumpRelocationDataList.PushTail( RelocationDataList[nIdx-1] );
				RelocationDataList[nIdx-1].nDataOffset = -1;

				*this << (short)( LongJumpRelocationDataList.GetSize() - 1 );
				*this << (short)( -1 );
			}
		}
		
		nLastPointerOffset = nPointerOffset;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstRelocatedPointerOffset
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
int		MEM_STREAM::GetFirstRelocatedPointerOffset()
{
	if ( RelocationDataList.GetSize() > 0 )
	{
		return ( RelocationDataList[0].nPointerOffset );
	}
	else
	{
		return 0;
	}
	
}

//-----------------------------------------------------------------------------
//	Name:		GetNbRelocatedLongJumps
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
int		MEM_STREAM::GetNbRelocatedLongJumps()
{
	return ( LongJumpRelocationDataList.GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		WriteLongJumpRelocationTable
//	Object:		
//	03-11-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::WriteLongJumpRelocationTable( IO_STREAM& Stream )
{
	for ( UInt32 i = 0 ; i < LongJumpRelocationDataList.GetSize() ; i++ )
	{
		Stream << LongJumpRelocationDataList[i].nPointerOffset;
		Stream << LongJumpRelocationDataList[i].nDataOffset;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetDataBankRawStream
//	Object:		
//	04-11-22:	VMA - Created
//-----------------------------------------------------------------------------
MEM_STREAM *	MEM_STREAM::GetRawStream(MEMORY_TYPE	eMemoryType,
												 LOD_TYPE		eLODType)
{
	return (_RawStreams[eMemoryType][eLODType]);
}

//-----------------------------------------------------------------------------
//	Name:		CleanDataBankRawStreams
//	Object:		
//	04-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::CleanRawStreams()
{
	for( int nMemType = 0 ; nMemType < MAX_MEMORY_TYPE; nMemType++)
	{
		for( int nLodType = 0 ; nLodType < MAX_LOD_TYPE; nLodType++)
		{
			if (_RawStreams[nMemType][nLodType])
			{
				delete _RawStreams[nMemType][nLodType];
				_RawStreams[nMemType][nLodType] = NULL;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddRawPointer
//	Object:		
//	04-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	MEM_STREAM::AddRawPointer(int						nPointerOffset,
								 void *						pRawData,
								 int						nSizeRawData,
								 MEMORY_TYPE				eMemoryType,
								 LOD_TYPE					eLODType,
								 int nPadding)
{
	if ( _RawStreams[eMemoryType][eLODType] == NULL )
	{
		_RawStreams[eMemoryType][eLODType] = new MEM_STREAM;
	}

	MEM_STREAM& Stream = *_RawStreams[eMemoryType][eLODType];

	Stream.AddPadding(nPadding);
	Stream.AddLongRelocationLink( nPointerOffset, Stream.GetPos() );
	Stream.PutBufferFast(pRawData, nSizeRawData);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		CompressTo
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
UInt32 	MEM_STREAM::CompressTo(MEM_STREAM &	Dest) const
{
	QDT_FAIL();
	return (0);
}

//-----------------------------------------------------------------------------
//	Name:		UnCompressTo
//	Object:		
//	04-07-30:	VPI - Created
//-----------------------------------------------------------------------------
UInt32	MEM_STREAM::UnCompressTo(MEM_STREAM &	Dest,
										 UInt32	DestUncompressSize) const
{
	QDT_FAIL();
	return (0);
}

//-----------------------------------------------------------------------------
//	Name:		IsMemStream
//	Object:		
//	05-01-06:	DAC - Created
//-----------------------------------------------------------------------------
bool	MEM_STREAM::IsMemStream() const 
{ 
	return	(true); 
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
