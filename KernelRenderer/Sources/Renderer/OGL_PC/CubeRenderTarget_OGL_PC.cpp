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
// CLASS: CUBE_RENDER_TARGET_OGL_PC
//
//
//	05-10-24:	SBE - Created
//*****************************************************************************

#include "Root.h"
#include "CubeRenderTarget_OGL_PC.h"
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#include "GLExtensions.h"

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CUBE_RENDER_TARGET_OGL_PC constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_OGL_PC::CUBE_RENDER_TARGET_OGL_PC()
{
	for (UInt32 i=0; i<6; i++)
		_uiSurface[i] = 0;

	RENDER_CONTEXT::GetRenderStats().AddCubeRenderTarget(1);
}

//-----------------------------------------------------------------------------
//	Name:		CUBE_RENDER_TARGET_OGL_PC constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_OGL_PC::CUBE_RENDER_TARGET_OGL_PC(RENDER_CONTEXT_OGL_PC*	pRenderContext,
						  UInt32					uiEdgeLength,
						  PIXEL_FORMAT				eFormat)
{
	for (UInt32 i=0; i<6; i++)
		_uiSurface[i] = 0;

	_Texture._eFormat = eFormat;
	_Texture._uiEdgeLength = uiEdgeLength;

	switch (_Texture._eFormat)
	{
		default: 
			QDT_ASSERT(0 && "TODO!!");
		case PF_R32F:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F_ARB, uiEdgeLength, uiEdgeLength, 0, GL_RGBA, GL_FLOAT, NULL);
			}
			QDT_TEST_GL_ERROR();

			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersEXT(6, _uiSurface);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface[i]);
				QDT_TEST_GL_ERROR();

				// attach texture to framebuffer object
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _Texture._gluiTextureIndex, 0);
				QDT_TEST_GL_ERROR();
			}
			break;
		}
		case PF_D24S8:
		{
			for (UInt32 i=0; i<6; i++)
			{
				glGenRenderbuffersEXT(1, &_uiSurface[i]);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _uiSurface[i]);
				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _Texture._uiEdgeLength, _Texture._uiEdgeLength);
				// pr le moment (0_5_0) impossibilite d'attacher un texture du depth buffer
				QDT_TEST_GL_ERROR();
			}
			break;
		}
		case PF_D24S8_DEPTHSTENCIL:
		{
			for (UInt32 i=0; i<6; i++)
			{
				glGenRenderbuffersEXT(1, &_uiSurface[i]);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _uiSurface[i]);
				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, _Texture._uiEdgeLength, _Texture._uiEdgeLength);
				// pr le moment (0_5_0) impossibilite d'attacher un texture du depth buffer
				QDT_TEST_GL_ERROR();
			}
			break;
		}
		case PF_A8R8G8B8:
		case PF_A16B16G16R16:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
					(_Texture._eFormat == PF_A8R8G8B8)? GL_RGBA8 : GL_RGBA16
					, uiEdgeLength, uiEdgeLength, 0, GL_RGBA, GL_FLOAT, NULL);
			}
			QDT_TEST_GL_ERROR();

			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersEXT(6, _uiSurface);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface[i]);
				QDT_TEST_GL_ERROR();

				// attach texture to framebuffer object
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _Texture._gluiTextureIndex, 0);
				QDT_TEST_GL_ERROR();
			}
			break;
		}
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	_Texture.SetAddressUType(AT_CLAMP);
	_Texture.SetAddressVType(AT_CLAMP);

	RENDER_CONTEXT::GetRenderStats().AddCubeRenderTarget(1);
}

//-----------------------------------------------------------------------------
//	Name:		~CUBE_RENDER_TARGET_OGL_PC destructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_OGL_PC::~CUBE_RENDER_TARGET_OGL_PC()
{
	if (_uiSurface > 0)
	{
		if (_Texture._eFormat == PF_D24S8_DEPTHSTENCIL)
		{
			for (UInt32 i=0; i<6; i++)
			{
				glDeleteRenderbuffersEXT(1, &_uiSurface[i]);
				_uiSurface[i] = 0;
			}
		}
		else
		{
			for (UInt32 i=0; i<6; i++)
			{
				glDeleteFramebuffersEXT(1, &_uiSurface[i]);
				_uiSurface[i] = 0;
			}
		}		
	}

	RENDER_CONTEXT::GetRenderStats().AddCubeRenderTarget(-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLTexture
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
const CUBE_TEXTURE_OGL_PC *	CUBE_RENDER_TARGET_OGL_PC::GetOGLTexture() const
{
	return &_Texture;
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLSurface
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
GLuint *	CUBE_RENDER_TARGET_OGL_PC::GetOGLSurface(CUBEMAP_FACE CF)
{
	return &_uiSurface[CF];
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_RENDER_TARGET_OGL_PC::GetWidth() const
{
	return _Texture.GetWidth();
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_RENDER_TARGET_OGL_PC::GetHeight() const
{
	return _Texture.GetHeight();
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT	CUBE_RENDER_TARGET_OGL_PC::GetPixelFormat() const
{
	return _Texture.GetPixelFormat();
}


//-----------------------------------------------------------------------------
//	Name:		ReadSurfaceContent
//	Object:		
//	06-03-07:	PDE - Created
//-----------------------------------------------------------------------------
void	CUBE_RENDER_TARGET_OGL_PC::ReadSurfaceContent(CUBEMAP_FACE	CF,
													  void *		buffer, 
													  Bool			bReturnFloat)
{
	RENDER_CONTEXT::SetCurrentRenderTarget(this, 0, CF);

	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT); // FBO version
	QDT_TEST_GL_ERROR();

	GLenum format;

	if (bReturnFloat == false)
	{
		if (GetPixelFormat() == PF_A8R8G8B8)
			format = GL_UNSIGNED_BYTE;
		else
			format = GL_UNSIGNED_SHORT;
	}
	else
	{
		format = GL_FLOAT;
	}

	glReadPixels(0, 0, GetWidth(), GetHeight(), GL_RGBA, format, buffer);
	QDT_TEST_GL_ERROR();
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
