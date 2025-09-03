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
//	CLASS:	QDT_CONST_STRING
//
//	01-12-11:	ONA - Created
//*****************************************************************************

#include "Root.h"
#include "QdtConstString.h"

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Memory/MemPoolHeader)

#define		QDT_INCLUDE_STRING
#define		QDT_INCLUDE_STDLIB
#define		QDT_INCLUDE_STDARG
#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#define		EMPTY_STRING	"\0EMPTY BUFFER"

#ifdef ENABLE_QDT_CONST_STRING

static char	EmptyBuffer[sizeof(EMPTY_STRING)] = EMPTY_STRING;

const QDT_CONST_STRING	QDT_CONST_STRING::CONST_EMPTY_STRING = "";

//	Change to 1 to enable mass verification of all QDT_CONST_STRING operation in DEBUG
//	It has CheckMemory verification included ( performance drain in DEBUG)
//#define ENABLE_VERIFY_STRING 

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
#ifdef QDT_STRING_TRACK_NEW_DELETE
QDT_DLIST<QDT_CONST_STRING*> *	QDT_CONST_STRING::pAllocated	= NULL;
QDT_CONST_STRING *				QDT_CONST_STRING::pSTracked	= NULL;
#endif

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_CONST_STRING::GetSize() const
{
	return	(strlen(_pBuffer));
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::IsEmpty() const
{
	return	(GetSize() == 0);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_CONST_STRING::operator const char *() const
{
	return	(GetBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	06-02-22:	VHA - Created
//-----------------------------------------------------------------------------
QDT_CONST_STRING::operator const char * ()
{
	return (GetBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*QDT_CONST_STRING &	QDT_CONST_STRING::operator=(const QDT_CONST_STRING &	sText)
{
	if	(this != &sText)
	{

#ifdef ENABLE_VERIFY_STRING		
		QDT_ASSERT(sText.Verify());
#endif

		Empty();
		if	(sText.IsEmpty() == false)
		{
			Copy(sText.GetBufferPtr(), sText.GetSize());
		}
	}
	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*QDT_CONST_STRING &	QDT_CONST_STRING::operator=(const char * const	psText)
{
	int		nSize;

	if	(GetBufferPtr() != psText)
	{
		Empty();
		
		if	(psText == NULL)
		{
			nSize = 0;
		}
		else
		{
			nSize = strlen(psText);
			Copy(psText, nSize);
		}
	}

	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*QDT_CONST_STRING	QDT_CONST_STRING::operator+(const QDT_CONST_STRING &	sText) const
{
	QDT_CONST_STRING	S(*this);
	S += sText;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	01-12-21:	ONA - Created
//-----------------------------------------------------------------------------
/*QDT_CONST_STRING	QDT_CONST_STRING::operator+(const char	c) const
{
	QDT_CONST_STRING	S(*this);
	S += c;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const QDT_CONST_STRING &	sText)
{
	int	nOldSize, nAddedSize;

	if	(sText.IsEmpty() == false)
	{
		nOldSize = GetSize();
		nAddedSize = sText.GetSize();

		NeedSpace(nOldSize + nAddedSize + 1);
		QDT_MEMCOPY(GetBufferWithOffset(nOldSize), sText.GetBufferPtr(), nAddedSize + 1);
		SetSize(nOldSize + nAddedSize);
	}

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const char * const	psText)
{
	int	nOldSize, nAddedSize;

	nOldSize = GetSize();
	if	(psText == NULL)
	{
		nAddedSize = 0;
	}
	else
	{
		nAddedSize = strlen(psText);
	}

	NeedSpace(nOldSize + nAddedSize + 1);
	QDT_MEMCOPY(GetBufferWithOffset(nOldSize), psText, nAddedSize + 1);
	SetSize(nOldSize + nAddedSize);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const bool	bValue)
{
	QDT_CONST_STRING	S;
	S.ValueOf(bValue);
	*this += S;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const char	cChar)
{
	char	sBuffer[2] = "*";
	sBuffer[0] = cChar;
	*this += sBuffer;
	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const int	nValue)
{
	QDT_CONST_STRING	S;
	S.ValueOf(nValue);
	*this += S;
	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const float	rValue)
{
	QDT_CONST_STRING	S;
	S.ValueOf(rValue);
	*this += S;
	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
/*const QDT_CONST_STRING &	QDT_CONST_STRING::operator+=(const double	rValue)
{
	QDT_CONST_STRING	S;
	S.ValueOf(rValue);
	*this += S;
	return	(*this);
}*/

//-----------------------------------------------------------------------------
//	Name:		Atof
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
float	QDT_CONST_STRING::Atof() const
{
	return	(static_cast<float>(atof(GetBuffer())));
}

//-----------------------------------------------------------------------------
//	Name:		Atoi
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_CONST_STRING::Atoi() const
{
	return	(atoi(GetBuffer()));
}

//-----------------------------------------------------------------------------
//	Name:		Atol
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
long	QDT_CONST_STRING::Atol() const
{
	return	(atol(GetBuffer()));
}

//-----------------------------------------------------------------------------
//	Name:		Atob
//	Object:		
//	03-02-06:	FDE - Created
//-----------------------------------------------------------------------------
/*bool	QDT_CONST_STRING::Atob() const
{
	QDT_CONST_STRING sTmp(*this);
	
	sTmp.TrimLeft();
	sTmp.TrimRight();
	sTmp.MakeLower();

	return	(sTmp == "true");
}*/

//-----------------------------------------------------------------------------
//	Name:		SetFromTime
//	Object:		
//	03-11-12:	GGO - Created
//-----------------------------------------------------------------------------
/*void	QDT_CONST_STRING::SetFromTime(const int	nTimeMs)
{
	QDT_ASSERT( nTimeMs >= 0 );

	int		nMiliSeconds, nSeconds, nMinutes;
	
	nMiliSeconds	=   nTimeMs % 1000;
	nSeconds		= ( nTimeMs / 1000 ) % 60;
	nMinutes		=   nTimeMs / 60000;
	
	// Display minutes, seconds and milliseconds.
	if ( nMiliSeconds != 0 )
	{
		if ( nMinutes > 0 )
		{
			Format( "%dm%02ds%03d", nMinutes, nSeconds, nMiliSeconds );
		}
		else
		if ( nSeconds > 0 ) 
		{
			QDT_ASSERT( nMinutes == 0);

			Format( "%ds%03d", nSeconds, nMiliSeconds );
		}
		else
		{
			QDT_ASSERT( ( nMinutes == 0) && ( nSeconds == 0 ) );

			Format( "0s%03d", nMiliSeconds );
		}
	}
	else	// Display minutes, seconds but not the milliseconds.
	if ( nSeconds != 0 )
	{
		QDT_ASSERT( nMiliSeconds == 0 );

		if ( nMinutes > 0 )
		{
			Format( "%dm%02ds", nMinutes, nSeconds );
		}
		else
		{
			QDT_ASSERT( nMinutes == 0);

			Format( "%ds", nSeconds );
		}
	}
	else
	{
		QDT_ASSERT( ( nMiliSeconds == 0 ) && ( nSeconds == 0 ) );

		Format( "%dm", nMinutes );
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFromFileSize
//	Object:		
//	04-02-25:	GGO - Created
//-----------------------------------------------------------------------------
void	QDT_CONST_STRING::SetFromFileSize(const int	nFileSizeBytes)
{
	int	nMegaBytes, nKiloBytes, nBytes;

	if ( nFileSizeBytes < 0 )
	{
		operator=( "error : negative file size" );
		return;
	}

	nBytes		=   nFileSizeBytes % 1024;
	nKiloBytes	= ( nFileSizeBytes / 1024 ) % 1024;
	nMegaBytes	=   nFileSizeBytes / ( 1024 * 1024 );

	if ( nMegaBytes > 0 )
	{
		Format( "%d.%02d Mb", nMegaBytes, nKiloBytes * 100 / 1024 );
	}
	else
	if ( nKiloBytes > 0 )
	{
		QDT_ASSERT( nMegaBytes == 0 );

		Format( "%d.%02d Kb", nKiloBytes, nBytes * 100 / 1024  );
	}
	else
	{
		QDT_ASSERT( ( nKiloBytes == 0 ) && ( nMegaBytes == 0 ) );

		Format( "%d bytes", nBytes );
	}
}*/

//-----------------------------------------------------------------------------
//	Name:		CompareNoCase
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::CompareNoCase(const QDT_CONST_STRING &	sText) const
{
#if defined (PLATFORM_PS3)
	QDT_FAIL(); // Cette methode n'existe pas 
	return (false);
#else
	if	(GetSize() != sText.GetSize())
	{
		return	(false);
	}
	return	(stricmp(GetBuffer(), sText.GetBuffer()) == 0);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		CompareNoCase
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::CompareNoCase(const char * const	psText) const
{
#if defined (PLATFORM_PS3)
	QDT_FAIL(); // Cette methode n'existe pas 
	return (false);
#else
	if	(IsEmpty())
	{
		return	(*psText == '\0');
	}
	return	(stricmp(GetBuffer(), psText) == 0);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator==(const QDT_CONST_STRING &	sText) const
{
	if	(GetSize() != sText.GetSize())
	{
		return	(false);
	}
	return	(memcmp(GetBuffer(), sText.GetBuffer(), GetSize()) == 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator==(const char * const	psText) const
{
	if	(IsEmpty())
	{
		return	(*psText == '\0');
	}
	return	(strcmp(GetBuffer(), psText) == 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator!=(const QDT_CONST_STRING &	sText) const
{
	return	(!((*this) == sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator!=(const char * const	psText) const
{
	return	(!((*this) == psText));
}

//-----------------------------------------------------------------------------
//	Name:		operator<
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator<(const QDT_CONST_STRING &	sText) const
{
	if	(IsEmpty())
	{
		if	(sText.IsEmpty())
		{
			return	(false);
		}
		else
		{
			return	(true);
		}
	}

	return	(strcmp(GetBuffer(), sText.GetBuffer()) < 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator<
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator<(const char * const	psText) const
{
	if	(IsEmpty())
	{
		if	(*psText == '\0')
		{
			return	(false);
		}
		else
		{
			return	(true);
		}
	}

	return	(strcmp(GetBuffer(), psText) < 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator<=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator<=(const QDT_CONST_STRING &	sText) const
{
	if	(IsEmpty())
	{
		return	(true);
	}

	return	(strcmp(GetBuffer(), sText.GetBuffer()) <= 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator<=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator<=(const char * const	psText) const
{
	if	(IsEmpty())
	{
		return	(true);
	}

	return	(strcmp(GetBuffer(), psText) <= 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator>
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator>(const QDT_CONST_STRING &	sText) const
{
	return	(!((*this) <= sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator>
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator>(const char * const	psText) const
{
	return	(!((*this) <= psText));
}

//-----------------------------------------------------------------------------
//	Name:		operator>=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator>=(const QDT_CONST_STRING &	sText) const
{
	return	(!((*this) < sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator>=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::operator>=(const char * const	psText) const
{
	return	(!((*this) < psText));
}

//-----------------------------------------------------------------------------
//	Name:		ToUpper
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_CONST_STRING::ToUpper() const
{
	QDT_STRING	S(*this);
	S.MakeUpper();
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ToLower
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_CONST_STRING::ToLower() const
{
	QDT_STRING	S(*this);
	S.MakeLower();
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_CONST_STRING::Write(IO_STREAM &	Stream) const
{
	Stream << GetSize();
	Stream.PutBuffer( GetBuffer(), GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		GetAt
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
char	QDT_CONST_STRING::GetAt(const int	nPosition) const
{
	QDT_ASSERT(nPosition <= GetSize());
	return	(GetBuffer()[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		WriteDependenciesBinary
//	Object:		
//	03-11-27:	VPI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	QDT_CONST_STRING::WriteDependenciesBinary(IO_STREAM &	Stream,
											int			nThisOffset)
{
	if ( _pBuffer )
	{
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pBuffer - (char*)this), Stream.GetPos() );
		Stream.PutBufferFast( _pBuffer, strlen(_pBuffer) + 1 ); //+1 is for \0
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		InitBinary
//	Object:		
//	03-11-27:	VPI - Created
//-----------------------------------------------------------------------------
void	QDT_CONST_STRING::InitBinary()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Print
//	Object:		
//	04-06-03:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	QDT_CONST_STRING::Print(IO_STREAM * pStream)
{
}
#endif


#ifdef QDT_STRING_TRACK_NEW_DELETE
//-----------------------------------------------------------------------------
//	Name:		AddAllocated
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CONST_STRING::AddAllocated(QDT_CONST_STRING *	pS)
{
	if	(pAllocated == NULL)
	{
		pAllocated = new QDT_DLIST<QDT_CONST_STRING*>[QDT_STRING_HASH_SIZE];
	}

	int nPos = HasCodeAllocated(pS);
	if	(pAllocated[nPos].IsEmpty() == false)
	{
		if	(pAllocated[nPos].Find(pAllocated[nPos].GetHead(), pS) != pAllocated[nPos].GetTail())
		{
			QDT_Error("QDT_CONST_STRING at address 0x%08X already newed", pS);
		}
	}
	
	if	(pS == pSTracked)
	{
		char			Buffer[4096] = "";
		char			LocalBuffer[2048] = "";
		unsigned int	i;
		void			*pAddress;

		for(i = 0 ; i < 32 ; ++i)
		{
			pAddress = OS::GetCallStack(i);
			if	(pAddress == NULL)
			{
				break;
			}
			sprintf(LocalBuffer, "\n	%08X : ", pAddress);
			strncat(Buffer, LocalBuffer, sizeof(Buffer));

			OS::GetFunctionName(pAddress, LocalBuffer, sizeof(LocalBuffer));
			strncat(Buffer, LocalBuffer, sizeof(LocalBuffer));
		}

		QDT_Message("QDT_CONST_STRING::AddAllocated(): string address 0x%08X newed \n CallStack: %s", pS, Buffer);
	}

	pAllocated[nPos].InsertTail(pS);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAllocated
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CONST_STRING::RemoveAllocated(QDT_CONST_STRING *	pS)
{
	QDT_ASSERT(pAllocated);

	QDT_DLIST<QDT_CONST_STRING*>::ITERATOR It;

	int nPos = HasCodeAllocated(pS);
	It = pAllocated[nPos].GetTail();
	if	(pAllocated[nPos].IsEmpty() == false)
	{
		It = pAllocated[nPos].Find(pAllocated[nPos].GetHead(), pS);
		if	(It == pAllocated[nPos].GetTail())
		{
			//QDT_Error("QDT_CONST_STRING at address 0x%08X has never been newed !!!", pS);
			return;
		}
	}
	
	if	(pS == pSTracked)
	{
		QDT_Message("QDT_CONST_STRING::RemoveAllocated(): string address 0x%08X removed", pS);
	}

	if	(It != pAllocated[nPos].GetTail())
	{
		pAllocated[nPos].Remove(It);
	}

}

//-----------------------------------------------------------------------------
//	Name:		HasCodeAllocated
//	Object:		
//	04-09-09:	DAC - Created
//-----------------------------------------------------------------------------
int	QDT_CONST_STRING::HasCodeAllocated(QDT_CONST_STRING *	pS)
{
	int nHashCode;
	QDT_ASSERT(pS != NULL);

	int iS = (int) pS;
	nHashCode = iS % QDT_STRING_HASH_SIZE;
	
	return	(nHashCode);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		DumpAllocated
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CONST_STRING::DumpAllocated()
{
#ifdef QDT_STRING_TRACK_NEW_DELETE
	int									i;
	QDT_DLIST<QDT_CONST_STRING*>::ITERATOR	It,ItEnd;
	QDT_CONST_STRING *						pS;

	for	(i = 0; i < QDT_STRING_HASH_SIZE; ++i)
	{
		It		= pAllocated[i].GetHead();
		ItEnd	= pAllocated[i].GetTail();
		while(It != ItEnd)
		{
			pS = (*It);
			QDT_Message("string address 0x%08X never removed: \"%s\"", pS, pS->GetBuffer());
			++It;
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//	Name:		IsAlpha
//	Object:		
//	05-04-08:	AMA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::IsAlpha(unsigned char	c)
{
	//A -> Z : 65 -> 90
	//a -> z : 97 -> 122
	
	if(( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ))
	{
		return ( true );
	}
	
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		IsNumeric
//	Object:		
//	05-04-08:	AMA - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::IsNumeric(unsigned char	c)
{
	//0 -> 9 : 48 -> 57
	if( c >= '0' && c <= '9' ) 
	{
		return ( true );
	}
	
	return( false );
}

//-----------------------------------------------------------------------------
//	Name:		IsAlphaNumeric
//	Object:		
//	05-12-15:	DAC - Created
//-----------------------------------------------------------------------------
bool	QDT_CONST_STRING::IsAlphaNumeric(unsigned char	c)
{
	return( QDT_CONST_STRING::IsAlpha(c) || QDT_CONST_STRING::IsNumeric(c) );
}

#endif


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
