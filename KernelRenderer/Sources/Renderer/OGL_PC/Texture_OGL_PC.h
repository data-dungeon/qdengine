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
//	CLASS:	TEXTURE_OGL_PC
//	The TEXTURE_OGL_PC class implements ...
//
//	05-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_OGL_PC_TEXTURE_OGL_PC_H__
#define __KRENDERER_RENDERER_OGL_PC_TEXTURE_OGL_PC_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(RENDER_STATS)

#include INCL_KRENDERER(Renderer\ITexture)
#define QDT_INCLUDE_PC_WINDOWS
#define QDT_INCLUDE_PC_GL
#include INCL_KCORE(Includes_PC\Includes_PC)

namespace	QDT
{
	namespace	KRENDERER
	{
		class TEXTURE_OGL_PC : public I_TEXTURE
		{
		public:

			TEXTURE_OGL_PC(RENDER_STATS & RenderStats);
			TEXTURE_OGL_PC(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT eFormat, UInt32 nMipMapLevels);
			TEXTURE_OGL_PC(GLuint id, UInt32 uiWidth, UInt32 uiHeight);
			~TEXTURE_OGL_PC();

			// Derivation from I_TEXTURE
			void *			Lock(UInt32 nMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X);
			void			Unlock(UInt32 nMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X);
			Bool			IsLocked() const;
			UInt32			GetWidth() const;
			UInt32			GetHeight() const;
			UInt32			GetDepth() const { return 0; }
			PIXEL_FORMAT	GetPixelFormat() const;
			void			SetAddressUType(ADDRESS_TYPE eAddressType);
			void			SetAddressVType(ADDRESS_TYPE eAddressType);
			void			SetFilterType(FILTER_TYPE eFilterType);
			ADDRESS_TYPE	GetAddressUType() const;
			ADDRESS_TYPE	GetAddressVType() const;
			FILTER_TYPE		GetFilterType() const;
			//

			const GLuint &	GetTextureIndex() const;
			virtual UInt32	GetTarget() const { return _bindingTarget; }
			virtual void *	GetHandle() const { return (void*) &GetTextureIndex(); }

			virtual Bool		GetUVNormalized() {return _UVNormalized;};
			virtual UInt32          GetNbrMipMapLevels() {return _uiMipMapLevels;};

			//UInt32 GetOGLBinding() {return _bindingTarget;};

		private:

			friend class RENDER_TARGET_OGL_PC;
			friend class RENDER_CONTEXT_OGL_PC;

			void			CreateTexture(UInt32 uiWidth, UInt32 uiHeight, PIXEL_FORMAT eFormat, UInt32 nMipMapLevels);

			GLuint			_gluiTextureIndex;

			PIXEL_FORMAT	_eFormat;
			UInt32			_uiMipMapLevels;
			UInt32			_uiWidth;
			UInt32			_uiHeight;
			Bool			_bLocked;
			ADDRESS_TYPE	_eAddressUType;
			ADDRESS_TYPE	_eAddressVType;
			FILTER_TYPE		_eFilterType;

			UInt32          _bindingTarget;  // For rectangle textures
			Bool            _nonPowerOfTwo;  // Needed to know if texture coords are normalized
			Bool            _UVNormalized;  // Needed to know if texture coords are normalized

			UInt32			_uiPixelSize;
			Bool			_bCompressed;

			// a virer quand/si passage en PBO
			GLenum			_gleFormat;
			GLint			_gliInternalFormat;
			GLenum			_gleType;
			Char *			_pDatasAllocated;
			Char *			_pDatasAligned;
			UInt32			_uiDataSize;

			Bool			_bNeedDelete;
		};
	}
}

#endif // __KRENDERER_RENDERER_OGL_PC_TEXTURE_OGL_PC_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
