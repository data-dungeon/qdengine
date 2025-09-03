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
// CLASS: TEXTURE_OGL_PC
//
//
//	05-09-01:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "Texture_OGL_PC.h"	
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
//	Name:		TEXTURE_OGL_PC constructor
//	Object:		
//	05-11-09:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_OGL_PC::TEXTURE_OGL_PC(RENDER_STATS & RenderStats)
:
_gluiTextureIndex(0),
_uiWidth(0),
_uiHeight(0),
_uiMipMapLevels(0),
_eAddressUType(AT_WRAP),
_eAddressVType(AT_WRAP),
_eFilterType(FT_TRILINEAR),
_pDatasAllocated(NULL),
_pDatasAligned(NULL),
_uiPixelSize(0),
_bCompressed(false),
_bLocked(false),
_bNeedDelete(true)
{
	RenderStats.AddTexture(1);
}

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_OGL_PC constructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_OGL_PC::TEXTURE_OGL_PC(UInt32		uiWidth,
							   UInt32		uiHeight, 
							   PIXEL_FORMAT	eFormat, 
							   UInt32		uiMipMapLevels)
:
_gluiTextureIndex(0),
_eAddressUType(AT_WRAP),
_eAddressVType(AT_WRAP),
_eFilterType(FT_TRILINEAR),
_pDatasAllocated(NULL),
_pDatasAligned(NULL),
_uiPixelSize(0),
_bCompressed(false),
_bLocked(false),
_bindingTarget(0),
_bNeedDelete(true)
{
	CreateTexture(uiWidth, uiHeight, eFormat, uiMipMapLevels);
	RENDER_CONTEXT::GetRenderStats().AddTexture(1);
}

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_OGL_PC constructor
//	Object:		
//	06-02-09:	FBO - Created
//-----------------------------------------------------------------------------
TEXTURE_OGL_PC::TEXTURE_OGL_PC(GLuint	id, 
							   UInt32	uiWidth, 
							   UInt32	uiHeight)
