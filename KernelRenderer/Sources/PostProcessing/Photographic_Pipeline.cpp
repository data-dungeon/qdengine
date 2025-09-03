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
//	CLASS:	PHOTOGRAPHIC_PIPELINE
//
//	06-01-24:	PDE - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include "Root.h"
#include INCL_KRENDERER(PostProcessing\Photographic_Pipeline)
#include INCL_KRENDERER(Renderer\RenderContext)
#include INCL_KRENDERER(Renderer\IVertexShader)
#include INCL_KRENDERER(Renderer\IPixelShader)
#include INCL_KRENDERER(Renderer\ITexture)
#include INCL_KRENDERER(Renderer\IRenderTarget)
#include INCL_KRENDERER(PostProcessing\Filter2D)
#include INCL_KRENDERER(PostProcessing\Filter2DConvolution)
#include INCL_KRENDERER(PostProcessing\ViewportState)
#include INCL_KCORE(Time\Time_ABC)
#include INCL_KMATH(Math\Math)

#ifdef PLATFORM_PC
	#include INCL_KRENDERER(Renderer/OGL_PC/RenderTarget_OGL_PC)
#endif

#include "shaders/LightCompositing_ps.h"
#include "shaders/ToneMapping_ps.h"
#include "shaders/LuminanceExtraction_ps.h"
#include "shaders/BrightPass_ps.h"
#include "shaders/AverageTextures_ps.h"
#include "shaders/ViewportCompositing_vs.h"
#include "shaders/ViewportCompositing_ps.h"
#include "shaders/LightDebug_ps.h"
#include "shaders/Average16_ps.h"

#ifdef PLATFORM_X360
	#include "shaders/AdaptativeKeyLight_X360_ps.h"
	#include "shaders/OverrideAlpha_X360_ps.h"
#else
	#include "shaders/AdaptativeKeyLight_ps.h"
	#include "shaders/OverrideAlpha_ps.h"
#endif


//=============================================================================
//	CODE STARTS HERE
//=============================================================================


Bool PHOTOGRAPHIC_PIPELINE::_bTargetNormalized = false;
Bool PHOTOGRAPHIC_PIPELINE::_bHDRMode = false;
Bool PHOTOGRAPHIC_PIPELINE::_bLegacyHardware = false;
Bool PHOTOGRAPHIC_PIPELINE::_bInitialized = false;
Bool PHOTOGRAPHIC_PIPELINE::_bDebugDOF = false;
Bool PHOTOGRAPHIC_PIPELINE::_bDebugLight = false;
Bool PHOTOGRAPHIC_PIPELINE::_bMotionBlurInitialized = false;

VIEWPORT_STATE* PHOTOGRAPHIC_PIPELINE::_pViewportState = NULL;

Float32 PHOTOGRAPHIC_PIPELINE::_fMotionBlurImportance   = 0.3f;
Float32 PHOTOGRAPHIC_PIPELINE::_fMotionBlurPersistance  = 0.2f;

FILTER_2D*			PHOTOGRAPHIC_PIPELINE::_pViewportCompositing    = NULL;
I_VERTEX_SHADER*	PHOTOGRAPHIC_PIPELINE::_pViewportCompositing_vs = NULL;
I_PIXEL_SHADER** 	PHOTOGRAPHIC_PIPELINE::_pViewportCompositing_ps = NULL;

I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pLightCompositing_ps[9] = {NULL, NULL, NULL, NULL, NULL,
																		   NULL, NULL, NULL, NULL};

I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pAverageTextures_ps[3] = {NULL, NULL, NULL};

I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pToneMap_ps        = NULL;
I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pLightDebug_ps     = NULL;
I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pLuminance_ps      = NULL;
I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pAdaptKeyLight_ps  = NULL;
I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pBrightPass_ps     = NULL;
I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pMotionBlur_ps     = NULL;
I_PIXEL_SHADER*			PHOTOGRAPHIC_PIPELINE::_pAverage16_ps    = NULL;

I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetFrameHDR		   = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetFrameLDR		   = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetFrameMotionBlur = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetCompositing	   = NULL;

//I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetFocal		   = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetDepth		   = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetBlur1		   = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetBlur2		   = NULL;
I_RENDER_TARGET*		PHOTOGRAPHIC_PIPELINE::_pTargetBloom		   = NULL;
I_RENDER_TARGET* 	    PHOTOGRAPHIC_PIPELINE::_pTargetBright          = NULL;

I_RENDER_TARGET**	    PHOTOGRAPHIC_PIPELINE::_pTargetsKey            = NULL;
I_RENDER_TARGET**	    PHOTOGRAPHIC_PIPELINE::_pViewportKey           = NULL;
VIEWPORT_STATE**        PHOTOGRAPHIC_PIPELINE::_pKeylightBindings      = NULL;
VIEWPORT_STATE*         PHOTOGRAPHIC_PIPELINE::_pMotionblurViewport    = NULL;
I_RENDER_TARGET**	    PHOTOGRAPHIC_PIPELINE::_pTargetsStar           = NULL;

I_TEXTURE*				PHOTOGRAPHIC_PIPELINE::_pNoiseTexture		   = NULL;


FILTER_2DCONVOLUTION*	  PHOTOGRAPHIC_PIPELINE::_pGaussianH		   = NULL;
FILTER_2DCONVOLUTION*	  PHOTOGRAPHIC_PIPELINE::_pGaussianV		   = NULL;
FILTER_2DCONVOLUTION*	  PHOTOGRAPHIC_PIPELINE::_pBloomH			   = NULL;
FILTER_2DCONVOLUTION*	  PHOTOGRAPHIC_PIPELINE::_pBloomV			   = NULL;
FILTER_2DCONVOLUTION*	  PHOTOGRAPHIC_PIPELINE::_pLightCompositing	   = NULL;
FILTER_2DCONVOLUTION**    PHOTOGRAPHIC_PIPELINE::_pDownScale4Square    = NULL;
FILTER_2DCONVOLUTION*     PHOTOGRAPHIC_PIPELINE::_pStarBranch          = NULL;
FILTER_2DCONVOLUTION*     PHOTOGRAPHIC_PIPELINE::_pDownScaleDof        = NULL;

FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pFilterIdentity    = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pLuminance         = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pAdaptKey          = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pTonemapping       = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pBrightPass        = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pAverageTextures   = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pLightDebug        = NULL;
FILTER_2D*				PHOTOGRAPHIC_PIPELINE::_pMotionBlur        = NULL;

UInt32 PHOTOGRAPHIC_PIPELINE::_uiMaxStarBranches = 0;
UInt32 PHOTOGRAPHIC_PIPELINE::_uiMaxViewports      = 0;

UInt32			 	    PHOTOGRAPHIC_PIPELINE::_uiPhotoPipelineID = 0;



UInt32		   		    PHOTOGRAPHIC_PIPELINE::_uiLowresFrameWidth   = 0;
UInt32		   		    PHOTOGRAPHIC_PIPELINE::_uiLowresFrameHeight  = 0;
UInt32		   		    PHOTOGRAPHIC_PIPELINE::_uiHighresBorderSize  = 0;
Float32			 	    PHOTOGRAPHIC_PIPELINE::_fLowresScale		 = 0;


Float32					PHOTOGRAPHIC_PIPELINE::_fKeyLightValue       = 0.0f;





