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
//	CLASS:	SIMPLE_TEXTURE
//
//	05-10-28:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Mesh/SimpleTexture)
#include	INCL_KCORE(Core/Endian)
#define		QDT_INCLUDE_STDIO
#define		QDT_INCLUDE_STRING
#include	INCL_KCORE(Includes/Includes)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SIMPLE_TEXTURE constructor
//	Object:		
//	05-10-28:	ELE - Created
//-----------------------------------------------------------------------------
SIMPLE_TEXTURE::SIMPLE_TEXTURE()
:
_pTex(NULL)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SIMPLE_TEXTURE destructor
//	Object:		
//	05-10-28:	ELE - Created
//-----------------------------------------------------------------------------
SIMPLE_TEXTURE::~SIMPLE_TEXTURE()
{
	RENDER_CONTEXT::ReleaseTexture(_pTex);
}

//-----------------------------------------------------------------------------
//	Name:		LoadTextureFromFile
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
Bool SIMPLE_TEXTURE::LoadFromFile(const Int8 * pFileName)
{
	return (LoadDDS(pFileName));
}

//-----------------------------------------------------------------------------
//	Name:		LoadDDS
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
Bool SIMPLE_TEXTURE::LoadDDS(const Int8 * pFileName)
{
	const UInt32 DDS_PFMASK		= 0x0000004F;
	const UInt32 DDS_LUMINANCE8 = 0x00000000;
	const UInt32 DDS_FOURCC		= 0x00000004;
	const UInt32 DDS_RGB		= 0x00000040;
	const UInt32 DDS_RGBA		= 0x00000041;
	const UInt32 DDS_DEPTH		= 0x00800000;

	const UInt32 DDS_COMPLEX = 0x00000008;
	const UInt32 DDS_CUBEMAP = 0x00000200;
	const UInt32 DDS_VOLUME  = 0x00200000;

	const UInt32 FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
	const UInt32 FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
	const UInt32 FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))
	
	struct DDS_PIXELFORMAT
    {
        UInt32	dwSize;
        UInt32	dwFlags;
        UInt32	dwFourCC;
        UInt32	dwRGBBitCount;
        UInt32	dwRBitMask;
        UInt32	dwGBitMask;
        UInt32	dwBBitMask;
        UInt32	dwABitMask;
    };

	struct DDS_HEADER
    {
        UInt32	dwSize;
        UInt32	dwFlags;
        UInt32	dwHeight;
        UInt32	dwWidth;
        UInt32	dwPitchOrLinearSize;
        UInt32	dwDepth;
        UInt32	dwMipMapCount;
        UInt32	dwReserved1[11];
        DDS_PIXELFORMAT ddspf;
        UInt32	dwCaps1;
        UInt32	dwCaps2;
        UInt32	dwReserved2[3];
    };
	
	DDS_HEADER Header;
	char filecode[4];
	FILE *fp;
	Bool bVolume = false;
	Bool bCubeMap = false;
	Bool bCompressed;
	UInt32 nComponents;
	PIXEL_FORMAT eFormat;

	// open file
#ifdef PLATFORM_PS3
	char	tmp[256];
	strcpy(tmp, "/app_home");
	strcat(tmp, pFileName);
	fp = fopen(tmp, "rb");
#else
	fp = fopen(pFileName, "rb");
#endif
    if (fp == NULL)
	{
		QDT_Error("The file %s has not been found!", pFileName);
        return (false);
	}

	// read in file marker, make sure its a DDS file
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        fclose(fp);
        return false;
    }

	// read in DDS header
    fread(&Header, sizeof(Header), 1, fp);
#ifdef PLATFORM_PS3
	ENDIAN::ConvertBuffer((UInt32*)&Header, sizeof(Header));
