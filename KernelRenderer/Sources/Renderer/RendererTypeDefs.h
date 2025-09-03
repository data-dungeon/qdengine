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
//	CLASS:	RENDER_CONTEXT
//	The RENDER_CONTEXT class implements ...
//
//	05-08-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_TYPEDEFS_H__
#define __KRENDERER_RENDERER_TYPEDEFS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Color/Color)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace	KRENDERER
	{
		typedef enum DISPLAY_MODE
		{
			DM_720P,
			DM_1080I,
			DM_1080P,
			DM_480I,
			DM_576I,
			DM_DEFAULT
		};

		typedef enum __PRIMITIVE_TYPE
		{
			PT_POINT_LIST,     // Specifies a point list  
			PT_LINE_LIST,      // Specifies a line list   
			PT_LINE_STRIP,     // Specifies a line strip  
			PT_TRIANGLE_LIST,  // Specifies a triangle list  
			PT_TRIANGLE_STRIP, // Specifies a triangle strip 
			PT_TRIANGLE_FAN,   // Specifies a triangle fan   
		} PRIMITIVE_TYPE;

		typedef enum __PIXEL_FORMAT
		{
			PF_A8R8G8B8,
			PF_X8R8G8B8,
			PF_A16B16G16R16,
			PF_A32B32G32R32,
			PF_R16,
			PF_DXTC1,
			PF_DXTC3,
			PF_DXTC5,
			PF_D24S8_DEPTHSTENCIL,
			PF_D16,
			PF_L8,
			PF_R32,
			PF_D24S8,
			PF_G32R32,
			PF_G16R16,
			PF_UNKNOWN,
		} PIXEL_FORMAT;

		typedef enum __CUBEMAP_FACE
		{
			CF_POSITIVE_X,
			CF_NEGATIVE_X,
			CF_POSITIVE_Y,
			CF_NEGATIVE_Y,
			CF_POSITIVE_Z,
			CF_NEGATIVE_Z,
			CF_NONE,
		} CUBEMAP_FACE;

		typedef enum __CULL_MODE
		{
			CULL_NONE = 0,
			CULL_CLOCKWISE,
			CULL_COUNTERCLOCKWISE,
		} CULL_MODE;

		typedef enum __CLEAR_FLAGS
		{
			CF_CLEAR_STENCIL = 1<<0,
			CF_CLEAR_COLOR	 = 1<<1,
			CF_CLEAR_ZBUFFER = 1<<2,
		} CLEAR_FLAGS;

		typedef enum __COLOR_CHANNEL
		{
			CC_ALPHA,
			CC_RED,
			CC_GREEN,
			CC_BLUE,
		} COLOR_CHANNEL;

		typedef enum __DECL_TYPE
		{
			DT_FLOAT1,
			DT_FLOAT2,
			DT_FLOAT3,
			DT_FLOAT4,
			DT_COLOR, // 4 chars! (ARGB for DirectX and RGBA for PSGL)
			DT_FLOAT16_2,
			DT_FLOAT16_4,
			DT_UBYTE4,
			DT_UBYTE4N,
			DT_FIXED_11_11_10_PS3, // This is for normals, PS3 only!
		} DECL_TYPE;

		static UInt32	__DECL_TYPE_SIZE[10] = {4, 8, 12, 16, 4, 4, 8, 4, 4, 4};

		typedef enum __DECL_USAGE
		{
			DU_POSITION,
			DU_NORMAL,
			DU_COLOR,
			DU_TEXCOORD0,
			DU_TEXCOORD1,
			DU_TEXCOORD2,
			DU_TEXCOORD3,
			DU_TEXCOORD4,
			DU_TEXCOORD5,
			DU_TEXCOORD6,
			DU_TEXCOORD7,
			DU_BLENDWEIGHT,
			DU_BLENDINDICES,
			DU_TANGENT,
			DU_BINORMAL,
		} DEDUCL_USAGE;

		typedef enum __BLEND_MODE
		{
			BM_ZERO,
			BM_ONE,
			BM_SRCCOLOR,
			BM_INVSRCCOLOR,
			BM_SRCALPHA,
			BM_INVSRCALPHA,
			BM_DSTALPHA,
			BM_INVDESTALPHA,
			BM_DSTCOLOR,
			BM_INVDSTCOLOR,
			BM_SRCALPHASAT,
		} BLEND_MODE;

		typedef enum __ADDRESS_TYPE
		{
			AT_WRAP,
			AT_CLAMP,
			AT_MIRROR,
		} ADDRESS_TYPE;

		typedef enum __FILTER_TYPE
		{
			FT_NONE,
			FT_BILINEAR,
			FT_TRILINEAR,
			FT_ANISOTROPIC_LEVEL_2,
			FT_ANISOTROPIC_LEVEL_4,
			FT_ANISOTROPIC_LEVEL_8,
			FT_ANISOTROPIC_LEVEL_16,
		} FILTER_TYPE;

		typedef enum __FILL_MODE
		{
			FM_LINE = 0,
			FM_SOLID
		} FILL_MODE;

		typedef enum __COMP_FUNC
		{
			CF_NEVER,
			CF_LESS,
			CF_EQUAL,
			CF_LESSEQUAL,
			CF_GREATER,
			CF_NOTEQUAL,
			CF_GREATEREQUAL,
			CF_ALWAYS
		} COMP_FUNC;

		typedef enum __SHADER_MODEL
		{
			SM_D3D_VS_1_1 = 0,
			SM_D3D_VS_2_0,
			SM_D3D_VS_3_0,
			SM_D3D_PS_1_1,
			SM_D3D_PS_1_2,
			SM_D3D_PS_1_3,
			SM_D3D_PS_1_4,
			SM_D3D_PS_2_0,
			SM_D3D_PS_3_0,
		} SHADER_MODEL;

		typedef enum __BLEND_OP
		{
			BOP_ADD,
			BOP_SUBTRACT,
			BOP_REVSUBTRACT,
			BOP_MIN,
			BOP_MAX,
		} BLEND_OP;

		struct DEBUG_VERTEX
		{
			float	_rX, _rY, _rZ;
			UInt32	_nDiffuse;
			DEBUG_VERTEX(void)						{ }
			DEBUG_VERTEX(VECTOR vPos, COLOR Color)	{ _rX=vPos.GetX(); _rY=vPos.GetY(); _rZ=vPos.GetZ(); _nDiffuse=Color.GetARGBValue(); }
		};
	}
}

#endif // __KRENDERER_RENDERER_TYPEDEFS_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
