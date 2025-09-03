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
//	CLASS:	VIEWPORT_STATE
//	The VIEWPORT_STATE class implements ...
//
//	06-01-24:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __TEST_RENDERER_APPLICATION_UTILITIES_VIEWPORT_STATE__
#define __TEST_RENDERER_APPLICATION_UTILITIES_VIEWPORT_STATE__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(I_RENDER_TARGET)
DECL_KRENDERER(PHOTOGRAPHIC_PIPELINE)
DECL_KRENDERER(I_TEXTURE)

#include INCL_KMATH(Math\Geometry\Matrix\Matrix)
#include INCL_KRENDERER(Renderer\RenderContext)

namespace	QDT
{

	namespace	KRENDERER
	{

		class VIEWPORT_STATE
		{
			friend class PHOTOGRAPHIC_PIPELINE;

		public:


			VIEWPORT_STATE();
			~VIEWPORT_STATE();


			void SetBloomParams(Float32 Radius, Float32 Threshold, Float32 Intensity);
	
			void SetStarsParams(UInt32 NumBranches, UInt32 NumPasses,
								Float32 Diffraction, Float32 Intensity,
								Float32 Size, Float32 Rotate);
					
			void SetNoiseParams(Float32 Cutoff, Float32 Scale, Float32 Intensity, Float32 Period);

			void SetExposure(Float32 exposure) {_fExposureControl = exposure;};
			void SetExposureAdaptTime(Float32 time) {_fAdaptTime = time;};
			void SetMinKeyLight(Float32 val) {_fMinKey = val;};
			void SetMaxKeyLight(Float32 val) {_fMaxKey = val;};

			void SetManualKey(Float32 val) {_fManualKey = val;};
			void SetManualRatio(Float32 val) {_fManualRatio = val;};

			void SetDofParams(Float32 distance, Float32 focalX,  Float32 focalB, Float32 circleOfConfusion, Float32 bias = 0.0f)
			{
				_fFocalDistance = distance;
				_fFocalX = focalX;
				_fFocalB = focalB;
				_fFocalBias = bias;
				_fCircleOfConfusion = (circleOfConfusion * (_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth())) / 1024.0f;
			}

			void EnableDof(Bool val){_bDofEnabled = val;};
			void EnableBloom(Bool val){_bBloomEnabled = val;};
			void EnableStars(Bool val){_bStarsEnabled = val;};
			void EnableAutoExposure(Bool val){_bAutoExposureEnabled = val;};
			void EnableNoise(Bool val){_bNoiseEnabled = val;};
			void EnableColorTransform(Bool val){_bColorControlEnabled = val;};

			void SetSize(Float32 width, Float32 height);
			void SetLocation(Float32 x, Float32 y, Float32 width, Float32 height);
			void ResetTransform();
			void Translate(Float32 x, Float32 y);
			void Rotate(Float32 angle);
			void Shear(Float32 x, Float32 y);

			void ColorIdentity() {_colorTransform.Identity();};
			void ColorSaturation(Float32 sat);
			void ColorMultiply(Float32 r, Float32 g, Float32 b);
			void ColorAdd(Float32 r, Float32 g, Float32 b);
			void ColorHueShift(Float32 hue);
			void ColorInvert();
			void ColorScreen(Float32 r, Float32 g, Float32 b);
			void ColorFadeToIdentity(Float32 val);

			void SetMaskTexture(I_TEXTURE* mask){_pMaskTexture = mask;};
			void SetOpacity(Float32 val){_fOpacity = val;};

			void GetScaleBorder(Float32 & fScaleBorderX, Float32 & fScaleBorderY);


		private:
			Bool	_bDofEnabled;
			Bool	_bBloomEnabled;
			Bool	_bStarsEnabled;
			Bool	_bAutoExposureEnabled;
			Bool    _bNoiseEnabled;
			Bool    _bColorControlEnabled;

			Float32	_fNoiseIntensity;
			Float32	_fNoiseCutoff;
			Float32	_fNoiseScale;
			Float32	_fNoisePeriod;

			I_TEXTURE* _pMaskTexture;
			Float32    _fOpacity;

			MATRIX _viewportTransform;
			MATRIX _colorTransform;		

			Float32	_uiViewPortWidth;
			Float32	_uiViewPortHeight;

			UInt32	_uiNumStarBranches;
			UInt32	_uiNumStarPasses;
			
			Float32	_fStarScale;
			Float32	_fStarDiffraction;
			Float32	_fStarIntensity;
			Float32	_fStarRotate;

			Float32	_fFocalX;
			Float32	_fFocalB;

			Float32	_fFocalDistance;
			Float32	_fFocalBias;
			Float32	_fCircleOfConfusion;
			Float32	_fExposureControl;
			Float32	_fBloomThreshold;
			Float32	_fBloomIntensity;
			Float32	_fBloomRadius;


			Float32 _fManualKey;
			Float32 _fManualRatio;
			Float32 _fMinKey;
			Float32 _fMaxKey;
			Float32 _fAdaptTime;

			// used by PHOTOGRAPHIC_PIPELINE for multi-viewport auto exposure
			I_RENDER_TARGET* _pKeyLight;
			UInt32           _uiPhotoPipelineID;


			//--- Exectution related noise values
			Float32	_fNoisePhase;
			VECTOR_2 _vNoiseOffset1;
			VECTOR_2 _vNoiseOffset2;
			//--------------------
			Bool	_bKeyLightSet;

		};
	}
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
