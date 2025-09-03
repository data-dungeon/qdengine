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
//	CLASS:	IHM_ELEMENT_RECT_DESC
//	The IHM_ELEMENT_RECT_DESC class implements ...
//
//	01-11-30:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_ELEMENT_RECT_DESC : public IHM_ELEMENT_ABC_DESC
{
public:

	enum
	{
		SET_RELATIVE_POSITION = IHM_ELEMENT_ABC_DESC::MAX_COMMANDS,
		GET_RELATIVE_POSITION,
		MAX_COMMANDS,
		NB_COMMANDS	 = (MAX_COMMANDS - 1),
	};

	/** Derivation from DESC: Receive command processing. */
	virtual bool	ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;


protected:

			bool	OnSetRelativePosition(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
			bool	OnGetRelativePosition(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;

	/** Constructor */
			 IHM_ELEMENT_RECT_DESC(OBJECT_TYPE Type, const char *pcName, bool bIsSingleton, int nNbrCommands, const char * const pcCommand);
	/** Destructor */
	virtual ~IHM_ELEMENT_RECT_DESC();
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
