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
//	CLASS:	FILE_ABC
//
//	02-02-14:	AMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(File/FileABC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

char	FILE_ABC::s_ReadBuffer[ 2 * FILE_ABC::BUFFER_SIZE ];

//-----------------------------------------------------------------------------
//	Name:		FILE_ABC constructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_ABC::FILE_ABC()
:
_nSize(-1),
_nPosition(0),
_nLastError(0)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		FILE_ABC destructor
//	Object:		
//	02-02-14:	AMA - Created
//-----------------------------------------------------------------------------
FILE_ABC::~FILE_ABC()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		SetSize
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_ABC::SetSize(const int	nSize)
{
	_nSize = nSize;
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
int	FILE_ABC::GetSize() const
{
	return (_nSize);
}

//-----------------------------------------------------------------------------
//	Name:		SetPosition
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_ABC::SetPosition(const unsigned int	nPosition)
{
	_nPosition = nPosition;
}

//-----------------------------------------------------------------------------
//	Name:		GetPosition
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
unsigned int	FILE_ABC::GetCurrentPosition() const
{
	return (_nPosition);
}

//-----------------------------------------------------------------------------
//	Name:		TestStatus
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
bool	FILE_ABC::TestStatus(int	nMask) const
{
	return	( _fStatus.IsSet(nMask) );
}

//-----------------------------------------------------------------------------
//	Name:		EnableStatus
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_ABC::EnableStatus(int	nStatus)
{
	_fStatus.Set(nStatus);
}

//-----------------------------------------------------------------------------
//	Name:		DisableStatus
//	Object:		
//	02-02-18:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_ABC::DisableStatus(int	nStatus)
{
	_fStatus.Unset(nStatus);
}

//-----------------------------------------------------------------------------
//	Name:		GetModuloPositionAndSize
//	Object:		
//	03-01-30:	AMA - Created
//-----------------------------------------------------------------------------
void	FILE_ABC::GetModuloPositionAndSize(UInt32		nPosition,
										   UInt32		nSize,
										   UInt32 &	nModuloPosition,
										   UInt32 &	nModuloSize)
{
	int	nSectorBeginIndex;
	int	nSectorEndIndex;
	int nSectorSize = GetSectorSize();

	if ( nPosition == 0 && nSize == 0 )
	{
		nModuloPosition = 0;
		nModuloSize		= 0;
	}
	else
	{
		// Calcul des Index des secteurs à lire
		nSectorBeginIndex	= nPosition / nSectorSize ;
		nSectorEndIndex		= ( nPosition + nSize - 1 ) / nSectorSize;
		
		// Calcul de la Position( modulo 2048 ) et de la Taille( modulo 2048 )
		nModuloPosition	= nSectorBeginIndex * nSectorSize;
		nModuloSize	= ( ( nSectorEndIndex - nSectorBeginIndex ) + 1 ) * nSectorSize;
	}
}

#ifdef PLATFORM_PC

//-----------------------------------------------------------------------------
//	Name:		ReadAndUncompress
//	Object:		
//	03-12-23:	VMA - Created
//-----------------------------------------------------------------------------
bool	FILE_ABC::ReadAndUncompress(void *				pBuffer,
									const unsigned int	nPosition,
									const int			nCompressedSize,
									const int			nUncompressedSize,
									const F_SYNC		eSync)
{
/*	int nbParts		 =  nCompressedSize / BUFFER_SIZE + 1;
	int nReadPos	 =  nPosition;
	int nIdxPart;
	int nReadBuffer  = 1;
	int nDecBuffer   = 0;

	z_stream	stream;

	stream.zalloc = z_alloc_func;
	stream.zfree  = z_free_func;

	// Read 1st part
	Read(s_ReadBuffer, nReadPos, BUFFER_SIZE, FS_NO_WAIT);

	inflateInit(&stream);

	stream.next_out = (Bytef*) pBuffer + 16;
	stream.avail_out = nUncompressedSize;

	for( nIdxPart = 1 ; nIdxPart < nbParts + 1; nIdxPart++ )
	{
		nReadPos += BUFFER_SIZE;
		while( !IsIdle() );

		if ( nIdxPart != nbParts )
		{
			Read(s_ReadBuffer + nReadBuffer * BUFFER_SIZE, nReadPos, BUFFER_SIZE, FS_NO_WAIT);
		}

		if ( nIdxPart == 1 )
		{
			stream.avail_in = BUFFER_SIZE - 16;
			stream.next_in = (Bytef*) s_ReadBuffer + 16;
		}
		else
		{
			stream.avail_in = BUFFER_SIZE;
			stream.next_in = (Bytef*) s_ReadBuffer + nDecBuffer * BUFFER_SIZE;
		}

		inflate( &stream, Z_SYNC_FLUSH);

		nDecBuffer = nReadBuffer;
		nReadBuffer = 1 - nReadBuffer;
	}

	inflateEnd(&stream);*/

	QDT_FAIL();

	return (true);
}

#endif

//-----------------------------------------------------------------------------
//	Name:		GetLastError
//	Object:		
//	05-01-05:	VMA - Created
//-----------------------------------------------------------------------------
int	FILE_ABC::GetLastError() const
{
	return (_nLastError);
}
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