float PHOTOGRAPHIC_PIPELINE::_pRainBow[32] =
{
		0.0f, 1.0f, 0.8f, 1.0f,
		0.0f, 0.2f, 1.0f, 1.0f,
		0.58f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.5f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
};


//-----------------------------------------------------------------------------
//	Name:		InitViewPort
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void PHOTOGRAPHIC_PIPELINE::InitViewPort(VIEWPORT_STATE *viewport, UInt8 ClearR, UInt8 ClearG, UInt8 ClearB)
{
	_pViewportState = viewport;

	if (_bLegacyHardware)
	{
		RENDER_CONTEXT::SetCurrentRenderTarget(_pTargetCompositing);
		UInt32 pixelWidth  = UInt32( _pViewportState->_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth() );
		UInt32 pixelHeight = UInt32( _pViewportState->_uiViewPortHeight * RENDER_CONTEXT::GetScreenHeight() );
		RENDER_CONTEXT::SetViewport(0, 0, pixelWidth, pixelHeight);
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, ClearR, ClearG, ClearB, 255);
	}
	else
	{
		if (_bHDRMode)
			RENDER_CONTEXT::SetCurrentRenderTarget(_pTargetFrameHDR);
		else
			RENDER_CONTEXT::SetCurrentRenderTarget(_pTargetFrameLDR);

		RENDER_CONTEXT::SetCurrentDepthBuffer(_pTargetDepth);

		UInt32 pixelWidth  = (UInt32)( _pViewportState->_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth() );
		UInt32 pixelHeight = (UInt32)( _pViewportState->_uiViewPortHeight * RENDER_CONTEXT::GetScreenHeight() );

		RENDER_CONTEXT::SetViewport(0, 0, _pTargetFrameLDR->GetWidth(), _pTargetFrameLDR->GetHeight());
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, ClearR, ClearG, ClearB, 255);
		RENDER_CONTEXT::SetViewport(0, 0, pixelWidth  + _uiHighresBorderSize * 2, pixelHeight + _uiHighresBorderSize * 2);
	}

	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
}



//-----------------------------------------------------------------------------
//	Name:		ToneMapImage
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::ToneMapImage()
{
	QDT_ASSERT(_bHDRMode);

	if (_pViewportState->_bAutoExposureEnabled)
		ComputeKeyLight();

//	ClearTarget(_pTargetFrameLDR);

	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);

	UInt32 pixelWidth  = (UInt32)( _pViewportState->_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth() );
	UInt32 pixelHeight = (UInt32)( _pViewportState->_uiViewPortHeight * RENDER_CONTEXT::GetScreenHeight() );

	UInt32 SecurityWidth  = pixelWidth + _uiHighresBorderSize * 2;
	UInt32 SecurityHeight = pixelHeight + _uiHighresBorderSize * 2;

	FILTER_2D* tonemapOp = _bDebugLight? _pLightDebug : _pTonemapping;

	tonemapOp->Init(_pTargetFrameHDR->GetTexture(), _pTargetFrameLDR,
					0, 0, SecurityWidth, SecurityHeight,
					0, 0, SecurityWidth, SecurityHeight);

	RENDER_CONTEXT::SetPixelShaderTexture(1, _pViewportState->_pKeyLight->GetTexture());
	RENDER_CONTEXT::SetPixelShaderConstant(0, _pViewportState->_fExposureControl);

	tonemapOp->Apply();

//#ifdef _DEBUG
	RENDER_CONTEXT::SetPixelShaderTexture(1, NULL);
//#endif
}


