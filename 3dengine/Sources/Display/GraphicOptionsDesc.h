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
//	CLASS:	GRAPHIC_OPTIONS_DESC
//	The GRAPHIC_OPTIONS_DESC class implements ...
//
//	02-03-27:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	GRAPHIC_OPTIONS_DESC : public DESC
{
public:

	enum
	{
		CMD_UNKNOWN = 0,

		CMD_ENABLE_LIGHTMAPPING,
		CMD_IS_LIGHTMAPPING_ENABLED,
		CMD_ENABLE_DETAIL_TEXTURE,
		CMD_IS_DETAIL_TEXTURE_ENABLED,
		CMD_ENABLE_ENVIRONMENT_MAPPING,
		CMD_IS_ENVIRONMENT_MAPPING_ENABLED,
		CMD_ENABLE_DYNAMIC_SHADOWS,
		CMD_IS_DYNAMIC_SHADOWS_ENABLED,
		CMD_ENABLE_SELF_SHADOWING,
		CMD_IS_SELF_SHADOWING_ENABLED,
		CMD_ENABLE_DYNAMIC_LIGHTING,
		CMD_IS_DYNAMIC_LIGHTING_ENABLED,
		CMD_ENABLE_LOD,
		CMD_IS_LOD_ENABLED,
		CMD_ENABLE_SHADOWS_LOD,
		CMD_IS_SHADOWS_LOD_ENABLED,
		CMD_SET_TEXTURES_DETAIL,
		CMD_GET_TEXTURES_DETAIL,
		CMD_SET_POLYGONES_DETAIL,
		CMD_GET_POLYGONES_DETAIL,

		MAX_CMDS,
		NBR_CMDS	=  (MAX_CMDS - 1),
	};

	/** For singleton  */
	static DESC*					Instance();
	static void						Suicide();

	/** Derivation from DESC*/
	bool							ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	COM_OBJECT*						New();
	void							Delete(class COM_OBJECT *);

private:

	/** Default constructor & Destructor*/
	explicit						GRAPHIC_OPTIONS_DESC();
								   ~GRAPHIC_OPTIONS_DESC();

	/** For singleton */
	static GRAPHIC_OPTIONS_DESC*	_pInstance;
	static bool						_bDestroyedOnce;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
