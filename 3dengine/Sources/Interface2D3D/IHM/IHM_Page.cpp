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
//	CLASS:	IHM_PAGE
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Page)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_ElementsList)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KCOM(ComBasis/GlobalVariable)
#include	INCL_KCOM(ComBasis/ComStack)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeBool)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_PAGE constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_PAGE::IHM_PAGE()
:
COMMUNICATOR				(GetGenericCommunicatorDescriptor()),
_Flags						( 0 ),
_pParentContainer			( NULL ),
_nActiveElementsListIndex	( -1 )
{
	// Do Nothing...
}


//-----------------------------------------------------------------------------
//	Name:		~IHM_PAGE destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_PAGE::~IHM_PAGE()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Open(IHM_INTERFACE *	pInterface)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Handle
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Handle(IHM_INTERFACE *	pInterface,
						 int				nEvent)
{
	if ( GetActiveElementsListIndex() > 0 )
	{
		IHM_ELEMENTS_LIST * pActiveElementsList = (IHM_ELEMENTS_LIST *) GetActiveElementsList().GetCommunicator();

		if ( pActiveElementsList != NULL )
		{
			pActiveElementsList->Handle( pInterface, this, nEvent );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		HandleMouse
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_PAGE::HandleMouse(IHM_INTERFACE *	pInterface,
							  int &				nEvent,
							  int				nMousePosX,
							  int				nMousePosY)
{
	QDT_ASSERT( IsVisible() == true );


	IHM_ELEMENTS_LIST * pElementsList;

	for ( unsigned int i = 0; i < _ElementsLists.GetSize(); ++i )
	{
		pElementsList = (IHM_ELEMENTS_LIST *) _ElementsLists[ i ].GetCommunicator();
		if ( ( pElementsList != NULL ) && ( pElementsList->IsVisible() == true ) )
		{
			pElementsList->HandleMouse( pInterface, this, nEvent, nMousePosX, nMousePosY );
		}
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		HandleTime
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::HandleTime(IHM_INTERFACE *	pInterface,
							 const float		rFrameTimeMs)
{
	QDT_ASSERT( IsVisible() == true );

	IHM_ELEMENTS_LIST * pElementsList;

	for ( unsigned int i = 0; i < _ElementsLists.GetSize(); ++i )
	{
		pElementsList = (IHM_ELEMENTS_LIST *) _ElementsLists[ i ].GetCommunicator();
		if ( ( pElementsList != NULL ) && ( pElementsList->IsVisible() == true ) )
		{
			pElementsList->HandleTime( pInterface, this, rFrameTimeMs );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Close(IHM_INTERFACE *	pInterface)
{
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It		= _ElementsLists.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd	= _ElementsLists.GetTail();

	while ( It != ItEnd )
	{
		((IHM_ELEMENTS_LIST *) (*It).GetCommunicator())->Close( pInterface, this );
		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Display(IHM_INTERFACE *	pInterface)
{
	if ( IsVisible() == false )
	{
		return;	
	}

	// We parse the elements lists in '_ElementLists' and display these which are visibles.
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It		= _ElementsLists.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd	= _ElementsLists.GetTail();

	while ( It != ItEnd )
	{
		IHM_ELEMENTS_LIST * pElement = (IHM_ELEMENTS_LIST *) (*It).GetCommunicator();
		if ( pElement->IsVisible() )
		{
			pElement->Display( pInterface, this );
		}
		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetActiveElementsListIndex
//	Object:		
//	01-08-24:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_PAGE::GetActiveElementsListIndex() const
{
	return ( _nActiveElementsListIndex );
}

//-----------------------------------------------------------------------------
//	Name:		GetActiveElementsList
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	IHM_PAGE::GetActiveElementsList() const
{
	QDT_ASSERT( _nActiveElementsListIndex < (int) _ElementsLists.GetSize() );
	return ( _ElementsLists[_nActiveElementsListIndex] );
}

//-----------------------------------------------------------------------------
//	Name:		SetActiveElementsList
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE::SetActiveElementsList(const COM_INSTANCE &ElementList)
{
	// No active elements list.
	/*if ( pElementList == NULL )
	{
		_nActiveElementsListIndex = -1;
		return ( true );
	}*/

	// We check if the INDEX_INT is correct (ie. is in the painterface pages).
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	  = _ElementsLists.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd  = _ElementsLists.GetTail();
	unsigned int i = 0;

	while ( It != ItEnd )
	{
		if ( *It == ElementList )
		{
			SetActiveElementsList( i );
			return ( true );
		}
		++i;
		++It;
	}

	_nActiveElementsListIndex = -1;
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetActiveElementsList
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE::SetActiveElementsList(const int	nElementsListIndex)
{
	if ( ( nElementsListIndex <  -1 )
	  || ( nElementsListIndex >=  static_cast<int>( _ElementsLists.GetSize() ) ) )
	{
		_nActiveElementsListIndex = -1;
		return ( false );
	}

	_nActiveElementsListIndex = nElementsListIndex;
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		AddElementsList
//	Object:		
//	01-08-24:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::AddElementsList(const COM_INSTANCE &ElementList)
{
	if ( _ElementsLists.Find( _ElementsLists.GetHead(), ElementList ) != _ElementsLists.GetTail() )
	{
		return;
	}

	IHM_ELEMENTS_LIST * pElementList = (IHM_ELEMENTS_LIST *) ElementList.GetCommunicator();
	IHM_PAGE * pPage = pElementList->GetParentContainer();

	// Change parent container.
	if ( pPage != NULL )
	{
		pPage->RemoveElementsList( ElementList );
	}

	pElementList->SetParentContainer( this );

	_ElementsLists.PushTail( ElementList );
}

//-----------------------------------------------------------------------------
//	Name:		RemoveElementsList
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE::RemoveElementsList(const COM_INSTANCE &ElementList)
{
		  QDT_VECTOR<COM_INSTANCE>::ITERATOR ItCur = _ElementsLists.GetHead();
	const QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _ElementsLists.GetTail();

	while ( ItCur != ItEnd )
	{
		COM_INSTANCE CurElementsList = *ItCur;

		if ( CurElementsList == ElementList )
		{
			_ElementsLists.Remove( ItCur );
			return ( true );
		}

		++ItCur;
	}
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		IsVisible
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE::IsVisible()
{
	return ( _Flags.IsSet( IS_VISIBLE ) );
}

//-----------------------------------------------------------------------------
//	Name:		Show
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Show()
{
	_Flags.Set( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		Hide
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Hide()
{
	_Flags.Unset( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrElementsLists
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_PAGE::GetNbrElementsLists() const
{
	return ( static_cast<int>( _ElementsLists.GetSize() ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetElementsList
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	IHM_PAGE::GetElementsList(const int	nIndex) const
{
	QDT_ASSERT( nIndex >= 0 );
	QDT_ASSERT( nIndex <  static_cast<int>( _ElementsLists.GetSize() ) );

	return ( _ElementsLists[ nIndex ] );
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Clear()
{
	_ElementsLists.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SetParentContainer
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::SetParentContainer(IHM_INTERFACE *	pParent)
{
	_pParentContainer = pParent;
	_Flags.Set( HAS_PARENT_CONTAINER );
}

//-----------------------------------------------------------------------------
//	Name:		GetParentContainer
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
IHM_INTERFACE *	IHM_PAGE::GetParentContainer() const
{
	return ( _pParentContainer );
}

//-----------------------------------------------------------------------------
//	Name:		HasParentContainer
//	Object:		
//	02-08-23:	AMA - Created
//-----------------------------------------------------------------------------
bool	IHM_PAGE::HasParentContainer()
{
	return ( _Flags.IsSet( HAS_PARENT_CONTAINER ) );
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pM = METHOD_DESCRIPTOR__Allocate("SetActiveElementsListIndex", "Set the active elements list of the page using its position.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nActiveElementsListIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE_ELEMENTS_LIST_INDEX, pM, EM_SetActiveElementsListIndex);

	pM = METHOD_DESCRIPTOR__Allocate("SetActiveElementsList", "Set the active elements list of the page.");
	METHOD_DESCRIPTOR__AddParameter(pM, "ActiveElementsList", "Ask 3D Engine to document this parameter.", IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE_ELEMENTS_LIST, pM, EM_SetActiveElementsList);

	pM = METHOD_DESCRIPTOR__Allocate("GetNbrElementsLists", "Returns the number of elements list in the interface.");
	METHOD_DESCRIPTOR__SetReturn(pM,"nNbrElementsList", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GET_NBR_ELEMENTS_LISTS, pM, EM_GetNbrElementsLists);

	pM = METHOD_DESCRIPTOR__Allocate("GetActiveElementsListIndex", "Return the index (position) of the active elements list of the interface.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "nActiveElementsListIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bHasAnActiveElementsList", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_ACTIVE_ELEMENTS_LIST_INDEX, pM, EM_GetActiveElementsListIndex);

	pM = METHOD_DESCRIPTOR__Allocate("GetActiveElementsList", "Returns the active elements list.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "ActiveElementsList", "Ask 3D Engine to document this parameter.", IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bHasAnActiveElementsList", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_ACTIVE_ELEMENTS_LIST, pM, EM_GetActiveElementsList);

	pM = METHOD_DESCRIPTOR::Allocate("AddElementsList", "Add an elements list into the current page.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "ElementsListToAdd", "Ask 3D Engine to document this parameter.", IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_ADD_ELEMENTS_LIST, pM, CM_AddElementsList, EM_AddElementsList);

	pM = METHOD_DESCRIPTOR__Allocate("GetElementsList", "Get a specific elements list.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nElementsListIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__SetReturn(pM,"ElementsList", "Ask 3D Engine to document this parameter.", IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GET_ELEMENTS_LIST, pM, EM_GetElementsList);

	pM = METHOD_DESCRIPTOR__Allocate("IsVisible", "Returns 'true' if the elements list is visible.");
	METHOD_DESCRIPTOR__SetReturn(pM,"bIsVisible", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_VISIBLE, pM, EM_IsVisible);

	pM = METHOD_DESCRIPTOR::Allocate("Show", "Show the elements list.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pCD->AddCommand(CMD_SHOW, pM, CM_Show, EM_Show);

	pM = METHOD_DESCRIPTOR__Allocate("Hide", "Hide the elements list.");
	pCD->AddMethod(CMD_HIDE, pM, EM_Hide);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_Construct(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	IHM_PAGE *	pObject;
	pObject = static_cast<IHM_PAGE *>(pO);
	QDT_NEW_PLACED(pObject, IHM_PAGE)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_Destruct(void * const	pO,
							  void * const	pR,
							  void * const	pP)
{
	IHM_PAGE *	pObject;
	pObject = static_cast<IHM_PAGE*> (pO);
	pObject->IHM_PAGE::~IHM_PAGE();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActiveElementsListIndex
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_SetActiveElementsListIndex(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	int * nActiveElementList = (int *) pP;
	
	pIhmPage->SetActiveElementsList(*nActiveElementList);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActiveElementsList
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_SetActiveElementsList(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	COM_INSTANCE * pCI = (COM_INSTANCE *) pP;

	pIhmPage->SetActiveElementsList(*pCI);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetNbrElementsLists
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_GetNbrElementsLists(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	int *	nElementsLists = (int *) pR;
	*nElementsLists = pIhmPage->GetNbrElementsLists();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetActiveElementsListIndex
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_GetActiveElementsListIndex(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	struct	PARAMETERS
	{
		int *	_nActiveElementsListIndex;
		bool *	_bHasAnActiveElementsList;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	*pParameters->_nActiveElementsListIndex = pIhmPage->GetActiveElementsListIndex();

	// The page has an active elements list.
	if ( pParameters->_nActiveElementsListIndex >= 0 )
	{
		QDT_ASSERT( *pParameters->_nActiveElementsListIndex < pIhmPage->GetNbrElementsLists() );
		*pParameters->_bHasAnActiveElementsList = true;
	}
	// The page has no active elements list.
	else
	{
		QDT_ASSERT( *pParameters->_nActiveElementsListIndex == -1 );
		*pParameters->_bHasAnActiveElementsList = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetActiveElementsList
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_GetActiveElementsList(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	struct	PARAMETERS
	{
		COM_INSTANCE *	_ActiveElementsList;
		bool *			_bHasAnActiveElementsList;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;


	if ( pIhmPage->GetActiveElementsListIndex() >= 0 )
	{
		COM_INSTANCE * pComInstance = pParameters->_ActiveElementsList;
		QDT_NEW_PLACED(pComInstance, COM_INSTANCE)(pIhmPage->GetActiveElementsList());
		*pParameters->_bHasAnActiveElementsList = true;
	}
	else
	{
		COM_INSTANCE CI(IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor()->Create());
		*pParameters->_ActiveElementsList = CI;
		*pParameters->_bHasAnActiveElementsList = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddElementsList
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_AddElementsList(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	COM_INSTANCE * pCI = (COM_INSTANCE *) pP;
	pIhmPage->AddElementsList(*pCI);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddElementsList
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_PAGE::CM_AddElementsList(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_ADD_ELEMENTS_LIST);
	COM_INSTANCE pObjet;
	COM_INSTANCE ElementList;
	Stack.GetThis(pObjet);
	
	IHM_PAGE	*pPage = static_cast<IHM_PAGE *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, ElementList);
	pPage->AddElementsList(ElementList);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}



//-----------------------------------------------------------------------------
//	Name:		EM_GetElementsList
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_GetElementsList(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	int * nIndex = (int *) pP;
	COM_INSTANCE * pCI = (COM_INSTANCE *) pR;
	QDT_NEW_PLACED(pCI, COM_INSTANCE)(pIhmPage->GetElementsList(*nIndex));
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsVisible
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_IsVisible(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	bool * bIsVisible = (bool *) pR;
	*bIsVisible = pIhmPage->IsVisible();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Show
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_Show(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	pIhmPage->Show();
}

//-----------------------------------------------------------------------------
//	Name:		CM_Show
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_PAGE::CM_Show(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_SHOW);
	COM_INSTANCE pObjet;
	Stack.GetThis(pObjet);
	
	IHM_PAGE	*pIhmPage = static_cast<IHM_PAGE *>(pObjet.GetCommunicator());

	pIhmPage->Show();

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);}

//-----------------------------------------------------------------------------
//	Name:		EM_Hide
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_PAGE::EM_Hide(void * const	pO,
						  void * const	pR,
						  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_PAGE *		pIhmPage;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmPage	= (IHM_PAGE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmPage);

	pIhmPage->Hide();
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_PAGE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_PAGE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_PAGE::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_PAGE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_PAGE, 
										QDT_COMMENT("80F4EC2D-8F9E-65F8-50E6-3626C442D05C"),
										sizeof(IHM_PAGE), 
										Init, 
										QDT_COMMENT("A page contains pages elements lists.")));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
