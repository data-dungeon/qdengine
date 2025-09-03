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
//	Category:	Renderer\Sources\Includes_X360
//
//	05-08-30:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#if defined (PLATFORM_X360)

#ifdef QDT_INCLUDE_X360_IO
	#include <io.h>
#endif
#undef QDT_INCLUDE_X360_IO

#ifdef QDT_INCLUDE_X360_TIME
	#include <time.h>
#endif
#undef QDT_INCLUDE_X360_TIME

#ifdef QDT_INCLUDE_X360_XTL
	#include	QDT_DISABLE_FORCE_USE_MATH
	#include <xtl.h>
	#include	QDT_FORCE_USE_MATH

	// NHO : I redefine this one 
	// (windows.h declares this as DWORD and an enum with the same name is declared in QDT::SINT::INPUT_MANAGER)
	//#undef DEVICE_TYPE
#endif
#undef QDT_INCLUDE_X360_XTL

#ifdef QDT_INCLUDE_X360_XGRAPHICS
	#include <xgraphics.h>
#endif
#undef QDT_INCLUDE_X360_XGRAPHICS

#ifdef QDT_INCLUDE_X360_XBDM
	#include <xbdm.h>
#endif
#undef QDT_INCLUDE_X360_XBDM

#ifdef QDT_INCLUDE_X360_D3D9
	#ifdef QDT_DEBUG
		#define D3D_DEBUG_INFO	
	#endif

	#ifdef QDT_NEW_REDEFINED
	#undef new
	#endif

	#include <d3d9.h>

	#ifdef QDT_NEW_REDEFINED
	#define new QDT_NEW_REDEFINED
	#endif

	// NHO : I undefine this one 
	// (winioctl.h declares this as DWORD and an enum with the same name is declared in QDT::SINT::INPUT)
	#undef DEVICE_TYPE
#endif
#undef QDT_INCLUDE_X360_D3D9

#ifdef QDT_INCLUDE_X360_D3DX9
	#ifdef QDT_NEW_REDEFINED
	#undef new
	#endif

	#undef sqrtf // Defined in our math.h
	#undef f32	 // Defined with bink
	#include <d3dx9.h>
	#define	sqrtf(a) NO_SQRT_LINKED_USE_MATH_Sqrt	

	#ifdef QDT_NEW_REDEFINED
	#define new QDT_NEW_REDEFINED
	#endif

#endif
#undef QDT_INCLUDE_X360_D3DX9

#ifdef QDT_INCLUDE_X360_FPIEEE
	// Exceptions
	#include <fpieee.h>
#endif
#undef QDT_INCLUDE_X360_FPIEEE

#ifdef QDT_INCLUDE_X360_WINSOCK
	#include <winsockx.h>
#endif
#undef QDT_INCLUDE_X360_WINSOCK

#ifdef QDT_INCLUDE_WIN32_EXCEPTIONS
	#include <eh.h>
#endif
#undef QDT_INCLUDE_WIN32_EXCEPTIONS

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


