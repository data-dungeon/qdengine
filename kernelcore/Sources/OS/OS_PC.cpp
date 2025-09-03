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
// CLASS: OS
//
//
//00-11-21 ONA Created: 
//*****************************************************************************

#include "Root.h"
#include "OS.h"

#include	INCL_KCORE(Stream/iostream)
#include	INCL_KCORE(Tools/QdtString)

#define		QDT_INCLUDE_STDIO
#include	INCL_KCORE(Includes/Includes)

#define		QDT_INCLUDE_PC_WINDOWS
#define		QDT_INCLUDE_WIN32_EXCEPTIONS
#include	INCL_KCORE(Includes_PC/Includes_PC)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

QDT_VECTOR<void (*)(unsigned int, _EXCEPTION_POINTERS *)>	OS::_vpExceptionFunc;

//-----------------------------------------------------------------------------
//	Name:		GetCallStack
//	Object:		
//	01-07-11:	ONA - Created
//-----------------------------------------------------------------------------
void *	OS::GetCallStack(unsigned int	nLevel)
{
	unsigned int	*pStack;
	unsigned int	*pCall = NULL;

	if	(_pEIP)
	{
		//	Special case for exceptions...
		if	(nLevel == 0)
		{
			return	(_pEIP);
		}
		--nLevel;
		pStack = reinterpret_cast<unsigned int *>(_pEBP);
	}
	else
	{
		__asm
		{
			mov	pStack, ebp
		}
	}

	MEMORY_BASIC_INFORMATION	MBI;

	do
	{
		// PIPO!!! PIPO!!!!
		// Strange test to avoid to get a invalid call pointer
		// This can happen if you go upper than possible
		if ((int)pStack&0x80000000L)			
		{
			break;
		}
		// PIPO!!! PIPO!!!!

		VirtualQuery(pStack, &MBI, sizeof(MBI));

		if	(MBI.State == MEM_RESERVE)
		{
			pStack = NULL;
		}
		else
		{
			switch(MBI.AllocationProtect)
			{
			case	PAGE_READONLY:
			case	PAGE_READWRITE:
			case	PAGE_WRITECOPY:
			case	PAGE_EXECUTE:
			case	PAGE_EXECUTE_READ:
			case	PAGE_EXECUTE_READWRITE:
			case	PAGE_EXECUTE_WRITECOPY:
			case	PAGE_GUARD:
			case	PAGE_NOACCESS:
			case	PAGE_NOCACHE:
				break;
			default:
				pStack = NULL;
				break;
			}
		}

		if	( pStack == NULL )
		{
			break;
		}

		pCall = ( unsigned int* )( *( pStack + 1 ) );
		pStack = ( unsigned int* )( *pStack );

		if	( pCall == NULL )
		{
			break;
		}
	}
	while( nLevel-- );


	return	( reinterpret_cast< void* >( pCall ) );
}

