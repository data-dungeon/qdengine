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
//	CLASS:	PHYSIC_JOINT
//
//	05-02-02:	CVI - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Joints/PhysicJointRevolute)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KPHYSICS(Physics/PhysicContext)
#include	INCL_KPHYSICS(Physics/IPhysicJointRevolute)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeFloat)
#include	INCL_KCOM(BaseType/TypeBool)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT_REVOLUTE Destructor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT_REVOLUTE::PHYSIC_JOINT_REVOLUTE()
:
PHYSIC_JOINT(CreateGenericCommunicatorDescriptor())
{
	_pJoint = PHYSIC_CONTEXT::Instance()->CreateJointRevolute();
}	

//-----------------------------------------------------------------------------
//	Name:		PHYSIC_JOINT_REVOLUTE Destructor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
PHYSIC_JOINT_REVOLUTE::~PHYSIC_JOINT_REVOLUTE()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		EnableLimit
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EnableLimit(bool bState)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->EnableLimit(bState);
}

//-----------------------------------------------------------------------------
//	Name:		IsLimitEnabled
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
bool		PHYSIC_JOINT_REVOLUTE::IsLimitEnabled() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->IsLimitEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		SetLimitHighValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetLimitHighValue(Float32 rHighValue)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetLimitHighValue(rHighValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitHighValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetLimitHighValue() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetLimitHighValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetLimitHighBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetLimitHighBounciness(Float32 rHighBounciness)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetLimitHighBounciness(rHighBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitHighBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetLimitHighBounciness() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetLimitHighBounciness());
}

//-----------------------------------------------------------------------------
//	Name:		SetLimitLowValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetLimitLowValue(Float32 rLowValue)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetLimitLowValue(rLowValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetLimitLowValue()
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetLimitLowValue());
}

