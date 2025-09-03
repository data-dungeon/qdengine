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
//	CLASS:	SHADER_CONTAINER
//	The SHADER_CONTAINER class implements a little container which contains
//  a pixel shader and a descriptor.
//	The descriptor describe the capacity of the shader according to the
//  lighting.
//  A shader can manage many sort of lighting : ambient/amissive, point light,
//	etc. The descriptor use 4 bit by type of lighting. For example, if the
//  shader manager ambient/emissive and 2 point lights, the descriptor will be
//  filled the following way:
//	_uiDescription = SD_AMBIENT_EMISSIVE;
//	_uiDescription = SD_POINT_LIGHT << 4;
//	_uiDescription = SD_POINT_LIGHT << 8;
//  With this method, we can check very quickly (one int comparison) if the
//	shader corresponds to the lighting condition.
//  A shader can manage 8 lights in one pass (32 bits -> 8 * 4bits enum) if we
//  don't exceed 16 type of lights.
//  The permutation possible are determined during the export stage.
//
//	05-12-13:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_MATERIAL_SHADERCCONTAINER_H__
#define __M3D_SCENEGRAPH_MATERIAL_SHADERCCONTAINER_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_PIXEL_SHADER)

namespace QDT
{
	namespace M3D
	{
		enum SHADER_DESCRIPTOR
		{
			SD_EMPTY = 0,
			SD_AMBIENT_EMISSIVE,
			SD_LIGHT_DIRECTIONAL,
			SD_LIGHT_POINT,
			SD_LIGHT_SPOT,
			SD_LIGHT_SPOT_SHADOW,
			SD_LIGHT_POINT_SHADOW,
			SD_LIGHT_DIRECTIONAL_SHADOW,
			SD_AMBIENT_EMISSIVE_SPHERICAL_HARMONICS,
			SD_LIGHT_SPOT_HARDWARE_SHADOW,
			SD_LIGHT_SPOT_HARDWARE_SHADOW_BLUR_LOW,
			SD_LIGHT_SPOT_HARDWARE_SHADOW_BLUR_MEDIUM,
			SD_LIGHT_SPOT_HARDWARE_SHADOW_BLUR_HIGH,
			SD_LIGHT_POINT_HARDWARE_SHADOW,
			SD_LIGHT_DIRECTIONAL_HARDWARE_SHADOW,
		};

		class SHADER_CONTAINER
		{
		public:

			SHADER_CONTAINER();
			~SHADER_CONTAINER();

			UInt32 GetDescription() const;
			I_PIXEL_SHADER * GetPixelShader() const;

			void SetPixelShader(I_PIXEL_SHADER * pShader);
			void SetDescription(UInt32 uiDescriptor);

		protected:

			UInt32				_uiDescription;
			I_PIXEL_SHADER *	_pPixelShader;
		};
	}
}

#endif // __M3D_SCENEGRAPH_MATERIAL_SHADERCCONTAINER_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
