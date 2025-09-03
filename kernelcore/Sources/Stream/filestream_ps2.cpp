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

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__ ;
#endif	//	QDT_INCLUDE_CI

//*****************************************************************************
//
// CLASS DESCRIPTION (FILE_STREAM)
//
//	The FILE_STREAM class implements ...
//
//*****************************************************************************

#include	"Include.h"

//=============================================================================
// CODE STARTS HERE
//=============================================================================
//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM constructor
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
FILE_STREAM::FILE_STREAM()
:
_nSize(0),
_pcBuffer(NULL),
_pcFilename(NULL),
_nFlags(0),
_nOffset(0),
_nBufferSize(16384),
_nHandle(-1),
_bValid(false)
{
	//	Do nothing...
}
	
//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM constructor
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
FILE_STREAM::FILE_STREAM(const char *	pFilename,
					     ACCESS_TYPE	AccessType)
:
_nSize(0),
_pcBuffer(NULL),
_pcFilename(NULL),
_nFlags(0),
_nOffset(0),
_nBufferSize(16384),
_nHandle(-1),
_bValid(false)
{
	Open(pFilename, AccessType);
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM destructor
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
FILE_STREAM::~FILE_STREAM()
{
	
	CloseFile();
	delete []_pcBuffer;
	delete []_pcFilename;
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM Open
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
bool	FILE_STREAM::Open(const char *	pFilename,
						  int			nAccessType)
{
//	<01-06-29:	ONA]	Verify that the buffer is a power of 2
	QDT_ASSERT(OS::GetNbrBits(_nBufferSize) == 1);
	//	Make the mask to optimize
	_nBufferSizeMask = _nBufferSize - 1;
//	[01-06-29:	ONA>	
	_pcFilename = new char [strlen(pFilename) + 1];
	strcpy(_pcFilename, pFilename);
	_pcBuffer = new char [_nBufferSize];
	_nSize = 0;

	char strPS2FileName[256];

	#if defined PS2_KIT
		FILE_PS2_HOST::ConvertFileName(_pcFilename, strPS2FileName);
	#elif defined PS2_CDVD
		FILE_PS2_CDVD::ConvertFileName(_pcFilename, strPS2FileName);
	#elif defined PS2_ATMON
		FILE_PS2_ATMON::ConvertFileName(_pcFilename, strPS2FileName);
	#endif

	switch (nAccessType & (~AT_TEXT))
	{
	default:
		QDT_FAIL();

	case	AT_READ_ONLY:
		_nFlags = READ;
		_nHandle = sceOpen(strPS2FileName, SCE_RDONLY);
		
		_nOffset = 0;
		if	(_nHandle < 0)
		{
			SetValid(false);
			return	(false);
		}

		_nSize = sceLseek(_nHandle, 0, SCE_SEEK_END);
		sceLseek(_nHandle, 0, SCE_SEEK_SET);

		FillBuffer();
		break;

	case	AT_CREATE:
		//	on est en écriture
		_nFlags = WRITE;

		_nHandle = sceOpen(strPS2FileName, SCE_WRONLY | SCE_CREAT);

		_nOffset = 0;
		if	(_nHandle < 0)
		{
			SetValid(false);
			return	(false);
		}
		break;

	case	AT_MODIFY:
		//	pour la modification, en fait on se place à la fin, et on ajoute des enregistrements...
		_nFlags = READ|WRITE;
		
		_nHandle = sceOpen(strPS2FileName, SCE_RDWR);

		if	(_nHandle < 0)
		{
			SetValid(false);
			return	(false);
		}

		_nSize = sceLseek(_nHandle, 0, SCE_SEEK_END);
		sceLseek(_nHandle, 0, SCE_SEEK_SET);

		FillBuffer();
		break;
	}

	SetValid(true);
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM Close
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::Close()
{
	CloseFile();
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
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::FillBuffer()
{
	int offset = _nOffset & (~_nBufferSizeMask);
	sceLseek(_nHandle, offset, SCE_SEEK_SET);
	if (offset + _nBufferSize < _nSize)
	{
		sceRead(_nHandle, _pcBuffer, _nBufferSize);
		sceLseek(_nHandle, -_nBufferSize, SCE_SEEK_CUR);
	}
	else
	{
		sceRead(_nHandle, _pcBuffer, _nSize - offset);
		sceLseek(_nHandle, -(_nSize - offset), SCE_SEEK_CUR);

	}
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM FlushFile
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::FlushFile()
{
	if (_nSize == 0)
		return;

	int offset = _nOffset & (~_nBufferSizeMask);
	if (offset + _nBufferSize < _nSize)
	{
		sceWrite(_nHandle, _pcBuffer, _nBufferSize);
	}
	else
	{
		sceWrite(_nHandle, _pcBuffer, _nSize - offset);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Restart
//	Object:		
//	01-08-10:	SBE - Created
//-----------------------------------------------------------------------------
bool	FILE_STREAM::Restart()
{
	if (_nFlags & READ)
	{
		sceLseek(_nHandle, 0, SCE_SEEK_SET);
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
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SetChars(const char * const	pC,
							  int					Size)
{
/*	const char *	pCurrentC = pC;

	QDT_ASSERT(_nFlags & WRITE);

	if	(Size >= 8)
	{
		int				nMaxWritableSize;
		int				nSizeToWrite, nCurrentSizeToWrite;

		nSizeToWrite = Size;

		while(nSizeToWrite > 0)
		{
			nMaxWritableSize = _nBufferSize - (_nOffset & _nBufferSizeMask);

			//	Find the max writable size to copy in one memcpy in the ring buffer
			if	(nSizeToWrite > nMaxWritableSize)
			{
				nCurrentSizeToWrite = nMaxWritableSize;
			}
			else
			{
				nCurrentSizeToWrite = nSizeToWrite;
			}

			//	Copy a part of the buffer
			memcpy(_pcBuffer + (_nOffset & _nBufferSizeMask), pCurrentC, nCurrentSizeToWrite);

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
					fwrite(_pcBuffer, _nBufferSize, 1, _pFile);
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
					fwrite(_pcBuffer, _nBufferSize, 1, _pFile);
				}
				if (_nOffset < _nSize)
				{
					FillBuffer();
				}
			}
		}
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetChars
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::GetChars(char *	pC,
							  int		Size)
{
	QDT_ASSERT(_nFlags & READ);
	while(Size--)
	{
		QDT_ASSERT(_nOffset <= _nSize);
		if ((_nOffset & _nBufferSizeMask) == 0 && _nOffset != 0)
		{
			FillBuffer();
		}
		*pC++ = *(_pcBuffer + (_nOffset & _nBufferSizeMask));
		_nOffset++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM IsEof
//	Object:		
//	01-08-02:	SBE - Created
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
//	01-08-02:	SBE - Created
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
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const char	n)
{
	SetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const int	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(int));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const long	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(long));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const short	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(short));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const unsigned char	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(unsigned char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const unsigned int	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(unsigned int));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const unsigned long	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(unsigned long));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const unsigned short	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(unsigned short));
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const float	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(float));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const double	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(double));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator<<
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator<<(const char * const	pC)
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
//	01-08-10:	SBE - Created
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
	QDT_ASSERT(nSize <= S.GetSize());
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
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(int &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(int));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(long &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(long));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(short &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(short));
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(char &	n)
{
	GetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(unsigned int &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(unsigned int));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(unsigned long &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(unsigned long));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(unsigned short &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(unsigned short));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(unsigned char &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(unsigned char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(bool &	b)
{
	int n = 0;
	GetChars(reinterpret_cast<char *> (&n), sizeof(char));
	b = (n == 1);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(float &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(float));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator>>(double &	n)
{
	GetChars(reinterpret_cast<char *> (&n), sizeof(double));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator>>
//	Object:		
//	01-08-02:	SBE - Created
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
//	01-08-10:	SBE - Created
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

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM operator +
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator+(const unsigned int	s)
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
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator-(const unsigned int	s)
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
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_STREAM::operator=(const unsigned int	s)
{
//	QDT_ASSERT(_nFlags & READ);	
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
//	01-08-02:	SBE - Created
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
//	01-08-02:	SBE - Created
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
//	01-12-11:	SBE - Created
//-----------------------------------------------------------------------------
int	FILE_STREAM::GetBuffer(void *	pBuffer,
							int		nSize)
{
	int nPreviousOffset;

	nPreviousOffset = _nOffset;

	GetChars((char*)pBuffer, nSize);

	return	(_nOffset - nPreviousOffset);
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM PutString
//	Object:		
//	01-08-02:	SBE - Created
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
//	01-12-11:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::PutBuffer(const void * const	pBuffer,
							  int					nSize)
{
	SetChars((const char* const)pBuffer, nSize);
}


//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetSize
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
int	FILE_STREAM::GetSize() const
{
	return _nSize;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM GetPos
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
int	FILE_STREAM::GetPos() const
{
	return _nOffset;
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM CloseFile
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::CloseFile()
{
	if (_nHandle >= 0)
	{
		if (_nFlags & WRITE)
		{
			FlushFile();
		}
		
		sceClose(_nHandle);
		_nHandle = -1;
	}

	SetValid(false);
}

//-----------------------------------------------------------------------------
//	Name:		FILE_STREAM SwitchWriteToRead
//	Object:		
//	01-08-02:	SBE - Created
//-----------------------------------------------------------------------------
void	FILE_STREAM::SwitchWriteToRead()
{
	QDT_ASSERT((_nFlags & WRITE));
	Close();
	Open(_pcFilename, AT_READ_ONLY);
	QDT_ASSERT(_nHandle >= 0);
	_nFlags &= ~WRITE;
	_nFlags |= READ;
}

//-----------------------------------------------------------------------------
//	Name:		GetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
int	FILESTREAM_PS_2::GetRealocSize() const
{
	return(IO_STREAM::BASE_SIZE);
}

//-----------------------------------------------------------------------------
//	Name:		SetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
void	FILESTREAM_PS_2::SetRealocSize(const int	nRealocSize)
{
	//Do Nothing
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
