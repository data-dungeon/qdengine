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
//	CLASS:	PATH_FIND_SOLVER
//	The PATH_FIND_SOLVER class implements ...
//
//	02-07-11:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_FIND_SOLVER
{
public:

	// Singleton
	static	PATH_FIND_SOLVER*	Instance();
	static	void				Release();

	// Ask in which sector is the unit
	COM_INSTANCE *				FindUnitSector(COM_HANDLE *pUnitHandle);

private:

								PATH_FIND_SOLVER();
							   ~PATH_FIND_SOLVER();	

	static	PATH_FIND_SOLVER *	_pInstance;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
