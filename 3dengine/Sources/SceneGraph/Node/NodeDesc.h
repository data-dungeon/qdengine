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
// CLASS: NODE_DESC
//
//
//00-11-16 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: NODE_DESC
//00-11-16 JVA Created: 
//=============================================================================
class NODE_DESC : public DESC
{	
public:
		
	enum
	{
		COMMAND_IS_PLAYING,
		COMMAND_PURGE_BEHAVIOR,
		COMMAND_LOOK_AT,
		COMMAND_GET_SKELETON,
		COMMAND_GET_GLOBAL_ROTATION,
		COMMAND_SET_COLLISION,
		COMMAND_SELF_DETACH,
		COMMAND_SET_QUATERNION,
		COMMAND_GET_QUATERNION,
		COMMAND_RESET_PITCH,
		COMMAND_RESET_HEADING,
		COMMAND_RESET_ROLL,
		COMMAND_INSTANCIATE,
		COMMAND_GET_FRAME_NB,
		COMMAND_HAS_A_MESH,
		COMMAND_HAS_A_SKELETON,
		COMMAND_GET_ENTITY,
		COMMAND_IS_PLAYING_SPECIFIC,
		COMMAND_GET_GLOBAL_ROT_Y,
		COMMAND_DUMP,
		COMMAND_IS_CHILD_OF,
		COMMAND_PLAY_ANIM,
		COMMAND_PUSH_ANIM,
		COMMAND_GET_ACTIVE_ANIM,
		COMMAND_RESET_CONTAINER_MATRIX,
		COMMAND_UPDATE_PARTICLE_EMITTER,
		COMMAND_SPAWN_PARTICLES,
		COMMAND_GET_BOX_GLOBAL_POSITION,
		COMMAND_CHECK_SUB_GRAPH,
		COMMAND_GET_PITCH_HEAD_ROLL_ANGLES,
		COMMAND_GET_PITCH_HEAD_ROLL,
		COMMAND_SET_GLOBAL_POSITION_FLOAT,
		COMMAND_GET_GLOBAL_POSITION_FLOAT,
		COMMAND_ADD_BOUNDING_VOLUME_ON_SOFT_BODY,
		COMMAND_REMOVE_BOUNDING_VOLUME_ON_SOFT_BODY,
		MAX_COMMANDS,
		NB_COMMANDS	= (MAX_COMMANDS - 1),
	};
	
	enum
	{
		NODE_MEM_POOL_SIZE = 1024
	};


	static DESC*			Instance(); 
	static void				Suicide(); 
	
	bool			ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack)const;

	COM_OBJECT		*New();
	void			Delete(class COM_OBJECT *pComObject = NULL);

protected:
	
	NODE_DESC();
	~NODE_DESC();

private:
	
	static NODE_DESC*		_pInstance;
	static bool				_bDestroyedOnce; 

	static MEM_POOL_HEADER * _pMPNode;
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
