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
//	The MESSAGE_LOG class implements ...
//
//	06-06-12:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MESSAGE_LOG_H__
#define __MESSAGE_LOG_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCORE(Template/QDT_Vector)
#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(Stream/FileTextStream)

#ifndef _MASTER

class	MESSAGE_LOG
{
public:
	MESSAGE_LOG(UInt32 logIdx);
	~MESSAGE_LOG();

	static void Release();
	static void PushMessageToLogIdx(const char*	pMessage, UInt32 nLogIdx = 0);

	void PushMessage(const char*	pMessage);
	UInt32 GetLogIdx() { return _nLogIdx; }

	void SetActiveState(Bool bNewState);

private:
	static QDT_VECTOR<MESSAGE_LOG*> g_pMessageLogs;

	FILE_TEXT_STREAM* _fileStream;
	Bool _bIsActive;
	UInt32 _nLogIdx;
};

#endif

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
