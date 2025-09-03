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
//	CLASS:	PATH_UNIT_DESC
//	The PATH_UNIT_DESC class implements ...
//
//	02-04-08:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	PATH_UNIT_DESC : DESC
{
public:

	enum
	{
		CMD_UNKNOWN = 0,
		MAX_CMDS,
		NBR_CMDS	= (MAX_CMDS - 1)
	};

	/** Singleton */
	static DESC*			Instance();
	static void				Suicide();

	/** Derivation from DESC: */
	bool					ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack)const;
	COM_OBJECT*				New();

private:

	/** Constructor & destructor */
							PATH_UNIT_DESC();
						   ~PATH_UNIT_DESC();

	/** Singleton */
	static PATH_UNIT_DESC*	_pInstance;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
