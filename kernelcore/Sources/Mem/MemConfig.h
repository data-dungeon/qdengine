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
//	Memory manager configuration file
//
//	06-02-09:	VHA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __MEM_CONFIG_H__
#define __MEM_CONFIG_H__

// general switch:
//////////////////////////////////////////////////
//#ifdef PLATFORM_PC // TODO => PS3
	#define QDT_MEM_ENABLED
	#define QDT_MEM_THREAD_SAFE
	#define QDT_MEM_BIGMEM_USE_MULTIPLE_CHUNKS

#ifdef QDT_DEBUG
/*	#define QDT_MEM_DEBUG

	#define QDT_MEM_USE_EXTRA_DEBUG_INFO
	#define QDT_MEM_USE_CORRUPTION_PROTECTION
	#define QDT_MEM_USE_CALLSTACK
	#define QDT_MEM_USE_TIME_ID */
	//#define QDT_MEM_USE_ARRAY_PROTECTION // Ensure that you only use delete[] on new[]
#endif
//#endif

//////////////////////////////////////////////////

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
#define QDT_MEM_USE_MEMPOOL_CORRUPTION_PROTECTION
#define QDT_MEM_USE_BIGMEM_CORRUPTION_PROTECTION
#endif

#ifdef QDT_MEM_USE_CALLSTACK
#define QDT_MEM_USE_MEMPOOL_CALLSTACK
#define QDT_MEM_USE_BIGMEM_CALLSTACK
#endif

#ifdef QDT_MEM_USE_TIME_ID
#define QDT_MEM_USE_MEMPOOL_TIME_ID
#define QDT_MEM_USE_BIGMEM_TIME_ID
#endif

#ifdef QDT_MEM_USE_MEMPOOL_CALLSTACK
#define QDT_MEM_USE_EXTRA_DEBUG_INFO
#endif

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
#define QDT_MEM_USE_EXTRA_DEBUG_INFO
#endif

#ifdef QDT_MEM_USE_MEMPOOL_TIME_ID
#define QDT_MEM_USE_EXTRA_DEBUG_INFO
#define QDT_MEM_USE_TIME_ID
#endif

#ifdef QDT_MEM_USE_BIGMEM_TIME_ID
#define QDT_MEM_USE_EXTRA_DEBUG_INFO
#define QDT_MEM_USE_TIME_ID
#endif

#endif // __MEM_CONFIG_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
