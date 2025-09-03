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
//	CLASS:	THREAD
//	The THREAD class implements ...
//
//	06-05-08:	VMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	"Thread.h"

#if	(defined PLATFORM_PC)
	#define		QDT_INCLUDE_PC_WINDOWS
	#include	INCL_KCORE(Includes_PC/Includes_PC)
#endif

#if	(defined PLATFORM_X360)
	#define		QDT_INCLUDE_X360_XTL
	#include	INCL_KCORE(Includes_X360/Includes_X360)
#endif

#if	(defined PLATFORM_PS3)
#include	"sys/timer.h"
#endif

//-----------------------------------------------------------------------------
//	Name:		Create
//	Object:		
//	06-05-17:	VMA - Created
//-----------------------------------------------------------------------------
THREAD::ID	THREAD::Create( THREAD_FUNCTION ThreadFunction, IntPtr nUserData, Char* Name, Int32 nPriority)
{
	#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
		ID	nThreadId;
		HANDLE hThread = CreateThread(NULL, 0, ThreadFunction,(void*) nUserData, 0, &nThreadId);
		QDT_ASSERT(hThread);
		return (nThreadId);
	#elif (defined PLATFORM_PS3)
		ID	nThreadId;
		UInt32 nRet = sys_ppu_thread_create(&nThreadId, ThreadFunction,(UInt64)nUserData, nPriority, 16*1024, SYS_PPU_THREAD_CREATE_JOINABLE, Name);
		QDT_ASSERT(nRet == CELL_OK);
		return (nThreadId);
	#endif 
}

//------------------------------------------------------++++++++------------------------
//	Name:		GetCurrentThreadId
//	Object:		
//	06-05-17:	VMA - Created
//-----------------------------------------------------------------------------
THREAD::ID	THREAD::GetCurrentThreadId()
{
	#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
	return (::GetCurrentThreadId());
	#elif (defined PLATFORM_PS3)
		ID	nThreadId;
		sys_ppu_thread_get_id(&nThreadId);
		return (nThreadId);
	#endif 
}

//-----------------------------------------------------------------------------
//	Name:		Suspend
//	Object:		
//	06-06-06:	VMA - Created
//-----------------------------------------------------------------------------
void	THREAD::Suspend(ID	nId)
{
	#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
		HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, false, nId);
		QDT_ASSERT(hThread);
		SuspendThread(hThread);
		CloseHandle(hThread);
	#elif (defined PLATFORM_PS3)
		sys_ppu_thread_stop(nId);
	#endif
}

//-----------------------------------------------------------------------------
//	Name:		Resume
//	Object:		
//	06-06-06:	VMA - Created
//-----------------------------------------------------------------------------
void	THREAD::Resume(ID	nId)
{
	#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
		HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, false, nId);
		QDT_ASSERT(hThread);
		ResumeThread(hThread);
		CloseHandle(hThread);
	#elif (defined PLATFORM_PS3)
		sys_ppu_thread_restart(nId);
	#endif
}

//-----------------------------------------------------------------------------
//	Name:		Sleep
//	Object:		
//	06-06-06:	VMA - Created
//-----------------------------------------------------------------------------
void	THREAD::Sleep(UInt32	nDurationinMs)
{
	#if (defined PLATFORM_PC) || (defined PLATFORM_X360)
		::Sleep(nDurationinMs);					// in ms on PC
	#elif (defined PLATFORM_PS3)
		sys_timer_usleep(nDurationinMs*1000);	// we are in µs microsecond in PS3
	#endif
}
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
