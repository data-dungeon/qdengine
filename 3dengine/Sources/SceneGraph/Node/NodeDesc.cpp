//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: NODE_DESC
//
//
//	00-11-16	JVA - Created
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[ ] = __FILE__;
#endif
//*****************************************************************************
//
// CLASS DESCRIPTION ( NODE_DESC )
//
//	The NODE_DESC class implements ...
//
//*****************************************************************************

#include	"include.h"
#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(SceneGraph\Entities)
#include	INC_3DENGINE(SceneGraph\Entities\Particles)
#include	INC_3DENGINE(SceneGraph\Entities\Mesh\SoftBody)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(Catalog)

#include	INC_KRNCORE(Time)
#include	INC_KRNCORE(Math/Geometry/Quaternions)
#include	INC_KRNCORE(Memory)

#ifndef	_DEBUG
	#include	"Inline.h"
	#include	INL_3DENGINE(SceneGraph\Entities)
	
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes0D/Point3D)
	#include	INL_KRNCORE(Math/Geometry/Shapes/Shapes1D/Vector)
	#include	INL_KRNCORE(Math/Geometry/Matrix)
	#include	INL_KRNCORE(Math/Geometry/Quaternions)
	#include	INL_KRNCORE(Math/Geometry/QuaternionVector)
	#include	INL_KRNCORE(Math/Geometry/More)
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================


NODE_DESC*		NODE_DESC::_pInstance = NULL;
bool			NODE_DESC::_bDestroyedOnce = false;
static const char * const pcComment = "Ask 3D Engine to document this class.";

MEM_POOL_HEADER *	NODE_DESC::_pMPNode = NULL;

