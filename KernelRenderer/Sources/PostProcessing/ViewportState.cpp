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
#include "ViewportState.h"

#include INCL_KRENDERER(Renderer\RenderContext)
#include INCL_KRENDERER(PostProcessing\Photographic_Pipeline)
#include INCL_KMATH(Math\Geometry\Quaternions\Quaternion)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VIEWPORT_STATE
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
VIEWPORT_STATE::VIEWPORT_STATE()
{
	_bKeyLightSet = false;

	_bDofEnabled          = false;
	_bBloomEnabled        = false;
	_bStarsEnabled        = false;
	_bNoiseEnabled        = false;
	_bColorControlEnabled = false;
	_bAutoExposureEnabled = true;

	_colorTransform.Identity();

	SetLocation(0, 0, 1.0f, 1.0f);
	SetBloomParams(8.0f, 0.5f, 1.0f);
	SetDofParams(0.3f, 0.3f, 0.0f, 4.0f);
	SetExposure(0.5f);
	SetStarsParams(6, 3, 0.5f, 0.8f, 8.0f, 0.0f);

	_pKeyLight        = NULL;
	_uiPhotoPipelineID = 0;


	_fNoisePhase  = 0.0f;
	_fNoisePeriod = 0.1f;
	_fNoiseScale  = 3.0f;
	_fNoiseCutoff = 0.5f;
	_fNoiseIntensity = 0.05f;


	_pMaskTexture = NULL;
	_fOpacity     = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		VIEWPORT_STATE
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
VIEWPORT_STATE::~VIEWPORT_STATE()
{
	PHOTOGRAPHIC_PIPELINE::ReleaseKeyTarget(this);
}

			

//-----------------------------------------------------------------------------
//	Name:		SetNoiseParams
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::SetNoiseParams(Float32 Cutoff, Float32 Scale, Float32 Intensity, Float32 Period)
{
	_fNoiseCutoff = Cutoff;
	_fNoiseScale  = Scale;
	_fNoiseIntensity = Intensity;
	_fNoisePeriod = Period;
}


//-----------------------------------------------------------------------------
//	Name:		SetBloomParams
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::SetBloomParams(Float32 Radius, Float32 Threshold, Float32 Intensity)
{
	_fBloomRadius    = Radius;
	_fBloomThreshold = Threshold;
	_fBloomIntensity = Intensity;
}

//-----------------------------------------------------------------------------
//	Name:		SetLocation
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::SetLocation(Float32 x, Float32 y, Float32 width, Float32 height)
{
	SetSize(width, height);

	ResetTransform();
	Translate(x, y);
}


//-----------------------------------------------------------------------------
//	Name:		SetLocation
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::SetSize(Float32 width, Float32 height)
{
	QDT_ASSERT(width <= 1.0f);
	QDT_ASSERT(height <= 1.0f);

	_uiViewPortWidth  = width;
	_uiViewPortHeight = height;
}



//-----------------------------------------------------------------------------
//	Name:		SetStarsParams
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::SetStarsParams(UInt32 NumBranches, UInt32 NumPasses,
										   Float32 Diffraction, Float32 Intensity,
										   Float32 Size, Float32 Rotate)

{
	QDT_ASSERT(NumBranches == 2 || NumBranches == 4 || NumBranches == 6);
	QDT_ASSERT(NumPasses % 2 == 1);

	_uiNumStarBranches = NumBranches;
	_uiNumStarPasses   = NumPasses;
	_fStarDiffraction  = Diffraction;
	_fStarIntensity    = Intensity;
	_fStarScale        = Size;
	_fStarRotate       = Rotate;
}

//-----------------------------------------------------------------------------
//	Name:		ResetTransform
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ResetTransform()
{
	_viewportTransform.Identity();
}




//-----------------------------------------------------------------------------
//	Name:		Translate
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::Translate(Float32 x, Float32 y)
{
	VECTOR tr = _viewportTransform.GetTranslation();
	_viewportTransform.SetTranslation(tr.GetX() + x, tr.GetY() - y / RENDER_CONTEXT::GetAspectRatio(), 0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		Rotate
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::Rotate(Float32 angle)
{
	MATRIX rot = MATRIX::IDENTITY;
	rot.SetRotZ(angle);
	MATRIX copy = _viewportTransform;
	_viewportTransform.Mul(copy, rot);
}

//-----------------------------------------------------------------------------
//	Name:		Shear
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::Shear(Float32 x, Float32 y)
{
	MATRIX shear = MATRIX(1, y, 0, 0,
						  x, 1, 0, 0,
						  0, 0, 1, 0,
						  0, 0, 0, 1);

	MATRIX copy = _viewportTransform;
	_viewportTransform.Mul(copy, shear);
}

//-----------------------------------------------------------------------------
//	Name:		ColorSaturation
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ColorSaturation(Float32 sat)
{
	float s1 = (0.33333f * (1.0f - sat)) + (1.0f * sat);
	float s2 = (1.0f - s1) / 2.0f;

	MATRIX satMat = MATRIX(s1, s2 , s2 , 0,
						   s2 , s1, s2 , 0,
						   s2 , s2 , s1, 0,
						   0  , 0  , 0  , 1);

	MATRIX tmp = _colorTransform; 

	_colorTransform.Mul(tmp, satMat);

}

//-----------------------------------------------------------------------------
//	Name:		ColorMultiply
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ColorMultiply(Float32 r, Float32 g, Float32 b)
{
	MATRIX mult = MATRIX(r, 0, 0, 0,
						 0, g, 0, 0,
						 0, 0, b, 0,
						 0, 0, 0, 1);

	MATRIX tmp = _colorTransform; 

	_colorTransform.Mul(tmp, mult);
}


//-----------------------------------------------------------------------------
//	Name:		ColorAdd
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ColorAdd(Float32 r, Float32 g, Float32 b)
{
	VECTOR tr = _colorTransform.GetTranslation();
	_colorTransform.SetTranslation(tr.GetX() + r, tr.GetY() + g , tr.GetZ() + b);
}

//-----------------------------------------------------------------------------
//	Name:		ColorHueShift
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ColorHueShift(Float32 hue)
{
	MATRIX hueMat = MATRIX::IDENTITY;
	hueMat.SetQuaternion(QUATERNION(VECTOR(1.0f, 1.0f, 1.0f), hue));

	MATRIX tmp = _colorTransform; 

	_colorTransform.Mul(tmp, hueMat);
}

//-----------------------------------------------------------------------------
//	Name:		ColorInvert
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ColorInvert()
{
	ColorMultiply(-1, -1, -1);
	ColorAdd(1, 1, 1);
}


//-----------------------------------------------------------------------------
//	Name:		ColorScreen
//	Object:		
//	06-02-01:	PDE - Created
//-----------------------------------------------------------------------------
void VIEWPORT_STATE::ColorScreen(Float32 r, Float32 g, Float32 b)
{
	ColorInvert();
	ColorMultiply(1.0f - r, 1.0f - g, 1.0f - b);
	ColorInvert();
}

//-----------------------------------------------------------------------------
//	Name:		ColorFadeToIdentity
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_STATE::ColorFadeToIdentity(Float32	val)
{
	Float32 invVal = 1.0f - val;

	MATRIX ident = MATRIX::IDENTITY;

	for (UInt32 x = 0; x < 4; x++)
	{
		for (UInt32 y = 0; y < 4; y++)
		{
			_colorTransform(x, y) = invVal * _colorTransform(x, y) + val * ident(x, y);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetScaleBorder
//	Object:		
//	06-02-20:	RMA - Created
//-----------------------------------------------------------------------------
void	VIEWPORT_STATE::GetScaleBorder(Float32 &	fScaleBorderX,
									   Float32 &	fScaleBorderY)
{
	UInt32 pixelWidth  = (UInt32)(_uiViewPortWidth * RENDER_CONTEXT::GetScreenWidth());
	UInt32 pixelHeight = (UInt32)(_uiViewPortHeight * RENDER_CONTEXT::GetScreenHeight());

	UInt32 newPixelWidth = pixelWidth + (PHOTOGRAPHIC_PIPELINE::GetBorderSize() * 2);
	UInt32 newPixelHeight = pixelHeight + (PHOTOGRAPHIC_PIPELINE::GetBorderSize() * 2);
	
	fScaleBorderX = (Float32)newPixelWidth / (Float32)pixelWidth;
	fScaleBorderY = (Float32)newPixelHeight / (Float32)pixelHeight;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================

