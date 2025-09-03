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
//	CLASS:	IHM_ELEMENT_TRIANGLE_DESC
//	The IHM_ELEMENT_TRIANGLE_DESC class implements ...
//
//	01-09-28:	ELE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_ELEMENT_TRIANGLE_DESC : public IHM_ELEMENT_POLY_ABC_DESC
{
public:

	enum
	{
		SET_POINTS = IHM_ELEMENT_POLY_ABC_DESC::MAX_COMMANDS,
		SET_COLORS,
		SET_UVS,
		GET_POINTS,
		GET_COLORS,
		GET_UVS,
		MAX_COMMANDS,
		NB_COMMANDS	 = (MAX_COMMANDS - 1),
	};

	/** For singleton : instance */
	static DESC *		Instance(); 
	/** For singleton : suicide */
	static void			Suicide(); 

	/** Derivation from DESC_ABC: Receive command processing */
	bool				ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack)const;

	/** Derivation from DESC_ABC: Create a new object */
	COM_OBJECT *		New();
	void				Delete(COM_OBJECT *);


protected:

//	bool	OnSetActiveBitmap(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
//	bool	OnSetInactiveBitmap(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;


private:

	/** Constructor */
	IHM_ELEMENT_TRIANGLE_DESC();
 	/** Destructor */
    ~IHM_ELEMENT_TRIANGLE_DESC();

   	/** For singleton : instance */
	static IHM_ELEMENT_TRIANGLE_DESC*	_pInstance;
	/** For singleton : Has been destroyed once ? */
	static bool							_bDestroyedOnce;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
