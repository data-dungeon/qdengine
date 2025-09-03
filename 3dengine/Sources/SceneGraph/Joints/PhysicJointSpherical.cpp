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
//	CLASS:	PHYSIC_JOINT_SPHERICAL
//
//	05-07-07:	ELE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJointSpherical)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicJointSpherical)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT_SPHERICAL constructor
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT_SPHERICAL::PHYSIC_JOINT_SPHERICAL()
:
PHYSIC_JOINT(CreateGenericCommunicatorDescriptor())
{
	_pJoint = PHYSIC_CONTEXT::Instance()->CreateJointSpherical();
}

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT_SPHERICAL destructor
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT_SPHERICAL::~PHYSIC_JOINT_SPHERICAL()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		EnableTwistLimit
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EnableTwistLimit(bool	bState)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->EnableTwistLimit(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsTwistLimitEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
bool	PHYSIC_JOINT_SPHERICAL::IsTwistLimitEnabled()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->IsTwistLimitEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLimitHighValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistLimitHighValue(Float32	rHighValue)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistLimitHighValue(rHighValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLimitHighValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistLimitHighValue()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistLimitHighValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLimitHighBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistLimitHighBounciness(Float32	rHighBounciness)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistLimitHighBounciness(rHighBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLimitHighBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistLimitHighBounciness()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistLimitHighBounciness());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLimitLowValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistLimitLowValue(Float32	rLowValue)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistLimitLowValue(rLowValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLimitLowValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistLimitLowValue()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistLimitLowValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistLimitLowBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistLimitLowBounciness(Float32	rLowBounciness)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistLimitLowBounciness(rLowBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistLimitLowBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistLimitLowBounciness()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistLimitLowBounciness());
}

//-----------------------------------------------------------------------------
//	Name:		EnableSwingLimit
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EnableSwingLimit(bool	bState)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->EnableSwingLimit(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsSwingLimitEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
bool	PHYSIC_JOINT_SPHERICAL::IsSwingLimitEnabled()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->IsSwingLimitEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingLimitValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetSwingLimitValue(Float32	rValue)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetSwingLimitValue(rValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingLimitValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetSwingLimitValue()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetSwingLimitValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingLimitBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetSwingLimitBounciness(Float32	rBounciness)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetSwingLimitBounciness(rBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingLimitBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetSwingLimitBounciness()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetSwingLimitBounciness());
}

//-----------------------------------------------------------------------------
//	Name:		EnableTwistSpring
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EnableTwistSpring(bool	bState)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->EnableTwistSpring(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsTwistSpringEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
bool	PHYSIC_JOINT_SPHERICAL::IsTwistSpringEnabled()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->IsTwistSpringEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistSpringValue(Float32	rValue)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistSpringValue(rValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistSpringValue()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistSpringValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistSpringDamper(Float32	rDamper)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistSpringDamper(rDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistSpringDamper()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistSpringDamper());
}

//-----------------------------------------------------------------------------
//	Name:		SetTwistSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetTwistSpringTargetAngle(Float32	rAngle)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetTwistSpringTargetAngle(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetTwistSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetTwistSpringTargetAngle()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetTwistSpringTargetAngle());
}

//-----------------------------------------------------------------------------
//	Name:		EnableSwingSpring
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EnableSwingSpring(bool	bState)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->EnableSwingSpring(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsSwingSpringEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
bool	PHYSIC_JOINT_SPHERICAL::IsSwingSpringEnabled()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->IsSwingSpringEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetSwingSpringValue(Float32	rValue)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetSwingSpringValue(rValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetSwingSpringValue()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetSwingSpringValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetSwingSpringDamper(Float32	rDamper)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetSwingSpringDamper(rDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetSwingSpringDamper()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetSwingSpringDamper());
}

//-----------------------------------------------------------------------------
//	Name:		SetSwingSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetSwingSpringTargetAngle(Float32	rAngle)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetSwingSpringTargetAngle(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetSwingSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetSwingSpringTargetAngle()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetSwingSpringTargetAngle());
}

//-----------------------------------------------------------------------------
//	Name:		EnableJointSpring
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EnableJointSpring(bool	bState)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->EnableJointSpring(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsJointSpringEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
bool	PHYSIC_JOINT_SPHERICAL::IsJointSpringEnabled()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->IsJointSpringEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetJointSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetJointSpringValue(Float32	rValue)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetJointSpringValue(rValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetJointSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetJointSpringValue()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetJointSpringValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetJointSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetJointSpringDamper(Float32	rDamper)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetJointSpringDamper(rDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetJointSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetJointSpringDamper()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetJointSpringDamper());
}

//-----------------------------------------------------------------------------
//	Name:		SetJointSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::SetJointSpringTargetAngle(Float32	rAngle)
{
	static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->SetJointSpringTargetAngle(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetJointSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_SPHERICAL::GetJointSpringTargetAngle()
{
	return	(static_cast<I_PHYSIC_JOINT_SPHERICAL *>(_pJoint)->GetJointSpringTargetAngle());
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR * pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableTwistLimit"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bTwistLimit"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_TWIST_LIMIT, pMD, EM_EnableTwistLimit);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsTwistLimitEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bTwistLimit"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_TWIST_LIMIT_ENABLED, pMD, EM_IsTwistLimitEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistLimitHighValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTwistLimitHighValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_LIMIT_HIGH_VALUE, pMD, EM_SetTwistLimitHighValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistLimitHighValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTwistLimitHighValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_LIMIT_HIGH_VALUE, pMD, EM_GetTwistLimitHighValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistLimitHighBounciness"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTwistLimitHighBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_LIMIT_HIGH_BOUNCINESS, pMD, EM_SetTwistLimitHighBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistLimitHighBounciness"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTwistLimitHighBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_LIMIT_HIGH_BOUNCINESS, pMD, EM_GetTwistLimitHighBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistLimitLowValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTwistLimitLowValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_LIMIT_LOW_VALUE, pMD, EM_SetTwistLimitLowValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistLimitHighValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTwistLimitLowValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_LIMIT_LOW_VALUE, pMD, EM_GetTwistLimitLowValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistLimitLowBounciness"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTwistLimitLowBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_LIMIT_LOW_BOUNCINESS, pMD, EM_SetTwistLimitLowBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistLimitLowBounciness"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTwistLimitLowBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_LIMIT_LOW_BOUNCINESS, pMD, EM_GetTwistLimitLowBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableSwingLimit"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bSwingLimit"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_SWING_LIMIT, pMD, EM_EnableSwingLimit);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsSwingLimitEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bSwingLimit"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_SWING_LIMIT_ENABLED, pMD, EM_IsSwingLimitEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSwingLimitValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rSwingLimitValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SWING_LIMIT_VALUE, pMD, EM_SetSwingLimitValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSwingLimitValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rSwingLimitValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SWING_LIMIT_VALUE, pMD, EM_GetSwingLimitValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSwingLimitBounciness"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rSwingLimitBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SWING_LIMIT_BOUNCINESS, pMD, EM_SetSwingLimitBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSwingLimitBounciness"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rSwingLimitBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SWING_LIMIT_BOUNCINESS, pMD, EM_GetSwingLimitBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableTwistSpring"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bTwistSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_TWIST_SPRING, pMD, EM_EnableTwistSpring);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsTwistSpringEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bTwistSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_TWIST_SPRING_ENABLED, pMD, EM_IsTwistSpringEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistSpringValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTwistSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_SPRING_VALUE, pMD, EM_SetTwistSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistSpringValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTwistSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_SPRING_VALUE, pMD, EM_GetTwistSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistSpringDamper"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_SPRING_DAMPER, pMD, EM_SetTwistSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistSpringDamper"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_SPRING_DAMPER, pMD, EM_GetTwistSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetTwistSpringTargetAngle"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_TWIST_SPRING_TARGET_ANGLE, pMD, EM_SetTwistSpringTargetAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetTwistSpringTargetAngle"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_TWIST_SPRING_TARGET_ANGLE, pMD, EM_GetTwistSpringTargetAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableSwingSpring"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bSwingSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_SWING_SPRING, pMD, EM_EnableSwingSpring);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsSwingSpringEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bSwingSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_SWING_SPRING_ENABLED, pMD, EM_IsSwingSpringEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSwingSpringValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rSwingSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SWING_SPRING_VALUE, pMD, EM_SetSwingSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSwingSpringValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rSwingSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SWING_SPRING_VALUE, pMD, EM_GetSwingSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSwingSpringDamper"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SWING_SPRING_DAMPER, pMD, EM_SetSwingSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSwingSpringDamper"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SWING_SPRING_DAMPER, pMD, EM_GetSwingSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSwingSpringTargetAngle"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SWING_SPRING_TARGET_ANGLE, pMD, EM_SetSwingSpringTargetAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSwingSpringTargetAngle"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SWING_SPRING_TARGET_ANGLE, pMD, EM_GetSwingSpringTargetAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableJointSpring"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bJointSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_JOINT_SPRING, pMD, EM_EnableJointSpring);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsJointSpringEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bJointSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_JOINT_SPRING_ENABLED, pMD, EM_IsJointSpringEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetJointSpringValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rJointSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_JOINT_SPRING_VALUE, pMD, EM_SetJointSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetJointSpringValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rJointSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_JOINT_SPRING_VALUE, pMD, EM_GetJointSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetJointSpringDamper"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_JOINT_SPRING_DAMPER, pMD, EM_SetJointSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetJointSpringDamper"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_JOINT_SPRING_DAMPER, pMD, EM_GetJointSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetJointSpringTargetAngle"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_JOINT_SPRING_TARGET_ANGLE, pMD, EM_SetJointSpringTargetAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetJointSpringTargetAngle"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_JOINT_SPRING_TARGET_ANGLE, pMD, EM_GetJointSpringTargetAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PHYSIC_JOINT_SPHERICAL::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_JOINT_SPHERICAL) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PHYSIC_JOINT_SPHERICAL::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PHYSIC_JOINT_SPHERICAL"),								
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_JOINT_SPHERICAL,			
										QDT_COMMENT("EA4EA77E-F78B-E090-49FC-904565C65C2E"),			
										sizeof(PHYSIC_JOINT_SPHERICAL),										
										ComInit,											
										QDT_COMMENT("Class for physic spherical joint. A joint is a constraint applied between two NODE."),
										PHYSIC_JOINT::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_Construct(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	PHYSIC_JOINT_SPHERICAL *pJointSpherical;
	pJointSpherical = static_cast<PHYSIC_JOINT_SPHERICAL *>(pO);

	QDT_NEW_PLACED(pJointSpherical, PHYSIC_JOINT_SPHERICAL)();
	QDT_ASSERT(pJointSpherical);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_Destruct(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	PHYSIC_JOINT_SPHERICAL *pJointSpherical;
	pJointSpherical = static_cast<PHYSIC_JOINT_SPHERICAL *>(pO);

	QDT_DELETE_PLACED(pJointSpherical, PHYSIC_JOINT_SPHERICAL);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableTwistLimit
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_EnableTwistLimit(void * const	pO,
													void * const	pR,
													void * const	pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->EnableTwistLimit(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsTwistLimitEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_IsTwistLimitEnabled(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsTwistLimitEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistLimitHighValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistLimitHighValue(void * const	pO,
														  void * const	pR,
														  void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistLimitHighValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistLimitHighValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistLimitHighValue(void * const	pO,
														  void * const	pR,
														  void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistLimitHighValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistLimitHighBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistLimitHighBounciness(void * const	pO,
															   void * const	pR,
															   void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistLimitHighBounciness(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistLimitHighBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistLimitHighBounciness(void * const	pO,
															   void * const	pR,
															   void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistLimitHighBounciness();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistLimitLowValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistLimitLowValue(void * const	pO,
														 void * const	pR,
														 void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistLimitLowValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistLimitLowValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistLimitLowValue(void * const	pO,
														 void * const	pR,
														 void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistLimitLowValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistLimitLowBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistLimitLowBounciness(void * const	pO,
															  void * const	pR,
															  void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistLimitLowBounciness(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistLimitLowBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistLimitLowBounciness(void * const	pO,
															  void * const	pR,
															  void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistLimitLowBounciness();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSwingLimit
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_EnableSwingLimit(void * const	pO,
													void * const	pR,
													void * const	pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->EnableSwingLimit(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsSwingLimitEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_IsSwingLimitEnabled(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsSwingLimitEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSwingLimitValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetSwingLimitValue(void * const	pO,
													  void * const	pR,
													  void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetSwingLimitValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSwingLimitValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetSwingLimitValue(void * const	pO,
													  void * const	pR,
													  void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSwingLimitValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSwingLimitBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetSwingLimitBounciness(void * const	pO,
														   void * const	pR,
														   void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetSwingLimitBounciness(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSwingLimitBounciness
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetSwingLimitBounciness(void * const	pO,
														   void * const	pR,
														   void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSwingLimitBounciness();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableTwistSpring
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_EnableTwistSpring(void * const	pO,
													 void * const	pR,
													 void * const	pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->EnableTwistSpring(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsTwistSpringEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_IsTwistSpringEnabled(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsTwistSpringEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistSpringValue(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistSpringValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistSpringValue(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistSpringValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistSpringDamper(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistSpringDamper(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistSpringDamper(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistSpringDamper();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetTwistSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetTwistSpringTargetAngle(void * const	pO,
															 void * const	pR,
															 void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetTwistSpringTargetAngle(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetTwistSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetTwistSpringTargetAngle(void * const	pO,
															 void * const	pR,
															 void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetTwistSpringTargetAngle();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSwingSpring
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_EnableSwingSpring(void * const	pO,
													 void * const	pR,
													 void * const	pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->EnableSwingSpring(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsSwingSpringEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_IsSwingSpringEnabled(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsSwingSpringEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSwingSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetSwingSpringValue(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetSwingSpringValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSwingSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetSwingSpringValue(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSwingSpringValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSwingSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetSwingSpringDamper(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetSwingSpringDamper(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSwingSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetSwingSpringDamper(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSwingSpringDamper();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetSwingSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetSwingSpringTargetAngle(void * const	pO,
															 void * const	pR,
															 void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetSwingSpringTargetAngle(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetSwingSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetSwingSpringTargetAngle(void * const	pO,
															 void * const	pR,
															 void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSwingSpringTargetAngle();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableJointSpring
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_EnableJointSpring(void * const	pO,
													 void * const	pR,
													 void * const	pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->EnableJointSpring(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsJointSpringEnabled
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_IsJointSpringEnabled(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsJointSpringEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetJointSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetJointSpringValue(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetJointSpringValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetJointSpringValue
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetJointSpringValue(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetJointSpringValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetJointSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetJointSpringDamper(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetJointSpringDamper(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetJointSpringDamper
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetJointSpringDamper(void * const	pO,
														void * const	pR,
														void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetJointSpringDamper();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetJointSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_SetJointSpringTargetAngle(void * const	pO,
															 void * const	pR,
															 void * const	pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	pJoint->SetJointSpringTargetAngle(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetJointSpringTargetAngle
//	Object:		
//	05-07-07:	ELE - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_SPHERICAL::EM_GetJointSpringTargetAngle(void * const	pO,
															 void * const	pR,
															 void * const	pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_SPHERICAL	*pJoint			= static_cast<PHYSIC_JOINT_SPHERICAL *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetJointSpringTargetAngle();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
