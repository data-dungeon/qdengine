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
//	CLASS:	DISPLAY_CONTEXT
//
//	02-06-07:	RMA - Created
//*****************************************************************************

#ifdef	QDT_INCLUDE_CI
	static char	__QDT_FILE__[] = __FILE__;
#endif	//	QDT_INCLUDE_CI

#include	"Include.h"

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_CONTEXT constructor
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_CONTEXT::DISPLAY_CONTEXT()
:
_pContext			(NULL),
_ViewportListInst	(COM_INSTANCE::NOT_INITIALIZED_AT_CONSTRUCTION),
_BackgroundColor	(255, 0, 0, 0)
{
}

//-----------------------------------------------------------------------------
//	Name:		DISPLAY_CONTEXT destructor
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
DISPLAY_CONTEXT::~DISPLAY_CONTEXT()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosX
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	DISPLAY_CONTEXT::GetWindowPosX() const		
{
	QDT_ASSERT(_pContext);
	return (_pContext->GetWindowPosX()); 
}

//-----------------------------------------------------------------------------
//	Name:		GetWindowPosY
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	DISPLAY_CONTEXT::GetWindowPosY() const			
{ 
	QDT_ASSERT(_pContext);
	return (_pContext->GetWindowPosY()); 
}
			 
//-----------------------------------------------------------------------------
//	Name:		GetWindowSizeX
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	DISPLAY_CONTEXT::GetWindowSizeX() const
{
	QDT_ASSERT(_pContext);
	return (_pContext->GetWindowSizeX()); 
}
			 
//-----------------------------------------------------------------------------
//	Name:		GetWindowSizeY
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
unsigned int	DISPLAY_CONTEXT::GetWindowSizeY() const			
{ 
	QDT_ASSERT(_pContext);
	return (_pContext->GetWindowSizeY()); 
}

//-----------------------------------------------------------------------------
//	Name:		SetWindowPos
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_CONTEXT::SetWindowPos(unsigned int	nX,
									  unsigned int	nY)
{
	QDT_ASSERT(_pContext);
	_pContext->SetWindowPos(nX, nY);
}

//-----------------------------------------------------------------------------
//	Name:		SetWindowSize
//	Object:		
//	02-06-07:	RMA - Created
//-----------------------------------------------------------------------------
void	DISPLAY_CONTEXT::SetWindowSize(unsigned int	nX,
									   unsigned int	nY)
{
	QDT_ASSERT(_pContext);
	_pContext->SetWindowSize(nX, nY);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
