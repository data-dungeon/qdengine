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

#ifdef QDT_INCLUDE_STDIO
	#include <stdio.h>
#endif
#undef QDT_INCLUDE_STDIO

#ifdef QDT_INCLUDE_STDARG
	#include <stdarg.h>
#endif
#undef QDT_INCLUDE_STDARG

#ifdef QDT_INCLUDE_STRING
	#include <string.h>
#endif
#undef QDT_INCLUDE_STRING

#ifdef QDT_INCLUDE_STDLIB
	#include <stdlib.h>
#endif
#undef QDT_INCLUDE_STDLIB

#ifdef QDT_INCLUDE_MEMORY
	#if (defined (PLATFORM_PS3) && !(defined(SPU))) || (!defined (PLATFORM_PS3))
		#include <memory.h> // memory.h not available on SPU
	#endif

	#if defined (PLATFORM_PS3)
		#include <string.h> // Some of the usual memcpy are in this file on PS3
	#endif
#endif
#undef QDT_INCLUDE_MEMORY

#ifdef QDT_INCLUDE_MATH
	#include <math.h>
#endif
#undef QDT_INCLUDE_MATH

#ifdef QDT_INCLUDE_LIMITS
	#include <limits.h>
#endif
#undef QDT_INCLUDE_LIMITS

#ifdef QDT_INCLUDE_FLOAT
	#include <float.h>
#endif
#undef QDT_INCLUDE_FLOAT

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
