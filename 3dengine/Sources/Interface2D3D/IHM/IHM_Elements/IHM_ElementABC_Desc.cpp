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
//
//	01-09-03:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_3DENGINE(M3D)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(OldCom/Communication)
#include	INC_KRNCORE(Display\RenderManager)

#ifndef _DEBUG

#endif


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_ABC_DESC constructor
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_ABC_DESC::IHM_ELEMENT_ABC_DESC(OBJECT_TYPE			Type,
										   const char *			pcName,
										   bool					bIsSingleton,
										   int					nNbrCommands,
										   const char * const	pcCommand)
:
DESC(Type, pcName, bIsSingleton, nNbrCommands, pcCommand)
{
	_pCommandDescsTab[0].Init(SET_OPENING_PAGE, 1, "SetOpeningPage", "Set the element as an opening page element and set the page to be opened when the user activates the current element.");
	_pCommandDescsTab[0].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "OpeningPage");

	_pCommandDescsTab[1].Init(ADD_LEFT_RIGHT_ELEMENT, 1, "AddLeftRightElement", "Set the element as a left-right element and add an element the user can parse trough using the left and right events.");
	_pCommandDescsTab[1].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "LeftRightElement");

	_pCommandDescsTab[2].Init(GET_OPENING_PAGE, 2, "GetOpeningPage", "Retrieve the selection page.");
	_pCommandDescsTab[2].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "OpeningPage");
	_pCommandDescsTab[2].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bIsOpeningPageElement");

	_pCommandDescsTab[3].Init(IS_VISIBLE, 1, "IsVisible", "Returns 'true' if the element is visible.");
	_pCommandDescsTab[3].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bIsVisible");

	_pCommandDescsTab[4].Init(SHOW, 0, "Show", "Show the element.");

	_pCommandDescsTab[5].Init(HIDE, 0, "Hide", "Hide the element.");

	_pCommandDescsTab[6].Init(SET_OPENING_PAGE_SOUND, 1, "SetOpeningPageSound", "If the element is an opening page element, set the sound that will be played when then user activates the current element.");
	_pCommandDescsTab[6].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "OpeningPageSound");

	_pCommandDescsTab[7].Init(SET_OPENING_PAGE_WITH_SOUND, 2, "SetOpeningPageWithSound", "Shortcut to use SetOpeningPage and SetOpeningPageSound with a single function call.");
	_pCommandDescsTab[7].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "OpeningPage");
	_pCommandDescsTab[7].SetParameterDesc(1, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "OpeningPageSound");

	_pCommandDescsTab[8].Init(SET_LEFT_RIGHT_SOUND, 1, "SetLeftRightSound", "Set the sound that has to be played when the user switches between elements. MUST BE CALLED WHEN THE ELEMENT HAS AT LEAST A LEFT-RIGHT ELEMENT.");
	_pCommandDescsTab[8].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "LeftRightSound");

	_pCommandDescsTab[9].Init(ON_SET_ACTIVE, 0, "OnSetActive", "Override this method to change the current element when getting active.");

	_pCommandDescsTab[10].Init(ON_SET_INACTIVE, 0, "OnSetInactive", "Override this method to change the current element when getting inactive.");

	_pCommandDescsTab[11].Init(IS_ACTIVE, 1, "IsActive", "Returns 'true' if the current element is active, else, returns 'false'.");
	_pCommandDescsTab[11].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,	1, PARAMETER_DESC::OUT_PARAMETER, "bIsActive");

	_pCommandDescsTab[12].Init(ON_MOUSE_IN, 0, "OnMouseIn", "Override this method to change the current element when the mouse gets on it.");

	_pCommandDescsTab[13].Init(ON_MOUSE_OUT, 0, "OnMouseOut", "Override this method to change the current element when the mouse gets out of it.");

	_pCommandDescsTab[14].Init(SET_SCROLL_UP_BUTTON_FOR, 1, "SetScrollUpButtonFor", "Attach this element to a text and use it to scroll up the attached text. Please note that if a text entirely fits in its bounding rect, there's no scroll." );
	_pCommandDescsTab[14].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ElementText");

	_pCommandDescsTab[15].Init(SET_SCROLL_DOWN_BUTTON_FOR, 1, "SetScrollDownButtonFor", "Attach this element to a text and use it to scroll down the attached text. Please note that if a text entirely fits in its bounding rect, there's no scroll." );
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ElementText");

	_pCommandDescsTab[16].Init(SET_ALWAYS_INACTIVE, 1, "SetAlwaysInactive", "If set to 'true', the element will receive no input at all. For example, use it to display bitmap on screen that should not diseappear when the user clicks on it." );
	_pCommandDescsTab[16].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bAlwaysInactive");

	_pCommandDescsTab[17].Init(ON_MOUSE_CLICK, 3, "OnMouseClick", "Override this command if you want to perform special actions when the user clicks on a element.\r\nIMPORTANT: You cannot override the 'OnMouseClick' method and set an opening page (for example). If so, a click will open the associated opening page, ignoring the 'OnMouseClick' method override." );
	_pCommandDescsTab[17].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Interface");
	_pCommandDescsTab[17].SetParameterDesc(1, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Page");
	_pCommandDescsTab[17].SetParameterDesc(2, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ElementsList");

	_pCommandDescsTab[18].Init(ON_MOUSE_MOVE, 2, "OnMouseMove", "Override this command if you want to perform special actions when the user moves the mouse." );
	_pCommandDescsTab[18].SetParameterDesc(0, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nMousePosX", "The mouse cursor horizontal position in pixel coordinates." );
	_pCommandDescsTab[18].SetParameterDesc(1, OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nMousePosY", "The mouse cursor vertical position in pixel coordinates." );

	_pCommandDescsTab[19].Init(DISPLAY_ELEMENT, 0, "DisplayElement", "Display an ihm_element into the frame buffer. WARNING: This method should be called within the 3d engine display loop to have an effect." );

	_pCommandDescsTab[20].Init(SET_ALWAYS_ON_TOP, 1, "SetAlwaysOnTop", "If set to 'true', the element is displayed after the other ones. It is therefore always visible." );
	_pCommandDescsTab[20].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bAlwaysOnTop" );

	_pCommandDescsTab[21].Init(SET_BLINKING, 1, "SetBlinking", "Make the element blink." );
	_pCommandDescsTab[21].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN, 1, PARAMETER_DESC::IN_PARAMETER, "bBlink" );
	
	_pCommandDescsTab[22].Init(SET_BLINKING_TIME, 1, "SetBlinkingTime", "The time of the blink (in ms). Default is 1 s." );
	_pCommandDescsTab[22].SetParameterDesc(0, OTYPE_KERNEL_FLOAT, 1, PARAMETER_DESC::IN_PARAMETER, "rBlinkTimeMs" );
}


//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_ABC_DESC destructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_ABC_DESC::~IHM_ELEMENT_ABC_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name:		OnSetOpeningPage
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetOpeningPage(IHM_ELEMENT_ABC *	pElement,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_PAGE )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPage: object is not of type OTYPE_MOTEUR3D_IHM_PAGE !");
		return ( false );
	}


	// Valid INDEX_INT.
	INDEX_INT *pPageIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pPageIndexInt );
	if ( pPageIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPage: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid page.
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pPageIndexInt->GetObject() );
	QDT_ASSERT( pPage );
	if ( ( pPage == NULL ) || ( pPage->IsLoaded() == false ) )
	{		
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPage: page invalid or not loaded !");
		return ( false );
	}


	pElement->SetOpeningPage( pPage );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnAddLeftRightElement
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnAddLeftRightElement(IHM_ELEMENT_ABC *	pElement,
													COMMAND_ID			CommandID,
													PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );
	if ( ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT )
	  && ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_MESH )
	  && ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP ) )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnAddLeftRightElement: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT, nor OTYPE_MOTEUR3D_IHM_ELEMENT_MESH nor OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pElementIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pElementIndexInt );
	if ( pElementIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnAddLeftRightElement: can't get an IHM_ELEMENT_ABC from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element.
	IHM_ELEMENT_ABC	*pLeftRightElement = static_cast<IHM_ELEMENT_ABC*>( pElementIndexInt->GetObject() );
	QDT_ASSERT( pLeftRightElement );
	if ( pLeftRightElement == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnAddLeftRightElement: invalid element !");
		return( false );
	}


	pElement->AddLeftRightElement( pLeftRightElement );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetOpeningPage
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnGetOpeningPage(IHM_ELEMENT_ABC *	pElement,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pOpeningPage = pElement->GetOpeningPage();
	OBJECT_ID Id;

	// Element has an opening page.
	if ( pOpeningPage != NULL )
	{
		INDEX_INT*	pIndex = pOpeningPage->GetIndex();
		Id.SetType( pIndex->GetComObject()->GetType() );
		Id.SetID(   pIndex->GetId() );

		Stack.SetBool( 1, true );
	}
	// Element has no opening page.
	else
	{
		Id.SetType( 0 );
		Id.SetID(   0 );

		Stack.SetBool( 1, false );
	}
	
	Stack.SetObjectID( 0, Id );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnIsVisible
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnIsVisible(IHM_ELEMENT_ABC *	pElement,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	Stack.SetBool( 0, pElement->IsVisible() );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnShow
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnShow(IHM_ELEMENT_ABC *	pElement,
									 COMMAND_ID			CommandID,
									 PARAMETER_STACK &	Stack) const
{
	pElement->Show();
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnHide
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnHide(IHM_ELEMENT_ABC *	pElement,
									 COMMAND_ID			CommandID,
									 PARAMETER_STACK &	Stack) const
{
	pElement->Hide();
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetOpeningPageSound
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetOpeningPageSound(IHM_ELEMENT_ABC *	pElement,
													COMMAND_ID			CommandID,
													PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );	
	if ( Id.GetType() != OTYPE_KERNEL_SOUND_DATA )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageSound: object is not of type OTYPE_KERNEL_SOUND_DATA !");
		return ( false );
	}


	// Valid INDEX_INT.
	INDEX_INT *pSoundIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pSoundIndexInt );
	if ( pSoundIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageSound: invalid INDEX_INT !");
		return( false );
	}


	// Valid page.
	SOUND_DATA	*pSoundData = static_cast<SOUND_DATA*>( pSoundIndexInt->GetObject() );
	QDT_ASSERT( pSoundData );
	if ( pSoundData == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageSound: invalid sound !" );
		return( false );
	}
	
	pElement->SetOpeningPageSound( pSoundData );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetOpeningPageSoundWithSound
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound(IHM_ELEMENT_ABC *	pElement,
														COMMAND_ID			CommandID,
														PARAMETER_STACK &	Stack) const
{
	// Locals.
	OBJECT_ID Id;
	INDEX_INT *pIndexInt;


	// The page.

	// Valid type.
	Id = Stack.GetObjectID( 0 );
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_PAGE )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENT_PAGE !");
		return ( false );
	}


	// Valid INDEX_INT.
	pIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pIndexInt );
	if ( pIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid page.
	IHM_PAGE	*pPage = static_cast<IHM_PAGE*>( pIndexInt->GetObject() );
	QDT_ASSERT( pPage );
	if ( ( pPage == NULL ) || ( pPage->IsLoaded() == false ) )
	{		
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound: page invalid or not loaded !");
		return ( false );
	}



	// The sound.

	// Valid type.
	Id = Stack.GetObjectID( 1 );	
	if ( Id.GetType() != OTYPE_KERNEL_SOUND_DATA )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound: object is not of type OTYPE_KERNEL_SOUND_DATA !");
		return ( false );
	}


	// Valid INDEX_INT.
	pIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pIndexInt );
	if ( pIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound: invalid INDEX_INT !");
		return( false );
	}


	// Valid sound.
	SOUND_DATA	*pSoundData = static_cast<SOUND_DATA*>( pIndexInt->GetObject() );
	QDT_ASSERT( pSoundData );
	if ( pSoundData == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetOpeningPageWithSound: invalid sound !" );
		return( false );
	}



	pElement->SetOpeningPage( pPage, pSoundData );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetLeftRightSound
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetLeftRightSound(IHM_ELEMENT_ABC *	pElement,
												  COMMAND_ID		CommandID,
												  PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );	
	if ( Id.GetType() != OTYPE_KERNEL_SOUND_DATA )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetLeftRightSound: object is not of type OTYPE_KERNEL_SOUND_DATA !");
		return ( false );
	}


	// Valid INDEX_INT.
	INDEX_INT *pSoundIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pSoundIndexInt );
	if ( pSoundIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENT_ABC_DESC::OnSetLeftRightSound: invalid INDEX_INT !");
		return( false );
	}


	// Valid page.
	SOUND_DATA	*pSoundData = static_cast<SOUND_DATA*>( pSoundIndexInt->GetObject() );
	QDT_ASSERT( pSoundData );
	if ( pSoundData == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetLeftRightSound: invalid sound !" );
		return( false );
	}
	

	pElement->SetLeftRightSound( pSoundData );

	return( true );	
}


