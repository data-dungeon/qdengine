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
//	CLASS:	CAMERA
//	The CAMERA class implements a camera inspired from Maya camera.
//	This allow us to have more "cinematographic" parameters.
//	Don't forget to update the matrices with the viewport aspect ration before
//  using the projection matrix or the view matrix!
//
//	06-02-03:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_CAMERA_H__
#define __M3D_SCENEGRAPH_ENTITIES_CAMERA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KRENDERER(VIEWPORT_STATE)

#include	INCL_3DENGINE(SceneGraph/Entities/EntityABC)
#include	INCL_KMATH(MAth/Geometry/Matrix/Matrix)

#define CAMERA_DEFAULT_FOCAL						35.5f
#define CAMERA_DEFAULT_HORIZONTAL_FIELD_OF_VIEW		0.95f // In radian, so 54.43 degrees
#define CAMERA_DEFAULT_ASPECT_RATIO					1.5f
#define CAMERA_DEFAULT_SCALE						1.0f
#define CAMERA_DEFAULT_NEAR_CLIPPING_DISTANCE		1.0f
#define CAMERA_DEFAULT_FAR_CLIPPING_DISTANCE		10000.0f

#define CAMERA_DEFAULT_ENABLE_DEPTH_OF_FIELD		false 
#define CAMERA_DEFAULT_FAR_DEPTH_OF_FIELD		 	600.0f // in cm		
#define CAMERA_DEFAULT_FOCUS_PLANE					400.0f   // in cm 
#define CAMERA_DEFAULT_FOCUS_RANGE					150.0f   // in cm
#define CAMERA_DEFAULT_FOCUS_SAFE_RANGE				50.0f
#define CAMERA_DEFAULT_CIRCLE_OF_CONFUSION			5.0f    // in pixels

#define CAMERA_DEFAULT_EXPOSURE						0.25f
#define CAMERA_DEFAULT_GAMMA						1.0f
#define CAMERA_DEFAULT_BLUR_RADIUS					8.0f
#define CAMERA_DEFAULT_BRIGHT_THRESHOLD				0.5f     // positive luminance value
#define CAMERA_DEFAULT_BRIGHT_INTENSITY				1.0f			
#define CAMERA_DEFAULT_ENABLE_BLOOM					false
#define CAMERA_DEFAULT_ENABLE_GHOSTS				false
#define CAMERA_DEFAULT_GHOSTS_DIFFRACTION			0.0f	
#define CAMERA_DEFAULT_GHOSTS_INTENSITY				0.5f
#define CAMERA_DEFAULT_ENABLE_STARS					false
#define CAMERA_DEFAULT_STARS_NBR_BRANCHES			4		// 2, 4 or 6
#define CAMERA_DEFAULT_STARS_DIFFRACTION			0.0f
#define CAMERA_DEFAULT_STARS_INTENSITY				0.5f 
#define CAMERA_DEFAULT_STARS_SIZE					8.0f // in pixels
#define CAMERA_DEFAULT_STARS_ANGLE					0.0f // in radians

#define CAMERA_DEFAULT_ENABLE_NOISE					false
#define CAMERA_DEFAULT_NOISE_INTENSITY				0.3f
#define CAMERA_DEFAULT_NOISE_CUTOFF					0.5f
#define CAMERA_DEFAULT_NOISE_SCALE					3.0f
#define CAMERA_DEFAULT_NOISE_FPS					15.0f

#define CAMERA_DEFAULT_COLORCONTROL_FADE			0.0f

#define COLORCONTROL_STAGES_COUNT					5	

namespace QDT
{
	namespace M3D
	{
		class CAMERA : public ENTITY_ABC
		{
		public:

			// Constructor & destructor
			CAMERA(COM_HANDLE * pCH);
			~CAMERA();

			// Operators
			CAMERA & operator=(const CAMERA & C);

			enum COLORCONTROL_OPERATION
			{
				CO_NONE = 0,
				CO_ADD,
				CO_MULTIPLY,
				CO_HUESHIFT,
				CO_SATURATION,
				CO_INVERT,
				CO_SCREEN,
				CO_BRIGHTNESS_CONTRAST,
				CO_FORCEDWORD = 0x7FFFFFFF
			};

