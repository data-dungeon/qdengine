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
//	CLASS:	QDT_STRING
//
//	01-12-11:	ONA - Created
//*****************************************************************************

#include "Root.h"

#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(Tools/QdtConstString)
#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Memory/MemPoolHeader)

#ifdef USE_NEW_MEMPOOL
#include	INCL_KCORE(Mem/MemPoolManager)
#endif

#define		QDT_INCLUDE_STRING
#define		QDT_INCLUDE_STDLIB
#define		QDT_INCLUDE_STDARG
#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#define		EMPTY_STRING	"\0EMPTY BUFFER"

static char	EmptyBuffer[sizeof(EMPTY_STRING)] = EMPTY_STRING;

const QDT_STRING	QDT_STRING::CONST_EMPTY_STRING;

//	Change to 1 to enable mass verification of all QDT_STRING operation in DEBUG
//	It has CheckMemory verification included ( performance drain in DEBUG)
//#define ENABLE_VERIFY_STRING 

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
#ifdef USE_NEW_MEMPOOL
MEM_POOL_MANAGER *	QDT_STRING::_pMPManager = NULL;
#else
MEM_POOL_HEADER *	QDT_STRING::_pMPH8 = NULL;
MEM_POOL_HEADER *	QDT_STRING::_pMPH16 = NULL;
MEM_POOL_HEADER *	QDT_STRING::_pMPH32 = NULL;
MEM_POOL_HEADER *	QDT_STRING::_pMPH64 = NULL;
MEM_POOL_HEADER *	QDT_STRING::_pMPH128 = NULL;
MEM_POOL_HEADER *	QDT_STRING::_pMPH256 = NULL;
MEM_POOL_HEADER *	QDT_STRING::_pMPH512 = NULL;
#endif

#ifdef QDT_STRING_TRACK_NEW_DELETE
QDT_DLIST<QDT_STRING*> *	QDT_STRING::pAllocated	= NULL;
QDT_STRING *				QDT_STRING::pSTracked	= NULL;
#endif


//-----------------------------------------------------------------------------
//	Name:		QDT_STRING constructor
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::QDT_STRING(const int	nReserve)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	Reserve(nReserve);

#ifdef QDT_STRING_TRACK_NEW_DELETE
	AddAllocated(this);
#endif
}



//-----------------------------------------------------------------------------
//	Name:		Construct
//	Object:		to solve placement new problems under PS2
//	01-12-20:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Construct()
{
	_pBuffer = NULL;
	_nSize = 0;
	_nAllocSize	= 0;
}

//-----------------------------------------------------------------------------
//	Name:		Destruct
//	Object:		
//	01-12-20:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Destruct()
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING constructor
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::QDT_STRING(const QDT_STRING &	sText)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
#ifdef QDT_STRING_TRACK_NEW_DELETE
	AddAllocated(this);
#endif

	if	(sText.IsEmpty() == false)
	{
		Copy(sText.GetBufferPtr(), sText.GetSize());
	}
}
#ifdef ENABLE_QDT_CONST_STRING
//-----------------------------------------------------------------------------
//	Name:		QDT_STRING constructor
//	Object:		
//	06-02-22:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING::QDT_STRING(const QDT_CONST_STRING & sText)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
#ifdef QDT_STRING_TRACK_NEW_DELETE
	AddAllocated(this);
#endif

	if	(sText.IsEmpty() == false)
	{
		Copy(sText.GetBuffer(), sText.GetSize());
	}
}
#endif
//-----------------------------------------------------------------------------
//	Name:		QDT_STRING constructor
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::QDT_STRING(const char * const	psText)
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
		nSize = strlen(psText);
	}

	if	(nSize)
	{
		Reserve(nSize + 1);
		Copy(psText, nSize);
	}

#ifdef QDT_STRING_TRACK_NEW_DELETE
	AddAllocated(this);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING constructor
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::QDT_STRING(const char * const	psText,
					   const int			nLength)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	if	(*psText)
	{
		int	nSize;
		nSize = strlen(psText);
		if	(nSize > nLength)
		{
			Copy(psText, nLength);
			SetAt(nLength, '\0');
		}
		else
		{
			Copy(psText, nSize);
		}
	}
	
#ifdef QDT_STRING_TRACK_NEW_DELETE
	AddAllocated(this);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_STRING constructor
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::QDT_STRING(const char	cChar,
					   const int	nRepeat)
