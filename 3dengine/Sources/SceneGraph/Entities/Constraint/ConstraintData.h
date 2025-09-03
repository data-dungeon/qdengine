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
//	The CONSTRAINT_DATA class implements ...
//
//	06-03-13:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef __M3D_SCENEGRAPH_ENTITIES_CONSTRAINT_CONSTRAINTDATA_H__
#define __M3D_SCENEGRAPH_ENTITIES_CONSTRAINT_CONSTRAINTDATA_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KCOM(CommunicatorType/Communicator)
#include	INCL_KPHYSICS(Physics/Enums)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)

namespace QDT
{
	namespace M3D
	{
		class	CONSTRAINT_DATA : public COMMUNICATOR
		{
		public:

			CONSTRAINT_DATA();
			~CONSTRAINT_DATA();

			// Accessors
			_INLINE_ const COM_INSTANCE &		GetActor1CI() const;
			_INLINE_ Int32						GetChildIndex1() const;
			_INLINE_ const COM_INSTANCE	&		GetActor2CI() const;
			_INLINE_ Int32						GetChildIndex2() const;
			_INLINE_ const VECTOR &				GetGlobalAnchor() const;
			_INLINE_ const VECTOR &				GetGlobalAxis() const;
			_INLINE_ bool						IsCollisionEnabled() const;
			_INLINE_ bool						IsProjectionEnabled() const;
			_INLINE_ Float32					GetProjectionDistance() const;
			_INLINE_ Float32					GetProjectionAngle() const;
			_INLINE_ JOINT_CUSTOM_MOTION		GetXMotionType() const;
			_INLINE_ JOINT_CUSTOM_MOTION		GetYMotionType() const;
			_INLINE_ JOINT_CUSTOM_MOTION		GetZMotionType() const;
			_INLINE_ JOINT_CUSTOM_MOTION		GetSwing1MotionType() const;
			_INLINE_ JOINT_CUSTOM_MOTION		GetSwing2MotionType() const;
			_INLINE_ JOINT_CUSTOM_MOTION		GetTwistMotionType() const;
			_INLINE_ Float32					GetLinearLimitValue() const;
			_INLINE_ Float32					GetLinearLimitRestitution() const;
			_INLINE_ Float32					GetLinearLimitSpring() const;
			_INLINE_ Float32					GetLinearLimitDamper() const;
			_INLINE_ Float32					GetSwing1LimitValue() const;
			_INLINE_ Float32					GetSwing1LimitRestitution() const;
			_INLINE_ Float32					GetSwing1LimitSpring() const;
			_INLINE_ Float32					GetSwing1LimitDamper() const;
			_INLINE_ Float32					GetSwing2LimitValue() const;
			_INLINE_ Float32					GetSwing2LimitRestitution() const;
			_INLINE_ Float32					GetSwing2LimitSpring() const;
			_INLINE_ Float32					GetSwing2LimitDamper() const;
			_INLINE_ Float32					GetTwistLowLimitValue() const;
			_INLINE_ Float32					GetTwistLowLimitRestitution() const;
			_INLINE_ Float32					GetTwistLowLimitSpring() const;
			_INLINE_ Float32					GetTwistLowLimitDamper() const;
			_INLINE_ Float32					GetTwistHighLimitValue() const;
			_INLINE_ Float32					GetTwistHighLimitRestitution() const;
			_INLINE_ Float32					GetTwistHighLimitSpring() const;
			_INLINE_ Float32					GetTwistHighLimitDamper() const;
			_INLINE_ JOINT_CUSTOM_DRIVE_TYPE	GetSwingDriveType() const;
			_INLINE_ Float32					GetSwingDriveSpring() const;
			_INLINE_ Float32					GetSwingDriveDamper() const;
			_INLINE_ const QUATERNION &			GetSwingDriveOrientation() const;
			_INLINE_ const VECTOR &				GetSwingDriveAngularVelocity() const;

