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
//	CLASS:	MESSAGE_LOG
//
//	06-06-12:	VHA - Created
//*****************************************************************************

#include	"root.h"

#include	INCL_KCORE(Core/MessageLog)
#include	INCL_KCORE(Stream/filestream)

#include	<stdio.h>

#ifndef _MASTER

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

QDT_VECTOR<MESSAGE_LOG*> MESSAGE_LOG::g_pMessageLogs;

//-----------------------------------------------------------------------------
//	Name:		MESSAGE_LOG constructor
//	Object:		
//	06-06-12:	VHA - Created
//-----------------------------------------------------------------------------
MESSAGE_LOG::MESSAGE_LOG(UInt32 logIdx)
{
	_bIsActive = true;
	_nLogIdx = logIdx;

	char logName[256];
	sprintf(logName, "log-%d.txt", logIdx);

	_fileStream = new FILE_TEXT_STREAM(logName, FILE_STREAM::AT_CREATE);
}

//-----------------------------------------------------------------------------
//	Name:		MESSAGE_LOG destructor
//	Object:		
//	06-06-12:	VHA - Created
//-----------------------------------------------------------------------------
MESSAGE_LOG::~MESSAGE_LOG()
{
	_fileStream->Close();
	delete _fileStream;
}

//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	06-06-12:	VHA - Created
//-----------------------------------------------------------------------------
void	MESSAGE_LOG::Release()
{
	g_pMessageLogs.ClearAll();
}

//-----------------------------------------------------------------------------
//	Name:		PushMessageToLog
//	Object:		
//	06-06-13:	VHA - Created
//-----------------------------------------------------------------------------
void	MESSAGE_LOG::PushMessageToLogIdx(	const char*	pMessage,
											UInt32		nLogIdx)
{
	UInt32 numMessageLogs;
	UInt32 i;

	numMessageLogs = g_pMessageLogs.GetSize();

	for(i=0;i<numMessageLogs;i++)
	{
		MESSAGE_LOG* pLog = g_pMessageLogs[i];
		if(pLog->GetLogIdx() == nLogIdx)
		{
			pLog->PushMessage(pMessage);
			return;
		}
	}

	// log doesn't exist, create
	MESSAGE_LOG* pNewLog = new MESSAGE_LOG(nLogIdx);
	pNewLog->PushMessage(pMessage);

	g_pMessageLogs.PushTail(pNewLog);
}

//-----------------------------------------------------------------------------
//	Name:		PushMessage
//	Object:		
//	06-06-13:	VHA - Created
//-----------------------------------------------------------------------------
void	MESSAGE_LOG::PushMessage(const char*	pMessage)
{
	(*_fileStream) << pMessage;
}

//-----------------------------------------------------------------------------
//	Name:		SetActiveState
//	Object:		
//	06-06-13:	VHA - Created
//-----------------------------------------------------------------------------
void	MESSAGE_LOG::SetActiveState(Bool	bNewState)
{
	_bIsActive = bNewState;
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
