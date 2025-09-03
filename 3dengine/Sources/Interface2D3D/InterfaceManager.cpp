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
//	CLASS:	INTERFACE_MANAGER
//
//	01-09-24:	GGO - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(Interface2D3D/InterfaceManager)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Elements/IHM_ElementABC)
#include	INCL_3DENGINE(Interface2D3D/IHM/IHM_Interface)
#include	INCL_3DENGINE(Display/DisplayManager)
#include	INCL_3DENGINE(M3D/Moteur3DModule)
#include	INCL_KINPUT(Input/InputObjectAbc)
#include	INCL_KINPUT(Input/InputDeviceInfo)
#include	INCL_KCOM(Services/ComService)
#include	INCL_KCOM(ComBasis/SingletonDescriptor)
#include	INCL_KCOM(ComBasis/MethodDescriptor)
#include	INCL_KCOM(BaseType/TypeBool)
#include	INCL_KSERV(Input/Input)
#include	INCL_KSERV(Input/DeviceObserver)
#include	INCL_KCORE(Time/Time_ABC)
#include	INCL_KCORE(OS/OS)
#include	INCL_KRENDERER(Renderer\RenderContext)

//-----------------------------------------------------------------------------
//	Name:		OnEventMouse
//	Object:		
//	01-09-21:	GGO - Created
//-----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
static	
void	OnEventMouse(IntPtr							nParam,
					 INPUT_OBJECT_ABC::OBJ_TYPE		ObjType,
					 int							ObjIndex,
					 int							nVal)
{
	// The interface manager.
	INTERFACE_MANAGER *pInterfaceManager = reinterpret_cast<INTERFACE_MANAGER*>( nParam );
	QDT_ASSERT(pInterfaceManager);

	// The window size.
	int nLeftWindowPos, nTopWindowPos;
	nLeftWindowPos	= 0;//RENDER_MANAGER::Instance()->GetRenderContext()->GetWindowPosX();
	nTopWindowPos	= 0;//RENDER_MANAGER::Instance()->GetRenderContext()->GetWindowPosY();

	// GGO - 24/09/2001
	// Dirty patch to get the mouse position.
	// INPUT should allow to get the absolute mouse position.
	int nXPos = 0;
	int nYPos = 0;
	
	OS::GetCursorPos( nXPos, nYPos );
	
	pInterfaceManager->SetMousePosX( nXPos - nLeftWindowPos );
	pInterfaceManager->SetMousePosY( nYPos - nTopWindowPos  );
	// GGO - 24/09/2001

	QDT_TRY
	{
		switch( ObjType )
		{
		case INPUT_OBJECT_ABC::OBJ_X_AXIS_REL:
			//_nMousePosX += nVal;
			break;

		case INPUT_OBJECT_ABC::OBJ_Y_AXIS_REL:
			//_nMousePosY += nVal;
			break;

		case INPUT_OBJECT_ABC::OBJ_BUTTON:
			if ( ObjIndex == 0 )
			{
				if ( pInterfaceManager->_bLButtonDown == false )
				{
					pInterfaceManager->SetInputValue( INTERFACE_MANAGER::ON_LBUTTON );
					pInterfaceManager->_bLButtonDown = true;
				}
				else
				{
					pInterfaceManager->_bLButtonDown = false;
				}

			}
			break;

		}
	}
	QDT_UNHANDLED_CATCH

}
#endif

