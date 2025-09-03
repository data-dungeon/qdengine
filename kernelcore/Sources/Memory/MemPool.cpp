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
//	CLASS:	MEM_POOL
//
//	02-01-03:	ONA - Created
//*****************************************************************************

#include "Root.h"
#include "MemPool.h"

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

int			MEM_POOL::_nMemoryUsed = 0;

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL constructor
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
MEM_POOL::MEM_POOL(const int	nSize,
				   const int	nCapacity)
:
_nSize(nSize),
_nCapacity(nCapacity),
_pBuffer(new	char[nSize * nCapacity])
{
#ifdef QDT_DEBUG
	memset(_pBuffer, 0xBB, nSize * nCapacity);
#endif
	_nMemoryUsed += nSize * nCapacity;
}

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL constructor
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
MEM_POOL::MEM_POOL(const MEM_POOL & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		MEM_POOL destructor
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
MEM_POOL::~MEM_POOL()
{
	_nMemoryUsed -= _nSize * _nCapacity;

#ifdef QDT_DEBUG
	memset(_pBuffer, 0xCC, _nSize * _nCapacity);
#endif
	delete	[]	(int*)_pBuffer;
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
MEM_POOL &	MEM_POOL::operator=(const MEM_POOL & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
void	MEM_POOL::Copy(const MEM_POOL & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL::GetSize() const
{
	return	(_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetCapacity
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL::GetCapacity() const
{
	return	(_nCapacity);
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL::GetBuffer() const
{
	return	(_pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	02-01-03:	ONA - Created
//-----------------------------------------------------------------------------
void *	MEM_POOL::GetBuffer(const int	nIndex) const
{
	QDT_ASSERT(nIndex < GetCapacity());
	char *	pBuffer = static_cast<char *>(_pBuffer);
	pBuffer += nIndex * GetSize();
	return	(pBuffer);
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	02-01-08:	ONA - Created
//-----------------------------------------------------------------------------
bool	MEM_POOL::IsInside(void *	pBuffer) const
{
	if	(pBuffer < _pBuffer)
	{
		return	(false);
	}
	char * pEndBuffer = static_cast<char *>(_pBuffer);
	pEndBuffer += GetCapacity() * GetSize();
	if	(pBuffer >= pEndBuffer)
	{
		return	(false);
	}
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		GetMemoryUsed
//	Object:		
//	04-04-21:	VMA - Created
//-----------------------------------------------------------------------------
int	MEM_POOL::GetMemoryUsed()
{
	return (_nMemoryUsed);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