#endif

	// check if image is a cubempa
    if (Header.dwCaps2 & DDS_CUBEMAP)
	{
        bCubeMap = true;
	}

    // check if image is a volume texture
    if ((Header.dwCaps2 & DDS_VOLUME) && (Header.dwDepth > 0))
	{
        bVolume = true;
	}

	// figure out what the image format is
    if (Header.ddspf.dwFlags & DDS_FOURCC) 
    {
        switch(Header.ddspf.dwFourCC)
        {
            case FOURCC_DXT1:
                eFormat = PF_DXTC1;
                nComponents = 3;
                bCompressed = true;
                break;

            case FOURCC_DXT3:
                eFormat = PF_DXTC3;
                nComponents = 4;
                bCompressed = true;
                break;

            case FOURCC_DXT5:
                eFormat = PF_DXTC5;
                nComponents = 4;
                bCompressed = true;
                break;

            default:
                fclose(fp);
                return false;
        }
    }
    else if (Header.ddspf.dwFlags == DDS_RGBA && Header.ddspf.dwRGBBitCount == 32)
    {
        eFormat = PF_A8R8G8B8; 
        bCompressed = false;
        nComponents = 4;
    }
    else if (Header.ddspf.dwFlags == DDS_RGB  && Header.ddspf.dwRGBBitCount == 32)
    {
        eFormat = PF_X8R8G8B8; 
        bCompressed = false;
        nComponents = 4;
    }
    else if (Header.ddspf.dwFlags == DDS_RGB  && Header.ddspf.dwRGBBitCount == 24)
    {
		QDT_FAIL();
        eFormat = PF_X8R8G8B8; 
        bCompressed = false;
        nComponents = 3;
    }
	else if ((Header.ddspf.dwFlags & DDS_PFMASK) == DDS_LUMINANCE8  && Header.ddspf.dwRGBBitCount == 8)
    {
        eFormat = PF_L8; 
        bCompressed = false;
        nComponents = 1;
    }
	else 
	{
        fclose(fp);
        return (false);
    }

	// store primary surface width/height/depth
    UInt32 nWidth  = Header.dwWidth;
    UInt32 nHeight = Header.dwHeight;
    UInt32 nDepth  = Header.dwDepth;
	UInt32 nMipMapCount = Header.dwMipMapCount;

	if (bVolume)
	{
		QDT_ASSERT(bCompressed == false);
		_pTex = RENDER_CONTEXT::CreateVolumeTexture(nWidth, nHeight, nDepth, eFormat, nMipMapCount);
	}
	else if (bCubeMap)
	{
		QDT_ASSERT(nWidth == nHeight);
		_pTex = RENDER_CONTEXT::CreateCubeTexture(nWidth, eFormat, nMipMapCount);
		nDepth = 6;
	}
	else
	{
		_pTex = RENDER_CONTEXT::CreateTexture(nWidth, nHeight, eFormat, nMipMapCount);
		nDepth = 1;
	}
	
	UInt32 (SIMPLE_TEXTURE::*Size)(UInt32, UInt32, PIXEL_FORMAT, UInt32);

	if (bCompressed)
	{
		Size = &SIMPLE_TEXTURE::SizeCompressed;
	}
	else
	{
		Size = &SIMPLE_TEXTURE::SizeRGB;
	}

	// load all surfaces for the image (6 surfaces for cubemaps)
    for (UInt32 nCubeFace = 0; nCubeFace < nDepth; nCubeFace++)
    {
		// QDT_Message("face %d\n", nCubeFace);
        UInt32 uiSize;

        // calculate surface size
        uiSize = (this->*Size)(nWidth, nHeight, eFormat, nComponents);

		UInt8 * pBuffer;
        // load surface
		pBuffer = static_cast<UInt8 *>(_pTex->Lock(0, nCubeFace));
		
        fread(pBuffer, 1, uiSize, fp);
		_pTex->Unlock(0, nCubeFace);

        int w = ClampSize(nWidth >> 1);
        int h = ClampSize(nHeight >> 1);

        // load all mipmaps for current surface
        for (UInt32 nMipMapLevel = 1; nMipMapLevel < nMipMapCount && (w || h); ++nMipMapLevel)
        {
            // calculate mipmap size
            uiSize = (this->*Size)(w, h, eFormat, nComponents);
			
			// Load mipmap
            pBuffer = static_cast<UInt8 *>(_pTex->Lock(nMipMapLevel, nCubeFace));
			fread(pBuffer, 1, uiSize, fp);
			_pTex->Unlock(nMipMapLevel, nCubeFace);

            // shrink to next power of 2
            w = ClampSize(w >> 1);
            h = ClampSize(h >> 1);
        }
    }

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		ClampSize
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SIMPLE_TEXTURE::ClampSize(UInt32 size)
{
	if (size == 0)
	{
		return (1);
	}

	return (size);
}

//-----------------------------------------------------------------------------
//	Name:		SizeCompressed
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SIMPLE_TEXTURE::SizeCompressed(UInt32 nWidth, 
								UInt32 nHeight, 
								PIXEL_FORMAT eFormat, 
								UInt32 nComponents)
{
	return ((nWidth+3)/4)*((nHeight+3)/4)*(eFormat == PF_DXTC1 ? 8 : 16);   
}

//-----------------------------------------------------------------------------
//	Name:		SizeRGB
//	Object:		
//	05-09-02:	RMA - Created
//-----------------------------------------------------------------------------
UInt32 SIMPLE_TEXTURE::SizeRGB(UInt32 nWidth, 
						   UInt32 nHeight, 
						   PIXEL_FORMAT eFormat, 
						   UInt32 nComponents)
{
	return (nWidth * nHeight * nComponents);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
