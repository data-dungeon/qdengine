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
//	CLASS:	DISPLAY_DUMMIES_DESC
//	The DISPLAY_DUMMIES_DESC class implements ...
//
//	02-06-04:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	DISPLAY_DUMMIES_DESC : public DESC
{
public:

	enum
	{
		CMD_UNKNOWN = 0,

		// Display 3D Dummies
		CMD_SET_DISPLAY_BOUNDING_BOXES,
		CMD_GET_DISPLAY_BOUNDING_BOXES,
		CMD_SET_DISPLAY_BOUNDING_SPHERES,
		CMD_GET_DISPLAY_BOUNDING_SPHERES,
		CMD_SET_DISPLAY_LIGHT_BOUNDING_VOLUMES,
		CMD_GET_DISPLAY_LIGHT_BOUNDING_VOLUMES,
		CMD_SET_DISPLAY_SKELETON_BOUNDING_VOLUMES,
		CMD_GET_DISPLAY_SKELETON_BOUNDING_VOLUMES,
		CMD_SET_DISPLAY_LIGHTS,
		CMD_GET_DISPLAY_LIGHTS,
		CMD_SET_DISPLAY_JOINTS,
		CMD_GET_DISPLAY_JOINTS,
		CMD_SET_DISPLAY_SKELETONS,
		CMD_GET_DISPLAY_SKELETONS,
		CMD_SET_DISPLAY_SHADOW_VOLUMES,
		CMD_GET_DISPLAY_SHADOW_VOLUMES,
		CMD_SET_DISPLAY_QUADTREES,
		CMD_GET_DISPLAY_QUADTREES,
		CMD_SET_DISPLAY_PARTICLE_EMITTERS,
		CMD_GET_DISPLAY_PARTICLE_EMITTERS,
		CMD_SET_DISPLAY_FORCE_FIELDS,
		CMD_GET_DISPLAY_FORCE_FIELDS,
		CMD_SET_DISPLAY_BOUNDING_CYLINDERS,
		CMD_GET_DISPLAY_BOUNDING_CYLINDERS,
		CMD_SET_DISPLAY_PHYSIC,
		CMD_GET_DISPLAY_PHYSIC,
		CMD_SET_DISPLAY_COLLISION_MAP,
		CMD_GET_DISPLAY_COLLISION_MAP,
		
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
	explicit						DISPLAY_DUMMIES_DESC();
								   ~DISPLAY_DUMMIES_DESC();

	/** For singleton */
	static DISPLAY_DUMMIES_DESC*	_pInstance;
	static bool						_bDestroyedOnce;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