			virtual void					Register();
			virtual void					Unregister();

			virtual void					Instanciate(COM_INSTANCE & Instance);
			virtual void					InstanciateRec(COM_INSTANCE & Instance);

			void							CopyProperties(const CAMERA & C);

			// This method need to be called in a viewport before using the view or projection matrix
			void							UpdateMatrices(Float32 fViewportAspectRatio, Float32 fScaleBorderX, Float32 fScaleBorderY);
			void							UpdateViewportState(VIEWPORT_STATE & ViewportState);

			// Accessors
			_INLINE_ const Float32&			GetFocal(void) const;
			_INLINE_ const Float32&			GetHorizontalFieldOfView(void) const;
			_INLINE_ const Float32&			GetAspectRatio(void) const;
			_INLINE_ const Float32&			GetNearClippingDistance(void) const;
			_INLINE_ const Float32&			GetFarClippingDistance(void) const;
			_INLINE_ const Float32&			GetScale(void) const;
			_INLINE_ const MATRIX &			GetProjectionMatrix(void) const;
			_INLINE_ const MATRIX &			GetViewMatrix(void) const;
			_INLINE_ const Bool&			GetEnableDepthOfField(void) const;
			_INLINE_ const Float32&			GetFarDepthOfField(void) const;
			_INLINE_ const Float32&			GetFocusPlane(void) const;
			_INLINE_ const Float32&			GetFocusRange(void) const;
			_INLINE_ const Float32&			GetFocusSafeRange(void) const;
			_INLINE_ const Float32&			GetCircleOfConfusion(void) const;
			_INLINE_ const Float32&			GetGamma(void) const;
			_INLINE_ const Float32&			GetExposure() const;
			_INLINE_ const Float32&			GetBrightBlurRadius() const;
			_INLINE_ const Float32&			GetBrightThreshold() const;
			_INLINE_ const Float32&			GetBrightIntensity() const;
			_INLINE_ const Bool&			IsBloomEnable() const;
			_INLINE_ const Bool&			IsGhostsEnable() const;
			_INLINE_ const Float32&			GetGhostsDiffraction() const;
			_INLINE_ const Float32&			GetGhostsIntensity() const;
			_INLINE_ const Bool&			IsStarsEnable() const;
			_INLINE_ const UInt32&			GetStarsNbrBranches() const;
			_INLINE_ const Float32&			GetStarsSize() const;
			_INLINE_ const Float32&			GetStarsDiffraction() const;
			_INLINE_ const Float32&			GetStarsIntensity() const;
			_INLINE_ const Float32&			GetStarsAngle() const;
			_INLINE_ const Bool&			IsNoiseEnable() const;
			_INLINE_ const Float32&			GetNoiseCutoff() const;
			_INLINE_ const Float32&			GetNoiseIntensity() const;
			_INLINE_ const Float32&			GetNoiseScale() const;
			_INLINE_ const Float32&			GetNoiseFps() const;
			_INLINE_ const Float32&			GetColorControlFade() const;
			_INLINE_ void					GetColorControlStage(const UInt32& nStageId, COLORCONTROL_OPERATION& OpCode, Float32& rParam0, Float32& rParam1, Float32& rParam2) const;

