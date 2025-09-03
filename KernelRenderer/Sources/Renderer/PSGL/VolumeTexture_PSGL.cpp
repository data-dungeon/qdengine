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
// CLASS: VOLUME_TEXTURE_PSGL
//
//
//	05-10-12:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "VolumeTexture_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer\RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VOLUME_TEXTURE_PSGL constructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
VOLUME_TEXTURE_PSGL::VOLUME_TEXTURE_PSGL(UInt32		uiWidth,
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
_uiSize(0),
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
			_gleFormat = GL_ARGB_SCE;
			_gliInternalFormat = GL_ARGB_SCE;
			_gleType = GL_UNSIGNED_INT_8_8_8_8_REV;
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

	// allocate the texture
	glTexImage3D(GL_TEXTURE_3D, 
					0, 
					_gliInternalFormat, 
					_uiWidth, 
					_uiHeight, 
					_uiDepth,
					0, 
					_gleFormat, 
					_gleType, 
					NULL);
	QDT_TEST_GL_ERROR();

	RENDER_CONTEXT::GetRenderStats().AddVolumeTexture(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfVolumeTextures(ComputeSize());
}

//-----------------------------------------------------------------------------
//	Name:		ComputeSize
//	Object:		
//	06-04-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	VOLUME_TEXTURE_PSGL::ComputeSize()
{
	UInt32 uiSize = 0;

	if (_bCompressed)
	{
		for (UInt32 uiMipMapLevel = 0; uiMipMapLevel < _uiMipMapLevels; uiMipMapLevel++)
		{
			uiSize += RENDER_CONTEXT::SizeCompressed(_uiWidth >> uiMipMapLevel, _uiHeight >> uiMipMapLevel, _eFormat);
		}
	}
	else
	{
		for (UInt32 uiMipMapLevel = 0; uiMipMapLevel < _uiMipMapLevels; uiMipMapLevel++)
		{
			int uiMipMapWidth  = _uiWidth >> uiMipMapLevel;
			int uiMipMapHeight = _uiHeight >> uiMipMapLevel;
			
			if (uiMipMapWidth == 0)  uiMipMapWidth = 1;
			if (uiMipMapHeight == 0) uiMipMapHeight = 1;
			
			uiSize += uiMipMapWidth * uiMipMapHeight * _uiPixelSize;
		}
	}

	return uiSize * _uiDepth;
}

//-----------------------------------------------------------------------------
//	Name:		~VOLUME_TEXTURE_PSGL destructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
VOLUME_TEXTURE_PSGL::~VOLUME_TEXTURE_PSGL()
{
	glDeleteTextures(1, & _gluiTextureIndex);

	RENDER_CONTEXT::GetRenderStats().AddVolumeTexture(-1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfVolumeTextures(- ComputeSize());
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void *	VOLUME_TEXTURE_PSGL::Lock(UInt32 uiMipMapLevel,
								UInt32 uiCubeFace)
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == false);
	_bLocked = true;
	
	if (_bCompressed)
	{
		QDT_FAIL(); // todo !!
		_uiDataSize = RENDER_CONTEXT::SizeCompressed(_uiWidth >> uiMipMapLevel, _uiHeight >> uiMipMapLevel, _eFormat);
	}
	else
	{
		_uiDataSize = (_uiWidth >> uiMipMapLevel) * (_uiHeight >> uiMipMapLevel) * (_uiDepth >> uiMipMapLevel) * _uiPixelSize;
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
void	VOLUME_TEXTURE_PSGL::Unlock(UInt32 uiMipMapLevel,
									UInt32 uiCubeFace)
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == true);
	_bLocked = false;

	QDT_Message("Unlock %d %d\n", uiMipMapLevel, uiCubeFace);

	glBindTexture(GL_TEXTURE_3D, _gluiTextureIndex);

	if (_bCompressed)
	{
		QDT_FAIL();
		glCompressedTexImage3D(GL_TEXTURE_3D, 
								uiMipMapLevel, 
								_gleFormat, 
								_uiWidth >> uiMipMapLevel, 
								_uiHeight >> uiMipMapLevel, 
								uiCubeFace + 1,
								0, 
								_uiDataSize, 
								_pDatasAligned);
	}
	else
	{
		glTexSubImage3D(GL_TEXTURE_3D, 
						uiMipMapLevel, 
						0,0,uiCubeFace,
						_uiWidth >> uiMipMapLevel, 
						_uiHeight >> uiMipMapLevel, 
						1, 
						_gleFormat, 
						_gleType, 
						_pDatasAligned);
	}
	QDT_TEST_GL_ERROR();

	delete [] _pDatasAllocated;

	_uiSize += _uiDataSize;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
Bool VOLUME_TEXTURE_PSGL::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 VOLUME_TEXTURE_PSGL::GetWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 VOLUME_TEXTURE_PSGL::GetHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetDepth
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	VOLUME_TEXTURE_PSGL::GetDepth() const
{
	return (_uiDepth);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
PIXEL_FORMAT VOLUME_TEXTURE_PSGL::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetTextureIndex
//	Object:		
//	05-10-12:	SBE - Created
//-----------------------------------------------------------------------------	
const GLuint & VOLUME_TEXTURE_PSGL::GetTextureIndex() const
{
	return (_gluiTextureIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void VOLUME_TEXTURE_PSGL::SetAddressUType(ADDRESS_TYPE eAddressType)
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
void VOLUME_TEXTURE_PSGL::SetAddressVType(ADDRESS_TYPE eAddressType)
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
void VOLUME_TEXTURE_PSGL::SetFilterType(FILTER_TYPE eFilterType)
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
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE VOLUME_TEXTURE_PSGL::GetAddressUType() const
{
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE VOLUME_TEXTURE_PSGL::GetAddressVType() const
{
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
FILTER_TYPE VOLUME_TEXTURE_PSGL::GetFilterType() const
{
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
