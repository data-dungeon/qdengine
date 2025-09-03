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
//	CLASS:	PARTICLE_EMITTER
//
//	01-11-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetSpawnRate
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetSpawnRate(float	SpawnRate)
{
	_rSpawnRate = SpawnRate;
}

//-----------------------------------------------------------------------------
//	Name:		GetSpawnRate
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
float	PARTICLE_EMITTER::GetSpawnRate() const
{
	return ( _rSpawnRate );
}

//-----------------------------------------------------------------------------
//	Name:		SetStartAngle
//	Object:		
//	01-11-23:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetStartAngle(const VECTOR &	Angle)
{
	_StartAngle.SetX(MATH::DegreToRadian(Angle.GetX()));
	_StartAngle.SetY(MATH::DegreToRadian(Angle.GetY()));
	_StartAngle.SetZ(MATH::DegreToRadian(Angle.GetZ()));
}

//-----------------------------------------------------------------------------
//	Name:		GetStartAngle
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE_EMITTER::GetStartAngle() const
{
	return ( _StartAngle );
}

//-----------------------------------------------------------------------------
//	Name:		SetEndAngle
//	Object:		
//	01-11-23:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEndAngle(const VECTOR &	Angle)
{
	_EndAngle.SetX(MATH::DegreToRadian(Angle.GetX()));
	_EndAngle.SetY(MATH::DegreToRadian(Angle.GetY()));
	_EndAngle.SetZ(MATH::DegreToRadian(Angle.GetZ()));
}

