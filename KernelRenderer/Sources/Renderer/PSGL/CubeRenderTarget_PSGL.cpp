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
// CLASS: CUBE_RENDER_TARGET_PSGL
//
//
//	05-10-24:	SBE - Created
//*****************************************************************************

#include "Root.h"
#include "CubeRenderTarget_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer/RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CUBE_RENDER_TARGET_PSGL constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_PSGL::CUBE_RENDER_TARGET_PSGL()
{
	for (UInt32 i=0; i<6; i++)
		_uiSurface[i] = 0;

	RENDER_CONTEXT::GetRenderStats().AddCubeRenderTarget(1);
}

//-----------------------------------------------------------------------------
//	Name:		CUBE_RENDER_TARGET_PSGL constructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_PSGL::CUBE_RENDER_TARGET_PSGL(RENDER_CONTEXT_PSGL*	pRenderContext,
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
			QDT_FAIL();
			break;

		case PF_D24S8_DEPTHSTENCIL:
		{
			for (UInt32 i=0; i<6; i++)
			{
				glGenRenderbuffersOES(1,&_uiSurface[i]);
				glBindRenderbufferOES(GL_RENDERBUFFER_OES, _uiSurface[i]);
				glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT, _Texture._uiEdgeLength, _Texture._uiEdgeLength);
				// pr le moment (0_5_0) impossibilite d'attacher un texture du depth buffer
			}

			break;
		}

		case PF_R32:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _Texture._gluiTextureIndex);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_ALLOCATION_HINT_SCE,GL_TEXTURE_TILED_GPU_SCE);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_ARGB_SCE, uiEdgeLength, uiEdgeLength, 0, GL_LUMINANCE, GL_FLOAT, NULL);
			}
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(6, _uiSurface);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface[i]);
				QDT_TEST_GL_ERROR();

				// attach texture to framebuffer object
				glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _Texture._gluiTextureIndex, 0);
				QDT_TEST_GL_ERROR();
			}

			break;
		}
	
		case PF_A16B16G16R16:
		{
			glGenTextures(1, &_Texture._gluiTextureIndex);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _Texture._gluiTextureIndex);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_ALLOCATION_HINT_SCE,GL_TEXTURE_TILED_GPU_SCE);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F_ARB, uiEdgeLength, uiEdgeLength, 0, GL_RGBA, GL_FLOAT, NULL);				
			}
			QDT_TEST_GL_ERROR();

			glGenFramebuffersOES(6, _uiSurface);
			QDT_TEST_GL_ERROR();

			for (UInt32 i=0; i<6; i++)
			{
				glBindFramebufferOES(GL_FRAMEBUFFER_OES, _uiSurface[i]);
				QDT_TEST_GL_ERROR();

				// attach texture to framebuffer object
				glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _Texture._gluiTextureIndex, 0);
				QDT_TEST_GL_ERROR();
			}

			break;
		}
	}

	_Texture.SetAddressUType(AT_CLAMP);
	_Texture.SetAddressVType(AT_CLAMP);

	RENDER_CONTEXT::GetRenderStats().AddCubeRenderTarget(1);
}

//-----------------------------------------------------------------------------
//	Name:		~CUBE_RENDER_TARGET_PSGL destructor
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_RENDER_TARGET_PSGL::~CUBE_RENDER_TARGET_PSGL()
{
	if (_uiSurface > 0)
	{
		if (_Texture._eFormat == PF_D24S8_DEPTHSTENCIL)
		{
			for (UInt32 i=0; i<6; i++)
			{
				glDeleteRenderbuffersOES(1, &_uiSurface[i]);
				_uiSurface[i] = 0;
			}
		}
		else
		{
			for (UInt32 i=0; i<6; i++)
			{
				glDeleteFramebuffersOES(1, &_uiSurface[i]);
				_uiSurface[i] = 0;
			}
		}		
	}

	RENDER_CONTEXT::GetRenderStats().AddCubeRenderTarget(-1);
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLTexture
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
const CUBE_TEXTURE_PSGL *	CUBE_RENDER_TARGET_PSGL::GetPSGLTexture() const
{
	return &_Texture;
}

//-----------------------------------------------------------------------------
//	Name:		GetPSGLSurface
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
GLuint *	CUBE_RENDER_TARGET_PSGL::GetPSGLSurface(CUBEMAP_FACE CF)
{
	return &_uiSurface[CF];
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_RENDER_TARGET_PSGL::GetWidth() const
{
	return _Texture.GetWidth();
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_RENDER_TARGET_PSGL::GetHeight() const
{
	return _Texture.GetHeight();
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-24:	SBE - Created
//-----------------------------------------------------------------------------
PIXEL_FORMAT	CUBE_RENDER_TARGET_PSGL::GetPixelFormat() const
{
	return _Texture.GetPixelFormat();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
