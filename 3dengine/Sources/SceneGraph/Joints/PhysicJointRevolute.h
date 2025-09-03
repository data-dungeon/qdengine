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
//	CLASS:	PHYSIC_JOINT_REVOLUTE
//	The PHYSIC_JOINT_REVOLUTE class implements ...
//
//	05-02-02:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_JOINTS_PHYSICJOINTREVOLUTE_H__
#define __M3D_SCENEGRAPH_JOINTS_PHYSICJOINTREVOLUTE_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include INCL_3DENGINE(SceneGraph/Joints/PhysicJoint)

namespace QDT
{
	namespace M3D
	{
		class PHYSIC_JOINT_REVOLUTE : public PHYSIC_JOINT
		{
		public:

			PHYSIC_JOINT_REVOLUTE();
			virtual ~PHYSIC_JOINT_REVOLUTE();

			// Setup joint limits
			void		EnableLimit(bool bState);
			bool		IsLimitEnabled() const;
			void		SetLimitHighValue(Float32 rHighValue);
			Float32		GetLimitHighValue() const;
			void		SetLimitHighBounciness(Float32 rHighBounciness);
			Float32		GetLimitHighBounciness() const;
			void		SetLimitLowValue(Float32 rLowValue);
			Float32		GetLimitLowValue();
			void		SetLimitLowBounciness(Float32 rLowBounciness);
			Float32		GetLimitLowBounciness() const;

			// Setup spring
			void		EnableSpring(bool bState);
			bool		IsSpringEnabled() const;
			void		SetSpringValue(Float32 rValue);
			Float32		GetSpringValue() const;
			void		SetSpringDamper(Float32 rDamper);
			Float32		GetSpringDamper() const;
			void		SetSpringTargetAngle(Float32 rAngle);
			Float32		GetSpringTargetAngle() const;

			// Communication
			static void						ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR* GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR* CreateGenericCommunicatorDescriptor();

		private:

			enum
			{
				CMD_ENABLE_LIMIT = CMD_PHYSIC_JOINT_LAST,
				CMD_IS_LIMIT_ENABLED,
				CMD_SET_LIMIT_HIGH_VALUE,
				CMD_GET_LIMIT_HIGH_VALUE,
				CMD_SET_LIMIT_HIGH_BOUNCINESS,
				CMD_GET_LIMIT_HIGH_BOUNCINESS,
				CMD_SET_LIMIT_LOW_VALUE,
				CMD_GET_LIMIT_LOW_VALUE,
				CMD_SET_LIMIT_LOW_BOUNCINESS,
				CMD_GET_LIMIT_LOW_BOUNCINESS,
				CMD_ENABLE_SPRING,
				CMD_IS_SPRING_ENABLED,
				CMD_SET_SPRING_VALUE,
				CMD_GET_SPRING_VALUE,
				CMD_SET_SPRING_DAMPER,
				CMD_GET_SPRING_DAMPER,
				CMD_SET_SPRING_TARGET_ANGLE,
				CMD_GET_SPRING_TARGET_ANGLE,
				CMD_PHYSIC_JOINT_REVOLUTE_LAST,
			};

			//	Commands
			static	void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static	void		EM_Destruct(void * const pO, void * const pR, void * const pP);
			static	void		EM_EnableLimit(void * const pO, void * const pR, void * const pP);
			static	void		EM_IsLimitEnabled(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetLimitHighValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetLimitHighValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetLimitHighBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetLimitHighBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetLimitLowValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetLimitLowValue(void * const pO, void * const pR, void * const pP);
			static	void		EM_SetLimitLowBounciness(void * const pO, void * const pR, void * const pP);
			static	void		EM_GetLimitLowBounciness(void * const pO, void * const pR, void * const pP);
			static  void		EM_EnableSpring(void * const pO, void * const pR, void * const pP);
			static  void		EM_IsSpringEnabled(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetSpringValue(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetSpringDamper(void * const pO, void * const pR, void * const pP);
			static  void		EM_SetSpringTargetAngle(void * const pO, void * const pR, void * const pP);
			static  void		EM_GetSpringTargetAngle(void * const pO, void * const pR, void * const pP);
		};
	}
}

#endif // __M3D_SCENEGRAPH_JOINTS_PHYSICJOINTREVOLUTE_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