//-----------------------------------------------------------------------------
// Name: NODE_DESC constructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
NODE_DESC::NODE_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE, "m3d_node", false, NB_COMMANDS, pcComment)
{	
	_pCommandDescsTab[13].Init(COMMAND_SET_LIGHTING_MODE, 1, "SetLighting", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[13].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bValue");
	
	_pCommandDescsTab[14].Init(COMMAND_LIVE_SKELETON_NODE, 1, "LiveSkeletonNode", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[14].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "SkeletonNode");
	
	_pCommandDescsTab[15].Init(COMMAND_LOOK_UP, 1, "LookUp", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");
	
	_pCommandDescsTab[16].Init(COMMAND_LOOK_DOWN, 1, "LookDown", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[16].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");
	
	_pCommandDescsTab[17].Init(COMMAND_TURN_LEFT, 1, "TurnLeft", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[17].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");
	
	_pCommandDescsTab[18].Init(COMMAND_TURN_RIGHT, 1, "TurnRight", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[18].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");
	
	_pCommandDescsTab[19].Init(COMMAND_ROLL_LEFT, 1, "RollLeft", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[19].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");
	
	_pCommandDescsTab[20].Init(COMMAND_ROLL_RIGHT, 1, "RollRight", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[20].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");
	
	_pCommandDescsTab[21].Init(COMMAND_MOVE_FORWARD, 1, "MoveForward", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[21].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rValue");
	
	_pCommandDescsTab[22].Init(COMMAND_MOVE_BACKWARD, 1, "MoveBackward", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[22].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rValue");
	
	_pCommandDescsTab[23].Init(COMMAND_STRAFE_LEFT, 1, "StrafeLeft", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[23].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rValue");
	
	_pCommandDescsTab[24].Init(COMMAND_STRAFE_RIGHT, 1, "StrafeRight", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[24].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rValue");
	
	_pCommandDescsTab[25].Init(COMMAND_MOVE_UP, 1, "MoveUp", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[25].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rValue");
	
	_pCommandDescsTab[26].Init(COMMAND_MOVE_DOWN, 1, "MoveDown", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[26].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rValue");
	
	_pCommandDescsTab[27].Init(COMMAND_IS_PLAYING, 1, "IsPlaying", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[27].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "boolean");
	
	_pCommandDescsTab[28].Init( COMMAND_PURGE_BEHAVIOR , 0 , "PurgeBehavior", "Ask 3D Engine to document this command." );
	
	_pCommandDescsTab[29].Init(COMMAND_LOOK_AT, 1, "LookAt", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[29].SetParameterDesc(0, OTYPE_KERNEL_POINT3D , 1, PARAMETER_DESC::IN_PARAMETER, "pGlobalPos");
	
	_pCommandDescsTab[30].Init(COMMAND_GET_SKELETON, 1, "GetSkeleton", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[30].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID , 1, PARAMETER_DESC::OUT_PARAMETER, "Skeleton node");
	_pCommandDescsTab[30].SetObsolete(true);

	_pCommandDescsTab[31].Init(COMMAND_GET_GLOBAL_ROTATION, 1, "GetGlobalRotation", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[31].SetParameterDesc(0, OTYPE_KERNEL_ANGLE3D , 1, PARAMETER_DESC::OUT_PARAMETER, "rotation");
	
	_pCommandDescsTab[32].Init(COMMAND_SET_COLLISION, 1, "SetCollision", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[32].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN , 1, PARAMETER_DESC::IN_PARAMETER, "status");
	
	_pCommandDescsTab[33].Init(COMMAND_SELF_DETACH, 0, "SelfDetach", "Ask 3D Engine to document this command." );
	
	_pCommandDescsTab[34].Init(COMMAND_SET_QUATERNION, 1, "SetQuaternion", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[34].SetParameterDesc(0, OTYPE_KERNEL_QUATERNION , 1, PARAMETER_DESC::IN_PARAMETER, "rotation");
	
	_pCommandDescsTab[35].Init(COMMAND_GET_QUATERNION, 1, "GetQuaternion", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[35].SetParameterDesc(0, OTYPE_KERNEL_QUATERNION , 1, PARAMETER_DESC::OUT_PARAMETER, "rotation");

	_pCommandDescsTab[36].Init(COMMAND_RESET_PITCH, 0, "ResetPitch", "Ask 3D Engine to document this command." );

	_pCommandDescsTab[37].Init(COMMAND_RESET_HEADING, 0, "ResetHeading", "Ask 3D Engine to document this command." );

	_pCommandDescsTab[38].Init(COMMAND_RESET_ROLL, 0, "ResetRoll", "Ask 3D Engine to document this command." );

	_pCommandDescsTab[39].Init(COMMAND_INSTANCIATE, 1, "Instanciate", "Must be call on the node we want to instanciate." );
	_pCommandDescsTab[39].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "NewNode", "Node to recieve the instanciation");

	_pCommandDescsTab[40].Init(COMMAND_HAS_A_MESH, 1, "HasAMesh", "Tells if the node's entity is a Mesh." );
	_pCommandDescsTab[40].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bRes");

	_pCommandDescsTab[41].Init(COMMAND_HAS_A_SKELETON, 1, "HasASkeleton", "Tells if the node's entity is a skeleton." );
	_pCommandDescsTab[41].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::OUT_PARAMETER, "bRes");

	_pCommandDescsTab[42].Init(COMMAND_GET_ENTITY, 1, "GetEntity", "returns the entity of the node." );
	_pCommandDescsTab[42].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "Entity");

	_pCommandDescsTab[43].Init(COMMAND_GET_FRAME_NB, 1, "GetFrame", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[43].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::OUT_PARAMETER, "rValue");

	_pCommandDescsTab[44].Init(COMMAND_IS_PLAYING_SPECIFIC, 2, "IsPlayingSpecific", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[44].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Animation");
	_pCommandDescsTab[44].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN	, 1, PARAMETER_DESC::OUT_PARAMETER, "return value");

	_pCommandDescsTab[45].Init(COMMAND_GET_GLOBAL_ROT_Y, 1, "GetRotY", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[45].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::OUT_PARAMETER, "rValue");

	_pCommandDescsTab[46].Init(COMMAND_DUMP, 1, "Dump", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[46].SetParameterDesc(0, OTYPE_KERNEL_CHAR, 256, PARAMETER_DESC::IN_PARAMETER , "FileName");

	_pCommandDescsTab[47].Init(COMMAND_IS_CHILD_OF, 2, "IsChildOf", "This method return true if the node is a child of the node placed in parameter. This return also true if the node is the same as the node placed in parameter!");
	_pCommandDescsTab[47].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Node");
	_pCommandDescsTab[47].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bRes");

	_pCommandDescsTab[48].Init(COMMAND_PLAY_ANIM, 1, "PlayAnim", "Add an animatiom in the node animation queue" );
	_pCommandDescsTab[48].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER , "AnimID");
	
	_pCommandDescsTab[49].Init(COMMAND_PUSH_ANIM, 1, "PushAnim", "Push an animation" );
	_pCommandDescsTab[49].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER , "AnimID");
	
	_pCommandDescsTab[50].Init(COMMAND_GET_ACTIVE_ANIM, 1, "GetActiveAnimation", "Get the active animation (ANIM_NODE)" );
	_pCommandDescsTab[50].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER , "AnimID");

	_pCommandDescsTab[51].Init(COMMAND_RESET_CONTAINER_MATRIX, 0, "ResetAnimationMatrix", "Reset animation matrix" );
	_pCommandDescsTab[51].SetObsolete(true);

	_pCommandDescsTab[52].Init(COMMAND_UPDATE_PARTICLE_EMITTER, 1, "UpdateParticleEmitter", "Set the particle emitter state as if it was activated nTime milliseconds ago" );
	_pCommandDescsTab[52].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nTime");

	_pCommandDescsTab[53].Init(COMMAND_SPAWN_PARTICLES, 1, "SpawnParticles", "Spwan immediately nNbrParticles particles");
	_pCommandDescsTab[53].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nNbrParticles");

	_pCommandDescsTab[54].Init(COMMAND_GET_BOX_GLOBAL_POSITION, 1, "GetBoxGlobalPosition", "Get global position of the bounding box" );
	_pCommandDescsTab[54].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::OUT_PARAMETER, "Position");

	_pCommandDescsTab[55].Init(COMMAND_CHECK_SUB_GRAPH, 2, "CheckSubGraph", "Check if child links are valid, use bCorrect with value true to force correction" );
	_pCommandDescsTab[55].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bCorrect");
	_pCommandDescsTab[55].SetParameterDesc(1, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::OUT_PARAMETER, "nResult");
	
	_pCommandDescsTab[56].Init(COMMAND_MOVE_NEAR_TO, 2, "MoveNearTo", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[56].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::IN_PARAMETER, "TargetPos");
	_pCommandDescsTab[56].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");

	_pCommandDescsTab[57].Init(COMMAND_MOVE_AWAY_FROM, 2, "MoveAwayFrom", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[57].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::IN_PARAMETER, "TargetPos");
	_pCommandDescsTab[57].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");

	_pCommandDescsTab[58].Init(COMMAND_MOVE_LEFT_AROUND, 2, "MoveLeftAround", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[58].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::IN_PARAMETER, "TargetPos");
	_pCommandDescsTab[58].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");

	_pCommandDescsTab[59].Init(COMMAND_MOVE_RIGHT_AROUND, 2, "MoveRightAround", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[59].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::IN_PARAMETER, "TargetPos");
	_pCommandDescsTab[59].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");

	_pCommandDescsTab[60].Init(COMMAND_MOVE_UP_AROUND, 2, "MoveUpAround", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[60].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::IN_PARAMETER, "TargetPos");
	_pCommandDescsTab[60].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");

	_pCommandDescsTab[61].Init(COMMAND_MOVE_DOWN_AROUND, 2, "MoveDownAround", "Ask 3D Engine to document this command." );
	_pCommandDescsTab[61].SetParameterDesc(0, OTYPE_KERNEL_POINT3D, 1, PARAMETER_DESC::IN_PARAMETER, "TargetPos");
	_pCommandDescsTab[61].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAngle");

	_pCommandDescsTab[62].Init(COMMAND_GET_PITCH_HEAD_ROLL_ANGLES, 1, "GetPitchHeadRollAngles", "Return an Angle3d containing the Pitch Head and Roll angles of this node." );
	_pCommandDescsTab[62].SetParameterDesc(0, OTYPE_KERNEL_ANGLE3D, 1, PARAMETER_DESC::OUT_PARAMETER, "Angle3d");
	
	_pCommandDescsTab[63].Init(COMMAND_GET_PITCH_HEAD_ROLL, 3, "GetPitchHeadRoll", "Return Quaternions for Pitch Head and Roll of this node." );
	_pCommandDescsTab[63].SetParameterDesc(0, OTYPE_KERNEL_QUATERNION, 1, PARAMETER_DESC::OUT_PARAMETER, "Quaternion Pitch");
	_pCommandDescsTab[63].SetParameterDesc(1, OTYPE_KERNEL_QUATERNION, 1, PARAMETER_DESC::OUT_PARAMETER, "Quaternion Heading");
	_pCommandDescsTab[63].SetParameterDesc(2, OTYPE_KERNEL_QUATERNION, 1, PARAMETER_DESC::OUT_PARAMETER, "Quaternion Roll ");
	
	_pCommandDescsTab[64].Init(COMMAND_SET_GLOBAL_POSITION_FLOAT, 3, "SetGlobalPosition", "Set the global position of the node.");
	_pCommandDescsTab[64].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rX", "The X coordinate of the new global position.");
	_pCommandDescsTab[64].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rY", "The Y coordinate of the new global position.");
	_pCommandDescsTab[64].SetParameterDesc(2, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rZ", "The Z coordinate of the new global position.");

	_pCommandDescsTab[65].Init(COMMAND_GET_GLOBAL_POSITION_FLOAT, 3, "GetGlobalPosition", "Get the global position of the node.");
	_pCommandDescsTab[65].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::OUT_PARAMETER, "rX", "The X coordinate of the global position.");
	_pCommandDescsTab[65].SetParameterDesc(1, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::OUT_PARAMETER, "rY", "The Y coordinate of the global position.");
	_pCommandDescsTab[65].SetParameterDesc(2, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::OUT_PARAMETER, "rZ", "The Z coordinate of the global position.");

	_pCommandDescsTab[66].Init(COMMAND_ADD_BOUNDING_VOLUME_ON_SOFT_BODY, 4, "AddBoundingVolumeOnSoftBody", "Add the bounding volume of the mesh as collision bounding volume for the soft body on the node");
	_pCommandDescsTab[66].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Node", "Node of the mesh which have the bounding volume");
	_pCommandDescsTab[66].SetParameterDesc(1, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nBVType", "Bounding volume type : BV_SPHERE, BV_BOX, BV_CYLINDERS");
	_pCommandDescsTab[66].SetParameterDesc(2, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rAbsorption", "Absorption of energy when collide, 0.0 - 1.0 = absorption, >= 1.0 = propulsion.");
	_pCommandDescsTab[66].SetParameterDesc(3, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rE", "rE is the distance from the bounding volume where particle will be replaced when a collision happen.");

	_pCommandDescsTab[67].Init(COMMAND_REMOVE_BOUNDING_VOLUME_ON_SOFT_BODY, 2, "RemoveBoundingVolumeOnSoftBody", "Remove the bounding volume of the mesh as collision bounding volume from the soft body on the node");
	_pCommandDescsTab[67].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Node", "Node of the mesh which have the bounding volume");
	_pCommandDescsTab[67].SetParameterDesc(1, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nBVType", "Bounding volume type : BV_SPHERE, BV_BOX, BV_CYLINDERS");

	CreateEnumTab(3);
	SetEnum(0, "BV_SPHERE", SOFT_BODY::BV_SPHERE);
	SetEnum(1, "BV_BOX", SOFT_BODY::BV_BOX);
	SetEnum(2, "BV_CYLINDERS", SOFT_BODY::BV_CYLINDERS);
}

//-----------------------------------------------------------------------------
// Name: NODE_DESC destructor
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
NODE_DESC::~NODE_DESC()
{

}

//-----------------------------------------------------------------------------
//	Name:		New
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
COM_OBJECT*	NODE_DESC::New()
{
	//MEM Pool init
	if	(_pMPNode == NULL)
	{
		_pMPNode = new MEM_POOL_HEADER(sizeof(NODE), NODE_MEM_POOL_SIZE);
	}

	_nNbObjects++;
	
	NODE *pNode = static_cast<NODE*>(_pMPNode->Allocate());

	//new (_pMPNode->Allocate()) NODE ();
	QDT_NEW_PLACED(pNode, NODE)();
	
	return	(pNode);
}

//-----------------------------------------------------------------------------
//	Name:		Delete
//	Object:		
//	02-01-11:	DAC - Created
//-----------------------------------------------------------------------------
void	NODE_DESC::Delete(class COM_OBJECT *	pComObject)
{
	if	(pComObject)
	{
		--_nNbObjects;
		static_cast<NODE*>(pComObject)->NODE::~NODE();
		_pMPNode->Deallocate(pComObject);
	}
}


//-----------------------------------------------------------------------------
// Name: NODE_DESC::Instance
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
DESC*	NODE_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = ( new NODE_DESC() );
	}
	
	return ( _pInstance );
}

//-----------------------------------------------------------------------------
// Name: NODE_DESC::Suicide
// Object:
//00 - 11 - 16 JVA Created:
//-----------------------------------------------------------------------------
void	NODE_DESC::Suicide()
{
	QDT_ASSERT( _bDestroyedOnce == false );
	QDT_ASSERT( _pInstance != NULL );
	
	if ( _pInstance != NULL )
	{
		delete _pInstance;
		_pInstance		 = NULL;
		_bDestroyedOnce	 = false;
	}
}


//-----------------------------------------------------------------------------
// Name: NODE_DESC::ReceiveCommandProc
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
bool	NODE_DESC::ReceiveCommandProc(void *pObject, COMMAND_ID CommandID , PARAMETER_STACK & Stack) const
{
	NODE	*pNode = static_cast <NODE *> (pObject);
	
	QDT_ASSERT(pNode);
	
	switch ( CommandID )
	{

	default:
		return DESC::DefaultCommand((COM_OBJECT *)pObject, CommandID, Stack);
		
	case NODE_DESC::COMMAND_ATTACH_CHILD:
		{
			const OBJECT_ID &ChildID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(ChildID.GetType(), ChildID.GetID());
			if (pIndexInt != NULL)
			{
				pNode->AttachChild(pIndexInt, true);			
			}
			else
			{
				QDT_Warning("The node %s has not been found in the data base", ChildID.GetName());
			}
		}
		break;
		
	case NODE_DESC::COMMAND_DETACH_CHILD:
		{
			const OBJECT_ID &ChildID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(ChildID.GetType(), ChildID.GetID());
			
			if (pIndexInt != NULL)
			{
				pNode->DetachChild(pIndexInt);		
			}
			else
			{
				QDT_Warning("The node %s has not been found in the data base", pIndexInt->GetName());
			}			
		}
		break;
		
	case NODE_DESC::COMMAND_ATTACH_CHILD_KEEP_MATRIX:
		{
			const OBJECT_ID &ChildID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(ChildID.GetType(), ChildID.GetID());
			
			if (pIndexInt != NULL)
			{
				pNode->AttachChildKeepGlobalMat(pIndexInt);			
			}
			else
			{
				QDT_Warning("NODE::AttachChildKeepMatrix : the node has not been found in the database.");
			}
		}
		break;
		
	case NODE_DESC::COMMAND_DETACH_CHILD_KEEP_MATRIX:
		{
			const OBJECT_ID &ChildID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(ChildID.GetType(), ChildID.GetID());
			
			if (pIndexInt != NULL)
			{
				pNode->DetachChildKeepGlobalMat(pIndexInt);		
			}
			else
			{
				QDT_Warning("NODE::DetachCHildKeepMatrix : the node has not been found in the database.");
			}			
		}
		break;
		
	case NODE_DESC::COMMAND_ATTACH_CATALOG:
		{
			/*const OBJECT_ID &CatalogID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(CatalogID.GetType(), CatalogID.GetID());
			
			if (pIndexInt != NULL)
			{
				CATALOG *pCatalog = static_cast<CATALOG *>(pIndexInt->GetObject());
				pNode->AttachCatalog(pCatalog);
			}
			else
			{
				QDT_Warning("NODE::AttachCatalog : the catalog has not been found in the database.");
			}*/
			break;
		}
		break;
		
	case NODE_DESC::COMMAND_DETACH_CATALOG:
		{
			/*const OBJECT_ID &CatalogID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(CatalogID.GetType(), CatalogID.GetID());
			
			if (pIndexInt != NULL)
			{
				pNode->DetachCatalog(pIndexInt);			
			}
			else
			{
				QDT_Warning("The catalog %s has not been found in the data base", pIndexInt->GetName() );
			}*/
			break;
		}
		break;
		
	case NODE_DESC::COMMAND_SET_GLOBAL_POSITION:
		{
			pNode->SetGlobalPosition(Stack[0]) ; 		
		}
		break ;
		
	case NODE_DESC::COMMAND_SET_LOCAL_POSITION:
		{
			pNode->SetLocalPosition(Stack[0]) ; 		
		}
		break;
		
	case NODE_DESC::COMMAND_SET_GLOBAL_ROTATION:
		{
			ANGLE3D angle(Stack[0]);
			angle.SetAlpha(MATH::DegreToRadian(angle.GetAlpha()));
			angle.SetBeta(MATH::DegreToRadian(angle.GetBeta()));
			angle.SetGamma(MATH::DegreToRadian(angle.GetGamma()));
			pNode->SetGlobalRotXYZ(angle);
		}
		break;
		
	case NODE_DESC::COMMAND_SET_LOCAL_ROTATION:
		{
			ANGLE3D angle(Stack[0]);
			angle.SetAlpha(MATH::DegreToRadian(angle.GetAlpha()));
			angle.SetBeta(MATH::DegreToRadian(angle.GetBeta()));
			angle.SetGamma(MATH::DegreToRadian(angle.GetGamma()));
			pNode->SetLocalRotXYZ(angle);
		}
		break;
		
	case NODE_DESC::COMMAND_GET_GLOBAL_POSITION:
		{
			POINT3D Pos(pNode->GetGlobalPosition()) ;
			Stack.SetPoint3D(0, Pos) ; 						
		}
		break ;
		
	case NODE_DESC::COMMAND_GET_GLOBAL_ROTATION :
		{
			ANGLE3D angle ;
			MATRIX mat;
			pNode->UpdateGlobalMatrix() ;
			mat = pNode->GetGlobalMatrix() ;
			angle = mat.GetRotXYZ() ;
			Stack.SetAngle3D(0,angle) ;
		}
		break ;
	case NODE_DESC::COMMAND_GET_LOCAL_POSITION:
		{
			POINT3D Pos(pNode->GetLocalPosition()) ;
			Stack.SetPoint3D(0, Pos) ; 		
		}	
		break;
		
	case NODE_DESC::COMMAND_SET_ENTITY:
		{
			const OBJECT_ID &EntityID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(EntityID.GetType(), EntityID.GetID());
			if (!pIndexInt)
			{
				QDT_Message("The entity has not been found");
				break; // Abort command
			}

			QDT_NOT_IMPLEMENTED();
			// pNode->SetEntityIndex(pIndexInt);
		}
		break ; 		
		
	case NODE_DESC::COMMAND_SET_LIGHTING_MODE:
		{
			pNode->GetFlags().Set(NODE::NO_LIGHTING, !Stack.GetBool(0));
		}
		break;
		
	case NODE_DESC::COMMAND_LIVE_SKELETON_NODE:
		{
			const OBJECT_ID &NodeID = Stack[0];
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(NodeID.GetType(), NodeID.GetID());
			
			if (pIndexInt != NULL)
			{
				//	Make live skeleton
				if ( pNode->GetEntityType() != ENTITY_SKELETON)
				{
					QDT_Error("NODE::LiveSkeleton(...): This node is not a skeleton entity !!");
					break;
				}
				pNode->LiveSkeleton( pIndexInt );
			}
			else
			{
				QDT_Warning("The node %s has not been found in the data base", pIndexInt->GetName());
			}
		}
		break;
		
	case NODE_DESC::COMMAND_LOOK_UP :
		{
			pNode->LookUp(MATH::DegreToRadian(Stack[0]));
		}
		break;
		
	case NODE_DESC::COMMAND_LOOK_DOWN :
		{
			pNode->LookDown(MATH::DegreToRadian(Stack[0]));
		}
		break;
		
	case NODE_DESC::COMMAND_TURN_LEFT :
		{
			pNode->TurnLeft(MATH::DegreToRadian(Stack[0]));
		}
		break;
		
	case NODE_DESC::COMMAND_TURN_RIGHT :
		{
			pNode->TurnRight(MATH::DegreToRadian(Stack[0]));
		}
		break;
		
	case NODE_DESC::COMMAND_ROLL_LEFT :
		{
			pNode->RollLeft(MATH::DegreToRadian(Stack[0]));
		}
		break;
		
	case NODE_DESC::COMMAND_ROLL_RIGHT :
		{
			pNode->RollRight(MATH::DegreToRadian(Stack[0]));
		}
		break;
		
	case NODE_DESC::COMMAND_MOVE_FORWARD :
		{
			pNode->MoveForward(Stack[0]);
		}
		break;
		
	case NODE_DESC::COMMAND_MOVE_BACKWARD :
		{
			pNode->MoveBackward(Stack[0]);
		}
		break;
		
	case NODE_DESC::COMMAND_STRAFE_LEFT :
		{
			pNode->StrafeLeft(Stack[0]);
		}
		break;
		
	case NODE_DESC::COMMAND_STRAFE_RIGHT :
		{
			pNode->StrafeRight(Stack[0]);
		}
		break;
		
	case NODE_DESC::COMMAND_MOVE_UP :
		{
			pNode->MoveUp(Stack[0]);
		}
		break;
		
	case NODE_DESC::COMMAND_MOVE_DOWN :
		{
			pNode->MoveDown(Stack[0]);
		}
		break;
		
	case NODE_DESC::COMMAND_IS_PLAYING :
		{
			bool bIsP = pNode->IsPlaying() ;
			Stack.SetBool(0, bIsP) ; 	
		}
		break;
		
	case NODE_DESC::COMMAND_PURGE_BEHAVIOR :
		pNode->RemoveContainer() ;
		break; 
		
	case NODE_DESC::COMMAND_LOOK_AT :
		pNode->LookAt( Stack[0] );
		break;
		
	case NODE_DESC::COMMAND_GET_SKELETON :
		{
			// Note of RMA :
			// This command should be suppressed
			// Please use HAS_A_SKELETON & GET_ENTITY
			//
			
			INDEX_INT	*pSklIdx ; 
			OBJECT_ID	tmpObjID;

			tmpObjID = Stack.GetObjectID(0);
			pSklIdx = DATABASE_MANAGER::Instance()->GetById(tmpObjID.GetType(), tmpObjID.GetID());
			if	(pSklIdx)
			{
				pSklIdx->RemoveReference();
			}

			QDT_NOT_IMPLEMENTED();
			// pSklIdx = pNode->GetSkeleton( ) ;
			if ( pSklIdx )
			{
				pSklIdx->AddReference();
				OBJECT_TYPE typeObj =  pSklIdx->GetType( ) ; 
				
				OBJECT_ID obj( 
					typeObj,
					pSklIdx->GetId()/*, 
					pSklIdx->GetName()*/ ) ;
				
				Stack.SetObjectID( 0, obj ) ; 
			}
		}
		break ;
		
	case NODE_DESC::COMMAND_SET_COLLISION :
		{	
			pNode->GetFlags().Set(NODE::NO_COLLISION, (bool)Stack[0]);	
		}
		break ;
		
	case NODE_DESC::COMMAND_SELF_DETACH:
		{
			pNode->DetachFromFather();
		}
		break;
		
	case NODE_DESC::COMMAND_SET_QUATERNION :
		{
			QUATERNION angle ;
			MATRIX mat;
			angle = Stack.GetQuaternion( 0 ) ;
			angle.Normalize();
			mat = pNode->GetGlobalMatrix() ;
			
			mat.SetQuaternion( angle ) ;
			pNode->SetGlobalMatrix(mat) ;
		}
		break;
		
	case NODE_DESC::COMMAND_GET_QUATERNION :
		{
			QUATERNION angle ;
			MATRIX mat;
			pNode->UpdateGlobalMatrix() ;
			mat = pNode->GetGlobalMatrix() ;
			mat.GetQuaternion(angle) ;
			Stack.SetQuaternion(0,angle) ;
		}
		break;
		
	case NODE_DESC::COMMAND_RESET_PITCH :
		{
			pNode->ResetPitch();
		}
		break ;
		
	case NODE_DESC::COMMAND_RESET_HEADING:
		{
			pNode->ResetHeading();
		}
		break ;
		
	case NODE_DESC::COMMAND_RESET_ROLL:
		{
			pNode->ResetRoll();
		}
		break ;
	case NODE_DESC::COMMAND_GET_FRAME_NB:
		{
			float rFrame ; 
			pNode->GetFrame( rFrame );
		}
		break ;
		
	case NODE_DESC::COMMAND_HAS_A_MESH :
		{
			if (pNode->GetEntity())
			{
				Stack.SetBool(0, pNode->HasAMesh());
			}
		}
		break;
		
	case NODE_DESC::COMMAND_HAS_A_SKELETON :
		{
			if (pNode->GetEntity())
			{
				Stack.SetBool(0, pNode->GetEntityType() == ENTITY_SKELETON);
			}
		}
		break;

	case NODE_DESC::COMMAND_GET_ENTITY :
		{
			OBJECT_ID	objID;
			INDEX_INT	*pIndex;

			objID = Stack.GetObjectID(0);
			pIndex = DATABASE_MANAGER::Instance()->GetById(objID.GetType(), objID.GetID());
			QDT_ASSERT(pIndex);
			pIndex->RemoveReference();

			QDT_NOT_IMPLEMENTED();
			//pIndex = pNode->GetEntityIndex();
			if (pIndex)
			{
				pIndex->AddReference();

				OBJECT_TYPE TypeObj =  pIndex->GetType(); 
				
				OBJECT_ID ObjectId(TypeObj, pIndex->GetId()/*, pIndex->GetName()*/);
				
				Stack.SetObjectID(0, ObjectId); 
			}
		}
		break;

	case NODE_DESC::COMMAND_GET_GLOBAL_ROT_Y:
		{
			VECTOR Vect(0.0f, 0.0f, 1.0f);
			pNode->GetGlobalMatrix().Apply(Vect);
			Stack.SetFloat(0, MATH::ATan(Vect.GetX(), Vect.GetZ()));
		}
		break;

		
	case NODE_DESC::COMMAND_INSTANCIATE:
		{
			const OBJECT_ID &oid = (Stack.GetObjectID(0));
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}
			
			NODE	*pNewNode = static_cast<NODE *>(pIndexInt->GetObject());

			pNode->Instanciate(pNewNode);
		}
		break ;	
		
	case NODE_DESC::COMMAND_IS_PLAYING_SPECIFIC : 
		{
			const OBJECT_ID &oidAnim = (Stack.GetObjectID(0));
			
			if (oidAnim.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_ANIM_PC)
			{
				QDT_Warning("Command aborted : the parameter is not an animation");
				break;
			}
			INDEX_INT *pAnimIndexInt = DATABASE_MANAGER::Instance()->GetById(oidAnim.GetType(), oidAnim.GetID());
			
			if	(!pAnimIndexInt)
			{
				QDT_Warning( "Command aborted : unable to find animation" );
				break;
			}
			bool res = pNode->IsPlayingSpecific( pAnimIndexInt )  ; 
			
			Stack.SetBool(1, res); 
			
		}
		break ;

	case NODE_DESC::COMMAND_DUMP:
		{
			char pcTemp[256];
			memcpy( pcTemp, Stack.GetCharTab( 0 ), 256 );
			pcTemp[ 255 ] = '\0';
			
			pNode->Dump(pcTemp);
		}
		break;

	case COMMAND_IS_CHILD_OF:
		{
			const OBJECT_ID &oid = (Stack.GetObjectID(0));
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}
			
			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());
			
			if (!pIndexInt)
			{
				QDT_Warning( "Command aborted : parameter 1, unable to find node" );
				break;
			}
			
			Stack.SetBool(1, pNode->IsChildOf(pIndexInt));
		}
		break;

	case	COMMAND_PLAY_ANIM:
		{
			OBJECT_ID		AnimID;
			INDEX_INT		*pIdx;

			AnimID	= Stack.GetObjectID(0);
			pIdx	= DATABASE_MANAGER::Instance()->GetById( AnimID.GetType(), AnimID.GetID() );
		
			pNode->PlayAnim( pIdx );
			break;
		}

	case	COMMAND_PUSH_ANIM:
		{
			OBJECT_ID		AnimID;
			INDEX_INT		*pIdx;

			AnimID	= Stack.GetObjectID(0);
			pIdx	= DATABASE_MANAGER::Instance()->GetById( AnimID.GetType(), AnimID.GetID() );
		
			pNode->PushAnim( pIdx );
			break;
		}

	case	COMMAND_GET_ACTIVE_ANIM:
		{
			OBJECT_ID		AnimID;
			INDEX_INT		*pIdx;
			OBJECT_ID		AnimIDParam;

			AnimIDParam = Stack.GetObjectID(0);
			if	(AnimIDParam.GetID() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_ANIM_NODE)
			{
				QDT_Error("Node::GetActiveAnim(...): Wrong object passed, ANIM_NODE expected");
			}

			pIdx = pNode->GetActiveAnim();
			if ( pIdx )
			{
				AnimID =  pIdx->GetObjectId();
			}
			Stack.SetObjectID( 0, AnimID );
			break;
		}


	case COMMAND_RESET_CONTAINER_MATRIX:
		{
			QDT_NOT_IMPLEMENTED();
			break;
		}

	case COMMAND_UPDATE_PARTICLE_EMITTER:
		{
			if	(pNode->GetEntityType() == ENTITY_PARTICLE_EMITTER)
			{
				INDEX_INT	*pZoneNodeIndex = pNode->GetZoneNodeIndex();
				if	(pZoneNodeIndex)
				{
					PARTICLE_EMITTER	*pParticleEmitter;
					PARTICLE_SYSTEM		*pParticleSystem;
					NODE				*pZoneNode = static_cast<NODE *>(pZoneNodeIndex->GetObject());

					if	(pZoneNode->GetEntityType() == ENTITY_ZONE)
					{
						ZONE *	pZone = static_cast<ZONE *>(pZoneNode->GetEntity());
						pParticleSystem = pZone->GetParticleSystem();
					}
					else
					{
						SCENE_GRAPH *	pSceneGraph = static_cast<SCENE_GRAPH *>(pZoneNode->GetEntity());
						pParticleSystem = pSceneGraph->GetParticleSystem();
					}

					pParticleEmitter = static_cast<PARTICLE_EMITTER *>(pNode->GetEntity());

					pParticleEmitter->UpdateParticleEmitter(Stack.GetInteger(0),
															pNode->GetGlobalMatrix(),
															*pParticleSystem);				
				}
			}
			else
			{
				QDT_Warning("Command aborted : the entity is not a particle emitter.");
			}
		}
		break;
		
	case COMMAND_SPAWN_PARTICLES:
		{
			if	(pNode->GetEntityType() == ENTITY_PARTICLE_EMITTER)
			{
				INDEX_INT	*pZoneNodeIndex = pNode->GetZoneNodeIndex();
				if	(pZoneNodeIndex)
				{
					NODE				*pZoneNode = static_cast<NODE *>(pZoneNodeIndex->GetObject());
					ZONE				*pZone = static_cast<ZONE *>(pZoneNode->GetEntity());
					PARTICLE_EMITTER	*pParticleEmitter = static_cast<PARTICLE_EMITTER *>(pNode->GetEntity());

					pParticleEmitter->SpawnParticles(Stack.GetInteger(0),
													 pNode->GetGlobalMatrix(),
													 *(pZone->GetParticleSystem()));
				}
			}
			else
			{
				QDT_Warning("Command aborted : the entity is not a particle emitter.");
			}
		}
		break;

	case COMMAND_GET_BOX_GLOBAL_POSITION:
		{
			Stack.SetPoint3D(0, pNode->GetGlobalCenter());
			break;
		}

	case COMMAND_CHECK_SUB_GRAPH:
		{
			bool	bCorrect = Stack.GetBool( 0 );
			
			int	nRes = pNode->CheckSubGraphFatherLink(bCorrect);
			Stack.SetInteger(0, nRes);
			break;
		}

	case	COMMAND_MOVE_NEAR_TO:
		{
			POINT3D		P;
			float		rVal;
			
			////////////////////////////////
			P		= Stack.GetPoint3D( 0 );
			rVal	= Stack.GetFloat( 1 );
			////////////////////////////////
			pNode->MoveNearTo( P, rVal );
			////////////////////////////////
		}
		break;

	case COMMAND_MOVE_AWAY_FROM:
		{
		
			POINT3D		P;
			float		rVal;
			
			////////////////////////////////
			P		= Stack.GetPoint3D( 0 );
			rVal	= Stack.GetFloat( 1 );
			////////////////////////////////
			pNode->MoveAwayFrom( P, rVal );
			////////////////////////////////
		}
		break;

	case COMMAND_MOVE_LEFT_AROUND:
		{
		
			POINT3D		P;
			float		rVal;
			
			////////////////////////////////
			P		= Stack.GetPoint3D( 0 );
			rVal	= Stack.GetFloat( 1 );
			////////////////////////////////
			pNode->MoveLeftAround( P, rVal , false );
			////////////////////////////////
		}
		break;

	case COMMAND_MOVE_RIGHT_AROUND:
		{
		
			POINT3D		P;
			float		rVal;
			
			////////////////////////////////
			P		= Stack.GetPoint3D( 0 );
			rVal	= Stack.GetFloat( 1 );
			////////////////////////////////
			pNode->MoveRightAround( P, rVal, false );
			////////////////////////////////
		}
		break;

	case COMMAND_MOVE_UP_AROUND:
		{
		
			POINT3D		P;
			float		rVal;
			
			////////////////////////////////
			P		= Stack.GetPoint3D( 0 );
			rVal	= Stack.GetFloat( 1 );
			////////////////////////////////
			pNode->MoveUpAround( P, rVal );
			////////////////////////////////
		}
		break;

	case COMMAND_MOVE_DOWN_AROUND:
		{
		
			POINT3D		P;
			float		rVal;
			
			////////////////////////////////
			P		= Stack.GetPoint3D( 0 );
			rVal	= Stack.GetFloat( 1 );
			////////////////////////////////
			pNode->MoveDownAround( P, rVal );
			////////////////////////////////
		}
		break;

	case COMMAND_GET_PITCH_HEAD_ROLL_ANGLES:
		{
			ANGLE3D	 Angle;
			
			pNode->GetPitchHeadRoll( Angle );
			Stack.SetAngle3D( 0, Angle );
		}
		break;

	case COMMAND_GET_PITCH_HEAD_ROLL:
		{
			QUATERNION	QPitch, QHead, QRoll;

			pNode->GetPitchHeadRoll( QPitch, QHead, QRoll );
			Stack.SetQuaternion( 0, QPitch );
			Stack.SetQuaternion( 1, QHead );
			Stack.SetQuaternion( 2, QRoll );
		}
		break;

	case COMMAND_SET_GLOBAL_POSITION_FLOAT:
		{
			POINT3D NewGlobalPos( Stack.GetFloat( 0 ),
								  Stack.GetFloat( 1 ),
								  Stack.GetFloat( 2 ) );

			pNode->SetGlobalPosition( NewGlobalPos );
		}
		break;
	
	case COMMAND_GET_GLOBAL_POSITION_FLOAT:
		{
			Stack.SetFloat( 0, pNode->GetGlobalPosition().GetX() );
			Stack.SetFloat( 1, pNode->GetGlobalPosition().GetY() );
			Stack.SetFloat( 2, pNode->GetGlobalPosition().GetZ() );
		}
		break;

	case COMMAND_ADD_BOUNDING_VOLUME_ON_SOFT_BODY :
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}

			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (pIndexInt == NULL)
			{
				QDT_Warning("Command aborted : the node has not been found");
				break;
			}

			NODE	*pMeshNode = static_cast<NODE *>(pIndexInt->GetObject());
			if	(pMeshNode->HasAMesh() == false)
			{
				QDT_Warning("Command aborted : the node entity is not a mesh");
				break;
			}

			SOFT_BODY	*pSoftBody = static_cast<SOFT_BODY *>(pNode->GetEntity());
			pSoftBody->AddBoundingVolume(pNode,
										 pMeshNode,
										 (SOFT_BODY::BOUNDING_VOLUME_TYPE)Stack.GetInteger(1),
										 Stack.GetFloat(2),
										 Stack.GetFloat(3));
		}
		break;

	case COMMAND_REMOVE_BOUNDING_VOLUME_ON_SOFT_BODY :
		{
			OBJECT_ID oid = (Stack.GetObjectID(0));
			
			if (oid.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_NODE)
			{
				QDT_Warning("Command aborted : the parameter is not a node");
				break;
			}

			INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById(oid.GetType(), oid.GetID());

			if (pIndexInt == NULL)
			{
				QDT_Warning("Command aborted : the node has not been found");
				break;
			}

			NODE	*pMeshNode = static_cast<NODE *>(pIndexInt->GetObject());
			if	(pMeshNode->HasAMesh() == false)
			{
				QDT_Warning("Command aborted : the node entity is not a mesh");
				break;
			}

			SOFT_BODY	*pSoftBody = static_cast<SOFT_BODY *>(pNode->GetEntity());
			pSoftBody->RemoveBoundingVolume(pMeshNode, (SOFT_BODY::BOUNDING_VOLUME_TYPE)Stack.GetInteger(1));
		}
		break;
	}

	return true;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