//-----------------------------------------------------------------------------
//	Name:		SetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetLimitLowBounciness(Float32 rLowBounciness)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetLimitLowBounciness(rLowBounciness);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetLimitLowBounciness() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetLimitLowBounciness());
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EnableSpring(bool bState)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->EnableSpring(bState);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
bool		PHYSIC_JOINT_REVOLUTE::IsSpringEnabled() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->IsSpringEnabled());
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetSpringValue(Float32 rValue)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetSpringValue(rValue);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetSpringValue() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetSpringValue());
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetSpringDamper(Float32 rDamper)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetSpringDamper(rDamper);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetSpringDamper() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetSpringDamper());
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::SetSpringTargetAngle(Float32 rAngle)
{
	static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->SetSpringTargetAngle(rAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
Float32	PHYSIC_JOINT_REVOLUTE::GetSpringTargetAngle() const
{
	return (static_cast<I_PHYSIC_JOINT_REVOLUTE *>(_pJoint)->GetSpringTargetAngle());
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void	PHYSIC_JOINT_REVOLUTE::ComInit(CLASS_DESCRIPTOR * const pCD)
{
	METHOD_DESCRIPTOR * pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableLimit"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bLimit"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_LIMIT, pMD, EM_EnableLimit);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsLimitEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bLimit"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_LIMIT_ENABLED, pMD, EM_IsLimitEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetLimitHighValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rLimitHighValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_LIMIT_HIGH_VALUE, pMD, EM_SetLimitHighValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetLimitHighValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rLimitHighValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_LIMIT_HIGH_VALUE, pMD, EM_GetLimitHighValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetLimitHighBounciness"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rLimitHighBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_LIMIT_HIGH_BOUNCINESS, pMD, EM_SetLimitHighBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetLimitHighBounciness"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rLimitHighBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_LIMIT_HIGH_BOUNCINESS, pMD, EM_GetLimitHighBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetLimitLowValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rLimitLowValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_LIMIT_LOW_VALUE, pMD, EM_SetLimitLowValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetLimitHighValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rLimitLowValue"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_LIMIT_LOW_VALUE, pMD, EM_GetLimitLowValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetLimitLowBounciness"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rLimitLowBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_LIMIT_LOW_BOUNCINESS, pMD, EM_SetLimitLowBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetLimitLowBounciness"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rLimitLowBounciness"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_LIMIT_LOW_BOUNCINESS, pMD, EM_GetLimitLowBounciness);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableSpring"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("bSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_SPRING, pMD, EM_EnableSpring);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsSpringEnabled"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("bSpring"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_SPRING_ENABLED, pMD, EM_IsSpringEnabled);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSpringValue"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SPRING_VALUE, pMD, EM_SetSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSpringValue"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rSpring"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SPRING_VALUE, pMD, EM_GetSpringValue);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSpringDamper"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SPRING_DAMPER, pMD, EM_SetSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSpringDamper"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rDamper"), QDT_COMMENT(""), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SPRING_DAMPER, pMD, EM_GetSpringDamper);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("SetSpringTargetAngle"), QDT_COMMENT(""));
	pMD->AddParameter(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_SPRING_TARGET_ANGLE, pMD, EM_SetSpringTargetAngle);

	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("GetSpringTargetAngle"), QDT_COMMENT(""));
	pMD->SetReturn(QDT_COMMENT("rTargetAngle"), QDT_COMMENT("In radians"), TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_GET_SPRING_TARGET_ANGLE, pMD, EM_GetSpringTargetAngle);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR* PHYSIC_JOINT_REVOLUTE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_JOINT_REVOLUTE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	PHYSIC_JOINT_REVOLUTE::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("PHYSIC_JOINT_REVOLUTE"),								
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_PHYSIC_JOINT_REVOLUTE,			
										QDT_COMMENT("30E61FF8-EB5D-505B-38DC-C8E8CC29E626"),			
										sizeof(PHYSIC_JOINT_REVOLUTE),										
										ComInit,											
										QDT_COMMENT("Class for physic revolute joint. A joint is a constraint applied between two NODE."),
										PHYSIC_JOINT::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_Construct(void * const pO, 
												void * const pR, 
												void * const pP)
{
	PHYSIC_JOINT_REVOLUTE *pJointRevolute;
	pJointRevolute = static_cast<PHYSIC_JOINT_REVOLUTE *>(pO);

	QDT_NEW_PLACED(pJointRevolute, PHYSIC_JOINT_REVOLUTE)();
	QDT_ASSERT(pJointRevolute);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	05-02-02:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_Destruct(void * const pO, 
											   void * const pR, 
											   void * const pP)
{
	PHYSIC_JOINT_REVOLUTE *pJointRevolute;
	pJointRevolute = static_cast<PHYSIC_JOINT_REVOLUTE *>(pO);

	QDT_DELETE_PLACED(pJointRevolute, PHYSIC_JOINT_REVOLUTE);
	QDT_ASSERT(pJointRevolute);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableLimit
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_EnableLimit(void * const pO, 
												  void * const pR, 
												  void * const pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->EnableLimit(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsLimitEnabled
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_IsLimitEnabled(void * const pO, 
													 void * const pR, 
													 void * const pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsLimitEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLimitHighValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetLimitHighValue(void * const pO, 
														void * const pR, 
														void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetLimitHighValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetLimitHighValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetLimitHighValue(void * const pO, 
														void * const pR, 
														void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetLimitHighValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLimitHighBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetLimitHighBounciness(void * const pO, 
															 void * const pR, 
															 void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetLimitHighBounciness(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetLimitHighBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetLimitHighBounciness(void * const pO, 
															 void * const pR, 
															 void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetLimitHighBounciness();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLimitLowValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetLimitLowValue(void * const pO, 
													   void * const pR, 
													   void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetLimitLowValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetLimitLowValue
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetLimitLowValue(void * const pO, 
													   void * const pR, 
													   void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetLimitLowValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetLimitLowBounciness(void * const pO, 
															void * const pR, 
															void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetLimitLowBounciness(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetLimitLowBounciness
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetLimitLowBounciness(void * const pO, 
															void * const pR, 
															void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetLimitLowBounciness();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_EnableSpring(void * const pO, 
												   void * const pR, 
												   void * const pP)
{
	bool	 				*pParameter		= static_cast<bool *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->EnableSpring(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_IsSpringEnabled(void * const pO, 
													  void * const pR, 
													  void * const pP)
{
	bool	 				*pReturn		= static_cast<bool *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->IsSpringEnabled();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetSpringValue(void * const pO, 
													 void * const pR, 
													 void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetSpringValue(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetSpringValue(void * const pO, 
													 void * const pR, 
													 void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSpringValue();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetSpringDamper(void * const pO, 
													  void * const pR, 
													  void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetSpringDamper(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetSpringDamper(void * const pO, 
													  void * const pR, 
													  void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSpringDamper();
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_SetSpringTargetAngle(void * const pO, 
														   void * const pR, 
														   void * const pP)
{
	float	 				*pParameter		= static_cast<float *>(pP);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	pJoint->SetSpringTargetAngle(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_EnableSpring
//	Object:		
//	05-02-04:	RMA - Created
//-----------------------------------------------------------------------------
void		PHYSIC_JOINT_REVOLUTE::EM_GetSpringTargetAngle(void * const pO, 
														   void * const pR, 
														   void * const pP)
{
	float	 				*pReturn		= static_cast<float *>(pR);
	COM_INSTANCE			*pComInstance	= static_cast<COM_INSTANCE *>(pO);
	PHYSIC_JOINT_REVOLUTE	*pJoint			= static_cast<PHYSIC_JOINT_REVOLUTE *>(pComInstance->GetCommunicator());

	*pReturn = pJoint->GetSpringTargetAngle();
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
