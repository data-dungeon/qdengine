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
//	CLASS:	ICE_ERROR_ITERATOR
//
//	06-02-14:	CAU - Created
//*****************************************************************************

#include "Root.h"
#include INCL_KCORE(IceError/IceErrorIterator)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{

namespace KCORE
{

#ifdef _DEBUG
	#include "IceErrorIterator.inl"
#endif //_DEBUG

//-----------------------------------------------------------------------------
//	Name:		ICE_ERROR_ITERATOR constructor
//	Object:		Class constructor.
//
//	@param		bModuleContinuity		the iterator can browse through modules if true, iterator is limited to one module otherwise.
//	@param		bErrorLevelContinuity	the iterator can browse through error levels if true, iterator is limited to one error level otherwise.
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR::ICE_ERROR_ITERATOR(const bool	bModuleContinuity,
									   const bool	bErrorLevelContinuity)
:
_bModuleContinuity(bModuleContinuity),
_bErrorLevelContinuity(bErrorLevelContinuity),
_pModule(NULL),
_eErrorLevel(ICE_ERROR_ABC::EL_UNKNOW),
_pError(NULL)
{

}

//-----------------------------------------------------------------------------
//	Name:		~ICE_ERROR_ITERATOR destructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR::~ICE_ERROR_ITERATOR()
{
	
}

//-----------------------------------------------------------------------------
//	Name:		Begin
//	Object:		Get the first error
//
//	@param	eModuleID	the id of the module to access to get the first error
//	@param	eErrorLevel	the error level to access in the module to get the error
//	@return				returns the first error of the specified error level of the specified module if it exists, a not valid iterator otherwise
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::Begin(const ERROR_MANAGER::MODULE_ID &		eModuleID,
											  const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel)
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	ERROR_MODULE_ABC * pModule;

	if(eModuleID == ERROR_MANAGER::MI_UNKNOW)
	{
		pModule = pErrorManager->GetFirstModule();
	}
	else
	{
		pModule = pErrorManager->FindModule(eModuleID);
	}
	
	if(!pModule)
	{
		return ICE_ERROR_ITERATOR();
	}

	ICE_ERROR_ABC::ERROR_LEVEL eErrLevel(eErrorLevel);
    if(eErrLevel==ICE_ERROR_ABC::EL_UNKNOW)
	{
		eErrLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	}
	
	QDT_ASSERT(eErrLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrLevel < ICE_ERROR_ABC::EL_MAX);	// must be valid

	ICE_ERROR_ITERATOR it(eModuleID == ERROR_MANAGER::MI_UNKNOW, eErrorLevel == ICE_ERROR_ABC::EL_UNKNOW);
	it._pModule = pModule;
	it._eErrorLevel = eErrLevel;
	
	if(it.GoToFirst())
	{
		return it;
	}

	it.Invalidate();
	return it;
}

//-----------------------------------------------------------------------------
//	Name:		Last
//	Object:		Get the Last error
//
//	@param		eModuleID	the id of the module to access to get the last error
//	@param		eErrorLevel	the error level to access in the module to get the last error
//	@return					returns the last error of the specified error level of the specified module if it exists, a not valid iterator otherwise
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::Last(const ERROR_MANAGER::MODULE_ID &	eModuleID,
											 const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel)
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	ERROR_MODULE_ABC * pModule;

	if(eModuleID == ERROR_MANAGER::MI_UNKNOW)
	{
		pModule = pErrorManager->GetLastModule();
	}
	else
	{
		pModule = pErrorManager->FindModule(eModuleID);
	}
	
	if(!pModule)
	{
		return ICE_ERROR_ITERATOR();
	}

	ICE_ERROR_ABC::ERROR_LEVEL eErrLevel(eErrorLevel);
    if(eErrLevel==ICE_ERROR_ABC::EL_UNKNOW)
	{
		eErrLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
	}
	
	QDT_ASSERT(eErrLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& eErrLevel < ICE_ERROR_ABC::EL_MAX);	// must be valid

	ICE_ERROR_ITERATOR it(eModuleID == ERROR_MANAGER::MI_UNKNOW, eErrorLevel == ICE_ERROR_ABC::EL_UNKNOW);
	it._pModule = pModule;
	it._eErrorLevel = eErrLevel;
	
	if(it.GoToLast())
	{
		return it;
	}

	it.Invalidate();
	return it;
}