//-----------------------------------------------------------------------------
// Name: OS::GetFunctionName
// Object: En fonction d'un a
//00-12-08 ONA Created: 
//-----------------------------------------------------------------------------
const void * const	OS::GetFunctionName( const void * const	pCallStackAddr,
										 char * const		pcBufferFuncName,
										 unsigned int		nSizeBuffer )
{
	const char		*pcCurrent;
	char			cBufferFinalFuncName[2048] = "";
	char			cBufferString[2048];
	unsigned int	nMaxDiffAddress = 0xffffffff;
	unsigned int	nDiffAddress;
	void			*pAddressRead;
	void			*pAddressFound = NULL;
	unsigned int	nSizeCopied;

	int				nCurrentAdr;
	const char *	pLastBuffer;

	static char			*pcStart = NULL, *pcEnd = NULL;
	static unsigned int	nNbrAdress = 0;
	static void * *		pTabAdr = NULL;

	const char *	pcStartWord;

	//	Initializing of the variables...
	if	( ( pcStart == NULL ) || ( pcEnd == NULL ) )
	{
		FILE	*pF;
		int		nSize;

		//	First : Load file
		OS::GetProjectFileName(cBufferString, sizeof(cBufferString));

		if	(strlen(cBufferString) == 0)
		{
			return	( NULL );
		}

		//	Assume that the last four bytes are ".exe"
		strcpy(cBufferString + strlen(cBufferString) - 4, ".map");

		pF = fopen( cBufferString , "rb" );
		if	( pF == NULL )
		{
			return	( NULL );
		}
		fseek( pF , 0 , SEEK_END );
		nSize = ftell( pF );
		fseek( pF, 0, SEEK_SET );

		pcStart = ( char* )malloc( nSize );
		fread( pcStart, nSize, 1, pF );
		fclose( pF );

		pcEnd = pcStart + nSize;

		//	Second : Parse the file to create the database 
		pcCurrent = pcStart;
		while	(pcCurrent < pcEnd)
		{
			pcCurrent = FindWord(pcCurrent, pcEnd, cBufferString, sizeof(cBufferString), nSizeCopied, pcStartWord);

			if	((nSizeCopied == 8) && (IsXDigit(cBufferString)))
			{
				++nNbrAdress;
			}
		}

		//	Third : Reserve and fill the table
		pTabAdr = reinterpret_cast<void * *>(malloc(nNbrAdress * sizeof(void *) * 2));

		pcCurrent = pcStart;
		pLastBuffer = NULL;
		nCurrentAdr = 0;

		while	(pcCurrent < pcEnd)
		{
			pcCurrent = FindWord(pcCurrent, pcEnd, cBufferString, sizeof(cBufferString), nSizeCopied, pcStartWord);

			if	((nSizeCopied == 8) && (IsXDigit(cBufferString)))
			{
				strupr(cBufferString);

				sscanf(cBufferString, "%08X", &pAddressRead);

				pTabAdr[nCurrentAdr + 0] = pAddressRead;
				pTabAdr[nCurrentAdr + 1] = (void *)pLastBuffer;

				nCurrentAdr += 2;
			}
			else
			{
				//	Record the buffer adress
				pLastBuffer = pcStartWord;
			}
		}
	}

	//	Now we can search the best adress...
	nCurrentAdr = 2 * nNbrAdress;
	while(nCurrentAdr > 0)
	{
		nCurrentAdr -= 2;

		pAddressRead = pTabAdr[nCurrentAdr + 0];
		if	(pAddressRead <= pCallStackAddr)
		{
			nDiffAddress = reinterpret_cast< unsigned int >( pCallStackAddr ) - reinterpret_cast<unsigned int>(pAddressRead);
			if	( nDiffAddress < nMaxDiffAddress )
			{
				nMaxDiffAddress = nDiffAddress;
				pAddressFound = pAddressRead;

				const char * pSrc = (const char *)pTabAdr[nCurrentAdr + 1];
				char * pDst = cBufferFinalFuncName;
				char	c;
				int		i = sizeof(cBufferFinalFuncName);

				while(i > 1)
				{
					c = *pSrc++;
					if	((c == ' ') || (c == '	') || (c == 0xd) || (c == 0xa))
					{
						*pDst++ = 0;
						break;
					}

					*pDst++ = c;
					--i;
				}
			}
		}
	}

	if	(pAddressFound)
	{
		OS::ConvertFuncName(pcBufferFuncName, cBufferFinalFuncName, nSizeBuffer);
	}

	//	On ne libère pas le bloc une fois qu'il a été alloué !!!
	//	En effet le fichier .map peut faire 2 Mo et être très long à charger
	//	Si on veux y accéder plusieurs fois. Comme OS::GetFunctionName() n'est
	//	appelé qu'en cas de problème (QDT_ASSERT, Erreurs graves) ce n'est pas
	//	très important qu'il y ait un MemoryLeak que dans ces cas là.
	//free(pcStart);

	return	( pAddressFound );
}

