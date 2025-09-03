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
//	CLASS:	IHM_ELEMENT_ABC_DESC
//	The IHM_ELEMENT_ABC_DESC class implements ...
//
//	01-09-03:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class IHM_ELEMENT_ABC;

class	IHM_ELEMENT_ABC_DESC : public DESC
{
public:

	enum
	{
		SET_OPENING_PAGE = 1,
		ADD_LEFT_RIGHT_ELEMENT,
		GET_OPENING_PAGE,
		IS_VISIBLE,
		SHOW,
		HIDE,
		SET_OPENING_PAGE_SOUND,
		SET_OPENING_PAGE_WITH_SOUND,
		SET_LEFT_RIGHT_SOUND,
		ON_SET_ACTIVE,
		ON_SET_INACTIVE,
		IS_ACTIVE,
		ON_MOUSE_IN,
		ON_MOUSE_OUT,
		SET_SCROLL_UP_BUTTON_FOR,
		SET_SCROLL_DOWN_BUTTON_FOR,
		SET_ALWAYS_INACTIVE,
		ON_MOUSE_CLICK,
		ON_MOUSE_MOVE,
		DISPLAY_ELEMENT,
		SET_ALWAYS_ON_TOP,
		SET_BLINKING,
		SET_BLINKING_TIME,
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

	bool	OnSetOpeningPage(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnAddLeftRightElement(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnGetOpeningPage(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnIsVisible(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnShow(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnHide(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetOpeningPageSound(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetOpeningPageWithSound(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetLeftRightSound(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetActive(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetInactive(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnIsActive(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnMouseIn(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnMouseOut(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetScrollUpButtonFor(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetScrollDownButtonFor(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetAlwaysInactive(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnMouseClick(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnMouseMove(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnDisplayElement(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetAlwaysOnTop(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetBlinking(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetBlinkingTime(IHM_ELEMENT_ABC *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;


	/** Constructor */
	         IHM_ELEMENT_ABC_DESC(OBJECT_TYPE Type, const char *pcName, bool bIsSingleton, int nNbrCommands, const char * const pcCommand);
	/** Destructor */
	virtual ~IHM_ELEMENT_ABC_DESC();


private:

	/** Private constructor since IHM_ELEMENT_ABC_DESC is the DESC of an abstract class. */
   	IHM_ELEMENT_ABC_DESC();
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
