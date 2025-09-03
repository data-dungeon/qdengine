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
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_3DENGINE(M3D)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(OldCom/Communication)


//=============================================================================
//	CODE STARTS HERE
//=============================================================================
IHM_INTERFACE_DESC*	IHM_INTERFACE_DESC::_pInstance      = NULL;
bool				IHM_INTERFACE_DESC::_bDestroyedOnce = false;
static const char * const	pcComment = "An interface is the highest container in the IHM hierachy. An interface contains pages.";

//-----------------------------------------------------------------------------
//	Name: MATH_DESC constructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_INTERFACE_DESC::IHM_INTERFACE_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_INTERFACE, "ihm_interface", false, NB_COMMANDS, pcComment )
{
	// AddPage
	_pCommandDescsTab[0].Init(ADD_PAGE, 1, "AddPage", "Add a page into the interface.");
	_pCommandDescsTab[0].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "PageToAdd", "The page you want to add into the interface." );

	// SetActivePageIndex
	_pCommandDescsTab[1].Init(SET_ACTIVE_PAGE_INDEX, 1, "SetActivePageIndex", "Set the active page of the interface using its index.");
	_pCommandDescsTab[1].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "nActivePageIndex", "The zero-based index of the page you want to want to be active." );

	// SetActivePage
	_pCommandDescsTab[2].Init(SET_ACTIVE_PAGE, 1, "SetActivePage", "Set the active page of the interface. The page should already be in the interface. If not, the command will be ignored.");
	_pCommandDescsTab[2].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ActivePage", "The page you want to be active" );

	// GetNbrPages
	_pCommandDescsTab[3].Init(GET_NBR_PAGES, 1, "GetNbrPages", "Returns the number of pages in the interface.");
	_pCommandDescsTab[3].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::OUT_PARAMETER, "nNbrPages");

	// GetActivePageIndex
	_pCommandDescsTab[4].Init(GET_ACTIVE_PAGE_INDEX, 2, "GetActivePageIndex", "Return the index (position) of the active page of the interface.");
	_pCommandDescsTab[4].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::OUT_PARAMETER, "nActivePageIndex", "The index of the active page");
	_pCommandDescsTab[4].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bHasAnActivePage", "'true' if the interface currently have an active page." );

	// GetActivePage
	_pCommandDescsTab[5].Init(GET_ACTIVE_PAGE, 2, "GetActivePage", "Returns the active page.");
	_pCommandDescsTab[5].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "ActivePage",		"The current active page.");
	_pCommandDescsTab[5].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bHasAnActivePage",	"'true' if the interface currently have an active page." );

	// Display
	_pCommandDescsTab[6].Init(DISPLAY, 0, "Display", "Display the interface.");

	// Activate
	_pCommandDescsTab[7].Init(ACTIVATE, 0, "Activate", "Add the interface to the interface manager. If an interface isn't activated, it can't be visible nor receive inputs.");

	// Disactivate
	_pCommandDescsTab[8].Init(DISACTIVATE, 0, "Disactivate", "Remove the interface from the interface manager.");

	// ClearAll
	_pCommandDescsTab[9].Init(CLEAR_ALL, 0, "ClearAll", "Remove all objects in the interface.");

	// IsActivated
	_pCommandDescsTab[10].Init(IS_ACTIVATED, 1, "IsActivated", "Test if an interface is activated.");
	_pCommandDescsTab[10].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bIsActivated", "'true' if the interface is activated.");

	// SetBackgroundPicture
	_pCommandDescsTab[11].Init(SET_BACKGROUND_PICTURE, 1, "SetBackgroundPicture", "Set a background picture (if you want one).");
	_pCommandDescsTab[11].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Bitmap", "The background picture (should be a texture_new).");

	// IsVisible
	_pCommandDescsTab[12].Init(IS_VISIBLE, 1, "IsVisible", "Test if an interface is visible.");
	_pCommandDescsTab[12].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bIsVisible", "'true' if the interface is visible.");

	// Show
	_pCommandDescsTab[13].Init(SHOW, 0, "Show", "Show the interface (but don't activate it).");

	// Hide
	_pCommandDescsTab[14].Init(HIDE, 0, "Hide", "Hide the interface (but don't disactivate it).");

	// ActivateInput
	_pCommandDescsTab[15].Init(ACTIVATE_INPUT, 1, "ActivateInput", "Active inputs. If set to false, no inputs are handled. By default, inputs are activated.");
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::IN_PARAMETER, "bActivate", "'true' if the inputs have to be handled.");

	// AreInputActivated
	_pCommandDescsTab[16].Init(ARE_INPUT_ACTIVATED, 1, "AreInputActivated", "Returns true if the inputs are activated on the current interface.");
	_pCommandDescsTab[16].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bActivated", "'true' if the inputs are activated.");

	// SetViewport
	_pCommandDescsTab[17].Init(SET_VIEWPORT, 1, "SetViewport", "Set the viewport in which the interface has to be displayed. Please note that if a element will be displayed its own viewport (if it has one) prior to its interface viewport.");
	_pCommandDescsTab[17].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID,   1, PARAMETER_DESC::IN_PARAMETER, "Viewport", "The viewport in which the interface and its element are displayed");
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC destructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_INTERFACE_DESC::~IHM_INTERFACE_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::New
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	IHM_INTERFACE_DESC::New()
{
	_nNbObjects++;

	return (new IHM_INTERFACE);
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Delete
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE_DESC::Delete(COM_OBJECT *	pC)
{
	_nNbObjects--;

	delete pC;
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Instance
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
DESC* IHM_INTERFACE_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = new IHM_INTERFACE_DESC();
	}
	
	return ( _pInstance );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Suicide
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void IHM_INTERFACE_DESC::Suicide()
{
	QDT_ASSERT( _bDestroyedOnce == false );
	QDT_ASSERT( _pInstance != NULL );
	
	if ( _pInstance != NULL )
	{
		delete _pInstance;
		_pInstance		 = NULL;
		_bDestroyedOnce	 = false;
	}
}


//-----------------------------------------------------------------------------
//	Name:		OnAddPage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnAddPage(void *			pObject,
									  COMMAND_ID		CommandID,
									  PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*> (pObject);	


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID(0);
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_PAGE )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::OnAddPage: object is not of type OTYPE_MOTEUR3D_IHM_PAGE !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pPageIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pPageIndexInt );
	if ( pPageIndexInt == NULL )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::OnAddPage: can't get an IHM_PAGE from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid page.
	IHM_PAGE	*pPage = static_cast<IHM_PAGE*>( pPageIndexInt->GetObject() );
	QDT_ASSERT( pPage );
	if ( ( pPage == NULL ) && ( pPageIndexInt->IsLoaded() == false ) )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::OnAddPage: page invalid or not loaded !");
		return( false );
	}


	pInterface->AddPage( pPage );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetActivePage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnSetActivePageIndex(void *				pObject,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );


	// Valid page index.
	QDT_ASSERT( Stack.GetInteger( 0 ) >= 0 );
	QDT_ASSERT( Stack.GetInteger( 0 ) <  pInterface->GetNbrPages() );
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) >= pInterface->GetNbrPages() ) )
	{
		QDT_Error( "In IHM_INTERFACE_DESC::OnSetActivePageIndex: invalid index %i.", Stack.GetInteger(0) );
		return ( false );
	}


	pInterface->SetActivePage( Stack.GetInteger( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetActivePage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnSetActivePage(void *				pObject,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID(0);	
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_PAGE )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::OnSetActivePage: object is not of type OTYPE_MOTEUR3D_IHM_PAGE !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pPageIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pPageIndexInt );
	if ( pPageIndexInt == NULL )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::OnSetActivePage: can't get an IHM_PAGE from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid page.
	IHM_PAGE	*pPage = static_cast<IHM_PAGE*>( pPageIndexInt->GetObject() );
	QDT_ASSERT( pPage );
	if ( ( pPage == NULL ) || ( pPageIndexInt->IsLoaded() == false ) )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::OnSetActivePage: page invalid or not loaded !");
		return( false );
	}


	pInterface->SetActivePage( pPage );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetNbrPage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnGetNbrPages(void *			pObject,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );


	Stack.SetInteger( 0, pInterface->GetNbrPages() );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetActivePageIndex
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnGetActivePageIndex(void *				pObject,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );


	int nActivePageIndex = pInterface->GetActivePageIndex();

	// Interface has an active page.
	if ( nActivePageIndex >= 0 )
	{
		QDT_ASSERT( nActivePageIndex < pInterface->GetNbrPages() );

		Stack.SetBool( 1, true );
	}
	// Interface has no active page.
	else
	{
		QDT_ASSERT( nActivePageIndex == -1 );

		Stack.SetBool( 1, false );
	}


	Stack.SetInteger( 0, nActivePageIndex );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetActivePage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnGetActivePage(void *				pObject,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	INDEX_INT	*pIndex;
	OBJECT_ID	ObjectId;


	// Dereference the dyn one given from the script.
	ObjectId	= Stack.GetObjectID( 0 );
	pIndex		= DATABASE_MANAGER::Instance()->GetById( ObjectId.GetType(), ObjectId.GetID() );
	QDT_ASSERT( pIndex )
	pIndex->RemoveReference();


	IHM_PAGE *pActivePage = pInterface->GetActivePage();

	// Interface has an active page.
	if ( pActivePage != NULL )
	{
		QDT_ASSERT( pActivePage->GetIndex() );

		pIndex		= pActivePage->GetIndex();
		ObjectId	= pIndex->GetObjectId();

		pIndex->AddReference();

		Stack.SetBool( 1, true );
	}
	// Interface has no active page.
	else
	{
		ObjectId.SetType( 0 );
		ObjectId.SetID(   0 );

		Stack.SetBool( 1, false );
	}
	
	Stack.SetObjectID( 0, ObjectId );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnDisplay
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnDisplay(void *			pObject,
									  COMMAND_ID		CommandID,
									  PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->Display();

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnActivate
//	Object:		
//	01-09-26:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnActivate(void *				pObject,
									   COMMAND_ID			CommandID,
									   PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->Activate();

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnDisactivate
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnDisactivate(void *			pObject,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->Disactivate();

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnClearAll
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnClearAll(void *				pObject,
									   COMMAND_ID			CommandID,
									   PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->ClearAll();

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnIsActivated
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnIsActivated(void *			pObject,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	Stack.SetBool( 0, pInterface->IsActivated() );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		SetBackgroundPicture
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::SetBackgroundPicture(void *				pObject,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );


	OBJECT_ID Id = Stack.GetObjectID( 0 );

	// Valid type.
	if ( Id.GetType() != OTYPE_KERNEL_TEXTURE_NEW )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::SetBackgroundPicture: object is not of type OTYPE_KERNEL_TEXTURE_NEW !");
		return false;
	}

	// Valid INDEX_INT.
	INDEX_INT *pIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pIndexInt );
	if ( pIndexInt == NULL )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::SetBackgroundPicture: can't get a TEXTURE_NEW from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}

	// Valid page.
	TEXTURE_NEW	*pTextureNew = static_cast<TEXTURE_NEW*>( pIndexInt->GetObject() );
	QDT_ASSERT( pTextureNew );
	if ( pTextureNew == NULL )
	{
		QDT_Warning("In IHM_INTERFACE_DESC::SetBackgroundPicture: invalid bitmap !");
		return( false );
	}


	pInterface->SetBackgroundPicture( pTextureNew );


	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnIsVisible
//	Object:		
//	01-10-12:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnIsVisible(void *				pObject,
										COMMAND_ID			CommandID,
										PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	Stack.SetBool( 0, pInterface->IsVisible() );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnShow
//	Object:		
//	02-02-14:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnShow(void *				pObject,
								   COMMAND_ID			CommandID,
								   PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->Show();

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnHide
//	Object:		
//	02-02-14:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnHide(void *				pObject,
								   COMMAND_ID			CommandID,
								   PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->Hide();

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnActivateInput
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnActivateInput(void *				pObject,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	pInterface->ActivateInput( Stack.GetBool( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnAreInputActivated
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnAreInputActivated(void *				pObject,
												COMMAND_ID			CommandID,
												PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );

	Stack.SetBool( 0, pInterface->AreInputActivated() );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetViewport
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::OnSetViewport(void *			pObject,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	OBJECT_ID		ObjectID;
	IHM_INTERFACE *	pInterface;
	INDEX_INT *		pIndexInt;
	VIEWPORT *		pViewport;
	
	pInterface	= static_cast<IHM_INTERFACE*>( pObject );
	
	ObjectID	= Stack.GetObjectID( 0 );
	if( ObjectID.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_VIEWPORT )
	{
		QDT_Warning( "ihm_interface::SetViewport : parameter is not a viewport." );
		return ( false );
	}

	pIndexInt	= DATABASE_MANAGER::Instance()->GetById( ObjectID.GetType(), ObjectID.GetID() );
	if ( pIndexInt == NULL )
	{
		QDT_Warning( "ihm_interface::SetViewport : can't retrieve viewport INDEX_INT." );
		return ( false );
	}

	pViewport	= static_cast<VIEWPORT*>( pIndexInt->GetObject() );
	if ( pViewport == NULL )
	{
		QDT_Warning( "ihm_interface::SetViewport : can't retrieve viewport from INDEX_INT." );
		return ( false );
	}

	pInterface->SetViewport( pViewport );

	return ( true );
}




//-----------------------------------------------------------------------------
//	Name: MATH_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE_DESC::ReceiveCommandProc(void *				pObject,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	IHM_INTERFACE *pInterface = static_cast<IHM_INTERFACE*>( pObject );
	QDT_ASSERT( pInterface );
	if ( !pInterface )
	{
		return ( false );
	}


	switch(CommandID)
	{
	// Add a page into the interface.
	case ADD_PAGE:
		return ( OnAddPage( pObject, CommandID, Stack ) );

	// Sets the active page of the interface unsing its position.
	case SET_ACTIVE_PAGE_INDEX:
		return ( OnSetActivePageIndex( pObject, CommandID, Stack ) );

	// Sets the active page of the interface.
	case SET_ACTIVE_PAGE:
		return ( OnSetActivePage( pObject, CommandID, Stack ) );

	// Returns the number of pages in the interface.
	case GET_NBR_PAGES:
		return ( OnGetNbrPages( pObject, CommandID, Stack ) );

	// Return the index (position) of the active page of the interface.
	case GET_ACTIVE_PAGE_INDEX:
		return ( OnGetActivePageIndex( pObject, CommandID, Stack ) );

	// Returns the active page.
	case GET_ACTIVE_PAGE:
		return ( OnGetActivePage( pObject, CommandID, Stack ) );

	// Display the interface.
	case DISPLAY:
		return ( OnDisplay( pObject, CommandID, Stack ) );

	// Make the interface visible et activate it.
	case ACTIVATE:
		return ( OnActivate( pObject, CommandID, Stack ) );

	// Disactivate the interface (make it invisible).
	case DISACTIVATE: 
		return ( OnDisactivate( pObject, CommandID, Stack ) );

	// Remove all objects in the interface.
	case CLEAR_ALL:
		return ( OnClearAll( pObject, CommandID, Stack ) );

	// Retrun 'true' if the interface is activated.
	case IS_ACTIVATED:
		return( OnIsActivated( pObject, CommandID, Stack ) );

	// Set a backgroundpicture (if you want one).
	case SET_BACKGROUND_PICTURE:
		return ( SetBackgroundPicture( pObject, CommandID, Stack ) );

	// Return true if all the interface is visible.
	case IS_VISIBLE:
		return( OnIsVisible( pObject, CommandID, Stack ) );

	// Show the interface (but don't activate it).
	case SHOW:
		return ( OnShow( pObject, CommandID, Stack ) );

	// Hide the interface (but don't disactivate it).
	case HIDE:
		return ( OnHide( pObject, CommandID, Stack ) );

	// Active inputs. If set to false, no inputs are handled.
	case ACTIVATE_INPUT:
		return ( OnActivateInput( pObject, CommandID, Stack ) );

	// Returns true if the inputs are activated on the current interface.");
	case ARE_INPUT_ACTIVATED:
		return ( OnAreInputActivated( pObject, CommandID, Stack ) );

	// Set the viewport in which the interface has to be displayed
	case SET_VIEWPORT:
		return ( OnSetViewport( pObject, CommandID, Stack ) );

	// Default.
	default:
		return ( DefaultCommand( pInterface, CommandID, Stack ) );
	}
}
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
