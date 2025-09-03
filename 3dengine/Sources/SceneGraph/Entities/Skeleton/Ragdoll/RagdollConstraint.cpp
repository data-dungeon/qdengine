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

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Skeleton/Ragdoll/RagdollConstraint)

#ifdef _DEBUG
	#include	"RagdollConstraint.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RAGDOLL_CONSTRAINT constructor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
RAGDOLL_CONSTRAINT::RAGDOLL_CONSTRAINT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAGDOLL_CONSTRAINT destructor
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
RAGDOLL_CONSTRAINT::~RAGDOLL_CONSTRAINT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ReadChunk
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::ReadChunk(IO_STREAM &	Stream)
{
	int	n;

	Stream >> _uiActor1Index;
	Stream >> _uiActor2Index;
	Stream >> _GlobalAnchor;
	Stream >> _GlobalAxis;
	Stream >> _GlobalNormal;
	Stream >> _bCollision;
	Stream >> _bProjection;
	Stream >> _rProjectionDistance;
	Stream >> _rProjectionAngle;
	Stream >> n;
	_XMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	_YMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	_ZMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	_Swing1MotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	_Swing2MotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	_TwistMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> _rLinearLimitValue;
	Stream >> _rLinearLimitRestitution;
	Stream >> _rLinearLimitSpring;
	Stream >> _rLinearLimitDamper;
	Stream >> _rSwing1LimitValue;
	Stream >> _rSwing1LimitRestitution;
	Stream >> _rSwing1LimitSpring;
	Stream >> _rSwing1LimitDamper;
	Stream >> _rSwing2LimitValue;
	Stream >> _rSwing2LimitRestitution;
	Stream >> _rSwing2LimitSpring;
	Stream >> _rSwing2LimitDamper;
	Stream >> _rTwistLowLimitValue;
	Stream >> _rTwistLowLimitRestitution;
	Stream >> _rTwistLowLimitSpring;
	Stream >> _rTwistLowLimitDamper;
	Stream >> _rTwistHighLimitValue;
	Stream >> _rTwistHighLimitRestitution;
	Stream >> _rTwistHighLimitSpring;
	Stream >> _rTwistHighLimitDamper;
	Stream >> n;
	_SwingDriveType = static_cast<JOINT_CUSTOM_DRIVE_TYPE>(n);
	Stream >> _rSwingDriveSpring;
	Stream >> _rSwingDriveDamper;
	Stream >> _SwingDriveOrientation;
	Stream >> _SwingDriveAngularVelocity;
}

//-----------------------------------------------------------------------------
//	Name:		WriteChunk
//	Object:		
//	06-03-20:	ELE - Created
//-----------------------------------------------------------------------------
void	RAGDOLL_CONSTRAINT::WriteChunk(IO_STREAM &	Stream)
{
	Stream.DebugOutput("Actor 1 index :");
	Stream << _uiActor1Index;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Actor 2 index :");
	Stream << _uiActor2Index;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Global anchor :");
	Stream << _GlobalAnchor;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Global axis :");
	Stream << _GlobalAxis;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Global normal :");
	Stream << _GlobalNormal;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Collisions :");
	Stream << _bCollision;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Projection :");
	Stream << _bProjection;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Projection distance :");
	Stream << _rProjectionDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Projection angle :");
	Stream << _rProjectionAngle;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type X :");
	Stream << _XMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type Y :");
	Stream << _YMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type Z :");
	Stream << _ZMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type swing 1 :");
	Stream << _Swing1MotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type swing 2 :");
	Stream << _Swing2MotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type twist :");
	Stream << _TwistMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit value :");
	Stream << _rLinearLimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit restitution :");
	Stream << _rLinearLimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit spring :");
	Stream << _rLinearLimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit damper :");
	Stream << _rLinearLimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit value :");
	Stream << _rSwing1LimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit restitution :");
	Stream << _rSwing1LimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit spring :");
	Stream << _rSwing1LimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit damper :");
	Stream << _rSwing1LimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit value :");
	Stream << _rSwing2LimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit restitution :");
	Stream << _rSwing2LimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit spring :");
	Stream << _rSwing2LimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit damper :");
	Stream << _rSwing2LimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist low limit value :");
	Stream << _rTwistLowLimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist lox limit restitution :");
	Stream << _rTwistLowLimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist low limit spring :");
	Stream << _rTwistLowLimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist low limit damper :");
	Stream << _rTwistLowLimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist high limit value :");
	Stream << _rTwistHighLimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist lox limit restitution :");
	Stream << _rTwistHighLimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist high limit spring :");
	Stream << _rTwistHighLimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist high limit damper :");
	Stream << _rTwistHighLimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive type :");
	Stream << _SwingDriveType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive spring :");
	Stream << _rSwingDriveSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive damper :");
	Stream << _rSwingDriveDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive target orientation :");
	Stream << _SwingDriveOrientation;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive target  velocity :");
	Stream << _SwingDriveAngularVelocity;
	Stream.DebugOutput("\r\n");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
