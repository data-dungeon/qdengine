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
//	CLASS:	QDT_CRC_32
//
//	02-12-09:	DAC - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(File/QDT_Crc32)
#include	INCL_KCORE(Stream/filestream)

#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QDT_CRC_32 constructor
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
QDT_CRC_32::QDT_CRC_32()
:
_pCrc32Table(NULL)
{

}

//-----------------------------------------------------------------------------
//	Name:		QDT_CRC_32 constructor
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
QDT_CRC_32::QDT_CRC_32(const QDT_CRC_32 & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		QDT_CRC_32 destructor
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
QDT_CRC_32::~QDT_CRC_32()
{
	Free();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
QDT_CRC_32 &	QDT_CRC_32::operator=(const QDT_CRC_32 & C)
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
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CRC_32::Copy(const QDT_CRC_32 & C)
{
	QDT_NOT_IMPLEMENTED();
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CRC_32::Init()
{
	// This is the official polynomial used by CRC32 in PKZip.
	// Often times the polynomial shown reversed as 0x04C11DB7.
	unsigned int nPolynomial;
	int			 i, j;
	unsigned int nCrc;

	nPolynomial = 0xEDB88320;

	Free();

	_pCrc32Table = new unsigned int[256];

	for	(i = 0; i < 256; i++)
	{
		nCrc = i;
		for	(j = 8; j > 0; j--)
		{
			if(nCrc & 1)
			{
				nCrc = (nCrc >> 1) ^ nPolynomial;
			}
			else
			{
				nCrc >>= 1;
			}
		}
		_pCrc32Table[i] = nCrc;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Free
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CRC_32::Free()
{
	if(_pCrc32Table)
	{
		delete []_pCrc32Table;
		_pCrc32Table = NULL;
	}
}


//-----------------------------------------------------------------------------
//	Name:		CalcCrc32
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
void	QDT_CRC_32::CalcCrc32(const unsigned char	cByte,
							  unsigned int &		nDwCrc32) const
{
	nDwCrc32 = ((nDwCrc32) >> 8) ^ _pCrc32Table[(cByte) ^ ((nDwCrc32) & 0x000000FF)];
}

//-----------------------------------------------------------------------------
//	Name:		StringCrc32
//	Object:		
//	04-12-20:	JPE - Created
//-----------------------------------------------------------------------------
QDT_CRC_32::CRC_ERRORS	QDT_CRC_32::BufferCrc32(const void * pBuffer, unsigned int nSize, unsigned int &nCrc32)
{
	QDT_ASSERT(pBuffer);

	CRC_ERRORS	eErrorCode;
	char *		pTmpStr;

	nCrc32	= 0xFFFFFFFF;
	eErrorCode	= CE_NO_ERROR;
	pTmpStr		= (char*)(pBuffer);
	
	// Is the table initialized?
	if	(_pCrc32Table == NULL)
	{
		Init();
	}

	while	(nSize)
	{
		CalcCrc32((unsigned char)*pTmpStr, nCrc32);
		pTmpStr++;
		nSize--;
	}
	
	nCrc32 = ~nCrc32;

	return	(eErrorCode);
}

//-----------------------------------------------------------------------------
//	Name:		StringCrc32
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
QDT_CRC_32::CRC_ERRORS	QDT_CRC_32::StringCrc32(const char *	pString,
												unsigned int &	nCrc32)
{
	QDT_ASSERT(pString);

	CRC_ERRORS	eErrorCode;
	char *		pTmpStr;

	nCrc32	= 0xFFFFFFFF;
	eErrorCode	= CE_NO_ERROR;
	pTmpStr		= const_cast<char*>(pString);

	
	// Is the table initialized?
	if	(_pCrc32Table == NULL)
	{
		Init();
	}

	while	(*pTmpStr != '\0')
	{
		CalcCrc32((unsigned char)*pTmpStr, nCrc32);
		pTmpStr++;
	}
	
	nCrc32 = ~nCrc32;

	return	(eErrorCode);
}


//-----------------------------------------------------------------------------
//	Name:		FileCrc32
//	Object:		
//	02-12-09:	DAC - Created
//-----------------------------------------------------------------------------
QDT_CRC_32::CRC_ERRORS	QDT_CRC_32::FileCrc32(const char *		pFileName,
											  unsigned int &	nCrc32)

{
	QDT_ASSERT(pFileName);
	QDT_ASSERT(strlen(pFileName));

	CRC_ERRORS		eErrorCode;
	bool			bOpen;
	FILE_STREAM		File;
	char			cBuffer[MAX_BUFFER_SIZE];
	int				nLoop, nCount;

	
	nCrc32		= 0xFFFFFFFF;
	eErrorCode	= CE_NO_ERROR;

	
	// Is the table initialized?
	if	(_pCrc32Table == NULL)
	{
		Init();
	}

	// Open the file
	bOpen = File.Open(pFileName, FILE_STREAM::AT_READ_ONLY);
	
	if	(bOpen == false)
	{
		eErrorCode = CE_ERROR_OPEN_FILE;
	}
	else
	{
		nCount = File.GetBuffer(cBuffer, sizeof(cBuffer));
		
		while	(nCount)
		{
			for	(nLoop = 0; nLoop < nCount; nLoop++)
			{
				CalcCrc32(cBuffer[nLoop], nCrc32);
			}
			
			nCount = File.GetBuffer(cBuffer, sizeof(cBuffer));
		}

		File.Close();
	}
	
	
	if	(bOpen)
	{
		File.Close();
	}

	nCrc32 = ~nCrc32;

	return (eErrorCode);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
