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

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/Constraint/ConstraintData)
#include	INCL_3DENGINE(M3D/Moteur3DModule)

#ifdef _DEBUG
	#include	"ConstraintData.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		CONSTRAINT_DATA constructor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
CONSTRAINT_DATA::CONSTRAINT_DATA()
:
COMMUNICATOR				(GetGenericCommunicatorDescriptor()),
_Actor1CI					(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_iChildIndex1				(-1),
_Actor2CI					(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_iChildIndex2				(-1),
_GlobalAnchor				(0.0f, 0.0f, 0.0f),
_GlobalAxis					(1.0f, 0.0f, 0.0f),
_bCollision					(false),
_bProjection				(true),
_rProjectionDistance		(0.1f),
_rProjectionAngle			(0.1f),
_XMotionType				(JOINT_CUSTOM_MOTION_LOCKED),
_YMotionType				(JOINT_CUSTOM_MOTION_LOCKED),
_ZMotionType				(JOINT_CUSTOM_MOTION_LOCKED),
_Swing1MotionType			(JOINT_CUSTOM_MOTION_LOCKED),
_Swing2MotionType			(JOINT_CUSTOM_MOTION_LOCKED),
_TwistMotionType			(JOINT_CUSTOM_MOTION_LOCKED),
_rLinearLimitValue			(0.0f),
_rLinearLimitRestitution	(0.0f),
_rLinearLimitSpring			(0.0f),
_rLinearLimitDamper			(0.0f),
_rSwing1LimitValue			(0.0f),
_rSwing1LimitRestitution	(0.0f),
_rSwing1LimitSpring			(0.0f),
_rSwing1LimitDamper			(0.0f),
_rSwing2LimitValue			(0.0f),
_rSwing2LimitRestitution	(0.0f),
_rSwing2LimitSpring			(0.0f),
_rSwing2LimitDamper			(0.0f),
_rTwistLowLimitValue		(0.0f),
_rTwistLowLimitRestitution	(0.0f),
_rTwistLowLimitSpring		(0.0f),
_rTwistLowLimitDamper		(0.0f),
_rTwistHighLimitValue		(0.0f),
_rTwistHighLimitRestitution	(0.0f),
_rTwistHighLimitSpring		(0.0f),
_rTwistHighLimitDamper		(0.0f),
_SwingDriveType				(JOINT_CUSTOM_DRIVE_NONE),
_rSwingDriveSpring			(0.0f),
_rSwingDriveDamper			(0.0f),
_SwingDriveOrientation		(1.0f, 0.0f, 0.0f, 0.0f),
_SwingDriveAngularVelocity	(0.0f, 0.0f, 0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		CONSTRAINT_DATA destructor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
CONSTRAINT_DATA::~CONSTRAINT_DATA()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		ComInit
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::ComInit(CLASS_DESCRIPTOR * const	pCD)
{
	pCD->SetConstructors(EM_Construct, EM_Destruct);

	// Chunk
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddReadChunk("CONSTRAI", 1, CM_ReadChunk1);
	static_cast<COMMUNICATOR_DESCRIPTOR *> (pCD)->AddWriteChunk("CONSTRAI", 1, CM_WriteChunk1);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CONSTRAINT_DATA::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT_DATA) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	CONSTRAINT_DATA::CreateGenericCommunicatorDescriptor()
{
	return ( new COMMUNICATOR_DESCRIPTOR(	QDT_COMMENT("CONSTRAINT_DATA"),
											MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_CONSTRAINT_DATA,
											QDT_COMMENT("498D45C1-E8C2-A8EB-1CEB-9378E82D30F9"),
											sizeof(CONSTRAINT_DATA),
											ComInit,
											QDT_COMMENT("Class for constraint data exported from maya"),
											COMMUNICATOR::GetGenericCommunicatorDescriptor()));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::EM_Construct(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	CONSTRAINT_DATA	*pConstraintData;
	pConstraintData = static_cast<CONSTRAINT_DATA *>(pO);

	QDT_NEW_PLACED(pConstraintData, CONSTRAINT_DATA)();
	QDT_ASSERT(pConstraintData);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::EM_Destruct(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	CONSTRAINT_DATA	*pConstraintData;
	pConstraintData = static_cast<CONSTRAINT_DATA *>(pO);

	QDT_DELETE_PLACED(pConstraintData, CONSTRAINT_DATA);
}

//-----------------------------------------------------------------------------
//	Name:		CM_ReadChunk1
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::CM_ReadChunk1(IO_STREAM &		Stream,
									   COMMUNICATOR *	pCommunicator)
{
	int				n;
	CONSTRAINT_DATA	*pConstraintData;
	pConstraintData = static_cast<CONSTRAINT_DATA *>(pCommunicator);

	Stream >> pConstraintData->_Actor1CI;
	Stream >> pConstraintData->_iChildIndex1;
	Stream >> pConstraintData->_Actor2CI;
	Stream >> pConstraintData->_iChildIndex2;
	Stream >> pConstraintData->_GlobalAnchor;
	Stream >> pConstraintData->_GlobalAxis;
	Stream >> pConstraintData->_bCollision;
	Stream >> pConstraintData->_bProjection;
	Stream >> pConstraintData->_rProjectionDistance;
	Stream >> pConstraintData->_rProjectionAngle;
	Stream >> n;
	pConstraintData->_XMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	pConstraintData->_YMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	pConstraintData->_ZMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	pConstraintData->_Swing1MotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	pConstraintData->_Swing2MotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> n;
	pConstraintData->_TwistMotionType = static_cast<JOINT_CUSTOM_MOTION>(n);
	Stream >> pConstraintData->_rLinearLimitValue;
	Stream >> pConstraintData->_rLinearLimitRestitution;
	Stream >> pConstraintData->_rLinearLimitSpring;
	Stream >> pConstraintData->_rLinearLimitDamper;
	Stream >> pConstraintData->_rSwing1LimitValue;
	Stream >> pConstraintData->_rSwing1LimitRestitution;
	Stream >> pConstraintData->_rSwing1LimitSpring;
	Stream >> pConstraintData->_rSwing1LimitDamper;
	Stream >> pConstraintData->_rSwing2LimitValue;
	Stream >> pConstraintData->_rSwing2LimitRestitution;
	Stream >> pConstraintData->_rSwing2LimitSpring;
	Stream >> pConstraintData->_rSwing2LimitDamper;
	Stream >> pConstraintData->_rTwistLowLimitValue;
	Stream >> pConstraintData->_rTwistLowLimitRestitution;
	Stream >> pConstraintData->_rTwistLowLimitSpring;
	Stream >> pConstraintData->_rTwistLowLimitDamper;
	Stream >> pConstraintData->_rTwistHighLimitValue;
	Stream >> pConstraintData->_rTwistHighLimitRestitution;
	Stream >> pConstraintData->_rTwistHighLimitSpring;
	Stream >> pConstraintData->_rTwistHighLimitDamper;
	Stream >> n;
	pConstraintData->_SwingDriveType = static_cast<JOINT_CUSTOM_DRIVE_TYPE>(n);
	Stream >> pConstraintData->_rSwingDriveSpring;
	Stream >> pConstraintData->_rSwingDriveDamper;
	Stream >> pConstraintData->_SwingDriveOrientation;
	Stream >> pConstraintData->_SwingDriveAngularVelocity;
}

//-----------------------------------------------------------------------------
//	Name:		CM_WriteChunk1
//	Object:		
//	06-03-13:	ELE - Created
//-----------------------------------------------------------------------------
void	CONSTRAINT_DATA::CM_WriteChunk1(IO_STREAM &		Stream,
										COMMUNICATOR *	pCommunicator)
{
	CONSTRAINT_DATA	*pConstraintData;
	pConstraintData = static_cast<CONSTRAINT_DATA *>(pCommunicator);

	Stream.DebugOutput("Actor 1 :");
	Stream << pConstraintData->_Actor1CI;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Child index 1 :");
	Stream << pConstraintData->_iChildIndex1;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Actor 2 :");
	Stream << pConstraintData->_Actor2CI;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Child index 2 :");
	Stream << pConstraintData->_iChildIndex2;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Global anchor :");
	Stream << pConstraintData->_GlobalAnchor;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Global axis :");
	Stream << pConstraintData->_GlobalAxis;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Collisions :");
	Stream << pConstraintData->_bCollision;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Projection :");
	Stream << pConstraintData->_bProjection;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Projection distance :");
	Stream << pConstraintData->_rProjectionDistance;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Projection angle :");
	Stream << pConstraintData->_rProjectionAngle;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type X :");
	Stream << pConstraintData->_XMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type Y :");
	Stream << pConstraintData->_YMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type Z :");
	Stream << pConstraintData->_ZMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type swing 1 :");
	Stream << pConstraintData->_Swing1MotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type swing 2 :");
	Stream << pConstraintData->_Swing2MotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Motion type twist :");
	Stream << pConstraintData->_TwistMotionType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit value :");
	Stream << pConstraintData->_rLinearLimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit restitution :");
	Stream << pConstraintData->_rLinearLimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit spring :");
	Stream << pConstraintData->_rLinearLimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Linear limit damper :");
	Stream << pConstraintData->_rLinearLimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit value :");
	Stream << pConstraintData->_rSwing1LimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit restitution :");
	Stream << pConstraintData->_rSwing1LimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit spring :");
	Stream << pConstraintData->_rSwing1LimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 1 limit damper :");
	Stream << pConstraintData->_rSwing1LimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit value :");
	Stream << pConstraintData->_rSwing2LimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit restitution :");
	Stream << pConstraintData->_rSwing2LimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit spring :");
	Stream << pConstraintData->_rSwing2LimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing 2 limit damper :");
	Stream << pConstraintData->_rSwing2LimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist low limit value :");
	Stream << pConstraintData->_rTwistLowLimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist lox limit restitution :");
	Stream << pConstraintData->_rTwistLowLimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist low limit spring :");
	Stream << pConstraintData->_rTwistLowLimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist low limit damper :");
	Stream << pConstraintData->_rTwistLowLimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist high limit value :");
	Stream << pConstraintData->_rTwistHighLimitValue;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist lox limit restitution :");
	Stream << pConstraintData->_rTwistHighLimitRestitution;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist high limit spring :");
	Stream << pConstraintData->_rTwistHighLimitSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Twist high limit damper :");
	Stream << pConstraintData->_rTwistHighLimitDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive type :");
	Stream << pConstraintData->_SwingDriveType;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive spring :");
	Stream << pConstraintData->_rSwingDriveSpring;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive damper :");
	Stream << pConstraintData->_rSwingDriveDamper;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive target orientation :");
	Stream << pConstraintData->_SwingDriveOrientation;
	Stream.DebugOutput("\r\n");

	Stream.DebugOutput("Swing drive target  velocity :");
	Stream << pConstraintData->_SwingDriveAngularVelocity;
	Stream.DebugOutput("\r\n");
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