//-----------------------------------------------------------------------------
//	Name:		DoCompositing
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void PHOTOGRAPHIC_PIPELINE::DoCompositing()
{
	if (_bHDRMode)
		ToneMapImage();

	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
	RENDER_CONTEXT::EnableColorWrite(true, true, true, true);
	RENDER_CONTEXT::EnableAlphaTest(false);
	RENDER_CONTEXT::SetAlphaTestFunc(CF_ALWAYS);

	UInt32 pixelWidth  = (UInt32)( _pViewportState->_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth() );
	UInt32 pixelHeight = (UInt32)( _pViewportState->_uiViewPortHeight * RENDER_CONTEXT::GetScreenHeight() );

	UInt32 SecurityWidth  = pixelWidth + _uiHighresBorderSize * 2;
	UInt32 SecurityHeight = pixelHeight + _uiHighresBorderSize * 2;

	UInt32 LowSecurityWidth  = (UInt32)( SecurityWidth  * _fLowresScale );
	UInt32 LowSecurityHeight = (UInt32)( SecurityHeight * _fLowresScale );

	if (_bHDRMode)
	{
		// Compute Bright pass into _pTargetBright
		if (_pViewportState->_bBloomEnabled ||
			_pViewportState->_bStarsEnabled)
		{
			ClearTarget(_pTargetBright);

			_pBrightPass->Init(_pTargetFrameHDR->GetTexture(), _pTargetBright,
								0, 0, SecurityWidth, SecurityHeight,
								0, 0, LowSecurityWidth, LowSecurityHeight);
			RENDER_CONTEXT::SetPixelShaderConstant(0, _pViewportState->_fExposureControl); // exposure
			RENDER_CONTEXT::SetPixelShaderConstant(1, _pViewportState->_fBloomThreshold); // bias
			RENDER_CONTEXT::SetPixelShaderConstant(2, _pViewportState->_fBloomIntensity); // scale
			RENDER_CONTEXT::SetPixelShaderTexture(1, _pViewportState->_pKeyLight->GetTexture());
			_pBrightPass->Apply();
			RENDER_CONTEXT::SetPixelShaderTexture(1, NULL);
		}



		// Blur the hell out of it into TargetBloom
		if (_pViewportState->_bBloomEnabled)
		{
			ClearTarget(_pTargetBlur1); 
			ClearTarget(_pTargetBloom);

			CommitBloomRadius(_pViewportState->_fBloomRadius);

			_pBloomH->Init(_pTargetBright->GetTexture(), _pTargetBlur1, 
							0, 0, LowSecurityWidth, LowSecurityHeight,
							0, 0, LowSecurityWidth, LowSecurityHeight); // Horizontal

			_pBloomH->Apply();
			_pBloomV->Init(_pTargetBlur1->GetTexture(), _pTargetBloom,
							0, 0, LowSecurityWidth, LowSecurityHeight,
							0, 0, LowSecurityWidth, LowSecurityHeight); // Vertical
			_pBloomV->Apply();
		}
	}

	// Downscale and blur the frame Image into _TargetBlur1 (uses TargetBlur2)

	if (_pViewportState->_bDofEnabled && !_bLegacyHardware)
	{
		_pDownScaleDof->Init(_pTargetFrameLDR->GetTexture(), _pTargetBlur1, 0, 0, SecurityWidth, SecurityHeight,
																			0, 0, LowSecurityWidth, LowSecurityHeight);
		_pDownScaleDof->Apply();

		// Apply a separable Gaussian blur
	
		//ClearTarget(_pTargetBlur2);
		
		_pGaussianH->Init(_pTargetBlur1->GetTexture(), _pTargetBlur2, 
						0, 0, LowSecurityWidth, LowSecurityHeight,
						0, 0, LowSecurityWidth, LowSecurityHeight); // Horizontal

		_pGaussianH->Apply();
		_pGaussianV->Init(_pTargetBlur2->GetTexture(), _pTargetBlur1,
						0, 0, LowSecurityWidth, LowSecurityHeight,
						0, 0, LowSecurityWidth, LowSecurityHeight); // Vertical
		_pGaussianV->Apply();
	
/*
		_pFilterIdentity->Init(_pTargetBlur1->GetTexture(), _pTargetBlur2, 
						0, 0, LowSecurityWidth, LowSecurityHeight,
						0, 0, LowSecurityWidth, LowSecurityHeight); // Horizontal

		_pFilterIdentity->Apply();
		_pFilterIdentity->Init(_pTargetBlur2->GetTexture(), _pTargetBlur1,
						0, 0, LowSecurityWidth, LowSecurityHeight,
						0, 0, LowSecurityWidth, LowSecurityHeight); // Vertical
		_pFilterIdentity->Apply();
*/
	}

	///------------------Compute Star branches (uses TargetBlur2)

	if (_bHDRMode)
	{
		if (_pViewportState->_bStarsEnabled)
		{
			QDT_ASSERT(_pViewportState->_uiNumStarBranches <= _uiMaxStarBranches);

			ClearTarget(_pTargetBlur2);

			for (UInt32 branch = 0; branch < _pViewportState->_uiNumStarBranches; branch++)
			{
				
				Float32 angle = _pViewportState->_fStarRotate  + (6.2831f / (Float32)_pViewportState->_uiNumStarBranches) * branch;

				Float32 dx = MATH::Cos(angle) * _pViewportState->_fStarScale;
				Float32 dy = MATH::Sin(angle) * _pViewportState->_fStarScale;

				Float32 tgtWidth = (Float32)_pTargetBlur1->GetWidth();
				Float32 tgtHeight = (Float32)_pTargetBlur1->GetHeight();

				if (_bTargetNormalized)
				{
					dx /= tgtWidth;
					dy /= tgtHeight;
				}

				_pStarBranch->InitStarBranch(dx, dy, _pViewportState->_fStarDiffraction);

				I_RENDER_TARGET* branchTarget = _pTargetsStar[branch];
				ClearTarget(branchTarget);
	

				_pStarBranch->Init(_pTargetBright->GetTexture(), branchTarget, 	0, 0, LowSecurityWidth, LowSecurityHeight,
																				0, 0, LowSecurityWidth, LowSecurityHeight);
				_pStarBranch->Apply();

				QDT_ASSERT(_pViewportState->_uiNumStarPasses % 2 == 1); // the number of passes must be odd
				for (UInt32 pass = 0; pass < _pViewportState->_uiNumStarPasses - 1; pass++) // pingpong n - 1 times using the same init
				{
					if (pass & 0x1)
					{
						RENDER_CONTEXT::SetCurrentRenderTarget(branchTarget);
						RENDER_CONTEXT::SetPixelShaderTexture(0, _pTargetBlur2->GetTexture());
					}
					else
					{
						RENDER_CONTEXT::SetCurrentRenderTarget(_pTargetBlur2);
						RENDER_CONTEXT::SetPixelShaderTexture(0, pass ? branchTarget->GetTexture() : _pTargetBright->GetTexture());			
					}
					RENDER_CONTEXT::SetViewport(0, 0, LowSecurityWidth, LowSecurityHeight);
					_pStarBranch->Apply();
				}

			}
		}


		///------------------Merge Star branches into TargetBlur2
		if (_pViewportState->_bStarsEnabled)
		{
			_pAverageTextures->SetPixelShader(_pAverageTextures_ps[_pViewportState->_uiNumStarBranches / 2 - 1]);
			_pAverageTextures->Init(_pTargetsStar[0]->GetTexture(), _pTargetBlur2,
									0, 0, LowSecurityWidth, LowSecurityHeight,
									0, 0, LowSecurityWidth, LowSecurityHeight);
			for (UInt32 numBind = 1; numBind < _pViewportState->_uiNumStarBranches; numBind++)
			{
				RENDER_CONTEXT::SetPixelShaderTexture(numBind, _pTargetsStar[numBind]->GetTexture());
			}

			RENDER_CONTEXT::SetPixelShaderConstant(0, _pViewportState->_fStarIntensity);

			_pAverageTextures->Apply();

			for (UInt32 numBind = 1; numBind < _pViewportState->_uiNumStarBranches; numBind++)
			{
				RENDER_CONTEXT::SetPixelShaderTexture(numBind, NULL);
			}
		}

	}
	
	//---- Perform light compositing into the Compositing target
	RENDER_CONTEXT::SetBlendMode(BM_SRCALPHA, BOP_ADD, BM_INVSRCALPHA);

	if (!_bLegacyHardware)
	{

		UInt32 ShaderId = 0;
		if (_pViewportState->_bStarsEnabled && _bHDRMode)  ShaderId += 1;
		if (_pViewportState->_bBloomEnabled && _bHDRMode)  ShaderId += 2;
		if (_pViewportState->_bDofEnabled && !_bLegacyHardware) ShaderId += 4;
		if (_bDebugDOF) ShaderId = 8;
		else if (_bDebugLight) ShaderId = 0;

		bool debugMode = _bDebugDOF || _bDebugLight;
		
		_pLightCompositing->OverridePixelShader(_pLightCompositing_ps[ShaderId]);


		_pLightCompositing->SetUseNoWeights(true);
		_pLightCompositing->EnableFixedKernel(false);
		_pLightCompositing->SetUseNoOffsets(true);//!_pViewportState->_bDofEnabled || debugMode );

		_pLightCompositing->Init(_pTargetFrameLDR->GetTexture(), _pTargetCompositing,
								_uiHighresBorderSize, _uiHighresBorderSize, pixelWidth, pixelHeight,
								0, 0, pixelWidth, pixelHeight);

		if (_pViewportState->_bDofEnabled && !debugMode)
		{
			Float32 diamX = _pViewportState->_fCircleOfConfusion;
			Float32 diamY = _pViewportState->_fCircleOfConfusion;
			
			if (_pTargetBlur2->GetTexture()->GetUVNormalized())
			{
				diamX /= (Float32)_pTargetCompositing->GetWidth();
				diamY /= (Float32)_pTargetCompositing->GetHeight();
			}

			RENDER_CONTEXT::SetPixelShaderConstant(0, diamX, diamY, 0.0f, 0.0f);
		}

		if (_bDebugDOF)
		{
			RENDER_CONTEXT::SetPixelShaderConstant(6, _pViewportState->_fFocalDistance
													,_pViewportState->_fFocalX 
													,_pViewportState->_fFocalB
													,_pViewportState->_fFocalBias); // focal setup
		}
		else if (!_bDebugLight)
		{
			if (_pViewportState->_bDofEnabled)
			{
				RENDER_CONTEXT::SetPixelShaderConstant(6, _pViewportState->_fFocalDistance ,
														  _pViewportState->_fFocalX ,
														  _pViewportState->_fFocalB,
														  _pViewportState->_fFocalBias); // focal setup

				RENDER_CONTEXT::SetPixelShaderTexture(1, _pTargetBlur1->GetTexture());
			}

			if (_pViewportState->_bBloomEnabled)
				RENDER_CONTEXT::SetPixelShaderTexture(2, _pTargetBloom->GetTexture());
			if (_pViewportState->_bStarsEnabled)
				RENDER_CONTEXT::SetPixelShaderTexture(4, _pTargetBlur2->GetTexture());

			if (_bTargetNormalized)
			{
				RENDER_CONTEXT::SetPixelShaderConstant(4, 1.0f, 1.0f, 0.0f, 0.0f);
			}
			else
			{
				RENDER_CONTEXT::SetPixelShaderConstant(4, _fLowresScale, _fLowresScale, 0, 0);
			}
		}

		
		_pLightCompositing->Apply();


		RENDER_CONTEXT::SetPixelShaderTexture(0, NULL);


		if (!debugMode)
		{
			if (_pViewportState->_bDofEnabled)
				RENDER_CONTEXT::SetPixelShaderTexture(1, NULL);
			if (_pViewportState->_bBloomEnabled)
				RENDER_CONTEXT::SetPixelShaderTexture(2, NULL);
			if (_pViewportState->_bStarsEnabled)
				RENDER_CONTEXT::SetPixelShaderTexture(4, NULL);
		}
	}


	bool bMotionBlur = (_pMotionblurViewport == _pViewportState);
	if(bMotionBlur)
	{
		MixMotionBlur(pixelWidth, pixelHeight);
	}

	//---- Perform viewport compositing into the framebuffer

	UInt32 CompositingId = 0;

	if (!_bDebugDOF && !_bDebugLight)
	{
		if (_pViewportState->_bNoiseEnabled && !_bLegacyHardware) CompositingId += 1;
		if (_pViewportState->_pMaskTexture) CompositingId += 2;
		if (_pViewportState->_bColorControlEnabled && !_bLegacyHardware) CompositingId += 4;
	}

	_pViewportCompositing->SetPixelShader(_pViewportCompositing_ps[CompositingId]);

	_pViewportCompositing->Init(_pTargetCompositing->GetTexture() , RENDER_CONTEXT::GetFrontBuffer(),
		0, 0, pixelWidth, pixelHeight, 0, 0, RENDER_CONTEXT::GetScreenWidth(), RENDER_CONTEXT::GetScreenHeight());

	Float32 w = _pViewportState->_uiViewPortWidth;
	Float32 h = _pViewportState->_uiViewPortHeight;
	Float32 r = RENDER_CONTEXT::GetAspectRatio();

	MATRIX clipToVir = MATRIX(w * 0.5f,  0     , 0, 0,   // move vertex coordinates to virtual
							  0       ,  h / (2.0f * r) , 0, 0,   // screen coordinate space
							  0       ,  0     , 0, 0,
							  w * 0.5f, -h / (2.0f * r) , 0, 1);

	MATRIX virToClip = MATRIX(2.0f, 0, 0, 0,    // move back to clip coordinate space
							  0, 2.0f * r, 0, 0,   
							  0, 0, 0, 0,
							  -1, 1, 0, 1);

	
	MATRIX mat, mat2;
	mat.Mul(clipToVir, _pViewportState->_viewportTransform);
	mat2.Mul(mat, virToClip);
	RENDER_CONTEXT::SetVertexShaderConstant(1, mat2);

	if (!_bDebugDOF && !_bDebugLight)
	{
		if (_pViewportState->_bColorControlEnabled)
		{
			RENDER_CONTEXT::SetPixelShaderConstant(0, _pViewportState->_colorTransform);
		}

		if (_pViewportState->_bNoiseEnabled)
		{
			_pViewportState->_fNoisePhase += TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime() * 0.001f;

			if (_pViewportState->_fNoisePhase > _pViewportState->_fNoisePeriod)
			{
				_pViewportState->_vNoiseOffset1 = _pViewportState->_vNoiseOffset2;
				_pViewportState->_vNoiseOffset2 = VECTOR_2(MATH::Rand(1.0f), MATH::Rand(1.0f));
				_pViewportState->_fNoisePhase = 0;
			}
			
			RENDER_CONTEXT::SetVertexShaderConstant(5, _pViewportState->_vNoiseOffset1.GetX(),
													_pViewportState->_vNoiseOffset1.GetY(),
													_pViewportState->_fNoiseScale, 0.0f );
			RENDER_CONTEXT::SetVertexShaderConstant(6, _pViewportState->_vNoiseOffset2.GetX(),
													_pViewportState->_vNoiseOffset2.GetY(),
													_pViewportState->_fNoiseScale, 0.0f );

			RENDER_CONTEXT::SetPixelShaderConstant(4, _pViewportState->_fNoisePhase / _pViewportState->_fNoisePeriod,
													_pViewportState->_fNoiseCutoff, _pViewportState->_fNoiseIntensity, 0.0f);

			RENDER_CONTEXT::SetPixelShaderTexture(1, _pNoiseTexture);
		}

		if (_pViewportState->_pMaskTexture)
		{   //We use 3 for compatibility with ps_1_3
			RENDER_CONTEXT::SetPixelShaderTexture(3, _pViewportState->_pMaskTexture);
		}
	}

	RENDER_CONTEXT::SetPixelShaderConstant(5, _pViewportState->_fOpacity);

	_pViewportCompositing->Apply();

	RENDER_CONTEXT::SetPixelShaderTexture(0, NULL);



	///-----------------Displays Key light debug

/*	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);

	UInt32 top;
	if (RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
		top = 20;
	else
		top = RENDER_CONTEXT::GetScreenHeight() - 120;

	for (int i = 0; i < 4; i++)
	{
		_pFilterIdentity->Init(_pTargetsKey[i]->GetTexture(), RENDER_CONTEXT::GetFrontBuffer(),
								0, 0, _pTargetsKey[i]->GetWidth(), _pTargetsKey[i]->GetHeight(),
								20 + i * 120, top, 100, 100);
		_pFilterIdentity->Apply();
	}
	

	_pFilterIdentity->Init(_pViewportState->_pKeyLight->GetTexture(), RENDER_CONTEXT::GetFrontBuffer(),
							0, 0, _pViewportState->_pKeyLight->GetWidth(), _pViewportState->_pKeyLight->GetHeight(),
							20 + 4 * 120, top, 100, 100);
	_pFilterIdentity->Apply();

	RENDER_CONTEXT::SetCurrentRenderTarget(RENDER_CONTEXT::GetFrontBuffer());
	RENDER_CONTEXT::SetCurrentDepthBuffer(RENDER_CONTEXT::GetDepthBuffer());
*/
	RENDER_CONTEXT::EnableDepthTest(true);
	RENDER_CONTEXT::EnableDepthWrite(true);
}

