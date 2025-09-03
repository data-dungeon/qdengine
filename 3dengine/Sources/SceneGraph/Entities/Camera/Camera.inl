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
// FILE: Camera.inl
//
//
//	01-02-08:	RMA - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name:		CAMERA::GetFocalAngle
// Object:		Get focal angle
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetFocal() const
{
	return ( _fFocal );
}

//-----------------------------------------------------------------------------
// Name:		CAMERA::GetAspectRatio
// Object:		Get aspect ratio
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetAspectRatio() const
{
	return (_fAspectRatio);
}

//-----------------------------------------------------------------------------
// Name:		CAMERA::GetNearClippingDistance
// Object:		Get near clipping distance
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetNearClippingDistance() const
{
	return (_fNearClippingDistance);
}

//-----------------------------------------------------------------------------
// Name:		CAMERA::GetFarClippingDistance
// Object:		Get far clipping distance
// 00-11-13		JVA Created:
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetFarClippingDistance() const
{
	return (_fFarClippingDistance);
}

//-----------------------------------------------------------------------------
//	Name:		GetViewMatrix
//	Object:		
//	05-11-02:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	CAMERA::GetViewMatrix() const
{
	return	(_mView);
}

//-----------------------------------------------------------------------------
//	Name:		SetFocal
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetFocal(Float32	fFocal)
{
	_fFocal = fFocal;
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectionMatrix
//	Object:		
//	05-10-26:	ELE - Created
//-----------------------------------------------------------------------------
const MATRIX &	CAMERA::GetProjectionMatrix() const
{
	return	(_mProjection);
}

//-----------------------------------------------------------------------------
//	Name:		SetAspectRatio
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetAspectRatio(Float32	fValue)
{
	_fAspectRatio = fValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetScale
//	Object:		
//	06-02-06:	FBO - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetScale(Float32	fScale)
{
	_fScale = fScale;	
}

//-----------------------------------------------------------------------------
//	Name:		SetNearClippingDistance
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetNearClippingDistance(Float32	fNearClippingDistance)
{
	_fNearClippingDistance = fNearClippingDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetFarClippingDistance
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetFarClippingDistance(Float32 fFarClippingDistance)
{
	_fFarClippingDistance = fFarClippingDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetHorizontalFieldOfView
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetHorizontalFieldOfView(Float32 fFieldOfView)
{
	_fHorizontalFieldOfView = fFieldOfView;
}

//-----------------------------------------------------------------------------
//	Name:		GetHorizontalFieldOfView
//	Object:		
//	06-02-03:	RMA - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetHorizontalFieldOfView() const
{
	return (_fHorizontalFieldOfView);
}

//-----------------------------------------------------------------------------
//	Name:		SetExposure
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetExposure(Float32	f)
{
	_fExposure = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetBrightBlurRadius
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetBrightBlurRadius(Float32	f)
{
	_fBrightBlurRadius = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetBrightThreshold
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetBrightThreshold(Float32	f)
{
	_fBrightThreshold = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetBrightIntensity
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetBrightIntensity(Float32	f)
{
	_fBrightIntensity = f;
}

//-----------------------------------------------------------------------------
//	Name:		EnableBloom
//	Object:		
//	06-02-16:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::EnableBloom(Bool	b)
{
	_bEnableBloom = b;
}

//-----------------------------------------------------------------------------
//	Name:		EnableGhosts
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::EnableGhosts(Bool b)
{
	_bEnableGhosts = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetGhostsDiffraction
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetGhostsDiffraction(Float32 f)
{
	_fGhostsDiffraction = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetGhostsIntensity
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetGhostsIntensity(Float32 f)
{
	_fGhostsIntensity = f;
}

//-----------------------------------------------------------------------------
//	Name:		EnableStars
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::EnableStars(Bool	b)
{
	_bEnableStars = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetStarsNbrBranches
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetStarsNbrBranches(UInt32	ui)
{
	_uiStarsNbrBranches = ui;
}

//-----------------------------------------------------------------------------
//	Name:		SetStarsSize
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetStarsSize(Float32	f)
{
	_fStarsSize = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetStarsDiffraction
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetStarsDiffraction(Float32	f)
{
	_fStarsDiffraction = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetStarsIntensity
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetStarsIntensity(Float32	f)
{
	_fStarsIntensity = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetStarsAngle
//	Object:		
//	06-02-17:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetStarsAngle(Float32	f)
{
	_fStarsAngle = f;
}

//-----------------------------------------------------------------------------
//	Name:		GetScale
//	Object:		
//	06-02-17:	RMA - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetScale() const
{
	return (_fScale);
}

//-----------------------------------------------------------------------------
//	Name:		SetEnableNoise
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetEnableNoise(Bool	b)
{
	_bEnableNoise = b;
}

//-----------------------------------------------------------------------------
//	Name:		SetNoiseCutoff
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetNoiseCutoff(Float32	f)
{
	_fNoiseCutoff = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetNoiseIntensity
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void CAMERA::SetNoiseIntensity(Float32 f)
{
	_fNoiseIntensity = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetNoiseScale
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void CAMERA::SetNoiseScale(Float32 f)
{
	_fNoiseScale = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetNoiseFps
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void CAMERA::SetNoiseFps(Float32 f)
{
	_fNoiseFps = f;
}

//-----------------------------------------------------------------------------
//	Name:		SetColorControlStage
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void CAMERA::SetColorControlStage(UInt32 nStageId, COLORCONTROL_OPERATION OpCode, Float32 rParam0, Float32 rParam1, Float32 rParam2)
{
	_colorControlStages[nStageId]._uiOperationId = OpCode;
	_colorControlStages[nStageId]._fParam0 = rParam0;
	_colorControlStages[nStageId]._fParam1 = rParam1;
	_colorControlStages[nStageId]._fParam2 = rParam2;
}


//-----------------------------------------------------------------------------
//	Name:		SetColorControlFade
//	Object:		
//	06-02-20:	PDE - Created
//-----------------------------------------------------------------------------
void CAMERA::SetColorControlFade(Float32 f)
{
	_fColorControlFade = f;
}

//-----------------------------------------------------------------------------
//	Name:		EnableDepthOfField
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
void CAMERA::SetEnableDepthOfField(Bool bEnableDepthOfField)
{
	_bEnableDepthOfField =bEnableDepthOfField;
}

//-----------------------------------------------------------------------------
//	Name:		SetFarDepthOfField
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
void CAMERA::SetFarDepthOfField(Float32 rFarDepthOfField)
{
	_fFarDepthOfField =rFarDepthOfField;
}

//-----------------------------------------------------------------------------
//	Name:		SetFocusPlane
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
void CAMERA::SetFocusPlane(Float32 rFocusPlane)
{
	_fFocusPlane =rFocusPlane;
}

//-----------------------------------------------------------------------------
//	Name:		SetFocusRange
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
void CAMERA::SetFocusRange(Float32 rFocusRange)
{
	_fFocusRange =rFocusRange;
}


//-----------------------------------------------------------------------------
//	Name:		SetFocusSafeRange
//	Object:		
//	06-02-27:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetFocusSafeRange(Float32	rFocusSafeRange)
{
	_fFocusSafeRange = rFocusSafeRange;
}


//-----------------------------------------------------------------------------
//	Name:		SetCircleOfConfusion
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
void CAMERA::SetCircleOfConfusion(Float32 rCircleOfConfusion)
{
	_fCircleOfConfusion =rCircleOfConfusion;
}

//-----------------------------------------------------------------------------
//	Name:		GetEnableDepthOfField
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
const Bool& CAMERA::GetEnableDepthOfField(void) const
{
	return _bEnableDepthOfField;
}

//-----------------------------------------------------------------------------
//	Name:		GetFarDepthOfField
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetFarDepthOfField(void) const
{
	return _fFarDepthOfField;
}

//-----------------------------------------------------------------------------
//	Name:		GetFocusPlane
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetFocusPlane(void) const
{
	return _fFocusPlane;
}

//-----------------------------------------------------------------------------
//	Name:		GetFocusRange
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetFocusRange(void) const
{
	return _fFocusRange;
}

//-----------------------------------------------------------------------------
//	Name:		GetFocusSafeRange
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetFocusSafeRange(void) const
{
	return _fFocusSafeRange;
}

//-----------------------------------------------------------------------------
//	Name:		GetCircleOfConfusion
//	Object:		
//	06-02-23:	BFA - Created
//-----------------------------------------------------------------------------
const Float32& CAMERA::GetCircleOfConfusion(void) const
{
	return _fCircleOfConfusion;
}


//-----------------------------------------------------------------------------
//	Name:		GetGamma
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetGamma(void	) const
{
	return _fGamma;
}

//-----------------------------------------------------------------------------
//	Name:		SetGamma
//	Object:		
//	06-03-01:	PDE - Created
//-----------------------------------------------------------------------------
void	CAMERA::SetGamma(Float32	rGamma)
{
	_fGamma = rGamma;
}


//-----------------------------------------------------------------------------
//	Name:		GetExposure
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetExposure() const
{
	return (_fExposure);
}

//-----------------------------------------------------------------------------
//	Name:		GetBrightBlurRadius
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetBrightBlurRadius() const
{
	return (_fBrightBlurRadius);
}

//-----------------------------------------------------------------------------
//	Name:		GetBrightThreshold
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetBrightThreshold() const
{
	return (_fBrightThreshold);
}

//-----------------------------------------------------------------------------
//	Name:		GetBrightIntensity
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetBrightIntensity() const
{
	return (_fBrightIntensity);
}

//-----------------------------------------------------------------------------
//	Name:		IsBloomEnable
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Bool&	CAMERA::IsBloomEnable() const
{
	return (_bEnableBloom);
}

//-----------------------------------------------------------------------------
//	Name:		IsGhostsEnable
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Bool&	CAMERA::IsGhostsEnable() const
{
	return (_bEnableGhosts);
}

//-----------------------------------------------------------------------------
//	Name:		GetGhostsDiffraction
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetGhostsDiffraction() const
{
	return (_fGhostsDiffraction);
}

//-----------------------------------------------------------------------------
//	Name:		GetGhostsIntensity
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetGhostsIntensity() const
{
	return (_fGhostsIntensity);
}

//-----------------------------------------------------------------------------
//	Name:		IsStarsEnable
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Bool&	CAMERA::IsStarsEnable() const
{
	return (_bEnableStars);
}

//-----------------------------------------------------------------------------
//	Name:		GetStarsNbrBranches
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const UInt32&	CAMERA::GetStarsNbrBranches() const
{
	return (_uiStarsNbrBranches);
}

//-----------------------------------------------------------------------------
//	Name:		GetStarsSize
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetStarsSize() const
{
	return (_fStarsSize);
}

//-----------------------------------------------------------------------------
//	Name:		GetStarsDiffraction
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetStarsDiffraction() const
{
	return (_fStarsDiffraction);
}

//-----------------------------------------------------------------------------
//	Name:		GetStarsIntensity
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetStarsIntensity() const
{
	return (_fStarsIntensity);
}

//-----------------------------------------------------------------------------
//	Name:		GetStarsAngle
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetStarsAngle() const
{
	return (_fStarsAngle);
}

//-----------------------------------------------------------------------------
//	Name:		IsNoiseEnable
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Bool&	CAMERA::IsNoiseEnable() const
{
	return (_bEnableNoise);
}

//-----------------------------------------------------------------------------
//	Name:		GetNoiseCutoff
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetNoiseCutoff() const
{
	return (_fNoiseCutoff);
}

//-----------------------------------------------------------------------------
//	Name:		GetNoiseIntensity
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetNoiseIntensity() const
{
	return (_fNoiseIntensity);
}

//-----------------------------------------------------------------------------
//	Name:		GetNoiseScale
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetNoiseScale() const
{
	return (_fNoiseScale);
}

//-----------------------------------------------------------------------------
//	Name:		GetNoiseFps
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32&	CAMERA::GetNoiseFps() const
{
	return (_fNoiseFps);
}

//-----------------------------------------------------------------------------
//	Name:		GetColorControlFade
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
const Float32 &	CAMERA::GetColorControlFade() const
{
	return (_fColorControlFade);
}

//-----------------------------------------------------------------------------
//	Name:		GetColorControlStage
//	Object:		
//	06-04-03:	NHO - Created
//-----------------------------------------------------------------------------
void	CAMERA::GetColorControlStage(const UInt32 &				nStageId,
									 COLORCONTROL_OPERATION &	OpCode,
									 Float32 &					rParam0,
									 Float32 &					rParam1,
									 Float32 &					rParam2) const
{
	OpCode = _colorControlStages[nStageId]._uiOperationId;
	rParam0 = _colorControlStages[nStageId]._fParam0;
	rParam1 = _colorControlStages[nStageId]._fParam1;
	rParam2 = _colorControlStages[nStageId]._fParam2;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
