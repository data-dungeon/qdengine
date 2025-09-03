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
// CLASS: TEXTURE_PSGL
//
//
//	05-09-01:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif

#include "Root.h"
#include "Texture_PSGL.h"
#include INCL_KRENDERER(Renderer\PSGL\Error)
#include INCL_KRENDERER(Renderer\RenderContext)
#include INCL_KRENDERER(Renderer/RenderStats)

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_PSGL constructor
//	Object:		
//	05-11-09:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_PSGL::TEXTURE_PSGL()
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
_bLocked(false)
{	
}

//-----------------------------------------------------------------------------
//	Name:		TEXTURE_PSGL constructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_PSGL::TEXTURE_PSGL(UInt32		uiWidth,
						UInt32			uiHeight, 
						PIXEL_FORMAT	eFormat, 
						UInt32			uiMipMapLevels)
:
_gluiTextureIndex(0),
_eAddressUType(AT_WRAP),
_eAddressVType(AT_WRAP),
_eFilterType(FT_TRILINEAR),
_pDatasAllocated(NULL),
_pDatasAligned(NULL),
_uiPixelSize(0),
_bCompressed(false),
_bLocked(false)
{
	CreateTexture(uiWidth, uiHeight, eFormat, uiMipMapLevels);
}

//-----------------------------------------------------------------------------
//	Name:		CreateTexture
//	Object:		
//	05-11-10:	SBE - Created
//-----------------------------------------------------------------------------
void	TEXTURE_PSGL::CreateTexture(UInt32			uiWidth,
									UInt32			uiHeight,
									PIXEL_FORMAT	eFormat,
									UInt32			uiMipMapLevels)
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

		case PF_R32:
			_gleFormat = GL_RGBA; //GL_ARGB_SCE
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
	}

	glGenTextures(1, &_gluiTextureIndex);
	QDT_ASSERT(_gluiTextureIndex);
	glBindTexture(GL_TEXTURE_2D, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();	

	if ((_eFormat == PF_R32) || (_eFormat == PF_D24S8_DEPTHSTENCIL))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
	}
	else
	{
		if (_uiMipMapLevels)     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else					 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	QDT_TEST_GL_ERROR();	

	RENDER_CONTEXT::GetRenderStats().AddTexture(1);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfTextures(ComputeSize());
}

//-----------------------------------------------------------------------------
//	Name:		ComputeSize
//	Object:		
//	06-04-12:	SBE - Created
//-----------------------------------------------------------------------------
UInt32	TEXTURE_PSGL::ComputeSize()
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

	return uiSize;
}

//-----------------------------------------------------------------------------
//	Name:		~TEXTURE_PSGL destructor
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
TEXTURE_PSGL::~TEXTURE_PSGL()
{
	QDT_ASSERT(_gluiTextureIndex);
	glDeleteTextures(1, & _gluiTextureIndex);
	RENDER_CONTEXT::GetRenderStats().IncrSizeOfTextures(- ComputeSize());
}

//-----------------------------------------------------------------------------
//	Name:		Lock
//	Object:		
//	05-10-11:	SBE - Created
//-----------------------------------------------------------------------------
void *	TEXTURE_PSGL::Lock(UInt32 uiMipMapLevel,
						  UInt32 uiCubeFace)
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == false);
	QDT_ASSERT(uiCubeFace == 0);

	_bLocked = true;
	
	if (_bCompressed)
	{
		_uiDataSize = RENDER_CONTEXT::SizeCompressed(MATH::Max(_uiWidth >> uiMipMapLevel, (UInt32)1), MATH::Max(_uiHeight >> uiMipMapLevel, (UInt32)1), _eFormat);
	}
	else
	{
		int uiMipMapWidth  = MATH::Max(_uiWidth >> uiMipMapLevel, (UInt32)1);
		int uiMipMapHeight = MATH::Max(_uiHeight >> uiMipMapLevel, (UInt32)1);
		
		_uiDataSize = uiMipMapWidth * uiMipMapHeight * _uiPixelSize;
	}

	QDT_ASSERT(_uiDataSize);
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
void	TEXTURE_PSGL::Unlock(UInt32 uiMipMapLevel,
							UInt32 uiCubeFace)
{
	AUTO_LOCK	AL(RENDER_CONTEXT::GetMutex());

	QDT_ASSERT(_bLocked == true);
	QDT_ASSERT(uiCubeFace == 0);

	_bLocked = false;

	QDT_ASSERT(_gluiTextureIndex);

	glBindTexture(GL_TEXTURE_2D, _gluiTextureIndex);

	UInt32 uiMipMapWidth = _uiWidth >> uiMipMapLevel;
	UInt32 uiMipMapHeight = _uiHeight >> uiMipMapLevel;

	if (uiMipMapWidth == 0) uiMipMapWidth = 1;
	if (uiMipMapHeight == 0) uiMipMapHeight = 1;

	if (_bCompressed)
	{
		glCompressedTexImage2D(GL_TEXTURE_2D, 
								uiMipMapLevel, 
								_gleFormat, 
								uiMipMapWidth, 
								uiMipMapHeight, 
								0, 
								_uiDataSize, 
								_pDatasAligned);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 
						uiMipMapLevel, 
						_gliInternalFormat, 
						uiMipMapWidth, 
						uiMipMapHeight, 
						0, 
						_gleFormat, 
						_gleType, 
						_pDatasAligned);
	}
	QDT_TEST_GL_ERROR();

	delete [] _pDatasAllocated;
}

