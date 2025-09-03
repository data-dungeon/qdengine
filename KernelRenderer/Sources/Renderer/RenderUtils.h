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
//	CLASS:	RENDER_UTILS
//	The RENDER_UTILS class implements ...
//
//	06-06-26:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __KRENDERER_RENDERER_RENDER_UTILS_H__
#define __KRENDERER_RENDERER_RENDER_UTILS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace	QDT
{
	namespace	KRENDERER
	{
		static UInt16 QDT_FLOAT_TO_HALF_I(UInt32 i);
		static UInt32 QDT_HALF_TO_FLOAT_I(UInt16 y);

		static _INLINE_ UInt32 QDT_PACK_NORMAL_11_11_10(Float32 x, Float32 y, Float32 z)
		{
			UInt32 uiX = static_cast<UInt32>((x + 1.0f) * 1023.5f);
			UInt32 uiY = static_cast<UInt32>((x + 1.0f) * 1023.5f);
			UInt32 uiZ = static_cast<UInt32>((x + 1.0f) * 511.5f);
			return (uiX + (uiY << 11) + (uiZ << 22)); 
		}

		static _INLINE_ UInt16 QDT_FLOAT_TO_HALF(Float32 i)
		{
			union { Float32 f; UInt32 i; } v;
			v.f = i;
			return QDT_FLOAT_TO_HALF_I(v.i);
		}

		static _INLINE_ Float32 QDT_HALF_TO_FLOAT(UInt16 y)
		{
			union { Float32 f; UInt32 i; } v;
			v.i = QDT_HALF_TO_FLOAT_I(y);
			return v.f;
		}

		static _INLINE_ UInt16 QDT_FLOAT_TO_HALF_I(UInt32 i)
		{
			register Int32 s =  (i >> 16) & 0x00008000;
			register Int32 e = ((i >> 23) & 0x000000ff) - (127 - 15);
			register Int32 m =   i        & 0x007fffff;

			if (e <= 0)
			{
				if (e < -10)
				{
					return 0;
				}
				m = (m | 0x00800000) >> (1 - e);

				return s | (m >> 13);
			}
			else if (e == 0xff - (127 - 15))
			{
				if (m == 0) // Inf
				{
					return s | 0x7c00;
				} 
				else    // NAN
				{
					m >>= 13;
					return s | 0x7c00 | m | (m == 0);
				}
			}
			else
			{
				if (e > 30) // Overflow
				{
					return s | 0x7c00;
				}

				return s | (e << 10) | (m >> 13);
			}
		}

		static _INLINE_ UInt32 QDT_HALF_TO_FLOAT_I(UInt16 y)
		{
			register Int32 s = (y >> 15) & 0x00000001;
			register Int32 e = (y >> 10) & 0x0000001f;
			register Int32 m =  y        & 0x000003ff;

			if (e == 0)
			{
				if (m == 0) // Plus or minus zero
				{
					return s << 31;
				}
				else // Denormalized number -- renormalize it
				{
					while (!(m & 0x00000400))
					{
						m <<= 1;
						e -=  1;
					}

					e += 1;
					m &= ~0x00000400;
				}
			}
			else if (e == 31)
			{
				if (m == 0) // Inf
				{
					return (s << 31) | 0x7f800000;
				}
				else // NaN
				{
					return (s << 31) | 0x7f800000 | (m << 13);
				}
			}

			e = e + (127 - 15);
			m = m << 13;

			return (s << 31) | (e << 23) | m;
		}
	}
}

#endif // __KRENDERER_RENDERER_RENDER_UTILS_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
