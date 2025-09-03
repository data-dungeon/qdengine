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
// CLASS: LIGHT_DESC
//
//
//00-11-20 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: LIGHT_DESC
//00-11-20 JVA Created: 
//=============================================================================
class LIGHT_DESC : public DESC
{	
public:
		
	enum
	{
		COMMAND_OMNI_LIGHT	= 1,
		COMMAND_SPOT_LIGHT,
		COMMAND_SET_COLOR,
		COMMAND_GET_COLOR,
		COMMAND_SET_HOTSPOT_ANGLE,
		COMMAND_SET_FALLOFF_ANGLE,
		COMMAND_SET_HOTSPOT_DISTANCE,
		COMMAND_SET_FALLOFF_DISTANCE,
		COMMAND_SET_MULTIPLIER,
		MAX_COMMANDS,
		NB_COMMANDS	=  (MAX_COMMANDS - 1),
	};
	
	static DESC*			Instance(); 
	static void				Suicide(); 
	
	bool			ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack)const;
		

	COM_OBJECT		*New();
protected:
	LIGHT_DESC();
	~LIGHT_DESC();

private:
	
	static LIGHT_DESC*		_pInstance;
	static bool				_bDestroyedOnce; 
};
//=============================================================================
// CODE ENDS HERE
//=============================================================================
