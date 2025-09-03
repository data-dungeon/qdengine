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


//=============================================================================
// Name: OS
//00-11-21 ONA Created: 
//=============================================================================

#ifndef __KCORE_OS_OS_H__
#define __KCORE_OS_OS_H__

#ifndef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KCORE(QDT_STRING);
DECL_KCORE(IO_STREAM);

#define		QDT_INCLUDE_PC_FPIEEE
#include	INCL_KCORE(Includes_PC/Includes_PC)
#include	INCL_KCORE(Template/QDT_Vector)

namespace QDT
{
	namespace KCORE
	{


		class OS
		{
		public:

			enum
			{
				SIZE_DUMP	=	32,
			};

			static void					SetEBP(void * pEBP);
			static void					SetEIP(void * pEIP);

			static bool					memcmp(const void * const pBuffer, unsigned char const cC, unsigned int const nSize);

			static bool					Convert0_8(double d, signed char &c);
			static signed char			Convert0_8(double	d);
			static bool					Convert8_8(double	d , short	&s);
			static short				Convert8_8(double	d);

			static void					Dump(IO_STREAM & Out, const void * const pBuffer, unsigned int nSize = SIZE_DUMP);

			//	Fonctions de gestions bas niveau de l'OS


			//-----------------------------------------------------------------------------
			// Name: OS::GetCallStack
			// Object: Récupère l'adresse de la fonction appelante au niveau nLevel+1
			//00-12-08 ONA Created: 
			//-----------------------------------------------------------------------------
			static void*				GetCallStack(unsigned int	nLevel);

			//-----------------------------------------------------------------------------
			// Name: OS::GetFunctionName
			// Object: Remplit le buffer envoyé par le nom de la fonction dont l'adresse est envoyé en paramètre
			//00-12-08 ONA Created: 
			//-----------------------------------------------------------------------------
			static const void * const	GetFunctionName(const void * const	pFuncAddr, char * const	pcBufferFuncName, unsigned int nSizeBuffer);

			static const char*			FindWord(const char * pcStart, const char * pcEnd, char * pcWriteBuffer, unsigned int nSize, unsigned int & nSizeCopied, const char * & pcStartWord);
			static bool					IsXDigit(const char * pcStr);

			static void					GetProjectName(char * const psBuffer, const int nSize);
			static void					GetProjectFileName(char * const psBuffer, const int nSize);

			static unsigned int			GetStrongBit(const unsigned int nSize);
			static unsigned int			GetNbrBits(const unsigned int nSize);

			static void					ConvertFuncName(char * const pcBufferFuncName, const char * const pcMapFuncName, const int nSizeBuffer);

			static void					GetProtection(char * const pcBuffer, const int nSizeBuffer, const char c);
			static void					GetConst(char * const pcBuffer, const int nSizeBuffer, const char c);
			static const char *			GetParameter(char * const pcBufferDst, const char * const pcBufferSrc, int nSizeBuffer, bool & bLastParameter, char * psNameSpace[32], const int nSizeTable);

		#if defined PLATFORM_PC
			static void					ReportException(unsigned int nType, ::_EXCEPTION_POINTERS * pEP);
			static void					ExceptionsOccured(unsigned int nType, ::_EXCEPTION_POINTERS * pEP);
			static void					InitExceptions();
			static void					AddExceptionFunction(void (*pExceptionFunc)(unsigned int, ::_EXCEPTION_POINTERS *));
		#endif

			static void					EnableDisplayLoadingMsg(const bool bFlag);
			static bool					IsDisplayLoadingMsgOn();
			static void					DisplayLoadingMsg();

			static void					EnableNoMessageBox(const bool bFlag);
			static bool					IsEnableNoMessageBoxOn();

			//Kernel Core Object ??
			static void					SetAppHandle(long);
			static long					GetAppHandle();
			static void					SetWinHandle(long);
			static long					GetWinHandle();
			static long					GetDispHandle();

			static void					EnableLogOutput(const bool bFlag);
			static bool					IsLogOutputEnabled();
			static bool					SetHandleExceptions(const bool bFlag);
			static bool					GetHandleExceptions();
			//Kernel Core Object ??

			//Temp mouse handling
			static void					GetCursorPos(int &nXPos, int &nYPos);
			static void					SetCursorPos(const int nXPos, const int nYPos);

			static void					GetWinLastErrorMsg(QDT_STRING&	strMsgBuffer);

			static unsigned int			GetBrowserLanguage();

			static const char *			GetPlatformString();

			static QDT_STRING			GetHumanReadableSize(UInt32	size);

		private:

			static void *				_pEBP;
			static void *				_pEIP;

		#if defined (PLATFORM_PC)
			static QDT_VECTOR<void (*)(unsigned int, ::_EXCEPTION_POINTERS *)>	_vpExceptionFunc;
		#endif

			static long					_nAppHandle;
			static long					_nWinHandle;
			static bool					_bLogOutput;
			static bool					_bHandleExceptions;
			static bool					_bDisplayLoadingMsg;
			static bool					_bNoMessageBox;
		};
	}
}

#endif
//=============================================================================
// CODE ENDS HERE
//=============================================================================
