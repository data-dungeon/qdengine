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
//	CLASS:	QDT_STRING_UNICODE
//
//	03-05-19:	NCA - Created
//*****************************************************************************

#include "Root.h"
#include	INCL_KCORE(Tools/QDTStringUnicode)

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Memory/MemPoolHeader)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//	Change to 1 to enable mass verification of all QDT_STRING_UNICODE operation in DEBUG
//	It has CheckMemory verification included ( performance drain in DEBUG)
//#define ENABLE_VERIFY_STRING 

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

unsigned short		QDT_STRING_UNICODE::EmptyBuffer = 0;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH8 = NULL;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH16 = NULL;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH32 = NULL;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH64 = NULL;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH128 = NULL;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH256 = NULL;
MEM_POOL_HEADER *	QDT_STRING_UNICODE::_pMPH512 = NULL;

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING_UNICODE constructor
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE::QDT_STRING_UNICODE(const int	nReserve)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	Reserve(nReserve);
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING_UNICODE constructor
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE::QDT_STRING_UNICODE(const QDT_STRING_UNICODE &	sText)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	if	(sText.IsEmpty() == false)
	{
		Copy(sText.GetBufferPtr(), sText.GetSize());
	}
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING_UNICODE constructor
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE::QDT_STRING_UNICODE(const QDT_STRING &	sText)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	if	(sText.IsEmpty() == false)
	{
		Copy( sText );
	}
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING_UNICODE constructor
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE::QDT_STRING_UNICODE(const unsigned short * const	psText)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	int		nSize;

	if (psText == NULL)
	{
		nSize = 0;
	}
	else
	{
		nSize = QDT_STRING_UNICODE::UnicodeStrLen( psText );
	}

	if	(nSize)
	{
		Reserve(nSize + 1);
		Copy(psText, nSize);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-05-21:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Copy(const unsigned short * const	pBuffer)
{
	int nSize = UnicodeStrLen( pBuffer );
	Copy( pBuffer, nSize );
}


//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Copy(	const unsigned short * const	pBuffer,
									const int						nSize)
{
	NeedSpace(nSize + 1);
	QDT_MEMCOPY(GetBufferPtr(), pBuffer, (nSize + 1) * sizeof( unsigned short) );
	SetSize(nSize);

#ifdef ENABLE_VERIFY_STRING	
	QDT_ASSERT(Verify());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-05-21:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Copy(const QDT_STRING_UNICODE &	sString)
{
	Copy( sString.GetBufferPtr(), sString.GetSize() );
}


//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-05-21:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Copy(const QDT_STRING &	sQDTString)
{
	unsigned short	nChar;
	int				nSize = sQDTString.GetSize();

	if (nSize)
	{
		NeedSpace(nSize + 1);
		SetSize(nSize);

		int		i;
		for (i=0; i<nSize; ++i)
		{
			nChar = sQDTString.GetBuffer()[i];
			nChar &= 0x00FF; // We need this line, else nChar == 0xFF??, instead of 0x00?? (don't ask me why).
			SetAt(i, nChar );
		}
	}

#ifdef ENABLE_VERIFY_STRING	
	QDT_ASSERT(Verify());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		~QDT_STRING_UNICODE destructor
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE::~QDT_STRING_UNICODE()
{
	ReleaseBuffer();
}

//-----------------------------------------------------------------------------
//	Name:		UnicodeStrLen
//	Object:		
//	03-05-20:	NCA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING_UNICODE::UnicodeStrLen( const unsigned short *	pBuffer)
{
	int i;

	for ( i=0; i<MAX_STRING_UNICODE_LENGTH; ++i )
	{
		if ( pBuffer[i] == 0 )
		{
			break;
		}
	}

	QDT_ASSERT( i != MAX_STRING_UNICODE_LENGTH );

	return( i );
}

//-----------------------------------------------------------------------------
//	Name:		CleanAnsi
//	Object:		
//	04-11-05:	GGO - Created
//-----------------------------------------------------------------------------
void QDT_STRING_UNICODE::CleanAnsi(QDT_STRING_UNICODE & sResult) const
{
	int					i, nSize = GetSize();

	sResult.Empty();

	for (i=0; i<nSize; ++i)
	{
		switch ( _pBuffer[i] )
		{
			case 0x0153:
				{
					sResult += (unsigned short) 'o';
					sResult += (unsigned short) 'e';
				}
				break;
			case 0x2013:
				{
					sResult += (unsigned short) '-';
				}
				break;
			case 0x2018:
			case 0x2019:
				{
					sResult += (unsigned short) '\'';
				}
				break;
			case 0x201C:
			case 0x201D:
				{
					sResult += (unsigned short) '\"';
				}
				break;
			case 0x2026:
				{
					sResult += (unsigned short) '.';
					sResult += (unsigned short) '.';
					sResult += (unsigned short) '.';
				}
				break;
			default:
				{
					QDT_ASSERT( ((_pBuffer[i] & 0xFF00) == 0) && "A not displayable character has been found in the string you are trying to play..." );
					sResult += _pBuffer[i];
				}
				break;
		}			
	}
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseBuffer
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::ReleaseBuffer()
{
	if	(_nAllocSize == 0)
	{
		QDT_ASSERT(_nSize == 0);
		QDT_ASSERT(_pBuffer == NULL);
		return;
	}
	else if	(_nAllocSize == 8)
	{
		_pMPH8->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH8->Verify());
	}
	else if	(_nAllocSize == 16)
	{
		_pMPH16->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH16->Verify());
	}
	else if	(_nAllocSize == 32)
	{
		_pMPH32->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH32->Verify());
	}
	else if	(_nAllocSize == 64)
	{
		_pMPH64->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH64->Verify());
	}
	else if	(_nAllocSize == 128)
	{
		_pMPH128->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH128->Verify());
	}
	else if	(_nAllocSize == 256)
	{
		_pMPH256->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH256->Verify());
	}
	else if	(_nAllocSize == 512)
	{
		_pMPH512->Deallocate(_pBuffer);
//		QDT_ASSERT(_pMPH512->Verify());
	}
	else
	{
		QDT_ENSURE(_nAllocSize > 512);

		delete [] _pBuffer;
	}
	_pBuffer = NULL;
	_nAllocSize = 0;
	_nSize = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Reserve
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Reserve(const int	nSize)
{
	int		nAllocSize;
	unsigned short *	pNewBuffer;

	if	(nSize <= GetMaxSize())
	{
		//	Smaller reserve, don't change the allocation
		if	(nSize < GetSize())
		{
			//	Shrink the string
			SetSize(nSize);
		}
		return;
	}

	int nRealSize = nSize * sizeof( unsigned short );

	if	(nRealSize <= 8)
	{
		if	(_pMPH8 == NULL)
		{
			_pMPH8 = new	MEM_POOL_HEADER(8, 64, 32);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH8->Allocate());

		nAllocSize = 8;
	}
	else if	(nRealSize <= 16)
	{
		if	(_pMPH16 == NULL)
		{
			_pMPH16 = new	MEM_POOL_HEADER(16, 96, 32);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH16->Allocate());

		nAllocSize = 16;
	}
	else if	(nRealSize <= 32)
	{
		if	(_pMPH32 == NULL)
		{
			_pMPH32 = new	MEM_POOL_HEADER(32, 96, 32);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH32->Allocate());

		nAllocSize = 32;
	}
	else if	(nRealSize <= 64)
	{
		if	(_pMPH64 == NULL)
		{
			_pMPH64 = new	MEM_POOL_HEADER(64, 64, 64);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH64->Allocate());

		nAllocSize = 64;
	}
	else if	(nRealSize <= 128)
	{
		if	(_pMPH128 == NULL)
		{
			_pMPH128 = new	MEM_POOL_HEADER(128, 192, 64);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH128->Allocate());

		nAllocSize = 128;
	}
	else if	(nRealSize <= 256)
	{
		if	(_pMPH256 == NULL)
		{
			_pMPH256 = new	MEM_POOL_HEADER(256, 32, 8);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH256->Allocate());

		nAllocSize = 256;
	}
	else if	(nRealSize <= 512)
	{
		if	(_pMPH512 == NULL)
		{
			_pMPH512 = new	MEM_POOL_HEADER(512, 16, 8);
		}

		pNewBuffer = static_cast<unsigned short *>(_pMPH512->Allocate());

		nAllocSize = 512;
	}
	else
	{
		QDT_ENSURE(nRealSize > 512);

		pNewBuffer = new	unsigned short[nSize];

		nAllocSize = nRealSize;
	}

	if	(GetBufferPtr())
	{
		int	nOldSize;
		nOldSize = GetSize();
		QDT_MEMCOPY(pNewBuffer, GetBufferPtr(), (GetSize() + 1) * sizeof(unsigned short) );		//	Copy the final L'\0' also
		ReleaseBuffer();

		_pBuffer = pNewBuffer;
		SetMaxSize(nAllocSize / sizeof(unsigned short) );
		//	Store the old size, because ReleaseBuffer() clear it !
		//	SetSize must be pushed after SetMaxSize()
		SetSize(nOldSize);
	}
	else
	{
		//	The buffer is empty, just put the final '\0'
		*pNewBuffer = 0;
		_pBuffer = pNewBuffer;
		SetMaxSize(nAllocSize  / sizeof(unsigned short) );
		QDT_ASSERT(GetSize() == 0);
	}

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Resize
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Resize(	const int	nSize,
									const unsigned short	nChar)
{
	int	nOldSize;

	nOldSize = GetSize();
	Reserve(nSize + 1);
	if	(nSize < nOldSize)
	{
		SetSize(nSize);
		SetAt(nSize, 0);
	}
	else
	{
		if	(nSize > nOldSize)
		{
			if	(nChar)
			{
				memset(GetBufferWithOffset(nOldSize), nChar, (nSize - nOldSize) * sizeof(unsigned short) );
				SetSize(nSize);
				SetAt(nSize, 0);
			}
		}
		else
		{
			//	Same size, do nothing...
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		QD_WCSSTR
//	Object:		Because PS2 implementation of wcsstr is bugged
//	05-10-26:	VMA - Created
//-----------------------------------------------------------------------------
unsigned short * QD_WCSSTR(const unsigned short *ws1, const unsigned short *ws2)
{
	const unsigned short *	os1, *os2;
	const unsigned short *	tptr;
	unsigned short		    c;

	os1 = ws1;
	os2 = ws2;

	if (os1 == NULL || *os2 == 0)
		return ((unsigned short *)os1);
	c = *os2;

	while (*os1)
	{
		if (*os1++ == c)
		{
			tptr = os1;
			while (((c = *++os2) == *os1++) && (c != 0));
			if (c == 0)
				return ((unsigned short *)tptr - 1);
			os1 = tptr;
			os2 = ws2;
			c = *os2;
		}
	}
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		Find
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
int	QDT_STRING_UNICODE::Find(const QDT_STRING_UNICODE &	sText,
							 const int					nStartPos) const
{
	unsigned short * pResultBuffer;

	if	(IsEmpty())
	{
		return	(-1);
	}

	if	(sText.IsEmpty())
	{
		return	(nStartPos);
	}

	pResultBuffer = QD_WCSSTR(GetBufferPtr() + nStartPos, sText.GetBuffer());
	if	(pResultBuffer)
	{
		int	nReturnValue = pResultBuffer - GetBufferPtr();

		QDT_ASSERT(nReturnValue != GetSize());
		QDT_ASSERT(nReturnValue < GetSize());

		return	(nReturnValue);
	}

	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		Replace
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
int	QDT_STRING_UNICODE::Replace(const QDT_STRING_UNICODE &	sOld,
								const QDT_STRING_UNICODE &	sNew)
{
	int					nPosition;
	int					nFoundPosition;
	QDT_STRING_UNICODE	S;
	int					nNbrFound;

	if	(IsEmpty())
	{
		return	(0);
	}

	if	(sOld.IsEmpty())
	{
		return	(0);
	}

	nPosition = 0;
	nNbrFound = 0;
	for(;;)
	{
		nFoundPosition = Find(sOld, nPosition);
		if	(nFoundPosition == -1)
		{
			break;
		}

		QDT_ASSERT(nPosition < GetSize());
		QDT_ASSERT(nFoundPosition < GetSize());

		S += ExtractMid(nPosition, nFoundPosition - nPosition);
		S += sNew;

		nPosition = nFoundPosition + sOld.GetSize();
		++nNbrFound;
	}

	if	(nNbrFound)
	{
		S += ExtractMid(nPosition);
		if	(S.IsEmpty() == false)
		{
			Copy(S.GetBufferPtr(), S.GetSize());
		}
		else
		{
			Empty();
		}
	}

	return	(nNbrFound);
}

//-----------------------------------------------------------------------------
//	Name:		ExtractMid
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE	QDT_STRING_UNICODE::ExtractMid(const int	nFirst,
												   const int	nCount) const
{
	QDT_STRING_UNICODE	S;

	QDT_ASSERT(nFirst >= 0);
	QDT_ASSERT(nCount >= 0);
	QDT_ASSERT(nFirst + nCount <= GetSize());

	S.Reserve(nCount + 1);

	QDT_MEMCOPY(S.GetBufferPtr(), GetBufferWithOffset(nFirst), nCount * sizeof(short) );
	S.SetSize(nCount);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ExtractMid
//	Object:		
//	05-09-23:	GGO - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE	QDT_STRING_UNICODE::ExtractMid(const int	nFirst) const
{
	return	(ExtractMid(nFirst, GetSize() - nFirst));
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING_UNICODE::GetSize() const
{
	return	(_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetSize
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::SetSize(const int	nSize)
{
	QDT_ASSERT(nSize < GetMaxSize());
	_nSize = nSize;
	if	( GetBufferPtr() )
	{
		SetAt(nSize, 0);
	}
	else
	{
		QDT_ASSERT(nSize == 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSize
//	Object:		
//	03-05-19:	GGO - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::UpdateSize()
{
	const unsigned short * pBuffer = GetBuffer();
	QDT_ASSERT( pBuffer != NULL );

	int nSize = UnicodeStrLen( pBuffer );

	SetSize( nSize );
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxSize
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING_UNICODE::GetMaxSize() const
{
	return	(_nAllocSize / sizeof(unsigned short) );
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxSize
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::SetMaxSize(const int	nMaxSize)
{
	_nAllocSize = nMaxSize * sizeof(unsigned short);
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
const unsigned short *	QDT_STRING_UNICODE::GetBuffer() const
{
	if	(_pBuffer)
	{
		return	(_pBuffer);
	}
	//	The string is empty, ensure the return of a correct buffer
	return	(&EmptyBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferPtr
//	Object:		not const counter part to GetBuffer() for internal use only
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
unsigned short *	QDT_STRING_UNICODE::GetBufferPtr() const
{
	return	(_pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	03-07-09:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING_UNICODE::ToString() const
{
	QDT_STRING	s;
	int			i, nSize;

	nSize = GetSize();

	for (i=0; i<nSize; ++i)
	{
		s += char( GetAt(i) & 0x0000FFFF );
	}

	s += '\0';

	return( s );
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::IsEmpty() const
{
	return	(GetSize() == 0);
}

//-----------------------------------------------------------------------------
//	Name:		Empty
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Empty(const bool	bPartial)
{
	if	(GetSize())
	{
		SetSize(0);
	}
	if	(bPartial == false)
	{
		ReleaseBuffer();
	}
#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
const unsigned short &	QDT_STRING_UNICODE::operator[](const int	nPosition) const
{
	QDT_ASSERT(nPosition >= 0);
	QDT_ASSERT(nPosition <= GetSize());
	return	(_pBuffer[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
unsigned short &	QDT_STRING_UNICODE::operator[](const int	nPosition)
{
	QDT_ASSERT(nPosition >= 0);
	QDT_ASSERT(nPosition <= GetSize());
	return	(_pBuffer[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE::operator const unsigned short *() const
{
	return	(GetBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-05-19:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE &	QDT_STRING_UNICODE::operator=(const QDT_STRING_UNICODE &	sText)
{
	if	(this != &sText)
	{

#ifdef ENABLE_VERIFY_STRING		
		QDT_ASSERT(sText.Verify());
#endif

		Empty();
		if	(sText.IsEmpty() == false)
		{
			Copy( sText );
		}
	}
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE &	QDT_STRING_UNICODE::operator=(const unsigned short * const	psText)
{
	int		nSize = 0;

	if	(GetBufferPtr() != psText)
	{
		Empty();
		
		if	(psText != NULL)
		{
			nSize = UnicodeStrLen( psText );
			Copy(psText, nSize);
		}
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-05-21:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE &	QDT_STRING_UNICODE::operator=(const QDT_STRING &	sText)
{
#ifdef ENABLE_VERIFY_STRING		
		QDT_ASSERT(sText.Verify());
#endif

	Empty();

	if	(sText.IsEmpty() == false)
	{
		Copy( sText );
	}

	return	(*this);
}


//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
QDT_STRING_UNICODE	QDT_STRING_UNICODE::operator+(const QDT_STRING_UNICODE &	sText) const
{
	QDT_STRING_UNICODE	S(*this);
	S += sText;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
const QDT_STRING_UNICODE &	QDT_STRING_UNICODE::operator+=(const QDT_STRING_UNICODE &	sText)
{
	int	nOldSize, nAddedSize;

	if	(sText.IsEmpty() == false)
	{
		nOldSize = GetSize();
		nAddedSize = sText.GetSize();

		NeedSpace(nOldSize + nAddedSize + 1);
		QDT_MEMCOPY(GetBufferWithOffset(nOldSize), sText.GetBufferPtr(), (nAddedSize + 1) * sizeof( unsigned short) );
		SetSize(nOldSize + nAddedSize);
	}

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
const QDT_STRING_UNICODE &	QDT_STRING_UNICODE::operator+=(const unsigned short * const	psText)
{
	int	nOldSize, nAddedSize;

	nOldSize = GetSize();
	if	(psText == NULL)
	{
		nAddedSize = 0;
	}
	else
	{
		nAddedSize = UnicodeStrLen(psText);
	}

	NeedSpace(nOldSize + nAddedSize + 1);

	QDT_MEMCOPY( GetBufferWithOffset(nOldSize), psText, (nAddedSize + 1) * sizeof(unsigned short) );

	SetSize(nOldSize + nAddedSize);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
const QDT_STRING_UNICODE &	QDT_STRING_UNICODE::operator+=(const unsigned short	nChar)
{
	unsigned short	sBuffer[2];
	sBuffer[0] = nChar;
	sBuffer[1] = 0;
	*this += sBuffer;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		CompareNoCase
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::Compare(const QDT_STRING_UNICODE &	sText) const
{
	int nSize = GetSize();
	if	( nSize != sText.GetSize())
	{
		return	(false);
	}

	int i;
	for (i=0; i<nSize; ++i)
	{
		if ( GetAt(i) != sText.GetAt(i) )
		{
			return( false );
		}
	}

	return	( true );
}

//-----------------------------------------------------------------------------
//	Name:		CompareNoCase
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::Compare(const unsigned short * const	psText) const
{
	if	(IsEmpty())
	{
		return	( UnicodeStrLen( psText ) == 0 );
	}

	int nSize = GetSize();
	if	( nSize != UnicodeStrLen( psText ))
	{
		return	(false);
	}

	int i;
	for (i=0; i<nSize; ++i)
	{
		if ( GetAt(i) != psText[i] )
		{
			return( false );
		}
	}

	return	( true );
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::operator==(const QDT_STRING_UNICODE &	sText) const
{
	if	(GetSize() != sText.GetSize())
	{
		return	(false);
	}
	return	(memcmp(GetBuffer(), sText.GetBuffer(), GetSize() * sizeof(unsigned short) ) == 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::operator==(const unsigned short * const	psText) const
{
	return	( Compare(psText) != 0);
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::operator!=(const QDT_STRING_UNICODE &	sText) const
{
	return	(!((*this) == sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::operator!=(const unsigned short * const	psText) const
{
	return	(!((*this) == psText));
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Read(IO_STREAM &	Stream)
{
	int	nSize;

	Stream >> nSize;
	nSize /= sizeof(unsigned short);
	Reserve( nSize + 1);
	Stream.GetBuffer( GetBufferPtr(), nSize * sizeof(unsigned short));
	SetSize(nSize);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::Write(IO_STREAM &	Stream) const
{
	Stream << GetSize() * sizeof(unsigned short);
	Stream.PutBuffer( GetBuffer(), GetSize() * sizeof(unsigned short) );
}


//-----------------------------------------------------------------------------
//	Name:		GetAt
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
unsigned short	QDT_STRING_UNICODE::GetAt(const int	nPosition) const
{
	QDT_ASSERT(nPosition <= GetSize());
	return	(GetBufferPtr()[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		SetAt
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::SetAt(	const int	nPosition,
									const unsigned short	nChar)
{
	QDT_ASSERT(nPosition <= GetSize());
	GetBufferPtr()[nPosition] = nChar;
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferWithOffset
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
unsigned short *	QDT_STRING_UNICODE::GetBufferWithOffset(const int	nPosition) const
{
	return	(GetBufferPtr() + nPosition );
}

//-----------------------------------------------------------------------------
//	Name:		NeedSpace
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::NeedSpace(const int	nSize)
{
	int	nMaxSize;
	nMaxSize = GetMaxSize();
	if	(nSize > nMaxSize)
	{
		Reserve((nSize * RATIO_INCREASE_BUFFER) / RATIO_BASE);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Verify
//	Object:		
//	01-12-11:	NCA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING_UNICODE::Verify() const
{
	if	(GetMaxSize() == 0)
	{
		if	(GetSize())
		{
			return	(false);
		}
		if	(GetBufferPtr())
		{
			return	(false);
		}
	}
	else
	{
		if	(GetBufferPtr() == NULL)
		{
			return	(false);
		}
		
		int nSize;
		if	(GetBufferPtr() == NULL)
		{
			nSize = 0;
		}
		else
		{
			nSize = UnicodeStrLen(GetBuffer());
		}

		if	(nSize != GetSize())
		{
			return	(false);
		}
	}
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		WriteDependenciesBinary
//	Object:		
//	03-12-10:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	QDT_STRING_UNICODE::WriteDependenciesBinary(IO_STREAM &	Stream,
												   int			nThisOffset)
{
	if ( _pBuffer )
	{
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pBuffer - (char*)this), Stream.GetPos() );
		Stream.PutBufferFast( _pBuffer, _nSize * 2 + 2 ); //+1 is for \0
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		InitBinary
//	Object:		
//	03-12-10:	VMA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING_UNICODE::InitBinary()
{
	// Do nothing
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
