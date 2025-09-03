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
//	CLASS:	OS
//
//	06-03-22:	VHA - Created
//*****************************************************************************

#include "Root.h"
#include	INCL_KCORE(OS/OS)

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Tools/QdtString)

#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

long		OS::_nWinHandle			= 0;
long		OS::_nAppHandle			= 0;
bool		OS::_bLogOutput			= false ; 
bool		OS::_bHandleExceptions	= true;
bool		OS::_bDisplayLoadingMsg = true;
bool		OS::_bNoMessageBox		= false;

void *	OS::_pEBP = NULL;
void *	OS::_pEIP = NULL;

//-----------------------------------------------------------------------------
//	Name:		SetEBP
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::SetEBP(void *	pEBP)
{
	_pEBP = pEBP;
}

//-----------------------------------------------------------------------------
//	Name:		SetEBP
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::SetEIP(void *	pEIP)
{
	_pEIP = pEIP;
}

//-----------------------------------------------------------------------------
//	Name:		memcmp
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	OS::memcmp( const void * const	pBuffer,
				    unsigned char const	cC,
				    unsigned int const	nSize )
{
	const unsigned char	*pP = static_cast< const unsigned char* >( pBuffer );
	unsigned int	ThisSize = nSize;

	while ( ThisSize-- )
	{
		if	( *pP++ != cC )
		{
			return	( false );
		}
	}

	return	( true );
}

//-----------------------------------------------------------------------------
//	Name:		Convert0_8
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	OS::Convert0_8(double		d,
					   signed char& c)
{
	int	l = ( int )( d * 127.0 );

	if	(l > QDT_INT8_MAX)
	{
		c = QDT_INT8_MAX;
		return	( false );
	}
	else if	(l < QDT_INT8_MIN)
	{
		c = QDT_INT8_MIN;
		return	( false );
	}

	c = ( signed char )l;
	return	( true );
}

//-----------------------------------------------------------------------------
//	Name:		Convert0_8
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
signed char	OS::Convert0_8( double	d )
{
	signed char	cValue;

	if	( Convert0_8( d , cValue ) == false )
	{
		QDT_FAIL();
	}

	return	( cValue );
}

//-----------------------------------------------------------------------------
//	Name:		Convert0_8
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
bool	OS::Convert8_8( double	d,
					    short	&s )
{
	int	l = ( int )( d * 128.0 );

	if	(l > QDT_INT16_MAX)
	{
		s = QDT_INT16_MAX;
		return	( false );
	}
	else if	(l < QDT_INT16_MIN)
	{
		s = QDT_INT16_MIN;
		return	( false );
	}

	s = ( short )l;
	return	( true );
}

//-----------------------------------------------------------------------------
//	Name:		Convert0_8
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
short	OS::Convert8_8( double	d )
{
	short	nValue;

	if	( Convert8_8( d , nValue ) == false )
	{
		QDT_FAIL();
	}

	return	( nValue );
}

//-----------------------------------------------------------------------------
//	Name:		Convert0_8
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::Dump(IO_STREAM &		Out,
				 const void * const	pBuffer,
				 unsigned int		nSize)
{
#ifndef _MASTER
	const unsigned char	*pC;
	unsigned int		i;
	char				pcBuffer[256];
	char				c;

	//	Affiche les codes hexadécimaux de la chaîne
	pC = static_cast< unsigned char* >( const_cast< void* >( pBuffer ) );
	for ( i = 0 ; i < nSize ; ++i )
	{
		sprintf( pcBuffer , "%02X", *pC++ );
		Out << pcBuffer << " ";
	}

	//	Affiche le contenu du buffer en format ASCII

	Out << "\"";
	pC = static_cast< unsigned char* >( const_cast< void* >( pBuffer ) );
	for( i = 0 ; i < nSize ; ++i )
	{
		c = *pC++;
		switch(c)
		{
		case	0:
		case	10:
		case	13:
		case	'	':
			//	Handle special case for the text format...
			c = ' ';
			break;
		default:
			break;
		}
		Out << c;
	}

	Out << "\"";
#endif
}

