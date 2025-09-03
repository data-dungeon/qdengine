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
//	The PHYSIC_JOINT_SPHERICAL class implements ...
//
//	05-07-07:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_JOINTS_PHYSICJOINTSPHERICAL_H__
#define __M3D_SCENEGRAPH_JOINTS_PHYSICJOINTSPHERICAL_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(SceneGraph/Joints/PhysicJoint)

namespace QDT
{
	namespace M3D
	{
		class	PHYSIC_JOINT_SPHERICAL : public PHYSIC_JOINT
		{
		public:

			PHYSIC_JOINT_SPHERICAL();
			virtual	~PHYSIC_JOINT_SPHERICAL();

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

			// Limits
			void		EnableTwistLimit(bool bState);
			bool		IsTwistLimitEnabled();
			void		SetTwistLimitHighValue(Float32 rHighValue);
			Float32		GetTwistLimitHighValue();
			void		SetTwistLimitHighBounciness(Float32 rHighBounciness);
			Float32		GetTwistLimitHighBounciness();
			void		SetTwistLimitLowValue(Float32 rLowValue);
			Float32		GetTwistLimitLowValue();
			void		SetTwistLimitLowBounciness(Float32 rLowBounciness);
			Float32		GetTwistLimitLowBounciness();
			void		EnableSwingLimit(bool bState);
			bool		IsSwingLimitEnabled();
			void		SetSwingLimitValue(Float32 rValue);
			Float32		GetSwingLimitValue();
			void		SetSwingLimitBounciness(Float32 rBounciness);
			Float32		GetSwingLimitBounciness();

			// Springs
			void		EnableTwistSpring(bool bState);
			bool		IsTwistSpringEnabled();
			void		SetTwistSpringValue(Float32 rValue);
			Float32		GetTwistSpringValue();
			void		SetTwistSpringDamper(Float32 rDamper);
			Float32		GetTwistSpringDamper();
			void		SetTwistSpringTargetAngle(Float32 rAngle);
			Float32		GetTwistSpringTargetAngle();
			void		EnableSwingSpring(bool bState);
			bool		IsSwingSpringEnabled();
			void		SetSwingSpringValue(Float32 rValue);
			Float32		GetSwingSpringValue();
			void		SetSwingSpringDamper(Float32 rDamper);
			Float32		GetSwingSpringDamper();
			void		SetSwingSpringTargetAngle(Float32 rAngle);
			Float32		GetSwingSpringTargetAngle();
			void		EnableJointSpring(bool bState);
			bool		IsJointSpringEnabled();
			void		SetJointSpringValue(Float32 rValue);
			Float32		GetJointSpringValue();
			void		SetJointSpringDamper(Float32 rDamper);
			Float32		GetJointSpringDamper();
			void		SetJointSpringTargetAngle(Float32 rAngle);
			Float32		GetJointSpringTargetAngle();

		private:

			enum
			{
				CMD_ENABLE_TWIST_LIMIT = CMD_PHYSIC_JOINT_LAST,
				CMD_IS_TWIST_LIMIT_ENABLED,
				CMD_SET_TWIST_LIMIT_HIGH_VALUE,
				CMD_GET_TWIST_LIMIT_HIGH_VALUE,
				CMD_SET_TWIST_LIMIT_HIGH_BOUNCINESS,
				CMD_GET_TWIST_LIMIT_HIGH_BOUNCINESS,
				CMD_SET_TWIST_LIMIT_LOW_VALUE,
				CMD_GET_TWIST_LIMIT_LOW_VALUE,
				CMD_SET_TWIST_LIMIT_LOW_BOUNCINESS,
				CMD_GET_TWIST_LIMIT_LOW_BOUNCINESS,
				CMD_ENABLE_SWING_LIMIT,
				CMD_IS_SWING_LIMIT_ENABLED,
				CMD_SET_SWING_LIMIT_VALUE,
				CMD_GET_SWING_LIMIT_VALUE,
				CMD_SET_SWING_LIMIT_BOUNCINESS,
				CMD_GET_SWING_LIMIT_BOUNCINESS,
				CMD_ENABLE_TWIST_SPRING,
				CMD_IS_TWIST_SPRING_ENABLED,
				CMD_SET_TWIST_SPRING_VALUE,
				CMD_GET_TWIST_SPRING_VALUE,
				CMD_SET_TWIST_SPRING_DAMPER,
				CMD_GET_TWIST_SPRING_DAMPER,
				CMD_SET_TWIST_SPRING_TARGET_ANGLE,
				CMD_GET_TWIST_SPRING_TARGET_ANGLE,
				CMD_ENABLE_SWING_SPRING,
				CMD_IS_SWING_SPRING_ENABLED,
				CMD_SET_SWING_SPRING_VALUE,
				CMD_GET_SWING_SPRING_VALUE,
				CMD_SET_SWING_SPRING_DAMPER,
				CMD_GET_SWING_SPRING_DAMPER,
				CMD_SET_SWING_SPRING_TARGET_ANGLE,
				CMD_GET_SWING_SPRING_TARGET_ANGLE,
				CMD_ENABLE_JOINT_SPRING,
				CMD_IS_JOINT_SPRING_ENABLED,
				CMD_SET_JOINT_SPRING_VALUE,
				CMD_GET_JOINT_SPRING_VALUE,
				CMD_SET_JOINT_SPRING_DAMPER,
				CMD_GET_JOINT_SPRING_DAMPER,
				CMD_SET_JOINT_SPRING_TARGET_ANGLE,
				CMD_GET_JOINT_SPRING_TARGET_ANGLE,
				CMD_PHYSIC_JOINT_REVOLUTE_LAST,
			};

			//	Commands
			static	void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void		EM_EnableTwistLimit(void * const pO, void * const pR, void * const pP);
			static	void		EM_IsTwistLimitEnabled(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetTwistLimitHighValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetTwistLimitHighValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetTwistLimitHighBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetTwistLimitHighBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetTwistLimitLowValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetTwistLimitLowValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetTwistLimitLowBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetTwistLimitLowBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_EnableSwingLimit(void * const pO, void * const pR, void * const pP);
			static	void		EM_IsSwingLimitEnabled(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetSwingLimitValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetSwingLimitValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetSwingLimitBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetSwingLimitBounciness(void * const pO, void * const pR, void * const pP);
			static  void		EM_EnableTwistSpring(void * const pO, void * const pR, void * const pP);
			static  void		EM_IsTwistSpringEnabled(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetTwistSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetTwistSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetTwistSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetTwistSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetTwistSpringTargetAngle(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetTwistSpringTargetAngle(void * const pO, void * const pR, void * const pP);
			static  void		EM_EnableSwingSpring(void * const pO, void * const pR, void * const pP);
			static  void		EM_IsSwingSpringEnabled(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetSwingSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetSwingSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetSwingSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetSwingSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetSwingSpringTargetAngle(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetSwingSpringTargetAngle(void * const pO, void * const pR, void * const pP);
			static  void		EM_EnableJointSpring(void * const pO, void * const pR, void * const pP);
			static  void		EM_IsJointSpringEnabled(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetJointSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetJointSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetJointSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetJointSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetJointSpringTargetAngle(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetJointSpringTargetAngle(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_SCENEGRAPH_JOINTS_PHYSICJOINTSPHERICAL_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
