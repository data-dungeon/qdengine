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
// CLASS: VERTEX_DECLARATION_PSGL
//
//
//	05-08-31:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "VertexDeclaration_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#define	QDT_INCLUDE_MEMORY
#include INCL_KCORE(Includes\Includes)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_PSGL Constructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_DECLARATION_PSGL::VERTEX_DECLARATION_PSGL(S_VERTEX_ELEMENT* pElements, 
												 UInt32 nNumElements)
{
	QDT_ASSERT(nNumElements < MAX_NUM_ELEMENTS);
	_nNumElements = nNumElements;
	QDT_MEMCOPY(&_aElements[0], pElements, _nNumElements * sizeof(S_VERTEX_ELEMENT));
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_PSGL Destructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_DECLARATION_PSGL::~VERTEX_DECLARATION_PSGL()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_DECLARATION_PSGL Destructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void VERTEX_DECLARATION_PSGL::GetDeclaration(S_VERTEX_ELEMENT* & pElements, 
											 UInt32& nNumElements)
{
	pElements = &_aElements[0];
	nNumElements = _nNumElements;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