:
_pBuffer(NULL),
_nSize(0),
_nAllocSize(0)
{
	NeedSpace(nRepeat + 1);
	SetSize(nRepeat);			//	final '\0' placed in SetSize()
	memset(GetBufferPtr(), cChar, nRepeat);
	

#ifdef QDT_STRING_TRACK_NEW_DELETE
	AddAllocated(this);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		~QDT_STRING destructor
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::~QDT_STRING()
{
	ReleaseBuffer();
#ifdef QDT_STRING_TRACK_NEW_DELETE
	RemoveAllocated(this);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		ReleaseBuffer
//	Object:		
//	01-12-19:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ReleaseBuffer()
{
	if	(_nAllocSize == 0)
	{
		QDT_ASSERT(_nSize == 0);
		QDT_ASSERT(_pBuffer == NULL);
		return;
	}
#ifdef USE_NEW_MEMPOOL
	else
	{
		if(!_pMPManager)
		{
			_pMPManager = MEM::GetInstance()->getMemPoolCustom(8, 64, 80, MP_CUSTOMID_QDT_STRING);
		}
		_pMPManager->FreeCustom(_pBuffer);
	}
#else
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
#endif
	_pBuffer = NULL;
	_nAllocSize = 0;
	_nSize = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Reserve
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Reserve(const int	nSize)
{
	int		nAllocSize;
	char *	pNewBuffer;

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

#ifdef USE_NEW_MEMPOOL
	//if	(nSize <= 512)
	{
		if(_pMPManager == NULL)
		{
			_pMPManager = MEM::GetInstance()->getMemPoolCustom(8, 64, 80, MP_CUSTOMID_QDT_STRING);
		}

#ifdef QDT_MEM_DEBUG
		pNewBuffer = (char*)_pMPManager->AllocateCustom(nSize,QDT_LIB_ID);
#else
		pNewBuffer = (char*)_pMPManager->AllocateCustom(nSize);
#endif

		QDT_ASSERT(pNewBuffer);

		nAllocSize = nSize;

	}
	/*else
	{
		pNewBuffer = new char[nSize];
		nAllocSize = nSize;
	}*/
#else
	if	(nSize <= 8)
	{
		if	(_pMPH8 == NULL)
		{
			_pMPH8 = new	MEM_POOL_HEADER(8, 1536, 32);
		}

		pNewBuffer = static_cast<char *>(_pMPH8->Allocate());

		nAllocSize = 8;
	}
	else if	(nSize <= 16)
	{
		if	(_pMPH16 == NULL)
		{
			_pMPH16 = new	MEM_POOL_HEADER(16, 1024, 32);
		}

		pNewBuffer = static_cast<char *>(_pMPH16->Allocate());

		nAllocSize = 16;
	}
	else if	(nSize <= 32)
	{
		if	(_pMPH32 == NULL)
		{
			_pMPH32 = new	MEM_POOL_HEADER(32, 1024, 32);
		}

		pNewBuffer = static_cast<char *>(_pMPH32->Allocate());

		nAllocSize = 32;
	}
	else if	(nSize <= 64)
	{
		if	(_pMPH64 == NULL)
		{
			_pMPH64 = new	MEM_POOL_HEADER(64, 736, 32);
		}

		pNewBuffer = static_cast<char *>(_pMPH64->Allocate());

		nAllocSize = 64;
	}
	else if	(nSize <= 128)
	{
		if	(_pMPH128 == NULL)
		{
			_pMPH128 = new	MEM_POOL_HEADER(128, 256, 64);
		}

		pNewBuffer = static_cast<char *>(_pMPH128->Allocate());

		nAllocSize = 128;
	}
	else if	(nSize <= 256)
	{
		if	(_pMPH256 == NULL)
		{
			_pMPH256 = new	MEM_POOL_HEADER(256, 32, 8);
		}

		pNewBuffer = static_cast<char *>(_pMPH256->Allocate());

		nAllocSize = 256;
	}
	else if	(nSize <= 512)
	{
		if	(_pMPH512 == NULL)
		{
			_pMPH512 = new	MEM_POOL_HEADER(512, 16, 8);
		}

		pNewBuffer = static_cast<char *>(_pMPH512->Allocate());

		nAllocSize = 512;
	}
	else
	{
		QDT_ENSURE(nSize > 512);
		pNewBuffer = new	char[nSize];
		nAllocSize = nSize;
	}
#endif

	if	(GetBufferPtr())
	{
		int	nOldSize;
		nOldSize = GetSize();
		QDT_MEMCOPY(pNewBuffer, GetBufferPtr(), GetSize() + 1);		//	Copy the final '\0' also
		ReleaseBuffer();

		_pBuffer = pNewBuffer;
		SetMaxSize(nAllocSize);
		//	Store the old size, because ReleaseBuffer() clear it !
		//	SetSize must be pushed after SetMaxSize()
		SetSize(nOldSize);
	}
	else
	{
		//	The buffer is empty, just put the final '\0'
		*pNewBuffer = '\0';
		_pBuffer = pNewBuffer;
		SetMaxSize(nAllocSize);
		QDT_ASSERT(GetSize() == 0);
	}

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Resize
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Resize(const int	nSize,
						   const int	nChar)
{
	int	nOldSize;

	nOldSize = GetSize();
	Reserve(nSize + 1);
	if	(nSize < nOldSize)
	{
		SetSize(nSize);
		SetAt(nSize, '\0');
	}
	else if	(nSize > nOldSize)
	{
		if	(nChar)
		{
			memset(GetBufferWithOffset(nOldSize), nChar, nSize - nOldSize);
			SetSize(nSize);
			SetAt(nSize, '\0');
		}
	}
	else
	{
		//	Same size, do nothing...
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::GetSize() const
{
	return	(_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSize
//	Object:		
//	02-03-27:	GGO - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::UpdateSize()
{
	SetSize(strlen(GetBuffer()));
}

//-----------------------------------------------------------------------------
//	Name:		GetMaxSize
//	Object:		
//	02-01-08:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::GetMaxSize() const
{
	return	(_nAllocSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetMaxSize
//	Object:		
//	02-01-08:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::SetMaxSize(const int	nMaxSize)
{
	_nAllocSize = nMaxSize;
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const char *	QDT_STRING::GetBuffer() const
{
	if	(_pBuffer)
	{
		return	(_pBuffer);
	}
	//	The string is empty, ensure the return of a correct buffer
	return	(EmptyBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetFullBufferPtr
//	Object:		//Use with Care it's for speed optimization
//	04-07-21:	VPI - Created
//-----------------------------------------------------------------------------
char *	QDT_STRING::GetFullBufferPtr()
{
		if	(_pBuffer)
	{
		return	(_pBuffer);
	}
	//	The string is empty, ensure the return of a correct buffer
	return	(EmptyBuffer);
}


//-----------------------------------------------------------------------------
//	Name:		GetBufferPtr
//	Object:		not const counter part to GetBuffer() for internal use only
//	01-12-20:	ONA - Created
//-----------------------------------------------------------------------------
char *	QDT_STRING::GetBufferPtr() const
{
	return	(_pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		IsEmpty
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::IsEmpty() const
{
	return	(GetSize() == 0);
}

//-----------------------------------------------------------------------------
//	Name:		Empty
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Empty(const bool	bPartial)
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
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const char &	QDT_STRING::operator[](const int	nPosition) const
{
	QDT_ASSERT(nPosition >= 0);
	QDT_ASSERT(nPosition <= GetSize());
	return	(_pBuffer[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
char &	QDT_STRING::operator[](const int	nPosition)
{
	QDT_ASSERT(nPosition >= 0);
	QDT_ASSERT(nPosition <= GetSize());
	return	(_pBuffer[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING::operator const char *() const
{
	return	(GetBuffer());
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING &	QDT_STRING::operator=(const QDT_STRING &	sText)
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
}
#ifdef ENABLE_QDT_CONST_STRING
//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	06-02-22:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING &	QDT_STRING::operator=(const QDT_CONST_STRING & sText)
{
#ifdef ENABLE_VERIFY_STRING		
	QDT_ASSERT(sText.Verify());
#endif

	Empty();
	if	(sText.IsEmpty() == false)
	{
		Copy(sText.GetBuffer(), sText.GetSize());
	}
	return	(*this);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING &	QDT_STRING::operator=(const char * const	psText)
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
}

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::operator+(const QDT_STRING &	sText) const
{
	QDT_STRING	S(*this);
	S += sText;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}
#ifdef ENABLE_QDT_CONST_STRING
//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	06-02-22:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::operator+(const QDT_CONST_STRING & sText) const
{
	QDT_STRING	S(*this);
	S += sText;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		operator+
//	Object:		
//	01-12-21:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::operator+(const char	c) const
{
	QDT_STRING	S(*this);
	S += c;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const QDT_STRING &	sText)
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
}
#ifdef ENABLE_QDT_CONST_STRING
//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	05-02-21:	VHA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const QDT_CONST_STRING & sText)
{
	int	nOldSize, nAddedSize;

	if	(sText.IsEmpty() == false)
	{
		nOldSize = GetSize();
		nAddedSize = sText.GetSize();

		NeedSpace(nOldSize + nAddedSize + 1);
		QDT_MEMCOPY(GetBufferWithOffset(nOldSize), sText.GetBuffer(), nAddedSize + 1);
		SetSize(nOldSize + nAddedSize);
	}

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const char * const	psText)
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
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const bool	bValue)
{
	QDT_STRING	S;
	S.ValueOf(bValue);
	*this += S;

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const char	cChar)
{
	char	sBuffer[2] = "*";
	sBuffer[0] = cChar;
	*this += sBuffer;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const int	nValue)
{
	QDT_STRING	S;
	S.ValueOf(nValue);
	*this += S;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const float	rValue)
{
	QDT_STRING	S;
	S.ValueOf(rValue);
	*this += S;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const double	rValue)
{
	QDT_STRING	S;
	S.ValueOf(rValue);
	*this += S;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Atof
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
float	QDT_STRING::Atof() const
{
	return	(static_cast<float>(atof(GetBuffer())));
}

//-----------------------------------------------------------------------------
//	Name:		Atoi
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::Atoi() const
{
	return	(atoi(GetBuffer()));
}

//-----------------------------------------------------------------------------
//	Name:		Atol
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
long	QDT_STRING::Atol() const
{
	return	(atol(GetBuffer()));
}

//-----------------------------------------------------------------------------
//	Name:		Atob
//	Object:		
//	03-02-06:	FDE - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::Atob() const
{
	QDT_STRING sTmp(*this);
	
	sTmp.TrimLeft();
	sTmp.TrimRight();
	sTmp.MakeLower();

	return	(sTmp == "true");
}

//-----------------------------------------------------------------------------
//	Name:		SetFromTime
//	Object:		
//	03-11-12:	GGO - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::SetFromTime(const int	nTimeMs)
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
void	QDT_STRING::SetFromFileSize(const int	nFileSizeBytes)
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
}

//-----------------------------------------------------------------------------
//	Name:		CompareNoCase
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::CompareNoCase(const QDT_STRING &	sText) const
{
#if defined (PLATFORM_PS3)
	if	(GetSize() != sText.GetSize())
	{
		return	(false);
	}

	QDT_STRING sCopy = *this;
	sCopy.MakeUpper();

	QDT_STRING sTextCopy = sText;
	sTextCopy.MakeUpper();

	return	(strcmp(sCopy.GetBuffer(), sTextCopy.GetBuffer()) == 0);
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
bool	QDT_STRING::CompareNoCase(const char * const	psText) const
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
bool	QDT_STRING::operator==(const QDT_STRING &	sText) const
{
	if	(GetSize() != sText.GetSize())
	{
		return	(false);
	}
	return	(memcmp(GetBuffer(), sText.GetBuffer(), GetSize()) == 0);
}
#ifdef ENABLE_QDT_CONST_STRING
//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	06-02-22:	VHA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator==(const QDT_CONST_STRING &	sText) const
{
	if	(GetSize() != sText.GetSize())
	{
		return	(false);
	}
	return	(memcmp(GetBuffer(), sText.GetBuffer(), GetSize()) == 0);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		operator==
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator==(const char * const	psText) const
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
bool	QDT_STRING::operator!=(const QDT_STRING &	sText) const
{
	return	(!((*this) == sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator!=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator!=(const char * const	psText) const
{
	return	(!((*this) == psText));
}

//-----------------------------------------------------------------------------
//	Name:		operator<
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator<(const QDT_STRING &	sText) const
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
bool	QDT_STRING::operator<(const char * const	psText) const
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
bool	QDT_STRING::operator<=(const QDT_STRING &	sText) const
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
bool	QDT_STRING::operator<=(const char * const	psText) const
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
bool	QDT_STRING::operator>(const QDT_STRING &	sText) const
{
	return	(!((*this) <= sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator>
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator>(const char * const	psText) const
{
	return	(!((*this) <= psText));
}

//-----------------------------------------------------------------------------
//	Name:		operator>=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator>=(const QDT_STRING &	sText) const
{
	return	(!((*this) < sText));
}

//-----------------------------------------------------------------------------
//	Name:		operator>=
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::operator>=(const char * const	psText) const
{
	return	(!((*this) < psText));
}

//-----------------------------------------------------------------------------
//	Name:		MakeUpper
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::MakeUpper()
{
#if defined (PLATFORM_PS3)
	for (UInt32 i = 0; i < _nSize; ++i)
	{
		if ( (_pBuffer[i] >= 'a') && (_pBuffer[i] <= 'z') )
			_pBuffer[i] -= 0x20;
	}
#else
	if	(IsEmpty() == false)
	{
		strupr(GetBufferPtr());
	}

	#ifdef ENABLE_VERIFY_STRING	
		QDT_ASSERT(Verify());
	#endif

#endif
}

//-----------------------------------------------------------------------------
//	Name:		ToUpper
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToUpper() const
{
	QDT_STRING	S(*this);
	S.MakeUpper();
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		MakeLower
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::MakeLower()
{
#if defined (PLATFORM_PS3)
	for (UInt32 i = 0; i < _nSize; ++i)
	{
		if ( (_pBuffer[i] >= 'A') && (_pBuffer[i] <= 'Z') )
			_pBuffer[i] += 0x20;
	}
#else
	if	(IsEmpty() == false)
	{
		strlwr(GetBufferPtr());
	}

	#ifdef ENABLE_VERIFY_STRING
		QDT_ASSERT(Verify());
	#endif

#endif

}

//-----------------------------------------------------------------------------
//	Name:		ToLower
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToLower() const
{
	QDT_STRING	S(*this);
	S.MakeLower();
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		Replace
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::Replace(const QDT_STRING &	sOld,
						const QDT_STRING &	sNew)
{
	int			nPosition;
	int			nFoundPosition;
	QDT_STRING	S;
	int			nNbrFound;

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






#define STR_FORMAT_BUFFER_SIZE 20 * 1024 
static char StrBuffer[STR_FORMAT_BUFFER_SIZE] ="";
//-----------------------------------------------------------------------------
//	Name:		Format
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Format(const char * const	psFormatText,
						   ...					)
{
	va_list	vl;
	int		nCount;

	va_start(vl, psFormatText);

	/*nSize = 2 * strlen(psFormatText);
	for(;;)
	{
		//Reserve(nSize);

//		nCount = _vsnprintf(GetBufferPtr(), nSize - 1, psFormatText, vl);
	*/	
		nCount = vsprintf(StrBuffer, psFormatText, vl);
		
		QDT_ENSURE(nCount < STR_FORMAT_BUFFER_SIZE);

		if	(nCount >= 0)
		{
			NeedSpace(nCount + 1);
			QDT_MEMCOPY(GetBufferPtr(), StrBuffer, nCount + 1);
			//break;
		}

		//	Restore the right context because the buffer is crashed by _vsnprintf
		//SetSize(0);

		//	The buffer is not enough big, increase it !
		//nSize *= 2;
	/*}*/

	va_end(vl);

	SetSize(nCount);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif
}


void	QDT_STRING::AppendFormat(const char * const	psFormatText,
								 ...					)
{
	va_list	vl;
	int		nCount;

	va_start(vl, psFormatText);


	nCount = vsprintf(StrBuffer, psFormatText, vl);
	
	QDT_ENSURE(nCount < STR_FORMAT_BUFFER_SIZE);

	va_end(vl);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(Verify());
#endif

	*this += StrBuffer;
}
//-----------------------------------------------------------------------------
//	Name:		Find
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::Find(const QDT_STRING &	sText,
					 const int			nStartPos) const
{
	char *	pResultBuffer;

	if	(IsEmpty())
	{
		return	(-1);
	}

	if	(sText.IsEmpty())
	{
		return	(nStartPos);
	}

	pResultBuffer = strstr(GetBufferPtr() + nStartPos, sText.GetBuffer());
	if	(pResultBuffer)
	{
		int	nReturnValue;
		nReturnValue = pResultBuffer - GetBufferPtr();

		QDT_ASSERT(nReturnValue != GetSize());
		QDT_ASSERT(nReturnValue < GetSize());

		return	(nReturnValue);
	}

	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		Find
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::Find(const int	nChar,
					 const int	nStartPos) const
{
	char *	pResultBuffer;

	if	(IsEmpty())
	{
		return	(-1);
	}

	pResultBuffer = strchr(GetBufferPtr() + nStartPos, nChar);
	if	(pResultBuffer)
	{
		return	(pResultBuffer - GetBufferPtr());
	}

	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		ReverseFind
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::ReverseFind(const QDT_STRING &	sText,
							const int			nStartPos) const
{
	char *	pBuffer;
	char *	pResultBuffer;
	char *	pBackupBuffer;

	if	(GetSize() <= nStartPos)
	{
		return	(-1);
	}

	if	(sText.IsEmpty())
	{
		return	(GetSize() - 1);
	}

	pBuffer = GetBufferPtr() + nStartPos;
	pBackupBuffer = NULL;
	for(;;)
	{
		pResultBuffer = strstr(pBuffer, sText);
		if	(pResultBuffer)
		{
			pBackupBuffer = pResultBuffer;
		}
		else
		{
			break;
		}

		pBuffer += sText.GetSize();
	}

	if	(pBackupBuffer)
	{
		return	(pBackupBuffer - (GetBufferPtr() + nStartPos));
	}

	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		ReverseFind
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::ReverseFind(const int	nChar,
							const int	nStartPos) const
{
	char *	pResultBuffer;

	if	(IsEmpty())
	{
		return	(-1);
	}

	pResultBuffer = strrchr(GetBufferPtr() + nStartPos, nChar);
	if	(pResultBuffer)
	{
		return	(pResultBuffer - GetBufferPtr() + nStartPos);
	}

	return	(-1);
}

//-----------------------------------------------------------------------------
//	Name:		FindCount
//	Object:		
//	04-06-01:	VPI - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::FindCount(const QDT_STRING &	sText,
						  const int				nStartPos) const
{
	int nCount = 0;
	int nPos;
	int nStart = nStartPos;
	do
	{
		nPos = Find(sText, nStart);
		if (nPos != -1)
		{
			nStart = nPos + 1;
			++nCount;
		}
	}
	while (nPos != -1);

	return (nCount);
}

//-----------------------------------------------------------------------------
//	Name:		FindCount
//	Object:		
//	04-06-01:	VPI - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::FindCount(const int	nChar,
						  const int	nStartPos) const
{
	QDT_STRING sChar;
	sChar += (char)nChar;
	return (FindCount( sChar, nStartPos));
}


//-----------------------------------------------------------------------------
//	Name:		EraseMid
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::EraseMid(const int	nFirst,
								 const int	nCount) const
{
	QDT_STRING	S;
	int			nNewSize;

	QDT_ASSERT(nFirst >= 0);
	QDT_ASSERT(nFirst + nCount <= GetSize());

	S.Reserve(GetSize() + 1);

	QDT_MEMCOPY(S.GetBufferPtr(), GetBufferPtr(), nFirst);
	nNewSize = GetSize() - (nFirst + nCount);
	QDT_MEMCOPY(S.GetBufferPtr() + nFirst, GetBufferWithOffset(nFirst + nCount), nNewSize);
	S.SetSize(nFirst + nNewSize);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		EraseMid
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::EraseMid(const int	nFirst) const
{
	return	(EraseMid(nFirst, GetSize() - nFirst));
}

//-----------------------------------------------------------------------------
//	Name:		EraseLeft
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::EraseLeft(const int	nCount) const
{
	return	(EraseMid(0, nCount));
}

//-----------------------------------------------------------------------------
//	Name:		EraseRight
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::EraseRight(const int	nCount) const
{
	return	(EraseMid(GetSize() - nCount, nCount));
}

//-----------------------------------------------------------------------------
//	Name:		ExtractMid
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ExtractMid(const int	nFirst,
								   const int	nCount) const
{
	QDT_STRING	S;

	QDT_ASSERT(nFirst >= 0);
	QDT_ASSERT(nCount >= 0);
	QDT_ASSERT(nFirst + nCount <= GetSize());

	S.Reserve(nCount + 1);

	QDT_MEMCOPY(S.GetBufferPtr(), GetBufferWithOffset(nFirst), nCount);
	S.SetSize(nCount);

#ifdef ENABLE_VERIFY_STRING
	QDT_ASSERT(S.Verify());
#endif

	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ExtractMid
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ExtractMid(const int	nFirst) const
{
	return	(ExtractMid(nFirst, GetSize() - nFirst));
}

//-----------------------------------------------------------------------------
//	Name:		ExtractLeft
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ExtractLeft(const int	nCount) const
{
	return	(ExtractMid(0, nCount));
}

//-----------------------------------------------------------------------------
//	Name:		ExtractRight
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ExtractRight(const int	nCount) const
{
	return	(ExtractMid(GetSize() - nCount, nCount));
}

//-----------------------------------------------------------------------------
//	Name:		TrimLeft
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	 QDT_STRING::TrimLeft(const char cTrim)
{
	int		nPosition;
	int		nChar;
	bool	bOut;

	nPosition = 0;
	bOut = false;
	while	(nPosition < GetSize())
	{
		nChar = operator[](nPosition);

		if	((cTrim != '\0') && (nChar == cTrim))
		{
			++nPosition;
		}
		else
		{
			switch(nChar)
			{
			case	' ':
			case	'\t':
			case	'\n':
			case	'\r':
				++nPosition;
				break;
			default:
				bOut = true;
				break;
			}
		}

		if	(bOut)
		{
			break;
		}
	}

	if	(nPosition)
	{
		QDT_MEMCOPY(GetBufferPtr(), GetBufferWithOffset(nPosition), GetSize() - nPosition);
		SetSize(GetSize() - nPosition);
	}

	return	(nPosition);
}

//-----------------------------------------------------------------------------
//	Name:		TrimRight
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
int	 QDT_STRING::TrimRight(const char cTrim)
{
	int		nPosition;
	int		nChar;
	bool	bOut;
	int		nOldSize;

	nPosition = GetSize() - 1;
	bOut = false;
	while	(nPosition >= 0)
	{
		nChar = operator[](nPosition);

		if	( (cTrim != '\0') && (nChar == cTrim))
		{
			--nPosition;
		}
		else
		{
			switch(nChar)
			{
			case	' ':
			case	'\t':
			case	'\n':
			case	'\r':
				--nPosition;
				break;
			default:
				bOut = true;
				break;
			}
		}

		if	(bOut)
		{
			break;
		}
	}

	nOldSize = GetSize();
	if	(nPosition != GetSize() - 1)
	{
		SetSize(nPosition + 1);
	}

	return	(nOldSize - GetSize());
}

//-----------------------------------------------------------------------------
//	Name:		Reverse
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Reverse()
{
	if	(IsEmpty())
	{
		return;
	}

#ifdef PLATFORM_PC
	_strrev(GetBufferPtr());
#else
	char *pCharBegin = GetBufferPtr();
	char *pCharEnd = GetBufferPtr() + strlen(_pBuffer);
	while (pCharBegin < pCharEnd)
	{
		char c = *pCharBegin;
		*pCharBegin = *pCharEnd;
		*pCharEnd = c;
		pCharBegin++;
		pCharEnd--;
	}
#endif	
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Read(IO_STREAM &	Stream)
{
	int	nSize;

	Stream >> nSize;
	Reserve( nSize + 1);
	Stream.GetBuffer( GetBufferPtr(), nSize );
	SetSize(nSize);
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Write(IO_STREAM &	Stream) const
{
	Stream << GetSize();
	Stream.PutBuffer( GetBuffer(), GetSize() );
}

//-----------------------------------------------------------------------------
//	Name:		ValueOf
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ValueOf(const float	rValue)
{
	Format("%g", rValue);
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	02-01-04:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToString(const float	rValue)
{
	QDT_STRING	S;
	S.ValueOf(rValue);
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ValueOf
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ValueOf(const int	nValue)
{
	Format("%ld", nValue);
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	02-01-04:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToString(const int	nValue)
{
	QDT_STRING	S;
	S.ValueOf(nValue);
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ValueOf
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ValueOf(const bool	bValue)
{
	Empty();
	if	(bValue)
	{
		static const char	sBuffer[] = "true";
		static const int	nSize = strlen(sBuffer);
		Copy(sBuffer, nSize);
	}
	else
	{
		static const char	sBuffer[] = "false";
		static const int	nSize = strlen(sBuffer);
		Copy(sBuffer, nSize);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	02-01-04:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToString(const bool	bValue)
{
	QDT_STRING	S;
	S.ValueOf(bValue);
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ValueOf
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ValueOf(const double	rValue)
{
	Format("%g", rValue);
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	02-01-04:	ONA - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToString(const double	rValue)
{
	QDT_STRING	S;
	S.ValueOf(rValue);
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		GetAt
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
char	QDT_STRING::GetAt(const int	nPosition) const
{
	QDT_ASSERT(nPosition <= GetSize());
	return	(GetBufferPtr()[nPosition]);
}

//-----------------------------------------------------------------------------
//	Name:		SetAt
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::SetAt(const int	nPosition,
						  const int	nChar)
{
	QDT_ASSERT(nPosition <= GetSize());
	GetBufferPtr()[nPosition] = nChar;
}

//-----------------------------------------------------------------------------
//	Name:		GetBufferWithOffset
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
char *	QDT_STRING::GetBufferWithOffset(const int	nPosition) const
{
	return	(GetBufferPtr() + nPosition);
}

//-----------------------------------------------------------------------------
//	Name:		NeedSpace
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::NeedSpace(const int	nSize)
{
	int	nMaxSize;
	nMaxSize = GetMaxSize();
	if	(nSize > nMaxSize)
	{
		Reserve((nSize * RATIO_INCREASE_BUFFER) / RATIO_BASE);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSize
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::SetSize(const int	nSize)
{
	QDT_ASSERT(nSize < GetMaxSize());
	_nSize = nSize;
	if	(GetBufferPtr())
	{
		SetAt(nSize, '\0');
	}
	else
	{
		QDT_ASSERT(nSize == 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::Copy(const char * const	pBuffer,
						 const int			nSize)
{
	NeedSpace(nSize + 1);
	QDT_MEMCOPY(GetBufferPtr(), pBuffer, nSize + 1);
	SetSize(nSize);

#ifdef ENABLE_VERIFY_STRING	
	QDT_ASSERT(Verify());
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Verify
//	Object:		
//	01-12-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	QDT_STRING::Verify() const
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
			nSize = strlen(GetBuffer());
		}

		if	(nSize != GetSize())
		{
			return	(false);
		}
	}
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		SplitPathFileName
//	Object:		
//	02-01-22:	DAC - Created
//  06-01-12:	RMA - Modified to manage path with "/"
//-----------------------------------------------------------------------------
bool	QDT_STRING::SplitPathFileName(const QDT_STRING &	sCompletePath,
									  QDT_STRING &			sPath,
									  QDT_STRING &			sName)
{
	int nPos = sCompletePath.ReverseFind("\\");

	if	(nPos >= 0)
	{
		sPath = sCompletePath.ExtractLeft(nPos);
		sName = sCompletePath.EraseLeft(nPos + 1);

		return	(true);
	}
	else
	{
		int nPos = sCompletePath.ReverseFind("/");
		
		if	(nPos >= 0)
		{
			sPath = sCompletePath.ExtractLeft(nPos);
			sName = sCompletePath.EraseLeft(nPos + 1);

			return	(true);
		}
		else
		{
			sPath = "";
			sName = sCompletePath;

			return	(false);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		WriteDependenciesBinary
//	Object:		
//	03-11-27:	VPI - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	QDT_STRING::WriteDependenciesBinary(IO_STREAM &	Stream,
											int			nThisOffset)
{
	if ( _pBuffer )
	{
		Stream.AddPadding(16);
		Stream.AddRelocationLink( nThisOffset + ((char*)&_pBuffer - (char*)this), Stream.GetPos() );
		Stream.PutBufferFast( _pBuffer, _nSize + 1 ); //+1 is for \0
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		InitBinary
//	Object:		
//	03-11-27:	VPI - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::InitBinary()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Print
//	Object:		
//	04-06-03:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	QDT_STRING::Print(IO_STREAM * pStream)
{
#ifndef USE_NEW_MEMPOOL
	QDT_ASSERT(pStream);
	pStream->PutString("\nQDT_STRING 8");
	if(_pMPH8)		_pMPH8->Print(pStream);;
	pStream->PutString("\nQDT_STRING 16");
	if(_pMPH16)		_pMPH16->Print(pStream);
	pStream->PutString("\nQDT_STRING 32");
	if(_pMPH32)		_pMPH32->Print(pStream);
	pStream->PutString("\nQDT_STRING 64");
	if(_pMPH64)		_pMPH64->Print(pStream);
	pStream->PutString("\nQDT_STRING 128");
	if(_pMPH128)	_pMPH128->Print(pStream);
	pStream->PutString("\nQDT_STRING 256");
	if(_pMPH256)	_pMPH256->Print(pStream);
	pStream->PutString("\nQDT_STRING 512");
	if(_pMPH512)	_pMPH512->Print(pStream);
#endif
}
#endif

//-----------------------------------------------------------------------------
//	Name:		GetMemoryUsed
//	Object:		
//	04-09-03:	DAC - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::GetMemoryUsed()
{
	int nTotal = 0;
#ifndef USE_NEW_MEMPOOL
	if(_pMPH8)
	{
		nTotal += _pMPH8->GetNbrTotal() * _pMPH8->GetSize();
	}
	
	if(_pMPH16)
	{
		nTotal += _pMPH16->GetNbrTotal() * _pMPH16->GetSize();
	}

	if(_pMPH32)
	{
		nTotal += _pMPH32->GetNbrTotal() * _pMPH32->GetSize();
	}

	if(_pMPH64)
	{
		nTotal += _pMPH64->GetNbrTotal() * _pMPH64->GetSize();
	}

	if(_pMPH128)
	{
		nTotal += _pMPH128->GetNbrTotal() * _pMPH128->GetSize();
	}

	if(_pMPH256)
	{
		nTotal += _pMPH256->GetNbrTotal() * _pMPH256->GetSize();
	}

	if(_pMPH512)
	{
		nTotal += _pMPH512->GetNbrTotal() * _pMPH512->GetSize();
	}
#endif
	return	(nTotal);
}

//-----------------------------------------------------------------------------
//	Name:		GetFreeMem
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
int	QDT_STRING::GetFreeMem()
{
		int nTotal = 0;
#ifndef USE_NEW_MEMPOOL
	if(_pMPH8)
	{
		nTotal += _pMPH8->GetFreeMem();
	}
	
	if(_pMPH16)
	{
		nTotal += _pMPH16->GetFreeMem();
	}

	if(_pMPH32)
	{
		nTotal += _pMPH32->GetFreeMem();
	}

	if(_pMPH64)
	{
		nTotal += _pMPH64->GetFreeMem();
	}

	if(_pMPH128)
	{
		nTotal += _pMPH128->GetFreeMem();
	}

	if(_pMPH256)
	{
		nTotal += _pMPH256->GetFreeMem();
	}

	if(_pMPH512)
	{
		nTotal += _pMPH512->GetFreeMem();
	}
#endif
	return	(nTotal);
}


#ifdef QDT_STRING_TRACK_NEW_DELETE
//-----------------------------------------------------------------------------
//	Name:		AddAllocated
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::AddAllocated(QDT_STRING *	pS)
{
	if	(pAllocated == NULL)
	{
		pAllocated = new QDT_DLIST<QDT_STRING*>[QDT_STRING_HASH_SIZE];
	}

	int nPos = HasCodeAllocated(pS);
	if	(pAllocated[nPos].IsEmpty() == false)
	{
		if	(pAllocated[nPos].Find(pAllocated[nPos].GetHead(), pS) != pAllocated[nPos].GetTail())
		{
			QDT_Error("QDT_STRING at address 0x%08X already newed", pS);
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

		QDT_Message("QDT_STRING::AddAllocated(): string address 0x%08X newed \n CallStack: %s", pS, Buffer);
	}

	pAllocated[nPos].InsertTail(pS);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveAllocated
//	Object:		
//	04-09-16:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::RemoveAllocated(QDT_STRING *	pS)
{
	QDT_ASSERT(pAllocated);

	QDT_DLIST<QDT_STRING*>::ITERATOR It;

	int nPos = HasCodeAllocated(pS);
	It = pAllocated[nPos].GetTail();
	if	(pAllocated[nPos].IsEmpty() == false)
	{
		It = pAllocated[nPos].Find(pAllocated[nPos].GetHead(), pS);
		if	(It == pAllocated[nPos].GetTail())
		{
			//QDT_Error("QDT_STRING at address 0x%08X has never been newed !!!", pS);
			return;
		}
	}
	
	if	(pS == pSTracked)
	{
		QDT_Message("QDT_STRING::RemoveAllocated(): string address 0x%08X removed", pS);
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
int	QDT_STRING::HasCodeAllocated(QDT_STRING *	pS)
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
void	QDT_STRING::DumpAllocated()
{
#ifdef QDT_STRING_TRACK_NEW_DELETE
	int									i;
	QDT_DLIST<QDT_STRING*>::ITERATOR	It,ItEnd;
	QDT_STRING *						pS;

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
bool	QDT_STRING::IsAlpha(unsigned char	c)
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
bool	QDT_STRING::IsNumeric(unsigned char	c)
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
bool	QDT_STRING::IsAlphaNumeric(unsigned char	c)
{
	return( QDT_STRING::IsAlpha(c) || QDT_STRING::IsNumeric(c) );
}

//-----------------------------------------------------------------------------
//	Name:		operator+=
//	Object:		
//	06-02-21:	CAU - Created
//-----------------------------------------------------------------------------
const QDT_STRING &	QDT_STRING::operator+=(const UInt64	&	nValue)
{
	QDT_STRING	S;
	S.ValueOf(nValue);
	*this += S;
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		ValueOf
//	Object:		
//	06-02-21:	CAU - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ValueOf(const UInt64 &	nValue)
{
	Format("%I64u", nValue);
}

//-----------------------------------------------------------------------------
//	Name:		ToString
//	Object:		
//	06-02-21:	CAU - Created
//-----------------------------------------------------------------------------
QDT_STRING	QDT_STRING::ToString(const UInt64 &	nValue)
{
	QDT_STRING	S;
	S.ValueOf(nValue);
	return	(S);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBufferToHexaString
//	Object:		Creates a string from a char buffer
//				Can be used to output a binary stream to a text format
//
//	@param		pBuffer	the char buffer to output as hexadecimal values
//	@param		nSize	the buffer size
//	06-06-12:	CAU - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ConvertBufferToHexaString(const char *	pBuffer,
											  const int &	nSize)
{
	QDT_ASSERT( pBuffer != NULL &&
				nSize != 0 );

	Reserve(nSize * 2 + 1);	// the '\0' will be added at the end of the string
		
	const char *	pEnd(pBuffer + nSize),
			   *	pItSrc(pBuffer);
	char *	pItDest(_pBuffer);

	while(pItSrc!=pEnd)
	{
		if((((*pItSrc) & 0xF0) >> 4) > 9)
		{
			(*pItDest) = 'A' + ((((*pItSrc) & 0xF0) >> 4) - 10);
		}
		else
		{
			(*pItDest) = '0' + (((*pItSrc) & 0xF0) >> 4);
		}

		++pItDest;

		if(((*pItSrc) & 0xF) > 9)
		{
			(*pItDest) = 'A' + (((*pItSrc) & 0xF) - 10);
		}
		else
		{
			(*pItDest) = '0' + ((*pItSrc) & 0xF);
		}	 

		++pItSrc;
		++pItDest;
	}
	(*pItDest) = '\0';
}

//-----------------------------------------------------------------------------
//	Name:		ConvertHexaStringToBuffer
//	Object:		Convert the string to a char buffer
//				Can be used to get a binary stream from a text format
//
//	@param		pBuffer	the char buffer that will receive the binary stream (MUST be NULL)
//	@param		nSize	the buffer size that will receive the stream size
//	06-06-12:	CAU - Created
//-----------------------------------------------------------------------------
void	QDT_STRING::ConvertHexaStringToBuffer(char *&	pBuffer,
											  int &		nSize) const
{
	QDT_ASSERT( pBuffer == NULL );

	int strSize = GetSize();
	if(strSize == 0)
	{
		nSize = 0;
		return;
	}
	nSize = strSize / 2;
	QDT_ASSERT( (strSize & 0x1) == 0 );	// the size must be even
	pBuffer = new char [nSize];
	
	const char *	pEnd(_pBuffer + strSize),
			   *	pItSrc(_pBuffer);
	char *	pItDest(pBuffer);

	while(pItSrc!=pEnd)
	{
		QDT_ASSERT( ((*pItSrc) >= '0' && (*pItSrc) <= '9') ||
					((*pItSrc) >= 'A' && (*pItSrc) <= 'F') );	// invalid HexaString
		if((*pItSrc) > '9')
		{
			(*pItDest) = (10 + ((*pItSrc) - 'A')) << 4;
		}
		else
		{
			(*pItDest) = ((*pItSrc) - '0') << 4;
		}

		++pItSrc;

		QDT_ASSERT( ((*pItSrc) >= '0' && (*pItSrc) <= '9') ||
					((*pItSrc) >= 'A' && (*pItSrc) <= 'F') );	// invalid HexaString

		if((*pItSrc) > '9')
		{
			(*pItDest) |= (10 + ((*pItSrc) - 'A'));
		}
		else
		{
			(*pItDest) |= ((*pItSrc) - '0');
		}

		++pItSrc;
		++pItDest;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
