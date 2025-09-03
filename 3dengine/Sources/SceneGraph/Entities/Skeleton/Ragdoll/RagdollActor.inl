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
//	CLASS:	RAGDOLL_ACTOR
//
//	06-03-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetDensity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetDensity() const
{
	return	(_rDensity);
}

//-----------------------------------------------------------------------------
//	Name:		GetMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetMass() const
{
	return	(_rMass);
}

//-----------------------------------------------------------------------------
//	Name:		GetCenterOfMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	RAGDOLL_ACTOR::GetCenterOfMass() const
{
	return	(_CenterOfMass);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetLinearDamping() const
{
	return	(_LinearDamping);
}

//-----------------------------------------------------------------------------
//	Name:		GetAngularDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetAngularDamping() const
{
	return	(_AngularDamping);
}

//-----------------------------------------------------------------------------
//	Name:		IsInfluencedByGravity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Bool	RAGDOLL_ACTOR::IsInfluencedByGravity() const
{
	return	(_bGravity);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrSolverIterations
//	Object:		
//	06-02-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RAGDOLL_ACTOR::GetNbrSolverIterations() const
{
	return	(_nNbrSolverIterations);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrShapesData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RAGDOLL_ACTOR::GetNbrShapesData() const
{
	return	(_nNbrShapesData);
}

//-----------------------------------------------------------------------------
//	Name:		GetShapeData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
const PHYSIC_SHAPE_DATA *	RAGDOLL_ACTOR::GetShapeData(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _nNbrShapesData);
	return	(_pShapesData[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetBounciness
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetBounciness() const
{
	return	(_rBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetStaticFriction
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetStaticFriction() const
{
	return	(_rStaticFriction);
}

//-----------------------------------------------------------------------------
//	Name:		GetDynamicFriction
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_ACTOR::GetDynamicFriction() const
{
	return	(_rDynamicFriction);
}

//-----------------------------------------------------------------------------
//	Name:		SetDensity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetDensity(Float32	rDensity)
{
	_rDensity = rDensity;
}

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetMass(Float32	rMass)
{
	_rMass = rMass;
}

//-----------------------------------------------------------------------------
//	Name:		SetCenterOfMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetCenterOfMass(const VECTOR &	CenterOfMass)
{
	_CenterOfMass = CenterOfMass;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetLinearDamping(Float32	LinearDamping)
{
	_LinearDamping = LinearDamping;
}

//-----------------------------------------------------------------------------
//	Name:		SetAngularDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetAngularDamping(Float32	AngularDamping)
{
	_AngularDamping = AngularDamping;
}

//-----------------------------------------------------------------------------
//	Name:		SetInfluencedByGravity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetInfluencedByGravity(Bool	bGravity)
{
	_bGravity = bGravity;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrSolverIterations
//	Object:		
//	06-02-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetNbrSolverIterations(UInt32	nNbrSolverIterations)
{
	_nNbrSolverIterations = nNbrSolverIterations;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrShapesData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetNbrShapesData(UInt32	nNbrShapesData)
{
	_nNbrShapesData = nNbrShapesData;
	_pShapesData = new PHYSIC_SHAPE_DATA*[nNbrShapesData];
}

//-----------------------------------------------------------------------------
//	Name:		SetShapeData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetShapeData(UInt32				nIndex,
										  PHYSIC_SHAPE_DATA *	pShapeData)
{
	QDT_ASSERT(nIndex < _nNbrShapesData);
	_pShapesData[nIndex] = pShapeData;
}

//-----------------------------------------------------------------------------
//	Name:		SetBounciness
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetBounciness(Float32	rBounciness)
{
	_rBounciness = rBounciness;
}

//-----------------------------------------------------------------------------
//	Name:		SetStaticFriction
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetStaticFriction(Float32	rStaticFriction)
{
	_rStaticFriction = rStaticFriction;
}

//-----------------------------------------------------------------------------
//	Name:		SetDynamicFriction
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_ACTOR::SetDynamicFriction(Float32	rDynamicFriction)
{
	_rDynamicFriction = rDynamicFriction;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
