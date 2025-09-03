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
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"
#include	INC_3DENGINE(M3D)
#include	INC_3DENGINE(Interface2D3D\IHM\IHM_Elements)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(OldCom/Communication)

#ifndef _DEBUG

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

IHM_ELEMENTS_LIST_DESC*	IHM_ELEMENTS_LIST_DESC::_pInstance      = NULL;
bool					IHM_ELEMENTS_LIST_DESC::_bDestroyedOnce = false;
static const char * const	pcComment = "The elements are the last containers since ihm_elements are put into them.";

//-----------------------------------------------------------------------------
//	Name: MATH_DESC constructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENTS_LIST_DESC::IHM_ELEMENTS_LIST_DESC()
:
DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST, "ihm_elementslist", false, NB_COMMANDS, pcComment)
{
	_pCommandDescsTab[0].Init(GET_NBR_ELEMENTS, 1, "GetNbrElements", "Retrieve an element of the elements list.");
	_pCommandDescsTab[0].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::OUT_PARAMETER, "nNbrElements");

	_pCommandDescsTab[1].Init(GET_ELEMENT, 2, "GetElement", "Retrieve an element of the elements list.");
	_pCommandDescsTab[1].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER,  "nElementIndex");
	_pCommandDescsTab[1].SetParameterDesc(1, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "Element");

	_pCommandDescsTab[2].Init(GET_ACTIVE_ELEMENT, 2, "GetActiveElement", "Retrieve the active element.");
	_pCommandDescsTab[2].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::OUT_PARAMETER, "ActiveElement");
	_pCommandDescsTab[2].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bHasAnActiveElement");

	_pCommandDescsTab[3].Init(GET_ACTIVE_ELEMENT_INDEX, 2, "GetActiveElementIndex", "Retrieve the index of the active element (-1 if none).");
	_pCommandDescsTab[3].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::OUT_PARAMETER, "nElementIndex");
	_pCommandDescsTab[3].SetParameterDesc(1, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bHasAnActiveElement");

	_pCommandDescsTab[4].Init(SET_ACTIVE_ELEMENT, 1, "SetActiveElement", "Set the active element.");
	_pCommandDescsTab[4].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Element");

	_pCommandDescsTab[5].Init(SET_ACTIVE_ELEMENT_INDEX, 1, "SetActiveElementIndex", "Set the active element using its position.");
	_pCommandDescsTab[5].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "nElementIndex");

	_pCommandDescsTab[6].Init(ADD_ELEMENT, 1, "AddElement", "Add an element.");
	_pCommandDescsTab[6].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "ElementToAdd");

	_pCommandDescsTab[7].Init(SET_ELEMENT_OPENING_PAGE, 2, "SetElementOpeningPage", "Set the opening page of an element of the current elements list.");
	_pCommandDescsTab[7].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "nElementIndex");
	_pCommandDescsTab[7].SetParameterDesc(1, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "OpeningPage");

	_pCommandDescsTab[8].Init(IS_VISIBLE, 1, "IsVisible", "Returns 'true' if the elements list is visible.");
	_pCommandDescsTab[8].SetParameterDesc(0, OTYPE_KERNEL_BOOLEAN,   1, PARAMETER_DESC::OUT_PARAMETER, "bIsVisible");

	_pCommandDescsTab[9].Init(SHOW, 0, "Show", "Show the elements list.");

	_pCommandDescsTab[10].Init(HIDE, 0, "Hide", "Hide the elements list.");

	_pCommandDescsTab[11].Init(SET_SWITCHING_SOUND, 1, "SetSwitchingSound", "Set the sound that has to be played when the user switches between elements.");
	_pCommandDescsTab[11].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Sound");

	_pCommandDescsTab[12].Init(SET_ACTION_SOUND, 1, "SetActionSound", "Set the sound that has to be played when an action occurs.");
	_pCommandDescsTab[12].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Sound");

	_pCommandDescsTab[13].Init(SWAP_ELEMENTS, 2, "SwapElements", "Swap two elements in the elements list. Use this if you want to change the display order of the element of the elements list.");
	_pCommandDescsTab[13].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "n1stElementIndex");
	_pCommandDescsTab[13].SetParameterDesc(1, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "n2ndElementIndex");

	_pCommandDescsTab[14].Init(REMOVE_ELEMENT_INDEX, 1, "RemoveElementIndex", "Remove an element from the current elements list.");
	_pCommandDescsTab[14].SetParameterDesc(0, OTYPE_KERNEL_INTEGER,   1, PARAMETER_DESC::IN_PARAMETER, "nElementIndex");

	_pCommandDescsTab[15].Init(REMOVE_ELEMENT, 1, "RemoveElement", "Remove an element from the current elements list.");
	_pCommandDescsTab[15].SetParameterDesc(0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Element");
	
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC destructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENTS_LIST_DESC::~IHM_ELEMENTS_LIST_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::New
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	IHM_ELEMENTS_LIST_DESC::New()
{
	_nNbObjects++;

	return ( new IHM_ELEMENTS_LIST );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Delete
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST_DESC::Delete(COM_OBJECT *	pC)
{
	_nNbObjects--;

	delete pC;
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Instance
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
DESC* IHM_ELEMENTS_LIST_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = new IHM_ELEMENTS_LIST_DESC();
	}
	
	return ( _pInstance );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Suicide
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void IHM_ELEMENTS_LIST_DESC::Suicide()
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
//	Name:		OnGetNbrElements
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnGetNbrElements(void *				pObject,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	Stack.SetInteger( 0, pElementsList->GetNbrElements() );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetElement
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnGetElement(void *				pObject,
											 COMMAND_ID			CommandID,
											 PARAMETER_STACK &	Stack) const
{	
	OBJECT_ID	ObjectId;
	INDEX_INT	*pIndex;

	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	// Valid index.
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) >= pElementsList->GetNbrElements() ) )
	{
		QDT_Warning( "In IHM_ELEMENTS_LIST_DESC::OnGetElement: invalid index %i.", Stack.GetInteger(0) );
		return ( false );
	}

	
	// Dereference the dyn one given from the script.
	ObjectId	= Stack.GetObjectID(1);
	pIndex		= DATABASE_MANAGER::Instance()->GetById( ObjectId.GetType(), ObjectId.GetID() );
	QDT_ASSERT( pIndex );
	pIndex->RemoveReference();

	
	// Set the element .
	pIndex = pElementsList->GetElement( (unsigned int)Stack.GetInteger( 0 ) )->GetIndex();
	QDT_ASSERT( pIndex );
	pIndex->AddReference();

	ObjectId = pIndex->GetObjectId();
	Stack.SetObjectID( 1, ObjectId );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetActiveElement
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnGetActiveElement(void *				pObject,
												   COMMAND_ID			CommandID,
												   PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	OBJECT_ID	ObjectId;
	INDEX_INT	*pIndex;


	// Dereference the dyn one given from the script.
	ObjectId	= Stack.GetObjectID( 0 );
	pIndex		= DATABASE_MANAGER::Instance()->GetById( ObjectId.GetType(), ObjectId.GetID() );
	QDT_ASSERT( pIndex )
	pIndex->RemoveReference();



	IHM_ELEMENT_ABC* pActiveElement = pElementsList->GetActiveElement();


	// Elements list has an active element.
	if ( pActiveElement != NULL )
	{
		QDT_ASSERT( pActiveElement->GetIndex() );

		pIndex		= pActiveElement->GetIndex();
		ObjectId	= pIndex->GetObjectId();

		pIndex->AddReference();

		Stack.SetBool( 1, true );
	}
	// Elements list has no active element.
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
//	Name:		OnGetActiveElementIndex
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnGetActiveElementIndex(void *				pObject,
														COMMAND_ID			CommandID,
														PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	int nActiveElementIndex = pElementsList->GetActiveElementIndex();

	// Elements list has an active element.
	if ( nActiveElementIndex >= 0 )
	{
		QDT_ASSERT( nActiveElementIndex < pElementsList->GetNbrElements() );

		Stack.SetBool( 1, true );
	}
	// Elements list has no active element.
	else
	{
		QDT_ASSERT( nActiveElementIndex == -1 );

		Stack.SetBool( 1, false );
	}


	Stack.SetInteger( 0, nActiveElementIndex );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetActiveElement
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnSetActiveElement(void *				pObject,
												   COMMAND_ID			CommandID,
												   PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID(0);
	if ( IHM_ELEMENT_ABC::IsTypeElementId( Id.GetType() ) == false )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetActiveElement: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT, nor OTYPE_MOTEUR3D_IHM_ELEMENT_MESH nor OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pElementIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pElementIndexInt );
	if ( pElementIndexInt )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetActiveElement: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element.
	IHM_ELEMENT_ABC	*pElement = static_cast<IHM_ELEMENT_ABC*>( pElementIndexInt->GetObject() );
	QDT_ASSERT( pElement );
	if( ( pElement == NULL ) || ( pElement->IsLoaded() ) )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetActiveElement: element invalid or not loaded !");
		return( false );
	}


	pElementsList->SetActiveElement( pElement );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetAvtiveElementIndex
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnSetActiveElementIndex(void *				pObject,
														COMMAND_ID			CommandID,
														PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid index.
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) >= pElementsList->GetNbrElements() ) )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetActiveElementIndex: invalid index %i.", Stack.GetInteger(0) );
		return ( false );
	}


	pElementsList->SetActiveElement( Stack.GetInteger( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnAddElement
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnAddElement(void *				pObject,
											 COMMAND_ID			CommandID,
											 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID(0);
	if ( IHM_ELEMENT_ABC::IsTypeElementId( Id.GetType() ) == false )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnAddElement: object is not of the right type (should be an element).");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pElementIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pElementIndexInt );
	if ( pElementIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnAddElement: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element.
	IHM_ELEMENT_ABC	*pElement = static_cast<IHM_ELEMENT_ABC*>( pElementIndexInt->GetObject() );
	QDT_ASSERT( pElement );
	if ( ( pElement == NULL ) || ( pElement->IsLoaded() == false ) )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnAddElement: element invalid or not loaded !");
		return( false );
	}


	pElementsList->AddElement( pElement );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetElementOpeningPage
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnSetElementOpeningPage(void *				pObject,
														COMMAND_ID			CommandID,
														PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 1 );	
	if ( IHM_ELEMENT_ABC::IsTypeElementId( Id.GetType() ) == false )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetElementOpeningPage: object is not of type OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT, nor OTYPE_MOTEUR3D_IHM_ELEMENT_MESH nor OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pPageIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pPageIndexInt );
	if ( pPageIndexInt == NULL )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetElementOpeningPage: element invalid or not loaded !");
		return( false );
	}


	// Valid page.
	IHM_PAGE	*pPage = static_cast<IHM_PAGE*>( pPageIndexInt->GetObject() );
	QDT_ASSERT( pPage );
	if ( ( pPage == NULL ) || ( pPage->IsLoaded() == false ) )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetElementOpeningPage: can't get an IHM_ELEMENTS_LIST from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}


	// Valid element index
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) >= pElementsList->GetNbrElements() ) )
	{
		QDT_Warning( "In IHM_ELEMENTS_LIST_DESC::OnSetElementOpeningPage: invalid index. %i", Stack.GetInteger(0) );
		return ( false );
	}

	
	pElementsList->SetElementOpeningPage( Stack.GetInteger( 0 ), pPage );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnIsVisible
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnIsVisible(void *				pObject,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	Stack.SetBool( 0, pElementsList->IsVisible() );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnShow
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnShow(void *				pObject,
									   COMMAND_ID			CommandID,
									   PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	pElementsList->Show();
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnHide
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnHide(void *				pObject,
									   COMMAND_ID			CommandID,
									   PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	pElementsList->Hide();
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetSwitchingSound
//	Object:		
//	01-09-05:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnSetSwitchingSound(void *				pObject,
													COMMAND_ID			CommandID,
													PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );	
	if ( Id.GetType() != OTYPE_KERNEL_SOUND_DATA )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetSwitchingSound: object is not of type OTYPE_KERNEL_SOUND_DATA !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pSoundIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pSoundIndexInt );
	if ( pSoundIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENTS_LIST_DESC::OnSetSwitchingSound: invalid INDEX_INT !");
		return( false );
	}


	// Valid page.
	SOUND_DATA	*pSoundData = static_cast<SOUND_DATA*>( pSoundIndexInt->GetObject() );
	QDT_ASSERT( pSoundData );
	if ( pSoundData == NULL )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetSwitchingSound: invalid sound !" );
		return( false );
	}

	
	pElementsList->SetSwitchingSound( pSoundData );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetActionSound
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnSetActionSound(void *				pObject,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );	
	if ( Id.GetType() != OTYPE_KERNEL_SOUND_DATA )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetActionSound: object is not of type OTYPE_KERNEL_SOUND_DATA !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pSoundIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pSoundIndexInt );
	if ( pSoundIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENTS_LIST_DESC::OnSetActionSound: invalid INDEX_INT !");
		return( false );
	}


	// Valid page.
	SOUND_DATA	*pSoundData = static_cast<SOUND_DATA*>( pSoundIndexInt->GetObject() );
	QDT_ASSERT( pSoundData );
	if ( pSoundData == NULL )
	{
		QDT_Warning("In IHM_ELEMENTS_LIST_DESC::OnSetActionSound: invalid sound !" );
		return( false );
	}

	
	pElementsList->SetActionSound( pSoundData );

	return( true );	
}

