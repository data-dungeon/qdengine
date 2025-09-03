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
// CLASS: DEBUG_STREAM
//
//
//00-12-11 FKE Created: 
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Stream/DebugStream)

#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
// CODE STARTS HERE
//=============================================================================
DEBUG_STREAM::DEBUG_STREAM(bool AccessType)
:
_nSize(0),
_nFlags(0)
{
	if (AccessType)
	{
//on est en lecture
		_nFlags = READ;
	}
	else
	{
//on est en écriture
		_nFlags = WRITE;
	}
}


DEBUG_STREAM::~DEBUG_STREAM()
{
	char c[128];
	sprintf(c, "<Size of stream : %i>\r\n", _nSize);
	QDT_OutputDebugString(c);
}


IO_STREAM& 
DEBUG_STREAM::operator << (const bool b)
{
	if (b)
	{
		QDT_OutputDebugString("<W bool> true... ");
	}
	else
	{
		QDT_OutputDebugString("<W bool> false... ");
	}
	_nSize += sizeof(char);
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		Restart
//	Object:		
//	04-03-15:	JPE - Created
//-----------------------------------------------------------------------------
bool	DEBUG_STREAM::Restart()
{
	QDT_NOT_IMPLEMENTED();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetString
//	Object:		
//	04-03-15:	JPE - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::GetString(char	*cChar,
								int		nInt)
{
	QDT_NOT_IMPLEMENTED();
}


IO_STREAM& 
DEBUG_STREAM::operator << (const char n)
{
	char c[128];
	sprintf(c, "<W char> %i... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator << (const int n)
{
	char c[128];
	sprintf(c, "<W int> %i... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM&
DEBUG_STREAM::operator << (const long n)
{
	char c[128];
	sprintf(c, "<W long> %i... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator << (const short n)
{
	char c[128];
	sprintf(c, "<W short> %i... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator << (const unsigned char n)
{
	char c[128];
	sprintf(c, "<W unsigned char> %u... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);

	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator << (const unsigned int n)
{
	char c[128];
	sprintf(c, "<W unsigned int> %u... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM&
DEBUG_STREAM::operator << (const unsigned long n)
{
	char c[128];
	sprintf(c, "<W unsigned long> %u... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator << (const unsigned short n)
{
	char c[128];
	sprintf(c, "W unsigned short> %u... ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}


IO_STREAM& 
DEBUG_STREAM::operator << (const float n)
{
	char c[128];
	sprintf(c, "<W float> %f ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator << (const double n)
{
	char c[128];
	sprintf(c, "<W double> %f ", n);
	QDT_OutputDebugString(c);
	_nSize += sizeof(n);
	return *this;
}

//chaine de caractère
IO_STREAM& 
DEBUG_STREAM::operator << (const char * const pC)
{
	QDT_OutputDebugString("<W string> ");
	QDT_OutputDebugString(pC);
	_nSize += strlen(pC);
	return *this;
}


//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & DEBUG_STREAM::operator << (IO_STREAM	&S)
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
void	DEBUG_STREAM::GetStreamFast(IO_STREAM &	S,
									int			nSize)
{
	QDT_ASSERT((UInt32)nSize <= S.GetSize());
	char tmpbuffer;
	for (int i=nSize; i>0; --i)
	{
		S>>tmpbuffer;
		operator<<(tmpbuffer);
	}
}


IO_STREAM& 
DEBUG_STREAM::operator >> (int &n)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (long &n)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (short &n)
{
	return *this;
}


IO_STREAM& 
DEBUG_STREAM::operator >> (char &n)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (unsigned int &n)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (unsigned long &n)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (unsigned short &n)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (unsigned char &n)
{
	return *this;
}


IO_STREAM& 
DEBUG_STREAM::operator >> (bool &b)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (float &f)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (double &d)
{
	return *this;
}

IO_STREAM& 
DEBUG_STREAM::operator >> (char *pC)
{
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-08-10:	VPI - Created
//-----------------------------------------------------------------------------
IO_STREAM & DEBUG_STREAM::operator >>(IO_STREAM	&S)
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
void	DEBUG_STREAM::GetLine(char * const	pBuffer,
							  const int		nSizeBuffer,
							  const char	cDelimiter)
{
	// do nothing
}

IO_STREAM&
DEBUG_STREAM::operator + (const unsigned int)
{
	return *this;
}

IO_STREAM&
DEBUG_STREAM::operator - (const unsigned int)
{
	return *this;	
}

IO_STREAM&
DEBUG_STREAM::operator = (const unsigned int)
{
	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		IsMemStream
//	Object:		
//	05-01-06:	DAC - Created
//-----------------------------------------------------------------------------
UInt32 DEBUG_STREAM::GetSize() const
{
	return _nSize;
}

//-----------------------------------------------------------------------------
//	Name:		GetPos
//	Object:		
//	XX-XX-XX:	UNKNOWN - Created
//-----------------------------------------------------------------------------
UInt32 DEBUG_STREAM::GetPos() const
{
	return 0;
}

//-----------------------------------------------------------------------------
//	Name:		GetPos
//	Object:		
//	XX-XX-XX:	UNKNOWN - Created
//-----------------------------------------------------------------------------
int DEBUG_STREAM::operator ==(const char *pc)
{
	QDT_ASSERT(_nFlags & READ);
	return -1;
}

//-----------------------------------------------------------------------------
//	Name:		GetPos
//	Object:		
//	XX-XX-XX:	UNKNOWN - Created
//-----------------------------------------------------------------------------
void DEBUG_STREAM::SwitchWriteToRead()
{
	QDT_ASSERT(!(_nFlags & WRITE));
	_nFlags &= ~WRITE;
	_nFlags |= READ;
}

//-----------------------------------------------------------------------------
//	Name:		SwitchReadToWrite
//	Object:		
//	03-10-28:	VMA - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::SwitchReadToWrite()
{
	QDT_ASSERT(!(_nFlags & READ));
	_nFlags &= ~READ;
	_nFlags |= WRITE;
}

//-----------------------------------------------------------------------------
//	Name:		DEBUG_STREAM IsEof
//	Object:		
//	01-08-02:	VPI - Created
//-----------------------------------------------------------------------------
bool	DEBUG_STREAM::IsEof() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	02-05-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	DEBUG_STREAM::IsValid() const
{
	return	(true);
}

#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::DebugOutput(const char *	pTxt)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::DebugOutput(unsigned char	Char)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutput
//	Object:		
//	02-02-13:	ELE - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::DebugOutput(int	Int)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		DebugOutputBinary
//	Object:		
//	02-02-14:	ELE - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::DebugOutputBinary(int	Int)
{
	// Do nothing
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
int	DEBUG_STREAM::GetRealocSize() const
{
	return(IO_STREAM::BASE_SIZE);
}

//-----------------------------------------------------------------------------
//	Name:		SetRealocSize
//	Object:		
//	04-07-26:	VPI - Created
//-----------------------------------------------------------------------------
void	DEBUG_STREAM::SetRealocSize(const int	nRealocSize)
{
	//Do Nothing
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