//-----------------------------------------------------------------------------
//	Name:		SetBloomRadius
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------

static Float32 _fBloomRadCachedvalue = -666.0f; 
void PHOTOGRAPHIC_PIPELINE::CommitBloomRadius(Float32 value)
{
	if (_fBloomRadCachedvalue != value)
	{
		_fBloomRadCachedvalue = value;

		_pBloomH->InitDirectionalGaussianBlur16(value, 0.0f);
		_pBloomV->InitDirectionalGaussianBlur16(0.0, value);

		if (_bTargetNormalized)
		{
			UInt32 blurWidth  = _pTargetBloom->GetWidth();
			UInt32 blurHeight = _pTargetBloom->GetHeight();
			_pBloomH->ScaleKernel(1.0f / blurWidth, 1.0f / blurHeight);
			_pBloomV->ScaleKernel(1.0f / blurWidth, 1.0f / blurHeight);
		}
	}
}



//-----------------------------------------------------------------------------
//	Name:		CommitCircleOfConfusion
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------

void PHOTOGRAPHIC_PIPELINE::CommitCircleOfConfusion(Float32 value)
{
	UInt32 frameWidth  = _pTargetCompositing->GetWidth();
	UInt32 frameHeight = _pTargetCompositing->GetHeight();
	_pLightCompositing->InitDiamond();

	if (_pTargetBlur2->GetTexture()->GetUVNormalized())
	{
		_pLightCompositing->ScaleKernel(_pViewportState->_fCircleOfConfusion / frameWidth, _pViewportState->_fCircleOfConfusion / frameHeight);
	}
	else
	{
		_pLightCompositing->ScaleKernel(_pViewportState->_fCircleOfConfusion, _pViewportState->_fCircleOfConfusion);
	}

	_pLightCompositing->SetUseNoWeights(true);
	_pLightCompositing->EnableFixedKernel(false);
}