const char*	OS::FindWord( const char *		pcStart,
						  const char *		pcEnd,
						  char *			pcWriteBuffer,
						  unsigned int		nSize,
						  unsigned int &	nSizeCopied,
						  const char * &	pcStartWord)
{
	char			cC;
	const char		*pcCurrent = pcStart;
	bool			bFlagFirstNonSpace = false;

	nSizeCopied = 0;

	strcpy( pcWriteBuffer, "" );

	QDT_ASSERT( nSize > 0 );

	pcStartWord = pcCurrent;

	while	(pcCurrent < pcEnd )
	{
		cC = *pcCurrent++;

		if	( bFlagFirstNonSpace == false )
		{
			//	Nous sommes dans une suite de caractère d'espacement
			if	((cC != ' ') && (cC != '	') && (cC != 0xd) && (cC != 0xa))
			{
				//	On a donc trouvé le premier caractère différent d'un espacement
				bFlagFirstNonSpace = true;

				pcStartWord = pcCurrent - 1;

				if	( nSizeCopied < nSize - 1 )
				{
					*pcWriteBuffer++ = cC;
					++nSizeCopied;
				}
			}
		}
		else
		{
			//	Nous sommes dans une suite de caractère différents des espacements
			if	((cC == ' ') || (cC == '	') || (cC == 0xd) || (cC == 0xa))
			{
				//	On vient de retrouver un caractère d'espacement

				//	On ajoute le caractère terminal de la chaîne de caractère
				//	La position est valide car on n'écrivait déjà pas plus de nSize-1 caractères
				*pcWriteBuffer = 0;

				return	(pcCurrent);
			}
			else
			{
				//	Ce n'est toujours pas un caractère d'espacement
				//	Donc on l'ajoute à la suite...
				if	(nSizeCopied < nSize - 1 )
				{
					*pcWriteBuffer++ = cC;
					++nSizeCopied;
				}
			}
		}
	}

	//	On est arrivé à la fin du fichier avant d'avoir trouvé le mot ou bien la fin du mot
	//	Donc on termine quand même la séquence que l'on vient d'enregistrer dans le buffer
	*pcWriteBuffer = 0;

	return	( pcCurrent );
}

bool	OS::IsXDigit( const char *	pcStr )
{
	while( *pcStr )
	{
		if	( isxdigit( *pcStr ) == false )
		{
			return	( false );
		}
		++pcStr;
	}
	return	( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectName
//	Object:		
//	01-07-06:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::GetProjectName(char * const	psBuffer,
						   const int	nSize)
{
	char	sBuffer[_MAX_PATH];
	int		i, j = -1;
	int		nSizeCopy;

	GetProjectFileName(sBuffer, sizeof(sBuffer));

	i = j = strlen(sBuffer);

	while(i >= 0)
	{
		--i;

		if	(sBuffer[i] == '\\')
		{
			break;
		}
		if	(sBuffer[i] == '/')
		{
			break;
		}

		if	(sBuffer[i] == '.')
		{
			j = i;
		}
	}

	//	Skip the '\' or the '/'
	++i;

	nSizeCopy = j - i;
	if	(nSizeCopy > nSize)
	{
		nSizeCopy = nSize;
	}

	strncpy(psBuffer, sBuffer + i, nSizeCopy);
	psBuffer[nSizeCopy] = '\0';
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectFileName
//	Object:		
//	01-07-06:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::GetProjectFileName(char * const	psBuffer,
							   const int	nSize)
{
	OSVERSIONINFO	O;
	HINSTANCE		hLib;
	HANDLE	hProcess;
	HMODULE	hModule;
	DWORD	dwSize2;

	//	function definitions for PSAPI
	BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *, DWORD, LPDWORD );
	DWORD (WINAPI *lpfGetModuleFileNameEx)( HANDLE, HMODULE, LPTSTR, DWORD );

	//	Initialize buffer
	*psBuffer = '\0';

	O.dwOSVersionInfoSize = sizeof(O);
	if	(GetVersionEx(&O) == false)
	{
		return;
	}

	if	(O.dwPlatformId != VER_PLATFORM_WIN32_NT)
	{
		//	We are not under Windows NT
		return;
	}


	hLib = LoadLibrary("psapi.dll");
	if	(hLib == NULL)
	{
		return;
	}

	lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *, DWORD, LPDWORD)) GetProcAddress( hLib, "EnumProcessModules" ) ;
	lpfGetModuleFileNameEx =(DWORD (WINAPI *)(HANDLE, HMODULE, LPTSTR, DWORD )) GetProcAddress( hLib, "GetModuleFileNameExA" ) ;

	if	((lpfEnumProcessModules == NULL) || (lpfGetModuleFileNameEx == NULL))
	{
		// some of the functions did not load properly... return
		FreeLibrary(hLib);
		return;
	}

	hProcess = GetCurrentProcess();

	if	(lpfEnumProcessModules(hProcess, &hModule, sizeof(hModule), &dwSize2) == false)
	{
		FreeLibrary(hLib);
		return;
	}

	if	(lpfGetModuleFileNameEx(hProcess, hModule, psBuffer, nSize) == false)
	{
		FreeLibrary(hLib);
		return;
	}
}