//-----------------------------------------------------------------------------
//	Name:		End
//	Object:		Get an invalid iterator
//
//	@return		an invalid iterator
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ITERATOR	ICE_ERROR_ITERATOR::End()
{
	return ICE_ERROR_ITERATOR();
}

//-----------------------------------------------------------------------------
//	Name:		Invalidate
//	Object:		Invalidate the iterator
//
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
void	ICE_ERROR_ITERATOR::Invalidate()
{
	_pModule		= NULL;
	_eErrorLevel	= ICE_ERROR_ABC::EL_UNKNOW;
	_pError			= NULL;
}

//-----------------------------------------------------------------------------
//	Name:		GoToModule
//	Object:		Defines the current module.
//				Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_UNKNOW+1 otherwise.
//				Sets the current error to the first one of the current error level of the specified module.
//
//	@param		eModuleId	the module id to set as the current one	
//	@return					true if the specified module exists, false otherwise and the iterator is invalidate
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToModule(const ERROR_MANAGER::MODULE_ID &	eModuleId)
{
	QDT_ASSERT(eModuleId > ERROR_MANAGER::MI_UNKNOW
			&& eModuleId < ERROR_MANAGER::MI_MAX);	// must be valid

	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_pModule = pErrorManager->GetModule(eModuleId);

	if(_bErrorLevelContinuity)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	}

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	_pError = _pModule->GetFirstError(_eErrorLevel);
	return true;
}


//-----------------------------------------------------------------------------
//	Name:		GoToErrorLevel
//	Object:		Defines the current error level.
//				Sets the current error as the first error of the specified error level of the current module.
//
//	@param		eErrorLevel	the error level to set as the current
//	@return					true if any module exists, false otherwise and the iterator is invalidate
//	06-02-16:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToErrorLevel(const ICE_ERROR_ABC::ERROR_LEVEL &	eErrorLevel)
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_eErrorLevel = eErrorLevel;

	if(!_pModule)
	{
		_pModule = pErrorManager->GetFirstModule();
	}

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	_pError = _pModule->GetFirstError(_eErrorLevel);
	return true;
}


//-----------------------------------------------------------------------------
//	Name:		GoToFirstModule
//	Object:		Go to the first module.
//				Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_UNKNOW+1 otherwise.
//				Set the current error to the first error of the specified error level.
//
//	@return		true if any module exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToFirstModule()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_pModule = pErrorManager->GetFirstModule();

	if	(_bErrorLevelContinuity
		|| _eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	}

    QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	_pError = _pModule->GetFirstError(_eErrorLevel);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		GoToFirstModuleWithErrors
//	Object:		Go to the first module that owns any errors.
//				Only in the current error level if ErrorLevelContinuity is not true, starts from EL_UNKNOW otherwise.
//				Set the current error to the first error of the specified error level that owns any errors.
//
//	@return		true if any module with errors exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToFirstModuleWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_pModule = pErrorManager->GetFirstModule();

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if(_bErrorLevelContinuity)
	{
		while(_pModule)
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
			while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
			{
				_pError = _pModule->GetFirstError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel +1);
			}
			_pModule = pErrorManager->GetNextModule(_pModule);
		}
	}
	else
	{
		if	(_eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
			|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
		}
		QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
				&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid
		
		while(_pModule)
		{
			_pError = _pModule->GetFirstError(_eErrorLevel);
			if(_pError)
			{
				return true;
			}
			_pModule = pErrorManager->GetNextModule(_pModule);
		}
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToLastModule
//	Object:		Go to the last module.
//				Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_MAX-1 otherwise.
//				Set the current error to the last error of the specified error level.
//
//	@return		true if any module exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToLastModule()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_pModule = pErrorManager->GetLastModule();

	if(_bErrorLevelContinuity)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX-1);
	}

	if	(_eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
	}
	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid
	
	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	_pError = _pModule->GetLastError(_eErrorLevel);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		GoToLastModuleWithErrors
//	Object:		Go to the last module that owns any errors.
//				Only in the current error level if ErrorLevelContinuity is not true, starts from EL_UNKNOW otherwise.
//				Set the current error to the last error of the specified error level that owns any errors.
//
//	@return		true if any module with errors exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToLastModuleWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_pModule = pErrorManager->GetLastModule();


	if	(_bErrorLevelContinuity 
		|| _eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
	}
	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if(_bErrorLevelContinuity)
	{
		while(_pModule)
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
			while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
			{
				_pError = _pModule->GetLastError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel -1);
			}
			_pModule = pErrorManager->GetPreviousModule(_pModule);
		}
	}
	else
	{
		while(_pModule)
		{
			_pError = _pModule->GetLastError(_eErrorLevel);
			if(_pError)
			{
				return true;
			}
			_pModule = pErrorManager->GetPreviousModule(_pModule);
		}
	}

	Invalidate();
	return false;
}


