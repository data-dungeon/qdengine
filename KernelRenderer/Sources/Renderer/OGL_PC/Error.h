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
//	File	:	Message.h
//	Category:	Renderer\Sources\Renderer\PSGL
//
//	05-09-29:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_ERROR_H__
#define __KRENDERER_RENDERER_OGL_PC_ERROR_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#ifdef QDT_DEBUG
	#define QDT_TEST_GL_ERROR() QDT_TestGLError(__FILE__, __LINE__);
	#define QDT_TEST_CG_ERROR() QDT_TestCgError(__FILE__, __LINE__);	
#else
	#define QDT_TEST_GL_ERROR()
	#define QDT_TEST_CG_ERROR()
#endif

namespace	QDT
{
	namespace	KRENDERER
	{
		void QDT_TestGLError(Char * sFileName, UInt32 nLineNumber);
		void QDT_TestCgError(Char * sFileName, UInt32 nLineNumber);
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_ERROR_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
