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
//	CLASS:	FILE_TEXT_STREAM
//
//	01-08-02:	VPI - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Stream/FileTextStream)

#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FILE_TEXT_STREAM constructor
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
FILE_TEXT_STREAM::FILE_TEXT_STREAM()
:
FILE_STREAM()
{
}


//-----------------------------------------------------------------------------
//	Name:		FILE_TEXT_STREAM constructor
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
FILE_TEXT_STREAM::FILE_TEXT_STREAM(const char *	pFilename,
								   int			nAccessType)
{
	Open(pFilename, nAccessType | AT_TEXT);
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILE_TEXT_STREAM::Open(const char *	pFilename,
							   int			nAccessType)
{
	return (FILE_STREAM::Open(pFilename, nAccessType | AT_TEXT));
}

//-----------------------------------------------------------------------------
//	Name:		Restart
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILE_TEXT_STREAM::Restart()
{
	return (FILE_STREAM::Restart());
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
int	FILE_TEXT_STREAM::operator==(const char	*pc)
{
	return (FILE_STREAM::operator==(pc));
}


//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		pour les booléens, vu qu'on ne connait pas la taille sur tous les compilos (normalisé?) on le transforme en char
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const bool	b)
{
	if	(b)
	{
		return	(this->operator<<("true"));
	}
	else
	{
		return	(this->operator<<("false"));
	}
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const char	n)
{
	SetChars(&n, sizeof(char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const int	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%d", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const long	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%ld", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const short	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%d", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const unsigned char	n)
{
	SetChars(reinterpret_cast <const char *> (&n), sizeof(unsigned char));
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const unsigned int	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%u", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const unsigned long	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%lu", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const unsigned short	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%u", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const float	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%f", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const double	n)
{
	char	sBuffer[256];
	sprintf(sBuffer, "%f", n);
	return	(this->operator<<(sBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		operator<<
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator<<(const char * const	pC)
{
	char			c;
	const char *	pCurrentC = pC;
	char			c2 = '\r';

	for(;;)
	{
		c = *pCurrentC++;
		if	(c == 0)
		{
			break;
		}
		if	(c == '\n')
		{
			SetChars(&c2, sizeof(char));
		}
		if	(c != '\r')
		{
			SetChars(&c, sizeof(char));
		}
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & FILE_TEXT_STREAM::operator <<(IO_STREAM	&S)
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
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(int &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(long &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(short &	n)
{
	QDT_FAIL();
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(char &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(unsigned int &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(unsigned long &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(unsigned short &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(unsigned char &	n)
{
	QDT_FAIL();
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(bool &	b)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(float &	n)
{
	QDT_FAIL();
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(double &	n)
{
	QDT_FAIL();
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator>>
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM &	FILE_TEXT_STREAM::operator>>(char *	pC)
{
	char	c;

	do
	{
		if	(IsEof())
		{
			break;
		}
		GetChars(&c, 1);
		*pC++ = c;
	}
	while	(c);
	*pC = '\0';
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & FILE_TEXT_STREAM::operator >>(IO_STREAM	&S)
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
//	Name:		GetString
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::GetString(char *	pC,
									int		s)
{
	int		i = s - 1;
	char	c;
	while (i--)
	{
		if	(IsEof())
		{
			break;
		}
		GetChars(&c, 1);
		if (c == 0xd)	// if \r , skip \n and break
		{
			GetChars(&c, 1);
			break;
		}
		if (c == 0xa) // if \n alone then break
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
//	Name:		PutString
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::PutString(const char *	p)
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
//	Name:		GetLine
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
/*
void	FILE_TEXT_STREAM::GetLine(char * const	pBuffer,
								  const int		nSizeBuffer,
								  const char	cDelimiter)
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
*/
//-----------------------------------------------------------------------------
//	Name:		FILE_TEXT_STREAM IsEof
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
bool	FILE_TEXT_STREAM::IsEof() const
{
	return(FILE_STREAM::IsEof());
}

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::DebugOutput(const char *	pTxt)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::DebugOutput(unsigned char	Char)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::DebugOutput(int	Int)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutputBinary
//	Object:		
//	02-02-14:	ELE - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::DebugOutputBinary(int	Int)
{
	// Do nothing
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
int	FILE_TEXT_STREAM::GetRealocSize() const
{
	return(IO_STREAM::BASE_SIZE);
}

//-----------------------------------------------------------------------------
//	Name:		SetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
void	FILE_TEXT_STREAM::SetRealocSize(const int	nRealocSize)
{
	//Do Nothing
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