//-----------------------------------------------------------------------------
// Name: GetIndex
// Object: 
//00-12-20 ONA Created: 
//-----------------------------------------------------------------------------
unsigned int	OS::GetStrongBit(const unsigned int	nSize)
{
	unsigned int	nIndex = 31;
	unsigned int	nShiftedSize = nSize;

	if	(nSize == 0)
	{
		return	(0);
	}

	if	(nSize & 0xffff0000)
	{
		if	(nSize & 0xff000000)
		{
			if	(nSize & 0xf0000000)
			{
				nIndex = 28;	//	Bit 28 à 31
				nShiftedSize >>= 28;
			}
			else
			{
				nIndex = 24;	//	Bit 24 à 27
				nShiftedSize >>= 24;
			}
		}
		else
		{
			if	(nSize & 0x00f00000)
			{
				nIndex = 20;	//	Bit 20 à 23
				nShiftedSize >>= 20;
			}
			else
			{
				nIndex = 16;	//	Bit 16 à 19
				nShiftedSize >>= 16;
			}
		}
	}
	else
	{
		if	(nSize & 0x0000ff00)
		{
			if	(nSize & 0x0000f000)
			{
				nIndex = 12;	//	Bit 12 à 15
				nShiftedSize >>= 12;
			}
			else
			{
				nIndex = 8;	//	Bit 8 à 11
				nShiftedSize >>= 8;
			}
		}
		else
		{
			if	(nSize & 0x000000f0)
			{
				nIndex = 4;		//	Bit 4 à 7
				nShiftedSize >>= 4;
			}
			else
			{
				nIndex = 0;		//	Bit 0 à 3
			}
		}
	}

	switch(nShiftedSize)
	{
	default:
	case	0:
		QDT_FAIL();
	case	1:
		break;
	case	2:
	case	3:
		nIndex += 1;
		break;
	case	4:
	case	5:
	case	6:
	case	7:
		nIndex += 2;
		break;
	case	8:
	case	9:
	case	10:
	case	11:
	case	12:
	case	13:
	case	14:
	case	15:
		nIndex += 3;
		break;
	}

	return	(nIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrBits
//	Object:		
//	01-06-29:	ONA - Created
//-----------------------------------------------------------------------------
unsigned int	OS::GetNbrBits(const unsigned int	nSize)
{
	unsigned int	nIndex;
	unsigned int	nNbrBits = 0;

	for(nIndex = 0 ; nIndex < 32 ; ++nIndex)
	{
		if	(nSize & (1 << nIndex))
		{
			++nNbrBits;
		}
	}
	return	(nNbrBits);
}

//-----------------------------------------------------------------------------
//	Name:		ConvertFuncName
//	Object:		
//	01-07-09:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::ConvertFuncName(char * const		pcBufferFuncName,
							const char * const	pcMapFuncName,
							const int			nSizeBuffer)
{
	char			sFuncName[256] = "";
	char			sTempName[256] = "";
	char			sReturn[256] = "";
	char			sParameter[256] = "";
	char			sModifierBefore[256] = "";
	char			sModifierAfter[256] = "";
	char			sProtection[128] = "";
	const char *	pAt;
	char *			pAt2;
	char			c;
	bool			bLastParameter;
	bool			bFlagSpecial = false;
	bool			bFlagConstructor = false;

	int				nCounter = 0;
	char *			psNameSpace[32];

	bool			bFlagTemplate = false;

	strcpy(pcBufferFuncName, "");

	//	First cut the string to find the correct name

	//	Find the "@@" sequence...

	pAt = strstr(pcMapFuncName, "@@");
	if	(pAt == NULL)
	{
		//	Don't found the correct pattern
		if	(strcmp(pcMapFuncName, "is"))
		{
			strncpy(pcBufferFuncName, pcMapFuncName, nSizeBuffer);
		}
		else
		{
			strncpy(pcBufferFuncName, "unknow()", nSizeBuffer);
		}
		return;
	}

	strncpy(sTempName, pcMapFuncName, pAt - pcMapFuncName);

	//	Transform "Scan@VIEWER@VIEW@QDT" en "QDT::VIEW::VIEWER::Scan"
	for(;;)
	{
		pAt2 = strrchr(sTempName, '@');

		if	(strlen(sFuncName))
		{
			strncat(sFuncName, "::", sizeof(sTempName));
		}

		if	(pAt2 == NULL)
		{
			//	This is the last... FuncName()
			char *	pFlagCopy = sTempName;

			if	(strncmp(sTempName, "??_", strlen("??_")) == 0)
			{
				pFlagCopy = sTempName + strlen("??_x");
				bFlagSpecial = true;
			}
			else if	(strncmp(sTempName, "??", strlen("??")) == 0)
			{
				pFlagCopy = sTempName + strlen("??x");
				bFlagSpecial = true;
			}
			else if	(strncmp(sTempName, "?", strlen("?")) == 0)
			{
				pFlagCopy = sTempName + strlen("?");
			}

			strncat(sFuncName, pFlagCopy, sizeof(sFuncName));

			//	Record the position...
			psNameSpace[nCounter++] = pFlagCopy;

			if	(bFlagSpecial)
			{
				strncat(sFuncName, "::", sizeof(sFuncName));

				if	(strncmp(sTempName, "??0", strlen("??0")) == 0)
				{
					strncat(sFuncName, pFlagCopy, sizeof(sFuncName));
					bFlagConstructor = true;
				}
				else if	(strncmp(sTempName, "??1", strlen("??1")) == 0)
				{
					strncat(sFuncName, "~", sizeof(sFuncName));
					strncat(sFuncName, pFlagCopy, sizeof(sFuncName));
					bFlagConstructor = true;
				}
				else if	(strncmp(sTempName, "??2", strlen("??2")) == 0)
				{
					strncat(sFuncName, "operator new", sizeof(sFuncName));
				}
				else if	(strncmp(sTempName, "??3", strlen("??3")) == 0)
				{
					strncat(sFuncName, "operator delete", sizeof(sFuncName));
				}
				else if	(	(strncmp(sTempName, "??_E", strlen("??_E")) == 0)
						||	(strncmp(sTempName, "??_G", strlen("??_G")) == 0)	)
				{
					strncat(sFuncName, "'scalar deleting destructor'", sizeof(sFuncName));
				}
				else
				{
					strncat(sFuncName, "unknow", sizeof(sFuncName));
				}
			}

			break;
		}
		else
		{
			//	Record the position...
			psNameSpace[nCounter++] = pAt2 + 1;
		}

		strncat(sFuncName, pAt2 + 1, sizeof(sFuncName));

		if	(strncmp(pAt2 + 1, "$", strlen("$")))
		{
			bFlagTemplate = true;
		}

		//	Crop the list
		*pAt2 = '\0';
	}

	//	sFuncName contains the correct name

	//	Now get the return value

	pAt += 2;		//	After the "@@" sequence

	c = *pAt++;

	switch(c)
	{
	case	'S':
		//	Static method
		strcpy(sModifierBefore, "static");
		c = *pAt++;
		OS::GetProtection(sProtection, sizeof(sProtection), c);
		break;
	case	'Y':
		strcpy(sModifierBefore, "\"C\"");
		c = *pAt++;
//		QDT_ASSERT(c == 'A');
		break;
	case	'M':	//	For MessageMap()
	case	'U':
		strcpy(sModifierBefore, "virtual");
		c = *pAt++;
		OS::GetConst(sModifierAfter, sizeof(sModifierAfter), c);
		c = *pAt++;
//		QDT_ASSERT(c == 'E');
		break;
	case	'Q':
	case	'I':
	case	'A':
		OS::GetProtection(sProtection, sizeof(sProtection), c);
		c = *pAt++;
		OS::GetConst(sModifierAfter, sizeof(sModifierAfter), c);
		c = *pAt++;
//		QDT_ASSERT(c == 'E');
		break;
	}

	if	(bFlagTemplate == false)
	{
		bLastParameter = false;

		if	(bFlagConstructor == false)
		{
			//	Get return value
			pAt = OS::GetParameter(sReturn, pAt, sizeof(sReturn), bLastParameter, psNameSpace, nCounter);
			QDT_ASSERT(bLastParameter == false);
		}
		else
		{
			c = *pAt++;
	//		QDT_ASSERT(c == '@');
		}

		//	Get each parameters

		c = *pAt;
		if	(c != 'Z')
		{
			//	There is one or more parameter...
			do
			{
				char	sTemp[128] = "";
				pAt = OS::GetParameter(sTemp, pAt, sizeof(sTemp), bLastParameter, psNameSpace, nCounter);

				if	(bLastParameter == false)
				{
					if	(strlen(sParameter))
					{
						strncat(sParameter, ", ", nSizeBuffer);
					}
					strncat(sParameter, sTemp, nSizeBuffer);
				}
			}
			while	(bLastParameter == false);
		}

		c = *pAt++;
	//	QDT_ASSERT(c == 'Z');
	}

	//	Now we can recompose the entire method !

	if	(strlen(sModifierBefore))
	{
		strncpy(pcBufferFuncName, sModifierBefore, nSizeBuffer);
		strncat(pcBufferFuncName, " ", nSizeBuffer);
	}

	if	(strlen(sReturn))
	{
		strncat(pcBufferFuncName, sReturn, nSizeBuffer);
		strncat(pcBufferFuncName, " ", nSizeBuffer);
	}

	strncat(pcBufferFuncName, sFuncName, nSizeBuffer);

	strncat(pcBufferFuncName, "(", nSizeBuffer);
	strncat(pcBufferFuncName, sParameter, nSizeBuffer);
	strncat(pcBufferFuncName, ")", nSizeBuffer);

	if	(strlen(sModifierAfter))
	{
		strncat(pcBufferFuncName, " ", nSizeBuffer);
		strncat(pcBufferFuncName, sModifierAfter, nSizeBuffer);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetProtection
//	Object:		
//	01-07-09:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::GetProtection(char * const	pcBuffer,
						  const int		nSizeBuffer,
						  const char	c)
{
	switch(c)
	{
	case	'Q':
		strncpy(pcBuffer, "public", nSizeBuffer);
		break;
	case	'I':
		strncpy(pcBuffer, "protected", nSizeBuffer);
		break;
	case	'A':
		strncpy(pcBuffer, "private", nSizeBuffer);
		break;
	default:
		strncpy(pcBuffer, "unknow", nSizeBuffer);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetConst
//	Object:		
//	01-07-09:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::GetConst(char * const	pcBuffer,
					 const int		nSizeBuffer,
					 const char		c)
{
	switch(c)
	{
	case	'A':
		strncpy(pcBuffer, "", nSizeBuffer);
		break;
	case	'B':
		strncpy(pcBuffer, "const", nSizeBuffer);
		break;
	default:
		strncpy(pcBuffer, "unknow", nSizeBuffer);
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetParameter
//	Object:		
//	01-07-09:	ONA - Created
//-----------------------------------------------------------------------------
const char *	OS::GetParameter(char * const		pcBufferDst,
								 const char * const	pcBufferSrc,
								 int				nSizeBuffer,
								 bool &				bLastParameter,
								 char *				psNameSpace[32],
								 const int			nSizeTable)
{
	const char *	pP = pcBufferSrc;
	char			c;
	bool			bPointer = false;
	bool			bReference = false;
	char			sBuffer1[1024] = "";
	char			sBuffer2[1024] = "";

	static char		sOldParameter[1024];

	bLastParameter = false;

	c = *pP++;
	switch(c)
	{
	case	'P':
	case	'Q':
		bPointer = true;
		c = *pP++;
		GetConst(pcBufferDst, nSizeBuffer, c);
		if	(strlen(pcBufferDst))
		{
			strncat(pcBufferDst, " ", nSizeBuffer);
		}
		c = *pP++;
		break;
	case	'A':
		bReference = true;
		c = *pP++;
		GetConst(pcBufferDst, nSizeBuffer, c);
		if	(strlen(pcBufferDst))
		{
			strncat(pcBufferDst, " ", nSizeBuffer);
		}
		c = *pP++;
		break;
	case	'@':
		bLastParameter = true;
		return	(pP);
	case	'0':
		strncpy(pcBufferDst, sOldParameter, nSizeBuffer);
		return	(pP);
	}

	switch(c)
	{
	case	'_':
		c = *pP++;
		if	(c == 'N')
		{
			strncat(pcBufferDst, "bool", nSizeBuffer);
		}
		else
		{
			strncat(pcBufferDst, "unknow", nSizeBuffer);
		}
		break;
	case	'D':
		strncat(pcBufferDst, "char", nSizeBuffer);
		break;
	case	'I':
		strncat(pcBufferDst, "unsigned int", nSizeBuffer);
		break;
	case	'J':
		strncat(pcBufferDst, "long", nSizeBuffer);
		break;
	case	'H':
		strncat(pcBufferDst, "int", nSizeBuffer);
		break;
	case	'M':
		strncat(pcBufferDst, "unknow", nSizeBuffer);
		break;
	case	'X':
		strncat(pcBufferDst, "void", nSizeBuffer);
		break;
	case	'Z':
		if	(*pP == 'Z')
		{
			//	Special case "ZZ" equal to "..."
			strncat(pcBufferDst, "...", nSizeBuffer);
		}
		else
		{
			bLastParameter = true;
		}
		break;

	case	'?':
		pP += 1;	//	Skip the 'A'
		pP += 1;	//	Skip the 'W'

	case	'W':
		pP += 1;	//	Skip the '4'

		for(;;)
		{
			char	String[2] = "*";
			c = *pP++;
			if	(c == '@')
			{
				break;
			}
			String[0] = c;
			strncat(sBuffer2, String, sizeof(sBuffer2));
		}

	case	'V':
	case	'U':
		//	Special case : CLASS or STRUCTURE
		{
			//	We've got to copy each members with the namespace
			c = *pP;
			if	((c < '1') || (c > '9'))
			{
				//	First copy the name
				for(;;)
				{
					char	String[2] = "*";
					c = * pP++;
					if	(c == '@')
					{
						break;
					}
					String[0] = c;
					strncat(sBuffer1, String, sizeof(sBuffer2));
				}
			}

			if	(strlen(sBuffer2))
			{
				strncat(sBuffer1, "::", sizeof(sBuffer1));
				strncat(sBuffer1, sBuffer2, sizeof(sBuffer1));
			}

			strcpy(sBuffer2, sBuffer1);

			//	Then copy the namespace sequence
			for(;;)
			{
				c = *pP++;
				if	(c == '@')
				{
					//	End of namespace sequence
					break;
				}
				int	nIndex = c - '0';
				strncpy(sBuffer1, psNameSpace[nSizeTable - 1 - nIndex], sizeof(sBuffer1));
				if	(strlen(sBuffer2))
				{
					strncat(sBuffer1, "::", sizeof(sBuffer1));
					strncat(sBuffer1, sBuffer2, sizeof(sBuffer1));
				}

				strcpy(sBuffer2, sBuffer1);
			}

			strncat(pcBufferDst, sBuffer1, nSizeBuffer);
		}
		break;

	default:
		strncat(pcBufferDst, "unknow", nSizeBuffer);
		break;
	}

	if	(bPointer)
	{
		strncat(pcBufferDst, " *", nSizeBuffer);
	}
	else if	(bReference)
	{
		strncat(pcBufferDst, " &", nSizeBuffer);
	}

	strncpy(sOldParameter, pcBufferDst, sizeof(sOldParameter));

	return	(pP);
}

//-----------------------------------------------------------------------------
//	Name:		DisplayLoadingMsg
//	Object:		
//	01-11-08:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::DisplayLoadingMsg()
{
	// QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EnableDisplayLoadingMsg
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::EnableDisplayLoadingMsg(const bool bFlag)
{
	_bDisplayLoadingMsg = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		IsDisplayLoadingMsgOn
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	OS::IsDisplayLoadingMsgOn()
{
	return	(_bDisplayLoadingMsg);
}

//-----------------------------------------------------------------------------
// Name: OS::SetAppHandle
// Object: 
//00-11-13 JVA Created: 
//-----------------------------------------------------------------------------
void	OS::SetAppHandle(long h)
{
	_nAppHandle = h;
}

//-----------------------------------------------------------------------------
// Name: OS::GetAppHandle
// Object: 
//00-11-13 JVA Created: 
//-----------------------------------------------------------------------------
long	OS::GetAppHandle()
{
	return	(_nAppHandle);
}

//-----------------------------------------------------------------------------
// Name: OS::SetWinHandle
// Object: 
//00-11-13 JVA Created: 
//-----------------------------------------------------------------------------
void	OS::SetWinHandle(long h)
{
	_nWinHandle = h;
}

//-----------------------------------------------------------------------------
// Name: OS::GetWinHandle
// Object: 
//00-11-13 JVA Created: 
//-----------------------------------------------------------------------------
long	OS::GetWinHandle()
{
	return	(_nWinHandle);
}

//-----------------------------------------------------------------------------
// Name: OS::GetDispHandle
// Object: 
//00-11-13 JVA Created: 
//-----------------------------------------------------------------------------
long	OS::GetDispHandle()
{
	QDT_FAIL();
	return	(0);
}

//-----------------------------------------------------------------------------
//	Name:		EnableLogOutput
//	Object:		
//	02-05-17:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::EnableLogOutput(const bool bFlag)
{
	_bLogOutput = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		IsLogOutputEnabled
//	Object:		
//	02-05-17:	DAC - Created
//-----------------------------------------------------------------------------
bool	OS::IsLogOutputEnabled()
{
	return	(_bLogOutput);
}

//-----------------------------------------------------------------------------
//	Name:		SetHandleExceptions
//	Object:		
//	02-06-27:	VPI - Created
//-----------------------------------------------------------------------------
bool	OS::SetHandleExceptions(const bool	bFlag)
{
	bool	bOldValue;
	bOldValue = _bHandleExceptions;
	_bHandleExceptions = bFlag;
	return	(bOldValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetHandleExceptions
//	Object:		
//	02-06-27:	VPI - Created
//-----------------------------------------------------------------------------
bool	OS::GetHandleExceptions()
{
	return	(_bHandleExceptions);
}

//-----------------------------------------------------------------------------
//	Name:		EnableNoMessageBox
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::EnableNoMessageBox(const bool	bFlag)
{
	_bNoMessageBox = bFlag;
}

//-----------------------------------------------------------------------------
//	Name:		IsEnableNoMessageBoxOn
//	Object:		
//	04-02-04:	DAC - Created
//-----------------------------------------------------------------------------
bool	OS::IsEnableNoMessageBoxOn()
{
	return	(_bNoMessageBox);
}

//-----------------------------------------------------------------------------
//	Name:		GetBrowserLanguage
//	Object:		
//	04-03-08:	SBE - Created
//-----------------------------------------------------------------------------
unsigned int	OS::GetBrowserLanguage()
{
	return 1;
}

//-----------------------------------------------------------------------------
//	Name:		GetHumanReadableSize
//	Object:		
//	06-03-23:	VHA - Created
//-----------------------------------------------------------------------------
QDT_STRING	OS::GetHumanReadableSize(UInt32	size)
{
	QDT_STRING newString;

	if(size<1024)
	{
		newString.Format("%d bytes", size); // bytes
	}
	else
	{
		if(size<1024*1024)
		{
			newString.Format("%.2f Kb", (float)size/1024); // Kb
		}
		else
		{
			newString.Format("%.2f Mb", ((float)size/1024)/1024); // Mb
		}
	}

	return newString;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
