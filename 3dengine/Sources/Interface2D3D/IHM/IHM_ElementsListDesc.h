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
//	CLASS:	IHM_ELEMENTS_LIST_DESC
//	The IHM_ELEMENTS_LIST_DESC class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_ELEMENTS_LIST_DESC : public DESC
{
public:

	enum
	{
		GET_NBR_ELEMENTS = 1,
		GET_ELEMENT,
		GET_ACTIVE_ELEMENT,
		GET_ACTIVE_ELEMENT_INDEX,
		SET_ACTIVE_ELEMENT,
		SET_ACTIVE_ELEMENT_INDEX,
		ADD_ELEMENT,
		SET_ELEMENT_OPENING_PAGE,
		IS_VISIBLE,
		SHOW,
		HIDE,
		SET_SWITCHING_SOUND,
		SET_ACTION_SOUND,
		SWAP_ELEMENTS,
		REMOVE_ELEMENT_INDEX,
		REMOVE_ELEMENT,
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

	bool	OnGetNbrElements(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetElement(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetActiveElement(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetActiveElementIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActiveElement(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActiveElementIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnAddElement(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetElementOpeningPage(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnIsVisible(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnShow(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnHide(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetSwitchingSound(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActionSound(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSwapElements(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnRemoveElementIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnRemoveElement(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	

private:

	/** Constructor */
	IHM_ELEMENTS_LIST_DESC();
 	/** Destructor */
   ~IHM_ELEMENTS_LIST_DESC();

   	/** For singleton : instance */
	static IHM_ELEMENTS_LIST_DESC*	_pInstance;
	/** For singleton : Has been destroyed once ? */
	static bool						_bDestroyedOnce;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
