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
//	CLASS:	2DFILTER
//	The 2DFILTER class implements ...
//
//	06-01-12:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __TEST_RENDERER_APPLICATION_UTILITIES_FILTER2D_H__
#define __TEST_RENDERER_APPLICATION_UTILITIES_FILTER2D_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(I_VERTEX_BUFFER)
DECL_KRENDERER(I_VERTEX_DECLARATION)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	FILTER_2D
		{
		public:

			FILTER_2D();
			FILTER_2D(I_PIXEL_SHADER* pFilterShader);
			virtual ~FILTER_2D();
		
			virtual void Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget,
							  UInt32 ScrX, UInt32 ScrY, UInt32 ScrWidth, UInt32 ScrHeight,
							  UInt32 DstX, UInt32 DstY, UInt32 DstWidth, UInt32 DstHeight);

			virtual void Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget);
			virtual void Init(I_RENDER_TARGET* InputTarget, I_RENDER_TARGET* OutputTarget);
			void LightInit();

			void Apply();
   			I_RENDER_TARGET* PingPong(UInt32 Iterations);

			void SetPixelShader(I_PIXEL_SHADER* filter) {_FilterShader = filter; _bIsFilterIdentity = false;};
			I_PIXEL_SHADER* GetPixelShader() {return _FilterShader;};


			void OverrideVertexShader(I_VERTEX_SHADER* vShader);

			void SetQuadMode(Bool val){_bQuadMode = val;}

		protected:

			I_TEXTURE*       _InputImage;
			I_RENDER_TARGET* _InputTarget;
			I_RENDER_TARGET* _OutputTarget;
			I_PIXEL_SHADER*  _FilterShader;
			I_VERTEX_SHADER* _FilterVertexShader;

		private:

			bool _bQuadMode;

			bool _bIsFilterIdentity;

			struct S_FILTER_VERTEX
			{
				Float32 x;
				Float32 y;
				Float32 z;
				Float32 w;
				Float32 u;
				Float32 v;
			};

			static void InitStaticStuff();

			static I_VERTEX_BUFFER* g_VBuffer;
			static I_VERTEX_BUFFER* g_VBufferQuad;
			static I_VERTEX_SHADER* g_VShader;
			static I_PIXEL_SHADER* g_Default_PShader;
			static I_PIXEL_SHADER* g_Default_PShader_square;
			static I_VERTEX_DECLARATION* g_VDecl;
			static Int32 g_refCount;
		};
	}
}

#endif // __TEST_RENDERER_APPLICATION_UTILITIES_FILTER2D_H__


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
