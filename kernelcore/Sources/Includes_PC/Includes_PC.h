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

#if defined (PLATFORM_PC)

#ifdef QDT_INCLUDE_PC_IO
	#include <io.h>
#endif
#undef QDT_INCLUDE_PC_IO

#ifdef QDT_INCLUDE_PC_TIME
	#include <time.h>
#endif
#undef QDT_INCLUDE_PC_TIME

#ifdef QDT_INCLUDE_PC_WINDOWS
	#define _WINSOCKAPI_
	#include <windows.h>
#endif
#undef QDT_INCLUDE_PC_WINDOWS

#ifdef QDT_INCLUDE_PC_D3D9
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
#endif
#undef QDT_INCLUDE_PC_D3D9

#ifdef QDT_INCLUDE_PC_D3DX9
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
#undef QDT_INCLUDE_PC_D3DX9

#ifdef QDT_INCLUDE_PC_DSOUND
	#include <dsound.h>
#endif
#undef QDT_INCLUDE_PC_DSOUND

#ifdef QDT_INCLUDE_PC_DINPUT
	#define DIRECTINPUT_VERSION 0x0800
	#include <dinput.h>
#endif
#undef QDT_INCLUDE_PC_DINPUT

#ifdef QDT_INCLUDE_PC_FPIEEE
	// Exceptions
	#include <fpieee.h>
#endif
#undef QDT_INCLUDE_PC_FPIEEE

#ifdef QDT_INCLUDE_PC_WINSOCK
	#include <winsock2.h>
#endif
#undef QDT_INCLUDE_PC_WINSOCK

#ifdef QDT_INCLUDE_PC_GL
	#include "gl/gl.h"
#endif
#undef QDT_INCLUDE_PC_GL

#ifdef QDT_INCLUDE_PC_GLAUX
	#include "gl/glAux.h"
#endif
#undef QDT_INCLUDE_PC_GLAUX

#ifdef QDT_INCLUDE_PC_GLEXT
	#include "gl/glext.h"
#endif
#undef QDT_INCLUDE_PC_GLEXT

#ifdef QDT_INCLUDE_PC_GLU
	#include "gl/glu.h"
#endif
#undef QDT_INCLUDE_PC_GLU

#ifdef QDT_INCLUDE_PC_GLUT
	#include "gl/glut.h"
#endif
#undef QDT_INCLUDE_PC_GLUT

#ifdef QDT_INCLUDE_PC_GLXEXT
	#include "gl/glxext.h"
#endif
#undef QDT_INCLUDE_PC_GLXEXT

#ifdef QDT_INCLUDE_PC_WGLEXT
	#include "gl/wglext.h"
#endif
#undef QDT_INCLUDE_PC_WGLEXT

#ifdef QDT_INCLUDE_PC_GLH_EXTENSIONS
	#include "gl/glh_extensions.h"
#endif
#undef QDT_INCLUDE_PC_GLH_EXTENSIONS

#ifdef QDT_INCLUDE_PC_GLH_GENEXT
	#include "gl/glh_genext.h"
#endif
#undef QDT_INCLUDE_PC_GLH_GENEXT

#ifdef QDT_INCLUDE_PC_CG
	#include <Cg/cg.h>
	#include <Cg/cgGL.h>
#endif
#undef QDT_INCLUDE_PC_CG

#ifdef QDT_INCLUDE_WIN32_EXCEPTIONS
	#include <eh.h>
#endif
#undef QDT_INCLUDE_WIN32_EXCEPTIONS

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================


