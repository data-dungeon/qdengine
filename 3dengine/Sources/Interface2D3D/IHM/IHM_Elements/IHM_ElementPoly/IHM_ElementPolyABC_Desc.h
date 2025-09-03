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
//	CLASS:	IHM_ELEMENT_POLY_ABC_DESC
//	The IHM_ELEMENT_POLY_ABC_DESC class implements ...
//
//	01-09-27:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_ELEMENT_POLY_ABC_DESC : public IHM_ELEMENT_ABC_DESC
{
public:

	enum
	{
		SET_ZDEPTH = IHM_ELEMENT_ABC_DESC::MAX_COMMANDS,
		UNDER_2D_INTERFACE,
		SET_TRANSPARENCY_MODE,
		SET_TEXTURE,
		SET_MATERIAL,
		MAX_COMMANDS,
		NB_COMMANDS	 = (MAX_COMMANDS - 1),
	};

	
	/** Derivation from DESC: Receive command processing. */
	virtual bool			ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	/** Derivation from DESC: Create a new object. Function is virtual since DESC of an abstract class. */
	virtual COM_OBJECT *	New() = 0;
	/** Derivation from DESC: Delete an object. Function is virtual since DESC of an abstract class. */
	virtual void			Delete(COM_OBJECT *) = 0;


protected:

	/** Constructor */
	         IHM_ELEMENT_POLY_ABC_DESC(OBJECT_TYPE Type, const char *pcName, int nNbrCommands, const char * const pcComment);
	/** Destructor */
	virtual ~IHM_ELEMENT_POLY_ABC_DESC();

private:

	/** Private constructor since IHM_ELEMENT_ABC_DESC is the DESC of an abstract class. */
   	IHM_ELEMENT_POLY_ABC_DESC();
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
