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

#ifndef __MEM_CONFIG_INTERFACE_H__
#define __MEM_CONFIG_INTERFACE_H__

enum MEM_CONFIG_STATE
{
	mem_disabled = 0,
	mem_enabled = 1,
	mem_inherit,
};

struct MEM_CONFIG
{
	MEM_CONFIG_STATE _bIsActive;
};

extern MEM_CONFIG g_MemConfig;

#ifdef QDT_MEM_DEBUG
const static UInt32 _iMaxFreeTrace = 10;
#endif

const static UInt32 _iMaxStackDepth = 8;

// MemPool config:
const static UInt32 _iPoolDestructionTreshold = 10; // Aka 10% free
const static UInt32 _iMaxSizeMPAlloc = 1024;
const static UInt32 _iStepSizeMPAlloc = 16;
const static UInt32 _iNumAdditionalPool = 1024;
const static UInt32 _iTotalPoolSize = 64*1024-16;

#ifdef QDT_MEM_USE_EXTRA_DEBUG_INFO
const static UInt32 _iPoolExtraDebugSize = _iMaxStackDepth * 4 + 32;
#else
const static UInt32 _iPoolExtraDebugSize = 0;
#endif

#ifdef QDT_MEM_USE_CORRUPTION_PROTECTION
const static UInt32 _iPoolCorruptionMarkerSize = 16;
//const static char _cPoolCorruptionMarkerString[] = "ABCDEFGHIJKLMNOP";
const static char _cPoolCorruptionMarkerString[] = "LocoRocoMujaMuja";
#else
const static UInt32 _iPoolCorruptionMarkerSize = 0; // to avoid #ifdefs everywhere in the code
#endif

// BigMem config:
#ifdef PLATFORM_PC
#define QDT_BIGMEM_DEFAULT_SIZE (128*1024*1024)
#else
#define QDT_BIGMEM_DEFAULT_SIZE (128*1024*1024)
#endif


const static UInt32	_iBigChunkDestructionTreshold = 10; // Aka 10% free
#ifdef QDT_MEM_ENABLED
#ifdef PLATFORM_PC
const static UInt32 _iBigChunkSize = 128*1024*1024;
#else
const static UInt32 _iBigChunkSize = 128*1024*1024;
#endif
#else
const static UInt32 _iBigChunkSize = 1*1024*1024;
#endif
const static UInt32 _iBigChunkAllocationAllignement = 16;
//#define QDT_MEM_USE_BIGMEM_CORRUPTION_PROTECTION

#ifdef QDT_MEM_USE_BIGMEM_CORRUPTION_PROTECTION
const static UInt32 _iBigCorruptionMarkerSize = 16;
//const static char _cCorruptionMarkerString[] = "ABCDEFGHIJKLMNOP";
const static char _cCorruptionMarkerString[] = "Kanar à l'orange";
#endif

//////////////////////////
#ifdef QDT_MEM_USE_MEMPOOL_CALLSTACK
#ifndef QDT_MEM_DEBUG
#error You must enable QDT_MEM_DEBUG to be able to define QDT_MEM_USE_MEMPOOL_CALLSTACK
#endif
#endif

#ifdef QDT_MEM_USE_BIGMEM_CALLSTACK
#ifndef QDT_MEM_DEBUG
#error You must enable QDT_MEM_DEBUG to be able to define QDT_MEM_USE_BIGMEM_CALLSTACK
#endif
#endif

#ifdef QDT_MEM_USE_MEMPOOL_TIME_ID
#ifndef QDT_MEM_DEBUG
#error You must enable QDT_MEM_DEBUG to be able to define QDT_MEM_USE_MEMPOOL_TIME_ID
#endif
#endif

#ifdef QDT_MEM_USE_BIGMEM_TIME_ID
#ifndef QDT_MEM_DEBUG
#error You must enable QDT_MEM_DEBUG to be able to define QDT_MEM_USE_BIGMEM_TIME_ID
#endif
#endif

#endif //__MEM_CONFIG_INTERFACE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
