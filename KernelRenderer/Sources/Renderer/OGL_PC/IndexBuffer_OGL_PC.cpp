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
//	CLASS:	INDEX_BUFFER_OGL_PC
//
//	05-09-15:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include "IndexBuffer_OGL_PC.h"	
#include "Error.h"	
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)
#include "GLExtensions.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		INDEX_BUFFER_OGL_PC constructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
INDEX_BUFFER_OGL_PC::INDEX_BUFFER_OGL_PC(UInt32	nNbrIndices)
:
_nNbrIndices(nNbrIndices),
_bLocked(false)
{
	glGenBuffersARB(1, &_nOGLIndexBuffer);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _nOGLIndexBuffer);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nNbrIndices * sizeof(GLuint), NULL, GL_STATIC_DRAW_ARB);
	QDT_TEST_GL_ERROR();
	RENDER_CONTEXT::GetRenderStats().AddIndexBuffer(1);
}

//-----------------------------------------------------------------------------
//	Name:		~INDEX_BUFFER_OGL_PC destructor
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
INDEX_BUFFER_OGL_PC::~INDEX_BUFFER_OGL_PC()
{
	glDeleteBuffersARB(1, &_nOGLIndexBuffer);
	RENDER_CONTEXT::GetRenderStats().AddVertexBuffer(-1);
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void *	INDEX_BUFFER_OGL_PC::Lock()
{
	QDT_ASSERT(_bLocked == false);

	_bLocked = true;
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _nOGLIndexBuffer);
	void *ptr = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddIndexBufferLock(1);
	
	return (ptr);
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
void	INDEX_BUFFER_OGL_PC::Unlock()
{
	QDT_ASSERT(_bLocked == true);
	
	_bLocked = false;
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _nOGLIndexBuffer);
	GLboolean bRes = glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
	QDT_ASSERT(bRes == GL_TRUE);

	QDT_TEST_GL_ERROR();
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrIndices
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	INDEX_BUFFER_OGL_PC::GetNbrIndices() const
{
	return _nNbrIndices;
}

//-----------------------------------------------------------------------------
//	Name:		IsDynamic
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
Bool	INDEX_BUFFER_OGL_PC::IsDynamic() const
{
	return _bDynamic;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
Bool	INDEX_BUFFER_OGL_PC::IsLocked() const
{
	return _bLocked;
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLIndexBuffer
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------
GLuint	INDEX_BUFFER_OGL_PC::GetOGLIndexBuffer() const
{
	return _nOGLIndexBuffer;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
