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
//	CLASS:	RAGDOLL_CONSTRAINT
//
//	06-03-20:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetActor1Index
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RAGDOLL_CONSTRAINT::GetActor1Index() const
{
	return	(_uiActor1Index);
}

//-----------------------------------------------------------------------------
//	Name:		GetActor2Index
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
UInt32	RAGDOLL_CONSTRAINT::GetActor2Index() const
{
	return	(_uiActor2Index);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalAnchor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	RAGDOLL_CONSTRAINT::GetGlobalAnchor() const
{
	return	(_GlobalAnchor);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalAxis
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	RAGDOLL_CONSTRAINT::GetGlobalAxis() const
{
	return	(_GlobalAxis);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalNormal
//	Object:		
//	06-03-22:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	RAGDOLL_CONSTRAINT::GetGlobalNormal() const
{
	return	(_GlobalNormal);
}

//-----------------------------------------------------------------------------
//	Name:		IsCollisionEnabled
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	RAGDOLL_CONSTRAINT::IsCollisionEnabled() const
{
	return	(_bCollision);
}

//-----------------------------------------------------------------------------
//	Name:		IsProjectionEnabled
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	RAGDOLL_CONSTRAINT::IsProjectionEnabled() const
{
	return	(_bProjection);
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectionDistance
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetProjectionDistance() const
{
	return	(_rProjectionDistance);
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectionAngle
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetProjectionAngle() const
{
	return	(_rProjectionAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetXMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	RAGDOLL_CONSTRAINT::GetXMotionType() const
{
	return	(_XMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetYMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	RAGDOLL_CONSTRAINT::GetYMotionType() const
{
	return	(_YMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetZMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	RAGDOLL_CONSTRAINT::GetZMotionType() const
{
	return	(_ZMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1MotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	RAGDOLL_CONSTRAINT::GetSwing1MotionType() const
{
	return	(_Swing1MotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2MotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	RAGDOLL_CONSTRAINT::GetSwing2MotionType() const
{
	return	(_Swing2MotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	RAGDOLL_CONSTRAINT::GetTwistMotionType() const
{
	return	(_TwistMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetLinearLimitValue() const
{
	return	(_rLinearLimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetLinearLimitRestitution() const
{
	return	(_rLinearLimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetLinearLimitSpring() const
{
	return	(_rLinearLimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetLinearLimitDamper() const
{
	return	(_rLinearLimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing1LimitValue() const
{
	return	(_rSwing1LimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing1LimitRestitution() const
{
	return	(_rSwing1LimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing1LimitSpring() const
{
	return	(_rSwing1LimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing1LimitDamper() const
{
	return	(_rSwing1LimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing2LimitValue() const
{
	return	(_rSwing2LimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing2LimitRestitution() const
{
	return	(_rSwing2LimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing2LimitSpring() const
{
	return	(_rSwing2LimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwing2LimitDamper() const
{
	return	(_rSwing2LimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistLowLimitValue() const
{
	return	(_rTwistLowLimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistLowLimitRestitution() const
{
	return	(_rTwistLowLimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistLowLimitSpring() const
{
	return	(_rTwistLowLimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistLowLimitDamper() const
{
	return	(_rTwistLowLimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistHighLimitValue() const
{
	return	(_rTwistHighLimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistHighLimitRestitution() const
{
	return	(_rTwistHighLimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistHighLimitSpring() const
{
	return	(_rTwistHighLimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetTwistHighLimitDamper() const
{
	return	(_rTwistHighLimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_DRIVE_TYPE	RAGDOLL_CONSTRAINT::GetSwingDriveType() const
{
	return	(_SwingDriveType);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwingDriveSpring() const
{
	return	(_rSwingDriveSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	RAGDOLL_CONSTRAINT::GetSwingDriveDamper() const
{
	return	(_rSwingDriveDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveOrientation
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
const QUATERNION &	RAGDOLL_CONSTRAINT::GetSwingDriveOrientation() const
{
	return	(_SwingDriveOrientation);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveAngularVelocity
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	RAGDOLL_CONSTRAINT::GetSwingDriveAngularVelocity() const
{
	return	(_SwingDriveAngularVelocity);
}

//-----------------------------------------------------------------------------
//	Name:		SetActor1Index
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetActor1Index(UInt32	uiActor1Index)
{
	_uiActor1Index = uiActor1Index;
}

//-----------------------------------------------------------------------------
//	Name:		SetActor2Index
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetActor2Index(UInt32	uiActor2Index)
{
	_uiActor2Index = uiActor2Index;
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAnchor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetGlobalAnchor(const VECTOR &	GlobalAnchor)
{
	_GlobalAnchor = GlobalAnchor;
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAxis
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetGlobalAxis(const VECTOR &	GlobalAxis)
{
	_GlobalAxis = GlobalAxis;
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalNormal
//	Object:		
//	06-03-22:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetGlobalNormal(const VECTOR &	GlobalNormal)
{
	_GlobalNormal = GlobalNormal;
}

//-----------------------------------------------------------------------------
//	Name:		EnableCollision
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::EnableCollision(bool	bCollision)
{
	_bCollision = bCollision;
}

//-----------------------------------------------------------------------------
//	Name:		EnableProjection
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::EnableProjection(bool	bProjection)
{
	_bProjection = bProjection;
}

//-----------------------------------------------------------------------------
//	Name:		SetProjectionDistance
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetProjectionDistance(Float32	rProjectionDistance)
{
	_rProjectionDistance = rProjectionDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetProjectionAngle
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetProjectionAngle(Float32	rProjectionAngle)
{
	_rProjectionAngle = rProjectionAngle;
}

//-----------------------------------------------------------------------------
//	Name:		SetXMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetXMotionType(JOINT_CUSTOM_MOTION	XMotionType)
{
	_XMotionType = XMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetYMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetYMotionType(JOINT_CUSTOM_MOTION	YMotionType)
{
	_YMotionType = YMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetZMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetZMotionType(JOINT_CUSTOM_MOTION	ZMotionType)
{
	_ZMotionType = ZMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1MotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing1MotionType(JOINT_CUSTOM_MOTION	Swing1MotionType)
{
	_Swing1MotionType = Swing1MotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2MotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing2MotionType(JOINT_CUSTOM_MOTION	Swing2MotionType)
{
	_Swing2MotionType = Swing2MotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistMotionType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistMotionType(JOINT_CUSTOM_MOTION	TwistMotionType)
{
	_TwistMotionType = TwistMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetLinearLimitValue(Float32	rLinearLimitValue)
{
	_rLinearLimitValue = rLinearLimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetLinearLimitRestitution(Float32	rLinearLimitRestitution)
{
	_rLinearLimitRestitution = rLinearLimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetLinearLimitSpring(Float32	rLinearLimitSpring)
{
	_rLinearLimitSpring = rLinearLimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetLinearLimitDamper(Float32	rLinearLimitDamper)
{
	_rLinearLimitDamper = rLinearLimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing1LimitValue(Float32	rSwing1LimitValue)
{
	_rSwing1LimitValue = rSwing1LimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing1LimitRestitution(Float32	rSwing1LimitRestitution)
{
	_rSwing1LimitRestitution = rSwing1LimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing1LimitSpring(Float32	rSwing1LimitSpring)
{
	_rSwing1LimitSpring = rSwing1LimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing1LimitDamper(Float32	rSwing1LimitDamper)
{
	_rSwing1LimitDamper = rSwing1LimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing2LimitValue(Float32	rSwing2LimitValue)
{
	_rSwing2LimitValue = rSwing2LimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing2LimitRestitution(Float32	rSwing2LimitRestitution)
{
	_rSwing2LimitRestitution = rSwing2LimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing2LimitSpring(Float32	rSwing2LimitSpring)
{
	_rSwing2LimitSpring = rSwing2LimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwing2LimitDamper(Float32	rSwing2LimitDamper)
{
	_rSwing2LimitDamper = rSwing2LimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistLowLimitValue(Float32	rTwistLowLimitValue)
{
	_rTwistLowLimitValue = rTwistLowLimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistLowLimitRestitution(Float32	rTwistLowLimitRestitution)
{
	_rTwistLowLimitRestitution = rTwistLowLimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistLowLimitSpring(Float32	rTwistLowLimitSpring)
{
	_rTwistLowLimitSpring = rTwistLowLimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistLowLimitDamper(Float32	rTwistLowLimitDamper)
{
	_rTwistLowLimitDamper = rTwistLowLimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitValue
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistHighLimitValue(Float32	rTwistHighLimitValue)
{
	_rTwistHighLimitValue = rTwistHighLimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitRestitution
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistHighLimitRestitution(Float32	rTwistHighLimitRestitution)
{
	_rTwistHighLimitRestitution = rTwistHighLimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistHighLimitSpring(Float32	rTwistHighLimitSpring)
{
	_rTwistHighLimitSpring = rTwistHighLimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetTwistHighLimitDamper(Float32	rTwistHighLimitDamper)
{
	_rTwistHighLimitDamper = rTwistHighLimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveType
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwingDriveType(JOINT_CUSTOM_DRIVE_TYPE	SwingDriveType)
{
	_SwingDriveType = SwingDriveType;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveSpring
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwingDriveSpring(Float32	rSwingDriveSpring)
{
	_rSwingDriveSpring = rSwingDriveSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveDamper
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwingDriveDamper(Float32	rSwingDriveDamper)
{
	_rSwingDriveDamper = rSwingDriveDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveOrientation
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwingDriveOrientation(const QUATERNION &	SwingDriveOrientation)
{
	_SwingDriveOrientation = SwingDriveOrientation;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveAngularVelocity
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::SetSwingDriveAngularVelocity(const VECTOR &	SwingDriveAngularVelocity)
{
	_SwingDriveAngularVelocity = SwingDriveAngularVelocity;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
