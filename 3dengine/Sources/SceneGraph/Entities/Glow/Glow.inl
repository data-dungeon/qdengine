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
//	CLASS:	GLOW
//
//	02-04-23:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetFlags
//	Object:		
//	02-04-24:	ELE - Created
//-----------------------------------------------------------------------------
int	GLOW::GetFlags(int	nFlagsMask) const
{
	return	(_nFlags & nFlagsMask);
}

//-----------------------------------------------------------------------------
//	Name:		GetNormal
//	Object:		
//	02-04-24:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	GLOW::GetNormal() const
{
	return	(_Normal);
}

//-----------------------------------------------------------------------------
//	Name:		GetOuterAngle
//	Object:		
//	02-04-25:	ELE - Created
//-----------------------------------------------------------------------------
float	GLOW::GetOuterAngle() const
{
	return	(MATH::RadianToDegre(_rOuterAngle));
}

//-----------------------------------------------------------------------------
//	Name:		GetInnerAngle
//	Object:		
//	02-04-30:	ELE - Created
//-----------------------------------------------------------------------------
float	GLOW::GetInnerAngle() const
{
	return	(MATH::RadianToDegre(_rInnerAngle));
}

//-----------------------------------------------------------------------------
//	Name:		GetColorMax
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
const COLOR &	GLOW::GetColorMax() const
{
	return	(_DiffuseColorMax);
}

//-----------------------------------------------------------------------------
//	Name:		GetColorMin
//	Object:		
//	02-04-25:	ELE - Created
//-----------------------------------------------------------------------------
const COLOR &	GLOW::GetColorMin() const
{
	return	(_DiffuseColorMin);
}

//-----------------------------------------------------------------------------
//	Name:		GetScaleMax
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	GLOW::GetScaleMax() const
{
	return	(_ScaleMax);
}

//-----------------------------------------------------------------------------
//	Name:		GetScaleMin
//	Object:		
//	02-04-24:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	GLOW::GetScaleMin() const
{
	return	(_ScaleMin);
}

//-----------------------------------------------------------------------------
//	Name:		GetUVs
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
const POINT2D *	GLOW::GetUVs() const
{
	return	(&_pUVs[0]);
}

