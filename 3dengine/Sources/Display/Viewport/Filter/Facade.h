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
//	Category:	IAMAPPne\Sources\Display\Viewport\Filter\
//
//	04-01-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_DISPLAY_VIEWPORT_FILTER_FACADE_H__
	#define	__3DENGINE_SOURCES_DISPLAY_VIEWPORT_FILTER_FACADE_H__

	#include	"..\Facade.h"
	
	#include	INC_3DENGINE(SceneGraph/Material)

	#ifdef PLATFORM_PS2			// PATCH PS2 Filtre en multiview => Pb de GEOMETRY dynamic ds le moteur, ca evite de tt saloper...
		#define DISPLAY_FILTER_NBVIEWPORT_MAX		3
	#else
		#define DISPLAY_FILTER_NBVIEWPORT_MAX		1
	#endif

	namespace	QDT
	{
		namespace	M3D
		{
			#include	"FilterLayer.h"
			#include	"FilterScratch.h"
			#include	"Filter.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_DISPLAY_VIEWPORT_FILTER_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
