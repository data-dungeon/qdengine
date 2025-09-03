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
// CLASS: BEHAVIOR_MANAGER_DESC
//
//
//00-11-20 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: BEHAVIOR_MANAGER_DESC
//00-11-20 JVA Created: 
//=============================================================================
class BEHAVIOR_MANAGER_DESC : public DESC
{	
public:
		
	enum
	{
		COMMAND_PLAY_ANIMATION = 1,
		COMMAND_APPLY_ROTATION,
		COMMAND_APPLY_SKIN,
		COMMAND_FREE_CAMERA,
		COMMAND_TARGET_CAMERA,
		COMMAND_SET_SPEED , 
		COMMAND_CLEAR_ANIMATION ,
		COMMAND_DELETE_ANIMATION ,
		COMMAND_PURGE_BEHAVIOR,
		COMMAND_DETACH_SKIN,
		COMMAND_MOVE_TO,
		COMMAND_MOVE_TO2,
		COMMAND_PLAY_FACE,
		COMMAND_CONSTRAINT_AIM ,
		COMMAND_CREATE_HIPS_ANIM,
		COMMAND_CREATE_HIPS_ANIM_COMB,

		CMD_INTERPOLATE_BLEND_SHAPE,
		CMD_BLINK_BLEND_SHAPE,
		CMD_PERIODIC_BLINK_BLEND_SHAPE,
		CMD_ANIMATE_BLEND_SHAPE,
		CMD_GET_BLEND_SHAPE_VALUE,

		COMMAND_CREATE_TARGET_ANIM,
		
		MAX_COMMANDS,
		NB_COMMANDS	=  (MAX_COMMANDS - 1),
	};
	
	static DESC*			Instance(); 
	static void				Suicide(); 

	bool			ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack)const;

	class COM_OBJECT *New();
	void	Delete(class COM_OBJECT *);
protected:
	BEHAVIOR_MANAGER_DESC();
	~BEHAVIOR_MANAGER_DESC();

private:
	
	static BEHAVIOR_MANAGER_DESC*		_pInstance;
	static bool				_bDestroyedOnce; 
};

//=============================================================================
// CODE ENDS HERE
//=============================================================================
