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
//	06-05-19:	BFA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(Core/Message)
#include	INCL_KCORE(Core/MessageLog)

#include	<String.h>
#include	<Stdarg.h>
#include	<Stdio.h>

QDT::KCORE::TQDT_CustomAssertFunc* QDT::KCORE::g_QDTCustomAssertFunc = &g_QDTCustomAssertFuncDefault;

#define BUFFER_SIZE 4096

#define ASSERT_MESSAGE_MAX_SIZE (1024)
static char g_Message[ASSERT_MESSAGE_MAX_SIZE]= "";

// multi plateform version
void QDT::KCORE::QDT_TraceAssertFunc(const char* pFormat, ...)
{
	if (pFormat==NULL)
	{
		g_Message[0]= '\0';
	}
	else
	{
		QDT_ASSERT_MSG(strlen(pFormat)<ASSERT_MESSAGE_MAX_SIZE, ("Debug Output too long..."));
		va_list marker;
		va_start(marker, pFormat);
		int nChar= vsprintf(g_Message, pFormat, marker);
		va_end(marker);
		QDT_ASSERT_MSG(nChar<ASSERT_MESSAGE_MAX_SIZE-1, ("Debug Output too long..."));
	}
}

// multi plateform version
int QDT::KCORE::QDT_AssertFunc(const char* pTitle, const char* pCondition, const char* pFile, int nLine, bool* pbDontCare)
{
	QDT_Message("%s(%d): QDT_ASSERT - Condition %s FAILED !", pFile, nLine, pCondition);
	if (g_Message[0]!='\0')
	{
		QDT_Message("\tReason : %s", g_Message);
	}

	if (g_QDTCustomAssertFunc!=NULL)
		return (*g_QDTCustomAssertFunc)(pTitle, pCondition, g_Message, pFile, nLine, pbDontCare) ? 2 : 0;

	return 1;
}

namespace QDT
{
namespace KCORE
{
//-----------------------------------------------------------------------------
//	Name:		QDT_MessageLog
//	Object:		
//	06-06-13:	VHA - Created
//-----------------------------------------------------------------------------
void	QDT_MessageLog(const char *	pTextMessage,...)
{
	if	(pTextMessage != NULL)
	{
		char Buffer[BUFFER_SIZE] ="";

		va_list	Args;

		va_start(Args, pTextMessage);
#ifdef PLATFORM_PC
		_vsnprintf(Buffer, BUFFER_SIZE, pTextMessage, Args);
#else
		vsnprintf(Buffer, BUFFER_SIZE, pTextMessage, Args);
#endif
		va_end(Args);
		
		MESSAGE_LOG::PushMessageToLogIdx(Buffer,0);
	}
}


//-----------------------------------------------------------------------------
//	Name:		QDT_MessageLog
//	Object:		
//	06-06-13:	VHA - Created
//-----------------------------------------------------------------------------
void	QDT_MessageLog(UInt32 logIdx, const char *	pTextMessage, ...)
{
	if	(pTextMessage != NULL)
	{
		char Buffer[BUFFER_SIZE] ="";

		va_list	Args;
		
		va_start(Args, pTextMessage);
#ifdef PLATFORM_PC
		_vsnprintf(Buffer, BUFFER_SIZE, pTextMessage, Args);
#else
		vsnprintf(Buffer, BUFFER_SIZE, pTextMessage, Args);
#endif
		va_end(Args);
		
		MESSAGE_LOG::PushMessageToLogIdx(Buffer,logIdx);
	}
}

}
}

