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

IHM_PAGE_DESC*	IHM_PAGE_DESC::_pInstance      = NULL;
bool			IHM_PAGE_DESC::_bDestroyedOnce = false;
static const char * const	pcComment = "A page contains pages elements lists.";

//-----------------------------------------------------------------------------
//	Name: MATH_DESC constructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_PAGE_DESC::IHM_PAGE_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_PAGE, "ihm_page", false, NB_COMMANDS, pcComment)
{
	_pCommandDescsTab[0].Init(GET_PARENT_PAGE, 1, "GetParentPage", "Returns the parent page of the current page.");
	_pCommandDescsTab[0].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ParentPage");

	_pCommandDescsTab[1].Init(SET_ACTIVE_ELEMENTS_LIST_INDEX, 1, "SetActiveElementsListIndex", "Set the active elements list of the page using its position.");
	_pCommandDescsTab[1].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "ActiveElementsList");

	_pCommandDescsTab[2].Init(SET_ACTIVE_ELEMENTS_LIST, 1, "SetActiveElementsList", "Set the active elements list of the page.");
	_pCommandDescsTab[2].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "nActiveElementsListIndex");

	_pCommandDescsTab[3].Init(GET_NBR_ELEMENTS_LISTS, 1, "GetNbrElementsLists", "Returns the number of elements list in the interface.");
	_pCommandDescsTab[3].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::OUT_PARAMETER, "nNbrElementsList");

	_pCommandDescsTab[4].Init(GET_ACTIVE_ELEMENTS_LIST_INDEX, 2, "GetActiveElementsListIndex", "Return the index (position) of the active elements list of the interface.");
	_pCommandDescsTab[4].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::OUT_PARAMETER, "nActiveElementsListIndex");
	_pCommandDescsTab[4].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bHasAnActiveElementsList");

	_pCommandDescsTab[5].Init(GET_ACTIVE_ELEMENTS_LIST, 2, "GetActiveElementsList", "Returns the active elements list.");
	_pCommandDescsTab[5].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "ActiveElementsList");
	_pCommandDescsTab[5].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bHasAnActiveElementsList");

	_pCommandDescsTab[6].Init(ADD_ELEMENTS_LIST, 1, "AddElementsList", "Add an elements list into the current page.");
	_pCommandDescsTab[6].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ElementsListToAdd");

	_pCommandDescsTab[7].Init(GET_ELEMENTS_LIST, 2, "GetElementsList", "Get a specific elements list.");
	_pCommandDescsTab[7].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "nElementsListIndex");
	_pCommandDescsTab[7].SetParameterDesc(1, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "ElementsList");

	_pCommandDescsTab[8].Init(IS_VISIBLE, 1, "IsVisible", "Returns 'true' if the elements list is visible.");
	_pCommandDescsTab[8].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bIsVisible");

	_pCommandDescsTab[9].Init(SHOW, 0, "Show", "Show the elements list.");

	_pCommandDescsTab[10].Init(HIDE, 0, "Hide", "Hide the elements list.");

}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC destructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_PAGE_DESC::~IHM_PAGE_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::New
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	IHM_PAGE_DESC::New()
{
	_nNbObjects++;

	return (new IHM_PAGE);
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Delete
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE_DESC::Delete(COM_OBJECT *	pC)
{
	_nNbObjects--;

	delete pC;
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Instance
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
DESC* IHM_PAGE_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = new IHM_PAGE_DESC();
	}
	
	return ( _pInstance );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Suicide
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void IHM_PAGE_DESC::Suicide()
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
//	Name:		OnGetParentPage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnGetParentPage(void *				pObject,
									   COMMAND_ID			CommandID,
									   PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );

	OBJECT_ID	ObjectId;
	INDEX_INT	*pIndex;


	// Dereference the dyn one given from the script.
	ObjectId	= Stack.GetObjectID( 0 );
	pIndex		= DATABASE_MANAGER::Instance()->GetById( ObjectId.GetType(), ObjectId.GetID() );
	QDT_ASSERT( pIndex )
	pIndex->RemoveReference();



	IHM_PAGE *pParentPage = pPage->GetParentPage();

	// Page has a parent page.
	if ( pParentPage != NULL )
	{
		QDT_ASSERT( pParentPage->GetIndex() );

		pIndex		= pParentPage->GetIndex();
		ObjectId	= pIndex->GetObjectId();

		pIndex->AddReference();

		Stack.SetBool( 1, true );
	}
	// Page has no parent page.
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
//	Name:		OnSetActivePage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnSetActiveElementsListIndex(void *				pObject,
													COMMAND_ID			CommandID,
													PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );


	// Valid index.
	QDT_ASSERT( Stack.GetInteger( 0 ) >= 0 );
	QDT_ASSERT( Stack.GetInteger( 0 ) <  pPage->GetNbrElementsLists() );
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) >= pPage->GetNbrElementsLists() ) )
	{
		QDT_Error( "In IHM_PAGE_DESC::OnSetActiveElementsListIndex: invalid index %i.", Stack.GetInteger(0) );
		return ( false );
	}


	pPage->SetActiveElementsList( Stack.GetInteger( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetActiveElementsList
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnSetActiveElementsList(void *				pObject,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID(0);
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST )
	{
		QDT_Warning("In IHM_PAGE_DESC::OnSetActivePage: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pElementsListIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pElementsListIndexInt );
	if ( pElementsListIndexInt == NULL )
	{
		QDT_Warning("In IHM_PAGE_DESC::OnSetActiveElementsList: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid elements list.
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pElementsListIndexInt->GetObject() );
	QDT_ASSERT( pElementsList );
	if ( ( pElementsList == NULL ) || ( pElementsList->IsLoaded() == false ) )
	{
		QDT_Warning("In IHM_PAGE_DESC::OnSetActiveElementsList: elements list invalid or not loaded !");
		return( false );
	}

	
	pPage->SetActiveElementsList( pElementsList );
	
	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetNbrPage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnGetNbrElementsLists(void *				pObject,
											 COMMAND_ID			CommandID,
											 PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );


	Stack.SetInteger( 0, pPage->GetNbrElementsLists() );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetActiveElementsListIndex
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnGetActiveElementsListIndex(void *				pObject,
													COMMAND_ID			CommandID,
													PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );


	int nActiveElementsListIndex = pPage->GetActiveElementsListIndex();

	// The page has an active elements list.
	if ( nActiveElementsListIndex >= 0 )
	{
		QDT_ASSERT( nActiveElementsListIndex < pPage->GetNbrElementsLists() );
		Stack.SetBool( 1, true );
	}
	// The page has no active elements list.
	else
	{
		QDT_ASSERT( nActiveElementsListIndex == -1 );
		Stack.SetBool( 1, false );
	}


	Stack.SetInteger( 0, nActiveElementsListIndex );
	
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetActivePage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnGetActiveElementsList(void *				pObject,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );

	OBJECT_ID	ObjectId;
	INDEX_INT	*pIndex;


	// Dereference the dyn one given from the script.
	ObjectId	= Stack.GetObjectID( 0 );
	pIndex		= DATABASE_MANAGER::Instance()->GetById( ObjectId.GetType(), ObjectId.GetID() );
	QDT_ASSERT( pIndex )
	pIndex->RemoveReference();



	IHM_ELEMENTS_LIST *pActiveElementsList = pPage->GetActiveElementsList();

	// The page has an active elements list.
	if ( pActiveElementsList != NULL )
	{
		QDT_ASSERT( pActiveElementsList->GetIndex() );

		pIndex		= pActiveElementsList->GetIndex();
		ObjectId	= pIndex->GetObjectId();

		pIndex->AddReference();

		Stack.SetBool( 1, true );
	}
	// The page has no active elements list.
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
//	Name:		OnAddPage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnAddElementsList(void *				pObject,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID(0);
	if ( Id.GetType() != M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST )
	{
		QDT_Warning("In IHM_PAGE_DESC::OnAddElementsList: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pElementsListIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pElementsListIndexInt );
	if ( pElementsListIndexInt == NULL )
	{
		QDT_Warning("In IHM_PAGE_DESC::OnAddElementsList: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid elements list.
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pElementsListIndexInt->GetObject() );
	QDT_ASSERT( pElementsList );
	if ( ( pElementsList == NULL ) || ( pElementsList->IsLoaded() == false ) )
	{
		QDT_Warning("In IHM_PAGE_DESC::OnAddElementsList: elements list invalid or not loaded !");
		return( false );
	}


	pPage->AddElementsList( pElementsList );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetActivePage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnGetElementsList(void *				pObject,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );

	OBJECT_ID	ObjectId;
	INDEX_INT	*pIndex;


	// Valid elements list index.
	QDT_ASSERT( Stack.GetInteger( 0 ) >= 0 );
	QDT_ASSERT( Stack.GetInteger( 0 ) <  pPage->GetNbrElementsLists() );
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) >= pPage->GetNbrElementsLists() ) )
	{
		QDT_Error( "In IHM_PAGE_DESC::OnGetElementsList: invalid index %i.", Stack.GetInteger(0) );
		return ( false );
	}


	// Dereference the dyn one given from the script.
	ObjectId	= Stack.GetObjectID( 1 );
	pIndex		= DATABASE_MANAGER::Instance()->GetById( ObjectId.GetType(), ObjectId.GetID() );
	QDT_ASSERT( pIndex )
	pIndex->RemoveReference();


	// Set the new object.
	pIndex		= pPage->GetElementsList( Stack.GetInteger( 0 ) )->GetIndex();
	QDT_ASSERT( pIndex );
	pIndex->AddReference();


	ObjectId	= pIndex->GetObjectId();
	Stack.SetObjectID( 1, ObjectId );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnIsVisible
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnIsVisible(void *				pObject,
								   COMMAND_ID			CommandID,
								   PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );

	Stack.SetBool( 0, pPage->IsVisible() );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnShow
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnShow(void *			pObject,
							  COMMAND_ID		CommandID,
							  PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );

	pPage->Show();
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnHide
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::OnHide(void *			pObject,
							  COMMAND_ID		CommandID,
							  PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );

	pPage->Hide();
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE_DESC::ReceiveCommandProc(void *			pObject,
										  COMMAND_ID		CommandID,
										  PARAMETER_STACK &	Stack) const
{
	IHM_PAGE *pPage = static_cast<IHM_PAGE*>( pObject );
	QDT_ASSERT( pPage );
	if ( !pPage )
	{
		return ( false );
	}

	switch(CommandID)
	{
	// Returns the parent page of the current page.
	case GET_PARENT_PAGE:
		return ( OnGetParentPage( pObject, CommandID, Stack ) );
		break;

	// Set the active elements list of the page using its position.
	case SET_ACTIVE_ELEMENTS_LIST_INDEX:
		return ( OnSetActiveElementsListIndex( pObject, CommandID, Stack ) );
		break;

	// Set the active elements list of the page.
	case SET_ACTIVE_ELEMENTS_LIST:
		return ( OnSetActiveElementsList( pObject, CommandID, Stack ) );
		break;
	
	// Returns the number of elements list in the interface.
	case GET_NBR_ELEMENTS_LISTS:
		return ( OnGetNbrElementsLists( pObject, CommandID, Stack ) );
		break;

	// Return the index (position) of the active elements list of the interface.
	case GET_ACTIVE_ELEMENTS_LIST_INDEX:
		return ( OnGetActiveElementsListIndex( pObject, CommandID, Stack ) );
		break;

	// Returns the active elements list.
	case GET_ACTIVE_ELEMENTS_LIST:
		return ( OnGetActiveElementsList( pObject, CommandID, Stack ) );
		break;

	// Add an elements list into the current page.
	case ADD_ELEMENTS_LIST:
		return ( OnAddElementsList( pObject, CommandID, Stack ) );
		break;

	// Get a specific elements list.
	case GET_ELEMENTS_LIST:
		return ( OnGetElementsList( pObject, CommandID, Stack ) );
		break;

	// Returns 'true' if the element is visible.
	case IS_VISIBLE:
		return( OnIsVisible( pObject, CommandID, Stack ) );
		break;

	// Show the element.
	case SHOW:
		return( OnShow( pObject, CommandID, Stack ) );
		break;

	// Hide the element.
	case HIDE:
		return( OnHide( pObject, CommandID, Stack ) );
		break;

	// Default.
	default:
		return ( DefaultCommand( pPage, CommandID, Stack ) );
	}
}
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
