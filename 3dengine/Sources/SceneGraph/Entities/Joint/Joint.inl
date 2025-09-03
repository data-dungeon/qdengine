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
// FILE: Joint.inl
//
//
//	01-02-08:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name: JOINT::Copy
//	Object: Copy method to avoid code duplication in operator = & 
//			copy constructor
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void JOINT::Copy(const JOINT & Joint)
{
	_nIndex = Joint._nIndex ;
}

//-----------------------------------------------------------------------------
//	Name: JOINT::SetIndex
//	Object: 
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void JOINT::SetIndex( int i)
{
	_nIndex = i;
}

//-----------------------------------------------------------------------------
//	Name: JOINT::GetIndex
//	Object: 
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
int JOINT::GetIndex() const
{
	return (_nIndex) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetType
//	Object:		
//	03-05-21:	SBE - Created
//-----------------------------------------------------------------------------
JOINT_TYPE	JOINT::GetType() const
{
	return (_Type);
}

//-----------------------------------------------------------------------------
//	Name:		SetType
//	Object:		
//	03-05-21:	SBE - Created
//-----------------------------------------------------------------------------
void	JOINT::SetType(JOINT_TYPE	Type)
{
	_Type = Type;
}
//-----------------------------------------------------------------------------
//	Name:		GetCollisionOBB
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
const OBB	JOINT::GetCollisionOBB() const
{
	return (_CollisionOBB);
}

//-----------------------------------------------------------------------------
//	Name:		GetCollisionCylinder
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
const CYLINDER	JOINT::GetCollisionCylinder() const
{
	return (_CollisionCylinder);
}

//-----------------------------------------------------------------------------
//	Name:		GetCollisionVolume
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
const BOUNDING_VOLUME *	JOINT::GetCollisionVolume() const
{
	switch(_nCollisionVType)
	{
	case NONE:
	default:  
		return (NULL);
		break;

	case COLLISION_OBB:
		return (&_CollisionOBB);

	case COLLISION_CYLINDER:
		return (&_CollisionCylinder);
   
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetCollisionVolumeType
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
int	JOINT::GetCollisionVolumeType() const
{
	return (_nCollisionVType);
}


//-----------------------------------------------------------------------------
//	Name:		SetCollisionVolume
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::SetCollisionVolume(const OBB&	pCV)
{
	_CollisionOBB = pCV;
	SetCollisionVolumeType(COLLISION_OBB);
}

//-----------------------------------------------------------------------------
//	Name:		SetCollisionVolume
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::SetCollisionVolume(const CYLINDER&	pCV)
{
	_CollisionCylinder = pCV;
	SetCollisionVolumeType(COLLISION_CYLINDER);	
}

//-----------------------------------------------------------------------------
//	Name:		SetCollisionVolumeType
//	Object:		
//	03-12-01:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::SetCollisionVolumeType(int	nType)
{
	_nCollisionVType = nType;
}


//-----------------------------------------------------------------------------
//	Name:		AddSoftBodyCollisionVolume
//	Object:		
//	04-01-05:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::AddSoftBodyCollisionVolume(BOUNDING_VOLUME *	bv)
{
	_vCollisionVolumes.PushTail(bv);
}

//-----------------------------------------------------------------------------
//	Name:		SetIsFoot
//	Object:		
//	04-01-07:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::SetIsFoot(bool	bValue)
{
	_bJointIsFoot = bValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetDistanceFromFloor
//	Object:		
//	04-01-07:	FPR - Created
//-----------------------------------------------------------------------------
void	JOINT::SetDistanceFromFloor(float	rValue)
{
	_rDistanceFromFloor = rValue;
}


//-----------------------------------------------------------------------------
//	Name:		GetIsFoot
//	Object:		
//	04-01-08:	FPR - Created
//-----------------------------------------------------------------------------
bool	JOINT::GetIsFoot() const
{
	return (_bJointIsFoot);
}

//-----------------------------------------------------------------------------
//	Name:		GetDistanceFromFloor
//	Object:		
//	04-01-08:	FPR - Created
//-----------------------------------------------------------------------------
float	JOINT::GetDistanceFromFloor() const
{
	return (_rDistanceFromFloor);
}

//-----------------------------------------------------------------------------
//	Name:		GetSoftBodyCollisionVolumes
//	Object:		
//	04-01-12:	ELE - Created
//-----------------------------------------------------------------------------
QDT_VECTOR < BOUNDING_VOLUME * > *	JOINT::GetSoftBodyCollisionVolumes()
{
	return	(&_vCollisionVolumes);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
