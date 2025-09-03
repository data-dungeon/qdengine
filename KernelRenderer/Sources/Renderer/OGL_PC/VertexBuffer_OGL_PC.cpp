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
// CLASS: VERTEX_BUFFER_OGL_PC
//
//
//	05-12-15:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "VertexBuffer_OGL_PC.h"
#include "Error.h"
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)
#include "GLExtensions.h"

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_OGL_PC Constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
VERTEX_BUFFER_OGL_PC::VERTEX_BUFFER_OGL_PC(UInt32 vertexCount, 
										   UInt32 vertexStride, 
										   Bool bDynamic)
:
_nOGLBufferIndex(0),
_nVertexCount(vertexCount),
_nVertexStride(vertexStride),
_bLocked(false)
{	
	glGenBuffersARB(1, &_nOGLBufferIndex);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _nOGLBufferIndex);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, _nVertexCount * _nVertexStride, NULL, GL_STATIC_DRAW_ARB);
	RENDER_CONTEXT::GetRenderStats().AddVertexBuffer(1);
}

//-----------------------------------------------------------------------------
//	Name:		VERTEX_BUFFER_OGL_PC Destructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
VERTEX_BUFFER_OGL_PC::~VERTEX_BUFFER_OGL_PC()
{
	glDeleteBuffersARB(1, &_nOGLBufferIndex);
	QDT_TEST_GL_ERROR();
	RENDER_CONTEXT::GetRenderStats().AddVertexBuffer(-1);
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void* VERTEX_BUFFER_OGL_PC::Lock()
{
	QDT_ASSERT(_bLocked == false);

	_bLocked = true;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _nOGLBufferIndex);
	QDT_TEST_GL_ERROR();

	void *ptr = glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVertexBufferLock(1);

	return (ptr);
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void VERTEX_BUFFER_OGL_PC::Unlock()
{
	QDT_ASSERT(_bLocked == true);
	
	_bLocked = false;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _nOGLBufferIndex);
	GLboolean bRes = glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	QDT_ASSERT(bRes == GL_TRUE);
	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexCount
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 VERTEX_BUFFER_OGL_PC::GetVertexCount() const
{
	return (_nVertexCount);
}

//-----------------------------------------------------------------------------
//	Name:		GetVertexStride
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 VERTEX_BUFFER_OGL_PC::GetVertexStride() const
{
	return (_nVertexStride);
}

//-----------------------------------------------------------------------------
//	Name:		IsDynamic
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
Bool VERTEX_BUFFER_OGL_PC::IsDynamic() const
{
	return (_bDynamic);
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
Bool VERTEX_BUFFER_OGL_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLIndex
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
GLuint VERTEX_BUFFER_OGL_PC::GetOGLIndex() const
{
	return (_nOGLBufferIndex);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
