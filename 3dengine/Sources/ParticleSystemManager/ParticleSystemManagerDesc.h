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
// CLASS: PARTICLE_SYSTEM_MANAGER_DESC
//
//
//00-11-22 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: PARTICLE_SYSTEM_MANAGER_DESC
//00-11-22 JVA Created: 
//=============================================================================
class PARTICLE_SYSTEM_MANAGER_DESC : public DESC
{	
public:
		
	enum
	{
		COMMAND_CREATE_PARTICLE_SHAPE			= 1,
		COMMAND_CREATE_PARTICLE_SHAPE_SPRITE,
		MAX_COMMANDS,
		NB_COMMANDS	=  (MAX_COMMANDS - 1),
	};
	
	static DESC*			Instance(); 
	static void				Suicide(); 
	
	bool			ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack)const;
	
	COM_OBJECT		*New();
	void			Delete(COM_OBJECT *);
	
protected:
	PARTICLE_SYSTEM_MANAGER_DESC();
	~PARTICLE_SYSTEM_MANAGER_DESC();

private:
	
	static PARTICLE_SYSTEM_MANAGER_DESC*		_pInstance;
	static bool				_bDestroyedOnce; 
};
//=============================================================================
// CODE ENDS HERE
//=============================================================================
