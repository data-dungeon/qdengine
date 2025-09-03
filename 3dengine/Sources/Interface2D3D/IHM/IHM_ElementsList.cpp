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
//	CLASS:	IHM_ELEMENTS_LIST
//
//	01-08-20:	GGO - Created
//*****************************************************************************
 

#include	"Root.h"
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_ElementsList)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Page)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
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
//	Name:		IHM_ELEMENTS_LIST constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENTS_LIST::IHM_ELEMENTS_LIST()
:
COMMUNICATOR			( GetGenericCommunicatorDescriptor() ),
_Flags					( 0 ),
//_pActionSound			( NULL ),
//_pSwitchingSound		( NULL ),
_pParentContainer		( NULL ),
_bHasParentContainer	( false ),
_nActiveElementIndex	( -1 )
{
	Show();
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENTS_LIST destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENTS_LIST::~IHM_ELEMENTS_LIST()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Open(IHM_INTERFACE *	pInterface,
								IHM_PAGE *		pPage)
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Handle
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Handle(IHM_INTERFACE *	pInterface,
								  IHM_PAGE *		pPage,
								  int				nEvent)
{
	bool	bPlaySwitchingSound = false;
	int		nNewElementIndex;


	// By default, elements list are up-down menus.

	// Up event.
	if ( nEvent == INTERFACE_MANAGER::ON_UP )
	{
		nNewElementIndex = _nActiveElementIndex - 1;
		if ( nNewElementIndex < 0 )
		{
			nNewElementIndex = _Elements.GetSize() - 1;
		}
		SetActiveElement( nNewElementIndex );

		bPlaySwitchingSound = true;
	}
	// Down event.
	else
	if ( nEvent == INTERFACE_MANAGER::ON_DOWN )
	{
		nNewElementIndex  = _nActiveElementIndex + 1;
		nNewElementIndex %= _Elements.GetSize();

		SetActiveElement( nNewElementIndex );

		bPlaySwitchingSound = true;
	}
	// Play the switching sound (if any).
	if ( bPlaySwitchingSound == true )
	{
		/*if ( ( _pSwitchingSound != NULL ) && ( _pSwitchingSound->IsLoaded() == true ) )
		{
			SOUND::Instance()->Play( _pSwitchingSound->GetIndex(), 0, 1, 0 );
		}*/
	}


	IHM_ELEMENT_ABC * pActiveElement = (IHM_ELEMENT_ABC *) GetActiveElement().GetCommunicator();

	if ( pActiveElement != NULL )
	{
		pActiveElement->Handle( pInterface, pPage, this, nEvent );
	}
}

//-----------------------------------------------------------------------------
//	Name:		HandleMouse
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENTS_LIST::HandleMouse(IHM_INTERFACE *	pInterface,
									   IHM_PAGE *		pPage,
									   int &			nEvent,
									   int				nMousePosX,
									   int				nMousePosY)
{
	QDT_ASSERT( IsVisible() == true );


	IHM_ELEMENT_ABC *pElement;

	for ( unsigned int i = 0; i < _Elements.GetSize(); ++i )
	{
		pElement = (IHM_ELEMENT_ABC *) _Elements[ i ].GetCommunicator();
		if ( ( pElement != NULL ) && ( pElement->IsVisible() == true ) )
		{
			pElement->HandleMouse( pInterface, pPage, this, nEvent, nMousePosX, nMousePosY );
		}
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		HandleTime
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::HandleTime(IHM_INTERFACE *	pInterface,
									  IHM_PAGE *		pPage,
									  const float		rFrameTimeMs)
{
	QDT_ASSERT( IsVisible() == true );


	IHM_ELEMENT_ABC *pElement;

	for ( unsigned int i = 0; i < _Elements.GetSize(); ++i )
	{
		pElement = (IHM_ELEMENT_ABC *) _Elements[ i ].GetCommunicator();
		pElement->HandleTime( pInterface, pPage, this, rFrameTimeMs );
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Close(IHM_INTERFACE *	pInterface,
								 IHM_PAGE *			pPage)
{
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	 = _Elements.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Elements.GetTail();

	while ( It != ItEnd )
	{
		((IHM_ELEMENT_ABC *) (*It).GetCommunicator())->Close( pInterface, pPage, this );
		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Display(IHM_INTERFACE *	pInterface,
								   IHM_PAGE *		pPage)
{
	if ( IsVisible() == false )
	{
		return;	
	}

	// We parse the elements lists in '_ElementLists' and display these which are visibles.
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	  = _Elements.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd  = _Elements.GetTail();

	while ( It != ItEnd )
	{
		if (((IHM_ELEMENT_ABC *) (*It).GetCommunicator())->IsVisible() == true )
		{
			((IHM_ELEMENT_ABC *) (*It).GetCommunicator())->Display( pInterface, pPage, this );
		}

		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetActiveElementIndex
//	Object:		
//	01-08-27:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENTS_LIST::GetActiveElementIndex() const
{
	return ( _nActiveElementIndex );
}

//-----------------------------------------------------------------------------
//	Name:		GetActiveElement
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	IHM_ELEMENTS_LIST::GetActiveElement() const
{
	QDT_ASSERT( _nActiveElementIndex < static_cast<int>( _Elements.GetSize() ) );
	return ( _Elements[ _nActiveElementIndex ] );
}
	
//-----------------------------------------------------------------------------
//	Name:		SetActiveElement
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST::SetActiveElement(const COM_INSTANCE &Element)
{
	// We check if 'pElement' is correct (ie. is in the elements list elements).
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	 = _Elements.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Elements.GetTail();
	unsigned int i = 0;

	while ( It != ItEnd )
	{
		if ( *It == Element )
		{
			SetActiveElement( i );
			return ( true );
		}
		++i;
		++It;
	}
	return ( false );
}
	
//-----------------------------------------------------------------------------
//	Name:		SetActiveElement
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST::SetActiveElement(const int	nElementIndex)
{
	if ( ( nElementIndex < -1 ) ||
	     ( nElementIndex >= static_cast<int>( _Elements.GetSize() ) ) )
	{
		_nActiveElementIndex = -1;
		return ( false );
	}

	// Disactivate previous active element.
	if ( _nActiveElementIndex != -1 )
	{
		IHM_ELEMENT_ABC * pPreviousActiveElement = (IHM_ELEMENT_ABC *) GetActiveElement().GetCommunicator();
		if ( pPreviousActiveElement != NULL )
		{
			pPreviousActiveElement->OnSetInactive();
		}
	}

	// Change the active element index.
	_nActiveElementIndex = nElementIndex;

	// Active new element.
	IHM_ELEMENT_ABC *pNewActiveElement = (IHM_ELEMENT_ABC *) GetActiveElement().GetCommunicator();
	if ( pNewActiveElement !=NULL )
	{
		pNewActiveElement->OnSetActive();
	}

	return (true );
}

//-----------------------------------------------------------------------------
//	Name:		AddElement
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::AddElement(const COM_INSTANCE &Element)
{
	if ( _Elements.Find( _Elements.GetHead(), Element ) != _Elements.GetTail() )
	{
		return;
	}

	IHM_ELEMENTS_LIST * pList = NULL;
	IHM_ELEMENT_ABC * pElement = (IHM_ELEMENT_ABC *) Element.GetCommunicator();

	if ( pElement->HasParentContainer() )
	{
		pList = pElement->GetParentContainer();
	}
	
	// Change parent element.
	if ( pList != NULL )
	{
		pList->RemoveElement( Element );
	}
	
	pElement->SetParentContainer( this );

	_Elements.PushTail( Element );
}

//-----------------------------------------------------------------------------
//	Name:		CM_AddElement
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENTS_LIST::CM_AddElement(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_ADD_ELEMENT);
	COM_INSTANCE pObjet;
	COM_INSTANCE Element;
	IHM_ELEMENT_ABC *pNewElement;
	Stack.GetThis(pObjet);
	
	IHM_ELEMENTS_LIST	*pIhmElementList = static_cast<IHM_ELEMENTS_LIST *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, Element);

	pNewElement = static_cast<IHM_ELEMENT_ABC *>(Element.GetCommunicator());

/*	if ( pIhmElementList->_Elements.Find( pIhmElementList->_Elements.GetHead(), Element ) != pIhmElementList->_Elements.GetTail() )
	{
		return (COM_STACK::NO_RET_VALUE);
	}

	IHM_ELEMENTS_LIST * pList = NULL;

	if ( pNewElement->HasParentContainer() )
	{
		pList = pNewElement->GetParentContainer();
	}
	
	// Change parent element.
	if ( pList != NULL )
	{
		pList->RemoveElement( Element );
	}
	
	pNewElement->SetParentContainer( pIhmElementList );

	pIhmElementList->_Elements.PushTail( Element );*/

	DISPLAY_MANAGER::Instance()->AddIhmElement(Element);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveElement
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST::RemoveElement(const int	nElementIndex)
{
	IHM_ELEMENT_ABC *	pElement;

	if ( ( nElementIndex < 0 ) ||
		 ( nElementIndex >= static_cast<int>( _Elements.GetSize() ) ) )
	{
		QDT_FAIL();
		return ( false );
	}

	QDT_VECTOR<COM_INSTANCE>::ITERATOR It = _Elements.GetHead();
	_Elements.Remove( It + nElementIndex );

	// Set the parent container of the removed element to NULL.
	pElement = (IHM_ELEMENT_ABC *) (*(It + nElementIndex)).GetCommunicator();
	QDT_ASSERT( pElement );
	//pElement->SetParentContainer( NULL );
	pElement->UnsetParentContainer();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		RemoveElement
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST::RemoveElement(const COM_INSTANCE &Element)
{
		  QDT_VECTOR<COM_INSTANCE>::ITERATOR ItCur = _Elements.GetHead();
	const QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Elements.GetTail();

	IHM_ELEMENT_ABC * pElement =(IHM_ELEMENT_ABC *) Element.GetCommunicator();

	while ( ItCur != ItEnd )
	{
		IHM_ELEMENT_ABC * pCurElement =(IHM_ELEMENT_ABC *) (*ItCur).GetCommunicator();

		if ( pCurElement == pElement )
		{
			//((IHM_ELEMENT_ABC *) CurElement.GetCommunicator())->SetParentContainer( NULL );
			pCurElement->UnsetParentContainer();
			//QDT_Message( "Remove Element %s : List %s", ((IHM_ELEMENT_ABC *) CurElement.GetCommunicator())->GetName().GetBuffer() , GetName().GetBuffer() );
			_Elements.Remove( ItCur );
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
bool	IHM_ELEMENTS_LIST::IsVisible()
{
	return ( _Flags.IsSet( IS_VISIBLE ) );
}

//-----------------------------------------------------------------------------
//	Name:		Show
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Show()
{
	_Flags.Set( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		Hide
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Hide()
{
	_Flags.Unset( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		GetElement
//	Object:		
//	01-08-24:	GGO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	IHM_ELEMENTS_LIST::GetElement(const int	nElementIndex) const
{
	QDT_ASSERT( nElementIndex >= 0 );
	QDT_ASSERT( nElementIndex <  static_cast<int>( _Elements.GetSize() ) );

	return ( _Elements[ nElementIndex ] );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrElements
//	Object:		
//	01-08-27:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENTS_LIST::GetNbrElements() const
{
	return ( static_cast<int>( _Elements.GetSize() ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetElementOpeningPage
//	Object:		
//	01-08-30:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::SetElementOpeningPage(int			nElementIndex,
												 IHM_PAGE*		pOpeningPage)
{
	QDT_ASSERT( nElementIndex >= 0 );
	QDT_ASSERT( nElementIndex < static_cast<int>( _Elements.GetSize() ) );

	if ( ( nElementIndex < 0 )
	  || ( nElementIndex >= static_cast<int>( _Elements.GetSize() ) ) )
	{
		return;
	}

	((IHM_ELEMENT_ABC *) _Elements[ nElementIndex ].GetCommunicator())->SetOpeningPage( pOpeningPage );
}

//-----------------------------------------------------------------------------
//	Name:		SetSwitchingSound
//	Object:		
//	01-09-05:	GGO - Created
//-----------------------------------------------------------------------------
/*void	IHM_ELEMENTS_LIST::SetSwitchingSound(SOUND_DATA *	pSound)
{
	_pSwitchingSound = pSound;
}

//-----------------------------------------------------------------------------
//	Name:		GetSwitchingSound
//	Object:		
//	01-10-17:	GGO - Created
//-----------------------------------------------------------------------------
SOUND_DATA *	IHM_ELEMENTS_LIST::GetSwitchingSound() const
{
	return	(_pSwitchingSound);
}*/

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Clear()
{
	_Elements.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SetActionSound
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
/*void	IHM_ELEMENTS_LIST::SetActionSound(SOUND_DATA *	pSound)
{
	_pActionSound = pSound;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetActionSound
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
SOUND_DATA *	IHM_ELEMENTS_LIST::GetActionSound() const
{
	return	(_pActionSound);
}*/

//-----------------------------------------------------------------------------
//	Name:		SwapElements
//	Object:		
//	02-01-08:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::SwapElements(const int	n1stElementIndex,
										const int	n2ndElementIndex)
{
	QDT_ASSERT( n1stElementIndex >= 0 );
	QDT_ASSERT( n2ndElementIndex >= 0 );
	QDT_ASSERT( n1stElementIndex < (int)_Elements.GetSize() );
	QDT_ASSERT( n2ndElementIndex < (int)_Elements.GetSize() );

	COM_INSTANCE CI = _Elements[ n1stElementIndex ];

	_Elements[ n1stElementIndex ] = _Elements[ n2ndElementIndex ];

	_Elements[ n2ndElementIndex ] = CI;
}

//-----------------------------------------------------------------------------
//	Name:		SetParentPage
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::SetParentContainer(IHM_PAGE * 	pParent)
{
	_pParentContainer = pParent;
	_bHasParentContainer = true;
}

//-----------------------------------------------------------------------------
//	Name:		GetParentPage
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
IHM_PAGE *	IHM_ELEMENTS_LIST::GetParentContainer() const
{
	return ( _pParentContainer );
}

//-----------------------------------------------------------------------------
//	Name:		HasParentContainer
//	Object:		
//	02-08-23:	AMA - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENTS_LIST::HasParentContainer()
{
	return (_bHasParentContainer);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pM = METHOD_DESCRIPTOR::Allocate("GetNbrElements", "Retrieve the number of elements list.",ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__SetReturn(pM,"nNbrElements", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddCommand(CMD_GET_NBR_ELEMENTS, pM, CM_GetNbrElements, EM_GetNbrElements);

	pM = METHOD_DESCRIPTOR__Allocate("GetElement", "Retrieve an element of the elements list.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "Element", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_GET_ELEMENT, pM, EM_GetElement);

	pM = METHOD_DESCRIPTOR__Allocate("GetActiveElement", "Retrieve the active element.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "ActiveElement", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bHasAnActiveElement", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_ACTIVE_ELEMENT, pM, EM_GetActiveElement);

	pM = METHOD_DESCRIPTOR__Allocate("GetActiveElementIndex", "Retrieve the index of the active element (-1 if none).");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "nElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bHasAnActiveElement", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_ACTIVE_ELEMENT_INDEX, pM, EM_GetActiveElementIndex);

	pM = METHOD_DESCRIPTOR__Allocate("SetActiveElement", "Set the active element.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Element", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE_ELEMENT, pM, EM_SetActiveElement);

	pM = METHOD_DESCRIPTOR__Allocate("SetActiveElementIndex", "Set the active element using its position.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE_ELEMENT_INDEX, pM, EM_SetActiveElementIndex);

	pM = METHOD_DESCRIPTOR::Allocate("AddElement", "Add an element.",ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameter(pM, "ElementToAdd", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_ADD_ELEMENT, pM, CM_AddElement, EM_AddElement);

	pM = METHOD_DESCRIPTOR__Allocate("SetElementOpeningPage", "Add an element.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "OpeningPage", "Ask 3D Engine to document this parameter.", IHM_PAGE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_ELEMENT_OPENING_PAGE, pM, EM_SetElementOpeningPage);

	pM = METHOD_DESCRIPTOR__Allocate("IsVisible", "Returns 'true' if the elements list is visible.");
	METHOD_DESCRIPTOR__SetReturn(pM,"bIsVisible", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_VISIBLE, pM, EM_IsVisible);

	pM = METHOD_DESCRIPTOR__Allocate("Show", "Show the elements list.");
	pCD->AddMethod(CMD_SHOW, pM, EM_Show);

	pM = METHOD_DESCRIPTOR__Allocate("Hide", "Hide the elements list.");
	pCD->AddMethod(CMD_HIDE, pM, EM_Hide);

	pM = METHOD_DESCRIPTOR__Allocate("SwapElements", "Swap two elements in the elements list. Use this if you want to change the display order of the element of the elements list.");
	METHOD_DESCRIPTOR__AddParameter(pM, "n1stElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "n2stElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SWAP_ELEMENTS, pM, EM_SwapElements);

	pM = METHOD_DESCRIPTOR__Allocate("RemoveElementIndex", "Remove an element from the current elements list.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nElementIndex", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_REMOVE_ELEMENT_INDEX, pM, EM_RemoveElementIndex);

	pM = METHOD_DESCRIPTOR__Allocate("RemoveElement", "Remove an element from the current elements list.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Element", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_REMOVE_ELEMENT, pM, EM_RemoveElement);

	/*pM = METHOD_DESCRIPTOR__Allocate("Copy", "Copy the IHM_LIST.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "IhmElementsList", "The IhmElementsList to copy.", IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_COPY, pM, EM_Copy);
	*/
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_Construct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	IHM_ELEMENTS_LIST *	pObject;
	pObject = static_cast<IHM_ELEMENTS_LIST *>(pO);
	QDT_NEW_PLACED(pObject, IHM_ELEMENTS_LIST)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_Destruct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	IHM_ELEMENTS_LIST *	pObject;
	pObject = static_cast<IHM_ELEMENTS_LIST*> (pO);
	pObject->IHM_ELEMENTS_LIST::~IHM_ELEMENTS_LIST();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetNbrElements
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_GetNbrElements(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	int * nNbrElements = (int *) pR;
	*nNbrElements = pIhmElementsList->GetNbrElements();
}

//-----------------------------------------------------------------------------
//	Name:		CM_GetNbrElements
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_ELEMENTS_LIST::CM_GetNbrElements(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_GET_NBR_ELEMENTS);
	COM_INSTANCE pObjet;
	Stack.GetThis(pObjet);
	
	IHM_ELEMENTS_LIST	*pIhmElementsList = static_cast<IHM_ELEMENTS_LIST *>(pObjet.GetCommunicator());

	//on push sur la pile la valeur de retour. Certains types sont valide seulement.	
	Stack.Push(pIhmElementsList->GetNbrElements());

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::ONE_RET_VALUE);
}


//-----------------------------------------------------------------------------
//	Name:		EM_GetElement
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_GetElement(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	struct	PARAMETERS
	{
		int				_nIndex;
		COM_INSTANCE *	_Element;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	if ( ( pParameters->_nIndex < 0 ) || ( pParameters->_nIndex >= pIhmElementsList->GetNbrElements() ) )
	{
		QDT_Warning(QDT_COMMENT("In IHM_ELEMENTS_LIST::EM_GetElement: invalid index %i"), pParameters->_nIndex );
		return;
	}

	//*pParameters->_Element = pIhmElementsList->GetElement(pParameters->_nIndex);
	COM_INSTANCE * pComInstance = pParameters->_Element;
	QDT_NEW_PLACED(pComInstance, COM_INSTANCE)(pIhmElementsList->GetElement(pParameters->_nIndex));
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetActiveElement
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_GetActiveElement(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	struct	PARAMETERS
	{
		COM_INSTANCE *	_ActiveElements;
		bool *			_bHasAnActiveElements;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	if ( pIhmElementsList->GetActiveElementIndex() >= 0 )
	{
		COM_INSTANCE * pComInstance = pParameters->_ActiveElements;
		QDT_NEW_PLACED(pComInstance, COM_INSTANCE)(pIhmElementsList->GetActiveElement());
		*pParameters->_bHasAnActiveElements = true;
	}
	else
	{
		COM_INSTANCE CI(IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor()->Create());
		*pParameters->_ActiveElements = CI;
		*pParameters->_bHasAnActiveElements = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetActiveElementIndex
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_GetActiveElementIndex(void * const	pO,
													void * const	pR,
													void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	struct	PARAMETERS
	{
		int *	_nActiveElementsIndex;
		bool *	_bHasAnActiveElements;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	*pParameters->_nActiveElementsIndex = pIhmElementsList->GetActiveElementIndex();

	// Elements list has an active element.
	if ( pParameters->_nActiveElementsIndex >= 0 )
	{
		QDT_ASSERT( *pParameters->_nActiveElementsIndex < pIhmElementsList->GetNbrElements() );
		*pParameters->_bHasAnActiveElements = true;
	}
	// Elements list has no active element.
	else
	{
		QDT_ASSERT( *pParameters->_nActiveElementsIndex == -1 );
		*pParameters->_bHasAnActiveElements = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActiveElement
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_SetActiveElement(void * const	pO,
											   void * const	pR,
											   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	COM_INSTANCE * pCO = (COM_INSTANCE *) pP;
	pIhmElementsList->SetActiveElement(*pCO);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActiveElementIndex
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_SetActiveElementIndex(void * const	pO,
													void * const	pR,
													void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	int * nIndex = (int *) pP;
	pIhmElementsList->SetActiveElement(*nIndex);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddElement
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_AddElement(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	COM_INSTANCE * pCO = (COM_INSTANCE *) pP;
	pIhmElementsList->AddElement(*pCO);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetElementOpeningPage
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_SetElementOpeningPage(void * const	pO,
													void * const	pR,
													void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	struct	PARAMETERS
	{
		int 			_nElementIndex;
		COM_INSTANCE 	_OpeningPage;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	IHM_PAGE * pPage;
	pPage = ( IHM_PAGE * ) pParameters->_OpeningPage.GetCommunicator();
	pIhmElementsList->SetElementOpeningPage(pParameters->_nElementIndex, pPage);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsVisible
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_IsVisible(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	bool * bIsVisible = (bool *) pR;
	*bIsVisible = pIhmElementsList->IsVisible();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Show
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_Show(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	pIhmElementsList->Show();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Hide
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_Hide(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	pIhmElementsList->Hide();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SwapElements
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_SwapElements(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	struct	PARAMETERS
	{
		int 	_n1stElementIndex;
		int 	_n2stElementIndex;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	int nNbrElements = pIhmElementsList->GetNbrElements();

	if ( ( pParameters->_n1stElementIndex < 0 ) ||
		 ( pParameters->_n2stElementIndex < 0 ) ||
		 ( pParameters->_n1stElementIndex >= nNbrElements ) ||
		 ( pParameters->_n2stElementIndex >= nNbrElements ) )
	{
		QDT_FAIL();
	}

	pIhmElementsList->SwapElements( pParameters->_n1stElementIndex, pParameters->_n2stElementIndex );
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveElementIndex
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_RemoveElementIndex(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	int * nIndex = (int *) pP;

	if ( ( *nIndex < 0 ) || ( *nIndex >= pIhmElementsList->GetNbrElements() ) )
	{
		QDT_FAIL();
	}
	
	pIhmElementsList->RemoveElement( *nIndex );
}

//-----------------------------------------------------------------------------
//	Name:		EM_RemoveElement
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_RemoveElement(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_ELEMENTS_LIST *		pIhmElementsList;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElementsList	= (IHM_ELEMENTS_LIST *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElementsList);

	COM_INSTANCE *pComInstance = (COM_INSTANCE *) pP;
	pIhmElementsList->RemoveElement( *pComInstance );
}

//-----------------------------------------------------------------------------
//	Name:		EM_Copy
//	Object:		
//	02-08-22:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENTS_LIST::EM_Copy(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *	pObject;
	COM_INSTANCE **	pParameter;

	pObject = (COM_INSTANCE*)pO;
	pParameter = (COM_INSTANCE**)pP;

	*pObject = **pParameter;
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENTS_LIST::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENTS_LIST"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENTS_LIST, 
										QDT_COMMENT("46C79755-1846-ADA0-550D-1851D1E20B1B"),
										sizeof(IHM_ELEMENTS_LIST), 
										Init, 
										QDT_COMMENT("The elements are the last containers since ihm_elements are put into them.")));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

