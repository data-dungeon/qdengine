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

#ifndef __THREAD_THREAD_H__
#define __THREAD_THREAD_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#if (defined PLATFORM_PS3)
	#define		QDT_INCLUDE_PS3_PPU_THREAD
	#include	INCL_KCORE(Includes_PS3/Includes_PS3)
#endif

#if		(defined PLATFORM_PC) || (defined PLATFORM_X360)
	#define	DECLARE_THREAD_FUNCTION(name, param)	unsigned long __stdcall name(void* param)
	#define EXIT_THREAD_FUNCTION()					return (0);
#elif	(defined PLATFORM_PS3)
	#define	DECLARE_THREAD_FUNCTION(name, param)	void		 name(UInt64 param)
	#define EXIT_THREAD_FUNCTION()					return;
#endif

class	THREAD
{
public:

	#if		(defined PLATFORM_PC) || (defined PLATFORM_X360)
		typedef unsigned long  ID;
		typedef unsigned long (__stdcall *THREAD_FUNCTION)(void* lpParameter);
	#elif	(defined PLATFORM_PS3)
		typedef sys_ppu_thread_t ID;
		typedef void	(*THREAD_FUNCTION)(UInt64 lpParameter);
	#endif

	static	ID		Create( THREAD_FUNCTION ThreadFunction, IntPtr nUserData, Char* Name, Int32 nPriority);
	static	ID		GetCurrentThreadId();
	static	void	Suspend(ID nId);
	static	void	Resume(ID nId);
	static	void	Sleep(UInt32 nDurationinMs);
	
};

#endif // __THREAD_THREAD_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