//-----------------------------------------------------------------------------
//	Name:		GoToNextModule
//	Object:		Go to the next module.
//				Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_UNKNOW+1 otherwise.
//				Set the current error to the first error of the specified error level.
//
//	@return		true if the next module exists, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToNextModule()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(_pModule);	// must exist

	_pModule = pErrorManager->GetNextModule(_pModule);

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if	(_bErrorLevelContinuity 
		|| _eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	}

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_pError = _pModule->GetFirstError(_eErrorLevel);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		GoToNextModuleWithErrors
//	Object:		Go to the next module that owns any errors.
//				Only in the current error level if ErrorLevelContinuity is not true, starts from EL_UNKNOW otherwise.
//				Set the current error to the first error of the specified error level that owns any errors.
//
//	@return						true if any next module with errors exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToNextModuleWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(_pModule);	// must exist

	if	(_bErrorLevelContinuity 
		|| _eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	}
	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_pModule = pErrorManager->GetNextModule(_pModule);
	while(_pModule)
	{
		if	((_bErrorLevelContinuity && _pModule->HasError())
			||(!_bErrorLevelContinuity && _pModule->HasError(_eErrorLevel)))
		{
			_pError = _pModule->GetFirstError(_eErrorLevel);
			QDT_ASSERT(_bErrorLevelContinuity || _pError);
			return true;
		}
		_pModule = pErrorManager->GetNextModule(_pModule);
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToPreviousModule
//	Object:		Go to the previous module.
//				Keeps the current error level if ErrorLevelContinuity is true, sets error level to EL_MAX-1 otherwise.
//				Set the current error to the last error of the last of the specified error level.
//
//	@return		true if the previous module exists, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToPreviousModule()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(_pModule);	// must exist

	_pModule = pErrorManager->GetPreviousModule(_pModule);

	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if	(_bErrorLevelContinuity
		|| _eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX-1);
	}

	_pError = _pModule->GetLastError(_eErrorLevel);
	return true;
}

