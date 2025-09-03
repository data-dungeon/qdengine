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
//	CLASS:	PARTICLE_SHAPE_ABC
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
/*
//-----------------------------------------------------------------------------
//	Name:		GetShapeData
//	Object:		
//	03-09-10:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE_DATA *	PARTICLE_SHAPE::GetShapeData() const
{
	return	(_pShapeData);
}

//-----------------------------------------------------------------------------
//	Name:		GetParticleType
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE::PARTICLE_TYPE	PARTICLE_SHAPE::GetParticleType() const
{
	return	(_pShapeData->GetParticleType());
}
*/
//-----------------------------------------------------------------------------
//	Name:		IsLifeSpanActive
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE_SHAPE::IsLifeSpanActive() const
{
	QDT_NOT_IMPLEMENTED();
	return	(true);

//	return	(_pShapeData->IsLifeSpanActive());
}

//-----------------------------------------------------------------------------
//	Name:		ActivateLifeSpan
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::ActivateLifeSpan()
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->ActivateLifeSpan();
}

//-----------------------------------------------------------------------------
//	Name:		DesactivateLifeSpan
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::DesactivateLifeSpan()
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->DesactivateLifeSpan();
}

//-----------------------------------------------------------------------------
//	Name:		GetLifeSpan
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE_SHAPE::GetLifeSpan() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);

//	return	(_pShapeData->GetLifeSpan());
}

//-----------------------------------------------------------------------------
//	Name:		SetLifeSpan
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetLifeSpan(float	rLifeSpan)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->SetLifeSpan(rLifeSpan);
}

//-----------------------------------------------------------------------------
//	Name:		GetMass
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE_SHAPE::GetMass() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);

//	return	(_pShapeData->GetMass());
}

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetMass(float	rMass)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->SetMass(rMass);
}

//-----------------------------------------------------------------------------
//	Name:		GetMomentOfInertia
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE_SHAPE::GetMomentOfInertia() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);

//	return	(_pShapeData->GetMomentOfInertia());
}

//-----------------------------------------------------------------------------
//	Name:		SetMomentOfInertia
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetMomentOfInertia(float	rMomentOfInertia)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->SetMomentOfInertia(rMomentOfInertia);
}

//-----------------------------------------------------------------------------
//	Name:		SetScale
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetScale(const VECTOR &	Scale)
{
	SetScale(0, Scale);
}

//-----------------------------------------------------------------------------
//	Name:		GetScaleNbKeys
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
int	PARTICLE_SHAPE::GetScaleNbKeys() const
{
	QDT_NOT_IMPLEMENTED();
	return	(0);

//	return	(_pShapeData->GetScaleNbKeys());
}

//-----------------------------------------------------------------------------
//	Name:		SetScaleNbKeys
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetScaleNbKeys(int	nNbrKeys)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->SetScaleNbKeys(nNbrKeys);
}

//-----------------------------------------------------------------------------
//	Name:		SetScale
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetScale(int				nKey,
								 const VECTOR &		Scale)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->SetScale(nKey, Scale);
}

//-----------------------------------------------------------------------------
//	Name:		GetScale
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::GetScale(float		rLife,
								 VECTOR &	Scale) const
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->GetScale(rLife, Scale);
}

//-----------------------------------------------------------------------------
//	Name:		GetScaleTimeKey
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE_SHAPE::GetScaleTimeKey(int	nKey) const
{
	QDT_NOT_IMPLEMENTED();
	return	(0.0f);

//	return	(_pShapeData->GetScaleTimeKey(nKey));
}

//-----------------------------------------------------------------------------
//	Name:		SetScaleTimeKey
//	Object:		
//	01-11-21:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetScaleTimeKey(int		nKey,
										float	rTimeKey)
{
	QDT_NOT_IMPLEMENTED();

//	_pShapeData->SetScaleTimeKey(nKey, rTimeKey);
}

/*
//-----------------------------------------------------------------------------
//	Name:		SetScaleTrack
//	Object:		
//	04-03-15:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_SHAPE::SetScaleTrack(const DATA_TRACK < VECTOR >& track)
{
	_pShapeData->SetScaleTrack(track);
}
*/
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
