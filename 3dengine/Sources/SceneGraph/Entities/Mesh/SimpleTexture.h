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
//	The SIMPLE_TEXTURE class implements ...
//
//	05-10-28:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_MESH_SIMPLETEXTURE_H__
#define __M3D_SCENEGRAPH_ENTITIES_MESH_SIMPLETEXTURE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_TEXTURE)

//Old Inclusions
#include	INC_KRNRENDERER(Renderer)

namespace QDT
{
	namespace M3D
	{
		class	SIMPLE_TEXTURE
		{
		public:

			SIMPLE_TEXTURE();
			~SIMPLE_TEXTURE();

			bool		LoadFromFile(const Int8 * pFileName);
			I_TEXTURE *	GetTexture() { return (_pTex); }

		private:

			bool		LoadDDS(const Int8 * pFileName);

			UInt32		SizeCompressed(UInt32 nWidth, UInt32 nHeight, PIXEL_FORMAT eFormat, UInt32 nComponents);	
			UInt32		SizeRGB(UInt32 nWidth, UInt32 nHeight, PIXEL_FORMAT eFormat, UInt32 nComponents);
			UInt32		ClampSize(UInt32 size);

			I_TEXTURE * _pTex;
		};
	}
}

#endif // __M3D_SCENEGRAPH_ENTITIES_MESH_SIMPLETEXTURE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