//-----------------------------------------------------------------------------
//	Name:		OnSwapElements
//	Object:		
//	02-01-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnSwapElements(void *				pObject,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	int nNbrElements = pElementsList->GetNbrElements();


	if ( ( Stack.GetInteger( 0 ) < 0 ) ||
		 ( Stack.GetInteger( 1 ) < 0 ) ||
		 ( Stack.GetInteger( 0 ) >= nNbrElements ) ||
		 ( Stack.GetInteger( 1 ) >= nNbrElements ) )
	{
		QDT_FAIL();

		return ( false );
	}

	pElementsList->SwapElements( Stack.GetInteger( 0 ), Stack.GetInteger( 1 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnRemoveElementIndex
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnRemoveElementIndex(void *				pObject,
													 COMMAND_ID			CommandID,
													 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );

	int nElementIndex = Stack.GetInteger( 0 );

	if ( ( nElementIndex < 0 ) || ( nElementIndex >= pElementsList->GetNbrElements() ) )
	{
		QDT_FAIL();

		return ( false );
	}

	pElementsList->RemoveElement( nElementIndex );
	
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnRemoveElement
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::OnRemoveElement(void *				pObject,
												COMMAND_ID			CommandID,
												PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );


	// Valid type.
	OBJECT_ID Id = Stack.GetObjectID( 0 );	
	if ( IHM_ELEMENT_ABC::IsTypeElementId( Id.GetType() ) == false )
	{
		QDT_Error("In IHM_ELEMENTS_LIST_DESC::OnRemoveElement: the object is not a element !");
		return false;
	}


	// Valid INDEX_INT.
	INDEX_INT *pElementIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pElementIndexInt );
	if ( pElementIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENTS_LIST_DESC::OnRemoveElement: invalid INDEX_INT !");
		return( false );
	}


	// Valid page.
	IHM_ELEMENT_ABC	*pElement = static_cast<IHM_ELEMENT_ABC*>( pElementIndexInt->GetObject() );
	QDT_ASSERT( pElement );
	if ( pElement == NULL )
	{
		QDT_Error("In IHM_ELEMENTS_LIST_DESC::OnRemoveElement: invalid element !" );
		return( false );
	}


	return ( pElementsList->RemoveElement( pElement ) );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST_DESC::ReceiveCommandProc(void *				pObject,
												   COMMAND_ID			CommandID,
												   PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENTS_LIST *pElementsList = static_cast<IHM_ELEMENTS_LIST*>( pObject );
	QDT_ASSERT( pElementsList );
	if ( !pElementsList )
	{
		return ( false );
	}


	switch(CommandID)
	{
	// Retrieve an element of the elements list.
	case GET_NBR_ELEMENTS:
		return ( OnGetNbrElements( pObject, CommandID, Stack ) );
	
	// Retrieve an element of the elements list.
	case GET_ELEMENT:
		return ( OnGetElement( pObject, CommandID, Stack ) );
	
	// Retrieve the active element.
	case GET_ACTIVE_ELEMENT:
		return ( OnGetActiveElement( pObject, CommandID, Stack ) );

	// Retrieve the index of the active element (-1 if none).
	case GET_ACTIVE_ELEMENT_INDEX:
		return ( OnGetActiveElementIndex( pObject, CommandID, Stack ) );

	// Set the active element.
	case SET_ACTIVE_ELEMENT:
		return ( OnSetActiveElement( pObject, CommandID, Stack ) );

	// Set the active element using its position.
	case SET_ACTIVE_ELEMENT_INDEX:
		return ( OnSetActiveElementIndex( pObject, CommandID, Stack ) );

	// Add an element.
	case ADD_ELEMENT:
		return ( OnAddElement( pObject, CommandID, Stack ) );

	// Set the opening page of an element of the current elements list.
	case SET_ELEMENT_OPENING_PAGE:
		return ( OnSetElementOpeningPage( pObject, CommandID, Stack ) );

	// Returns 'true' if the element is visible.
	case IS_VISIBLE:
		return ( OnIsVisible( pObject, CommandID, Stack ) );

	// Show the element.
	case SHOW:
		return ( OnShow( pObject, CommandID, Stack ) );

	// Hide the element.
	case HIDE:
		return ( OnHide( pObject, CommandID, Stack ) );

	// Set the sound that has to be played when the user switches between elements.
	case SET_SWITCHING_SOUND:
		return ( OnSetSwitchingSound( pObject, CommandID, Stack ) );

	// Set the sound that has to be played when an action occurs.
	case SET_ACTION_SOUND:
		return ( OnSetActionSound( pObject, CommandID, Stack ) );

	// Swap two elements in the elements list.
	case SWAP_ELEMENTS:
		return ( OnSwapElements( pObject, CommandID, Stack ) );

	// Remove an element from the current elements list.");
	case REMOVE_ELEMENT_INDEX:
		return ( OnRemoveElementIndex( pObject, CommandID, Stack ) );

	// Remove an element from the current elements list.
	case REMOVE_ELEMENT:
		return ( OnRemoveElement( pObject, CommandID, Stack ) );

	// Default.
	default:
		return ( DefaultCommand( pElementsList, CommandID, Stack ) );
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
