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
//	CLASS:	HAIR_CELL
//
//	05-12-02:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetMass
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetMass(Float32	rMass)
{
	_rMass = rMass;

	if	(_pActor)
	{
		_pActor->SetMass(_rMass);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetDamping
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetDamping(Float32	rLinearDamping,
							  Float32	rAngularDamping)
{
	_rLinearDamping = rLinearDamping;
	_rAngularDamping = rAngularDamping;

	if	(_pActor)
	{
		_pActor->SetLinearDamping(_rLinearDamping);
		_pActor->SetAngularDamping(_rAngularDamping);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingLimit
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetSwingLimit(Float32	rSwingLimit)
{
	_rSwingLimit = rSwingLimit;

	if	(_pJoint)
	{
		_pJoint->SetSwing1LimitValue(_rSwingLimit);
		_pJoint->SetSwing2LimitValue(_rSwingLimit);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSpring
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetSpring(Float32	rSpring,
							 Float32	rDamping)
{
	_rSpringValue = rSpring;
	_rSpringDamping = rDamping;

	if	(_pJoint)
	{
		_pJoint->SetSwingDriveSpring(_rSpringValue);
		_pJoint->SetSwingDriveDamping(_rSpringDamping);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetBounciness
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetBounciness(Float32	rBounciness)
{
	_rBounciness = rBounciness;

	if	(_pMaterial)
	{
		_pMaterial->SetBounciness(rBounciness);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFriction
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetFriction(Float32	rFriction)
{
	_rFriction = rFriction;

	if	(_pMaterial)
	{
		_pMaterial->SetDynamicFriction(_rFriction);
		_pMaterial->SetStaticFriction(_rFriction);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetLevel
//	Object:		
//	05-12-07:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetLevel(UInt32	uiLevel)
{
	_uiLevel = uiLevel;
}

//-----------------------------------------------------------------------------
//	Name:		GetLevel
//	Object:		
//	06-02-27:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	HAIR_CELL::GetLevel() const
{
	return	(_uiLevel);
}

//-----------------------------------------------------------------------------
//	Name:		SetFather
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetFather(HAIR_CELL *	pFather)
{
	_pFather = pFather;
}

//-----------------------------------------------------------------------------
//	Name:		GetFather
//	Object:		
//	05-12-08:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL *	HAIR_CELL::GetFather()
{
	return	(_pFather);
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstChild
//	Object:		
//	06-03-08:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL *	HAIR_CELL::GetFirstChild()
{
	return	(_pFirstChild);
}

//-----------------------------------------------------------------------------
//	Name:		SetBrother
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetBrother(HAIR_CELL *	pBrother)
{
	_pBrother = pBrother;
}

//-----------------------------------------------------------------------------
//	Name:		GetBrother
//	Object:		
//	05-12-02:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL *	HAIR_CELL::GetBrother()
{
	return	(_pBrother);
}

//-----------------------------------------------------------------------------
//	Name:		SetNextSameLevel
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
void	HAIR_CELL::SetNextSameLevel(HAIR_CELL *	pCell)
{
	_pNextSameLevel = pCell;
}

//-----------------------------------------------------------------------------
//	Name:		GetNextSameLevel
//	Object:		
//	06-02-28:	ELE - Created
//-----------------------------------------------------------------------------
HAIR_CELL *	HAIR_CELL::GetNextSameLevel()
{
	return	(_pNextSameLevel);
}

//-----------------------------------------------------------------------------
//	Name:		GetActor
//	Object:		
//	05-12-05:	ELE - Created
//-----------------------------------------------------------------------------
I_PHYSIC_DYNAMIC_ACTOR *	HAIR_CELL::GetActor()
{
	return	(_pActor);
}

//-----------------------------------------------------------------------------
//	Name:		GetNodeChild
//	Object:		
//	05-12-05:	ELE - Created
//-----------------------------------------------------------------------------
const NODE_CHILD *	HAIR_CELL::GetNodeChild() const
{
	return	(_pNodeChild);
}

//-----------------------------------------------------------------------------
//	Name:		GetAxis
//	Object:		
//	06-03-09:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	HAIR_CELL::GetAxis() const
{
	return	(_Axis);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
