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
//	CLASS:	IHM_PAGE_DESC
//	The IHM_PAGE_DESC class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_PAGE_DESC : public DESC
{
public :

	enum
	{
		GET_PARENT_PAGE = 1,
		SET_ACTIVE_ELEMENTS_LIST_INDEX,
		SET_ACTIVE_ELEMENTS_LIST,
		GET_NBR_ELEMENTS_LISTS,
		GET_ACTIVE_ELEMENTS_LIST_INDEX,
		GET_ACTIVE_ELEMENTS_LIST,
		ADD_ELEMENTS_LIST,
		GET_ELEMENTS_LIST,
		IS_VISIBLE,
		SHOW,
		HIDE,
		MAX_COMMANDS,
		NB_COMMANDS	=  (MAX_COMMANDS - 1),
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

	bool	OnGetParentPage(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActiveElementsListIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActiveElementsList(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetNbrElementsLists(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetActiveElementsListIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetActiveElementsList(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnAddElementsList(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetElementsList(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnIsVisible(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnShow(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnHide(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;


private:

	/** Constructor */
	IHM_PAGE_DESC();
 	/** Destructor */
   ~IHM_PAGE_DESC();

   	/** For singleton : instance */
	static IHM_PAGE_DESC*	_pInstance;
	/** For singleton : Has been destroyed once ? */
	static bool				_bDestroyedOnce;
};
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