//-----------------------------------------------------------------------------
//	Name:		ComputeKeyLight
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void PHOTOGRAPHIC_PIPELINE::ComputeKeyLight()
{
	BindKeyTarget(_pViewportState);

	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);

	UInt32 pixelWidth  = (UInt32)( _pViewportState->_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth() );
	UInt32 pixelHeight = (UInt32)( _pViewportState->_uiViewPortHeight * RENDER_CONTEXT::GetScreenHeight() );

	UInt32 frameWidth  = pixelWidth  + (_uiHighresBorderSize * 2);
	UInt32 frameHeight = pixelHeight + (_uiHighresBorderSize * 2);
	_pLuminance->Init(_pTargetFrameHDR->GetTexture(), _pTargetsKey[0], 0, 0, frameWidth, frameHeight, 0, 0, 64, 64);
	_pLuminance->Apply();

	for (int i = 0; i < 3; i++)
	{
		UInt32 size = _pTargetsKey[i]->GetWidth();
		_pDownScale4Square[i]->Init(_pTargetsKey[i]->GetTexture(), _pTargetsKey[i + 1],
									 0, 0, size, size, 0, 0, size / 4, size / 4);
		_pDownScale4Square[i]->Apply();
	}

	//RENDER_CONTEXT::SetPixelShaderTexture(0, NULL);

	_pAdaptKey->Init(_pTargetsKey[3], _pViewportState->_pKeyLight);
	RENDER_CONTEXT::SetBlendMode(BM_SRCALPHA, BOP_ADD, BM_INVSRCALPHA);


	RENDER_CONTEXT::SetPixelShaderConstant(1, _pViewportState->_fMinKey);
	RENDER_CONTEXT::SetPixelShaderConstant(2, _pViewportState->_fMaxKey);
	RENDER_CONTEXT::SetPixelShaderConstant(3, _pViewportState->_fManualKey);
	RENDER_CONTEXT::SetPixelShaderConstant(4, _pViewportState->_fManualRatio);

	
#ifdef PLATFORM_X360

	float delta = 1.0f;

	if (_pViewportState->_bKeyLightSet && _pViewportState->_fAdaptTime > 0.0f)
		delta = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime() * 0.001f / _pViewportState->_fAdaptTime;

	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
	RENDER_CONTEXT::SetPixelShaderConstant(0, delta);
	RENDER_CONTEXT::SetPixelShaderTexture(1, _pViewportState->_pKeyLight->GetTexture());
	_pViewportState->_bKeyLightSet = true;

#else

	if (_pViewportState->_bKeyLightSet)
	{
		float delta = TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime() * 0.001f / _pViewportState->_fAdaptTime;
		RENDER_CONTEXT::SetPixelShaderConstant(0, delta);
	}
	else
	{
		RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
		RENDER_CONTEXT::SetPixelShaderConstant(0, 1.0f);
		_pViewportState->_bKeyLightSet = true;
	}

#endif

	_pAdaptKey->Apply();
}


void PHOTOGRAPHIC_PIPELINE::ResetExposure(VIEWPORT_STATE *viewport)
{
	ReleaseKeyTarget(viewport);
	BindKeyTarget(viewport);
}



void PHOTOGRAPHIC_PIPELINE::BindKeyTarget(VIEWPORT_STATE *viewport)
{
	if (!_bHDRMode) return;

	if (!viewport->_pKeyLight || viewport->_uiPhotoPipelineID < _uiPhotoPipelineID)
	{
		viewport->_uiPhotoPipelineID = _uiPhotoPipelineID;
		viewport->_bKeyLightSet = false;

		for (UInt32 i = 0; i < _uiMaxViewports; i++)
		{
			if (!_pKeylightBindings[i])
			{
				_pKeylightBindings[i] = viewport;
				viewport->_pKeyLight = _pViewportKey[i];
				viewport->_uiPhotoPipelineID  = _uiPhotoPipelineID;
				return;
			}
		}

		QDT_ASSERT(false); // no more tonemapping viewports availible
						   // you sould consider using ReleaseKeyTarget
						   // on unused VIEWPORT_STATEs
	}
}

void PHOTOGRAPHIC_PIPELINE::ReleaseKeyTarget(VIEWPORT_STATE *viewport)
{
	if (!_bHDRMode) return;

	if (viewport->_pKeyLight && _pKeylightBindings)
	{
		for (UInt32 i = 0; i < _uiMaxViewports; i++)
		{
			if (_pKeylightBindings[i] == viewport)
			{
				viewport->_pKeyLight = NULL;
				viewport->_bKeyLightSet = false;
				_pKeylightBindings[i] = NULL;
				return;
			}
		}

		QDT_ASSERT(false);
	}
}