:
_gluiTextureIndex(id),
_eAddressUType(AT_WRAP),
_eAddressVType(AT_WRAP),
_eFilterType(FT_TRILINEAR),
_pDatasAllocated(NULL),
_pDatasAligned(NULL),
_uiPixelSize(0),
_bCompressed(false),
_bLocked(false),
_bindingTarget(0),
_bNeedDelete(false),
_uiWidth(uiWidth),
_uiHeight(uiHeight)
{
	QDT_ASSERT(_uiWidth);
	QDT_ASSERT(_uiHeight);

	_bindingTarget = (_uiWidth == _uiHeight) ? GL_TEXTURE_2D : GL_TEXTURE_RECTANGLE_ARB;

	RENDER_CONTEXT::GetRenderStats().AddTexture(1);
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
void	TEXTURE_OGL_PC::CreateTexture(UInt32		uiWidth,
									  UInt32		uiHeight,
									  PIXEL_FORMAT	eFormat,
									  UInt32		uiMipMapLevels)
{
	_uiWidth = uiWidth;
	_uiHeight = uiHeight;
	_eFormat = eFormat;
	_uiMipMapLevels = uiMipMapLevels;

	switch (eFormat)
	{
		default:
			QDT_FAIL();
		break;

		case PF_DXTC1:
			_gleFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			_bCompressed = true;
		break;

		case PF_DXTC3:
			_gleFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			_bCompressed = true;
		break;

		case PF_DXTC5:
			_gleFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			_bCompressed = true;
		break;

		case PF_A8R8G8B8:
		case PF_X8R8G8B8:
			_gleFormat = GL_BGRA;
			_gliInternalFormat = GL_RGBA;
			_gleType = GL_UNSIGNED_BYTE;
			_uiPixelSize = 4;
		break;

		case PF_L8:
			_gleFormat = GL_LUMINANCE;
			_gliInternalFormat = GL_LUMINANCE8;
			_gleType = GL_UNSIGNED_BYTE;
			_uiPixelSize = 1;
		break;

		case PF_R32F:
			// Attention c'est bizarre ça demander à SBE
			_gleFormat = GL_RGBA;
			_gliInternalFormat = GL_RGBA32F_ARB;
			_gleType = GL_FLOAT;
			_uiPixelSize = 4;
		break;

		case PF_D24S8_DEPTHSTENCIL:
			_gleFormat = GL_DEPTH_COMPONENT; //GL_ARGB_SCE
			_gliInternalFormat = GL_DEPTH_COMPONENT24;
			_gleType = GL_FLOAT;
			_uiPixelSize = 4;
		break;

		case PF_A16B16G16R16:
			_gleFormat = GL_RGBA;
			_gliInternalFormat = GL_RGBA16F_ARB;
			_gleType = GL_FLOAT;
			_uiPixelSize = 8;
		break;

		case PF_A32B32G32R32:
			_gleFormat = GL_RGBA;
			_gliInternalFormat = GL_RGBA32F_ARB;
			_gleType = GL_FLOAT;
			_uiPixelSize = 16;
		break;
	}

	_nonPowerOfTwo = !(((uiWidth&(uiWidth-1))==0) && ((uiHeight&(uiHeight-1))==0));
	_bindingTarget = (uiWidth == uiHeight) ? GL_TEXTURE_2D : GL_TEXTURE_RECTANGLE_ARB;
	_UVNormalized = (_bindingTarget == GL_TEXTURE_2D);

	glGenTextures(1, &_gluiTextureIndex);
	QDT_TEST_GL_ERROR();

	glBindTexture(_bindingTarget, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();

	if ((_eFormat == PF_R32F) || (_eFormat == PF_D24S8_DEPTHSTENCIL))
	{
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(_bindingTarget, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
	}
	else
	{
		if (_uiMipMapLevels)     glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else					 glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);

		if (_UVNormalized) //freaking rectangle texture
		{
			glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		~TEXTURE_OGL_PC destructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_OGL_PC::~TEXTURE_OGL_PC()
{
	if (_bNeedDelete)
	{
		glDeleteTextures(1, & _gluiTextureIndex);
	}

	RENDER_CONTEXT::GetRenderStats().AddTexture(-1);
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void *	TEXTURE_OGL_PC::Lock(UInt32 uiMipMapLevel,
						  UInt32 uiCubeFace)
{
	QDT_ASSERT(_bLocked == false);
	QDT_ASSERT(uiCubeFace == 0);

	_bLocked = true;
	
	if (_bCompressed)
	{
		_uiDataSize = RENDER_CONTEXT::SizeCompressed(_uiWidth >> uiMipMapLevel, _uiHeight >> uiMipMapLevel, _eFormat);
	}
	else
	{
		_uiDataSize = (_uiWidth >> uiMipMapLevel) * (_uiHeight >> uiMipMapLevel) * _uiPixelSize;
	}

	_pDatasAllocated = new Char[_uiDataSize + 128];
	_pDatasAligned =(Char *)(((IntPtr)_pDatasAllocated+127)&~127);

	RENDER_CONTEXT::GetRenderStats().AddTextureLock(1);

	return _pDatasAligned;
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void	TEXTURE_OGL_PC::Unlock(UInt32 uiMipMapLevel,
							UInt32 uiCubeFace)
{
	QDT_ASSERT(_bLocked == true);
	QDT_ASSERT(uiCubeFace == 0);

	_bLocked = false;

	glBindTexture(_bindingTarget, _gluiTextureIndex);

	if (_bCompressed)
	{
		glCompressedTexImage2DARB(_bindingTarget, 
								uiMipMapLevel, 
								_gleFormat, 
								_uiWidth >> uiMipMapLevel, 
								_uiHeight >> uiMipMapLevel, 
								0, 
								_uiDataSize, 
								_pDatasAligned);
		QDT_TEST_GL_ERROR();
	}
	else
	{
		glTexImage2D(_bindingTarget, 
						uiMipMapLevel, 
						_gliInternalFormat, 
						_uiWidth >> uiMipMapLevel, 
						_uiHeight >> uiMipMapLevel, 
						0, 
						_gleFormat, 
						_gleType, 
						_pDatasAligned);
		QDT_TEST_GL_ERROR();
	}

	delete [] _pDatasAllocated;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
Bool TEXTURE_OGL_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 TEXTURE_OGL_PC::GetWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 TEXTURE_OGL_PC::GetHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
PIXEL_FORMAT TEXTURE_OGL_PC::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetTextureIndex
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
const GLuint & TEXTURE_OGL_PC::GetTextureIndex() const
{
	return (_gluiTextureIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void TEXTURE_OGL_PC::SetAddressUType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[2] = 
	{
		GL_REPEAT,
		GL_CLAMP,
	};

	if (eAddressType != _eAddressUType)
	{
		_eAddressUType = eAddressType;
		glBindTexture(_bindingTarget, _gluiTextureIndex);
		
		glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_S, PSGLTextureAddress[eAddressType]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void TEXTURE_OGL_PC::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[2] = 
	{
		GL_REPEAT,
		GL_CLAMP,
	};

	if (eAddressType != _eAddressVType)
	{
		_eAddressVType = eAddressType;
		glBindTexture(_bindingTarget, _gluiTextureIndex);
		
		glTexParameteri(_bindingTarget, GL_TEXTURE_WRAP_T, PSGLTextureAddress[eAddressType]);
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
void TEXTURE_OGL_PC::SetFilterType(FILTER_TYPE eFilterType)
{
	glBindTexture(_bindingTarget, _gluiTextureIndex);
	
	switch (eFilterType)
	{
	default:
		QDT_ASSERT(0);

	case FT_NONE:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_BILINEAR:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_TRILINEAR:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_2:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_4:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_8:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_16:
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(_bindingTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		QDT_TEST_GL_ERROR();
		break;
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE TEXTURE_OGL_PC::GetAddressUType() const
{
	return _eAddressUType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE TEXTURE_OGL_PC::GetAddressVType() const
{
	return _eAddressVType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
FILTER_TYPE TEXTURE_OGL_PC::GetFilterType() const
{
	return _eFilterType;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