//-----------------------------------------------------------------------------
//	Name:		OnEventKeyboard
//	Object:		
//	01-09-21:	GGO - Created
//-----------------------------------------------------------------------------
static	
void	OnEventKeyboard(IntPtr						nParam,
					    INPUT_OBJECT_ABC::OBJ_TYPE	ObjType,
					    int							ObjIndex,
					    int							nVal)
{
	int nEventValue = 0;

	QDT_TRY
	{
		if ( ObjType == INPUT_OBJECT_ABC::OBJ_KEY )
		{
			if ( nVal > 0 )
			{
				switch ( ObjIndex )
				{
				case 104: // Up.
					nEventValue |= INTERFACE_MANAGER::ON_UP;
					break;

				case 109: // Down.
					nEventValue |= INTERFACE_MANAGER::ON_DOWN;
					break;

				case 106: // Left.
					nEventValue |= INTERFACE_MANAGER::ON_LEFT;
					break;

				case 107: // Right.
					nEventValue |= INTERFACE_MANAGER::ON_RIGHT;
					break;

				case 27: // Return.
				case 56: // Space
				case 90: // Return num.
					nEventValue |= INTERFACE_MANAGER::ON_OK;
					break;
				}


				QDT_ASSERT( reinterpret_cast<INTERFACE_MANAGER*>( nParam ) );
				reinterpret_cast<INTERFACE_MANAGER*>( nParam )->SetInputValue( nEventValue );
			}
		}
	}
	QDT_UNHANDLED_CATCH

}

//-----------------------------------------------------------------------------
//	Name:		INTERFACE_MANAGER constructor
//	Object:		
//	01-09-24:	GGO - Created
//-----------------------------------------------------------------------------
INTERFACE_MANAGER::INTERFACE_MANAGER()
:
_pLocalDatabase			( NULL ),
_bLButtonDown			( false ),
_bIHMDebugDisplayEnable	( false ),
_nWindowWidth			( 0 ),
_nWindowHeight			( 0 )
{
	_pLocalDatabase = COM_SERVICE::Instance()->CreateDynamicDatabase("InterfaceManagerDatabase");

	int					nDeviceId;
	INPUT_DEVICE_INFO	DeviceInfo;

#if defined(PLATFORM_PC)
	// Build Mouse Device Info
	DeviceInfo.SetNbrXRelAxes	( INPUT::MOUSE_DEFAULT_NB_XAXIS );
	DeviceInfo.SetNbrYRelAxes	( INPUT::MOUSE_DEFAULT_NB_YAXIS );
	DeviceInfo.SetNbrButtons	( INPUT::MOUSE_DEFAULT_NB_BUTTONS );

	// Get a mouse.
	nDeviceId = INPUT::Instance()->GetDevice( DeviceInfo );
	
	if( nDeviceId != -1 )
	{
		_MouseObserver.AddEventCallback( OnEventMouse, reinterpret_cast<IntPtr>( this ) );
		bool bMouseFound = INPUT::Instance()->AddDeviceObserver( nDeviceId, &_MouseObserver );

		QDT_ASSERT( bMouseFound );
	}
#endif

	// Build Keyboard Device Info
	DeviceInfo.Reset();
	DeviceInfo.SetNbrKeys( INPUT::KEYBOARD_DEFAULT_NB_KEYS );

	// Get a keyboard.
	nDeviceId = INPUT::Instance()->GetDevice( DeviceInfo );

	if( nDeviceId != -1 )
	{
		_KeyboardObserver.AddEventCallback( OnEventKeyboard, reinterpret_cast<IntPtr>( this ) );
		bool bKeyboardFound = INPUT::Instance()->AddDeviceObserver( nDeviceId, &_KeyboardObserver );

		QDT_ASSERT( bKeyboardFound );
	}
}	

//-----------------------------------------------------------------------------
//	Name:		~INTERFACE_MANAGER destructor
//	Object:		
//	01-09-24:	GGO - Created
//	01-10-15:	RMA - Modified : DEVICE_OBSERVER are killed by database when
//				exit!! No need to clean them else be sure they haven't been
//				cleaned before (crash)
//-----------------------------------------------------------------------------
INTERFACE_MANAGER::~INTERFACE_MANAGER()
{
#if defined(PLATFORM_PC)
	INPUT::Instance()->RemoveDeviceObserver(&_MouseObserver);
#endif
	INPUT::Instance()->RemoveDeviceObserver(&_KeyboardObserver);

	//not ClearAll ..can be already deleted
	//vIndexIntToProtect.Clear();
	COM_SERVICE::Instance()->DeleteDatabase(_pLocalDatabase);
	
	//Clean the interfaces
	ClearAllInterfaces();
}

