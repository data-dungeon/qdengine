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
//	CLASS:	MACROS
//	The MACROS class implements ...
//
//	05-09-21:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#ifndef __KCORE_CORE_MACROS_H__
#define __KCORE_CORE_MACROS_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace QDT
{
	namespace KCORE
	{

		#ifndef _DEBUG
			#ifdef PLATFORM_PS3
				//__attribute((always_inline)) inline
				#define _INLINE_ inline
			#else
				#define _INLINE_ inline
				#pragma	inline_depth(32)
				#pragma	auto_inline(on)
			#endif
		#else
			#define _INLINE_ 
		#endif

		#define _EXPLICIT_		explicit

		#ifdef PLATFORM_PC
			#define QDT_DLL_EXPORT	__declspec( dllexport )
			#define _CDECL_			__cdecl
		#else
			#define QDT_DLL_EXPORT
			#define _CDECL_
		#endif

		#if (defined _MASTER) || (defined PLATFORM_PS3) || (defined PLATFORM_X360)
			#define QDT_TRY
			#define QDT_UNHANDLED_CATCH
			#define QDT_CATCH if(0)
		#elif (defined QDT_DEBUG) // aka FastDebug
			#define QDT_TRY try
			#define QDT_UNHANDLED_CATCH catch(...){ QDT_Error("TRY FAILED in file %s line %d",__FILE__, __LINE__);}
			#define QDT_CATCH catch(...)
		#else
			#define QDT_TRY try
			#define QDT_UNHANDLED_CATCH catch(...){ QDT_Error("TRY FAILED in file %s line %d",__FILE__, __LINE__);}
			#define QDT_CATCH catch(...)
		#endif

		#define QDT_ASSERT_VU0_ADDRESS_ALIGNMENT(v)
		#define QDT_ASSERT_SSE_ADDRESS_ALIGNEMENT(v)		{ QDT_ASSERT_ADDRESS_ALIGNMENT(v,0); }	// 16 bytes alignement

		#define	QDT_READ_ONLY_ACCESS(pt, type)
		#define	QDT_WRITE_ONLY_ACCESS(pt, type)
		#define	QDT_READ_WRITE_ACCESS(pt, type)


		#ifdef _MASTER
			#define QDT_COMMENT(str) NULL
		#else
			#define QDT_COMMENT(str) str
		#endif

		#define	QDT_FORCE_USE_MATH			<KernelCore/Sources/Core/ForceUseMath.h>
		#define QDT_DISABLE_FORCE_USE_MATH	<KernelCore/Sources/Core/DisableForceUseMath.h>		

		#if (defined PLATFORM_PS3)
			#define HOME_DIR "/app_home/"
		#elif (defined PLATFORM_X360)
			#define HOME_DIR "game:\\"
		#else
			#define HOME_DIR
		#endif

		#ifdef PLATFORM_PS3
			#define QDT_ALIGN(n) __attribute__ ((aligned(n)))
		#else
			#define QDT_ALIGN(n) __declspec(align(n)) 
		#endif


		#ifdef PLATFORM_PC 
			#define QDT_CREATE_VERTEXSHADER( ShaderName ) \
					((RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RENDER_CONTEXT_TYPE::RCT_DIRECT3D9)? \
					 (RENDER_CONTEXT::CreateVertexShader ((UInt8*) ShaderName ## _D3D, sizeof(ShaderName ## _D3D))) : \
					 (RENDER_CONTEXT::CreateVertexShader((UInt8*) ShaderName ## _OGL, sizeof(ShaderName ## _OGL))) )
						
			#define QDT_CREATE_PIXELSHADER( ShaderName ) \
					((RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RENDER_CONTEXT_TYPE::RCT_DIRECT3D9)? \
						(RENDER_CONTEXT::CreatePixelShader((UInt8*) ShaderName ## _D3D, sizeof(ShaderName ## _D3D))) : \
					    (RENDER_CONTEXT::CreatePixelShader((UInt8*) ShaderName ## _OGL, sizeof(ShaderName ## _OGL))) )		
		#endif

		#ifdef PLATFORM_PS3
			#define QDT_CREATE_VERTEXSHADER( ShaderName ) \
					 (RENDER_CONTEXT::CreateVertexShader((UInt8*) ShaderName ## _RSX, sizeof(ShaderName ## _RSX))) 
						
			#define QDT_CREATE_PIXELSHADER( ShaderName ) \
					(RENDER_CONTEXT::CreatePixelShader((UInt8*) ShaderName ## _RSX, sizeof(ShaderName ## _RSX)))
		#endif

		#ifdef PLATFORM_X360
			#define QDT_CREATE_VERTEXSHADER( ShaderName ) \
					 (RENDER_CONTEXT::CreateVertexShader((UInt8*) ShaderName ## _X360, sizeof(ShaderName ## _X360))) 
						
			#define QDT_CREATE_PIXELSHADER( ShaderName ) \
					(RENDER_CONTEXT::CreatePixelShader((UInt8*) ShaderName ## _X360, sizeof(ShaderName ## _X360)))
		#endif


		#if		(defined PLATFORM_PC)
			#define	QDT_APPEND_PLATFORM(string) string##Gen
		#elif	(defined PLATFORM_PS3)
			#define	QDT_APPEND_PLATFORM(string) string##PS3
		#else
			#define	QDT_APPEND_PLATFORM(string) string##Gen
		#endif

		#if		(defined PLATFORM_PC)
			#define LITTLE_ENDIAN
		#elif	(defined PLATFORM_PS3)
			#define BIG_ENDIAN
		#elif	(defined PLATFORM_X360)
			#define BIG_ENDIAN
		#else
			#error	UNKNOWN PLATFORM
		#endif

		//#undef memcpy
		#define QDT_MEMCOPY	memcpy		
		//#define memcpy NO_MEMCPY_LINKED_USE_QDT_MEMCOPY
	}
}
#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
