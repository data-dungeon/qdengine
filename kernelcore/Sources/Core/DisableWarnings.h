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
//	05-09-21:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KCORE_CORE_DISABLE_WARNINGS_H__
#define __KCORE_CORE_DISABLE_WARNINGS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{
		#if defined (PLATFORM_PC)
			#pragma	warning(disable : 4514)			//	Désactive unreferenced inline function has been removed
			#pragma	warning(disable : 4100)			//	Désactive : "'_P' : unreferenced formal parameter"
			#pragma	warning(disable : 4244)			//	Désactive : "'=' : conversion from 'int' to 'unsigned short', possible loss of data"
			#pragma	warning(disable : 4710)			//	Désactive : "function 'virtual __thiscall TEXTURE::~TEXTURE(void)' not inlined" présent en mode RELEASE
			#pragma	warning(disable : 4711)			//	Désactive : "function 'class DDRAW_ENUM * __thiscall FOR_2D::GetDDrawEnum(int)' selected for automatic inline expansion" présent en mode RELASE
			#pragma warning(disable : 4786)			//	Désactive : "identifier was truncated to '255' characters in the browser information"
			#pragma	warning(disable : 4290)			//  Désactive : "C++ Exception Specification ignored"
			#pragma	warning(disable : 4201)			//  Désactive : "C++ Exception Specification ignored"

			#pragma	warning(error : 4129)			//	'character' : unrecognized character escape sequence
			#pragma	warning(error : 4133)			//	'type' : incompatible types - from 'type1' to 'type2'
			#pragma	warning(error : 4150)			//	deletion of pointer to incomplete type 'type'; no destructor called
			#pragma	warning(error : 4506)			//	no definition for inline function 'function'
			#pragma	warning(error : 4706)			//	assignment within conditional expression
			#pragma	warning(error : 4715)			//	'function' : not all control paths return a value
		#endif // PLATFORM_PC

		#if defined (PLATFORM_PS3)
			#pragma warning(disable : 80)
		#endif

#pragma message("---------------------- toto ----------------------------")
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