//-----------------------------------------------------------------------------
//	Name:		GetEndAngle
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE_EMITTER::GetEndAngle() const
{
	return ( _EndAngle );
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionSpeed
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionSpeed(float	Speed)
{
	_rEmissionSpeed = Speed;
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpeed
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
float	PARTICLE_EMITTER::GetEmissionSpeed() const
{
	return ( _rEmissionSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionSpinAngle
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionSpinAngle(float	SpinAngle)
{
	_rEmissionSpinAngle = SpinAngle;
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpinAngle
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
float	PARTICLE_EMITTER::GetEmissionSpinAngle() const
{
	return ( _rEmissionSpinAngle );
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionSpinAxis
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionSpinAxis(const VECTOR &	Vector)
{
	_EmissionSpinAxis.Copy(Vector);
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpinAxis
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE_EMITTER::GetEmissionSpinAxis()
{
	return ( _EmissionSpinAxis );
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionSpinAngleSpeed
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionSpinAngleSpeed(float	SpinAngleSpeed)
{
	_rEmissionSpinAngleSpeed = SpinAngleSpeed;
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpinAngleSpeed
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
float	PARTICLE_EMITTER::GetEmissionSpinAngleSpeed() const
{
	return ( _rEmissionSpinAngleSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		SetEmissionSpinAxisSpeed
//	Object:		
//	01-11-20:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetEmissionSpinAxisSpeed(const VECTOR &	Vector)
{
	_EmissionSpinAxisSpeed.Copy(Vector);
}

//-----------------------------------------------------------------------------
//	Name:		GetEmissionSpinAxisSpeed
//	Object:		
//	02-04-12:	GGO - Created
//-----------------------------------------------------------------------------
const VECTOR &	PARTICLE_EMITTER::GetEmissionSpinAxisSpeed() const
{
	return ( _EmissionSpinAxisSpeed );
}

//-----------------------------------------------------------------------------
//	Name:		GetParticleShapeInst
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	PARTICLE_EMITTER::GetParticleShapeInst() const
{
	return (_ParticleShapeInst);
}

//-----------------------------------------------------------------------------
//	Name:		GetParticleShape
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
PARTICLE_SHAPE *	PARTICLE_EMITTER::GetParticleShape() const
{
	QDT_ASSERT(_ParticleShapeInst.HasHandle());
	return (static_cast<PARTICLE_SHAPE*>(_ParticleShapeInst.GetCommunicator()));
}

//-----------------------------------------------------------------------------
//	Name:		SetParticleShapeInst
//	Object:		
//	03-03-24:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetParticleShapeInst(const COM_INSTANCE &	ParticleShapeInst)
{
	_ParticleShapeInst = ParticleShapeInst;
}

//-----------------------------------------------------------------------------
//	Name:		IsActive
//	Object:		
//	01-11-26:	ELE - Created
//-----------------------------------------------------------------------------
bool	PARTICLE_EMITTER::IsActive() const
{
	return	(_bActive);
}

//-----------------------------------------------------------------------------
//	Name:		SetActive
//	Object:		
//	01-11-26:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetActive(bool	bActive)
{
	_bActive = bActive;
}

//-----------------------------------------------------------------------------
//	Name:		SetAutoSpawn
//	Object:		
//	04-03-16:	ELE - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetAutoSpawn(bool	bAutoSpawn)
{
	_bAutoSpawn = bAutoSpawn;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	03-10-13:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetBoundingBox(const OBB&	pBB)
{
	_BoundingBox = pBB;
}

//-----------------------------------------------------------------------------
//	Name:		SetViewMatrix
//	Object:		
//	03-10-14:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetViewMatrix(const MATRIX *	pViewMat)
{
//	_pSpriteTable->SetViewMatrix(pViewMat);
}

//-----------------------------------------------------------------------------
//	Name:		GetForceFieldInst
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	PARTICLE_EMITTER::GetForceFieldInst(int	nNumForce) const
{
	return (_vForceFields[nNumForce]);
}

//-----------------------------------------------------------------------------
//	Name:		GetForceFields
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
QDT_VECTOR <COM_INSTANCE>	PARTICLE_EMITTER::GetForceFields() const
{
	return (_vForceFields);
}

//-----------------------------------------------------------------------------
//	Name:		SetForceFieldInst
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::AddForceFieldInst(const COM_INSTANCE &	Forceinst)
{
	_vForceFields.PushTail(Forceinst);
}

//-----------------------------------------------------------------------------
//	Name:		SetForceFields
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetForceFields(QDT_VECTOR <COM_INSTANCE> pForcesInst)
{
	_vForceFields = pForcesInst;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrForceFields
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
int	PARTICLE_EMITTER::GetNbrForceFields()
{
	return (_nNbrForceFields);
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrForceFields
//	Object:		
//	03-10-15:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetNbrForceFields(int	nNbr)
{
	_nNbrForceFields = nNbr;
}

//-----------------------------------------------------------------------------
//	Name:		SetVisibilityDistance
//	Object:		
//	04-03-08:	FPR - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetVisibilityDistance(float	rDistance)
{
	_rVisibilityDistance = rDistance;
}

//-----------------------------------------------------------------------------
//	Name:		GetVisibilityDistance
//	Object:		
//	04-03-08:	FPR - Created
//-----------------------------------------------------------------------------
float	PARTICLE_EMITTER::GetVisibilityDistance() const
{
	return (_rVisibilityDistance);
}

//-----------------------------------------------------------------------------
//	Name:		SetNeedUpdate
//	Object:		
//	04-03-19:	RMA - Created
//-----------------------------------------------------------------------------
void	PARTICLE_EMITTER::SetNeedUpdate(bool	b)
{
	_bNeedUpdate = b;
}

//-----------------------------------------------------------------------------
//	Name:		NeedUpdate
//	Object:		
//	04-03-19:	RMA - Created
//-----------------------------------------------------------------------------
bool	PARTICLE_EMITTER::NeedUpdate() const
{
	return (_bNeedUpdate);
}

//-----------------------------------------------------------------------------
//	Name:		GetBoundingBox
//	Object:		
//	04-08-30:	RMA - Created
//-----------------------------------------------------------------------------
const OBB &	PARTICLE_EMITTER::GetBoundingBox() const
{
	return (_BoundingBox);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
