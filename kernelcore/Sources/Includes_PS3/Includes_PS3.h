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
//	File	:	Facade.h
//	Category:	Renderer\Sources\Includes_PC
//
//	05-08-30:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#if defined (PLATFORM_PS3)

#ifdef QDT_INCLUDE_PS3_PSGL
	#include <psgl\PSGL.h>
#endif
#undef QDT_INCLUDE_PS3_PSGL

#ifdef QDT_INCLUDE_PS3_PSGLU
	#include <psgl\PSGLU.h>
#endif
#undef QDT_INCLUDE_PS3_PSGLU

#ifdef QDT_INCLUDE_PS3_GCM
	#include <cell\gcm.h>
#endif
#undef QDT_INCLUDE_PS3_GCM

#ifdef QDT_INCLUDE_PS3_DEBUGFONT
	#include <cell/DbgFont.h>
#endif
#undef QDT_INCLUDE_PS3_DEBUGFONT

#ifdef QDT_INCLUDE_PS3_PAD
	#include <cell/pad.h>
#endif
#undef QDT_INCLUDE_PS3_PAD

#ifdef QDT_INCLUDE_PS3_FS_FILE_API
	#include <cell/fs/cell_fs_file_api.h>
#endif
#undef QDT_INCLUDE_PS3_FS_FILE_API

#ifdef QDT_INCLUDE_PS3_SPURS
	#include <cell/spurs/control.h>
	#include <cell/spurs/task.h>
#endif
#undef QDT_INCLUDE_PS3_SPURS

#ifdef QDT_INCLUDE_PS3_SPU_INITIALIZE
	#include <sys/spu_initialize.h>
#endif
#undef QDT_INCLUDE_PS3_SPU_INITIALIZE

#ifdef QDT_INCLUDE_PS3_NETWORK_SYS_SOCKET
	#include <sys/socket.h>
#endif
#undef QDT_INCLUDE_PS3_NETWORK_SYS_SOCKET

#ifdef QDT_INCLUDE_PS3_PPU_THREAD
	#include <sys/ppu_thread.h>
#endif
#undef QDT_INCLUDE_PS3_PPU_THREAD

#ifdef QDT_INCLUDE_PS3_SPU_THREAD
	#include <sys/spu_thread.h>
#endif
#undef QDT_INCLUDE_PS3_SPU_THREAD

#ifdef QDT_INCLUDE_PS3_SYSUTIL
	#include <sysutil/sysutil_common.h>
	#include <sysutil/sysutil_sysparam.h>
#endif
#undef QDT_INCLUDE_PS3_SYSUTIL

#ifdef QDT_INCLUDE_PS3_MULTISTREAM
#define USE_MSTREAM
#ifdef USE_MSTREAM
	#include <cell/mstream.h>
#endif
#endif
#undef QDT_INCLUDE_PS3_MULTISTREAM

#ifdef QDT_INCLUDE_PS3_MATH
#include	QDT_DISABLE_FORCE_USE_MATH
//#define		_VECTORMATH_NO_SCALAR_CAST
#include	<vectormath/cpp/vectormath_aos.h>
using namespace	Vectormath::Aos;
#include	QDT_FORCE_USE_MATH
#endif
#undef QDT_INCLUDE_PS3_MATH

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
