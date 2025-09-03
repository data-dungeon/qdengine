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
//	CLASS:	PATH_FINDING_MANAGER
//	The PATH_FINDING_MANAGER class implements ...
//
//	02-07-11:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_FINDING_MANAGER
{
public:

	PATH_FINDING_MANAGER*				Instance();
	void								Suicide();

	void								PushSector(INDEX_INT	*pi);
	void								RemoveSector(INDEX_INT	*pi);

	void								PushUnit(INDEX_INT	*pi);
	void								RemoveUnit(INDEX_INT *pi);

private:

										PATH_FINDING_MANAGER();
									   ~PATH_FINDING_MANAGER();

	static PATH_FINDING_MANAGER		   *_pInstance;

	QDT_DLIST<INDEX_INT*>				_dlpiSectors;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
