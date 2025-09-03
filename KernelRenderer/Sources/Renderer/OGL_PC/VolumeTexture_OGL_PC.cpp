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
// CLASS: VOLUME_TEXTURE_OGL_PC
//
//
//	05-12-19:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "VolumeTexture_OGL_PC.h"	
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
//	Name:		VOLUME_TEXTURE_OGL_PC constructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
VOLUME_TEXTURE_OGL_PC::VOLUME_TEXTURE_OGL_PC(UInt32		uiWidth,
						UInt32			uiHeight, 
						UInt32			uiDepth,
						PIXEL_FORMAT	eFormat, 
						UInt32			uiMipMapLevels)
:
_gluiTextureIndex(0),
_uiWidth(uiWidth),
_uiHeight(uiHeight),
_uiDepth(uiDepth),
_eFormat(eFormat),
_uiMipMapLevels(uiMipMapLevels),
_eAddressUType(AT_WRAP),
_eAddressVType(AT_WRAP),
_eFilterType(FT_TRILINEAR),
_pDatasAllocated(NULL),
_pDatasAligned(NULL),
_uiPixelSize(0),
_bCompressed(false),
_bLocked(false)
{
	switch (_eFormat)
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
	}

	glGenTextures(1, &_gluiTextureIndex);
	glBindTexture(GL_TEXTURE_3D, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*_MIPMAP_LINEAR*/);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVolumeTexture(1);
}

//-----------------------------------------------------------------------------
//	Name:		~VOLUME_TEXTURE_OGL_PC destructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
VOLUME_TEXTURE_OGL_PC::~VOLUME_TEXTURE_OGL_PC()
{
	glDeleteTextures(1, & _gluiTextureIndex);
	RENDER_CONTEXT::GetRenderStats().AddVolumeTexture(-1);
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void *	VOLUME_TEXTURE_OGL_PC::Lock(UInt32 uiMipMapLevel,
									UInt32 uiCubeFace)
{
	QDT_ASSERT(_bLocked == false);

	_bLocked = true;
	
	if (_bCompressed)
	{
		_uiDataSize = RENDER_CONTEXT::SizeCompressed(_uiWidth >> uiMipMapLevel, _uiHeight >> uiMipMapLevel, _eFormat) / 8;
	}
	else
	{
		_uiDataSize = (_uiWidth >> uiMipMapLevel) * (_uiHeight >> uiMipMapLevel) * _uiPixelSize / 8;
	}

	if (uiCubeFace == 0)
	{
		UInt32 uiSize2Alloc = _uiDataSize * _uiDepth;

		_pDatasAllocated = new Char[uiSize2Alloc + 128];
		_pDatasAligned =(Char *)(((IntPtr)_pDatasAllocated+127)&~127);
	}

	return &_pDatasAligned[_uiDataSize * uiCubeFace];
}

//-----------------------------------------------------------------------------
//	Name:		Unlock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void	VOLUME_TEXTURE_OGL_PC::Unlock(UInt32 uiMipMapLevel,
									UInt32 uiCubeFace)
{
	QDT_ASSERT(_bLocked == true);
	_bLocked = false;

	if (uiCubeFace == (_uiDepth - 1))
	{
		glBindTexture(GL_TEXTURE_3D, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();

		if (_bCompressed)
		{
			glCompressedTexImage3DARB(GL_TEXTURE_3D, 
									uiMipMapLevel, 
									_gleFormat, 
									_uiWidth >> uiMipMapLevel, 
									_uiHeight >> uiMipMapLevel, 
									_uiDepth,
									0, 
									_uiDataSize, 
									_pDatasAligned);
		}
		else
		{
			glTexImage3D(GL_TEXTURE_3D, 
							uiMipMapLevel, 
							_gliInternalFormat, 
							_uiWidth >> uiMipMapLevel, 
							_uiHeight >> uiMipMapLevel, 
							_uiDepth,
							0, 
							_gleFormat, 
							_gleType, 
							_pDatasAligned);
		}
		QDT_TEST_GL_ERROR();

		delete [] _pDatasAllocated;
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------	
Bool VOLUME_TEXTURE_OGL_PC::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 VOLUME_TEXTURE_OGL_PC::GetWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 VOLUME_TEXTURE_OGL_PC::GetHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetDepth
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	VOLUME_TEXTURE_OGL_PC::GetDepth() const
{
	return (_uiDepth);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------	
PIXEL_FORMAT VOLUME_TEXTURE_OGL_PC::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetTextureIndex
//	Object:		
//	05-12-19:	SBE - Created
//-----------------------------------------------------------------------------	
const GLuint & VOLUME_TEXTURE_OGL_PC::GetTextureIndex() const
{
	return (_gluiTextureIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void VOLUME_TEXTURE_OGL_PC::SetAddressUType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[2] = 
	{
		GL_REPEAT,
		GL_CLAMP,
	};

	if (eAddressType != _eAddressUType)
	{
		_eAddressUType = eAddressType;
		glBindTexture(GL_TEXTURE_3D, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, PSGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void VOLUME_TEXTURE_OGL_PC::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[2] = 
	{
		GL_REPEAT,
		GL_CLAMP,
	};

	if (eAddressType != _eAddressUType)
	{
		_eAddressVType = eAddressType;
		glBindTexture(GL_TEXTURE_3D, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, PSGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void VOLUME_TEXTURE_OGL_PC::SetFilterType(FILTER_TYPE eFilterType)
{
	glBindTexture(GL_TEXTURE_3D, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();
		
	switch (eFilterType)
	{
	default:
		QDT_ASSERT(0);

	case FT_NONE:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_BILINEAR:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_NEAREST*/);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_TRILINEAR:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_2:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_4:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_8:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_16:
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		QDT_TEST_GL_ERROR();
		break;
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE VOLUME_TEXTURE_OGL_PC::GetAddressUType() const
{
	return _eAddressUType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE VOLUME_TEXTURE_OGL_PC::GetAddressVType() const
{
	return _eAddressVType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
FILTER_TYPE VOLUME_TEXTURE_OGL_PC::GetFilterType() const
{
	return _eFilterType;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
