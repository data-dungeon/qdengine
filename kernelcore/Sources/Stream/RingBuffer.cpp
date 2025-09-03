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
//	CLASS:	RING_BUFFER
//
//	01-08-02:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Stream/RingBuffer)

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
//-----------------------------------------------------------------------------
//	Name:		RING_BUFFER constructor
//	Object:		
//	01-08-07:	JLT - Created
//-----------------------------------------------------------------------------
RING_BUFFER::RING_BUFFER(unsigned long Size)
:MEM_STREAM(NULL, Size)
,_nWriteOffset(0)
{

}

//-----------------------------------------------------------------------------
//	Name:		~RING_BUFFER destructor
//	Object:		
//	01-08-07:	JLT - Created
//-----------------------------------------------------------------------------
RING_BUFFER::~RING_BUFFER()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		SetChars
//	Object:		
//	01-08-08:	JLT - Created
//-----------------------------------------------------------------------------
void RING_BUFFER::SetChars(const char *pC, int Size)
{
	if (_nWriteOffset < _nOffset)
	{
		QDT_ASSERT(Size <= (Int32)_nOffset - (Int32)_nWriteOffset);
		QDT_MEMCOPY(_pcBuffer+_nWriteOffset, pC, Size);
		_nWriteOffset += Size;
	}
	else
	{
		int delta = _nBufferSize - _nWriteOffset;
		QDT_ASSERT(Size < (Int32)(delta + _nOffset));

		if (Size < delta)
		{
			QDT_MEMCOPY(_pcBuffer+_nWriteOffset, pC, Size);
			_nWriteOffset += Size;
		}
		else
		{
			QDT_MEMCOPY(_pcBuffer+_nWriteOffset, pC, delta);
			QDT_MEMCOPY(_pcBuffer, pC+delta, Size - delta);
			_nWriteOffset = Size-delta;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetChars
//	Object:		
//	01-08-07:	JLT - Created
//-----------------------------------------------------------------------------
void RING_BUFFER::GetChars(char *pC, int Size)
{
	QDT_ASSERT(_nOffset >= 0);
	QDT_ASSERT(_nOffset < _nSize);
	while	(Size--)
	{
		*pC++ = *(_pcBuffer + _nOffset);
		if (_nOffset == (_nSize-1))
			_nOffset = 0;
		else
			++_nOffset;
	}
	QDT_ASSERT(_nOffset >= 0);
	QDT_ASSERT(_nOffset < _nSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetBuffer
//	Object:		
//	01-09-10:	JLT - Created
//-----------------------------------------------------------------------------
void RING_BUFFER::GetWriteBuffer(int	Byte2Write, 
								 char** pBuffer1, 
								 int *	pSize1, 
								 char** pBuffer2, 
								 int *	pSize2)
{
	if (!_pcBuffer)
		return;
	
	if (_nOffset <= _nWriteOffset)
	{
		int DeltaEnd = _nBufferSize - _nWriteOffset;
		if ((Int32)(DeltaEnd + _nOffset) < Byte2Write)
		{
			*pBuffer1 = *pBuffer2 = NULL;
			*pSize1 = *pSize2 = 0;
		}
		else
		{
			if (Byte2Write <= DeltaEnd)
			{
				*pBuffer1 = _pcBuffer + _nWriteOffset;
				*pSize1 = Byte2Write;
				*pBuffer2 = NULL;
				*pSize2 = 0;
				_nWriteOffset += Byte2Write;
				if (_nWriteOffset == _nBufferSize)
					_nWriteOffset = 0;
			}
			else
			{
				*pBuffer1 = _pcBuffer + _nWriteOffset;
				*pSize1 = DeltaEnd;
				*pBuffer2 = _pcBuffer;
				*pSize2 = Byte2Write - DeltaEnd;
				_nWriteOffset = *pSize2;
			}
		}
	}
	else
	{
		int Delta = _nOffset - _nWriteOffset;
		if (Delta < Byte2Write)
		{
			*pBuffer1 = *pBuffer2 = NULL;
			*pSize1 = *pSize2 = 0;
		}
		else
		{
			*pBuffer1 = _pcBuffer + _nWriteOffset;
			*pSize1 = Byte2Write;
			*pBuffer2 = NULL;
			*pSize2 = 0;
			_nWriteOffset += Byte2Write;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		CanRead
//	Object:		
//	01-09-11:	JLT - Created
//-----------------------------------------------------------------------------
bool RING_BUFFER::CanRead(int Byte2Read)
{
	if (!_pcBuffer)
		return false;

	if (_nOffset <= _nWriteOffset)
		return (((Int32)_nWriteOffset - (Int32)_nOffset) >= Byte2Read);
	else
		return (((Int32)_nBufferSize - (Int32)_nOffset + (Int32)_nWriteOffset) >= Byte2Read);
}


//-----------------------------------------------------------------------------
//	Name:		reserve memory
//	Object:		
//	01-08-07:	JLT - Created
//-----------------------------------------------------------------------------
void RING_BUFFER::ReserveMemory(int ByteCount)
{
	QDT_ASSERT(ByteCount > 0);

	if (ByteCount <= 0)
		return;

	delete [] _pcBuffer; _pcBuffer = NULL;

	_nSize = _nBufferSize = ByteCount;
	_pcBuffer = new char[ByteCount];
}

//-----------------------------------------------------------------------------
//	Name:		GetFillinginPercent
//	Object:		
//	01-09-25:	JLT - Created
//-----------------------------------------------------------------------------
float RING_BUFFER::GetFillinginPercent() const
{
	return ((float)GetAvailableSize() / (float)_nBufferSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetAvailableSize
//	Object:		
//	02-02-06:	JLT - Created
//-----------------------------------------------------------------------------
int	RING_BUFFER::GetAvailableSize() const
{
	if (_nOffset <= _nWriteOffset)
	{
		return (_nWriteOffset - _nOffset);
	}
	else
	{
		return (_nBufferSize - _nOffset + _nWriteOffset);
	}
}

IO_STREAM& RING_BUFFER::operator + (const unsigned int s)
{
	QDT_ASSERT(_nOffset >= 0);
	QDT_ASSERT(_nOffset < _nSize);
	
	if (CanRead(s))
	{
		if (s + _nOffset > _nSize)
		{
			_nOffset = s + _nOffset - _nSize;
		}
		else
		{
			_nOffset += s;
		}
	}
	QDT_ASSERT(_nOffset >= 0);
	QDT_ASSERT(_nOffset < _nSize);
	return *this;	
}

IO_STREAM&
RING_BUFFER::operator - (const unsigned int s)
{
	QDT_NOT_IMPLEMENTED();
	return *this;	
}

//-----------------------------------------------------------------------------
//	Name:		CanWrite
//	Object:		
//	01-10-01:	JLT - Created
//-----------------------------------------------------------------------------
bool RING_BUFFER::CanWrite(int Byte2Write)
{
	int offset = _nWriteOffset;

	char* pBuffer1;
	char* pBuffer2; 
	int Size1; 
	int Size2;
	
	GetWriteBuffer(Byte2Write, &pBuffer1, &Size1, &pBuffer2, &Size2);
	
	_nWriteOffset = offset;

	return (Byte2Write == Size1 + Size2);
}


//-----------------------------------------------------------------------------
//	Name:		SetReadOffset
//	Object:		
//	02-01-04:	DAC - Created
//-----------------------------------------------------------------------------
void	RING_BUFFER::SetReadOffset(unsigned int	offset)
{
	QDT_ASSERT(offset < static_cast<unsigned int>(_nSize)); 
	_nOffset = offset;
}
	
//-----------------------------------------------------------------------------
//	Name:		SetWriteOffset
//	Object:		
//	02-01-04:	DAC - Created
//-----------------------------------------------------------------------------
void	RING_BUFFER::SetWriteOffset(unsigned int offset)
{
	QDT_ASSERT(offset < static_cast<unsigned int>(_nSize)); 
	_nWriteOffset = offset;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
