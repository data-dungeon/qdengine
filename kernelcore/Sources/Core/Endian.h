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
//	CLASS:	ENDIAN
//	The ENDIAN class implements ...
//
//	05-11-22:	VMA - Created
//*****************************************************************************

#ifndef _KCORE_CORE_ENDIAN_H__
#define _KCORE_CORE_ENDIAN_H__
#pragma once

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{
	namespace	KCORE
	{
		class	ENDIAN
		{
		public:
				_INLINE_ static void Create();
				_INLINE_ static void Release();
				_INLINE_ static ENDIAN *	Instance();
				_INLINE_ static void SetInstance(ENDIAN * pInstance);
				_INLINE_ static bool Exist();
				
				_INLINE_ ENDIAN();
				_INLINE_ ~ENDIAN();

				_INLINE_ void	EnableSwap(Bool	bSwap);
				_INLINE_ void	EnableSwap();
				_INLINE_ void	DisableSwap();
				_INLINE_ Bool	IsSwapEnabled();

				Int8    Convert( Int8    nVal );
				Int16   Convert( Int16   nVal );
				Int32   Convert( Int32   nVal );
				Int64   Convert( Int64   nVal );
				UInt8   Convert( UInt8   nVal );
				UInt16  Convert( UInt16  nVal );
				UInt32  Convert( UInt32  nVal );
				UInt64  Convert( UInt64  nVal );
				Float32 Convert( Float32 fVal );
				Float64 Convert( Float64 fVal );

				void    ConvertBuffer( Int8    * pBuffer,  UInt32 nSize );
				void	ConvertBuffer( Int16   * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( Int32   * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( Int64   * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( UInt8   * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( UInt16  * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( UInt32  * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( UInt64  * pBuffer,  UInt32 nSize );
				void    ConvertBuffer( Float32 * pBuffer,  UInt32 nSize );
				void	ConvertBuffer( Float64 * pBuffer,  UInt32 nSize );

		private:
				Bool _bSwap;

				static	ENDIAN *	_pInstance;
		};
	}
}

#include INCL_KCORE(Core/Assert)

#ifndef _DEBUG
#include "Endian.inl"
#endif

#endif //_KCORE_CORE_ENDIAN_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
