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
//	CLASS:	ICE_TEXT_ERROR
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SetAdditionnalErrorText
//	Object:		Adds additionnal text to the error.
//
//	@param		sText	the additionnal text.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ICE_TEXT_ERROR::SetAdditionnalErrorText(const QDT_STRING &	sText)
{
	_sText = sText;
}

//-----------------------------------------------------------------------------
//	Name:		GetErrorMessage
//	Object:		This methods must be overloaded to return the error as a QDT_STRING.
//	
//	@return		the error as a string.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
QDT_STRING	ICE_TEXT_ERROR::GetErrorMessage() const
{
	return _sText;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
