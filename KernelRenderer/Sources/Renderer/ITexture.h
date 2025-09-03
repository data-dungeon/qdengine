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
//	CLASS:	I_TEXTURE
//	The I_TEXTURE class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_I_TEXTURE_H__
#define __KRENDERER_RENDERER_I_TEXTURE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_KRENDERER(Renderer/RendererTypeDefs)

namespace	QDT
{
	namespace	KRENDERER
	{
		class I_TEXTURE
		{
		public:

			virtual	~I_TEXTURE() {}

			virtual void *			Lock(UInt32 uiMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X) = 0;
			virtual void			Unlock(UInt32 uiMipMapLevel = 0, UInt32 uiCubeFace = CF_POSITIVE_X) = 0;
			virtual Bool			IsLocked() const = 0;
			virtual UInt32			GetWidth() const = 0;
			virtual UInt32			GetHeight() const = 0;
			virtual UInt32			GetDepth() const = 0;
			virtual PIXEL_FORMAT	GetPixelFormat() const = 0;
			virtual void			SetAddressUType(ADDRESS_TYPE eAddressType) = 0;
			virtual void			SetAddressVType(ADDRESS_TYPE eAddressType) = 0;
			virtual void			SetFilterType(FILTER_TYPE eFilterType) = 0;
			virtual ADDRESS_TYPE	GetAddressUType() const = 0;
			virtual ADDRESS_TYPE	GetAddressVType() const = 0;
			virtual FILTER_TYPE		GetFilterType() const = 0;
			virtual UInt32			GetTarget() const = 0;
			virtual void *			GetHandle() const = 0;
			virtual Bool			GetUVNormalized() = 0;
			virtual UInt32          GetNbrMipMapLevels() = 0;
			virtual void			GenerateMipMaps() = 0;
		};
	}
}

#endif // __KRENDERER_RENDERER_I_TEXTURE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
