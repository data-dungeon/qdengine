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
//	CLASS:	IHM_ELEMENT_TEXT_DESC
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\..\Include.h"
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_3DENGINE(M3D)
#include	INC_KRNCOM(OldCom/Communication)
#include	INC_KRNServ(GenericDisplay)


#ifndef _DEBUG

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

IHM_ELEMENT_TEXT_DESC*	IHM_ELEMENT_TEXT_DESC::_pInstance      = NULL;
bool					IHM_ELEMENT_TEXT_DESC::_bDestroyedOnce = false;
static const char * const	pcComment = "This ihm_element displays a text into the display window. If the element has no font set, the text can't be displayed. Please refer to PA010293.doc to get more informations on ihm_element_text. By default, text of the element is left aligned horizontally and centered vertically.";


//-----------------------------------------------------------------------------
//	Name: MATH_DESC constructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TEXT_DESC::IHM_ELEMENT_TEXT_DESC()
:
IHM_ELEMENT_RECT_DESC( M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_TEXT, "ihm_elementtext", false, NB_COMMANDS, pcComment )
{
	const int nNbrCommandsElementRect = IHM_ELEMENT_RECT_DESC::NB_COMMANDS;

	_pCommandDescsTab[ nNbrCommandsElementRect + 0 ].Init( GET_TEXT , 1, "GetText", "Returns the text of the element." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 0 ].SetParameterDesc( 0 , OTYPE_KERNEL_CHAR,      DEFAULT_SET_TEXT_BUFFER_SIZE, PARAMETER_DESC::OUT_PARAMETER, "Text");

	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].Init( SET_TEXT , 1, "SetText", "Set the text of the element." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].SetParameterDesc( 0 , OTYPE_KERNEL_CHAR,      DEFAULT_SET_TEXT_BUFFER_SIZE, PARAMETER_DESC::IN_PARAMETER,  "Text");

	_pCommandDescsTab[ nNbrCommandsElementRect + 2 ].Init( GET_FONT , 1, "GetFont", "Returns the font of the element." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 2 ].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID, 1,   PARAMETER_DESC::OUT_PARAMETER, "Font");

	_pCommandDescsTab[ nNbrCommandsElementRect + 3 ].Init( SET_FONT , 1, "SetFont", "Set the font of the element." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 3 ].SetParameterDesc( 0 , OTYPE_KERNEL_OBJECT_ID, 1,   PARAMETER_DESC::IN_PARAMETER,  "Font");

	_pCommandDescsTab[ nNbrCommandsElementRect + 4 ].Init( SET_VERTICAL_ALIGN , 1, "SetVerticalAlign", "Set the vertical alignement of the text." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 4 ].SetParameterDesc( 0 , OTYPE_KERNEL_INTEGER,   1,   PARAMETER_DESC::IN_PARAMETER,  "VAlign");

	_pCommandDescsTab[ nNbrCommandsElementRect + 5 ].Init( CHANGE_TEXT_COLOR, 1, "ChangeTextColor",	"Change the color of the text in the ihm_element. Please note that you should call 'ChangeTextColor' after setting the text of the ihm_elementtext." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 5 ].SetParameterDesc( 0 , OTYPE_KERNEL_COLOR,     1,   PARAMETER_DESC::IN_PARAMETER,  "VAlign");

	_pCommandDescsTab[ nNbrCommandsElementRect + 6 ].Init( SET_SCROLL_STEP, 1, "SetScrollStep",	"Set the scrolling step if the element has to scroll. The scroll step is in pixel (and is always the same whatever the screen resolution). Default value is 50 pixels." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 6 ].SetParameterDesc( 0 , OTYPE_KERNEL_INTEGER,   1,   PARAMETER_DESC::IN_PARAMETER,  "nStep");
	_pCommandDescsTab[ nNbrCommandsElementRect + 6 ].SetObsolete( true );

	_pCommandDescsTab[ nNbrCommandsElementRect + 7 ].Init( SCROLL, 1, "Scroll",	"Scroll the text within the ihm_elementtext." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 7 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT,     1,     PARAMETER_DESC::IN_PARAMETER,  "rScrollPercent",  "If set to 0, the text is at its top, and if 'nScrollPercent' is set to 100, the bottom of the text gets visible. Please alse note that negative percentage are allowed. So are ones larger than 100." );

	_pCommandDescsTab[ nNbrCommandsElementRect + 8 ].Init( SET_SCROLL_SPEED, 1, "SetScrollSpeed",	"Sets the speed the ihm_elementtext scrolls." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 8 ].SetParameterDesc( 0 , OTYPE_KERNEL_FLOAT,     1,     PARAMETER_DESC::IN_PARAMETER,  "nScrollPercent", "Default is 1.0." );

	_pCommandDescsTab[ nNbrCommandsElementRect + 9 ].Init( SCROLL_UP, 0, "ScrollUp", "Scroll up the text in the ihm_elementtext. If the text fits entirely, does nothing." );

	_pCommandDescsTab[ nNbrCommandsElementRect + 10 ].Init( SCROLL_DOWN, 0, "ScrollDown", "Scroll down the text in the ihm_elementtext. If the text fits entirely, does nothing." );
	

	CreateEnumTab( 3 );
	REG_ENUM( 0, IHM_ELEMENT_TEXT::VALIGN_CENTERED );
	REG_ENUM( 1, IHM_ELEMENT_TEXT::VALIGN_TOP );
	REG_ENUM( 2, IHM_ELEMENT_TEXT::VALIGN_BOTTOM );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC destructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_TEXT_DESC::~IHM_ELEMENT_TEXT_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::New
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	IHM_ELEMENT_TEXT_DESC::New()
{
	_nNbObjects++;

	return (new IHM_ELEMENT_TEXT);
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Delete
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_TEXT_DESC::Delete(COM_OBJECT *	pC)
{
	_nNbObjects--;

	delete pC;
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Instance
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
DESC* IHM_ELEMENT_TEXT_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = new IHM_ELEMENT_TEXT_DESC();
	}
	
	return ( _pInstance );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Suicide
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void IHM_ELEMENT_TEXT_DESC::Suicide()
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
//	Name:		OnGetText
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnGetText(IHM_ELEMENT_TEXT *	pElement,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	char *pTmp;

	QDT_STRING Text = pElement->GetText();

	QDT_ASSERT( Text.GetSize() < DEFAULT_SET_TEXT_BUFFER_SIZE );


	memcpy( Stack.GetCharTab( 0 ), Text.GetBuffer(), DEFAULT_SET_TEXT_BUFFER_SIZE );

	//DAC This sucks ! old com rulez, to ensure that buffer always end with a \0
	pTmp = Stack.GetCharTab(0);
	pTmp[ DEFAULT_SET_TEXT_BUFFER_SIZE - 1 ] = '\0';
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetText
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnSetText(IHM_ELEMENT_TEXT *	pElement,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	char pcTemp[DEFAULT_SET_TEXT_BUFFER_SIZE];
	memcpy( pcTemp, Stack.GetCharTab( 0 ), DEFAULT_SET_TEXT_BUFFER_SIZE );
	pcTemp[ DEFAULT_SET_TEXT_BUFFER_SIZE - 1 ] = '\0';
	QDT_STRING Text( pcTemp );

	pElement->SetText( Text );
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetFont
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnGetFont(IHM_ELEMENT_TEXT *	pElement,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	// Valid font.
	DISPLAY_FONT* pFont = pElement->GetFont();
	QDT_ASSERT( pFont );
	if ( ( pFont == NULL ) || ( pFont->IsLoaded() == false ) )
	{
		QDT_Warning( "In IHM_ELEMENT_TEXT_DESC::OnGetFont: invalid or not loaded font!" );
		return ( false );
	}


	// Valid INDEX_INT.
	INDEX_INT *pIndex = pFont->GetIndex();
	QDT_ASSERT( pIndex );
	if ( pIndex == NULL )
	{
		QDT_Error( "In IHM_ELEMENT_TEXT_DESC::OnGetFont: invalid INDEX_INT!" );
		return ( false );
	}


	// Valid type.
	QDT_ASSERT( pIndex->GetComObject()->GetType() == OTYPE_KERNEL_DISPLAY_FONT );
	if ( pIndex->GetComObject()->GetType() != OTYPE_KERNEL_DISPLAY_FONT )
	{
		QDT_Warning("In IHM_ELEMENT_TEXT_DESC::OnGetFont: object is not of type OTYPE_KERNEL_DISPLAY_FONT !");
		return false;
	}

	
	OBJECT_ID Id( pIndex->GetComObject()->GetType(), pIndex->GetId() );
	Stack.SetObjectID( 0, Id );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetFont
//	Object:		
//	01-09-04:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnSetFont(IHM_ELEMENT_TEXT *	pElement,
										 COMMAND_ID			CommandID,
										 PARAMETER_STACK &	Stack) const
{
	OBJECT_ID Id = Stack.GetObjectID( 0 );

	// Valid type.
	QDT_ASSERT( Id.GetType() == OTYPE_KERNEL_DISPLAY_FONT );
	if ( Id.GetType() != OTYPE_KERNEL_DISPLAY_FONT )
	{
		QDT_Warning("In IHM_ELEMENT_TEXT_DESC::OnSetFont: object is not of type OTYPE_KERNEL_DISPLAY_FONT !");
		return false;
	}

	// Valid INDEX_INT.
	INDEX_INT *pFontIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pFontIndexInt );
	if ( !pFontIndexInt )
	{
		QDT_Warning("In IHM_ELEMENT_TEXT_DESC::OnSetFont: can't get an DISPLAY_FONT from %i::%i !", Id.GetType(), Id.GetID() );
		return( false );
	}

	// Valid font.
	DISPLAY_FONT *pFont = static_cast<DISPLAY_FONT*>( pFontIndexInt->GetObject() );
	QDT_ASSERT( pFont );
	if ( pFont == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_TEXT_DESC::OnSetFont: font invalid or not loaded !");
		return( false );
	}


	pElement->SetFont( pFont );

	return( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetVerticalAlign
//	Object:		
//	01-10-19:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnSetVerticalAlign(IHM_ELEMENT_TEXT *	pElement,
												  COMMAND_ID			CommandID,
												  PARAMETER_STACK &		Stack) const
{
	if ( ( Stack.GetInteger( 0 ) < 0 ) || ( Stack.GetInteger( 0 ) > 2 ) )
	{
		return ( false );	
	}

	pElement->SetVerticalAlignement( (IHM_ELEMENT_TEXT::VALIGN)Stack.GetInteger( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnChangeTextColor
//	Object:		
//	01-12-07:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnChangeTextColor(IHM_ELEMENT_TEXT *	pElement,
												 COMMAND_ID			CommandID,
												 PARAMETER_STACK &	Stack) const
{
	pElement->ChangeTextColor( Stack.GetColor( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetScrollStep
//	Object:		
//	02-03-27:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnSetScrollStep(IHM_ELEMENT_TEXT *	pElement,
											   COMMAND_ID			CommandID,
											   PARAMETER_STACK &	Stack) const
{
	pElement->SetScrollStep( Stack.GetInteger( 0 ) );
	
	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnScroll
//	Object:		
//	02-03-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnScroll(IHM_ELEMENT_TEXT *	pElement,
										COMMAND_ID			CommandID,
										PARAMETER_STACK &	Stack) const
{
	pElement->Scroll( Stack.GetFloat( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnSetScrollSpeed
//	Object:		
//	02-04-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnSetScrollSpeed(IHM_ELEMENT_TEXT *	pElement,
												COMMAND_ID			CommandID,
												PARAMETER_STACK &	Stack) const
{
	pElement->SetScrollSpeed( Stack.GetFloat( 0 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnScrollUp
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnScrollUp(IHM_ELEMENT_TEXT *	pElement,
										  COMMAND_ID			CommandID,
										  PARAMETER_STACK &		Stack) const
{
	pElement->ScrollUp();

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		OnScrollDown
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::OnScrollDown(IHM_ELEMENT_TEXT *	pElement,
											COMMAND_ID			CommandID,
											PARAMETER_STACK &	Stack) const
{
	pElement->ScrollDown();

	return ( true );
}



//-----------------------------------------------------------------------------
//	Name: IHM_ELEMENT_TEXT_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_TEXT_DESC::ReceiveCommandProc(void *			pObject,
												  COMMAND_ID		CommandID,
												  PARAMETER_STACK &	Stack) const
{	
	IHM_ELEMENT_TEXT *pElement = static_cast<IHM_ELEMENT_TEXT*> (pObject);	
	QDT_ASSERT( pElement );
	if ( !pElement )
	{
		return ( false );
	}


	switch( CommandID )
	{
	// Returns the text of the element.
	case GET_TEXT:
		return ( OnGetText( pElement, CommandID, Stack ) );
	
	// Set the text of the element.
	case SET_TEXT:
		return ( OnSetText( pElement, CommandID, Stack ) );

	// Returns the font of the element.
	case GET_FONT:
		return ( OnGetFont( pElement, CommandID, Stack ) );

	// Set the font of the element.
	case SET_FONT:
		return ( OnSetFont( pElement, CommandID, Stack ) );

	// Set the vertical alignement of the text.
	case SET_VERTICAL_ALIGN:
		return ( OnSetVerticalAlign( pElement, CommandID, Stack ) );

	// Change the color of the text in the ihm_element.
	case CHANGE_TEXT_COLOR:
		return ( OnChangeTextColor( pElement, CommandID, Stack ) );

	// Set the scrolling step if the element has to scroll.
	case SET_SCROLL_STEP:
		return ( OnSetScrollStep( pElement, CommandID, Stack ) );

	// Scroll the text within the ihm_elementtext.
	case SCROLL:
		return ( OnScroll( pElement, CommandID, Stack ) );

	// Sets the speed the ihm_eleemnttext scrolls.
	case SET_SCROLL_SPEED:
		return ( OnSetScrollSpeed( pElement, CommandID, Stack ) );

	// Scroll up the text in the ihm_elementtext.
	case SCROLL_UP:
		return ( OnScrollUp( pElement, CommandID, Stack ) );

	// Scroll down the text in the ihm_elementtext.
	case SCROLL_DOWN:
		return ( OnScrollDown( pElement, CommandID, Stack ) );

	// Default: if no command was recognized here, send the command to the base class.
	default:
		return ( IHM_ELEMENT_RECT_DESC::ReceiveCommandProc( pElement, CommandID, Stack ) );
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
