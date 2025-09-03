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
// CLASS: VERTEX_BUFFER_PSGL
//
//
//	05-08-31:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "VertexBuffer_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_PSGL Constructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_BUFFER_PSGL::VERTEX_BUFFER_PSGL(UInt32 vertexCount, 
									   UInt32 vertexStride, 
									   Bool bDynamic)
:
_nPSGLBufferIndex(0),
_nVertexCount(vertexCount),
_nVertexStride(vertexStride),
_bLocked(false)
{	
	glGenBuffers(1, &_nPSGLBufferIndex);
	glBindBuffer(GL_ARRAY_BUFFER, _nPSGLBufferIndex);
	glBufferData(GL_ARRAY_BUFFER, _nVertexCount * _nVertexStride, NULL, GL_STATIC_DRAW);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVertexBuffer(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfVertexBuffers(_nVertexCount * _nVertexStride);				
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_PSGL Destructor
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
VERTEX_BUFFER_PSGL::~VERTEX_BUFFER_PSGL()
{
	glDeleteBuffers(1, &_nPSGLBufferIndex);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVertexBuffer(-1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfVertexBuffers(- _nVertexCount * _nVertexStride);				
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void* VERTEX_BUFFER_PSGL::Lock()
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == false);

	_bLocked = true;
	glBindBuffer(GL_ARRAY_BUFFER, _nPSGLBufferIndex);
	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVertexBufferLock(1);

	return (ptr);
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
void VERTEX_BUFFER_PSGL::Unlock()
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == true);
	
	_bLocked = false;
	glBindBuffer(GL_ARRAY_BUFFER, _nPSGLBufferIndex);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexCount
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 VERTEX_BUFFER_PSGL::GetVertexCount() const
{
	return (_nVertexCount);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexStride
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 VERTEX_BUFFER_PSGL::GetVertexStride() const
{
	return (_nVertexStride);
}

//-----------------------------------------------------------------------------
//	Name:		IsDynamic
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
Bool VERTEX_BUFFER_PSGL::IsDynamic() const
{
	return (_bDynamic);
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
Bool VERTEX_BUFFER_PSGL::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLIndex
//	Object:		
//	05-08-31:	RMA - Created
//-----------------------------------------------------------------------------	
GLuint VERTEX_BUFFER_PSGL::GetPSGLIndex() const
{
	return (_nPSGLBufferIndex);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
