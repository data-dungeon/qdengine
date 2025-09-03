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
//	Category:	3DEngine\Sources\SceneGraph\Texture
//
//	05-11-09:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	__3DENGINE_SOURCES_SCENEGRAPH_TEXTURE_FACADE_H__
	#define	__3DENGINE_SOURCES_SCENEGRAPH_TEXTURE_FACADE_H__

	#include	"..\Facade.h"
	#include	DEC_KRNRENDERER(Renderer)

	namespace	QDT
	{
		namespace	M3D
		{
			#include "EngineTextureABC.h"
			#include "EngineTextureFile.h"
		}
	}

	#ifdef _DISPLAY_TRACE_FACADE_INCLUDE_
		#pragma QDT_TRACE_FACADE_INCLUDE()
	#endif	//	_DISPLAY_TRACE_FACADE_INCLUDE_

#endif	//	__3DENGINE_SOURCES_SCENEGRAPH_TEXTURE_FACADE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
