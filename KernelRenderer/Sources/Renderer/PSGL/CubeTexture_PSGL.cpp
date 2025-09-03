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
// CLASS: CUBE_TEXTURE_PSGL
//
//
//	05-10-11:	SBE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "CubeTexture_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer\RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CUBE_TEXTURE_PSGL constructor
//	Object:		
//	05-12-05:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_TEXTURE_PSGL::CUBE_TEXTURE_PSGL()
:
_gluiTextureIndex(0),
_uiEdgeLength(0),
_uiMipMapLevels(0),
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
}

//-----------------------------------------------------------------------------
//	Name:		CUBE_TEXTURE_PSGL constructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_TEXTURE_PSGL::CUBE_TEXTURE_PSGL(UInt32 uiEdgeLength,
						PIXEL_FORMAT eFormat, 
						UInt32 uiMipMapLevels)
:
_gluiTextureIndex(0),
_uiEdgeLength(uiEdgeLength),
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	QDT_TEST_GL_ERROR();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	QDT_TEST_GL_ERROR(); // tocheck GL_INVALID_ENUM

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	QDT_TEST_GL_ERROR();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	QDT_TEST_GL_ERROR();	

	RENDER_CONTEXT::GetRenderStats().AddCubeTexture(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfCubeTextures(ComputeSize());
}

//-----------------------------------------------------------------------------
//	Name:		ComputeSize
//	Object:		
//	06-04-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	CUBE_TEXTURE_PSGL::ComputeSize()
{
	UInt32 uiSize = 0;

	if (_bCompressed)
	{
		for (UInt32 uiMipMapLevel = 0; uiMipMapLevel <= _uiMipMapLevels; uiMipMapLevel++)
		{
			uiSize += RENDER_CONTEXT::SizeCompressed(MATH::Max(_uiEdgeLength >> uiMipMapLevel, (UInt32)1), MATH::Max(_uiEdgeLength >> uiMipMapLevel, (UInt32)1), _eFormat);
		}
	}
	else
	{
		for (UInt32 uiMipMapLevel = 0; uiMipMapLevel <= _uiMipMapLevels; uiMipMapLevel++)
		{
			int uiMipMapWidth  = MATH::Max(_uiEdgeLength >> uiMipMapLevel, (UInt32)1);
			int uiMipMapHeight = MATH::Max(_uiEdgeLength >> uiMipMapLevel, (UInt32)1);
			
			uiSize += uiMipMapWidth * uiMipMapHeight * _uiPixelSize;
		}
	}

	QDT_ASSERT(uiSize);

	return uiSize * 6;
}

//-----------------------------------------------------------------------------
//	Name:		~CUBE_TEXTURE_PSGL destructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
CUBE_TEXTURE_PSGL::~CUBE_TEXTURE_PSGL()
{
	glDeleteTextures(1, & _gluiTextureIndex);

	RENDER_CONTEXT::GetRenderStats().AddCubeTexture(- 1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfCubeTextures(- ComputeSize());
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void *	CUBE_TEXTURE_PSGL::Lock(UInt32 uiMipMapLevel,
						  UInt32 uiCubeFace)
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == false);
	_bLocked = true;
	
	if (_bCompressed)
	{
		_uiDataSize = RENDER_CONTEXT::SizeCompressed(_uiEdgeLength >> uiMipMapLevel, _uiEdgeLength >> uiMipMapLevel, _eFormat);
	}
	else
	{
		_uiDataSize = (_uiEdgeLength >> uiMipMapLevel) * (_uiEdgeLength >> uiMipMapLevel) * _uiPixelSize;
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
void	CUBE_TEXTURE_PSGL::Unlock(UInt32 uiMipMapLevel,
						  UInt32 uiCubeFace)
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == true);
	_bLocked = false;

	glBindTexture(GL_TEXTURE_CUBE_MAP, _gluiTextureIndex);

	if (_bCompressed)
	{
		glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + uiCubeFace, 
								uiMipMapLevel, 
								_gleFormat, 
								_uiEdgeLength >> uiMipMapLevel, 
								_uiEdgeLength >> uiMipMapLevel, 
								0, 
								_uiDataSize, 
								_pDatasAligned);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + uiCubeFace, 
						uiMipMapLevel, 
						_gliInternalFormat, 
						_uiEdgeLength >> uiMipMapLevel, 
						_uiEdgeLength >> uiMipMapLevel, 
						0, 
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
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
Bool CUBE_TEXTURE_PSGL::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 CUBE_TEXTURE_PSGL::GetWidth() const
{
	return (_uiEdgeLength);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
UInt32 CUBE_TEXTURE_PSGL::GetHeight() const
{
	return (_uiEdgeLength);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
PIXEL_FORMAT CUBE_TEXTURE_PSGL::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetTextureIndex
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------	
const GLuint & CUBE_TEXTURE_PSGL::GetTextureIndex() const
{
	return (_gluiTextureIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void CUBE_TEXTURE_PSGL::SetAddressUType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[3] = 
	{
		GL_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_MIRRORED_REPEAT,
	};

	if (eAddressType != _eAddressUType)
	{
		_eAddressUType = eAddressType;
		glBindTexture(GL_TEXTURE_CUBE_MAP, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, PSGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void CUBE_TEXTURE_PSGL::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[3] = 
	{
		GL_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_MIRRORED_REPEAT,
	};

	if (eAddressType != _eAddressVType)
	{
		_eAddressVType = eAddressType;
		glBindTexture(GL_TEXTURE_CUBE_MAP, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, PSGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-10-06:	SBE - Created
//-----------------------------------------------------------------------------	
void CUBE_TEXTURE_PSGL::SetFilterType(FILTER_TYPE eFilterType)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();
		
	switch (eFilterType)
	{
	default:
		QDT_ASSERT(0);

	case FT_NONE:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_BILINEAR:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*_MIPMAP_NEAREST*/);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_TRILINEAR:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_2:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_4:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_8:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_16:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		QDT_TEST_GL_ERROR();
		break;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE CUBE_TEXTURE_PSGL::GetAddressUType() const
{
	return _eAddressUType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE CUBE_TEXTURE_PSGL::GetAddressVType() const
{
	return _eAddressVType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
FILTER_TYPE CUBE_TEXTURE_PSGL::GetFilterType() const
{
	return _eFilterType;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
