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
//	CLASS:	DEFINE
//	The DEFINE class implements ...
//
//	01-11-12:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_CORE_TYPES_H__
#define __KCORE_CORE_TYPES_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		#if defined (PLATFORM_PC)
			typedef char				Char;
			typedef char				Int8;
			typedef short				Int16;
			typedef int					Int32;
			typedef __int64				Int64;
			typedef unsigned char		UInt8;
			typedef unsigned char		UChar;
			typedef unsigned short		UInt16;
			typedef unsigned int		UInt32;
			typedef unsigned __int64	UInt64;
			typedef float				Float32;
			typedef double				Float64;
			typedef bool				Bool;
			typedef UInt32				IntPtr;
		#elif defined (PLATFORM_PS3)
			typedef char				Char;
			typedef char				Int8;
			typedef short				Int16;
			typedef int					Int32;
			typedef long long			Int64;
			typedef unsigned char		UInt8;
			typedef unsigned char		UChar;
			typedef unsigned short		UInt16;
			typedef unsigned int		UInt32;
			typedef unsigned long long	UInt64;
			typedef float				Float32;
			typedef double				Float64;
			typedef bool				Bool;
			typedef UInt32				IntPtr;
		#elif defined (PLATFORM_X360)
			typedef char				Char;
			typedef char				Int8;
			typedef short				Int16;
			typedef int					Int32;
			typedef __int64				Int64;
			typedef unsigned char		UInt8;
			typedef unsigned char		UChar;
			typedef unsigned short		UInt16;
			typedef unsigned int		UInt32;
			typedef unsigned __int64	UInt64;
			typedef float				Float32;
			typedef double				Float64;
			typedef bool				Bool;
			typedef UInt32				IntPtr;
		#else	
			// PLATFORM Undefined!
			#error
		#endif
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
