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
//	CLASS:	INDEX_BUFFER_PSGL
//
//	05-09-15:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "IndexBuffer_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INDEX_BUFFER_PSGL constructor
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
INDEX_BUFFER_PSGL::INDEX_BUFFER_PSGL(UInt32	nNbrIndices)
:
_nNbrIndices(nNbrIndices),
_bLocked(false)
{
	glGenBuffers(1, &_nPSGLIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _nPSGLIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nNbrIndices * sizeof(UInt16), NULL, GL_STATIC_DRAW);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddIndexBuffer(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfIndexBuffers(nNbrIndices * sizeof(UInt16));
}

//-----------------------------------------------------------------------------
//	Name:		~INDEX_BUFFER_PSGL destructor
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
INDEX_BUFFER_PSGL::~INDEX_BUFFER_PSGL()
{
	glDeleteBuffers(1, &_nPSGLIndexBuffer);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddIndexBuffer(-1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfIndexBuffers(- _nNbrIndices * sizeof(UInt16));
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
void *	INDEX_BUFFER_PSGL::Lock()
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == false);

	_bLocked = true;
	glBindBuffer(GL_ARRAY_BUFFER, _nPSGLIndexBuffer);
	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddIndexBufferLock(1);

	return (ptr);
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
void	INDEX_BUFFER_PSGL::Unlock()
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == true);
	
	_bLocked = false;
	glBindBuffer(GL_ARRAY_BUFFER, _nPSGLIndexBuffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndices
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	INDEX_BUFFER_PSGL::GetNbrIndices() const
{
	return _nNbrIndices;
}

//-----------------------------------------------------------------------------
//	Name:		IsDynamic
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
Bool	INDEX_BUFFER_PSGL::IsDynamic() const
{
	return _bDynamic;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
Bool	INDEX_BUFFER_PSGL::IsLocked() const
{
	return _bLocked;
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLIndexBuffer
//	Object:		
//	05-09-15:	SBE - Created
//-----------------------------------------------------------------------------
GLuint	INDEX_BUFFER_PSGL::GetPSGLIndexBuffer() const
{
	return _nPSGLIndexBuffer;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
