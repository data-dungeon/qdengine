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
//	CLASS:	ERROR_MODULE_ABC
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetModuleID
//	Object:		Module ID Accessor.
//
//	@return		the id of the module.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
const ERROR_MANAGER::MODULE_ID &	ERROR_MODULE_ABC::GetModuleID() const
{
	return _eID;
}

//-----------------------------------------------------------------------------
//	Name:		RegisterNewErrorCallback
//	Object:		Registers the callback of the NewError method.
//
//	@param		fCallBack	the callback to register
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::RegisterNewErrorCallback(NEW_ERROR_CALLBACK	fCallBack)
{
	QDT_ASSERT(!_fNewErrorCallback	// must have been unregistered
			   && fCallBack);		// must exist
	_fNewErrorCallback = fCallBack;
}

//-----------------------------------------------------------------------------
//	Name:		UnregisterNewErrorCallback
//	Object:		Unregister the callback of the NewError method.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::UnregisterNewErrorCallback()
{
	QDT_ASSERT(_fNewErrorCallback);	// must have been registered
	_fNewErrorCallback = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		RegisterDeleteErrorCallback
//	Object:		Registers the callback of the DeleteError method.
//
//	@param		fCallBack	the callback to register
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::RegisterDeleteErrorCallback(DELETE_ERROR_CALLBACK	fCallBack)
{
	QDT_ASSERT(!_fDeleteErrorCallback	// must have been unregistered
			   && fCallBack);			// must exist
	_fDeleteErrorCallback = fCallBack;
}

//-----------------------------------------------------------------------------
//	Name:		UnregisterDeleteErrorCallback
//	Object:		Unregister the callback of the NewError method.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::UnregisterDeleteErrorCallback()
{
	QDT_ASSERT(_fDeleteErrorCallback);	// must have been registered
	_fDeleteErrorCallback = NULL;
}

//-----------------------------------------------------------------------------
//	Name:		CallNewErrorCallback
//	Object:		Call the NewError method callback.
//
//	@param		pError	the error to use in the callback
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::CallNewErrorCallback(ICE_ERROR_ABC *	pError)
{
	QDT_ASSERT(pError);	// pError must exist
	if(_fNewErrorCallback)
	{
		(*_fNewErrorCallback)(this,pError);
	}
}

//-----------------------------------------------------------------------------
//	Name:		CallDeleteErrorCallback
//	Object:		Call the DeleteError method callback.
//
//	@param		pError	the error to use in the callback
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::CallDeleteErrorCallback(ICE_ERROR_ABC *	pError)
{
	QDT_ASSERT(pError);	// pError must exist
	if(_fDeleteErrorCallback)
	{
		(*_fDeleteErrorCallback)(this,pError);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrErrors
//	Object:		Get the module's error count per error level.
//
//	@param		eErrorLevel	the error level
//	@return					the number of error in the specified error level
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
int	ERROR_MODULE_ABC::GetNbrErrors(const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel) const
{
	QDT_ASSERT(eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	return static_cast<int>(_aErrors[eErrorLevel].GetNbElements());
}

//-----------------------------------------------------------------------------
//	Name:		GetError
//	Object:		Get an error of a specified error level.
//
//	@param		eErrorLevel	the error level
//	@param		nErrorIndex	the error index in the error level
//	@return					the specified error
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MODULE_ABC::GetError(const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel,
									   const int &							nErrorIndex) const
{
	QDT_ASSERT(eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid
	QDT_ASSERT(nErrorIndex >= 0
			&& nErrorIndex < static_cast<int>(_aErrors[eErrorLevel].GetNbElements()));	// error index must be valid

	return static_cast<ICE_ERROR_ABC *>(_aErrors[eErrorLevel].GetIndex(nErrorIndex));
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstError
//	Object:		Get the first error of a specified error level.
//
//	@param		eErrorLevel	the error level.
//	@return					the first error in the specified level.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MODULE_ABC::GetFirstError(const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel) const
{
	QDT_ASSERT(eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	return static_cast<ICE_ERROR_ABC *>(_aErrors[eErrorLevel].GetHead());
}

//-----------------------------------------------------------------------------
//	Name:		GetLastError
//	Object:		Get the last error of a specified error level.
//
//	@param		eErrorLevel	the error level
//	@return					the last error in the specified level
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MODULE_ABC::GetLastError(const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel) const
{
	QDT_ASSERT(eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	return static_cast<ICE_ERROR_ABC *>(_aErrors[eErrorLevel].GetQueue());
}

//-----------------------------------------------------------------------------
//	Name:		HasError
//	Object:		Check if the error level has at least one error.
//
//	@param		eErrorLevel	specifies the error level
//	@return					true if the error level has errors, false otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ERROR_MODULE_ABC::HasError(const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel) const
{
	QDT_ASSERT(eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	return (_aErrors[eErrorLevel].GetNbElements()!=0);
}

//-----------------------------------------------------------------------------
//	Name:		GetNextError
//	Object:		Get the next error from the specified one.
//
//	@param		pError	the current error
//	@return				the next error after the current one in the same error level if it exists, NULL otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MODULE_ABC::GetNextError(const ICE_ERROR_ABC *	pError) const
{
	QDT_ASSERT(pError); // must exist

	QDT_ASSERT(pError->GetErrorLevel() > ICE_ERROR_ABC::EL_UNKNOW
			&& pError->GetErrorLevel() < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	QDT_ASSERT(_aErrors[pError->GetErrorLevel()].Find(pError)!=-1); // error must be managed

	return static_cast<ICE_ERROR_ABC *>(pError->GetNext());
}

//-----------------------------------------------------------------------------
//	Name:		GetPreviousError
//	Object:		Get the previous error from the specified one .
//
//	@param		pError	the current error
//	@return				the previous error before the current one  in the same error level if it exists, NULL otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MODULE_ABC::GetPreviousError(const ICE_ERROR_ABC *	pError) const
{
	QDT_ASSERT(pError); // must exist

	QDT_ASSERT(pError->GetErrorLevel() > ICE_ERROR_ABC::EL_UNKNOW
			&& pError->GetErrorLevel() < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	QDT_ASSERT(_aErrors[pError->GetErrorLevel()].Find(pError)!=-1); // error must be managed

	return static_cast<ICE_ERROR_ABC *>(pError->GetPrev());
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		Loop method to provide frame per frame process.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::Loop()
{

}

//-----------------------------------------------------------------------------
//	Name:		DumpAdditionalInfo
//	Object:		This method can be overloaded to dump additionnal information in the module
//
//	@param		pTextDataFile	the text stream to write into
//	06-02-20:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::DumpAdditionalInfo(TEXT_DATA_FILE_ABC *	pTextDataFile) const
{
	QDT_ASSERT(pTextDataFile);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
