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
//	CLASS:	FILTER_2DCONVOLUTION
//	The FILTER_2DCONVOLUTION class implements ...
//
//	06-01-16:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __TEST_RENDERER_APPLICATION_UTILITIES_FILTER_2DCONVOLUTION_H__
#define __TEST_RENDERER_APPLICATION_UTILITIES_FILTER_2DCONVOLUTION_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif


DECL_KRENDERER(I_RENDER_TARGET)

DECL_KMATH(VECTOR_2)
DECL_KMATH(VECTOR)

#include INCL_KRENDERER(PostProcessing\Filter2D)

namespace	QDT
{
	namespace	KRENDERER
	{
		class	FILTER_2DCONVOLUTION : public FILTER_2D
		{
		public:

			FILTER_2DCONVOLUTION();
			explicit FILTER_2DCONVOLUTION(const FILTER_2DCONVOLUTION & C);
			~FILTER_2DCONVOLUTION();

			void SetOffsets(VECTOR_2* offests);
			void SetWeights(VECTOR* coeffs);
			void EnableFixedKernel(Bool value = true);


			void InitCircleBlur(Float32 radius);
			void InitBoxBlur16(Float32 radius);
			void InitDirectionalGaussianBlur(Float32 DirX, Float32 DirY);
			void InitDirectionalGaussianBlur16(Float32 DirX, Float32 DirY);
			void InitPoissonDisk();
			void InitDiamond();
			void InitStarBranch(Float32 DirX, Float32 DirY, Float32 Diffraction);

			void TranslateKernel(Float32 trX, Float32 trY);
			void ScaleKernel(Float32 scaleX, Float32 scaleY);

			void SetNumSample(UInt32 num) {_numSample = num;};
			void SetUseNoWeights(Bool val) {_UseNoweights = val;};
			void SetUseNoOffsets(Bool val) {_UseNoOffsets = val;};			

			void OverridePixelShader(I_PIXEL_SHADER* pShader);

			virtual void Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget,
							  UInt32 ScrX, UInt32 ScrY, UInt32 ScrWidth, UInt32 ScrHeight,
							  UInt32 DstX, UInt32 DstY, UInt32 DstWidth, UInt32 DstHeight);

			virtual void Init(I_TEXTURE* InputImage, I_RENDER_TARGET* OutputTarget);
			virtual void Init(I_RENDER_TARGET* InputTarget, I_RENDER_TARGET* OutputTarget);

		private:

			I_PIXEL_SHADER* _pShaderOverride;

			Bool        _FixedKernel;
			Bool        _UseNoweights,
						_UseNoOffsets;
			Bool        _PixelShaderOverriden;
			Float32   	_Offsets[16 * 2];
			Float32	    _Weights[16 * 4];
			UInt32      _numSample;
			I_PIXEL_SHADER*  _Convolution;

			
			static Int32 g_refcount;

			static I_PIXEL_SHADER*  g_Convolution8;
			static I_PIXEL_SHADER*  g_Convolution16;
			static I_PIXEL_SHADER*  g_Convolution8_fixed;

			static I_PIXEL_SHADER*  g_Convolution8_square;
			static I_PIXEL_SHADER*  g_Convolution16_square;
			static I_PIXEL_SHADER*  g_Convolution8_fixed_square;

			static I_VERTEX_SHADER* g_VertexShader;
			static I_VERTEX_SHADER* g_VertexShader_fixed;

		};
	}
}

#endif // __TEST_RENDERER_APPLICATION_UTILITIES_FILTER_2DCONVOLUTION_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