			// Modifiors
			_INLINE_ void	SetActor1CI(const COM_INSTANCE & Actor1CI);
			_INLINE_ void	SetChildIndex1(Int32 iChildIndex1);
			_INLINE_ void	SetActor2CI(const COM_INSTANCE & Actor2CI);
			_INLINE_ void	SetChildIndex2(Int32 iChildIndex2);
			_INLINE_ void	SetGlobalAnchor(const VECTOR & GlobalAnchor);
			_INLINE_ void	SetGlobalAxis(const VECTOR & GlobalAxis);
			_INLINE_ void	EnableCollision(bool bCollision);
			_INLINE_ void	EnableProjection(bool bProjection);
			_INLINE_ void	SetProjectionDistance(Float32 rProjectionDistance);
			_INLINE_ void	SetProjectionAngle(Float32 rProjectionAngle);
			_INLINE_ void	SetXMotionType(JOINT_CUSTOM_MOTION XMotionType);
			_INLINE_ void	SetYMotionType(JOINT_CUSTOM_MOTION YMotionType);
			_INLINE_ void	SetZMotionType(JOINT_CUSTOM_MOTION ZMotionType);
			_INLINE_ void	SetSwing1MotionType(JOINT_CUSTOM_MOTION Swing1MotionType);
			_INLINE_ void	SetSwing2MotionType(JOINT_CUSTOM_MOTION Swing2MotionType);
			_INLINE_ void	SetTwistMotionType(JOINT_CUSTOM_MOTION TwistMotionType);
			_INLINE_ void	SetLinearLimitValue(Float32	rLinearLimitValue);
			_INLINE_ void	SetLinearLimitRestitution(Float32 rLinearLimitRestitution);
			_INLINE_ void	SetLinearLimitSpring(Float32 rLinearLimitSpring);
			_INLINE_ void	SetLinearLimitDamper(Float32 rLinearLimitDamper);
			_INLINE_ void	SetSwing1LimitValue(Float32 rSwing1LimitValue);
			_INLINE_ void	SetSwing1LimitRestitution(Float32 rSwing1LimitRestitution);
			_INLINE_ void	SetSwing1LimitSpring(Float32 rSwing1LimitSpring);
			_INLINE_ void	SetSwing1LimitDamper(Float32 rSwing1LimitDamper);
			_INLINE_ void	SetSwing2LimitValue(Float32 rSwing2LimitValue);
			_INLINE_ void	SetSwing2LimitRestitution(Float32 rSwing2LimitRestitution);
			_INLINE_ void	SetSwing2LimitSpring(Float32 rSwing2LimitSpring);
			_INLINE_ void	SetSwing2LimitDamper(Float32 rSwing2LimitDamper);
			_INLINE_ void	SetTwistLowLimitValue(Float32 rTwistLowLimitValue);
			_INLINE_ void	SetTwistLowLimitRestitution(Float32 rTwistLowLimitRestitution);
			_INLINE_ void	SetTwistLowLimitSpring(Float32 rTwistLowLimitSpring);
			_INLINE_ void	SetTwistLowLimitDamper(Float32 rTwistLowLimitDamper);
			_INLINE_ void	SetTwistHighLimitValue(Float32 rTwistHighLimitValue);
			_INLINE_ void	SetTwistHighLimitRestitution(Float32 rTwistHighLimitRestitution);
			_INLINE_ void	SetTwistHighLimitSpring(Float32 rTwistHighLimitSpring);
			_INLINE_ void	SetTwistHighLimitDamper(Float32 rTwistHighLimitDamper);
			_INLINE_ void	SetSwingDriveType(JOINT_CUSTOM_DRIVE_TYPE SwingDriveType);
			_INLINE_ void	SetSwingDriveSpring(Float32 rSwingDriveSpring);
			_INLINE_ void	SetSwingDriveDamper(Float32 rSwingDriveDamper);
			_INLINE_ void	SetSwingDriveOrientation(const QUATERNION & SwingDriveOrientation);
			_INLINE_ void	SetSwingDriveAngularVelocity(const VECTOR & SwingDriveAngularVelocity);

			// Communication
			static void							ComInit(CLASS_DESCRIPTOR * const pCD);
			static COMMUNICATOR_DESCRIPTOR *	GetGenericCommunicatorDescriptor();
			static COMMUNICATOR_DESCRIPTOR *	CreateGenericCommunicatorDescriptor();

		private:

			// Communication
			static void		EM_Construct(void * const pO, void * const pR, void * const pP);
			static void		EM_Destruct(void * const pO, void * const pR, void * const pP);

			// Chunks
			static void		CM_ReadChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);
			static void		CM_WriteChunk1(IO_STREAM & Stream, COMMUNICATOR * pCommunicator);

			COM_INSTANCE	_Actor1CI;
			Int32			_iChildIndex1; // In case of _Actor1CI has a NODE_ARRAY
			COM_INSTANCE	_Actor2CI;
			Int32			_iChildIndex2; // In case of _Actor2CI has a NODE_ARRAY

			VECTOR	_GlobalAnchor;
			VECTOR	_GlobalAxis;

			bool	_bCollision;
			bool	_bProjection;
			Float32	_rProjectionDistance;
			Float32	_rProjectionAngle;

			JOINT_CUSTOM_MOTION	_XMotionType;
			JOINT_CUSTOM_MOTION	_YMotionType;
			JOINT_CUSTOM_MOTION	_ZMotionType;
			JOINT_CUSTOM_MOTION	_Swing1MotionType;
			JOINT_CUSTOM_MOTION	_Swing2MotionType;
			JOINT_CUSTOM_MOTION	_TwistMotionType;

			Float32	_rLinearLimitValue;
			Float32	_rLinearLimitRestitution;
			Float32	_rLinearLimitSpring;
			Float32	_rLinearLimitDamper;

			Float32	_rSwing1LimitValue;
			Float32	_rSwing1LimitRestitution;
			Float32	_rSwing1LimitSpring;
			Float32	_rSwing1LimitDamper;

			Float32	_rSwing2LimitValue;
			Float32	_rSwing2LimitRestitution;
			Float32	_rSwing2LimitSpring;
			Float32	_rSwing2LimitDamper;

			Float32	_rTwistLowLimitValue;
			Float32	_rTwistLowLimitRestitution;
			Float32	_rTwistLowLimitSpring;
			Float32	_rTwistLowLimitDamper;
			Float32	_rTwistHighLimitValue;
			Float32	_rTwistHighLimitRestitution;
			Float32	_rTwistHighLimitSpring;
			Float32	_rTwistHighLimitDamper;

			JOINT_CUSTOM_DRIVE_TYPE	_SwingDriveType;
			Float32					_rSwingDriveSpring;
			Float32					_rSwingDriveDamper;
			QUATERNION				_SwingDriveOrientation;
			VECTOR					_SwingDriveAngularVelocity;
		};
	}
}

#ifndef _DEBUG
	#include "ConstraintData.inl"
#endif

#endif // __M3D_SCENEGRAPH_ENTITIES_CONSTRAINT_CONSTRAINTDATA_H__

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
