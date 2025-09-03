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
// FILE: ForceFieldABC.inl
//
//
//	01-03-12:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::GetMagnitude
//	Object:		Get the magnitude
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
float FORCE_FIELD_ABC::GetMagnitude() const
{
	return (_rMagnitude);
}

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::GetDirection
//	Object:		Get the direction
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR & FORCE_FIELD_ABC::GetDirection() const
{
	return (_Direction);
}

//-----------------------------------------------------------------------------
//	Name:		FORCE_FIELD_ABC::SetDirection
//	Object:		Set the direction
//	01-03-12:	RMA - Created
//-----------------------------------------------------------------------------
void FORCE_FIELD_ABC::SetDirection(const VECTOR & Vector)
{
	_Direction = Vector;
}

//-----------------------------------------------------------------------------
//	Name:		SetTransformed
//	Object:		
//	01-11-30:	ELE - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::SetTransformed(bool	bValue)
{
	_bTransformed = bValue;
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	03-10-02:	FPR - Created
//-----------------------------------------------------------------------------
FORCE_FIELD_ABC::TYPE_FORCE_FIELD	FORCE_FIELD_ABC::GetType() const
{
	return (_eType);
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	03-10-02:	FPR - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::SetType(TYPE_FORCE_FIELD	eType)
{
	_eType = eType;
}

//-----------------------------------------------------------------------------
//	Name:		IsUniformField
//	Object:		
//	03-10-02:	FPR - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::IsUniformField() const
{
	return (_eType == TFF_UNIFORM_FORCE_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		IsRadialField
//	Object:		
//	03-10-02:	FPR - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::IsRadialField() const
{
	return (_eType == TFF_RADIAL_FORCE_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		IsVortexField
//	Object:		
//	03-10-02:	FPR - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::IsVortexField() const
{
	return (_eType == TFF_VORTEX_FORCE_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		IsLifeField
//	Object:		
//	03-10-02:	FPR - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::IsLifeField() const
{
	return (_eType == TFF_LIFE_FORCE_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		IsPerturbationField
//	Object:		
//	03-10-07:	FPR - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::IsPerturbationField() const
{
		return (_eType == TFF_PERTURBATION_FORCE_FIELD);
}

//-----------------------------------------------------------------------------
//	Name:		IsActive
//	Object:		
//	04-11-08:	RMA - Created
//-----------------------------------------------------------------------------
bool	FORCE_FIELD_ABC::IsActive() const
{
	return (_bActive);
}

//-----------------------------------------------------------------------------
//	Name:		SetActive
//	Object:		
//	04-11-08:	RMA - Created
//-----------------------------------------------------------------------------
void	FORCE_FIELD_ABC::SetActive(bool b)
{
	_bActive = b;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
