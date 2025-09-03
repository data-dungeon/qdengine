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
// CLASS: RENDER_TARGET_PSGL
//
//
//	05-10-24:	SBE - Created
//*****************************************************************************

#include "Root.h"
#include "RenderTarget_PSGL.h"

#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RENDER_TARGET_PSGL constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_PSGL::RENDER_TARGET_PSGL()
:
_uiSurface(0)
{
}

//-----------------------------------------------------------------------------
//	Name:		RENDER_TARGET_PSGL constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_PSGL::RENDER_TARGET_PSGL(RENDER_CONTEXT_PSGL*	pRenderContext,
						  UInt32					uiWidth,
                          UInt32					uiHeight,
						  PIXEL_FORMAT				eFormat)
:
_uiSurface(0)
{
	UInt32 nPixelSize = 4;
	_Texture._eFormat = eFormat;
	_Texture._uiWidth = uiWidth;
	_Texture._uiHeight = uiHeight;

	switch (_Texture._eFormat)
	{
		default: 
			QDT_ASSERT(0 && "TODO!!");

		case PF_D24S8:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_2D, _Texture._gluiTextureIndex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_ALLOCATION_HINT_SCE, GL_TEXTURE_TILED_GPU_SCE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, uiWidth, uiHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT_24_8_SCE, NULL);

			glGenFramebuffersOES(1, &_uiSurface);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface);

			// attach texture to framebuffer object
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_TEXTURE_2D, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();

			glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
			break;
		}

		case PF_D24S8_DEPTHSTENCIL:
		{
			glGenRenderbuffersOES(1,&_uiSurface);
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, _uiSurface);
			glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT, _Texture._uiWidth, _Texture._uiHeight);
			// pr le moment (0_5_0) impossibilite d'attacher un texture du depth buffer

			QDT_TEST_GL_ERROR();
			break;
		}

		case PF_R32:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_2D, _Texture._gluiTextureIndex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_ALLOCATION_HINT_SCE,GL_TEXTURE_TILED_GPU_SCE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ARGB_SCE, uiWidth, uiHeight, 0, GL_LUMINANCE, GL_FLOAT, NULL);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE32F_ARB, uiWidth, uiHeight, 0, GL_LUMINANCE, GL_FLOAT, NULL);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(1, &_uiSurface);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface);

			// attach texture to framebuffer object
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();

			glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
			break;
		}

		case PF_A8R8G8B8:
//		case PF_A16B16G16R16:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_2D, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Attention ! hackSBE Demo => wrap forcé en repeat car bug
			// dans flow.cpp : _pRenderTarget->GetTexture()->SetAddressUType(AT_WRAP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_ALLOCATION_HINT_SCE,GL_TEXTURE_TILED_GPU_SCE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ARGB_SCE, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(1, &_uiSurface);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();

			glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
			break;
		}

		case PF_A16B16G16R16:
		{
			nPixelSize = 8;

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_2D, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_ALLOCATION_HINT_SCE, GL_TEXTURE_TILED_GPU_SCE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(1, &_uiSurface);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();

			glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
			break;
		}

		/*case PF_G16R16:
		{
			nPixelSize = 8;

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_2D, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_ALLOCATION_HINT_SCE, GL_TEXTURE_TILED_GPU_SCE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA16F_ARB , uiWidth, uiHeight, 0, GL_ALPHA_LUMINANCE_SCE, GL_HALF_FLOAT_ARB , NULL);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(1, &_uiSurface);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();

			glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
			break;
		}*/

		case PF_A32B32G32R32:
		{
			nPixelSize = 16;

			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_2D, _Texture._gluiTextureIndex);
			QDT_TEST_GL_ERROR();
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_ALLOCATION_HINT_SCE, GL_TEXTURE_TILED_GPU_SCE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, uiWidth, uiHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(1, &_uiSurface);
			glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface);
			QDT_TEST_GL_ERROR();

			// attach texture to framebuffer object
			glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _Texture._gluiTextureIndex, 0);
			QDT_TEST_GL_ERROR();

			glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
			break;
		}	
	}

	if (_Texture._gluiTextureIndex != 0)
	{
		_Texture.SetAddressUType(AT_CLAMP);
		_Texture.SetAddressVType(AT_CLAMP);
	}

	RENDER_CONTEXT::GetRenderStats().AddRenderTarget(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfRenderTargets(nPixelSize * uiWidth * uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		~RENDER_TARGET_PSGL destructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
RENDER_TARGET_PSGL::~RENDER_TARGET_PSGL()
{
	if (_uiSurface > 0)
	{
		if (_Texture._eFormat == PF_D24S8_DEPTHSTENCIL)
		{
			glDeleteRenderbuffersOES(1, &_uiSurface);
		}
		else
		{
			glDeleteFramebuffersOES(1, &_uiSurface);
		}
		
		_uiSurface = 0;
	}

	RENDER_CONTEXT::GetRenderStats().AddRenderTarget(-1);

	UInt32 nPixelSize = 4;

	if (_Texture._eFormat == PF_A16B16G16R16)	nPixelSize = 8;
	if (_Texture._eFormat == PF_A32B32G32R32)	nPixelSize = 16;

	RENDER_CONTEXT::GetRenderStats().IncrSizeOfRenderTargets(- nPixelSize * _Texture._uiWidth * _Texture._uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLTexture
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
const TEXTURE_PSGL *	RENDER_TARGET_PSGL::GetPSGLTexture() const
{
	return &_Texture;
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLSurface
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
GLuint *	RENDER_TARGET_PSGL::GetPSGLSurface()
{
	return &_uiSurface;
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_TARGET_PSGL::GetWidth() const
{
	return _Texture.GetWidth();
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	RENDER_TARGET_PSGL::GetHeight() const
{
	return _Texture.GetHeight();
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT	RENDER_TARGET_PSGL::GetPixelFormat() const
{
	return _Texture.GetPixelFormat();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