//-----------------------------------------------------------------------------
//	Name:		GoToPreviousModuleWithErrors
//	Object:		Go to the previous module that owns any errors.
//				Only in the current error level if ErrorLevelContinuity is not true, starts from EL_MAX-1 otherwise.
//				Set the current error to the last error of the specified error level that owns any errors.
//
//	@return		true if any previous module with errors exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToPreviousModuleWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(_pModule);	// must exist

	if	(_bErrorLevelContinuity
		|| _eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
	}

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW 
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_pModule = pErrorManager->GetPreviousModule(_pModule);
	while(_pModule)
	{
		if	((_bErrorLevelContinuity && _pModule->HasError())
			||(!_bErrorLevelContinuity && _pModule->HasError(_eErrorLevel)))
		{
			_pError = _pModule->GetLastError(_eErrorLevel);
			QDT_ASSERT(_bErrorLevelContinuity || _pError);
			return true;
		}
		_pModule = pErrorManager->GetPreviousModule(_pModule);
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToFirstErrorLevel
//	Object:		Go to the first error level.
//				Only in the current module if moduleContinuity is not true, else it sets the current module to the first managed module.
//				Set the current error to the first error of the found error level.
//
//	@return		true if any module exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool ICE_ERROR_ITERATOR::GoToFirstErrorLevel()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	if(_bModuleContinuity)	// set current module to the first one
	{
		_pModule = pErrorManager->GetFirstModule();
	}

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	if(_pModule)
	{
		_pError = _pModule->GetFirstError(_eErrorLevel);
		return true;
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToFirstErrorLevelWithErrors
//	Object:		Go to the first error level that owns any errors.
//				Only in the current module if moduleContinuity is not true, else it starts from first managed module to look for the first error level with errors.
//				Set the current error to the first error of the found error level.
//
//	@return		true if a error levels with error has been found, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToFirstErrorLevelWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	if(!_pModule || _bModuleContinuity)
	{
		_pModule = pErrorManager->GetFirstModule();
	}
	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if(_bModuleContinuity)	// set current module to the first one
	{
		while(_pModule)
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
			while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
			{
				if(_pModule->HasError(_eErrorLevel))
				{
					_pError = _pModule->GetFirstError(_eErrorLevel);
					return true;
				}

				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel +1);
			};

			_pModule = pErrorManager->GetNextModule(_pModule);
		}
	}
	else
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
		while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
		{
			if(_pModule->HasError(_eErrorLevel))
			{
				_pError = _pModule->GetFirstError(_eErrorLevel);
				return true;
			}

			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel +1);
		};
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToLastErrorLevel
//	Object:		Go to the last error level.
//				Only in the current module if moduleContinuity is not true, else it sets the current module to the last managed module
//				Set the current error to the last error of the found error level.
//
//	@return		true if any error level exists, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToLastErrorLevel()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	if(_bModuleContinuity)	// set current module to the first one
	{
		_pModule = pErrorManager->GetLastModule();
	}

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
	if(_pModule)
	{
		_pError = _pModule->GetLastError(_eErrorLevel);
		return true;
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToLastErrorLevelWithErrors
//	Object:		Go to the last error level that owns any errors.
//				Only in the current module if moduleContinuity is not true, else it starts from the last managed module to look for the first error level with errors.
//				Set the current error to the lax error of the found error level.
//
//	@return		true if a error levels with error has been found, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToLastErrorLevelWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	if(!_pModule || _bModuleContinuity)
	{
		_pModule = pErrorManager->GetLastModule();
	}
	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if(_bModuleContinuity)	// set current module to the last one
	{
		while(_pModule)
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
			while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
			{
				if(_pModule->HasError(_eErrorLevel))
				{
					_pError = _pModule->GetLastError(_eErrorLevel);
					return true;
				}

				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel -1);
			};

			_pModule = pErrorManager->GetPreviousModule(_pModule);
		}
	}
	else
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
		while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
		{
			if(_pModule->HasError(_eErrorLevel))
			{
				_pError = _pModule->GetLastError(_eErrorLevel);
				return true;
			}

			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel -1);
		};
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToNextErrorLevel
//	Object:		Go to the next error level.
//				Only in the current module if moduleContinuity is not true, else it continues from the current module.
//				Set the current error to the first error of the next error level.
//
//	@return		true if the next error level exists, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToNextErrorLevel()
{
	QDT_ASSERT(_pModule);	// must exist

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
	if(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
	{
		_pError = _pModule->GetFirstError(_eErrorLevel);
		return true;
	}

	if(!_bModuleContinuity)
	{
		Invalidate();
		return false;
	}

	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	_pModule = pErrorManager->GetNextModule(_pModule);
	if(_pModule)
	{
		_pError = _pModule->GetFirstError(_eErrorLevel);
		return true;	
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToNextErrorLevelWithErrors
//	Object:		Go to the next error level that owns any errors.
//				Only in the current module if moduleContinuity is not true, else it continues from the current module to find the next error level that owns errors.
//				Set the current error to the first error of the founded error level.
//
//	@return		true if any next error levels with errors exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToNextErrorLevelWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(_pModule);	// must exist

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
	if(_bModuleContinuity)
	{
		while(_pModule)
		{
			while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
			{
				if(_pModule->HasError(_eErrorLevel))
				{
					_pError = _pModule->GetFirstError(_eErrorLevel);
					QDT_ASSERT(_pError); //must exist
					return true;
				};

				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
			};

			_pModule = pErrorManager->GetNextModule(_pModule);
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
		}
	}
	else
	{
		while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
		{
			if(_pModule->HasError(_eErrorLevel))
			{
				_pError = _pModule->GetFirstError(_eErrorLevel);
				QDT_ASSERT(_pError); //must exist
				return true;
			};

			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
		};
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToPreviousErrorLevel
//	Object:		Go to the previous error level.
//				Only in the current module if moduleContinuity is not true, else it continues from the current module.
//				Set the current error to the last error of the previous error level.
//
//	@return		true if the previous error level exists, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToPreviousErrorLevel()
{
	QDT_ASSERT(_pModule);	// must exist

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);

	if(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
	{
		_pError = _pModule->GetLastError(_eErrorLevel);
		return true;
	}

	if(!_bModuleContinuity)
	{
		Invalidate();
		return false;
	}

	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
	_pModule = pErrorManager->GetPreviousModule(_pModule);
	if(_pModule)
	{
		_pError = _pModule->GetLastError(_eErrorLevel);
		return true;
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToPreviousErrorLevelWithErrors
//	Object:		Go to the previous error level that owns any errors.
//				Only in the current module if moduleContinuity is not true, else it continues from the current module to find the previous error level that owns errors.
//				Set the current error to the last error of the founded error level.
//
//	@return		true if any previous error levels with errors exist, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToPreviousErrorLevelWithErrors()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	QDT_ASSERT(_pModule);	// must exist

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
	if(_bModuleContinuity)
	{
		while(_pModule)
		{
			while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
			{
				if(_pModule->HasError(_eErrorLevel))
				{
					_pError = _pModule->GetLastError(_eErrorLevel);
					QDT_ASSERT(_pError); //must exist
					return true;
				};

				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
			};

			_pModule = pErrorManager->GetPreviousModule(_pModule);
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
		}
	}
	else
	{
		while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
		{
			if(_pModule->HasError(_eErrorLevel))
			{
				_pError = _pModule->GetLastError(_eErrorLevel);
				QDT_ASSERT(_pError); //must exist
				return true;
			};

			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
		};
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToFirst
//	Object:		Go to the first error.
//				Only in the current errorLevel if errorLevelContinuity is not true, else it starts from the EL_UNKNOW to look for the first error.
//				Only in the current module if moduleContinuity  is not true, else it starts from the first managed module to look for the first error.
//
//	@return		true if an error has been found, false otherwise and the iterator is invalidate.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToFirst()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist

	if(!_pModule)
	{
		_pModule = pErrorManager->GetFirstModule();
	}
	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if	(_eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
	}
	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid
	
	if(_bModuleContinuity)	// start from the first module to look for the first error
	{
		ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
		QDT_ASSERT(pErrorManager);	// must exist

		_pModule = pErrorManager->GetFirstModule();

		if(_bErrorLevelContinuity)	// look in all error level
		{
			while(_pModule)
			{
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
				while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
				{
					_pError = _pModule->GetFirstError(_eErrorLevel);
					if(_pError)
					{
						return true;
					}
					_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
				}

				_pModule = pErrorManager->GetNextModule(_pModule);
			}
		}
		else	// only look for the first error in the current error level
		{
			while(_pModule)
			{
				_pError = _pModule->GetFirstError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}

				_pModule = pErrorManager->GetNextModule(_pModule);
			}
		}
	}
	else	// only look for the first error in the current module
	{
		if(_bErrorLevelContinuity)	// look in all error level
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
			while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
			{
				_pError = _pModule->GetFirstError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
			}
		}
		else	// only look for the first error in the current error level
		{
			_pError = _pModule->GetFirstError(_eErrorLevel);
			return (_pError!=NULL);
		}
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToLast
//	Object:		Go to the last error.
//				Only in the current errorLevel if errorLevelContinuity is not true, else it starts from the EL_MAX-1 to look for the last error.
//				Only in the current module if moduleContinuity  is not true, else it starts from the MI_MAX-1 to look for the last error.
//
//	@return		true if an error has been found, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToLast()
{
	ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
	QDT_ASSERT(pErrorManager);	// must exist
	if(!_pModule)
	{
		_pModule = pErrorManager->GetLastModule();
	}
	if(!_pModule)
	{
		Invalidate();
		return false;
	}

	if	(_eErrorLevel <= ICE_ERROR_ABC::EL_UNKNOW
		|| _eErrorLevel >= ICE_ERROR_ABC::EL_MAX)
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
	}
	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	if(_bModuleContinuity)	// start from the last module to look for the last error
	{
		_pModule = pErrorManager->GetLastModule();

		if(_bErrorLevelContinuity)	// look in all error level
		{
			while(_pModule)
			{
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
				while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
				{
					_pError = _pModule->GetLastError(_eErrorLevel);
					if(_pError)
					{
						return true;
					}
					_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
				}

				_pModule = pErrorManager->GetPreviousModule(_pModule);
			}
		}
		else	// only look for the last error in the current error level
		{
			while(_pModule)
			{
				_pError = _pModule->GetLastError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}

				_pModule = pErrorManager->GetPreviousModule(_pModule);
			}
		}
	}
	else	// only look for the last error in the current module
	{
		if(_bErrorLevelContinuity)	// look in all error level
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX -1);
			while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
			{
				_pError = _pModule->GetLastError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
			}
		}
		else	// only look for the last error in the current error level
		{
			_pError = _pModule->GetLastError(_eErrorLevel);
			return (_pError!=NULL);
		}
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToNext
//	Object:		Go to the next error.
//				Only in the current errorLevel if errorLevelContinuity is not true, else it continues from the current error level to look for the next error.
//				Only in the current module if moduleContinuity  is not true, else it continues from the current module to look for the next error.
//
//	@return		true if an error has been found, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToNext()
{
	QDT_ASSERT(_pModule);	// must exist

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	QDT_ASSERT(_pError);	// must exist

	ICE_ERROR_ABC * pErr = _pModule->GetNextError(_pError);
	if	(pErr 
		&& pErr->GetErrorLevel() == _pError->GetErrorLevel())
	{
		_pError = pErr;
        return true;
	}
	
	if(_bModuleContinuity)	// look in all modules from the current one
	{
		ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
		QDT_ASSERT(pErrorManager);	// must exist

		if(_bErrorLevelContinuity)	// look in all error level from the current one
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel +1);
			while(_pModule)
			{
				while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
				{
					_pError = _pModule->GetFirstError(_eErrorLevel);
					if(_pError)
					{
						return true;
					}
					_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel +1);
				}

				_pModule = pErrorManager->GetNextModule(_pModule);
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_UNKNOW + 1);
			}
		}
		else	// look only in the current error level
		{
			_pModule = pErrorManager->GetNextModule(_pModule);
			while(_pModule)
			{
				_pError = _pModule->GetFirstError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}
				_pModule = pErrorManager->GetNextModule(_pModule);
			}
		}
		
		Invalidate();
		return false;
	}
	
	// look only in the current module
	if(_bErrorLevelContinuity)	// look in all error level from the current one
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
		while(_eErrorLevel < ICE_ERROR_ABC::EL_MAX)
		{
			_pError = _pModule->GetFirstError(_eErrorLevel);
			if(_pError)
			{
				return true;
			}
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel + 1);
		}
	}

	Invalidate();
	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GoToPrevious