//-----------------------------------------------------------------------------
//	Name:		Initialize
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void PHOTOGRAPHIC_PIPELINE::Initialize(UInt32 ScreenWidth, UInt32 ScreenHeight,
									   UInt32 MaxStarBranches, UInt32 MaxViewports)
{
	if (_bInitialized) 
	{
		Release();
	}

	_uiPhotoPipelineID++;


	// Query capabilities -----------
	_bHDRMode = RENDER_CONTEXT::SupportFloatingPointBlending();
	if (!RENDER_CONTEXT::SupportShaderModel(SM_D3D_PS_3_0))
	{
		_bHDRMode = false;
		_bLegacyHardware = true;
	}
	else
		_bLegacyHardware = false;
	//-------------------------------


	_uiMaxStarBranches = MaxStarBranches;
	_uiMaxViewports    = MaxViewports;


	AllocateTargets(ScreenWidth, ScreenHeight);


	if (_bHDRMode && !_bLegacyHardware)
	{
		_pLuminance_ps      = QDT_CREATE_PIXELSHADER(LuminanceExtraction_ps);

#ifdef PLATFORM_X360
		_pAdaptKeyLight_ps  = QDT_CREATE_PIXELSHADER(AdaptativeKeyLight_X360_ps);
#else
		_pAdaptKeyLight_ps  = QDT_CREATE_PIXELSHADER(AdaptativeKeyLight_ps);
#endif

		_pLuminance = new FILTER_2D(_pLuminance_ps);
		_pAdaptKey  = new FILTER_2D(_pAdaptKeyLight_ps);

		_pDownScale4Square = new FILTER_2DCONVOLUTION*[4];
		_pTargetsKey = new I_RENDER_TARGET*[4];
		UInt32 size = 64;

		_pAverage16_ps = QDT_CREATE_PIXELSHADER(Average16_ps); 

		for (UInt32 i = 0; i < 4; i++)
		{
			_pTargetsKey[i] = RENDER_CONTEXT::CreateRenderTarget(size, size, PF_A16B16G16R16);

			_pTargetsKey[i]->GetTexture()->SetAddressUType(AT_WRAP);
			_pTargetsKey[i]->GetTexture()->SetAddressVType(AT_WRAP);
			_pTargetsKey[i]->GetTexture()->SetFilterType(FT_NONE);

			_pDownScale4Square[i] = new FILTER_2DCONVOLUTION();

			_pDownScale4Square[i]->InitBoxBlur16(4.0f /(float)size);
			_pDownScale4Square[i]->OverridePixelShader(_pAverage16_ps);
			_pDownScale4Square[i]->SetUseNoWeights(true);


			size /= 4;
		}


		_pViewportKey = new I_RENDER_TARGET*[MaxViewports];
		_pKeylightBindings = new VIEWPORT_STATE*[MaxViewports];

		for (UInt32 k = 0; k < MaxViewports; k++)
		{
			_pKeylightBindings[k] = NULL;
			_pViewportKey[k] = RENDER_CONTEXT::CreateRenderTarget(1, 1, PF_A16B16G16R16);
			_pViewportKey[k]->GetTexture()->SetAddressUType(AT_WRAP);
			_pViewportKey[k]->GetTexture()->SetAddressVType(AT_WRAP);
		}

		/*
		RENDER_CONTEXT::SetCurrentRenderTarget(_pPreviousKey);
		RENDER_CONTEXT::SetViewport(0, 0, 1, 1);
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR | CF_CLEAR_ZBUFFER, 255, 255, 0, 255);
		*/

		_pToneMap_ps = QDT_CREATE_PIXELSHADER(ToneMapping_ps);
		_pTonemapping = new FILTER_2D(_pToneMap_ps);

		_pLightDebug_ps = QDT_CREATE_PIXELSHADER(LightDebug_ps);
		_pLightDebug = new FILTER_2D(_pLightDebug_ps);

		_pBloomH	= new FILTER_2DCONVOLUTION();
		_pBloomV	= new FILTER_2DCONVOLUTION();
	}

	if (!_bLegacyHardware) // things that require PS_2_a
	{
		_pLightCompositing_ps[8] = QDT_CREATE_PIXELSHADER(LightCompositing_DEBUG);
		_pLightCompositing_ps[7] = QDT_CREATE_PIXELSHADER(LightCompositing_DOF_BLOOM_STARS);	  
		_pLightCompositing_ps[6] = QDT_CREATE_PIXELSHADER(LightCompositing_DOF_BLOOM);			  		  
		_pLightCompositing_ps[5] = QDT_CREATE_PIXELSHADER(LightCompositing_DOF_STARS);			  
		_pLightCompositing_ps[4] = QDT_CREATE_PIXELSHADER(LightCompositing_DOF);					  		  
		_pLightCompositing_ps[3] = QDT_CREATE_PIXELSHADER(LightCompositing_BLOOM_STARS);			  
		_pLightCompositing_ps[2] = QDT_CREATE_PIXELSHADER(LightCompositing_BLOOM);				  			  
		_pLightCompositing_ps[1] = QDT_CREATE_PIXELSHADER(LightCompositing_STARS);
		_pLightCompositing_ps[0] = QDT_CREATE_PIXELSHADER(LightCompositing);	

		_pStarBranch = new FILTER_2DCONVOLUTION();

		_pAverageTextures_ps[0] = QDT_CREATE_PIXELSHADER(AverageTextures_ps_2);
		_pAverageTextures_ps[1] = QDT_CREATE_PIXELSHADER(AverageTextures_ps_4);
		_pAverageTextures_ps[2] = QDT_CREATE_PIXELSHADER(AverageTextures_ps_6);
		_pAverageTextures = new FILTER_2D(_pAverageTextures_ps[0]);

		_pBrightPass_ps = QDT_CREATE_PIXELSHADER(BrightPass_ps);
		_pBrightPass = new FILTER_2D(_pBrightPass_ps);

		_pGaussianH = new FILTER_2DCONVOLUTION();
		_pGaussianV = new FILTER_2DCONVOLUTION();

		_pLightCompositing = new FILTER_2DCONVOLUTION();

        _pDownScaleDof = new FILTER_2DCONVOLUTION();
	}

	_pViewportCompositing_vs =  QDT_CREATE_VERTEXSHADER(ViewportCompositing_vs);

	_pViewportCompositing_ps = new I_PIXEL_SHADER*[8];
	_pViewportCompositing_ps[0] =  QDT_CREATE_PIXELSHADER(ViewportCompositing);
	_pViewportCompositing_ps[2] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_M);

	if (!_bLegacyHardware)
	{
		_pViewportCompositing_ps[1] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_N);
		_pViewportCompositing_ps[3] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_NM);
		_pViewportCompositing_ps[4] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_C);
		_pViewportCompositing_ps[5] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_NC);
		_pViewportCompositing_ps[6] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_MC);
		_pViewportCompositing_ps[7] =  QDT_CREATE_PIXELSHADER(ViewportCompositing_NMC);
	}

	#ifdef PLATFORM_X360
		_pMotionBlur_ps = QDT_CREATE_PIXELSHADER(OverrideAlpha_X360_ps);
	#else
		_pMotionBlur_ps = QDT_CREATE_PIXELSHADER(OverrideAlpha_ps);
	#endif
	
	_pMotionBlur             = new FILTER_2D(_pMotionBlur_ps);
	_pViewportCompositing    = new FILTER_2D(_pViewportCompositing_ps[0]);
	_pViewportCompositing->OverrideVertexShader(_pViewportCompositing_vs);
	_pViewportCompositing->SetQuadMode(true); // non optimized drawing to enable rotation 
	

	_pFilterIdentity = new FILTER_2D();


	// fills a texture with some random values
	_pNoiseTexture = RENDER_CONTEXT::CreateTexture(128, 128, PF_L8);
	char* texMem = (char*)_pNoiseTexture->Lock();
	for (UInt32 b = 0; b < 128 * 128; b++)
	{
		*(texMem++) = MATH::Abs(MATH::Rand() % 256);
	}
	_pNoiseTexture->Unlock();


	_pMotionblurViewport = NULL;

	InitFilters();
	_bInitialized = true;
}