			// Modifiors
			_INLINE_ void					SetFocal(Float32 fFocal);
			_INLINE_ void					SetHorizontalFieldOfView(Float32 fFieldOfView);
			_INLINE_ void					SetAspectRatio(Float32 fValue);
			_INLINE_ void					SetNearClippingDistance(Float32 fNearClippingDistance);
			_INLINE_ void					SetFarClippingDistance(Float32 fFarClippingDistance);
			_INLINE_ void					SetScale(Float32 fScale);
			_INLINE_ void					SetExposure(Float32 f);
			_INLINE_ void					SetBrightBlurRadius(Float32 f);
			_INLINE_ void					SetBrightThreshold(Float32 f);
			_INLINE_ void					SetBrightIntensity(Float32 f);
			_INLINE_ void					EnableBloom(Bool b);
			_INLINE_ void					EnableGhosts(Bool b);
			_INLINE_ void					SetGhostsDiffraction(Float32 f);
			_INLINE_ void					SetGhostsIntensity(Float32 f);
			_INLINE_ void					EnableStars(Bool b);
			_INLINE_ void					SetStarsNbrBranches(UInt32 ui);
			_INLINE_ void					SetStarsSize(Float32 f);
			_INLINE_ void					SetStarsDiffraction(Float32 f);
			_INLINE_ void					SetStarsIntensity(Float32 f);
			_INLINE_ void					SetStarsAngle(Float32 f);
			_INLINE_ void					SetEnableNoise(Bool b);
			_INLINE_ void					SetNoiseCutoff(Float32 f);
			_INLINE_ void					SetNoiseIntensity(Float32 f);
			_INLINE_ void					SetNoiseScale(Float32 f);
			_INLINE_ void					SetNoiseFps(Float32 f);
			_INLINE_ void					SetEnableDepthOfField(Bool bEnableDepthOfField);
			_INLINE_ void					SetFarDepthOfField(Float32 rFarDepthOfField);
			_INLINE_ void					SetFocusPlane(Float32 rFocusPlane);
			_INLINE_ void					SetFocusRange(Float32 rFocusRange);
			_INLINE_ void					SetFocusSafeRange(Float32 rFocusSafeRange);
			_INLINE_ void					SetCircleOfConfusion(Float32 rCircleOfConfusion);
			_INLINE_ void					SetGamma(Float32 rGamma);

			_INLINE_ void					SetColorControlFade(Float32 rGlobalFade);
			_INLINE_ void					SetColorControlStage(UInt32 nStageId, COLORCONTROL_OPERATION OpCode, Float32 rParam0=0.0f, Float32 rParam1=0.0f, Float32 rParam2=0.0f);


			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			struct COLORCONTROL_STAGE
			{
				COLORCONTROL_OPERATION  _uiOperationId;
				Float32 				_fParam0;
				Float32 				_fParam1;
				Float32 				_fParam2;

				COLORCONTROL_STAGE(){_uiOperationId = CO_NONE;}
			};

			COLORCONTROL_STAGE  _colorControlStages[COLORCONTROL_STAGES_COUNT];
			Float32				_fColorControlFade;

			void	Copy(const CAMERA & C);

			// Optic
			Float32				_fFocal;
			Float32				_fHorizontalFieldOfView;
			Float32				_fAspectRatio;
			Float32				_fScale;
			Float32				_fNearClippingDistance;
			Float32				_fFarClippingDistance;
			
			// Depth of field
			Bool				_bEnableDepthOfField;
			Float32				_fFarDepthOfField;
			Float32				_fFocusPlane;
			Float32				_fFocusRange;
			Float32				_fFocusSafeRange;
			Float32				_fCircleOfConfusion;

			// Noise
			Bool				_bEnableNoise;
			Float32				_fNoiseCutoff;
			Float32				_fNoiseIntensity;
			Float32				_fNoiseScale;
			Float32				_fNoiseFps;

			// HDR
			Float32				_fExposure;
			Float32				_fGamma;
			Float32				_fBrightBlurRadius; // This parameter affects ghosts & bloom
			Float32				_fBrightThreshold;	// This parameter affects bloom, ghosts & stars
			Float32				_fBrightIntensity;	// This parameter affects bloom, ghosts & stars

			Bool				_bEnableBloom;

			Bool				_bEnableGhosts;
			Float32				_fGhostsDiffraction;
			Float32				_fGhostsIntensity;
			
			Bool				_bEnableStars;
			UInt32				_uiStarsNbrBranches;
			Float32				_fStarsDiffraction;
			Float32				_fStarsIntensity;
			Float32				_fStarsSize;
			Float32				_fStarsAngle;

			// Matrices
			MATRIX				_mProjection;
			MATRIX				_mView;

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunk
			static void		CM_ReadChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
		};
	}
}

#ifndef _DEBUG
	#include "Camera.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_CAMERA_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