//	Object:		Get the previous error.
//				Only in the current errorLevel if errorLevelContinuity is not true, else it continues from the current error level to look for the previous error.
//				Only in the current module if moduleContinuity  is not true, else it continues from the current module to look for the previous error.
//
//	@return		true if an error has been found, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ICE_ERROR_ITERATOR::GoToPrevious()
{
	QDT_ASSERT(_pModule);	// must exist

	QDT_ASSERT(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW
			&& _eErrorLevel < ICE_ERROR_ABC::EL_MAX);	// error level must be valid

	QDT_ASSERT(_pError);	// must exist

	ICE_ERROR_ABC * pErr = _pModule->GetPreviousError(_pError);
	if	(pErr 
		&& pErr->GetErrorLevel() == _pError->GetErrorLevel())
	{
		_pError = pErr;
        return true;
	}
	
	if(_bModuleContinuity)	// look in all modules from the current one to look for the last error
	{
		ERROR_MANAGER * pErrorManager = ERROR_MANAGER::Instance();
		QDT_ASSERT(pErrorManager);	// must exist

		if(_bErrorLevelContinuity)	// look in all error level from the current one
		{
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel -1);
			while(_pModule)
			{
				while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
				{
					_pError = _pModule->GetLastError(_eErrorLevel);
					if(_pError)
					{
						return true;
					}
					_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel -1);
				}

				_pModule = pErrorManager->GetPreviousModule(_pModule);
				_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(ICE_ERROR_ABC::EL_MAX - 1);
			}
		}
		else	// look only in the current error level
		{
			_pModule = pErrorManager->GetPreviousModule(_pModule);
			while(_pModule)
			{
				_pError = _pModule->GetLastError(_eErrorLevel);
				if(_pError)
				{
					return true;
				}

				_pModule = pErrorManager->GetPreviousModule(_pModule);
			}
		}

		Invalidate();
		return false;
	}
	
	// look only in the current module
	if(_bErrorLevelContinuity)	// look in all error level from the current one
	{
		_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
		while(_eErrorLevel > ICE_ERROR_ABC::EL_UNKNOW)
		{
			_pError = _pModule->GetLastError(_eErrorLevel);
			if(_pError)
			{
				return true;
			}
			_eErrorLevel = static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(_eErrorLevel - 1);
		}
	}

	Invalidate();
	return false;
}

}; // KCORE

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
