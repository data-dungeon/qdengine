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
// CLASS: VERTEX_DECLARATION_OGL_PC
//
//
//	05-12-15:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "VertexDeclaration_OGL_PC.h"	
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#define	QDT_INCLUDE_MEMORY
#include INCL_KCORE(Includes/Includes)
#include "GLExtensions.h"

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_OGL_PC Constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
VERTEX_DECLARATION_OGL_PC::VERTEX_DECLARATION_OGL_PC(S_VERTEX_ELEMENT* pElements, 
													 UInt32 nNumElements)
{
	QDT_ASSERT(nNumElements < MAX_NUM_ELEMENTS);
	_nNumElements = nNumElements;
	memcpy(&_aElements[0], pElements, _nNumElements * sizeof(S_VERTEX_ELEMENT));
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_OGL_PC Destructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
VERTEX_DECLARATION_OGL_PC::~VERTEX_DECLARATION_OGL_PC()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_OGL_PC Destructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void VERTEX_DECLARATION_OGL_PC::GetDeclaration(S_VERTEX_ELEMENT* & pElements, 
											   UInt32& nNumElements)
{
	pElements = &_aElements[0];
	nNumElements = _nNumElements;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
