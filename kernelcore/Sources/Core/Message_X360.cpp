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
//	Message.cpp
//
//	05-09-19:	RMA - Created
//*****************************************************************************

#include "Root.h"
#include "Message.h"
//#include	"Include.h"

#include	INCL_KCORE(IceError/ErrorManager)
#include	INCL_KCORE(IceError/KCoreError)
#include	INCL_KCORE(IceError/KCoreErrorModule)
#include	INCL_KCORE(Tools/QdtStringUnicode)

#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_STDARG
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#define		QDT_INCLUDE_X360_XTL
#include	INCL_KCORE(Includes_X360/Includes_X360)

#include	INCL_KRENDERER(Renderer/RenderContext)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{
	namespace	KCORE
	{

enum
{
	BUFFER_SIZE = 4096,
};

static char Buffer[BUFFER_SIZE] ="";

//-----------------------------------------------------------------------------
//	Name:		QDT_Message
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	QDT_Message(const char*	TextMessage, ...)
{
#ifndef _MASTER
	va_list	Args;
	
	va_start(Args, TextMessage);
	vsprintf(Buffer, TextMessage, Args);
	va_end(Args);
	
	printf("%s\n",Buffer);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_MessageThreadSafe
//	Object:		
//	06-03-21:	GRI - Created
//-----------------------------------------------------------------------------
void	QDT_MessageThreadSafe(const char*	TextMessage, ...)
{
#ifndef _MASTER
	va_list	Args;
	
	va_start(Args, TextMessage);
	vsprintf(Buffer, TextMessage, Args);
	va_end(Args);
	
	printf("%s\n",Buffer);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_Warning
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	QDT_Warning(const char*	TextWarning, ...)
{
#ifndef _MASTER
	strcpy(Buffer, "[WARNING] : ");
	va_list	Args;

	va_start(Args, TextWarning);
	vsprintf(Buffer + strlen(Buffer), TextWarning, Args);
	va_end(Args);

	// Register the warning
	KCORE_ERROR * pErr = static_cast<KCORE_ERROR*>( ERROR_MANAGER::Instance()->NewError(ERROR_MANAGER::MI_KCORE, KCORE_ERROR_MODULE::KE_QDT_WARNING));
	pErr->SetAdditionnalErrorText(Buffer);

	printf("%s\n",Buffer, true);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_Error
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	QDT_Error(const char*	TextError, ...)
{
#ifndef _MASTER
	strcpy(Buffer, "[ERROR] : ");
	va_list	Args;

	va_start(Args, TextError);
	vsprintf(Buffer + strlen(Buffer), TextError, Args);
	va_end(Args);

	// Register the error
	KCORE_ERROR * pErr = static_cast<KCORE_ERROR*>( ERROR_MANAGER::Instance()->NewError(ERROR_MANAGER::MI_KCORE, KCORE_ERROR_MODULE::KE_QDT_ERROR));
	pErr->SetAdditionnalErrorText(Buffer);

	printf("%s\n",Buffer, true);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		QDT_MessageBox
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
int		QDT_MessageBox( int	hWnd, const char *	pText, const char * pCaption, const int	nFlags)
{
#ifndef _MASTER
	strcpy(Buffer, "[MESSAGE BOX] : [");
	strcat(Buffer, pCaption);	
	strcat(Buffer, "] : ");
	strcat(Buffer, pText);
	printf("%s\n",Buffer, true);
#endif
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		g_QDTCustomAssertFuncDefault
//	Object:		
//	06-05-19:	BFA - Created
//-----------------------------------------------------------------------------
bool QDT::KCORE::g_QDTCustomAssertFuncDefault(const char* pTitle, const char* pCondition, const char* pMessage, const char* pFile, int nLine, bool* pbIgnoreAll)
{
	char Buffer[BUFFER_SIZE] ="";
	MESSAGEBOX_RESULT	Result;
	XOVERLAPPED			Overlapped;
	LPCWSTR	ButtonText[] = { L"Yes", L"No" };
	ZeroMemory( &Overlapped, sizeof( XOVERLAPPED ) );

	sprintf(Buffer,"%s\nCondition: %s\n%s (%d)\nBreak in debugger ?", pMessage, pCondition, pFile, nLine);
	XShowMessageBoxUI( 0, (LPCWSTR)QDT_STRING_UNICODE(pTitle).GetBuffer(), (LPCWSTR)QDT_STRING_UNICODE(Buffer).GetBuffer(), 2, ButtonText, 0, XMB_ERRORICON, &Result, &Overlapped);
	printf("%s\n",Buffer);

	while ( !XHasOverlappedIoCompleted( &Overlapped ) )
	{
		//if ( GetCurrentThreadId() == 0xf9000000 )
		//{
		//	RENDER_CONTEXT::Present();
		//}
	}

	if( XGetOverlappedResult( &Overlapped, NULL, TRUE ) == ERROR_SUCCESS )
    {
      return (Result.dwButtonPressed == 0);
    }

	return 1;
}

	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
