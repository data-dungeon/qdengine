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

#include	"Root.h"
#include	"Mutex.h"

#include	INCL_KCORE(Mem/MemPoolManager)

#ifdef	PLATFORM_PC
	#define		QDT_INCLUDE_PC_WINDOWS
	#include	INCL_KCORE(Includes_PC/Includes_PC)
#endif

#ifdef	PLATFORM_X360
	#define		QDT_INCLUDE_X360_XTL
	#include	INCL_KCORE(Includes_X360/Includes_X360)
#endif

//-----------------------------------------------------------------------------
//	Name:		MUTEX constructor
//	Object:		
//	06-05-18:	VHA - Created
//-----------------------------------------------------------------------------
MUTEX::MUTEX(Bool	bIsRecursive)
{
#if		(defined	PLATFORM_PC) || (defined	PLATFORM_X360)
	#ifdef QDT_MEM_ENABLED
		QDT_ASSERT(g_MemConfig._bIsActive != mem_inherit);
		if(g_MemConfig._bIsActive == mem_enabled)
		{
			_lock = MEM::GetInstance()->_pMemPoolManager->Allocate(sizeof(CRITICAL_SECTION));
		}
		else
		{
			_lock = malloc(sizeof(CRITICAL_SECTION));
		}
	#else
		_lock = new CRITICAL_SECTION;
	#endif
	InitializeCriticalSection((CRITICAL_SECTION*)_lock);
#elif	(defined PLATFORM_PS3)
	sys_mutex_attribute_t mut_attr;
	sys_mutex_attribute_initialize(mut_attr);
	mut_attr.attr_recursive = (bIsRecursive) ? SYS_SYNC_RECURSIVE : SYS_SYNC_NOT_RECURSIVE;
	int ret = sys_mutex_create(&_lock, &mut_attr);
	QDT_ASSERT(ret == CELL_OK);
#endif
#ifdef QDT_DEBUG
	_bIsRecursive = bIsRecursive;
#endif
}

//-----------------------------------------------------------------------------
//	Name:		~MUTEX destructor
//	Object:		
//	06-05-18:	VHA - Created
//-----------------------------------------------------------------------------
MUTEX::~MUTEX()
{
#if	(defined	PLATFORM_PC) || (defined	PLATFORM_X360)
		DeleteCriticalSection((CRITICAL_SECTION*)_lock);
		#ifdef QDT_MEM_ENABLED
			MEM::GetInstance()->Free(_lock);
		#else
			delete (CRITICAL_SECTION*)_lock;
		#endif
		#elif	PLATFORM_PS3
		int ret = sys_mutex_destroy(_lock);
		QDT_ASSERT(ret == CELL_OK);
#endif
	}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	06-05-18:	VHA - Created
//-----------------------------------------------------------------------------
void	MUTEX::Lock()
{	
#if		(defined	PLATFORM_PC) || (defined	PLATFORM_X360)
	EnterCriticalSection((CRITICAL_SECTION*)_lock);
	QDT_ASSERT(_bIsRecursive || ((CRITICAL_SECTION*)_lock)->RecursionCount<=1);
#elif	(defined PLATFORM_PS3)
	int ret = sys_mutex_lock(_lock,0);
	QDT_ASSERT(ret == CELL_OK);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	06-05-18:	VHA - Created
//-----------------------------------------------------------------------------
void	MUTEX::Unlock()
{
#if		(defined	PLATFORM_PC) || (defined	PLATFORM_X360)
	LeaveCriticalSection((CRITICAL_SECTION*)_lock);
#elif	(defined PLATFORM_PS3)
	int ret = sys_mutex_unlock(_lock);
	QDT_ASSERT(ret == CELL_OK);
#endif
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================