//-----------------------------------------------------------------------------
//	Name:		GetSpinAngle
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
float	GLOW::GetSpinAngle() const
{
	return	(MATH::RadianToDegre(_rSpinAngle));
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterialInst
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	GLOW::GetMaterialInst() const
{
	return	(_MaterialInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance
//	Object:		
//	02-04-25:	ELE - Created
//-----------------------------------------------------------------------------
float	GLOW::GetDistance() const
{
	return	(_rDistance);
}

//-----------------------------------------------------------------------------
//	Name:		SetFlags
//	Object:		
//	02-04-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetFlags(int	nFlagsMask,
					   int	nFlagsSet)
{
	_nFlags &= ~nFlagsMask;
	_nFlags |=  nFlagsSet;
}

//-----------------------------------------------------------------------------
//	Name:		SetNormal
//	Object:		
//	02-04-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetNormal(const VECTOR &	Normal)
{
	_Normal = Normal;
	_Normal.Normalize();
}

//-----------------------------------------------------------------------------
//	Name:		SetOuterAngle
//	Object:		
//	02-04-25:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetOuterAngle(const float	rAngle)
{
	_rOuterAngle = MATH::DegreToRadian(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		SetInnerAngle
//	Object:		
//	02-04-30:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetInnerAngle(const float	rAngle)
{
	_rInnerAngle = MATH::DegreToRadian(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		SetColorMax
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetColorMax(const COLOR &	Color)
{
	_DiffuseColorMax = Color;
}

//-----------------------------------------------------------------------------
//	Name:		SetColorMin
//	Object:		
//	02-04-25:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetColorMin(const COLOR &	Color)
{
	_DiffuseColorMin = Color;
}

//-----------------------------------------------------------------------------
//	Name:		SetScaleMax
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetScaleMax(const VECTOR &	Scale)
{
	_ScaleMax = Scale;
}

//-----------------------------------------------------------------------------
//	Name:		SetScaleMin
//	Object:		
//	02-04-24:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetScaleMin(const VECTOR &	Scale)
{
	_ScaleMin = Scale;
}

//-----------------------------------------------------------------------------
//	Name:		SetUVs
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetUVs(const POINT2D *	pUVs)
{
	_pUVs[0] = pUVs[0];
	_pUVs[1] = pUVs[1];
	_pUVs[2] = pUVs[2];
	_pUVs[3] = pUVs[3];
}

//-----------------------------------------------------------------------------
//	Name:		SetSpinAngle
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetSpinAngle(const float	rSpinAngle)
{
	_rSpinAngle = MATH::DegreToRadian(rSpinAngle);
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterialInst
//	Object:		
//	02-04-23:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetMaterialInst(const COM_INSTANCE & MaterialInst)
{
	_MaterialInst = MaterialInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance
//	Object:		
//	02-04-25:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetDistance(const float	rDistance)
{
	_rDistance = rDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetLensFlareInnerAngle
//	Object:		
//	02-04-30:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetLensFlareInnerAngle(float	rAngle)
{
	_rLensFlareInnerAngle = MATH::DegreToRadian(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		SetCirclePosition
//	Object:		
//	02-04-26:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCirclePosition(int		nCircleIndex,
								float	rPosition)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesPosition[nCircleIndex] = rPosition;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleIntensity
//	Object:		
//	02-04-26:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleIntensity(int	nCircleIndex,
								 float	rIntensity)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesIntensity[nCircleIndex] = rIntensity;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleSize
//	Object:		
//	02-04-26:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleSize(int		nCircleIndex,
							float	rSize)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesSize[nCircleIndex] = rSize;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleSpinAngle
//	Object:		
//	02-05-02:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleSpinAngle(int	nCircleIndex,
								 float	rAngle)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesSpinAngle[nCircleIndex] = MATH::DegreToRadian(rAngle);
	}
}

//-----------------------------------------------------------------------------
//	Name:		EnableCircleOwnColor
//	Object:		
//	02-05-02:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::EnableCircleOwnColor(int	nCircleIndex,
								   bool	bValue)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesOwnColor[nCircleIndex] = bValue;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleColorMin
//	Object:		
//	02-05-02:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleColorMin(int				nCircleIndex,
								const COLOR &	Color)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesColorMin[nCircleIndex] = Color;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleColorMax
//	Object:		
//	02-05-02:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleColorMax(int				nCircleIndex,
								const COLOR &	Color)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_pCirclesColorMax[nCircleIndex] = Color;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetCircleMaterialInst
//	Object:		
//	02-04-26:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetCircleMaterialInst(int						nCircleIndex,
									const COM_INSTANCE &	MaterialInst)
{
	if	(nCircleIndex < _nNbrCircles)
	{
		_vCirclesMaterialInst.PushTail(MaterialInst);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSpawnTime
//	Object:		
//	02-04-30:	ELE - Created
//-----------------------------------------------------------------------------
void	GLOW::SetSpawnTime(int	nSpawnTime)
{
	_nSpawnTime = nSpawnTime;
}

//-----------------------------------------------------------------------------
//	Name:		IsActive
//	Object:		
//	04-03-30:	RMA - Created
//-----------------------------------------------------------------------------
bool	GLOW::IsActive() const
{
	return (GetFlags(GLOW_IS_ACTIVE) != 0);
}

//-----------------------------------------------------------------------------
//	Name:		SetActive
//	Object:		
//	04-03-30:	RMA - Created
//-----------------------------------------------------------------------------
void	GLOW::SetActive(bool b)
{
	if (b)
	{
		SetFlags(GLOW_IS_ACTIVE, GLOW_IS_ACTIVE);
	}
	else
	{
		SetFlags(GLOW_IS_ACTIVE, 0);
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