//-----------------------------------------------------------------------------
//	Name:		IsLocked
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
Bool TEXTURE_PSGL::IsLocked() const
{
	return (_bLocked);
}

//-----------------------------------------------------------------------------
//	Name:		GetWidth
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 TEXTURE_PSGL::GetWidth() const
{
	return (_uiWidth);
}

//-----------------------------------------------------------------------------
//	Name:		GetHeight
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
UInt32 TEXTURE_PSGL::GetHeight() const
{
	return (_uiHeight);
}

//-----------------------------------------------------------------------------
//	Name:		GetPixelFormat
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
PIXEL_FORMAT TEXTURE_PSGL::GetPixelFormat() const
{
	return (_eFormat);
}

//-----------------------------------------------------------------------------
//	Name:		GetTextureIndex
//	Object:		
//	05-09-01:	RMA - Created
//-----------------------------------------------------------------------------	
const GLuint & TEXTURE_PSGL::GetTextureIndex() const
{
	return (_gluiTextureIndex);
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressUType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------	
void TEXTURE_PSGL::SetAddressUType(ADDRESS_TYPE eAddressType)
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
		QDT_ASSERT(_gluiTextureIndex);
		glBindTexture(GL_TEXTURE_2D, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, PSGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetAddressVType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------	
void TEXTURE_PSGL::SetAddressVType(ADDRESS_TYPE eAddressType)
{
	static UInt32 PSGLTextureAddress[3] = 
	{
		GL_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_MIRRORED_REPEAT,
	};

	if (eAddressType != _eAddressUType)
	{
		_eAddressVType = eAddressType;
		QDT_ASSERT(_gluiTextureIndex);
		glBindTexture(GL_TEXTURE_2D, _gluiTextureIndex);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, PSGLTextureAddress[eAddressType]);
		QDT_TEST_GL_ERROR();
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		SetFilterType
//	Object:		
//	05-10-06:	RMA - Created
//-----------------------------------------------------------------------------	
void TEXTURE_PSGL::SetFilterType(FILTER_TYPE eFilterType)
{
	QDT_ASSERT(_gluiTextureIndex);
	glBindTexture(GL_TEXTURE_2D, _gluiTextureIndex);
	QDT_TEST_GL_ERROR();
		
	switch (eFilterType)
	{
	default:
		QDT_ASSERT(0);

	case FT_NONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_BILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_TRILINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1); // Disabled!
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_2:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_4:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_8:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
		QDT_TEST_GL_ERROR();
		break;

	case FT_ANISOTROPIC_LEVEL_16:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		QDT_TEST_GL_ERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		QDT_TEST_GL_ERROR();
		break;
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressUType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE TEXTURE_PSGL::GetAddressUType() const
{
	return _eAddressUType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetAddressVType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
ADDRESS_TYPE TEXTURE_PSGL::GetAddressVType() const
{
	return _eAddressVType;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetFilterType
//	Object:		
//	05-12-15:	SBE - Created
//-----------------------------------------------------------------------------	
FILTER_TYPE TEXTURE_PSGL::GetFilterType() const
{
	return _eFilterType;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

