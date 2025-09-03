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
//	CLASS:	DYNAMIC_PHYSIC_DATA
//
//	06-02-16:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetDensity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetDensity() const
{
	return	(_rDensity);
}

//-----------------------------------------------------------------------------
//	Name:		GetMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetMass() const
{
	return	(_rMass);
}

//-----------------------------------------------------------------------------
//	Name:		GetCenterOfMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	DYNAMIC_PHYSIC_DATA::GetCenterOfMass() const
{
	return	(_CenterOfMass);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetLinearDamping() const
{
	return	(_LinearDamping);
}

//-----------------------------------------------------------------------------
//	Name:		GetAngularDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetAngularDamping() const
{
	return	(_AngularDamping);
}

//-----------------------------------------------------------------------------
//	Name:		IsKinematic
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Bool	DYNAMIC_PHYSIC_DATA::IsKinematic() const
{
	return	(_bKinematic);
}

//-----------------------------------------------------------------------------
//	Name:		IsInfluencedByGravity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
Bool	DYNAMIC_PHYSIC_DATA::IsInfluencedByGravity() const
{
	return	(_bGravity);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrSolverIterations
//	Object:		
//	06-02-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	DYNAMIC_PHYSIC_DATA::GetNbrSolverIterations() const
{
	return	(_nNbrSolverIterations);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrShapesData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	DYNAMIC_PHYSIC_DATA::GetNbrShapesData() const
{
	return	(_nNbrShapesData);
}

//-----------------------------------------------------------------------------
//	Name:		GetShapeData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
const PHYSIC_SHAPE_DATA *	DYNAMIC_PHYSIC_DATA::GetShapeData(UInt32	nIndex) const
{
	QDT_ASSERT(nIndex < _nNbrShapesData);
	return	(_pShapesData[nIndex]);
}

//-----------------------------------------------------------------------------
//	Name:		GetBounciness
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetBounciness() const
{
	return	(_rBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetStaticFriction
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetStaticFriction() const
{
	return	(_rStaticFriction);
}

//-----------------------------------------------------------------------------
//	Name:		GetDynamicFriction
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
Float32	DYNAMIC_PHYSIC_DATA::GetDynamicFriction() const
{
	return	(_rDynamicFriction);
}

//-----------------------------------------------------------------------------
//	Name:		SetDensity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetDensity(Float32	rDensity)
{
	_rDensity = rDensity;
}

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetMass(Float32	rMass)
{
	_rMass = rMass;
}

//-----------------------------------------------------------------------------
//	Name:		SetCenterOfMass
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetCenterOfMass(const VECTOR &	CenterOfMass)
{
	_CenterOfMass = CenterOfMass;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetLinearDamping(Float32	LinearDamping)
{
	_LinearDamping = LinearDamping;
}

//-----------------------------------------------------------------------------
//	Name:		SetAngularDamping
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetAngularDamping(Float32	AngularDamping)
{
	_AngularDamping = AngularDamping;
}

//-----------------------------------------------------------------------------
//	Name:		SetKinematic
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetKinematic(Bool	bKinematic)
{
	_bKinematic = bKinematic;
}

//-----------------------------------------------------------------------------
//	Name:		SetInfluencedByGravity
//	Object:		
//	06-02-16:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetInfluencedByGravity(Bool	bGravity)
{
	_bGravity = bGravity;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrSolverIterations
//	Object:		
//	06-02-20:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetNbrSolverIterations(UInt32	nNbrSolverIterations)
{
	_nNbrSolverIterations = nNbrSolverIterations;
}

//-----------------------------------------------------------------------------
//	Name:		SetNbrShapesData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetNbrShapesData(UInt32	nNbrShapesData)
{
	_nNbrShapesData = nNbrShapesData;
	_pShapesData = new PHYSIC_SHAPE_DATA*[nNbrShapesData];
}

//-----------------------------------------------------------------------------
//	Name:		SetShapeData
//	Object:		
//	06-02-17:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetShapeData(UInt32				nIndex,
										  PHYSIC_SHAPE_DATA *	pShapeData)
{
	QDT_ASSERT(nIndex < _nNbrShapesData);
	_pShapesData[nIndex] = pShapeData;
}

//-----------------------------------------------------------------------------
//	Name:		SetBounciness
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetBounciness(Float32	rBounciness)
{
	_rBounciness = rBounciness;
}

//-----------------------------------------------------------------------------
//	Name:		SetStaticFriction
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetStaticFriction(Float32	rStaticFriction)
{
	_rStaticFriction = rStaticFriction;
}

//-----------------------------------------------------------------------------
//	Name:		SetDynamicFriction
//	Object:		
//	06-02-21:	ELE - Created
//-----------------------------------------------------------------------------
void	DYNAMIC_PHYSIC_DATA::SetDynamicFriction(Float32	rDynamicFriction)
{
	_rDynamicFriction = rDynamicFriction;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
