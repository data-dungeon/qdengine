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
//	CLASS:	ENDIAN
//
//	05-11-22:	VMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Core/Endian)

#ifdef _DEBUG
#	include	"Endian.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

ENDIAN *	ENDIAN::_pInstance = NULL;

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Int8	ENDIAN::Convert(Int8	nVal)
{
	return (nVal);
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Int16	ENDIAN::Convert(Int16	nVal)
{
	if ( _bSwap )
	{	
		Int16	nRes;
		UInt8 *  pVal = (UInt8 *) &nVal;
		UInt8 *  pRes = (UInt8 *) &nRes;

		pRes[0] = pVal[1];
		pRes[1] = pVal[0];

		return (nRes);
	}
	else
	{
		return (nVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Int32	ENDIAN::Convert(Int32	nVal)
{
	if ( _bSwap )
	{
		Int32	nRes;
		UInt8 *  pVal = (UInt8 *) &nVal;
		UInt8 *  pRes = (UInt8 *) &nRes;

		pRes[0] = pVal[3];
		pRes[1] = pVal[2];
		pRes[2] = pVal[1];
		pRes[3] = pVal[0];

		return (nRes);
	}
	else
	{
		return (nVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Int64	ENDIAN::Convert(Int64	nVal)
{
	if ( _bSwap )
	{
		Int64	nRes;
		UInt8 *  pVal = (UInt8 *) &nVal;
		UInt8 *  pRes = (UInt8 *) &nRes;

		pRes[0] = pVal[7];
		pRes[1] = pVal[6];
		pRes[2] = pVal[5];
		pRes[3] = pVal[4];
		pRes[4] = pVal[3];
		pRes[5] = pVal[2];
		pRes[6] = pVal[1];
		pRes[7] = pVal[0];

		return (nRes);
	}
	else
	{
		return (nVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
UInt8	ENDIAN::Convert(UInt8	nVal)
{
	return (nVal);
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
UInt16	ENDIAN::Convert(UInt16	nVal)
{
	if ( _bSwap )
	{
		UInt16	nRes;
		UInt8 *  pVal = (UInt8 *) &nVal;
		UInt8 *  pRes = (UInt8 *) &nRes;

		pRes[0] = pVal[1];
		pRes[1] = pVal[0];

		return (nRes);
	}
	else
	{
		return (nVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
UInt32	ENDIAN::Convert(UInt32	nVal)
{
	if ( _bSwap )
	{
		Int32	nRes;
		UInt8 *  pVal = (UInt8 *) &nVal;
		UInt8 *  pRes = (UInt8 *) &nRes;

		pRes[0] = pVal[3];
		pRes[1] = pVal[2];
		pRes[2] = pVal[1];
		pRes[3] = pVal[0];

		return (nRes);
	}
	else
	{
		return (nVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
UInt64	ENDIAN::Convert(UInt64	nVal)
{
	if ( _bSwap )
	{
		UInt64	nRes;
		UInt8 *  pVal = (UInt8 *) &nVal;
		UInt8 *  pRes = (UInt8 *) &nRes;

		pRes[0] = pVal[7];
		pRes[1] = pVal[6];
		pRes[2] = pVal[5];
		pRes[3] = pVal[4];
		pRes[4] = pVal[3];
		pRes[5] = pVal[2];
		pRes[6] = pVal[1];
		pRes[7] = pVal[0];

		return (nRes);
	}
	else
	{
		return (nVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Float32	ENDIAN::Convert(Float32	fVal)
{
	if ( _bSwap )
	{
		Float32	fRes;
		UInt8 *  pVal = (UInt8 *) &fVal;
		UInt8 *  pRes = (UInt8 *) &fRes;

		pRes[0] = pVal[3];
		pRes[1] = pVal[2];
		pRes[2] = pVal[1];
		pRes[3] = pVal[0];

		return (fRes);
	}
	else
	{
		return (fVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Convert
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
Float64	ENDIAN::Convert(Float64	fVal)
{
	if ( _bSwap )
	{
		UInt64	fRes;
		UInt8 *  pVal = (UInt8 *) &fVal;
		UInt8 *  pRes = (UInt8 *) &fRes;

		pRes[0] = pVal[7];
		pRes[1] = pVal[6];
		pRes[2] = pVal[5];
		pRes[3] = pVal[4];
		pRes[4] = pVal[3];
		pRes[5] = pVal[2];
		pRes[6] = pVal[1];
		pRes[7] = pVal[0];

		return (*(Float64*)&fRes);
	}
	else
	{
		return (fVal);
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	Int8 *	pBuffer,
								UInt32	nSize)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	Int16 *	pBuffer,
								UInt32	nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(Int16);

	QDT_ASSERT( nSize % sizeof(Int16) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	Int32 *	pBuffer,
								UInt32	nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(Int32);

	QDT_ASSERT( nSize % sizeof(Int32) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	Int64 *	pBuffer,
								UInt32	nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(Int64);

	QDT_ASSERT( nSize % sizeof(Int64) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	UInt8 *	pBuffer,
								UInt32	nSize)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	UInt16 *	pBuffer,
								UInt32		nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(UInt16);

	QDT_ASSERT( nSize % sizeof(UInt16) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	UInt32 *	pBuffer,
								UInt32		nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(UInt32);

	QDT_ASSERT( nSize % sizeof(UInt32) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	UInt64 *	pBuffer,
								UInt32		nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(UInt64);

	QDT_ASSERT( nSize % sizeof(UInt64) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	Float32 *	pBuffer,
								UInt32		nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(Float32);

	QDT_ASSERT( nSize % sizeof(Float32) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ConvertBuffer
//	Object:		
//	05-11-22:	VMA - Created
//-----------------------------------------------------------------------------
void	ENDIAN::ConvertBuffer(	Float64 *	pBuffer,
								UInt32		nSize)
{
	if ( _bSwap == false ) return;

	UInt32 nIdx;
	UInt32 nCount = nSize / sizeof(Float64);

	QDT_ASSERT( nSize % sizeof(Float64) == 0 );

	for( nIdx = 0 ; nIdx < nCount ; nSize++ )
	{
		*pBuffer = Convert(*pBuffer);
		pBuffer++;
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
