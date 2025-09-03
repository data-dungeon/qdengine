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
//	CLASS:	IHM_ELEMENT_BITMAP_DESC
//	The IHM_ELEMENT_BITMAP_DESC class implements ...
//
//	01-08-20:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

class	IHM_ELEMENT_BITMAP;

class	IHM_ELEMENT_BITMAP_DESC : public IHM_ELEMENT_RECT_DESC
{
public:

	enum
	{
		SET_BITMAP = IHM_ELEMENT_RECT_DESC::MAX_COMMANDS,
		SET_CROPPING_COORDINATES,
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

	bool	OnSetBitmap(IHM_ELEMENT_BITMAP *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;
	bool	OnSetCroppingCoordinates(IHM_ELEMENT_BITMAP *pElement, COMMAND_ID CommandID, PARAMETER_STACK &Stack) const;


private:

	/** Constructor */
			IHM_ELEMENT_BITMAP_DESC();
 	/** Destructor */
	virtual ~IHM_ELEMENT_BITMAP_DESC();

   	/** For singleton : instance */
	static IHM_ELEMENT_BITMAP_DESC*	_pInstance;
	/** For singleton : Has been destroyed once ? */
	static bool						_bDestroyedOnce;
};

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