//-----------------------------------------------------------------------------
//	Name:		OnSetActive
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetActive(IHM_ELEMENT_ABC *	pElement,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetInactive
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetInactive(IHM_ELEMENT_ABC *	pElement,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnIsActive
//	Object:		
//	01-10-05:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnIsActive(IHM_ELEMENT_ABC *	pElement,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	Stack.SetBool( 0, pElement->IsActive() );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnMouseIn
//	Object:		
//	01-10-12:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnMouseIn(IHM_ELEMENT_ABC *	pElement,
										COMMAND_ID			CommandID,
										PARAMETER_STACK &	Stack) const
{
	return ( true );
}

	
//-----------------------------------------------------------------------------
//	Name:		OnMouseOut
//	Object:		
//	01-10-12:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnMouseOut(IHM_ELEMENT_ABC *	pElement,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetScrollUpButtonFor
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor(IHM_ELEMENT_ABC *	pElement,
													 COMMAND_ID			CommandID,
													 PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pIndexInt );
	if ( pIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor: can't get an IHM_ELEMENT_ABC from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element.
	IHM_ELEMENT_TEXT *pTextElement = static_cast<IHM_ELEMENT_TEXT*>( pIndexInt->GetObject() );
	QDT_ASSERT( pTextElement );
	if ( pTextElement == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor: invalid element !");
		return( false );
	}


	pElement->SetScrollUpButtonFor( pTextElement );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetScrollDownButtonFor
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetScrollDownButtonFor(IHM_ELEMENT_ABC *	pElement,
													   COMMAND_ID			CommandID,
													   PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pIndexInt );
	if ( pIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor: can't get an IHM_ELEMENT_ABC from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element.
	IHM_ELEMENT_TEXT *pTextElement = static_cast<IHM_ELEMENT_TEXT*>( pIndexInt->GetObject() );
	QDT_ASSERT( pTextElement );
	if ( pTextElement == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetScrollUpButtonFor: invalid element !");
		return( false );
	}


	pElement->SetScrollDownButtonFor( pTextElement );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetAlwaysInactive
//	Object:		
//	01-12-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetAlwaysInactive(IHM_ELEMENT_ABC *pElement,
												  COMMAND_ID		CommandID,
												  PARAMETER_STACK &	Stack) const
{
	pElement->SetAlwaysInactive( Stack.GetBool( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnMouseClick
//	Object:		
//	01-12-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnMouseClick(IHM_ELEMENT_ABC *	pElement,
										   COMMAND_ID			CommandID,
										   PARAMETER_STACK &	Stack) const
{
	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_INTERFACE )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetAlwaysInactive: object is not of type OTYPE_MOTEUR3D_IHM_INTERFACE !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pIndexInt );
	if ( pIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetAlwaysInactive: can't get an IHM_INTERFACE from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element.
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pIndexInt->GetObject() );
	QDT_ASSERT( pInterface );
	if ( pInterface == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_ABC_DESC::OnSetAlwaysInactive: invalid interface !");
		return( false );
	}


	// Default processing for a click on an element is to disactivate the interface.
	pInterface->Disactivate();


	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnMouseMove
//	Object:		
//	01-12-10:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnMouseMove(IHM_ELEMENT_ABC *	pElement,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnDisplayElement
//	Object:		
//	01-12-12:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnDisplayElement(IHM_ELEMENT_ABC *	pElement,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	pElement->DisplayElement();

	return ( true );	
}


//-----------------------------------------------------------------------------
//	Name:		OnSetAlwaysOnTop
//	Object:		
//	01-12-14:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetAlwaysOnTop(IHM_ELEMENT_ABC *	pElement,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	pElement->SetAlwaysOnTop( Stack.GetBool( 0 ) );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		OnSetBlinking
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetBlinking(IHM_ELEMENT_ABC *	pElement,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	pElement->SetBlinking( Stack.GetBool( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetBlinkingTime
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::OnSetBlinkingTime(IHM_ELEMENT_ABC *	pElement,
												COMMAND_ID			CommandID,
												PARAMETER_STACK &	Stack) const
{
	pElement->SetBlinkingTime( Stack.GetFloat( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC_DESC::ReceiveCommandProc(void *				pObject,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENT_ABC *pElement = static_cast<IHM_ELEMENT_ABC*> (pObject);	
	QDT_ASSERT( pElement );
	if ( !pElement )
	{
		return ( false );
	}

	switch(CommandID)
	{
	// Set the element as an opening page element and set the page to be opened when the user activates the current element.
	case SET_OPENING_PAGE:
		return( OnSetOpeningPage( pElement, CommandID, Stack ) );

	// Set the element as a left-right element and add an element the user can parse trough using the left and right events.
	case ADD_LEFT_RIGHT_ELEMENT:
		return( OnAddLeftRightElement( pElement, CommandID, Stack ) );

	//Retrieve the selection page.
	case GET_OPENING_PAGE:
		return( OnGetOpeningPage( pElement, CommandID, Stack ) );

	// Returns 'true' if the element is visible.
	case IS_VISIBLE:
		return( OnIsVisible( pElement, CommandID, Stack ) );

	// Show the element.
	case SHOW:
		return( OnShow( pElement, CommandID, Stack ) );

	// Hide the element.
	case HIDE:
		return( OnHide( pElement, CommandID, Stack ) );

	// If the element is an opening page element, set the sound that will be played when then user activates the current element.
	case SET_OPENING_PAGE_SOUND:
		return ( OnSetOpeningPageSound( pElement, CommandID, Stack ) );

	// Shortcut to use SetOpeningPage and SetOpeningPageSound with a single function call.
	case SET_OPENING_PAGE_WITH_SOUND:
		return ( OnSetOpeningPageWithSound( pElement, CommandID, Stack ) );

	// Set the sound that has to be played when the user switches between elements.
	case SET_LEFT_RIGHT_SOUND:
		return ( OnSetLeftRightSound( pElement, CommandID, Stack ) );

	// Override this method to change the current element when getting active.
	case ON_SET_ACTIVE:
		return ( OnSetActive( pElement, CommandID, Stack ) );

	// Override this method to change the current element when getting inactive.
	case ON_SET_INACTIVE:
		return ( OnSetInactive( pElement, CommandID, Stack ) );

	// Returns 'true' if the current element is active, else, returns 'false'.
	case IS_ACTIVE:
		return ( OnIsActive( pElement, CommandID, Stack ) );

	// Override this method to change the current element when the mouse gets on it.
	case ON_MOUSE_IN:
		return ( OnMouseIn( pElement, CommandID, Stack ) );

	// Override this method to change the current element when the mouse gets out of it.
	case ON_MOUSE_OUT:
		return ( OnMouseOut( pElement, CommandID, Stack ) );

	// Attach this element to a text and use it to scroll up the attached text.
	case SET_SCROLL_UP_BUTTON_FOR:
		return ( OnSetScrollUpButtonFor( pElement, CommandID, Stack ) );

	// Attach this element to a text and use it to scroll down the attached text.
	case SET_SCROLL_DOWN_BUTTON_FOR:
		return ( OnSetScrollDownButtonFor( pElement, CommandID, Stack ) );

	// If set to 'true', the element will receive no input at all.
	case SET_ALWAYS_INACTIVE:
		return ( OnSetAlwaysInactive( pElement, CommandID, Stack ) );

	// Override this command if you want to perform special actions when the user clicks on a element.
	case ON_MOUSE_CLICK:
		return ( OnMouseClick( pElement, CommandID, Stack ) );

	// Override this command if you want to perform special actions when the user moves the mouse." );
	case ON_MOUSE_MOVE:
		return ( OnMouseMove( pElement, CommandID, Stack ) );

	// Display an ihm_element into the frame buffer.
	case DISPLAY_ELEMENT:
		return ( OnDisplayElement( pElement, CommandID, Stack ) );

	// If set to 'true', the element is displayed after the other ones
	case SET_ALWAYS_ON_TOP:
		return ( OnSetAlwaysOnTop( pElement, CommandID, Stack ) );

	// Make the element blink.
	case SET_BLINKING:
		return ( OnSetBlinking( pElement, CommandID, Stack ) );

	// The time of the blink (in ms).
	case SET_BLINKING_TIME:
		return ( OnSetBlinkingTime( pElement, CommandID, Stack ) );

	// Default.
	default:
		return ( DefaultCommand( pElement, CommandID, Stack ) );
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
