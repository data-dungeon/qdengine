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
//	CLASS:	IHM_ELEMENT_RECT_DESC
//
//	01-11-30:	GGO - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"..\Include.h"


#ifndef _DEBUG

#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_RECT_DESC constructor
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_RECT_DESC::IHM_ELEMENT_RECT_DESC(OBJECT_TYPE		Type,
											 const char *		pcName,
											 bool				bIsSingleton,
											 int				nNbrCommands,
											 const char * const	pcCommand)
:
IHM_ELEMENT_ABC_DESC( Type, pcName, bIsSingleton, nNbrCommands, pcCommand )
{
	const int nNbrCommandsElementABC = IHM_ELEMENT_ABC_DESC::NB_COMMANDS;

	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].Init(SET_RELATIVE_POSITION, 4, "SetRelativePosition", "Set the relative position (in percentage of the rendering window (or viewport if any) size) of an element.");
	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].SetParameterDesc(0, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::IN_PARAMETER, "rLeftRelativePos");
	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].SetParameterDesc(1, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::IN_PARAMETER, "rRightRelativePos");
	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].SetParameterDesc(2, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::IN_PARAMETER, "rTopRelativePos");
	_pCommandDescsTab[ nNbrCommandsElementABC + 0 ].SetParameterDesc(3, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::IN_PARAMETER, "rBottomRelativePos");

	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].Init(GET_RELATIVE_POSITION, 4, "GetRelativePosition", "Get the relative position of an element (in percentage of the rendering window (or viewport if any)).");
	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].SetParameterDesc(0, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::OUT_PARAMETER, "rLeftRelativePos");
	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].SetParameterDesc(1, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::OUT_PARAMETER, "rRightRelativePos");
	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].SetParameterDesc(2, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::OUT_PARAMETER, "rTopRelativePos");
	_pCommandDescsTab[ nNbrCommandsElementABC + 1 ].SetParameterDesc(3, OTYPE_KERNEL_FLOAT,    1, PARAMETER_DESC::OUT_PARAMETER, "rBottomRelativePos");
}


//-----------------------------------------------------------------------------
//	Name:		IHM_ELEMENT_RECT_DESC destructor
//	Object:		
//	01-11-30:	GGO - Created
//-----------------------------------------------------------------------------
IHM_ELEMENT_RECT_DESC::~IHM_ELEMENT_RECT_DESC()
{
}


//-----------------------------------------------------------------------------
//	Name:		OnSetRelativePosition
//	Object:		
//	01-09-11:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_RECT_DESC::OnSetRelativePosition(void *				pObject,
													 COMMAND_ID			CommandID,
													 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENT_RECT *pElement = static_cast<IHM_ELEMENT_RECT*>( pObject );

	// Set the position of the element.
	pElement->SetRelativePosLeft(   Stack.GetFloat( 0 ) );
	pElement->SetRelativePosRight(  Stack.GetFloat( 1 ) );
	pElement->SetRelativePosTop(    Stack.GetFloat( 2 ) );
	pElement->SetRelativePosBottom( Stack.GetFloat( 3 ) );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name:		OnGetRelativePosition
//	Object:		
//	01-09-28:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_RECT_DESC::OnGetRelativePosition(void *				pObject,
													 COMMAND_ID			CommandID,
													 PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENT_RECT *pElement = static_cast<IHM_ELEMENT_RECT*>( pObject );

	// Get the position of the element.
	Stack.SetFloat( 0, pElement->GetRelativePosLeft() );
	Stack.SetFloat( 1, pElement->GetRelativePosRight() );
	Stack.SetFloat( 2, pElement->GetRelativePosTop() );
	Stack.SetFloat( 3, pElement->GetRelativePosBottom() );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: MATH_DESC::ReceiveCommandProc
//	Object: 
//	01-06-13:	GGO - Created
//-----------------------------------------------------------------------------
bool	IHM_ELEMENT_RECT_DESC::ReceiveCommandProc(void *			pObject,
												  COMMAND_ID		CommandID,
												  PARAMETER_STACK &	Stack) const
{
	IHM_ELEMENT_RECT *pElement = static_cast<IHM_ELEMENT_RECT*> (pObject);	
	QDT_ASSERT( pElement );
	if ( !pElement )
	{
		return ( false );
	}


	switch( CommandID )
	{
	// Set the relative position (in percentage of the rendering window size) of an element.
	case SET_RELATIVE_POSITION:
		return ( OnSetRelativePosition( pObject, CommandID, Stack ) );

	// Get the relative position of an element.
	case GET_RELATIVE_POSITION:
		return ( OnGetRelativePosition( pObject, CommandID, Stack ) );

	// Default: if no command was recognized here, send the command to the base class.
	default:
		return ( IHM_ELEMENT_ABC_DESC::ReceiveCommandProc( pObject, CommandID, Stack ) );
	}
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
