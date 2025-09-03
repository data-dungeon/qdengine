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
//	CLASS:	IHM_INTERFACE_DESC
//	The IHM_INTERFACE_DESC class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_INTERFACE_DESC : public DESC
{
public:

	enum
	{
		ADD_PAGE = 1,
		SET_ACTIVE_PAGE_INDEX,
		SET_ACTIVE_PAGE,
		GET_NBR_PAGES,
		GET_ACTIVE_PAGE_INDEX,
		GET_ACTIVE_PAGE,
		DISPLAY,
		ACTIVATE,
		DISACTIVATE,
		CLEAR_ALL,
		IS_ACTIVATED,
		SET_BACKGROUND_PICTURE,
		IS_VISIBLE,
		SHOW,
		HIDE,
		ACTIVATE_INPUT,
		ARE_INPUT_ACTIVATED,
		SET_VIEWPORT,
		MAX_COMMANDS,
		NB_COMMANDS	 = (MAX_COMMANDS - 1),
	};


	/** For singleton : instance */
	static DESC *		Instance(); 
	/** For singleton : suicide */
	static void			Suicide(); 
	
	/** Derivation from DESC_ABC: Receive command processing */
	bool				ReceiveCommandProc(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;

	/** Derivation from DESC_ABC: Create a new object */
	COM_OBJECT *		New();
	void				Delete(COM_OBJECT *);


protected:

	bool	OnAddPage(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActivePageIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActivePage(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetNbrPages(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetActivePageIndex(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetActivePage(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnDisplay(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnActivate(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnDisactivate(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnClearAll(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnIsActivated(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	SetBackgroundPicture(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnIsVisible(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnShow(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnHide(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnActivateInput(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnAreInputActivated(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetViewport(void *pObject, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;


private:

	/** Constructor */
	IHM_INTERFACE_DESC();
 	/** Destructor */
   ~IHM_INTERFACE_DESC();

   	/** For singleton : instance */
	static IHM_INTERFACE_DESC*	_pInstance;
	/** For singleton : Has been destroyed once ? */
	static bool					_bDestroyedOnce;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
