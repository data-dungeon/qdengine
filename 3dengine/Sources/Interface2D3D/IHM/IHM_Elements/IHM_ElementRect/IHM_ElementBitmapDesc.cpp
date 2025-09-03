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
//	CLASS:	IHM_ELEMENT_BITMAP_DESC
//
//	01-08-20:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\Include.h"
#include	INC_3DENGINE(M3D)
#include	INC_KRNCOM(OldCom/Database/DatabaseManager)
#include	INC_KRNCOM(OldCom/Communication)

#ifndef _DEBUG

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

IHM_ELEMENT_BITMAP_DESC*	IHM_ELEMENT_BITMAP_DESC::_pInstance      = NULL;
bool						IHM_ELEMENT_BITMAP_DESC::_bDestroyedOnce = false;
static const char * const	pcComment = "This ihm_element displays a bitmap into the display window.";

//-----------------------------------------------------------------------------
//	Name: MATH_DESC constructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_BITMAP_DESC::IHM_ELEMENT_BITMAP_DESC()
:
IHM_ELEMENT_RECT_DESC(M3D_TYPE_REGISTRY::OTYPE_MOTEUR3D_IHM_ELEMENT_BITMAP, "ihm_elementbitmap", false, NB_COMMANDS, pcComment )
{
	const int nNbrCommandsElementRect = IHM_ELEMENT_RECT_DESC::NB_COMMANDS;

	_pCommandDescsTab[ nNbrCommandsElementRect + 0 ].Init( SET_BITMAP, 1, "SetBitmap", "Set the bitmap that have to be displayed when the element is visible." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 0 ].SetParameterDesc( 0, OTYPE_KERNEL_OBJECT_ID, 1, PARAMETER_DESC::IN_PARAMETER, "Bitmap");

	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].Init( SET_CROPPING_COORDINATES , 4, "SetCroppingCoordinates", "Set the cropping coordinates of the bitmap in case you want to display only a part of the bitmap. If the function is not called, all the bitmap is displayed. The coordinates are pixels numbers." );
	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].SetParameterDesc( 0 , OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nLeft");
	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].SetParameterDesc( 1 , OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nRight");
	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].SetParameterDesc( 2 , OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nTop");
	_pCommandDescsTab[ nNbrCommandsElementRect + 1 ].SetParameterDesc( 3 , OTYPE_KERNEL_INTEGER, 1, PARAMETER_DESC::IN_PARAMETER, "nBottom");
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC destructor
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_BITMAP_DESC::~IHM_ELEMENT_BITMAP_DESC()
{

}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::New
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
COM_OBJECT *	IHM_ELEMENT_BITMAP_DESC::New()
{
	_nNbObjects++;

	return (new IHM_ELEMENT_BITMAP);
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Delete
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void	IHM_ELEMENT_BITMAP_DESC::Delete(COM_OBJECT *	pC)
{
	_nNbObjects--;

	delete pC;
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Instance
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
DESC* IHM_ELEMENT_BITMAP_DESC::Instance()
{
	QDT_ASSERT( _bDestroyedOnce == false );

	if ( _pInstance == NULL )
	{
		_pInstance = new IHM_ELEMENT_BITMAP_DESC();
	}
	
	return ( _pInstance );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::Suicide
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
void IHM_ELEMENT_BITMAP_DESC::Suicide()
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
//	Name:		OnSetBitmap
//	Object:		
//	01-09-06:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_BITMAP_DESC::OnSetBitmap(IHM_ELEMENT_BITMAP *	pElement,
											 COMMAND_ID				CommandID,
											 PARAMETER_STACK &		Stack) const
{
	OBJECT_ID Id = Stack.GetObjectID( 0 );

	// Valid type.
	if ( Id.GetType() != OTYPE_KERNEL_TEXTURE_NEW )
	{
		QDT_Warning("In IHM_ELEMENT_BITMAP_DESC::OnSetActiveBitmap: object is not of type OTYPE_KERNEL_TEXTURE_NEW !");
		return false;
	}

	// Valid INDEX_INT.
	INDEX_INT *pTextureIndexInt = DATABASE_MANAGER::Instance()->GetById( Id.GetType(), Id.GetID() );
	QDT_ASSERT( pTextureIndexInt );
	if ( pTextureIndexInt == NULL )
	{
		QDT_Error("In IHM_ELEMENT_BITMAP_DESC::OnSetActiveBitmap: invalid INDEX_INT !" );
		return( false );
	}

	// Valid font.
	TEXTURE_NEW	*pTexture = static_cast<TEXTURE_NEW*>( pTextureIndexInt->GetObject() );
	QDT_ASSERT( pTexture );
	if ( pTexture == NULL )
	{
		QDT_Warning("In IHM_ELEMENT_TEXT_DESC::OnSetActiveBitmap: invalid TEXTURE_NEW !");
		return( false );
	}


	pElement->SetBitmap( pTexture );

	return( true );	
}


//-----------------------------------------------------------------------------
//	Name:		OnSetCroppingCoordinates
//	Object:		
//	01-11-15:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_BITMAP_DESC::OnSetCroppingCoordinates(IHM_ELEMENT_BITMAP *	pElement,
														  COMMAND_ID			CommandID,
														  PARAMETER_STACK &		Stack) const
{
	pElement->SetCroppingCoordinates( Stack.GetInteger( 0 ),
									  Stack.GetInteger( 1 ),
									  Stack.GetInteger( 2 ),
									  Stack.GetInteger( 3 ) );
	return ( true );
}



//-----------------------------------------------------------------------------
//	Name: MATH_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_BITMAP_DESC::ReceiveCommandProc(void *				pObject,
													COMMAND_ID			CommandID,
													PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENT_BITMAP *pElement = static_cast<IHM_ELEMENT_BITMAP*> (pObject);	
	QDT_ASSERT( pElement );
	if ( !pElement )
	{
		return ( false );
	}


	switch( CommandID )
	{
	// Set the bitmap that have to be displayed when the element is active.
	case SET_BITMAP:
		return ( OnSetBitmap( pElement, CommandID, Stack ) );

	// Set the cropping coordinates of the bitmap.
	case SET_CROPPING_COORDINATES:
		return( OnSetCroppingCoordinates( pElement, CommandID, Stack ) );

	// Default: if no command was recognized here, send the command to the base class.
	default:
		return ( IHM_ELEMENT_RECT_DESC::ReceiveCommandProc( pObject, CommandID, Stack ) );
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
