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
//	CLASS:	ERROR_MANAGER
//
//	06-02-14:	CAU - Created
//*****************************************************************************

#include "Root.h"
#include	INCL_KCORE(IceError/ErrorManager)
#include	INCL_KCORE(IceError/ErrorModuleABC)
#include	INCL_KCORE(IceError/IceErrorIterator)
#include	INCL_KCORE(OS/OS)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{

namespace KCORE
{

#ifdef _DEBUG
	#include "ErrorManager.inl"
#endif //_DEBUG

ERROR_MANAGER *	ERROR_MANAGER::_pSingleton = NULL;

//-----------------------------------------------------------------------------
//	Name:		ERROR_MANAGER constructor
//	Object:		Class Constructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MANAGER::ERROR_MANAGER()
{

}

//-----------------------------------------------------------------------------
//	Name:		~ERROR_MANAGER destructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MANAGER::~ERROR_MANAGER()
{

}

//-----------------------------------------------------------------------------
//	Name:		AddModule
//	Object:		Add a new module to manage.
//
//  @param		pModule	the module to add.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void ERROR_MANAGER::AddModule(ERROR_MODULE_ABC * pModule)
{
	QDT_ASSERT(pModule); // module must exist
	QDT_ASSERT(_lModules.Find(pModule)==-1); // pModule must not be already managed
	QDT_ASSERT(ERROR_MANAGER::FindModule(pModule->GetModuleID())==NULL); // module with same id already exists

	_lModules.InsertQueue(pModule);
}

//-----------------------------------------------------------------------------
//	Name:		RemoveModule
//	Object:		Remove a module from managed list.
//
//	@param		pModule	the module to remove.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::RemoveModule(ERROR_MODULE_ABC *	pModule)
{
	QDT_ASSERT(pModule); // module must exist
	QDT_ASSERT(_lModules.Find(pModule)!=-1); // pModule must be managed

	_lModules.Sup(pModule);
}

//-----------------------------------------------------------------------------
//	Name:		HasErrors
//	Object:		Check if one of the module has errors.
//
//	@return		true if any error exists, false otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ERROR_MANAGER::HasErrors() const
{
	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule)
	{
		if(pModule->HasError())
			return true;
	
		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrErrors
//	Object:		Access to Error count.
//
//	@return		the number of error in all the modules.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
int	ERROR_MANAGER::GetNbrErrors() const
{
	int count(0);

	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule)
	{
		count += pModule->GetNbrErrors();

		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}

	return count;
}

//-----------------------------------------------------------------------------
//	Name:		GetError
//	Object:		Access to an error.
//
//	@param		nIndex	the index of the error ]0..nIndex..GetNbrErrors].
//	@return				a pointer on the specified error if it exists, NULL otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MANAGER::GetError(const int &	nIndex) const
{
	QDT_ASSERT(nIndex>=0 && 
			   nIndex<GetNbrErrors());	// index must be valid
	int localIndex(nIndex),
		nNbrErrors;

	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule)
	{
		nNbrErrors = pModule->GetNbrErrors();
		if(localIndex<nNbrErrors)
		{
			return pModule->GetError(localIndex);
		}
		localIndex -= nNbrErrors;
	
		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		HasErrors
//	Object:		Check if the specified module has errors.
//
//	@param		pModule	the module to check
//	@return				true if any error exists in the module, false otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ERROR_MANAGER::HasErrors(ERROR_MODULE_ABC *	pModule) const
{
	QDT_ASSERT(pModule);	// module must exist
	QDT_ASSERT(_lModules.Find(pModule)!=-1);	// module must be managed
	return (pModule->GetNbrErrors()>0);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrErrors
//	Object:		Access to Error count of a specified module.
//
//	@param		pModule	the specified module.
//	@return				the number of error in the module.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
int	ERROR_MANAGER::GetNbrErrors(ERROR_MODULE_ABC *	pModule) const
{
	QDT_ASSERT(pModule);	// module must exist
	QDT_ASSERT(_lModules.Find(pModule)!=-1);	// module must be managed
	return pModule->GetNbrErrors();
}

//-----------------------------------------------------------------------------
//	Name:		GetError
//	Object:		Access to an error.
//
//	@param		pModule	the specified module.
//	@param		nIndex	the index of the error [0..nIndex..pModule->GetNbrErrors].
//	@return				a pointer on the specified error if it exists, NULL otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MANAGER::GetError(ERROR_MODULE_ABC *	pModule,
										const int			nIndex) const
{
	QDT_ASSERT(pModule);	// module must exist
	QDT_ASSERT(_lModules.Find(pModule)!=-1);	// module must be managed
	return pModule->GetError(nIndex);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrModules
//	Object:		Get the number of managed modules.
//
//	@return		the number of managed modules.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
int	ERROR_MANAGER::GetNbrModules() const
{
	return static_cast<int>(_lModules.GetNbElements());
}

//-----------------------------------------------------------------------------
//	Name:		GetModule
//	Object:		Get a module.
//
//	@param		nModuleIndex	the index of the module to get.
//	@return						the specified module.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC *	ERROR_MANAGER::GetModule(const int &	nModuleIndex) const
{
	QDT_ASSERT(nModuleIndex >= 0 
			&& nModuleIndex < static_cast<int>(_lModules.GetNbElements()));	// index must be valid
	return static_cast<ERROR_MODULE_ABC *>(_lModules.GetIndex(static_cast<unsigned long>(nModuleIndex)));
}

//-----------------------------------------------------------------------------
//	Name:		GetFirstModule
//	Object:		Get the first module.
//
//	@return		the first managed module, NULL if none
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC *	ERROR_MANAGER::GetFirstModule() const
{
	return static_cast<ERROR_MODULE_ABC *>(_lModules.GetHead());
}

//-----------------------------------------------------------------------------
//	Name:		GetLastModule
//	Object:		Get the last module.
//
//	@return		the last managed module, NULL if none
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC *	ERROR_MANAGER::GetLastModule() const
{
	return static_cast<ERROR_MODULE_ABC *>(_lModules.GetQueue());
}

//-----------------------------------------------------------------------------
//	Name:		GetNextModule
//	Object:		Get the next module.
//
//	@param		pModule	the current module.
//	@return				the next managed module after the current one, NULL if none.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC *	ERROR_MANAGER::GetNextModule(const ERROR_MODULE_ABC *	pModule) const
{
	QDT_ASSERT(pModule);	// must exist
	QDT_ASSERT(FindModule(pModule->GetModuleID())==pModule); // must be managed

	return static_cast<ERROR_MODULE_ABC *>(pModule->GetNext());
}

//-----------------------------------------------------------------------------
//	Name:		GetPreviousModule
//	Object:		Get the next module.
//
//	@param		pModule	the current module.
//	@return				the previous managed module before the current one, NULL if none.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC *	ERROR_MANAGER::GetPreviousModule(const ERROR_MODULE_ABC *	pModule) const
{
	QDT_ASSERT(pModule);	// must exist
	QDT_ASSERT(FindModule(pModule->GetModuleID())==pModule); // must be managed

	return static_cast<ERROR_MODULE_ABC *>(pModule->GetPrev());
}

//-----------------------------------------------------------------------------
//	Name:		FindModule
//	Object:		Find a module in the list from its ID.
//
//	@param		eModuleID	the id of the module to find
//	@return					a pointer of the module if it exists in the list, NULL otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC *	ERROR_MANAGER::FindModule(const MODULE_ID &	eModuleID) const
{
	QDT_ASSERT(eModuleID>ERROR_MANAGER::MI_UNKNOW
			&& eModuleID<ERROR_MANAGER::MI_MAX); // module id must be correct

	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule)
	{
		if(pModule->GetModuleID()==eModuleID)
			return pModule;

		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		NewError
//	Object:		Creates a new error.
//
//	@param		eModuleID	the id of the module.
//	@param		iErrorCode	the error code.
//	@return					a pointer on the created error.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MANAGER::NewError(const MODULE_ID &	eModuleID,
										const int			iErrorCode)
{
	ERROR_MODULE_ABC * pModule = FindModule(eModuleID);
	QDT_ASSERT(pModule); // module must be managed

	// creation of the error
    ICE_ERROR_ABC * pNewError = pModule->NewError(iErrorCode);
    if(pNewError)   // creation can fail if the manager is already throwing an error
    {
	    // callback function on the creation of the error
	    pModule->CallNewErrorCallback(pNewError);
        return pNewError;
    }

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		DeleteError
//	Object:		Delete an existing error.
//
//	@param		pError	the error to remove
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::DeleteError(ICE_ERROR_ABC *	pError)
{
	QDT_ASSERT(pError); // pError must exist

	// find the module that owns the error
	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule && !pModule->HasError(pError))
	{
		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}

	QDT_ASSERT(pModule); // error isn't managed by any module
	DeleteError(pModule, pError);
}

//-----------------------------------------------------------------------------
//	Name:		DeleteError
//	Object:		Delete an existing error specifiying the module.
//				Allows to skip the FindModule method if module is known.
//
//	@param		pModule	the module that owns the error
//	@param		pError	the error to remove
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::DeleteError(ERROR_MODULE_ABC *	pModule,
								   ICE_ERROR_ABC *		pError)
{
	QDT_ASSERT(pModule); // must exist
	QDT_ASSERT(pError); // must exist
	QDT_ASSERT(pModule->HasError(pError));	// error isn't managed by the module

	// callback function on the error removing
	pModule->CallDeleteErrorCallback(pError);
	pModule->DeleteError(pError);
}

//-----------------------------------------------------------------------------
//	Name:		Loop
//	Object:		Call the loop methods on each modules.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::Loop()
{
	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule)
	{
		pModule->Loop();

		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}
}

//-----------------------------------------------------------------------------
//	Name:		DumpCrashLog
//	Object:		Provides error outputing.
//
//	@param		pTextDataFile	the text stream to write into
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MANAGER::DumpCrashLog(TEXT_DATA_FILE_ABC * pTextDataFile) const
{
	QDT_ASSERT(pTextDataFile);	// must exist

	// Write header section
	QDT_VERIFY(pTextDataFile->PushSection("HEADER"));
		// plateform 
		QDT_VERIFY(pTextDataFile->PushElement("PLATFORM", OS::GetPlatformString()));
		// error count
		QDT_VERIFY(pTextDataFile->PushElement("NBR_ERRORS", QDT_STRING::ToString(GetNbrErrors())));
		// ...
	QDT_VERIFY(pTextDataFile->PopSection());

	// Dump Each module
	ERROR_MODULE_ABC * pModule = static_cast<ERROR_MODULE_ABC*>(_lModules.GetHead());
	while(pModule)
	{
		QDT_VERIFY(pTextDataFile->PushSection("MODULE"));
			
			pModule->Dump(pTextDataFile);

		QDT_VERIFY(pTextDataFile->PopSection());
		pModule = static_cast<ERROR_MODULE_ABC*>(pModule->GetNext());
	}
}

}; // KCORE

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
