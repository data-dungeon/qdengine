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

#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_STDARG
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

#include <sysutil/sysutil_msgdialog.h>
#include <PSGL/psgl.h>

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
}

//-----------------------------------------------------------------------------
//	Name:		g_QDTCustomAssertFuncDefault
//	Object:		
//	06-05-19:	BFA - Created
//-----------------------------------------------------------------------------
bool SysUtilInit = false;
volatile bool SysUtilReplied;
bool SysUtilBreak;

static void cb_dialog_yesno(int button_type, void* userdata)
{
	SysUtilReplied = true;

	switch(button_type)
	{
	case CELL_MSGDIALOG_BUTTON_YES:
		{
			SysUtilBreak =true;
			break;
		}
	case CELL_MSGDIALOG_BUTTON_NO:
		{
			SysUtilBreak =false;
			break;
		}
	default:
		{
			SysUtilBreak =false;
			break;
		}
	}

	return;
}

#define BUFFER_SIZE 512

bool g_QDTCustomAssertFuncDefault(const char* pTitle, const char* pCondition, const char* pMessage, const char* pFile, int nLine, bool* pbIgnoreAll)
{
	char Buffer[BUFFER_SIZE] ="";

	if(SysUtilInit == false)
	{
		cellSysutilInit();
		SysUtilInit = true;
	}

	sprintf(Buffer,"%s\nCondition: %s\n%s (%d)\nBreak in debugger ?", pMessage, pCondition, pFile, nLine);

	SysUtilReplied = false;
	cellMsgDialogOpen(
		CELL_MSGDIALOG_DIALOG_TYPE_NORMAL
		|CELL_MSGDIALOG_BUTTON_TYPE_YESNO
		|CELL_MSGDIALOG_DEFAULT_CURSOR_NO,
		Buffer,
		cb_dialog_yesno,
		NULL,
		NULL);

	while(SysUtilReplied == false)
	{
		cellSysutilCheckCallback();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		psglSwap();
	}

	return SysUtilBreak;	//=break
}

	}
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
