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
//	File	:	Include.h
//	Category:	3DEngine\Sources\
//
//	01-07-16:	ONA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifdef PLATFORM_XBOX
#pragma bss_seg(".m3d_bss")
#endif

#ifndef	__3DENGINE_SOURCES_INCLUDE_H__
	#ifndef	_DEBUG
		#define	__3DENGINE_SOURCES_INCLUDE_H__
	#else
		//	We define it only in release to avoid inline compilation problems
		//	But as in _DEBUG we don't have this kind of problem
		//	We don't define it to ensure a compilation error with double incusion !
	#endif

	#include	"Facade.h"
	#include	"..\QDT_BuildInfo.h"

	using namespace	QDT::M3D;

#endif	//	__3DENGINE_SOURCES_INCLUDE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
