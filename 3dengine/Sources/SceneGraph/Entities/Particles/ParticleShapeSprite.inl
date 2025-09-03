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
//	CLASS:	PARTICLE_SHAPE_SPRITE
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetMaterialInst
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetMaterialInst(const COM_INSTANCE & MaterialInst)
{
	_MaterialInst = MaterialInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterialInst
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	PARTICLE_SHAPE_SPRITE::GetMaterialInst() const
{
	return	(_MaterialInst);
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetShapeSpriteData
//	Object:		
//	03-09-10:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE_SPRITE_DATA *	PARTICLE_SHAPE_SPRITE::GetShapeSpriteData() const
{
	return	(_pShapeSpriteData);
}
*/
//-----------------------------------------------------------------------------
//	Name:		GetUVsNbKeys
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
int	PARTICLE_SHAPE_SPRITE::GetUVsNbKeys() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return	(_pShapeSpriteData->GetUVsNbKeys());
}

//-----------------------------------------------------------------------------
//	Name:		SetUVsNbKeys
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetUVsNbKeys(int	nNbrKeys)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetUVsNbKeys(nNbrKeys);
}

//-----------------------------------------------------------------------------
//	Name:		SetUVs
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetUVs(int			nKey,
									  const POINT2D	pUVs[4])
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetUVs(nKey, pUVs);
}

//-----------------------------------------------------------------------------
//	Name:		GetUVs
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::GetUVs(float		rLife,
									  POINT2D	pUVs[4]) const
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->GetUVs(rLife, pUVs);
}

//-----------------------------------------------------------------------------
//	Name:		GetUVsTimeKey
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE_SHAPE_SPRITE::GetUVsTimeKey(int	nKey) const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);

//	return	(_pShapeSpriteData->GetUVsTimeKey(nKey));
}

//-----------------------------------------------------------------------------
//	Name:		SetUVsTimeKey
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetUVsTimeKey(int	nKey,
											 float	rTimeKey)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetUVsTimeKey(nKey, rTimeKey);
}

//-----------------------------------------------------------------------------
//	Name:		AddColorTracks
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::AddColorTracks(int	nNbrColorTracks)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->AddColorTracks(nNbrColorTracks);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveColorTrack
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::RemoveColorTrack(int	nTrackIndex)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->RemoveColorTrack(nTrackIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetColorTrack
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
int	PARTICLE_SHAPE_SPRITE::GetColorTrack() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return	(_pShapeSpriteData->GetColorTrack());
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrColorTracks
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
int	PARTICLE_SHAPE_SPRITE::GetNbrColorTracks() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return	(_pShapeSpriteData->GetNbrColorTracks());
}

//-----------------------------------------------------------------------------
//	Name:		SetColorTrack
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetColorTrack(int	nColorTrack)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetColorTrack(nColorTrack);
}

//-----------------------------------------------------------------------------
//	Name:		SetDiffuseColor
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetDiffuseColor(const COLOR &	Color)
{
	SetDiffuseColor(0, 0, Color);
}

//-----------------------------------------------------------------------------
//	Name:		GetDiffuseColorNbKeys
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
int	PARTICLE_SHAPE_SPRITE::GetDiffuseColorNbKeys(int nTrackIndex) const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return	(_pShapeSpriteData->GetDiffuseColorNbKeys(nTrackIndex));
}

//-----------------------------------------------------------------------------
//	Name:		SetDiffuseColorNbKeys
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetDiffuseColorNbKeys(int	nTrackIndex,
													 int	nNbrKeys)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetDiffuseColorNbKeys(nTrackIndex, nNbrKeys);
}

//-----------------------------------------------------------------------------
//	Name:		SetDiffuseColor
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetDiffuseColor(int				nTrackIndex,
											   int				nKey,
											   const COLOR &	Color)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetDiffuseColor(nTrackIndex, nKey, Color);
}

//-----------------------------------------------------------------------------
//	Name:		GetDiffuseColor
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::GetDiffuseColor(int		nTrackIndex,
											   float	rLife,
											   COLOR &	Color) const
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->GetDiffuseColor(nTrackIndex, rLife, Color);
}

//-----------------------------------------------------------------------------
//	Name:		GetDiffuseColorTimeKey
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE_SHAPE_SPRITE::GetDiffuseColorTimeKey(int	nTrackIndex,
													  int	nKey) const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);

//	return	(_pShapeSpriteData->GetDiffuseColorTimeKey(nTrackIndex, nKey));
}

//-----------------------------------------------------------------------------
//	Name:		SetDiffuseColorTimeKey
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetDiffuseColorTimeKey(int	nTrackIndex,
													  int	nKey,
													  float	rTimeKey)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetDiffuseColorTimeKey(nTrackIndex, nKey, rTimeKey);
}
/*
//-----------------------------------------------------------------------------
//	Name:		SetDiffuseColorTrack
//	Object:		
//	03-12-16:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetDiffuseColorTrack(QDT_VECTOR < DATA_TRACK < COLOR > * >	DiffuseColorTrack)
{
	_pShapeSpriteData->SetDiffuseColorTrack(DiffuseColorTrack);
}
*/
//-----------------------------------------------------------------------------
//	Name:		IsAligned
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE_SHAPE_SPRITE::IsAligned() const
{
	QDT_NOT_IMPLEMENTED();
	return	(false);

//	return	(_pShapeSpriteData->IsAligned());
}

//-----------------------------------------------------------------------------
//	Name:		SetAligned
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetAligned(bool	b)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetAligned(b);
}

//-----------------------------------------------------------------------------
//	Name:		GetAlignmentAxis
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::GetAlignmentAxis(VECTOR &	Axis1,
												VECTOR &	Axis2) const
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->GetAlignmentAxis(Axis1, Axis2);
}
/*
//-----------------------------------------------------------------------------
//	Name:		GetAlignmentAxis1
//	Object:		
//	04-03-18:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE_SHAPE_SPRITE::GetAlignmentAxis1() const
{
	return	(_pShapeSpriteData->GetAlignmentAxis1());
}

//-----------------------------------------------------------------------------
//	Name:		GetAlignmentAxis2
//	Object:		
//	04-03-18:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE_SHAPE_SPRITE::GetAlignmentAxis2() const
{
	return	(_pShapeSpriteData->GetAlignmentAxis2());
}
*/
//-----------------------------------------------------------------------------
//	Name:		SetAlignmentAxis
//	Object:		
//	04-03-15:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE_SPRITE::SetAlignmentAxis(const VECTOR &	Axis1,
												const VECTOR &	Axis2)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeSpriteData->SetAlignmentAxis(Axis1, Axis2);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
