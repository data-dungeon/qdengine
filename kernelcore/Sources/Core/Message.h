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
//	File	:	Message.h
//	Category:	KernelCore/Sources/Core/
//
//	05-09-19:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_CORE_MESSAGE_H__
#define __KCORE_CORE_MESSAGE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		class OPTIONAL_FUNC_CONTAINER
		{
			public:
				typedef void (*OutputFunc) (const char* strOutput);
				static OutputFunc _pOutputFunc;
		};

		enum
		{
			QDT_ABORTRETRYIGNORE	= ( 1 << 0),
			QDT_OK					= ( 1 << 1),
			QDT_OKCANCEL			= ( 1 << 2),
			QDT_RETRYCANCEL			= ( 1 << 3),
			QDT_YESNO				= ( 1 << 4),
			QDT_YESNOCANCEL			= ( 1 << 5),

			QDT_ICONEXCLAMATION		= ( 1 << 6),
			QDT_ICONWARNING			= ( 1 << 7),
			QDT_ICONINFORMATION		= ( 1 << 8),
			QDT_ICONASTERISK		= ( 1 << 9),
			QDT_ICONQUESTION		= ( 1 << 10),
			QDT_ICONSTOP			= ( 1 << 11),
			QDT_ICONERROR			= ( 1 << 12),
			QDT_ICONHAND			= ( 1 << 13),

			QDT_DEFBUTTON1			= ( 1 << 14),
			QDT_DEFBUTTON2			= ( 1 << 15),
			QDT_DEFBUTTON3			= ( 1 << 16),
			QDT_DEFBUTTON4			= ( 1 << 17),


			QDT_APPLMODAL			= ( 1 << 18),

			QDT_SETFOREGROUND		= ( 1 << 19),
			QDT_TOPMOST				= ( 1 << 20),
		};

		enum
		{
			QDT_IDABORT = 1,
			QDT_IDCANCEL,
			QDT_IDIGNORE,
			QDT_IDNO,
			QDT_IDOK,
			QDT_IDRETRY,
			QDT_IDYES,
		};

		extern void (*g_QDTCustomTraceFunc)(const char*);
		typedef bool (TQDT_CustomAssertFunc)(const char* pTitle, const char* pCondition, const char* pMessage, const char* pFile, int nLine, bool* pbDontCare);
		extern TQDT_CustomAssertFunc*	g_QDTCustomAssertFunc;
		extern TQDT_CustomAssertFunc	g_QDTCustomAssertFuncDefault;


		void	QDT_Message				(const char* pTextMessage, ...);

		void	QDT_MessageLog			(const char* pTextMessage, ...); 
		void	QDT_MessageLog			(UInt32 logIdx, const char* pTextMessage, ...);

		void	QDT_MessageThreadSafe	(const char* pTextMessage, ...);
		void	QDT_Warning				(const char* pTextWarning, ...);
		void	QDT_Error				(const char* pTextError, ...);
		int		QDT_MessageBox			(int hWnd, const char* pTextMessage, const char* pCaption, const int nFlags = QDT_OK);
		void	QDT_OutputDebugString	(const char* pOutputString);

		int		QDT_AssertFunc			(const char* pTitle, const char* pCondition, const char* pFile, int nLine, bool* pbDontCare);
	    void	QDT_TraceAssertFunc		(const char* pFormat, ...);
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