void  PHOTOGRAPHIC_PIPELINE::AllocateTargets(UInt32 ScreenWidth, UInt32 ScreenHeight)
{
	I_RENDER_TARGET* prevTarget = RENDER_CONTEXT::GetCurrentRenderTarget();
	I_RENDER_TARGET* prevDepth  = RENDER_CONTEXT::GetCurrentDepthBuffer();

	Float32 ratio = (Float32)ScreenWidth / (Float32)ScreenHeight;

	_fLowresScale = (Float32)ScreenWidth / MATH::Sqrt((Float32)DOWNSAMPLE_NUMPIXELS * ratio);

	if (_fLowresScale -  MATH::Floor(_fLowresScale) > 0.5f)
	{
		_fLowresScale = 1.0f / MATH::Ceil(_fLowresScale);
	}
	else
	{
		_fLowresScale = 1.0f / MATH::Floor(_fLowresScale);
	}
 	_uiLowresFrameWidth   = (UInt32)( (Float32)ScreenWidth  * _fLowresScale );
	_uiLowresFrameHeight  = (UInt32)( (Float32)ScreenHeight * _fLowresScale );
	_uiHighresBorderSize = (UInt32)((Float32)SECURITY_BORDER / _fLowresScale);


	UInt32 frameBufferWidth  = ScreenWidth  + (_uiHighresBorderSize * 2);
	UInt32 frameBufferHeight = ScreenHeight + (_uiHighresBorderSize * 2);

	UInt32 lowresWidth  = _uiLowresFrameWidth  + SECURITY_BORDER * 2;
	UInt32 lowresHeight = _uiLowresFrameHeight + SECURITY_BORDER * 2;

	_pTargetDepth = RENDER_CONTEXT::CreateRenderTarget(frameBufferWidth, frameBufferHeight, PF_D24S8_DEPTHSTENCIL);

	
	if (_bHDRMode)
	{
		_pTargetFrameHDR = RENDER_CONTEXT::CreateRenderTarget(frameBufferWidth, frameBufferHeight, PF_A16B16G16R16);
		_pTargetFrameHDR->GetTexture()->SetFilterType(FT_NONE);
		ClearTarget(_pTargetFrameHDR);
	}

	_pTargetFrameLDR        = RENDER_CONTEXT::CreateRenderTarget(frameBufferWidth, frameBufferHeight, PF_A8R8G8B8);
	ClearTarget(_pTargetFrameLDR);
	
	_bTargetNormalized = _pTargetFrameLDR->GetTexture()->GetUVNormalized();	

		//_pTargetFrameLDR->GetTexture()->SetFilterType(FT_NONE);
		
	_pTargetCompositing = RENDER_CONTEXT::CreateRenderTarget(ScreenWidth, ScreenHeight, PF_A8R8G8B8);
	ClearTarget(_pTargetCompositing);
	_pTargetFrameMotionBlur	= RENDER_CONTEXT::CreateRenderTarget(ScreenWidth, ScreenHeight, PF_A8R8G8B8);
	ClearTarget(_pTargetFrameMotionBlur);


	

	UInt32 blurWidth  = _uiLowresFrameWidth + SECURITY_BORDER * 2;
	UInt32 blurHeight = _uiLowresFrameHeight + SECURITY_BORDER * 2;
	_pTargetBlur1  = RENDER_CONTEXT::CreateRenderTarget(blurWidth, blurHeight, PF_A8R8G8B8);
	_pTargetBlur2  = RENDER_CONTEXT::CreateRenderTarget(blurWidth, blurHeight, PF_A8R8G8B8);
	_pTargetBright = RENDER_CONTEXT::CreateRenderTarget(blurWidth, blurHeight, PF_A8R8G8B8);
	_pTargetBloom  = RENDER_CONTEXT::CreateRenderTarget(blurWidth, blurHeight, PF_A8R8G8B8);


	_pTargetsStar = new I_RENDER_TARGET*[_uiMaxStarBranches];

	for (UInt32 i = 0; i < _uiMaxStarBranches; i++)
	{
		_pTargetsStar[i] = RENDER_CONTEXT::CreateRenderTarget(blurWidth, blurHeight, PF_A8R8G8B8);
		ClearTarget(_pTargetsStar[i]);
	}

	RENDER_CONTEXT::SetCurrentRenderTarget(prevTarget);
	RENDER_CONTEXT::SetCurrentDepthBuffer(prevDepth);
}

//-----------------------------------------------------------------------------
//	Name:		InitFilters
//	Object:		
//	06-02-27:	PDE - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::InitFilters()
{
	if (!_bLegacyHardware)
	{
		_pGaussianH->InitDirectionalGaussianBlur(5.0, 0.0f);
		_pGaussianV->InitDirectionalGaussianBlur(0.0, 5.0f);
		_pDownScaleDof->InitBoxBlur16((float)_pTargetFrameLDR->GetWidth() / (float)_pTargetBlur2->GetWidth());

        _pDownScaleDof->InitBoxBlur16(((float)_pTargetFrameLDR->GetWidth() / (float)_pTargetBlur2->GetWidth()) * (14.0f / 16.0f));
 

		if (_pTargetBlur2->GetTexture()->GetUVNormalized())
		{
			_pGaussianH->ScaleKernel(1.0f / _uiLowresFrameWidth, 1.0f / _uiLowresFrameHeight);
			_pGaussianV->ScaleKernel(1.0f / _uiLowresFrameWidth, 1.0f / _uiLowresFrameHeight);
			_pDownScaleDof->ScaleKernel(1.0f / _pTargetFrameLDR->GetWidth(), 1.0f / _pTargetFrameLDR->GetHeight());
		}

		_pLightCompositing->InitDiamond();
		_pLightCompositing->OverridePixelShader(_pLightCompositing_ps[0]);
		_pLightCompositing->SetUseNoWeights(true);
		_pLightCompositing->EnableFixedKernel(false);


	}
}



void  PHOTOGRAPHIC_PIPELINE::ReleaseTargets()
{
	_fBloomRadCachedvalue     = -666.0f;

	if (_bHDRMode)
	{
		RENDER_CONTEXT::ReleaseRenderTarget(_pTargetFrameHDR);
		_pTargetFrameHDR = NULL;
	}

	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetFrameLDR);
	_pTargetFrameLDR = NULL;

	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetFrameMotionBlur);
	_pTargetFrameMotionBlur = NULL;

	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetCompositing);
	_pTargetCompositing = NULL;

	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetDepth);
	_pTargetDepth = NULL;
	
	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetBlur1);
	_pTargetBlur1 = NULL;
	
	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetBlur2);
	_pTargetBlur2 = NULL;

	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetBright);
	_pTargetBright = NULL;

	RENDER_CONTEXT::ReleaseRenderTarget(_pTargetBloom);
	_pTargetBloom = NULL;

	for (UInt32 i = 0; i < _uiMaxStarBranches; i++)
	{
		RENDER_CONTEXT::ReleaseRenderTarget(_pTargetsStar[i]);
		_pTargetsStar[i] = NULL;
	}

	if(_pTargetsStar)
	{
		delete[] _pTargetsStar;
		_pTargetsStar = NULL;
	}
}


void  PHOTOGRAPHIC_PIPELINE::ChangeResolution(UInt32 ScreenWidth, UInt32 ScreenHeight)
{
	_uiPhotoPipelineID++;
	
	if (_bHDRMode)
	{
		for (UInt32 i = 0; i < _uiMaxViewports; i++)
		{
			_pKeylightBindings[i] = NULL;
		}
	}

	ReleaseTargets();
	AllocateTargets(ScreenWidth, ScreenHeight);
	InitFilters();
}


//-----------------------------------------------------------------------------
//	Name:		ChangeResolution
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void  PHOTOGRAPHIC_PIPELINE::InvalidateKeyLights()
{
	_uiPhotoPipelineID++;
	
	if (_bHDRMode)
	{
		for (UInt32 i = 0; i < _uiMaxViewports; i++)
		{
			_pKeylightBindings[i] = NULL;
		}
	}
}



