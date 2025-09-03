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
//	CLASS:	CONSTRAINT_DATA
//
//	06-03-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetActor1CI
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	CONSTRAINT_DATA::GetActor1CI() const
{
	return	(_Actor1CI);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildIndex1
//	Object:		
//	06-03-23:	ELE - Created
//-----------------------------------------------------------------------------
Int32	CONSTRAINT_DATA::GetChildIndex1() const
{
	return	(_iChildIndex1);
}

//-----------------------------------------------------------------------------
//	Name:		GetActor2CI
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	CONSTRAINT_DATA::GetActor2CI() const
{
	return	(_Actor2CI);
}

//-----------------------------------------------------------------------------
//	Name:		GetChildIndex1
//	Object:		
//	06-03-23:	ELE - Created
//-----------------------------------------------------------------------------
Int32	CONSTRAINT_DATA::GetChildIndex2() const
{
	return	(_iChildIndex2);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalAnchor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	CONSTRAINT_DATA::GetGlobalAnchor() const
{
	return	(_GlobalAnchor);
}

//-----------------------------------------------------------------------------
//	Name:		GetGlobalAxis
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	CONSTRAINT_DATA::GetGlobalAxis() const
{
	return	(_GlobalAxis);
}

//-----------------------------------------------------------------------------
//	Name:		IsCollisionEnabled
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
bool	CONSTRAINT_DATA::IsCollisionEnabled() const
{
	return	(_bCollision);
}

//-----------------------------------------------------------------------------
//	Name:		IsProjectionEnabled
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
bool	CONSTRAINT_DATA::IsProjectionEnabled() const
{
	return	(_bProjection);
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectionDistance
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetProjectionDistance() const
{
	return	(_rProjectionDistance);
}

//-----------------------------------------------------------------------------
//	Name:		GetProjectionAngle
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetProjectionAngle() const
{
	return	(_rProjectionAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetXMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	CONSTRAINT_DATA::GetXMotionType() const
{
	return	(_XMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetYMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	CONSTRAINT_DATA::GetYMotionType() const
{
	return	(_YMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetZMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	CONSTRAINT_DATA::GetZMotionType() const
{
	return	(_ZMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1MotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	CONSTRAINT_DATA::GetSwing1MotionType() const
{
	return	(_Swing1MotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2MotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	CONSTRAINT_DATA::GetSwing2MotionType() const
{
	return	(_Swing2MotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_MOTION	CONSTRAINT_DATA::GetTwistMotionType() const
{
	return	(_TwistMotionType);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetLinearLimitValue() const
{
	return	(_rLinearLimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetLinearLimitRestitution() const
{
	return	(_rLinearLimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetLinearLimitSpring() const
{
	return	(_rLinearLimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetLinearLimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetLinearLimitDamper() const
{
	return	(_rLinearLimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing1LimitValue() const
{
	return	(_rSwing1LimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing1LimitRestitution() const
{
	return	(_rSwing1LimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing1LimitSpring() const
{
	return	(_rSwing1LimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing1LimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing1LimitDamper() const
{
	return	(_rSwing1LimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing2LimitValue() const
{
	return	(_rSwing2LimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing2LimitRestitution() const
{
	return	(_rSwing2LimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing2LimitSpring() const
{
	return	(_rSwing2LimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwing2LimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwing2LimitDamper() const
{
	return	(_rSwing2LimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistLowLimitValue() const
{
	return	(_rTwistLowLimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistLowLimitRestitution() const
{
	return	(_rTwistLowLimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistLowLimitSpring() const
{
	return	(_rTwistLowLimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLowLimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistLowLimitDamper() const
{
	return	(_rTwistLowLimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistHighLimitValue() const
{
	return	(_rTwistHighLimitValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistHighLimitRestitution() const
{
	return	(_rTwistHighLimitRestitution);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistHighLimitSpring() const
{
	return	(_rTwistHighLimitSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistHighLimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetTwistHighLimitDamper() const
{
	return	(_rTwistHighLimitDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
JOINT_CUSTOM_DRIVE_TYPE	CONSTRAINT_DATA::GetSwingDriveType() const
{
	return	(_SwingDriveType);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwingDriveSpring() const
{
	return	(_rSwingDriveSpring);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
Float32	CONSTRAINT_DATA::GetSwingDriveDamper() const
{
	return	(_rSwingDriveDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveOrientation
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
const QUATERNION &	CONSTRAINT_DATA::GetSwingDriveOrientation() const
{
	return	(_SwingDriveOrientation);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingDriveAngularVelocity
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
const VECTOR &	CONSTRAINT_DATA::GetSwingDriveAngularVelocity() const
{
	return	(_SwingDriveAngularVelocity);
}

//-----------------------------------------------------------------------------
//	Name:		SetActor1CI
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetActor1CI(const COM_INSTANCE &	Actor1CI)
{
	_Actor1CI = Actor1CI;
}

//-----------------------------------------------------------------------------
//	Name:		SetChildIndex1
//	Object:		
//	06-03-23:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetChildIndex1(Int32	iChildIndex1)
{
	_iChildIndex1 = iChildIndex1;
}

//-----------------------------------------------------------------------------
//	Name:		SetActor2CI
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetActor2CI(const COM_INSTANCE &	Actor2CI)
{
	_Actor2CI = Actor2CI;
}

//-----------------------------------------------------------------------------
//	Name:		SetChildIndex2
//	Object:		
//	06-03-23:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetChildIndex2(Int32	iChildIndex2)
{
	_iChildIndex2 = iChildIndex2;
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAnchor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetGlobalAnchor(const VECTOR &	GlobalAnchor)
{
	_GlobalAnchor = GlobalAnchor;
}

//-----------------------------------------------------------------------------
//	Name:		SetGlobalAxis
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetGlobalAxis(const VECTOR &	GlobalAxis)
{
	_GlobalAxis = GlobalAxis;
}

//-----------------------------------------------------------------------------
//	Name:		EnableCollision
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::EnableCollision(bool	bCollision)
{
	_bCollision = bCollision;
}

//-----------------------------------------------------------------------------
//	Name:		EnableProjection
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::EnableProjection(bool	bProjection)
{
	_bProjection = bProjection;
}

//-----------------------------------------------------------------------------
//	Name:		SetProjectionDistance
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetProjectionDistance(Float32	rProjectionDistance)
{
	_rProjectionDistance = rProjectionDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetProjectionAngle
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetProjectionAngle(Float32	rProjectionAngle)
{
	_rProjectionAngle = rProjectionAngle;
}

//-----------------------------------------------------------------------------
//	Name:		SetXMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetXMotionType(JOINT_CUSTOM_MOTION	XMotionType)
{
	_XMotionType = XMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetYMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetYMotionType(JOINT_CUSTOM_MOTION	YMotionType)
{
	_YMotionType = YMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetZMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetZMotionType(JOINT_CUSTOM_MOTION	ZMotionType)
{
	_ZMotionType = ZMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1MotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing1MotionType(JOINT_CUSTOM_MOTION	Swing1MotionType)
{
	_Swing1MotionType = Swing1MotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2MotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing2MotionType(JOINT_CUSTOM_MOTION	Swing2MotionType)
{
	_Swing2MotionType = Swing2MotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistMotionType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistMotionType(JOINT_CUSTOM_MOTION	TwistMotionType)
{
	_TwistMotionType = TwistMotionType;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetLinearLimitValue(Float32	rLinearLimitValue)
{
	_rLinearLimitValue = rLinearLimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetLinearLimitRestitution(Float32	rLinearLimitRestitution)
{
	_rLinearLimitRestitution = rLinearLimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetLinearLimitSpring(Float32	rLinearLimitSpring)
{
	_rLinearLimitSpring = rLinearLimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetLinearLimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetLinearLimitDamper(Float32	rLinearLimitDamper)
{
	_rLinearLimitDamper = rLinearLimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing1LimitValue(Float32	rSwing1LimitValue)
{
	_rSwing1LimitValue = rSwing1LimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing1LimitRestitution(Float32	rSwing1LimitRestitution)
{
	_rSwing1LimitRestitution = rSwing1LimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing1LimitSpring(Float32	rSwing1LimitSpring)
{
	_rSwing1LimitSpring = rSwing1LimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing1LimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing1LimitDamper(Float32	rSwing1LimitDamper)
{
	_rSwing1LimitDamper = rSwing1LimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing2LimitValue(Float32	rSwing2LimitValue)
{
	_rSwing2LimitValue = rSwing2LimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing2LimitRestitution(Float32	rSwing2LimitRestitution)
{
	_rSwing2LimitRestitution = rSwing2LimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing2LimitSpring(Float32	rSwing2LimitSpring)
{
	_rSwing2LimitSpring = rSwing2LimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwing2LimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwing2LimitDamper(Float32	rSwing2LimitDamper)
{
	_rSwing2LimitDamper = rSwing2LimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistLowLimitValue(Float32	rTwistLowLimitValue)
{
	_rTwistLowLimitValue = rTwistLowLimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistLowLimitRestitution(Float32	rTwistLowLimitRestitution)
{
	_rTwistLowLimitRestitution = rTwistLowLimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistLowLimitSpring(Float32	rTwistLowLimitSpring)
{
	_rTwistLowLimitSpring = rTwistLowLimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLowLimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistLowLimitDamper(Float32	rTwistLowLimitDamper)
{
	_rTwistLowLimitDamper = rTwistLowLimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitValue
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistHighLimitValue(Float32	rTwistHighLimitValue)
{
	_rTwistHighLimitValue = rTwistHighLimitValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitRestitution
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistHighLimitRestitution(Float32	rTwistHighLimitRestitution)
{
	_rTwistHighLimitRestitution = rTwistHighLimitRestitution;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistHighLimitSpring(Float32	rTwistHighLimitSpring)
{
	_rTwistHighLimitSpring = rTwistHighLimitSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistHighLimitDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetTwistHighLimitDamper(Float32	rTwistHighLimitDamper)
{
	_rTwistHighLimitDamper = rTwistHighLimitDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveType
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwingDriveType(JOINT_CUSTOM_DRIVE_TYPE	SwingDriveType)
{
	_SwingDriveType = SwingDriveType;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveSpring
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwingDriveSpring(Float32	rSwingDriveSpring)
{
	_rSwingDriveSpring = rSwingDriveSpring;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveDamper
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwingDriveDamper(Float32	rSwingDriveDamper)
{
	_rSwingDriveDamper = rSwingDriveDamper;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveOrientation
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwingDriveOrientation(const QUATERNION &	SwingDriveOrientation)
{
	_SwingDriveOrientation = SwingDriveOrientation;
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingDriveAngularVelocity
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::SetSwingDriveAngularVelocity(const VECTOR &	SwingDriveAngularVelocity)
{
	_SwingDriveAngularVelocity = SwingDriveAngularVelocity;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
