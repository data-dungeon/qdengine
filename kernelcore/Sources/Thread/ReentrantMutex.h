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
//	CLASS:	REENTRANT_MUTEXT
//	A reentrant mutex is a mutual exclusion mechanism. In a reentrant mutex, the 
//	same thread can acquire the lock multiple times. However, the lock must be 
//	released the same number of times or else other threads will be unable 
//	to acquire the lock.
//
//	06-03-07:	GRI - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __REENTRANT_MUTEX_H__
#define __REENTRANT_MUTEX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifdef	PLATFORM_PC
#define QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)
#define		QDT_INCLUDE_PC_WINDOWS
#include	INCL_KCORE(Includes_PC/Includes_PC)
#elif	PLATFORM_PS3
#include <sys/synchronization.h>
#endif

class	REENTRANT_MUTEX
{
private:
#ifdef	PLATFORM_PC
	CRITICAL_SECTION lock;
#elif	PLATFORM_PS3
	sys_mutex_t lock;
#endif

public:
	REENTRANT_MUTEX()
	{
#ifdef	PLATFORM_PC
		InitializeCriticalSection(&lock);
#elif	PLATFORM_PS3
		sys_mutex_attribute_t mut_attr;
		sys_mutex_attribute_initialize(mut_attr);
		mut_attr.attr_recursive = SYS_SYNC_RECURSIVE;
		int ret = sys_mutex_create(&lock, &mut_attr);
		QDT_ASSERT(ret == CELL_OK);
#endif
	}

	~REENTRANT_MUTEX()
	{
#ifdef	PLATFORM_PC
		DeleteCriticalSection(&lock);
#elif	PLATFORM_PS3
		int ret = sys_mutex_destroy(lock);
		QDT_ASSERT(ret == CELL_OK);
#endif
	}

	void Lock()
	{
#ifdef	PLATFORM_PC
		EnterCriticalSection(&lock);
#elif	PLATFORM_PS3
		int ret = sys_mutex_lock(lock,0);
		QDT_ASSERT(ret == CELL_OK);
		// @gri : use that to place a breakpoint on PS3.
		//static int toto;
		//if (ret != CELL_OK)
		//{
		//	toto++;
		//}
#endif
	}

	void Unlock()
	{
#ifdef	PLATFORM_PC
		LeaveCriticalSection(&lock);
#elif	PLATFORM_PS3
		int ret = sys_mutex_unlock(lock);
		QDT_ASSERT(ret == CELL_OK);
#endif
	}
};


#endif // __MUTEX_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
