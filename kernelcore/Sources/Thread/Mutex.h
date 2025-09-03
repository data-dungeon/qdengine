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
//	CLASS:	MUTEX
//	The MUTEX class implements ...
//
//	06-03-07:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __THREAD_MUTEX_H__
#define __THREAD_MUTEX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifdef	PLATFORM_PS3
	#include <sys/synchronization.h>
#endif

class	MUTEX
{
private:
#if		(defined	PLATFORM_PC) || (defined	PLATFORM_X360)
	void* _lock;
#elif	(defined PLATFORM_PS3)
	sys_mutex_t _lock;
#endif
#ifdef QDT_DEBUG
	Bool	_bIsRecursive;
#endif

public:
	MUTEX(Bool	bIsRecursive = false);
	~MUTEX();

	void Lock();
	void Unlock();
};

class	AUTO_LOCK
{
public:
	AUTO_LOCK(MUTEX* pMutex)
	{
		_pMutex = pMutex;
		_pMutex->Lock();
	}

	~AUTO_LOCK()
	{
		_pMutex->Unlock();
	}

	MUTEX* _pMutex;
};

#endif // __THREAD_MUTEX_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
