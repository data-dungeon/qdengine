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
//	CLASS:	IHM_INTERFACE
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Interface)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Page)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_ElementsList)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementPoly/IHM_ElementQuad)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Scenegraph/Entities/Interface3D/Interface3D)
#include	INCL_3DENGINE(Display/Viewport/Viewport)
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
//	Name:		IHM_INTERFACE constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_INTERFACE::IHM_INTERFACE()
:
COMMUNICATOR			( GetGenericCommunicatorDescriptor() ),
_nActivePageIndex		( -1 ),
_pViewport				( NULL),
_Flags					( IS_VISIBLE | INPUT_ACTIVATED ),
_BackGrndIhmElmtQuad	( IHM_ELEMENT_QUAD::GetGenericCommunicatorDescriptor()->Create() )
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_INTERFACE destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_INTERFACE::~IHM_INTERFACE()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Open
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Open()
{
	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Handle
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Handle(int	nEvent)
{
	// Activated inputs?
	if ( AreInputActivated() == false )
	{
		return;
	}

	if ( GetActivePageIndex() > -1 )
	{
		IHM_PAGE * pActivePage = (IHM_PAGE *) GetActivePage().GetCommunicator();

		if ( pActivePage != NULL )
		{
			pActivePage->Handle( this, nEvent );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		HandleMouse
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_INTERFACE::HandleMouse(int &	nEvent,
								   int		nMousePosX,
								   int		nMousePosY)
{
	// Activated inputs?
	if ( AreInputActivated() == false )
	{
		return;
	}

	// Visible?
	if ( IsVisible() == false )
	{
		return;
	}

	IHM_PAGE *pPage;

	for ( unsigned int i = 0; i < _Pages.GetSize(); ++i )
	{
		pPage = (IHM_PAGE *) _Pages[ i ].GetCommunicator();
		if ( ( pPage != NULL ) && ( pPage->IsVisible() == true ) )
		{
			pPage->HandleMouse( this, nEvent, nMousePosX, nMousePosY );
		}
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		HandleTime
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::HandleTime(const float	rFrameTimeMs)
{
	if ( IsVisible() == false )
	{
		return;
	}

	IHM_PAGE *pPage;

	for ( unsigned int i = 0; i < _Pages.GetSize(); ++i )
	{
		pPage = (IHM_PAGE *) _Pages[ i ].GetCommunicator();
		if ( ( pPage != NULL ) && ( pPage->IsVisible() == true ) )
		{
			pPage->HandleTime( this, rFrameTimeMs );
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Close
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Close()
{
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	  = _Pages.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Pages.GetTail();

	while ( It != ItEnd )
	{
		((IHM_PAGE *) (*It).GetCommunicator())->Close( this );
		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Display()
{
	if ( IsVisible() == false )
	{
		return;
	}

	// Display the background picture (if any).
	DisplayBackgroundPicture(_pViewport);

	// We parse the pages in '_Pages' and display these which are visible.
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	  = _Pages.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Pages.GetTail();

	while ( It != ItEnd )
	{
		if ( ((IHM_PAGE *) (*It).GetCommunicator())->IsVisible() == true )
		{
			((IHM_PAGE *) (*It).GetCommunicator())->Display( this );
		}
		++It;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetActivePage
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE::SetActivePage(const COM_INSTANCE &Page)
{
	QDT_VECTOR<COM_INSTANCE>::ITERATOR It	  = _Pages.GetHead();
	QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Pages.GetTail();
	unsigned int i = 0;

	while ( It != ItEnd )
	{
		if ( *It == Page )
		{
			SetActivePage( i );
			return ( true );
		}
		++i;
		++It;
	}

	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetActivePage
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE::SetActivePage(const int	nPageIndex)
{
	if ( ( nPageIndex <  -1 ) 
	  || ( nPageIndex >= static_cast<int>( _Pages.GetSize() ) ) )
	{
		_nActivePageIndex = -1;
		return ( false );
	}

	_nActivePageIndex = nPageIndex;
	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetActivePage
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	IHM_INTERFACE::GetActivePage() const
{
	QDT_ASSERT( _nActivePageIndex < static_cast<int>( _Pages.GetSize() ) );
	return ( _Pages[ _nActivePageIndex ] );
}

//-----------------------------------------------------------------------------
//	Name:		AddPage
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::AddPage(const COM_INSTANCE &PageToAdd)
{
	if ( _Pages.Find( _Pages.GetHead(), PageToAdd ) != _Pages.GetTail() )
	{
		return;
	}

	IHM_PAGE * pPageToAdd = (IHM_PAGE *) PageToAdd.GetCommunicator();
	IHM_INTERFACE * pInterface = pPageToAdd->GetParentContainer();
	
	// Set the parent container of the page to add.
	if ( pInterface != NULL )
	{
		pInterface->RemovePage( PageToAdd );
	}
	
	pPageToAdd->SetParentContainer( this );
	_Pages.PushTail( PageToAdd );
}

//-----------------------------------------------------------------------------
//	Name:		RemovePage
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE::RemovePage(const COM_INSTANCE &Page)
{
		  QDT_VECTOR<COM_INSTANCE>::ITERATOR ItCur = _Pages.GetHead();
	const QDT_VECTOR<COM_INSTANCE>::ITERATOR ItEnd = _Pages.GetTail();

	while ( ItCur != ItEnd )
	{
		IHM_PAGE * pCurPage = ((IHM_PAGE *) (*ItCur).GetCommunicator());
		IHM_PAGE * pPage	= ((IHM_PAGE *) (Page).GetCommunicator());
	
		if ( pCurPage == pPage )
		{
			_Pages.Remove( ItCur );
			//((IHM_PAGE *) CurPage.GetCommunicator() )->SetParentContainer( NULL );
			return ( true );
		}
		++ItCur;
	}
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrPages
//	Object:		
//	01-08-28:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_INTERFACE::GetNbrPages()
{
	return ( static_cast<int>( _Pages.GetSize() ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetActivePageIndex
//	Object:		
//	01-09-03:	GGO - Created
//-----------------------------------------------------------------------------
int	IHM_INTERFACE::GetActivePageIndex() const
{
	return ( _nActivePageIndex );
}

//-----------------------------------------------------------------------------
//	Name:		Activate
//	Object:		
//	01-09-26:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Activate()
{
	COM_INSTANCE CI(GetHandle());
	INTERFACE_MANAGER::Instance()->AddInterface( CI );
}

//-----------------------------------------------------------------------------
//	Name:		Disactivate
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Disactivate()
{
	Hide();
	COM_INSTANCE CI(GetHandle());
	INTERFACE_MANAGER::Instance()->RemoveInterface( CI );	
}

//-----------------------------------------------------------------------------
//	Name:		IsActivated
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE::IsActivated()
{
	COM_INSTANCE CI(GetHandle());
	return ( INTERFACE_MANAGER::Instance()->IsInterfaceActivated( CI ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetPage
//	Object:		
//	01-09-28:	GGO - Created
//-----------------------------------------------------------------------------
COM_INSTANCE	IHM_INTERFACE::GetPage(int	nPageIndex) const
{
	QDT_ASSERT ( nPageIndex < static_cast<int>( _Pages.GetSize() ) );
	return ( _Pages[ nPageIndex ] );
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::ClearAll()
{
	unsigned int		nCurrentPageIndex;
	int					nCurrentElementsListIndex;
	IHM_PAGE *			pCurrentPage;
	IHM_ELEMENTS_LIST *	pCurrentElementsList;

	for ( nCurrentPageIndex = 0; nCurrentPageIndex < _Pages.GetSize(); ++nCurrentPageIndex )
	{
		pCurrentPage = (IHM_PAGE *) _Pages[ nCurrentPageIndex ].GetCommunicator();

		for ( nCurrentElementsListIndex = 0; nCurrentElementsListIndex < pCurrentPage->GetNbrElementsLists(); ++nCurrentElementsListIndex )
		{
			pCurrentElementsList = (IHM_ELEMENTS_LIST *) pCurrentPage->GetElementsList( nCurrentElementsListIndex ).GetCommunicator();
			pCurrentElementsList->Clear();
		}

		pCurrentPage->Clear();
	}

	_Pages.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		SetBackgroundMaterial
//	Object:		
//	02-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::SetBackgroundMaterial(const COM_INSTANCE &	MaterialInst)
{
	IHM_ELEMENT_QUAD * pIhmElementQuad;

	pIhmElementQuad = static_cast<IHM_ELEMENT_QUAD*>( _BackGrndIhmElmtQuad.GetCommunicator() );

	pIhmElementQuad->SetMaterialInst( MaterialInst );	
}

//-----------------------------------------------------------------------------
//	Name:		DisplayBackgroundPicture
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::DisplayBackgroundPicture(VIEWPORT * pViewport)
{
	POINT2D				pPoints[ 4 ];
	IHM_ELEMENT_QUAD *	pIhmElementQuad;

	pIhmElementQuad = static_cast<IHM_ELEMENT_QUAD*>(_BackGrndIhmElmtQuad.GetCommunicator());

	if ( pIhmElementQuad->GetMaterialInst().HasHandle() )
	{
		pPoints[ 0 ].SetXY(   0.0f,   0.0f );
		pPoints[ 1 ].SetXY(   0.0f, 100.0f );
		pPoints[ 2 ].SetXY( 100.0f,   0.0f );
		pPoints[ 3 ].SetXY( 100.0f, 100.0f );
		pIhmElementQuad->SetVertices( pPoints );

		pPoints[ 0 ].SetXY( 0.0f, 0.0f );
		pPoints[ 1 ].SetXY( 0.0f, 1.0f );
		pPoints[ 2 ].SetXY( 1.0f, 0.0f );
		pPoints[ 3 ].SetXY( 1.0f, 1.0f );
		pIhmElementQuad->SetUVs( pPoints );

		pIhmElementQuad->DisplayElement( pViewport );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsVisible
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE::IsVisible()
{
	return ( _Flags.IsSet( IS_VISIBLE ) );
}

//-----------------------------------------------------------------------------
//	Name:		Show
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Show()
{
	_Flags.Set( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		Hide
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Hide()
{
	_Flags.Unset( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		ActiveInput
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::ActivateInput(const bool	bActivate)
{
	_Flags.Set( INPUT_ACTIVATED, bActivate );
}

//-----------------------------------------------------------------------------
//	Name:		AreInputActivated
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_INTERFACE::AreInputActivated() const
{
	return ( _Flags.IsSet( INPUT_ACTIVATED ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetViewport
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::SetViewport(VIEWPORT * pViewport)
{
	_pViewport = pViewport;
}
	
//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
VIEWPORT *	IHM_INTERFACE::GetViewport() const
{
	return ( _pViewport );
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

	pM = METHOD_DESCRIPTOR::Allocate("AddPage", "Add a page into the interface.", ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	METHOD_DESCRIPTOR__AddParameterReference(pM, "PageToAdd", "Ask 3D Engine to document this parameter.", IHM_PAGE::GetGenericCommunicatorDescriptor());
	pCD->AddCommand(CMD_ADD_PAGE, pM, CM_AddPage, EM_AddPage);

	pM = METHOD_DESCRIPTOR__Allocate("SetActivePageIndex", "Set the active page of the interface using its index.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nActivePageIndex", "The zero-based index of the page you want to want to be active.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE_PAGE_INDEX, pM, EM_SetActivePageIndex);

	pM = METHOD_DESCRIPTOR__Allocate("SetActivePage", "Set the active page of the interface. The page should already be in the interface. If not, the command will be ignored.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "ActivePage", "The page you want to be active", IHM_PAGE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_ACTIVE_PAGE, pM, EM_SetActivePage);

	pM = METHOD_DESCRIPTOR__Allocate("GetNbrPages", "Returns the number of pages in the interface.");
	METHOD_DESCRIPTOR__SetReturn(pM,"nNbrPages", "Ask 3D Engine to document this parameter.", TYPE_INT::GetDescriptor());
	pCD->AddMethod(CMD_GET_NBR_PAGES, pM, EM_GetNbrPages);

	pM = METHOD_DESCRIPTOR__Allocate("GetActivePageIndex", "Return the index (position) of the active page of the interface.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "nActivePageIndex", "The index of the active page", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bHasAnActivePage", "'true' if the interface currently have an active page.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_ACTIVE_PAGE_INDEX, pM, EM_GetActivePageIndex);

	pM = METHOD_DESCRIPTOR__Allocate("GetActivePage", "Returns the active page.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "nActivePage", "The current active page.", IHM_PAGE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bHasAnActivePage", "'true' if the interface currently have an active page.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_ACTIVE_PAGE, pM, EM_GetActivePage);

	pM = METHOD_DESCRIPTOR__Allocate("Display", "Display the interface.");
	pCD->AddMethod(CMD_DISPLAY, pM, EM_Display);

	pM = METHOD_DESCRIPTOR::Allocate("Activate", "Add the interface to the interface manager. If an interface isn't activated, it can't be visible nor receive inputs.",ACCESS_RIGHTS_INTERFACE::AR_SCP_LEVEL);
	pCD->AddCommand(CMD_ACTIVATE, pM, CM_Activate, EM_Activate);

	pM = METHOD_DESCRIPTOR__Allocate("Disactivate", "Remove the interface from the interface manager.");
	pCD->AddMethod(CMD_DISACTIVATE, pM, EM_Disactivate);

	pM = METHOD_DESCRIPTOR__Allocate("ClearAll", "Remove all objects in the interface.");
	pCD->AddMethod(CMD_CLEAR_ALL, pM, EM_ClearAll);

	pM = METHOD_DESCRIPTOR__Allocate("IsActivated", "Test if an interface is activated.");
	METHOD_DESCRIPTOR__SetReturn(pM,"bIsActivated", "'true' if the interface is activated.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_ACTIVATED, pM, EM_IsActivated);

	pM = METHOD_DESCRIPTOR__Allocate("IsVisible", "Test if an interface is visible.");
	METHOD_DESCRIPTOR__SetReturn(pM,"bIsVisible", "'true' if the interface is visible.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_VISIBLE, pM, EM_IsVisible);

	pM = METHOD_DESCRIPTOR__Allocate("Show", "Show the interface (but don't activate it).");
	pCD->AddMethod(CMD_SHOW, pM, EM_Show);

	pM = METHOD_DESCRIPTOR__Allocate("Hide", "Hide the interface (but don't disactivate it).");
	pCD->AddMethod(CMD_HIDE, pM, EM_Hide);

	pM = METHOD_DESCRIPTOR__Allocate("ActivateInput", "Set a background picture (if you want one).");
	METHOD_DESCRIPTOR__AddParameter(pM, "bActivate", "'true' if the inputs have to be handled.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ACTIVATE_INPUT, pM, EM_ActivateInput);

	pM = METHOD_DESCRIPTOR__Allocate("AreInputActivated", "Returns true if the inputs are activated on the current interface.");
	METHOD_DESCRIPTOR__SetReturn(pM,"bActivated", "'true' if the inputs are activated.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ARE_INPUT_ACTIVATED, pM, EM_AreInputActivated);

	pM = METHOD_DESCRIPTOR__Allocate("SetViewport", "Set the viewport in which the interface has to be displayed. Please note that if a element will be displayed its own viewport (if it has one) prior to its interface viewport.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Viewport", "The viewport in which the interface and its element are displayed", VIEWPORT::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_VIEWPORT, pM, EM_SetViewport);

	pM = METHOD_DESCRIPTOR__Allocate("Copy", "Copy the IHM_INTERFACE.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "IhmInterface", "The IhmInterface to copy.", IHM_INTERFACE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_COPY, pM, EM_Copy);

	pM = METHOD_DESCRIPTOR__Allocate("SetBackgroundMaterial", "Set a background material (if you want one).");
	//METHOD_DESCRIPTOR__AddParameter(pM, "Material", "The background material (should be a 'm3d_material').", MATERIAL::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_BACKGROUND_MATERIAL, pM, EM_SetBackgroundMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Construct(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	IHM_INTERFACE *	pObject;
	pObject = static_cast<IHM_INTERFACE *>(pO);
	QDT_NEW_PLACED(pObject, IHM_INTERFACE)();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Destruct(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	IHM_INTERFACE *	pObject;
	pObject = static_cast<IHM_INTERFACE*> (pO);
	pObject->IHM_INTERFACE::~IHM_INTERFACE();
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddPage
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_AddPage(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	COM_INSTANCE ** pCI = (COM_INSTANCE **) pP;
	
	pIhmInterface->AddPage(**pCI);
}

//-----------------------------------------------------------------------------
//	Name:		CM_AddPage
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_INTERFACE::CM_AddPage(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_ADD_PAGE);
	COM_INSTANCE pObjet;
	COM_INSTANCE Page;
	Stack.GetThis(pObjet);
	
	IHM_INTERFACE	*pIhmInterface = static_cast<IHM_INTERFACE *>(pObjet.GetCommunicator());

	Stack.GetParameter(0, Page);
	pIhmInterface->AddPage(Page);

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActivePageIndex
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_SetActivePageIndex(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	int * nIndex = (int *) pP;

	QDT_ASSERT( *nIndex >= 0 );
	QDT_ASSERT( *nIndex <  pIhmInterface->GetNbrPages() );
	if ( ( *nIndex < 0 ) || ( *nIndex >= pIhmInterface->GetNbrPages() ) )
	{
		QDT_Error(QDT_COMMENT("In IHM_INTERFACE::EM_SetActivePageIndex: invalid index %i."), *nIndex );
	}

	pIhmInterface->SetActivePage( *nIndex );
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetActivePage
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_SetActivePage(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	COM_INSTANCE ** pCI = (COM_INSTANCE **) pP;
	pIhmInterface->SetActivePage(**pCI);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetNbrPages
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_GetNbrPages(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	int * nNbrPages = (int *) pR;
	*nNbrPages = pIhmInterface->GetNbrPages();
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetActivePageIndex
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_GetActivePageIndex(void * const	pO,
											 void * const	pR,
											 void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);



	struct	PARAMETERS
	{
		int *	_nActivePageIndex;
		bool *	_bHasAnActivePage;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	*pParameters->_nActivePageIndex = pIhmInterface->GetActivePageIndex();

	// Elements list has an active element.
	if ( pParameters->_nActivePageIndex >= 0 )
	{
		QDT_ASSERT( *pParameters->_nActivePageIndex < pIhmInterface->GetNbrPages() );
		*pParameters->_bHasAnActivePage = true;
	}
	// Elements list has no active element.
	else
	{
		QDT_ASSERT( *pParameters->_nActivePageIndex == -1 );
		*pParameters->_bHasAnActivePage = false;
	}

}

//-----------------------------------------------------------------------------
//	Name:		EM_GetActivePage
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_GetActivePage(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	struct	PARAMETERS
	{
		COM_INSTANCE *	_nActivePage;
		bool *			_bHasAnActivePage;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	if ( pIhmInterface->GetActivePageIndex() >= 0 )
	{
		COM_INSTANCE * pComInstance = pParameters->_nActivePage;
		QDT_NEW_PLACED(pComInstance, COM_INSTANCE)(pIhmInterface->GetActivePage());
		*pParameters->_bHasAnActivePage = true;
	}
	else
	{
		COM_INSTANCE CI(IHM_PAGE::GetGenericCommunicatorDescriptor()->Create());
		*pParameters->_nActivePage = CI;
		*pParameters->_bHasAnActivePage = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_Display
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Display(void * const	pO,
								  void * const	pR,
								  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	pIhmInterface->Display();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Activate
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Activate(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	pIhmInterface->Activate();
}

//-----------------------------------------------------------------------------
//	Name:		CM_Activate
//	Object:		
//	06-03-13:	VHA - Created
//-----------------------------------------------------------------------------
int	IHM_INTERFACE::CM_Activate(COM_PARAM	P)
{
	COM_STACK	Stack(P, GetGenericCommunicatorDescriptor(), CMD_ACTIVATE);
	COM_INSTANCE pObjet;
	Stack.GetThis(pObjet);
	
	IHM_INTERFACE	*pIhmInterface = static_cast<IHM_INTERFACE *>(pObjet.GetCommunicator());

	pIhmInterface->Activate();

	//indiquer qu'on à une valeur de retour
	return (COM_STACK::NO_RET_VALUE);
}


//-----------------------------------------------------------------------------
//	Name:		EM_Disactivate
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Disactivate(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	pIhmInterface->Disactivate();
}

//-----------------------------------------------------------------------------
//	Name:		EM_ClearAll
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_ClearAll(void * const	pO,
								   void * const	pR,
								   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	pIhmInterface->ClearAll();
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsActivated
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_IsActivated(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	bool * bActivated = (bool *) pR;
	*bActivated = pIhmInterface->IsActivated();
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetBackgroundMaterial
//	Object:		
//	02-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_SetBackgroundMaterial(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	/*COM_INSTANCE *	pIhmInterfaceInst;
	IHM_INTERFACE *	pIhmInterface;
	COM_INSTANCE *	pInst;

	pIhmInterfaceInst	= static_cast<COM_INSTANCE*>(pO);
	pIhmInterface		= static_cast<IHM_INTERFACE*>(pIhmInterfaceInst->GetCommunicator());
	pInst				= static_cast<COM_INSTANCE*>(pP);

	pIhmInterface->SetBackgroundMaterial( *pInst );*/
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsVisible
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_IsVisible(void * const	pO,
									void * const	pR,
									void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	bool * bVisible = (bool *) pR;
	*bVisible = pIhmInterface->IsVisible();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Show
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Show(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	pIhmInterface->Show();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Hide
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Hide(void * const	pO,
							   void * const	pR,
							   void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	pIhmInterface->Hide();
}

//-----------------------------------------------------------------------------
//	Name:		EM_ActivateInput
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_ActivateInput(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	bool * bActivate = (bool *) pP;
	pIhmInterface->ActivateInput(*bActivate);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AreInputActivated
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_AreInputActivated(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	bool * bActivate = (bool *) pP;
	*bActivate = pIhmInterface->AreInputActivated();

}

//-----------------------------------------------------------------------------
//	Name:		EM_SetViewport
//	Object:		
//	02-08-20:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_SetViewport(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *	pObjet;	
	IHM_INTERFACE *		pIhmInterface;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmInterface	= (IHM_INTERFACE *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmInterface);

	COM_INSTANCE * pComInstance = (COM_INSTANCE *) pP;
	VIEWPORT * pViewport = (VIEWPORT *) (*pComInstance).GetCommunicator();
	pIhmInterface->SetViewport(pViewport);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Copy
//	Object:		
//	02-08-22:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_INTERFACE::EM_Copy(void * const	pO,
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
COMMUNICATOR_DESCRIPTOR *	IHM_INTERFACE::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_INTERFACE) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_INTERFACE::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_INTERFACE"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_INTERFACE, 
										QDT_COMMENT("580D3021-6802-F00C-78E8-6B5F1CD6DDDE"),
										sizeof(IHM_INTERFACE), 
										Init, 
										QDT_COMMENT("An interface is the highest container in the IHM hierachy. An interface contains pages.")));
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
