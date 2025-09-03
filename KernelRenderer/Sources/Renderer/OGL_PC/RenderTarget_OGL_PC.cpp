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
// CLASS: RENDER_TARGET_OGL_PC
//
//
//	05-12-19:	SBE - Created
//*****************************************************************************

#include "Root.h"
#include "RenderTarget_OGL_PC.h"	
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GLEXT
#define QDT_INCLUDE_PC_WGLEXT
#include INCL_KCORE(Includes_PC/Includes_PC)
#include INCL_KRENDERER(Renderer/OGL_PC/Error)
#include INCL_KRENDERER(Renderer/OGL_PC/RenderContext_OGL_PC)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)
#include "GLExtensions.h"

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RENDER_TARGET_OGL_PC constructor
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_OGL_PC::RENDER_TARGET_OGL_PC(RENDER_STATS & RenderStats)
:
_uiSurface(0),
_Texture(RenderStats)
{
	RenderStats.AddRenderTarget(1);
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_TARGET_OGL_PC constructor
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_OGL_PC::RENDER_TARGET_OGL_PC(RENDER_CONTEXT_OGL_PC*	pRenderContext,
										   UInt32					uiWidth,
										   UInt32					uiHeight,
										   PIXEL_FORMAT				eFormat)
:
_uiSurface(0),
_Texture(pRenderContext->GetRenderStats())
{
	_Texture._eFormat = eFormat;
	_Texture._uiWidth = uiWidth;
	_Texture._uiHeight = uiHeight;

	_Texture._nonPowerOfTwo = !(((uiWidth&(uiWidth-1))==0) && ((uiHeight&(uiHeight-1))==0));
	_Texture._bindingTarget = (uiWidth == uiHeight) ? GL_TEXTURE_2D : GL_TEXTURE_RECTANGLE_ARB;
	_Texture._UVNormalized = (_Texture._bindingTarget == GL_TEXTURE_2D);


	switch (_Texture._eFormat)
	{
		default: 
			QDT_ASSERT(0 && "TODO!!");

		case PF_A16B16G16R16:
		{
			glGenFramebuffersEXT(1, &_uiSurface);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface);
			QDT_TEST_GL_ERROR();

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			glTexImage2D(_Texture._bindingTarget, 0, GL_RGBA16F_ARB, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
			break;
		}
		case PF_A32B32G32R32:
		{
			glGenFramebuffersEXT(1, &_uiSurface);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface);
			QDT_TEST_GL_ERROR();

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			glTexImage2D(_Texture._bindingTarget, 0, GL_RGBA32F_ARB, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
			break;
		}
		
		case PF_R32F:
		{
			glGenFramebuffersEXT(1, &_uiSurface);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface);
			QDT_TEST_GL_ERROR();

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			glTexImage2D(_Texture._bindingTarget, 0, GL_RGBA32F_ARB, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
			break;

			// The following code should work and is better!! but...

			/*glGenFramebuffersEXT(1, &_uiSurface);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface);
			QDT_TEST_GL_ERROR();

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			glTexImage2D(_Texture._bindingTarget, 0, GL_LUMINANCE32F_ARB, uiWidth, uiHeight, 0, GL_LUMINANCE, GL_FLOAT, NULL);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
			break;*/
		}

		case PF_D24S8:
		{
			glGenFramebuffersEXT(1, &_uiSurface);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface);
			QDT_TEST_GL_ERROR();

		// TEST
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			glTexImage2D(_Texture._bindingTarget, 0, GL_RGBA8, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
		// TEST fin


			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

//			glTexImage2D(_Texture._bindingTarget, 0, GL_DEPTH_COMPONENT, uiWidth, uiHeight, 0, GL_UNSIGNED_INT_24_8_NV, GL_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, uiWidth, uiHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT/*GL_FLOAT*/, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
			break;
		}

		case PF_D24S8_DEPTHSTENCIL:
		{
			glGenRenderbuffersEXT(1, &_uiSurface);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _uiSurface);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, _Texture._uiWidth, _Texture._uiHeight);
			// pr le moment (0_5_0) impossibilite d'attacher un texture du depth buffer
			QDT_TEST_GL_ERROR();
			break;
		}
		case PF_A8R8G8B8:
		{
			glGenFramebuffersEXT(1, &_uiSurface);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _uiSurface);
			QDT_TEST_GL_ERROR();

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(_Texture._bindingTarget, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();

			glTexImage2D(_Texture._bindingTarget, 0, GL_RGBA8, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(_Texture._bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, _Texture._bindingTarget, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();
			break;
		}
	}

    switch (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
	{
	default:
		QDT_FAIL(); // Impossible!

	case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		QDT_Message("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT!!!");
		QDT_FAIL();
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		QDT_Message("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT!!!");
		QDT_FAIL();
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		QDT_Message("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT!!!");
		QDT_FAIL();
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		QDT_Message("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT!!!");
		QDT_FAIL();
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		QDT_Message("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT!!!");
		QDT_FAIL();
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		QDT_Message("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT!!!");
		QDT_FAIL();
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		QDT_Message("GL_FRAMEBUFFER_UNSUPPORTED_EXT!!!");
		QDT_FAIL();
		break;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	_Texture.SetAddressUType(AT_CLAMP);
	_Texture.SetAddressVType(AT_CLAMP);

	RENDER_CONTEXT::GetRenderStats().AddRenderTarget(1);
}

//-----------------------------------------------------------------------------
//	Name:		~RENDER_TARGET_OGL_PC destructor
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_OGL_PC::~RENDER_TARGET_OGL_PC()
{
	if (_uiSurface > 0)
	{
		if (_Texture._eFormat == PF_D24S8_DEPTHSTENCIL)
		{
			glDeleteRenderbuffersEXT(1, &_uiSurface);
		}
		else
		{
			glDeleteFramebuffersEXT(1, &_uiSurface);
		}

		QDT_TEST_GL_ERROR();		
		
		_uiSurface = 0;
	}

	RENDER_CONTEXT::GetRenderStats().AddRenderTarget(-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLTexture
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
const TEXTURE_OGL_PC *	RENDER_TARGET_OGL_PC::GetOGLTexture() const
{
	return &_Texture;
}

//-----------------------------------------------------------------------------
//	Name:		GetOGLSurface
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
GLuint *	RENDER_TARGET_OGL_PC::GetOGLSurface()
{
	return &_uiSurface;
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_TARGET_OGL_PC::GetWidth() const
{
	return _Texture.GetWidth();
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_TARGET_OGL_PC::GetHeight() const
{
	return _Texture.GetHeight();
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT	RENDER_TARGET_OGL_PC::GetPixelFormat() const
{
	return _Texture.GetPixelFormat();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
