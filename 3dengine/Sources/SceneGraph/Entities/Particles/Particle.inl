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
//	CLASS:	PARTICLE
//
//	01-11-19:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetMass
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE::GetMass() const
{
	return	(_rMass);
}

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetMass(float	rMass)
{
	_rMass = rMass;

	if	(rMass == 0.0f)
	{
		_OneOnMass = 0.0f;
	}
	else
	{
		_OneOnMass = 1.0f / rMass;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetMomentOfInertia
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE::GetMomentOfInertia() const
{
	return	(_rMomentOfInertia);
}

//-----------------------------------------------------------------------------
//	Name:		SetMomentOfInertia
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetMomentOfInertia(float	rMomentOfInertia)
{
	_rMomentOfInertia = rMomentOfInertia;
}

//-----------------------------------------------------------------------------
//	Name:		GetPosition
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
const POINT3D &	PARTICLE::GetPosition() const
{
	return	(_Position);
}

//-----------------------------------------------------------------------------
//	Name:		SetPosition
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetPosition(const POINT3D &	Position)
{
	_Position = Position;
}

//-----------------------------------------------------------------------------
//	Name:		UpdatePosition
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::UpdatePosition(const VECTOR &	dPosition)
{
	_Position.Add(dPosition);
}

//-----------------------------------------------------------------------------
//	Name:		GetSpinAngle
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE::GetSpinAngle() const
{
	return	(_rSpinAngle);
}

//-----------------------------------------------------------------------------
//	Name:		SetSpinAngle
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetSpinAngle(float	rSpinAngle)
{
	_rSpinAngle = rSpinAngle;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSpinAngle
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::UpdateSpinAngle(float	rSpinAngle)
{
	_rSpinAngle += rSpinAngle;
}

//-----------------------------------------------------------------------------
//	Name:		GetSpinAxis
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE::GetSpinAxis() const
{
	return	(_SpinAxis);
}

//-----------------------------------------------------------------------------
//	Name:		SetSpinAxis
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetSpinAxis(const VECTOR &	SpinAxis)
{
	_SpinAxis = SpinAxis;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSpinAxis
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::UpdateSpinAxis(const VECTOR &	dSpinAxis)
{
	_SpinAxis.Add( dSpinAxis );
}

//-----------------------------------------------------------------------------
//	Name:		GetSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE::GetSpeed() const
{
	return	(_Speed);
}

//-----------------------------------------------------------------------------
//	Name:		SetSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetSpeed(const VECTOR &	Speed)
{
	_Speed = Speed;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::UpdateSpeed(const VECTOR &	dSpeed)
{
	_Speed.Add(dSpeed);
}

//-----------------------------------------------------------------------------
//	Name:		GetSpinAngleSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE::GetSpinAngleSpeed() const
{
	return	(_rSpinAngleSpeed);
}

//-----------------------------------------------------------------------------
//	Name:		SetSpinAngleSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetSpinAngleSpeed(float	rSpinAngleSpeed)
{
	_rSpinAngleSpeed = rSpinAngleSpeed;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSpinAngleSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::UpdateSpinAngleSpeed(float	rSpinAngleSpeed)
{
	_rSpinAngleSpeed += rSpinAngleSpeed;
}

//-----------------------------------------------------------------------------
//	Name:		GetSpinAxisSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE::GetSpinAxisSpeed() const
{
	return	(_SpinAxisSpeed);
}

//-----------------------------------------------------------------------------
//	Name:		SetSpinAxisSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetSpinAxisSpeed(const VECTOR &	SpinAxisSpeed)
{
	_SpinAxisSpeed = SpinAxisSpeed;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateSpinAxisSpeed
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::UpdateSpinAxisSpeed(const VECTOR &	dSpinAxisSpeed)
{
	_SpinAxisSpeed.Add(dSpinAxisSpeed);
}

//-----------------------------------------------------------------------------
//	Name:		GetLife
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
float	PARTICLE::GetLife() const
{
	return	(_rLife);
}

//-----------------------------------------------------------------------------
//	Name:		OnlyUniformField
//	Object:		
//	02-01-31:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE::OnlyUniformField() const
{
	return	(_bOnlyUniformField);
}

//-----------------------------------------------------------------------------
//	Name:		GetShapeInst
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	PARTICLE::GetShapeInst() const
{
	return (_ShapeInst);
}

//-----------------------------------------------------------------------------
//	Name:		SetShapeInst
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetShapeInst(const COM_INSTANCE &	ShapeInst)
{
	_ShapeInst = ShapeInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	01-11-19:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE::PARTICLE_TYPE	PARTICLE::GetType() const
{
	return	(_nType);
}

//-----------------------------------------------------------------------------
//	Name:		IsActive
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE::IsActive() const
{
	return	(_bActive);
}

//-----------------------------------------------------------------------------
//	Name:		Activate
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::Activate()
{
	_bActive = true;
}

//-----------------------------------------------------------------------------
//	Name:		Desactivate
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::Desactivate()
{
	_bActive = false;
}

//-----------------------------------------------------------------------------
//	Name:		SetInfluencedByFog
//	Object:		
//	02-01-28:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetInfluencedByFog(bool	bValue)
{
	_bIsInfluencedByFog = bValue;
}

//-----------------------------------------------------------------------------
//	Name:		IsParticleSprite
//	Object:		
//	01-11-22:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE::IsParticleSprite() const
{
	return	(false);
}

//-----------------------------------------------------------------------------
//	Name:		GetNext
//	Object:		
//	02-01-31:	ELE - Created
//-----------------------------------------------------------------------------
PARTICLE *	PARTICLE::GetNext() const
{
	return	(_pNext);
}

//-----------------------------------------------------------------------------
//	Name:		SetNext
//	Object:		
//	02-01-31:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE::SetNext(PARTICLE *	pParticle)
{
	_pNext = pParticle;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
