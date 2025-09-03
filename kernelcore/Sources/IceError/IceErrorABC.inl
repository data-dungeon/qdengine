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
//	CLASS:	ICE_ERROR_ABC
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetErrorLevel
//	Object:		Error Level accessor.
//
//	@return		the error level.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
const ICE_ERROR_ABC::ERROR_LEVEL &	ICE_ERROR_ABC::GetErrorLevel() const
{
	return _eLevel;
}

//-----------------------------------------------------------------------------
//	Name:		GetErrorCode
//	Object:		Error Code Accessor.
//
//	@return		the error code.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
const int &	ICE_ERROR_ABC::GetErrorCode() const
{
	return _nCode;
}

//-----------------------------------------------------------------------------
//	Name:		IsWarning
//	Object:		Checks error' level
//
//	@return		true if the level is set to a warning, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ABC::IsWarning() const
{
	return (_eLevel > EL_UNKNOW && _eLevel < EL_ERROR);
}

//-----------------------------------------------------------------------------
//	Name:		IsError
//	Object:		Checks error' level
//
//	@return		true if the level is set to an error, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ABC::IsError() const
{
	return (_eLevel==EL_ERROR
			|| _eLevel==EL_FATAL_ERROR);
}

//-----------------------------------------------------------------------------
//	Name:		IsFatalError
//	Object:		Checks error' level
//
//	@return		true if the level is set to a fatal error, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ABC::IsFatalError() const
{
	return (_eLevel==EL_FATAL_ERROR);
}

//-----------------------------------------------------------------------------
//	Name:		GetErrorTime
//	Object:		Access error creation time
//
//	@return		the time stamp of the error
//	06-02-20:	CAU - Created
//-----------------------------------------------------------------------------
const UInt64 &	ICE_ERROR_ABC::GetErrorTime() const
{
	return _nTime;
}

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		This method can be overloaded to dump additionnal information about the error
//
//	@param		pTextDataFile	the text stream to write into
//	06-02-20:	CAU - Created
//-----------------------------------------------------------------------------
void	ICE_ERROR_ABC::Dump(TEXT_DATA_FILE_ABC *	pTextDataFile) const
{
	QDT_ASSERT(pTextDataFile);	// must exist
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