//-----------------------------------------------------------------------------
//	Name:		Instance
//	Object:		
//	01-09-24:	GGO - Created
//-----------------------------------------------------------------------------
INTERFACE_MANAGER *	INTERFACE_MANAGER::Instance()
{
	return (static_cast<INTERFACE_MANAGER *>(GetSingletonDescriptor()->Instance()));
}

//-----------------------------------------------------------------------------
//	Name:		Suicide
//	Object:		
//	01-09-24:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::Suicide()
{
	GetSingletonDescriptor()->Release();
}

//-----------------------------------------------------------------------------
//	Name:		ClearAllInterfaces
//	Object:		
//	01-11-28:	DAC - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::ClearAllInterfaces()
{
	/*INDEX_INT							*pII;
	QDT_DLIST<COM_INSTANCE>::ITERATOR		it, itEnd;
	
	it		= _lstInterfaces.GetHead();
	itEnd	= _lstInterfaces.GetTail();

	while(it != itEnd)
	{
		pII = (*it);
		QDT_ASSERT(pII);
		
		if	(pII->IsDynamic())
		{
			pII->RemoveReference();
			//DATABASE_MANAGER::Instance()->Delete( pII->GetType(), pII->GetId() );
		}
		++it;
		
	}*/
	
	_lstInterfaces.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		
//	01-09-24:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::Loop(float	rTime)
{
	Update();

	HandleInterfaces( rTime );

	_nInputValue = 0;
}

//-----------------------------------------------------------------------------
//	Name:		Update
//	Object:		
//	02-04-05:	ELE - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::Update()
{

/*	if	(RENDER_CONTEXT::IsFullScreen() == false)
	{
		_nWindowWidth	= RENDER_CONTEXT::GetScreenWidth();
		_nWindowHeight	= RENDER_CONTEXT::GetScreenHeight();
	}
	else
	{
		int			nCurrentAdapter = RENDER_MANAGER::Instance()->GetCurrentAdapter();
		int			nCurrentMode = RENDER_MANAGER::Instance()->GetCurrentAdapterMode();
		RENDER_MODE	Mode;

		RENDER_MANAGER::Instance()->GetRenderContext()->GetAdapterMode(nCurrentAdapter, nCurrentMode, Mode);

		if ( (_nWindowWidth != Mode.Width) || (_nWindowHeight != Mode.Height) )
		{	
			_nWindowWidth = Mode.Width;
			_nWindowHeight = Mode.Height;

			DISPLAY_MANAGER::Instance()->Update2D();
		}
	}*/

	_nWindowWidth	= RENDER_CONTEXT::GetScreenWidth();
	_nWindowHeight	= RENDER_CONTEXT::GetScreenHeight();

	DISPLAY_MANAGER::Instance()->Update2D();
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::Display()
{
//	COLOR	Color(0, 0, 0, 0);
//	RENDER_MANAGER::Instance()->GetRenderContext()->ClearBackground(RENDER_CONTEXT::CLEARBACKGROUND_DEPTHBUFFER, Color, 1.0f, 0.0f);

	//RENDER_MANAGER::Instance()->GetDisplayWindowSize( _nWindowWidth, _nWindowHeight );
	
	DisplayInterfaces();

	IHM_ELEMENT_ABC * pElement;

	// We display the always on top elements.
	for ( unsigned int i = 0; i < _vAlwaysOnTopElements.GetSize(); ++i )
	{
		//QDT_ASSERT( _vAlwaysOnTopElements[ i ] );
		pElement = (IHM_ELEMENT_ABC *) _vAlwaysOnTopElements[ i ].GetCommunicator();
		
		pElement->DisplayElement(NULL);

#ifndef _MASTER
		if ( IsIHMDebugDisplayEnabled() == true )
		{
			pElement->DebugDisplay(NULL);
		}
#endif
	}
	_vAlwaysOnTopElements.Clear();
}

//-----------------------------------------------------------------------------
//	Name:		HandleInterfaces
//	Object:		
//	01-09-25:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::HandleInterfaces(float	rTime)
{
	IHM_INTERFACE	*pInterface;

	// No interface.
	if ( _lstInterfaces.IsEmpty() == true )
	{
		return;
	}


	// Remove destroyed interfaces.
	QDT_DLIST<COM_INSTANCE>::ITERATOR	ItCur = _lstInterfaces.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR	ItEnd = _lstInterfaces.GetTail();
	while ( ItCur != ItEnd )
	{	
		//QDT_ASSERT( *ItCur );
		
		pInterface = static_cast<IHM_INTERFACE*>( (*ItCur).GetCommunicator() );
		QDT_ASSERT( pInterface );
		
		//increment iterator before calling any operations, so if the interface
		//is removed it doesn't affect us.
		++ItCur;

		
		pInterface->Handle( _nInputValue );
		pInterface->HandleTime(static_cast<float>(TIME::Instance()->GetDefaultClock()->GetDeltaFrameTime()) );
#if defined(PLATFORM_PC)
		pInterface->HandleMouse( _nInputValue, _nMousePosX, _nMousePosY );	
#endif
	}
}

//-----------------------------------------------------------------------------
//	Name:		DisplayInterfaces
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::DisplayInterfaces()
{
	IHM_INTERFACE	*pInterface;


	// No interface.
	if ( _lstInterfaces.IsEmpty() == true )
	{
		return;
	}


	// Add the element to display in '_vElementsToDraw'.
	QDT_DLIST<COM_INSTANCE>::ITERATOR	ItCur = _lstInterfaces.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR	ItEnd = _lstInterfaces.GetTail();

	while ( ItCur != ItEnd )
	{
		//QDT_ASSERT(  *ItCur );
		QDT_ASSERT( (*ItCur).GetCommunicator() );

		// The current interface.
		pInterface = static_cast<IHM_INTERFACE*>( (*ItCur).GetCommunicator() );
		pInterface->Display();

		++ItCur;
	}
}

//-----------------------------------------------------------------------------
//	Name:		AddInterface
//	Object:		
//	01-09-25:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::AddInterface(const COM_INSTANCE &Interface)
{
	//QDT_ASSERT( pInterface );

	// Check if the interface isn't already in the list.	
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItCur = _lstInterfaces.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd = _lstInterfaces.GetTail();

	while ( ItCur != ItEnd )
	{
		if ( *ItCur == Interface )
		{
			return;
		}

		++ItCur;
	}


	// Add the interface.
	//if ( Interface->GetIndex() != NULL )
//	{
		_lstInterfaces.PushTail( Interface );
	//	pInterface->GetIndex()->AddReference();
	//}
}


//-----------------------------------------------------------------------------
//	Name:		RemoveInterface
//	Object:		
//	01-09-26:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::RemoveInterface(const COM_INSTANCE &Interface)
{
	//QDT_ASSERT( pInterface );

	if ( _lstInterfaces.IsEmpty() == true )
	{
		return;
	}


	QDT_DLIST<COM_INSTANCE>::ITERATOR ItCur = _lstInterfaces.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd = _lstInterfaces.GetTail();

	while ( ItCur != ItEnd )
	{
		if ( *ItCur == Interface )
		{
			_lstInterfaces.Remove( ItCur );
			//pInterface->GetIndex()->RemoveReference();
			return;
		}

		++ItCur;
	}
}


//-----------------------------------------------------------------------------
//	Name:		IsInterfaceActivated
//	Object:		
//	01-10-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERFACE_MANAGER::IsInterfaceActivated(const COM_INSTANCE &Interface)
{
//	QDT_ASSERT( pInterface );

	QDT_DLIST<COM_INSTANCE>::ITERATOR ItCur = _lstInterfaces.GetHead();
	QDT_DLIST<COM_INSTANCE>::ITERATOR ItEnd = _lstInterfaces.GetTail();

	while ( ItCur != ItEnd )
	{
		if ( *ItCur == Interface )
		{
			return ( true );
		}

		++ItCur;
	}

	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetMousePosX
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::SetMousePosX(int	nX)
{
	_nMousePosX = nX;
}


//-----------------------------------------------------------------------------
//	Name:		SetMousePosY
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::SetMousePosY(int	nY)
{
	_nMousePosY = nY;
}


//-----------------------------------------------------------------------------
//	Name:		GetMousePosX
//	Object:		
//	02-01-24:	GGO - Created
//-----------------------------------------------------------------------------
int	INTERFACE_MANAGER::GetMousePosX() const
{
	return ( _nMousePosX );
}


//-----------------------------------------------------------------------------
//	Name:		GetMousePosY
//	Object:		
//	02-01-24:	GGO - Created
//-----------------------------------------------------------------------------
int	INTERFACE_MANAGER::GetMousePosY() const
{
	return ( _nMousePosY );
}

//-----------------------------------------------------------------------------
//	Name:		SetInputValue
//	Object:		
//	01-09-27:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::SetInputValue(int	nInputValue)
{
	_nInputValue |= nInputValue;
}


//-----------------------------------------------------------------------------
//	Name:		GetWindowSize
//	Object:		
//	01-11-12:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::GetWindowSize(int &	nWidth,
										 int &	nHeight) const
{
	nWidth	= _nWindowWidth;
	nHeight	= _nWindowHeight;
}


//-----------------------------------------------------------------------------
//	Name:		GetWindowWidth
//	Object:		
//	01-11-12:	GGO - Created
//-----------------------------------------------------------------------------
int	INTERFACE_MANAGER::GetWindowWidth() const
{
	return ( _nWindowWidth );
}


//-----------------------------------------------------------------------------
//	Name:		GetWindowHeight
//	Object:		
//	01-11-12:	GGO - Created
//-----------------------------------------------------------------------------
int	INTERFACE_MANAGER::GetWindowHeight() const
{
	return ( _nWindowHeight );
}


//-----------------------------------------------------------------------------
//	Name:		AddAlwaysOnTopElement
//	Object:		
//	01-12-14:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::AddAlwaysOnTopElement(const COM_INSTANCE &Element)
{
	//QDT_ASSERT( pElement );

	_vAlwaysOnTopElements.PushTail( Element );
}

#ifndef _MASTER

//-----------------------------------------------------------------------------
//	Name:		EnableIHMDebugDisplay
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::EnableIHMDebugDisplay(const bool	bEnable)
{
	_bIHMDebugDisplayEnable = bEnable;
}

//-----------------------------------------------------------------------------
//	Name:		IsIHMDebugDisplayEnabled
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERFACE_MANAGER::IsIHMDebugDisplayEnabled() const
{
	return ( _bIHMDebugDisplayEnable );
}

//-----------------------------------------------------------------------------
//	Name:		GetIHMDebugDisplayColor
//	Object:		
//	02-06-26:	GGO - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT	INTERFACE_MANAGER::GetIHMDebugDisplayColor() const
{
	return ( COLOR_FLOAT( 1.0f, 1.0f, 0.0f, 0.0f ) );
}

#endif

//-----------------------------------------------------------------------------
//	Name:		COM_Init
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::COM_Init(CLASS_DESCRIPTOR * const	pCD)
{
	METHOD_DESCRIPTOR *	pMD;

	pCD->SetConstructors(EM_Construct, EM_Destruct);

#ifndef _MASTER
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("EnableIHMDebugDisplay"), QDT_COMMENT("If enabled, segments are displayed around every ihm_element."));
	pMD->AddParameter(QDT_COMMENT("bEnable"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_ENABLE_IHM_DEBUG_DISPLAY, pMD, EM_EnableIHMDebugDisplay);
#else
	pCD->AddMethod(CMD_ENABLE_IHM_DEBUG_DISPLAY, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif

#ifndef _MASTER
	pMD = METHOD_DESCRIPTOR::Allocate(QDT_COMMENT("IsIHMDebugDisplayEnabled"), QDT_COMMENT("Return 'true' if the debug display of the ihm_element is enabled."));
	pMD->SetReturn(QDT_COMMENT("bEnable"), QDT_COMMENT(""), TYPE_BOOL::GetDescriptor());
	pCD->AddMethod(CMD_IS_IHM_DEBUG_DISPLAY_ENABLED, pMD, EM_IsIHMDebugDisplayEnabled);
#else
	pCD->AddMethod(CMD_IS_IHM_DEBUG_DISPLAY_ENABLED, CLASS_DESCRIPTOR::_pDoNothingMD, CLASS_DESCRIPTOR::EM_DoNothing);
#endif
}

//-----------------------------------------------------------------------------
//	Name:		GetSingletonDescriptor
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	INTERFACE_MANAGER::GetSingletonDescriptor()
{
	return	(SINGLETON_DESCRIPTOR::FindSingDescriptor(MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_INTERFACE_MANAGER));
}

//-----------------------------------------------------------------------------
//	Name:		CreateSingletonDescriptor
//	Object:		
//	05-09-29:	AMA - Created
//-----------------------------------------------------------------------------
SINGLETON_DESCRIPTOR *	INTERFACE_MANAGER::CreateSingletonDescriptor()
{
	return (new SINGLETON_DESCRIPTOR(	QDT_COMMENT("INTERFACE_MANAGER"),
										MOTEUR_3D_MODULE::OTYPE_MOTEUR3D_INTERFACE_MANAGER, 
										QDT_COMMENT("3A338BCF-CE98-CF69-2171-1DE680E3BED7"),
										sizeof(INTERFACE_MANAGER), 
										COM_Init,
										QDT_COMMENT("The interface manager singleton")));
}

//-----------------------------------------------------------------------------
//	Name:		EM_Construct
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::EM_Construct(void * const	pO,
										void * const	pR,
										void * const	pP)
{
	INTERFACE_MANAGER *pIM = static_cast<INTERFACE_MANAGER *>(pO);

	QDT_NEW_PLACED(pIM, INTERFACE_MANAGER)();
	QDT_ASSERT(pIM);
}

//-----------------------------------------------------------------------------
//	Name:		EM_Destruct
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::EM_Destruct(void * const	pO,
									   void * const	pR,
									   void * const	pP)
{
	INTERFACE_MANAGER *pIM = static_cast<INTERFACE_MANAGER *>(pO);

	QDT_DELETE_PLACED(pIM, INTERFACE_MANAGER);
}
								
#ifndef _MASTER
//-----------------------------------------------------------------------------
//	Name:		EM_EnableIHMDebugDisplay
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::EM_EnableIHMDebugDisplay(void * const	pO,
													void * const	pR,
													void * const	pP)
{
	bool	*pParameter = static_cast<bool *>(pP);
	
	INTERFACE_MANAGER::Instance()->EnableIHMDebugDisplay(*pParameter);
}

//-----------------------------------------------------------------------------
//	Name:		EM_IsIHMDebugDisplayEnabled
//	Object:		
//	03-03-26:	ELE - Created
//-----------------------------------------------------------------------------
void	INTERFACE_MANAGER::EM_IsIHMDebugDisplayEnabled(void * const	pO,
													   void * const	pR,
													   void * const	pP)
{
	bool	*pReturn = static_cast<bool *>(pR);
	
	*pReturn = INTERFACE_MANAGER::Instance()->IsIHMDebugDisplayEnabled();
}
#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
