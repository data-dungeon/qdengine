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
//	The PHOTOGRAPHIC_PIPELINE class implements ...
//
//	06-01-24:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __TEST_RENDERER_APPLICATION_UTILITIES_PHOTOGRAPHIC_PIPELINE__
#define __TEST_RENDERER_APPLICATION_UTILITIES_PHOTOGRAPHIC_PIPELINE__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_VERTEX_SHADER)
DECL_KRENDERER(I_PIXEL_SHADER)
DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(I_TEXTURE)
DECL_KRENDERER(FILTER_2DCONVOLUTION)
DECL_KRENDERER(FILTER_2D)
DECL_KRENDERER(VIEWPORT_STATE)

#include INCL_KMATH(Math\Geometry\Matrix\Matrix)

#define SECURITY_BORDER 3

#define DOWNSAMPLE_NUMPIXELS  (200 * 150)


namespace	QDT
{
	namespace	KRENDERER
	{
		class  PHOTOGRAPHIC_PIPELINE
		{
		public:

			static void Initialize(UInt32 ScreenWidth,
								   UInt32 ScreenHeight,
								   UInt32 MaxStarBranches = 6,
								   UInt32 MaxViewports    = 8);

			static void Release();
			
			static void ChangeResolution(UInt32 ScreenWidth, UInt32 ScreenHeight);

			static void	InitViewPort(VIEWPORT_STATE *viewport, UInt8 ClearR = 0, UInt8 ClearG = 0, UInt8 ClearB = 0);
			static void	SetMotionBlurViewport(VIEWPORT_STATE *viewport);
			static bool	IsMotionBlurEnable();

			static void		SetupMotionBlur(Float32 importance, Float32 persistance);
			static void		SetMotionBlurImportance(Float32 rImportance);
			static void		SetMotionBlurPersistance(Float32 rPersistance);
			static Float32& GetMotionBlurImportance();
			static Float32& GetMotionBlurPersistance();

			static void DoCompositing();

			static void CommitBloomRadius(Float32 value);
			static void CommitGhostsParams(Float32 Diffraction, Float32 Intensity);
			static void CommitCircleOfConfusion(Float32 value);

			static void BindKeyTarget(VIEWPORT_STATE *viewport);
			static void ReleaseKeyTarget(VIEWPORT_STATE *viewport);

			static void InvalidateKeyLights();

			static UInt32 GetBorderSize() {return _uiHighresBorderSize;};
			static void EnableDebugDOF(Bool val) {_bDebugDOF = val;};
			static void EnableDebugLight(Bool val) {_bDebugLight = val;};
			static void ResetExposure(VIEWPORT_STATE *viewport);

			static float QueryKeyLight();
			static void UpdateSystemMemKeyLight();


		private:

			static void ClearTarget(I_RENDER_TARGET *tgt);

			static void AllocateTargets(UInt32 ScreenWidth, UInt32 ScreenHeight);
			static void ReleaseTargets();
			static void InitFilters();
			static void MixMotionBlur(UInt32 width, UInt32 height);

			static VIEWPORT_STATE* _pViewportState;
			static UInt32		   _uiMaxViewports;

			//Star parametters
			static UInt32 _uiMaxStarBranches;

			static UInt32 _uiPhotoPipelineID;


			static I_RENDER_TARGET**	  _pTargetsStar;
			static FILTER_2DCONVOLUTION * _pStarBranch;

			static Bool				   _bHDRMode;
			static Bool				   _bLegacyHardware;

			static Bool    _bInitialized;
			static Bool    _bTargetNormalized;

			static I_TEXTURE *          _pNoiseTexture;

			static I_PIXEL_SHADER *	    _pLightCompositing_ps[9];
			static I_PIXEL_SHADER *	    _pAverageTextures_ps[3];
			static I_PIXEL_SHADER *	    _pToneMap_ps;
			static I_PIXEL_SHADER *	    _pLightDebug_ps;
			static I_PIXEL_SHADER *	    _pLuminance_ps;
			static I_PIXEL_SHADER *	    _pAdaptKeyLight_ps;
			static I_PIXEL_SHADER *	    _pBrightPass_ps;
			static I_VERTEX_SHADER *	_pViewportCompositing_vs;
			static I_PIXEL_SHADER **    _pViewportCompositing_ps;
			static I_PIXEL_SHADER *     _pMotionBlur_ps;
			static I_PIXEL_SHADER *     _pAverage16_ps;

			static FILTER_2D *			_pAverageTextures;
			static FILTER_2D *			_pViewportCompositing;

			static I_RENDER_TARGET *   _pTargetFrameHDR;


			static I_RENDER_TARGET *   _pTargetFrameLDR;
			static I_RENDER_TARGET *   _pTargetFrameMotionBlur;
			static I_RENDER_TARGET *   _pTargetCompositing;
			static I_RENDER_TARGET *   _pTargetDepth;

			static I_RENDER_TARGET *   _pTargetBlur1;
			static I_RENDER_TARGET *   _pTargetBlur2;
			static I_RENDER_TARGET *   _pTargetBright;
			static I_RENDER_TARGET *   _pTargetBloom;

			static FILTER_2DCONVOLUTION * _pGaussianH;
			static FILTER_2DCONVOLUTION * _pGaussianV;
			static FILTER_2DCONVOLUTION * _pBloomH;
			static FILTER_2DCONVOLUTION * _pBloomV;

			static FILTER_2DCONVOLUTION * _pLightCompositing;

			static FILTER_2DCONVOLUTION*  _pDownScaleDof;

			static FILTER_2DCONVOLUTION** _pDownScale4Square;
			static I_RENDER_TARGET**	  _pTargetsKey;

			static I_RENDER_TARGET**	  _pViewportKey;
			static VIEWPORT_STATE**       _pKeylightBindings;
			static VIEWPORT_STATE*        _pMotionblurViewport;

			static FILTER_2D            * _pFilterIdentity;
			static FILTER_2D            * _pLuminance;
			static FILTER_2D            * _pAdaptKey;
			static FILTER_2D            * _pTonemapping;
			static FILTER_2D            * _pLightDebug;
			static FILTER_2D            * _pBrightPass;
			static FILTER_2D            * _pMotionBlur;

			static float _pRainBow[32];

			static void ComputeKeyLight();
			static void ToneMapImage();

			static UInt32		   _uiLowresFrameWidth;
			static UInt32		   _uiLowresFrameHeight;
			static UInt32		   _uiHighresBorderSize;
			static Float32		   _fLowresScale;

			static Bool		       _bDebugDOF;
			static Bool		       _bDebugLight;
			static Bool		       _bMotionBlurInitialized;

			static Float32 _fMotionBlurImportance;
			static Float32 _fMotionBlurPersistance;

			static Float32 _fKeyLightValue;

			PHOTOGRAPHIC_PIPELINE() {};
			~PHOTOGRAPHIC_PIPELINE() {};
		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
