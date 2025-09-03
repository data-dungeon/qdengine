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
//	CLASS:	IHM_ELEMENT_ABC
//
//	01-08-20:	GGO - Created
//*****************************************************************************
 
#include	"Root.h"
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementRect/IHM_ElementText)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Interface)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Page)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_ElementsList)
#include	INCL_3DENGINE(SceneGraph/Entities/Interface3D/Interface3D)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KCOM(BaseType/TypeInt)
#include	INCL_KCOM(BaseType/TypeFloat)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_ABC constructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_ABC::IHM_ELEMENT_ABC(CLASS_DESCRIPTOR * const pCD)
:
COMMUNICATOR				(pCD),
_Flags						( 0 ),
_rBlinkingTime				( 1000.0f ),
_rCurBlinkTime				( 0.0f ),
_rZDepth					( 0.0f ),
//_pViewport					( NULL ),
_pInterface3D				( NULL ),
_pScrollingTextElement		( NULL ),
_pParentContainer			( NULL ),
_bVerticesToReset			( true ),
_bWired						( true ),
_pOpeningPage				( NULL ),
//_pOpeningPageSound			( NULL ),
_nVisibleLeftRightElement	( 0 )
//_pLeftRightSound			( NULL )
{
	Show();
}

//-----------------------------------------------------------------------------
//	Name:		~IHM_ELEMENT_ABC destructor
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_ABC::~IHM_ELEMENT_ABC()
{
	if (GetInterface3D())
	{
		COM_INSTANCE Instance(GetHandle());
		GetInterface3D()->RemoveIhmElement(Instance);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetSelectionPage
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetOpeningPage(IHM_PAGE * pOpeningPage)
{
	// Remove old flags.
	ClearFlags();
	_Flags.Set( OPENING_PAGE_ELEMENT );
	_pOpeningPage = pOpeningPage;
}

//-----------------------------------------------------------------------------
//	Name:		AddLeftRightElement
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::AddLeftRightElement(IHM_ELEMENT_ABC * pLeftRightElement)
{
	// Remove old flags.
	ClearFlags();
	_Flags.Set( LEFT_RIGHT_ELEMENT );
	/*if ( _pLeftRightElements == NULL )
	{
		_pLeftRightElements = new QDT_VECTOR<COM_INSTANCE>;
	}*/

	_LeftRightElements.PushTail( pLeftRightElement );
}
	
//-----------------------------------------------------------------------------
//	Name:		GetSelectionPage
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
IHM_PAGE *	IHM_ELEMENT_ABC::GetOpeningPage()
{
	return ( _pOpeningPage );
}
	
//-----------------------------------------------------------------------------
//	Name:		Handle
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::Handle(IHM_INTERFACE *		pInterface,
								IHM_PAGE *			pPage,
								IHM_ELEMENTS_LIST *	pElementsList,
								int					nEvent)
{
	// Is the element isn't valid, we return.
	if ( IsValid() == false )
	{
		return;
	}
	// Opening page element.
	if ( IsOpeningPageElement() == true )
	{
		// Open the opening page.
		if ( nEvent == INTERFACE_MANAGER::ON_OK )
		{
			((IHM_PAGE *) pInterface->GetActivePage().GetCommunicator())->Hide();
			_pOpeningPage->Show();
			COM_INSTANCE CI(_pOpeningPage->GetHandle());
			pInterface->SetActivePage( CI );

			// Play the sound (if any).
			/*if ( ( _pOpeningPageSound != NULL ) && ( _pOpeningPageSound->IsLoaded() == true ) )
			{
				SOUND::Instance()->Play( _pOpeningPageSound->GetIndex(),0, 0, 0 );
			}*/
		}
	}
	else
	// Left-right element.
	if ( IsLeftRightElement() == true )
	{
		// Switch between the available elements.
		if ( nEvent == INTERFACE_MANAGER::ON_LEFT )
		{
			++_nVisibleLeftRightElement;
			_nVisibleLeftRightElement %= _LeftRightElements.GetSize();
		}
		else
		if ( nEvent == INTERFACE_MANAGER::ON_RIGHT )
		{
			--_nVisibleLeftRightElement;
			if ( _nVisibleLeftRightElement < 0 )
			{
				_nVisibleLeftRightElement  = _LeftRightElements.GetSize() - 1;
			}
			else
			{
				_nVisibleLeftRightElement %= _LeftRightElements.GetSize();
			}
		}
		// Play the sound (if any).
		if ( ( nEvent == INTERFACE_MANAGER::ON_LEFT ) || ( nEvent == INTERFACE_MANAGER::ON_RIGHT ) )
		{
			/*if ( ( _pLeftRightSound != NULL ) && ( _pLeftRightSound->IsLoaded() == true ) )
			{
				SOUND::Instance()->Play( _pLeftRightSound->GetIndex(), 0, 0, 0 );
			}*/
		}
	}
	else
	// If an element isn't a left-right element, nor an opening page element,
	// we clear the interface.
	if ( nEvent == INTERFACE_MANAGER::ON_OK )
	{
		pInterface->Hide();
	}
}

//-----------------------------------------------------------------------------
//	Name:		HandleMouse
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::HandleMouse(IHM_INTERFACE *		pInterface,
									 IHM_PAGE *				pPage,
									 IHM_ELEMENTS_LIST *	pElementsList,
									 int &					nEvent,
									 int					nMousePosX,
									 int					nMousePosY)
{
	QDT_ASSERT( IsVisible() == true );

	// Is the element isn't valid, we return.
	if ( IsValid() == false )
	{
		return;
	}


	// If the element accepts no inputs, return.
	if ( IsAlwaysInactive() == true )
	{
		return;
	}


	// Handle mouse move (default does nothing).
	OnMouseMove( nMousePosX, nMousePosY );


	// The sound that will be played.
	//SOUND_DATA *pSoundToPlay = pElementsList->GetActionSound();


	// Mouse on the element?
	if ( IsPointInElement( pInterface->GetViewport(), nMousePosX, nMousePosY ) == true )
	{
		// The user has clicked.
		if ( nEvent == INTERFACE_MANAGER::ON_LBUTTON )
		{
			// Opening page element.
			if ( IsOpeningPageElement() == true )
			{
				// Remove the lbutton event from the event to prevent click on
				// elements in the opened page.
				nEvent &= ~INTERFACE_MANAGER::ON_LBUTTON;

				// Open the page.
				if ( (IHM_PAGE *) pInterface->GetActivePage().GetCommunicator() != NULL )
				{
					((IHM_PAGE *) pInterface->GetActivePage().GetCommunicator())->Hide();
				}
				_pOpeningPage->Show();
				COM_INSTANCE CI(_pOpeningPage->GetHandle());
				pInterface->SetActivePage( CI );

				// Play the sound (if any).
				/*if ( _pOpeningPageSound != NULL )
				{
					pSoundToPlay = _pOpeningPageSound;
				}*/
			}
			else
			// Left-right element.
			if ( IsLeftRightElement() == true )
			{
				// Switch between the available elements.
				++_nVisibleLeftRightElement;
				_nVisibleLeftRightElement %= _LeftRightElements.GetSize();
				
				// Play the sound (if any).
				/*if ( _pLeftRightSound != NULL )
				{
					pSoundToPlay = _pLeftRightSound;
				}*/
			}
			else
			// Scroll up element.
		/*	if ( _Flags.IsSet( SCROLLING_BUTTON_UP ) == true )
			{
				_pScrollingTextElement->ScrollUp(pInterface->GetViewport());
			}
			else
			// Scroll down element.
			if ( _Flags.IsSet( SCROLLING_BUTTON_DOWN ) == true )
			{
				_pScrollingTextElement->ScrollDown(pInterface->GetViewport());
			}
			else*/
			// If an element isn't a left-right element, nor an opening page element, we perform default behaviour.
			{
				OnMouseClick( pInterface, pPage, pElementsList );
			}


			// Play the sound, if any.
			/*if ( ( pSoundToPlay != NULL ) && ( pSoundToPlay->IsLoaded() == true ) )
			{
				SOUND::Instance()->Play( pSoundToPlay->GetIndex(), 0, 0, 0 );
			}*/
		}

		COM_INSTANCE CI(GetHandle());
		pElementsList->SetActiveElement( CI );

		OnMouseIn( pInterface, pPage, pElementsList );
	}
	else
	{
		OnMouseOut(pInterface, pPage, pElementsList);
	}
}
#endif
									 
//-----------------------------------------------------------------------------
//	Name:		OnMouseClick
//	Object:		
//	01-12-06:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::OnMouseClick(IHM_INTERFACE *		pInterface,
									  IHM_PAGE *			pPage,
									  IHM_ELEMENTS_LIST *	pElementsList)
{
	struct	PARAMETERS
	{
		PARAMETERS(COM_HANDLE * pInterface, COM_HANDLE * pPage, COM_HANDLE * pElementsList) 
		:
		_Interface(pInterface),
		_Page(pPage),
		_ElementsList(pElementsList)
		{ 
		};
		
		COM_INSTANCE _Interface;
		COM_INSTANCE _Page;
		COM_INSTANCE _ElementsList;
	};

	PARAMETERS P(pInterface->GetHandle(), pPage->GetHandle(), pElementsList->GetHandle());

	COM_INSTANCE CI(GetHandle());
	GetClassDescriptor()->CallMethod(CMD_ON_MOUSE_CLICK, &CI, NULL, &P);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		OnMouseMove
//	Object:		
//	01-12-10:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::OnMouseMove(int	nMousePosX,
									 int	nMousePosY)
{
	struct	PARAMETERS
	{
		int		_nMousePosX;
		int		_nMousePosY;
	};

	PARAMETERS	P;

	P._nMousePosX = nMousePosX;
	P._nMousePosY = nMousePosY;

	COM_INSTANCE CI(GetHandle());
	GetClassDescriptor()->CallMethod(CMD_ON_MOUSE_MOVE, &CI, NULL, &P);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		HandleTime
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::HandleTime(IHM_INTERFACE *		pInterface,
									IHM_PAGE *			pPage,
									IHM_ELEMENTS_LIST *	pElementsList,
									const float			rFrameTimeMs)
{
	// Blinking.
	if ( ( IsBlinking() == true ) && ( IsVisible() == true ) )
	{
		QDT_ASSERT(  rFrameTimeMs  >= 0.0f );
		QDT_ASSERT( _rCurBlinkTime >= 0.0f );
		QDT_ASSERT( _rBlinkingTime >= 0.0f );

		if ( _rCurBlinkTime > _rBlinkingTime )
		{
			_rCurBlinkTime = 0.0f;
		}
		else
		{
			_rCurBlinkTime += rFrameTimeMs;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-08-27:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::Display(IHM_INTERFACE *		pInterface,
								 IHM_PAGE *				pPage,
								 IHM_ELEMENTS_LIST *	pElementsList)
{
	// Display the element itself.
	if ( HasToBeDisplayed() == true )
	{
		if ( IsAlwaysOnTop() == true )
		{
			COM_INSTANCE CI(GetHandle());
			INTERFACE_MANAGER::Instance()->AddAlwaysOnTopElement( CI );
		}
		else
		{
			DisplayElement(pInterface->GetViewport());
#ifndef _MASTER
			if ( INTERFACE_MANAGER::Instance()->IsIHMDebugDisplayEnabled() == true )
			{
				DebugDisplay(pInterface->GetViewport());
			}
#endif
		}
	}


	// And display the correct left-right element (if any).
	if ( IsLeftRightElement() == true )
	{
		QDT_ASSERT(  IsOpeningPageElement() == false );
		//QDT_ASSERT( _pLeftRightElements != NULL );
		QDT_ASSERT( _LeftRightElements.GetSize() > 0 );
		QDT_ASSERT( _nVisibleLeftRightElement >= 0 );
		QDT_ASSERT( _nVisibleLeftRightElement < (int)_LeftRightElements.GetSize() );

		QDT_VECTOR<IHM_ELEMENT_ABC *>::ITERATOR It = _LeftRightElements.GetHead();
		It += _nVisibleLeftRightElement;

		(*It)->DisplayElement(pInterface->GetViewport());
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsVisible
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsVisible() const
{
	return ( _Flags.IsSet( IS_VISIBLE ) );
}

//-----------------------------------------------------------------------------
//	Name:		Show
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::Show()
{
	_Flags.Set( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		Hide
//	Object:		
//	01-08-20:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::Hide()
{
	_Flags.Unset( IS_VISIBLE );
}

//-----------------------------------------------------------------------------
//	Name:		SetOpeningPageSound
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
/*void	IHM_ELEMENT_ABC::SetOpeningPageSound(SOUND_DATA *	pSound)
{
	_pOpeningPageSound = pSound;
}*/

//-----------------------------------------------------------------------------
//	Name:		SetOpeningPage
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
/*void	IHM_ELEMENT_ABC::SetOpeningPage(IHM_PAGE *		pOpeningPage,
										SOUND_DATA *	pOpeningPageSound)
{
	SetOpeningPage( pOpeningPage );
	SetOpeningPageSound( pOpeningPageSound );
}*/

//-----------------------------------------------------------------------------
//	Name:		IsOpeningPageElement
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsOpeningPageElement()
{
	return ( _Flags.IsSet( OPENING_PAGE_ELEMENT ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsLeftRightElement
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsLeftRightElement()
{
	return ( _Flags.IsSet( LEFT_RIGHT_ELEMENT ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetLeftRightSound
//	Object:		Set the sound that has to be played when the user switches
//				between elements.
//  WARNING:	MUST BE CALLED WHEN THE ELEMENT HAS AT LEAST A LEFT-RIGHT ELEMENT.
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
/*void	IHM_ELEMENT_ABC::SetLeftRightSound(SOUND_DATA *	pSound)
{
	_pLeftRightSound = pSound;
}*/

//-----------------------------------------------------------------------------
//	Name:		PlayOpeningPageSound
//	Object:		
//	01-09-07:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::PlayOpeningPageSound()
{
	/*QDT_ASSERT( IsOpeningPageElement() == true );

	if ( ( IsOpeningPageElement() == false ) || ( _pOpeningPageSound == NULL ) )
	{
		return;
	}

	SOUND::Instance()->Play( _pOpeningPageSound->GetIndex(), 0, 1, 0 );*/
}

//-----------------------------------------------------------------------------
//	Name:		OnSetActive
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::OnSetActive()
{
	if ( _Flags.IsSet( IS_ACTIVE ) == false )
	{
		_Flags.Set( IS_ACTIVE );

		COM_INSTANCE CI(GetHandle());
		GetClassDescriptor()->CallMethod(CMD_ON_SET_ACTIVE, &CI, NULL, NULL);
	}
}
	
//-----------------------------------------------------------------------------
//	Name:		OnSetInactive
//	Object:		
//	01-10-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::OnSetInactive()
{
	if ( _Flags.IsSet( IS_ACTIVE ) == true )
	{
		_Flags.Unset( IS_ACTIVE );

		COM_INSTANCE CI(GetHandle());
		GetClassDescriptor()->CallMethod(CMD_ON_SET_INACTIVE, &CI, NULL, NULL);
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsActive
//	Object:		
//	01-10-05:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsActive()
{
	return ( _Flags.IsSet( IS_ACTIVE ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetZDepth
//	Object:		
//	01-09-28:	ELE - Created
//-----------------------------------------------------------------------------
float	IHM_ELEMENT_ABC::GetZDepth()
{
	return	(_rZDepth);
}

//-----------------------------------------------------------------------------
//	Name:		SetZDepth
//	Object:		
//	02-02-28:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetZDepth(const float	rZ)
{
	_rZDepth = rZ;
}

//-----------------------------------------------------------------------------
//	Name:		IsUnder2DInterface
//	Object:		
//	01-10-22:	ELE - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsUnder2DInterface() const
{
	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		SetVerticesToReset
//	Object:		
//	01-11-07:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetVerticesToReset(bool	bValue)
{
	_bVerticesToReset = bValue;
}
/*
//-----------------------------------------------------------------------------
//	Name:		SetViewport
//	Object:		
//	01-10-01:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetViewport(VIEWPORT *	pViewport)
{
	if	(_pViewport != pViewport)
	{
		_pViewport = pViewport;
		_Flags.Set( HAS_VIEWPORT );
		_bVerticesToReset = true;
		_pInterface3D = NULL;
	}
}

//-----------------------------------------------------------------------------
//	Name:		HasViewport
//	Object:		
//	02-08-23:	AMA - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::HasViewport() const
{
	return ( _Flags.IsSet( HAS_VIEWPORT ) );	
}

//-----------------------------------------------------------------------------
//	Name:		GetViewport
//	Object:		
//	01-10-01:	ELE - Created
//-----------------------------------------------------------------------------
VIEWPORT *	IHM_ELEMENT_ABC::GetViewport() const
{
	return	(_pViewport);
}
*/
//-----------------------------------------------------------------------------
//	Name:		OnMouseIn
//	Object:		
//	01-10-12:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::OnMouseIn(IHM_INTERFACE *		pInterface,
								   IHM_PAGE *			pPage,
								   IHM_ELEMENTS_LIST *	pElementsList)
{
	if ( _Flags.IsSet( IS_MOUSE_ON ) == false )
	{
		_Flags.Set( IS_MOUSE_ON );

		COM_INSTANCE CI(GetHandle());
		GetClassDescriptor()->CallMethod(CMD_ON_MOUSE_IN, &CI, NULL, NULL);

		// Play the switching sound (if any).
		/*SOUND_DATA *pSound = pElementsList->GetSwitchingSound();
		if ( pSound != NULL )
		{
			SOUND::Instance()->Play( pSound->GetIndex(), 0, 1, 0 );
		}*/
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		OnMouseOut
//	Object:		
//	01-10-12:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::OnMouseOut(IHM_INTERFACE *		pInterface,
									IHM_PAGE *			pPage,
									IHM_ELEMENTS_LIST *	pElementsList)
{
	if ( _Flags.IsSet( IS_MOUSE_ON ) == true )
	{
		_Flags.Unset( IS_MOUSE_ON );

		COM_INSTANCE CI(GetHandle());
		GetClassDescriptor()->CallMethod(CMD_ON_MOUSE_OUT, &CI, NULL, NULL);
	}
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetScrollUpButtonFor
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::SetScrollUpButtonFor(IHM_ELEMENT_TEXT * pTextElementInstance)
{
	// Remove old flags.
	ClearFlags();

	_Flags.Set( SCROLLING_BUTTON_UP );

	_pScrollingTextElement = pTextElementInstance;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		SetScrollDownButtonFor
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::SetScrollDownButtonFor(IHM_ELEMENT_TEXT * pTextElementInstance)
{
	// Remove old flags.
	ClearFlags();

	_Flags.Set( SCROLLING_BUTTON_DOWN );

	_pScrollingTextElement = pTextElementInstance;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ClearFlags
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::ClearFlags()
{
	_Flags.Unset( OPENING_PAGE_ELEMENT| LEFT_RIGHT_ELEMENT | SCROLLING_BUTTON_UP | SCROLLING_BUTTON_DOWN );
}

//-----------------------------------------------------------------------------
//	Name:		SetAlwaysInactive
//	Object:		
//	01-12-04:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetAlwaysInactive(const bool	bAlwaysInactive)
{
	if ( bAlwaysInactive == true )
	{
		_Flags.Set( ALWAYS_INACTIVE );
	}
	else
	{
		_Flags.Unset( ALWAYS_INACTIVE );
	}
}

//-----------------------------------------------------------------------------
//	Name:		IsAlwaysInactive
//	Object:		
//	01-12-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsAlwaysInactive() const
{
	return ( _Flags.IsSet( ALWAYS_INACTIVE ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetAlwaysOnTop
//	Object:		
//	01-12-14:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetAlwaysOnTop(const bool	bAlwaysOnTop)
{
	_Flags.Set( ALWAYS_ON_TOP );
}

//-----------------------------------------------------------------------------
//	Name:		IsAlwaysOnTop
//	Object:		
//	01-12-14:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsAlwaysOnTop() const
{
	return ( _Flags.IsSet( ALWAYS_ON_TOP ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetBlinking
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetBlinking(const bool	bBlink)
{
	_Flags.Set( BLINKING );
}

//-----------------------------------------------------------------------------
//	Name:		IsBlinking
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsBlinking() const
{
	return ( _Flags.IsSet( BLINKING ) );
}

//-----------------------------------------------------------------------------
//	Name:		SetBlinkingTime
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetBlinkingTime(const float	rBlinkTimeMS)
{
	_rBlinkingTime = rBlinkTimeMS;
}

//-----------------------------------------------------------------------------
//	Name:		IsToBeDisplayed
//	Object:		
//	02-01-09:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::HasToBeDisplayed() const
{
	QDT_ASSERT(	_rBlinkingTime >= 0.0f );
	QDT_ASSERT( _rCurBlinkTime >= 0.0f );

	if ( IsVisible() == false )
	{
		return ( false );
	}

	if ( IsBlinking() == false )
	{
		return ( true );
	}
	
	// Blinking element.
	return ( _rCurBlinkTime < 0.5f * _rBlinkingTime );
}

//-----------------------------------------------------------------------------
//	Name:		IsValid
//	Object:		
//	01-10-23:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsValid()
{
	// Opening page element.
	if ( _Flags.IsSet( OPENING_PAGE_ELEMENT ) == true )
	{
		// Flags are OK.
		if ( _Flags.IsSet( LEFT_RIGHT_ELEMENT ) == true )
		{
			return ( false );
		}
		if ( _Flags.IsSet( SCROLLING_BUTTON_UP ) == true )
		{
			return ( false );
		}
		if ( _Flags.IsSet( SCROLLING_BUTTON_DOWN ) == true )
		{
			return ( false );
		}
	}
	// Left-right element.
	else
	if ( _Flags.IsSet( LEFT_RIGHT_ELEMENT ) == true )
	{
		if ( _Flags.IsSet( SCROLLING_BUTTON_UP ) == true )
		{
			return ( false );
		}
		if ( _Flags.IsSet( SCROLLING_BUTTON_DOWN ) == true )
		{
			return ( false );
		}

		if ( _LeftRightElements.GetSize() <= 0 )
		{
			return ( false );
		}
	}
	// Scroll up button
	else
	if ( _Flags.IsSet( SCROLLING_BUTTON_UP ) == true )
	{
		if ( _Flags.IsSet( SCROLLING_BUTTON_DOWN ) == true )
		{
			return ( false );
		}
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		IsTypeElementId
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::IsTypeElementId(OBJECT_TYPE	TypeId)
{
	if ( ( TypeId == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT     ) ||
		 ( TypeId == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_MESH     ) ||
		 ( TypeId == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP   ) ||
		 ( TypeId == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_QUAD     ) ||
		 ( TypeId == MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_TRIANGLE ) )
	{
		return ( true );
	}
	else
	{
		return ( false );
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetWire
//	Object:		
//	02-03-04:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetWire(bool	bValue)
{
	_bWired = bValue;
}

//-----------------------------------------------------------------------------
//	Name:		SetParentElementsList
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetParentContainer(IHM_ELEMENTS_LIST *	pParent)
{
	_pParentContainer = pParent;
	_Flags.Set( HAS_PARENT_CONTAINER );
}

//-----------------------------------------------------------------------------
//	Name:		HasParentContainer
//	Object:		
//	02-08-23:	AMA - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::HasParentContainer() const
{
	return ( _Flags.IsSet( HAS_PARENT_CONTAINER ) );
}

//-----------------------------------------------------------------------------
//	Name:		UnsetParentContainer
//	Object:		
//	02-08-23:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::UnsetParentContainer()
{
	_Flags.Unset( HAS_PARENT_CONTAINER );
	_pParentContainer = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		GetParentElementsList
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENTS_LIST *	IHM_ELEMENT_ABC::GetParentContainer() const
{
	return ( _pParentContainer );
}

//-----------------------------------------------------------------------------
//	Name:		HasInterfaceContainer
//	Object:		
//	02-08-26:	AMA - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::HasInterfaceContainer() const
{
	if ( HasParentContainer() )
	{
		IHM_ELEMENTS_LIST * pList = GetParentContainer();
		if ( pList->HasParentContainer() )
		{
			IHM_PAGE * pPage = pList->GetParentContainer();
			if ( pPage->HasParentContainer() )
			{
				return (true);
			}
		}
	}
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		GetInterfaceContainer
//	Object:		
//	02-07-03:	GGO - Created
//-----------------------------------------------------------------------------
IHM_INTERFACE *	IHM_ELEMENT_ABC::GetInterfaceContainer() const
{
	if ( HasParentContainer() )
	{
		IHM_ELEMENTS_LIST * pList = GetParentContainer();
		if ( pList->HasParentContainer() )
		{
			IHM_PAGE * pPage = pList->GetParentContainer();
			if ( pPage->HasParentContainer() )
			{
				return (pPage->GetParentContainer());
			}
		}
	}
	return (NULL);
}

//-----------------------------------------------------------------------------
//	Name:		Init
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pM;

	pM = METHOD_DESCRIPTOR__Allocate("SetOpeningPage", "Set the element as an opening page element and set the page to be opened when the user activates the current element.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "OpeningPage", "Ask 3D Engine to document this parameter.", IHM_PAGE::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_SET_OPENING_PAGE, pM, EM_SetOpeningPage);

	pM = METHOD_DESCRIPTOR__Allocate("AddLeftRightElement", "Set the element as a left-right element and add an element the user can parse trough using the left and right events.");
	METHOD_DESCRIPTOR__AddParameter(pM, "LeftRightElement", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor());
	pCD->AddMethod(CMD_ADD_LEFT_RIGHT_ELEMENT, pM, EM_AddLeftRightElement);

	pM = METHOD_DESCRIPTOR__Allocate("GetOpeningPage", "Retrieve the selection page.");
	METHOD_DESCRIPTOR__AddParameterReference(pM, "OpeningPage", "Ask 3D Engine to document this parameter.", IHM_PAGE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameterReference(pM, "bIsOpeningPageElement", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_GET_OPENING_PAGE, pM, EM_GetOpeningPage);

	pM = METHOD_DESCRIPTOR__Allocate("IsVisible", "Returns 'true' if the element is visible.");
	METHOD_DESCRIPTOR__SetReturn(pM, "bIsVisible", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_VISIBLE, pM, EM_IsVisible);

	pM = METHOD_DESCRIPTOR__Allocate("Show", "Show the element.");
	pCD->AddMethod(CMD_SHOW, pM, EM_Show);

	pM = METHOD_DESCRIPTOR__Allocate("Hide", "Hide the element.");
	pCD->AddMethod(CMD_HIDE, pM, EM_Hide);

	pM = METHOD_DESCRIPTOR__Allocate("OnSetActive", "Override this method to change the current element when getting active.");
	pCD->AddMethod(CMD_ON_SET_ACTIVE, pM, EM_OnSetActive);

	pM = METHOD_DESCRIPTOR__Allocate("OnSetInactive", "Override this method to change the current element when getting inactive.");
	pCD->AddMethod(CMD_ON_SET_INACTIVE, pM, EM_OnSetInactive);

	pM = METHOD_DESCRIPTOR__Allocate("IsActive", "Returns 'true' if the element is visible.");
	METHOD_DESCRIPTOR__SetReturn(pM, "bIsActive", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_ACTIVE, pM, EM_IsActive);

	pM = METHOD_DESCRIPTOR__Allocate("OnMouseIn", "Override this method to change the current element when the mouse gets on it.");
#if defined(PLATFORM_PC)
	pCD->AddMethod(CMD_ON_MOUSE_IN, pM, EM_OnMouseIn);
#else
	pCD->AddMethod(CMD_ON_MOUSE_IN, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("OnMouseOut", "Override this method to change the current element when the mouse gets out of it.");
#if defined(PLATFORM_PC)
	pCD->AddMethod(CMD_ON_MOUSE_OUT, pM, EM_OnMouseOut);
#else
	pCD->AddMethod(CMD_ON_MOUSE_OUT, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("SetScrollUpButtonFor", "Attach this element to a text and use it to scroll up the attached text. Please note that if a text entirely fits in its bounding rect, there's no scroll.");
	METHOD_DESCRIPTOR__AddParameter(pM, "ElementText", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor());
#if defined(PLATFORM_PC)
	pCD->AddMethod(CMD_SET_SCROLL_UP_BUTTON_FOR, pM, EM_SetScrollUpButtonFor);
#else
	pCD->AddMethod(CMD_SET_SCROLL_UP_BUTTON_FOR, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("SetScrollDownButtonFor", "Attach this element to a text and use it to scroll up the attached text. Please note that if a text entirely fits in its bounding rect, there's no scroll.");
	METHOD_DESCRIPTOR__AddParameter(pM, "ElementText", "Ask 3D Engine to document this parameter.", IHM_ELEMENT_TEXT::GetGenericCommunicatorDescriptor());
#if defined(PLATFORM_PC)
	pCD->AddMethod(CMD_SET_SCROLL_DOWN_BUTTON_FOR, pM, EM_SetScrollDownButtonFor);
#else
	pCD->AddMethod(CMD_SET_SCROLL_DOWN_BUTTON_FOR, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("SetAlwaysInactive", "If set to 'true', the element will receive no input at all. For example, use it to display bitmap on screen that should not diseappear when the user clicks on it.");
	METHOD_DESCRIPTOR__AddParameter(pM, "bAlwaysInactive", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_ALWAYS_INACTIVE, pM, EM_SetAlwaysInactive);

	pM = METHOD_DESCRIPTOR__Allocate("OnMouseClick", "Override this command if you want to perform special actions when the user clicks on a element.\r\nIMPORTANT: You cannot override the 'OnMouseClick' method and set an opening page (for example). If so, a click will open the associated opening page, ignoring the 'OnMouseClick' method override.");
	METHOD_DESCRIPTOR__AddParameter(pM, "Interface", "Ask 3D Engine to document this parameter.", IHM_INTERFACE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "Page", "Ask 3D Engine to document this parameter.", IHM_PAGE::GetGenericCommunicatorDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "ElementsList", "Ask 3D Engine to document this parameter.", IHM_ELEMENTS_LIST::GetGenericCommunicatorDescriptor());
#if defined(PLATFORM_PC)
	pCD->AddMethod(CMD_ON_MOUSE_CLICK, pM, EM_OnMouseClick);
#else
	pCD->AddMethod(CMD_ON_MOUSE_CLICK, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("OnMouseMove", "Override this command if you want to perform special actions when the user moves the mouse.");
	METHOD_DESCRIPTOR__AddParameter(pM, "nMousePosX", "The mouse cursor horizontal position in pixel coordinates.", TYPE_INT::GetDescriptor());
	METHOD_DESCRIPTOR__AddParameter(pM, "nMousePosY", "The mouse cursor vertical position in pixel coordinates.", TYPE_INT::GetDescriptor());
#if defined(PLATFORM_PC)
	pCD->AddMethod(CMD_ON_MOUSE_MOVE, pM, EM_OnMouseMove);
#else
	pCD->AddMethod(CMD_ON_MOUSE_MOVE, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

	pM = METHOD_DESCRIPTOR__Allocate("DisplayElement", "Display an ihm_element into the frame buffer. WARNING: This method should be called within the 3d engine display loop to have an effect.");
	pCD->AddMethod(CMD_DISPLAY_ELEMENT, pM, EM_DisplayElement);

	pM = METHOD_DESCRIPTOR__Allocate("SetAlwaysOnTop", "If set to 'true', the element is displayed after the other ones. It is therefore always visible.");
	METHOD_DESCRIPTOR__AddParameter(pM, "bAlwaysOnTop", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_ALWAYS_ON_TOP, pM, EM_SetAlwaysOnTop);

	pM = METHOD_DESCRIPTOR__Allocate("SetBlinking", "Make the element blink.");
	METHOD_DESCRIPTOR__AddParameter(pM, "bBlink", "Ask 3D Engine to document this parameter.", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_SET_BLINKING, pM, EM_SetBlinking);

	pM = METHOD_DESCRIPTOR__Allocate("SetBlinkingTime", "The time of the blink (in ms). Default is 1 s.");
	METHOD_DESCRIPTOR__AddParameter(pM, "rBlinkTimeMs", "Ask 3D Engine to document this parameter.", TYPE_FLOAT::GetDescriptor());
	pCD->AddMethod(CMD_SET_BLINKING_TIME, pM, EM_SetBlinkingTime);

	pM = METHOD_DESCRIPTOR__Allocate("ResizeOn169", "Tells 3DEngine to resize the element in 16/9 screen mode (avoid streching effect).");
	METHOD_DESCRIPTOR__AddParameter(pM, "bValue", "", TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_RESIZE_ON_169, pM, EM_ResizeOn169);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetOpeningPage
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_SetOpeningPage(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
	
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	COM_INSTANCE ** pComInstance = (COM_INSTANCE **) pP;

	IHM_PAGE * pPage;
	pPage = (IHM_PAGE *) (**pComInstance).GetCommunicator();
	pIhmElement->SetOpeningPage(pPage);
}

//-----------------------------------------------------------------------------
//	Name:		EM_AddLeftRightElement
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_AddLeftRightElement(void * const	pO,
												void * const	pR,
												void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
	
	pObjet		= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	COM_INSTANCE * pComInstance = (COM_INSTANCE *) pP;
	IHM_ELEMENT_ABC * pElement = (IHM_ELEMENT_ABC *) (*pComInstance).GetCommunicator();
	pIhmElement->AddLeftRightElement(pElement);
}

//-----------------------------------------------------------------------------
//	Name:		EM_GetOpeningPage
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_GetOpeningPage(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);
	
	struct	PARAMETERS
	{
		COM_INSTANCE *	_pOpeningPage;
		bool *			_bIsOpeningPageElement;
	};

	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;
	
	COM_INSTANCE * pComInstance = pParameters->_pOpeningPage;
	QDT_NEW_PLACED(pComInstance, COM_INSTANCE)(pIhmElement->GetOpeningPage()->GetHandle());

	// Element has an opening page.
	if ( pIhmElement->_Flags.IsSet( OPENING_PAGE_ELEMENT ) )
	{
		*pParameters->_bIsOpeningPageElement = true;
	}
	// Element has no opening page.
	else
	{
		*pParameters->_bIsOpeningPageElement = false;
	}
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsVisible
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_IsVisible(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	bool * bIsVisible = (bool *) pR;
	
	*bIsVisible = (bool) pIhmElement->IsVisible();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Show
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_Show(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	pIhmElement->Show();
}

//-----------------------------------------------------------------------------
//	Name:		EM_Hide
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_Hide(void * const	pO,
								 void * const	pR,
								 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	pIhmElement->Hide();
}

//-----------------------------------------------------------------------------
//	Name:		EM_OnSetActive
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_OnSetActive(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		EM_OnSetInactive
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_OnSetInactive(void * const	pO,
										  void * const	pR,
										  void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	// Do Nothing...
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsActive
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_IsActive(void * const	pO,
									 void * const	pR,
									 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	bool * bIsActive = (bool *) pR;
	*bIsActive = (bool) pIhmElement->IsActive();
}

//-----------------------------------------------------------------------------
//	Name:		EM_OnMouseIn
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::EM_OnMouseIn(void * const	pO,
									  void * const	pR,
									  void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	// Do Nothing...
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_OnMouseOut
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::EM_OnMouseOut(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	// Do Nothing...
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_SetScrollUpButtonFor
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::EM_SetScrollUpButtonFor(void * const	pO,
												 void * const	pR,
												 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	COM_INSTANCE * pComInstance = (COM_INSTANCE *) pP;
	IHM_ELEMENT_TEXT * pElement = (IHM_ELEMENT_TEXT *) (*pComInstance).GetCommunicator();
	pIhmElement->SetScrollUpButtonFor(pElement);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_SetScrollDownButtonFor
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::EM_SetScrollDownButtonFor(void * const	pO,
												   void * const	pR,
												   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	COM_INSTANCE * pComInstance = (COM_INSTANCE *) pP;
	IHM_ELEMENT_TEXT * pElement = (IHM_ELEMENT_TEXT *) (*pComInstance).GetCommunicator();
	pIhmElement->SetScrollDownButtonFor(pElement);
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_SetAlwaysInactive
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_SetAlwaysInactive(void * const	pO,
											  void * const	pR,
											  void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	bool * bAlwaysInactive = (bool *) pP;
	pIhmElement->SetAlwaysInactive(*bAlwaysInactive);
}

//-----------------------------------------------------------------------------
//	Name:		EM_OnMouseClick
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::EM_OnMouseClick(void * const	pO,
										 void * const	pR,
										 void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	struct	PARAMETERS
	{
		COM_INSTANCE 	_Interface;
		COM_INSTANCE 	_Page;
		COM_INSTANCE 	_ElementsList;
	};
	
	PARAMETERS *	pParameters;
	pParameters = (PARAMETERS *)pP;

	IHM_INTERFACE *pInterface = (IHM_INTERFACE*) pParameters->_Interface.GetCommunicator();
	QDT_ASSERT( pInterface );
	if ( pInterface == NULL )
	{
		QDT_Warning(QDT_COMMENT("In IHM_ELEMENT_ABC::EM_OnMouseClick: invalid interface !"));
		return;
	}

	// Default processing for a click on an element is to disactivate the interface.
	pInterface->Disactivate();
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_OnMouseMove
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
void	IHM_ELEMENT_ABC::EM_OnMouseMove(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	// Do Nothing...
}
#endif

//-----------------------------------------------------------------------------
//	Name:		EM_DisplayElement
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_DisplayElement(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	pIhmElement->DisplayElement(NULL);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetAlwaysOnTop
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_SetAlwaysOnTop(void * const	pO,
										   void * const	pR,
										   void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	bool * bAlwaysOnTop = (bool *) pP;

	pIhmElement->SetAlwaysOnTop(*bAlwaysOnTop);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetBlinking
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_SetBlinking(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	bool * bBlinking = (bool *) pP;

	pIhmElement->SetBlinking(*bBlinking);
}

//-----------------------------------------------------------------------------
//	Name:		EM_SetBlinkingTime
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_SetBlinkingTime(void * const	pO,
											void * const	pR,
											void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	float * nBlinkingTime = (float *) pP;

	pIhmElement->SetBlinkingTime(*nBlinkingTime);
}

//-----------------------------------------------------------------------------
//	Name:		EM_ResizeOn169
//	Object:		
//	05-04-20:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::EM_ResizeOn169(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	COM_INSTANCE *		pObjet;	
	IHM_ELEMENT_ABC *	pIhmElement;
		
	pObjet	= (COM_INSTANCE *) pO;
	pIhmElement	= (IHM_ELEMENT_ABC *) pObjet->GetCommunicator();
	QDT_ASSERT(pIhmElement);

	bool * b = (bool *) pP;

	pIhmElement->Set169Resizing(*b);
}

//-----------------------------------------------------------------------------
//	Name:		GetGenericCommunicatorDescriptor
//	Object:		
//	02-08-19:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_ABC::GetGenericCommunicatorDescriptor()
{
	return ( COMMUNICATOR_DESCRIPTOR::FindComDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_ABC) );
}

//-----------------------------------------------------------------------------
//	Name:		CreateGenericCommunicatorDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
COMMUNICATOR_DESCRIPTOR *	IHM_ELEMENT_ABC::CreateGenericCommunicatorDescriptor()
{
	return (new COMMUNICATOR_DESCRIPTOR(QDT_COMMENT("IHM_ELEMENT_ABC"), 
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_IHM_ELEMENT_ABC, 
										QDT_COMMENT("2C4029A8-990D-840A-C78E-DF8F1FE835C8"),
										sizeof(IHM_ELEMENT_ABC), 
										Init, 
										QDT_COMMENT("Abstract Class for all IHM_ELEMENT_* .")));
}

//-----------------------------------------------------------------------------
//	Name:		SetInterface
//	Object:		
//	02-12-06:	RMA - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetInterface3D(INTERFACE_3D *	pInterface)
{
	_pInterface3D = pInterface;
	_Flags.Unset(HAS_VIEWPORT);
	_bVerticesToReset = true;
//	_pViewport = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		GetInterface
//	Object:		
//	02-12-06:	RMA - Created
//-----------------------------------------------------------------------------
INTERFACE_3D *	IHM_ELEMENT_ABC::GetInterface3D() const
{
	return (_pInterface3D);
}

//-----------------------------------------------------------------------------
//	Name:		Set169Resizing
//	Object:		
//	05-04-14:	ELE - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::Set169Resizing(bool	b)
{
	if	(b)
	{
		_Flags.SetFlags(RESIZE_ON_169, RESIZE_ON_169);
	}
	else
	{
		_Flags.SetFlags(RESIZE_ON_169, 0);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetLoadingScreenIhm
//	Object:		
//	05-05-17:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_ABC::SetLoadingScreenIhm(bool	b)
{
	_Flags.Set( LOADING_SCREEN_IHM, b );
}

//-----------------------------------------------------------------------------
//	Name:		GetLoadingScreenIhm
//	Object:		
//	05-05-17:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_ABC::GetLoadingScreenIhm() const
{
	return ( _Flags.IsSet( LOADING_SCREEN_IHM ) );
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