//-----------------------------------------------------------------------------
//	Name:		ExceptionsOccured
//	Object:		
//	01-07-13:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::ExceptionsOccured(unsigned int			nType,
							  _EXCEPTION_POINTERS *	pEP)
{
	if	(GetHandleExceptions() == false)
	{
		return;
	}

	char	sBuffer[256];

	OS::SetEIP((void*)pEP->ContextRecord->Eip);
	OS::SetEBP((void*)pEP->ContextRecord->Ebp);

	switch(nType)
	{
	case	EXCEPTION_ACCESS_VIOLATION:
		strcpy(sBuffer, "EXCEPTION_ACCESS_VIOLATION : The thread tried to read from or write to a virtual address for which it does not have the appropriate access.\n");
		break;
	case	EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		strcpy(sBuffer, "EXCEPTION_ARRAY_BOUNDS_EXCEEDED : The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.\n");
		break;
	case	EXCEPTION_BREAKPOINT:
		strcpy(sBuffer, "BREAKPOINT : A breakpoint was encountered.\n");
		break;
	case	EXCEPTION_DATATYPE_MISALIGNMENT:
		strcpy(sBuffer, "EXCEPTION_DATATYPE_MISALIGNMENT : The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.\n");
		break;
	case	EXCEPTION_FLT_DENORMAL_OPERAND:
		strcpy(sBuffer, "EXCEPTION_FLT_DENORMAL_OPERAND : One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.\n");
		break;
	case	EXCEPTION_FLT_DIVIDE_BY_ZERO:
		strcpy(sBuffer, "EXCEPTION_FLT_DIVIDE_BY_ZERO : The thread tried to divide a floating-point value by a floating-point divisor of zero.\n");
		break;
	case	EXCEPTION_FLT_INEXACT_RESULT:
		strcpy(sBuffer, "EXCEPTION_FLT_INEXACT_RESULT : The result of a floating-point operation cannot be represented exactly as a decimal fraction.\n");
		break;
	case	EXCEPTION_FLT_INVALID_OPERATION:
		strcpy(sBuffer, "EXCEPTION_FLT_INVALID_OPERATION : This exception represents any floating-point exception not included in this list.\n");
		break;
	case	EXCEPTION_FLT_OVERFLOW:
		strcpy(sBuffer, "EXCEPTION_FLT_OVERFLOW : The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.\n");
		break;
	case	EXCEPTION_FLT_STACK_CHECK:
		strcpy(sBuffer, "EXCEPTION_FLT_STACK_CHECK : The stack overflowed or underflowed as the result of a floating-point operation.\n");
		break;
	case	EXCEPTION_FLT_UNDERFLOW:
		strcpy(sBuffer, "EXCEPTION_FLT_UNDERFLOW : The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.\n");
		break;
	case	EXCEPTION_ILLEGAL_INSTRUCTION:
		strcpy(sBuffer, "EXCEPTION_ILLEGAL_INSTRUCTION : The thread tried to execute an invalid instruction.\n");
		break;
	case	EXCEPTION_IN_PAGE_ERROR:
		strcpy(sBuffer, "EXCEPTION_IN_PAGE_ERROR : The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.\n");
		break;
	case	EXCEPTION_INT_DIVIDE_BY_ZERO:
		strcpy(sBuffer, "EXCEPTION_INT_DIVIDE_BY_ZERO : The thread tried to divide an integer value by an integer divisor of zero.\n");
		break;
	case	EXCEPTION_INT_OVERFLOW:
		strcpy(sBuffer, "EXCEPTION_INT_OVERFLOW : The result of an integer operation caused a carry out of the most significant bit of the result.\n");
		break;
	case	EXCEPTION_INVALID_DISPOSITION:
		strcpy(sBuffer, "EXCEPTION_INVALID_DISPOSITION : An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.\n");
		break;
	case	EXCEPTION_NONCONTINUABLE_EXCEPTION:
		strcpy(sBuffer, "EXCEPTION_NONCONTINUABLE_EXCEPTION : The thread tried to continue execution after a noncontinuable exception occurred.\n");
		break;
	case	EXCEPTION_PRIV_INSTRUCTION:
		strcpy(sBuffer, "EXCEPTION_PRIV_INSTRUCTION : The thread tried to execute an instruction whose operation is not allowed in the current machine mode.\n");
		break;
	case	EXCEPTION_SINGLE_STEP:
		strcpy(sBuffer, "EXCEPTION_SINGLE_STEP : A trace trap or other single-instruction mechanism signaled that one instruction has been executed.\n");
		break;
	case	EXCEPTION_STACK_OVERFLOW:
		strcpy(sBuffer, "EXCEPTION_STACK_OVERFLOW : The thread used up its stack.\n");
		break;
	default:
		strcpy(sBuffer, "EXCEPTION : Unknown exception.\n");
		break;
	}

	QDT_Error(sBuffer);

	OS::SetEBP(NULL);
	OS::SetEIP(NULL);

	//	Report to all the callback
	ReportException(nType, pEP);
}

