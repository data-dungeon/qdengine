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
//	CLASS:	LIGHTSET_MANAGER
//	The LIGHTSET_MANAGER class implements ...
//
//	02-11-04:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

// Constants
enum
{
	LIGHTSET_MESH_INFLUENCE		= 1,
	LIGHTSET_MATERIAL_INFLUENCE	= 8,
};

class	LIGHTSET_MANAGER
{
public:

	static LIGHTSET_MANAGER*	Instance();
	static void					Suicide();

	void						PushLightTrack(const COM_INSTANCE &LightTrackInst);
	void						Update();

private:

								LIGHTSET_MANAGER();
							   ~LIGHTSET_MANAGER();

	static LIGHTSET_MANAGER	   *_pInstance;

	QDT_DLIST<COM_INSTANCE>		_dlLightTracks;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