//-----------------------------------------------------------------------------
//	Name:		Release
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::Release()
{
	if (!_bInitialized) return;

	ReleaseTargets();


	RENDER_CONTEXT::ReleasePixelShader(_pMotionBlur_ps);
	RENDER_CONTEXT::ReleasePixelShader(_pLuminance_ps);
	RENDER_CONTEXT::ReleasePixelShader(_pBrightPass_ps);
	RENDER_CONTEXT::ReleaseTexture(_pNoiseTexture);

	delete _pMotionBlur;

	if (_bHDRMode)
	{
		RENDER_CONTEXT::ReleasePixelShader(_pAverage16_ps);

		for (int i = 0; i < 4; i++)
		{
			RENDER_CONTEXT::ReleaseRenderTarget(_pTargetsKey[i]);
			delete _pDownScale4Square[i];
		}
		delete[] _pTargetsKey;
		delete[] _pDownScale4Square;

		RENDER_CONTEXT::ReleasePixelShader(_pAdaptKeyLight_ps);
		RENDER_CONTEXT::ReleasePixelShader(_pToneMap_ps);
		RENDER_CONTEXT::ReleasePixelShader(_pLightDebug_ps);
		delete _pAdaptKey;
		delete _pLuminance;
		delete _pTonemapping;
		delete _pLightDebug;

		for (UInt32 k = 0; k < _uiMaxViewports; k++)
		{
			RENDER_CONTEXT::ReleaseRenderTarget(_pViewportKey[k]);
		}

		delete[] _pViewportKey;
		delete[] _pKeylightBindings;
		_pKeylightBindings = NULL;
	}

	if (!_bLegacyHardware)
	{
		for (UInt32 t = 0; t < 3; t++)
		{
			RENDER_CONTEXT::ReleasePixelShader(_pAverageTextures_ps[t]);
		}
		delete _pAverageTextures;

		for (UInt32 s = 0; s < 9; s++)
		{
			RENDER_CONTEXT::ReleasePixelShader(_pLightCompositing_ps[s]);
		}




		delete _pBrightPass;

		delete _pGaussianH;
		delete _pGaussianV;
		delete _pBloomH;
		delete _pBloomV;
		delete _pLightCompositing;
		delete _pStarBranch;
		delete _pDownScaleDof;
	}

	RENDER_CONTEXT::ReleaseVertexShader(_pViewportCompositing_vs);
	RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[0]);
	RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[2]);

	if (!_bLegacyHardware)
	{
		RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[1]);
		RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[3]);
		RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[4]);
		RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[5]);
		RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[6]);
		RENDER_CONTEXT::ReleasePixelShader(_pViewportCompositing_ps[7]);
	}
	delete[] _pViewportCompositing_ps;

	
	delete _pViewportCompositing;
	delete _pFilterIdentity;

	_bInitialized = false;
}


void PHOTOGRAPHIC_PIPELINE::SetMotionBlurViewport(VIEWPORT_STATE *viewport)
{			
	_pMotionblurViewport = viewport;
	_bMotionBlurInitialized = false;
}

//-----------------------------------------------------------------------------
//	Name:		MixMotionBlur
//	Object:		
//	06-03-20:	PDE - Created
//-----------------------------------------------------------------------------
static Float32 _lastMotionBlurTick = 0.0f;
void	PHOTOGRAPHIC_PIPELINE::MixMotionBlur(UInt32 width, UInt32 height)
{
#ifdef PLATFORM_X360
	RENDER_CONTEXT::SetBlendMode(BM_ONE, BOP_ADD, BM_ZERO);
#else
	RENDER_CONTEXT::SetBlendMode(BM_SRCALPHA, BOP_ADD, BM_INVSRCALPHA);
#endif
	Float32 time = TIME::Instance()->GetFrameTime();

	if (!_bMotionBlurInitialized)
	{
		_pFilterIdentity->Init(_pTargetCompositing->GetTexture(), _pTargetFrameMotionBlur, 0, 0, width, height, 0, 0, width, height);
		RENDER_CONTEXT::Clear(CF_CLEAR_COLOR, 0, 0, 0, 0);
		_pFilterIdentity->Apply();
		_bMotionBlurInitialized = true;
		_lastMotionBlurTick = time;
	}

	_pMotionBlur->Init(_pTargetFrameMotionBlur->GetTexture(), _pTargetCompositing, 0, 0, width, height, 0, 0, width, height);
	RENDER_CONTEXT::SetPixelShaderConstant(0, _fMotionBlurImportance);
#ifdef PLATFORM_X360
	RENDER_CONTEXT::SetPixelShaderTexture(1, _pTargetCompositing->GetTexture());
#endif
	_pMotionBlur->Apply();
	
	if (time - _lastMotionBlurTick > (_fMotionBlurPersistance * 100.0f))
	{
		_pMotionBlur->Init(_pTargetCompositing->GetTexture(), _pTargetFrameMotionBlur, 0, 0, width, height, 0, 0, width, height);
		RENDER_CONTEXT::SetPixelShaderConstant(0, 0.3f);
#ifdef PLATFORM_X360
		RENDER_CONTEXT::SetPixelShaderTexture(1, _pTargetFrameMotionBlur->GetTexture());
#endif
		_pMotionBlur->Apply();
		_lastMotionBlurTick = time;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetupMotionBlur
//	Object:		
//	06-03-20:	PDE - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::SetupMotionBlur(Float32	importance,
											   Float32	persistance)
{
	_fMotionBlurImportance  = importance;
	_fMotionBlurPersistance = persistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetMotionBlurImportance
//	Object:		
//	06-03-22:	BFA - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::SetMotionBlurImportance(Float32 rImportance)
{
	_fMotionBlurImportance  = rImportance;
}

//-----------------------------------------------------------------------------
//	Name:		SetMotionBlurPersistance
//	Object:		
//	06-03-22:	BFA - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::SetMotionBlurPersistance(Float32 rPersistance)
{
	_fMotionBlurPersistance = rPersistance;
}

//-----------------------------------------------------------------------------
//	Name:		ClearTarget
//	Object:		
//	06-03-22:	PDE - Created
//-----------------------------------------------------------------------------
void	PHOTOGRAPHIC_PIPELINE::ClearTarget(I_RENDER_TARGET *	tgt)
{
	RENDER_CONTEXT::SetCurrentRenderTarget(tgt);
	if(RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_DIRECT3D9)
		RENDER_CONTEXT::SetCurrentDepthBuffer(_pTargetDepth); // we must ensure it is allocated nd bigger than tgt
	RENDER_CONTEXT::SetViewport(0, 0, tgt->GetWidth(),  tgt->GetHeight());
	RENDER_CONTEXT::Clear(CF_CLEAR_COLOR, 0, 0, 0, 0);
}


//-----------------------------------------------------------------------------
//	Name:		IsMotionBlurEnable
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
bool	PHOTOGRAPHIC_PIPELINE::IsMotionBlurEnable()
{
	return (_pMotionblurViewport!=NULL);
}

//-----------------------------------------------------------------------------
//	Name:		GetMotionBlurImportance
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
Float32&	PHOTOGRAPHIC_PIPELINE::GetMotionBlurImportance()
{
	return (_fMotionBlurImportance);
}

//-----------------------------------------------------------------------------
//	Name:		GetMotionBlurPersistance
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
Float32&	PHOTOGRAPHIC_PIPELINE::GetMotionBlurPersistance()
{
	return (_fMotionBlurPersistance);
}


void PHOTOGRAPHIC_PIPELINE::UpdateSystemMemKeyLight()
{
	#ifdef PLATFORM_PC
	#include INCL_KRENDERER(Renderer/OGL_PC/RenderTarget_OGL_PC)

	QDT_ASSERT(RENDER_CONTEXT::GetRenderContextType() == RENDER_CONTEXT::RCT_OPENGL_FOR_MAYA);


	RENDER_TARGET_OGL_PC* target = (RENDER_TARGET_OGL_PC*)_pTargetsKey[2];

	float buffer[100];
	target->ReadSurfaceContent(buffer, true);



	_fKeyLightValue = buffer[0];
	#else
		_fKeyLightValue = 0.0f;
	#endif
}


Float32 PHOTOGRAPHIC_PIPELINE::QueryKeyLight()
{
	return _fKeyLightValue;
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