//-----------------------------------------------------------------------------
//	Name:		InitExceptions
//	Object:		
//	01-07-13:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::InitExceptions()
{
	// Must be called in app to be enabled (ie IAM not runtime for the moment)
#ifndef _MASTER
	_set_se_translator(ExceptionsOccured);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		AddExceptionFunction
//	Object:		
//	01-07-13:	ONA - Created
//-----------------------------------------------------------------------------
void	OS::AddExceptionFunction(void	(*pExceptionFunc)(unsigned int, _EXCEPTION_POINTERS *))
{
	_vpExceptionFunc.PushTail(pExceptionFunc);
}

//-----------------------------------------------------------------------------
//	Name:		GetCursorPos
//	Object:		
//	01-11-12:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::GetCursorPos(int &	nXPos,
						 int &	nYPos)
{
	POINT pt;
	::GetCursorPos( &pt );
	
	nXPos = pt.x;
	nYPos = pt.y;
}

//-----------------------------------------------------------------------------
//	Name:		SetCursorPos
//	Object:		
//	01-11-12:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::SetCursorPos(const int	nXPos,
						 const int	nYPos)
{
	::SetCursorPos( nXPos, nYPos );
}

//-----------------------------------------------------------------------------
//	Name:		GetWinLastErrorMsg
//	Object:		
//	02-05-15:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::GetWinLastErrorMsg(QDT_STRING&	strMsgBuffer)
{
	LPVOID		lpMsgBuf;
	
	FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL,
					GetLastError(),
					0, // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL );
				
	strMsgBuffer = static_cast<char*>(lpMsgBuf);
	
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

//-----------------------------------------------------------------------------
//	Name:		ReportException
//	Object:		
//	04-05-18:	DAC - Created
//-----------------------------------------------------------------------------
void	OS::ReportException(unsigned int			nType,
							::_EXCEPTION_POINTERS *	pEP)
{
	QDT_VECTOR<void (*)(unsigned int, _EXCEPTION_POINTERS *)>::ITERATOR	I, IEnd;

	I = _vpExceptionFunc.GetHead();
	IEnd = _vpExceptionFunc.GetTail();

	while(I != IEnd)
	{
		(*I)(nType, pEP);
		++I;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPlatformString
//	Object:		
//	04-11-23:	DAC - Created
//-----------------------------------------------------------------------------
const char *	OS::GetPlatformString()
{	
	static const char * sPlatformString = "PC";	
	return	(sPlatformString);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
