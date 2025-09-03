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

#include	"Root.h"
#include	INCL_KCORE(IceError/ErrorModuleABC)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{

namespace KCORE
{

#ifdef _DEBUG
	#include "ErrorModuleABC.inl"
#endif // _DEBUG

//-----------------------------------------------------------------------------
//	Name:		ERROR_MODULE_ABC constructor
//	Object:		Class constructor.
//
//	@param		eModuleID	the error module id
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC::ERROR_MODULE_ABC(const ERROR_MANAGER::MODULE_ID& eModuleID)
:
_eID(eModuleID),
_fNewErrorCallback(NULL),
_fDeleteErrorCallback(NULL)
{
	QDT_ASSERT(eModuleID > ERROR_MANAGER::MI_UNKNOW
			&& eModuleID < ERROR_MANAGER::MI_MAX);
}

//-----------------------------------------------------------------------------
//	Name:		ERROR_MODULE_ABC constructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ERROR_MODULE_ABC::~ERROR_MODULE_ABC()
{
	DeleteAllErrors();
}


//-----------------------------------------------------------------------------
//	Name:		GetNbrErrors
//	Object:		Access to the Module's Error count.
//
//	@return		the number of error in this module.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
int	ERROR_MODULE_ABC::GetNbrErrors() const
{
	int count(0);
	const QDT_LIST * pList = (&_aErrors[0]) + ICE_ERROR_ABC::EL_MAX - 1;
	QDT_ASSERT(pList);
	
	for(;pList>=(&_aErrors[0]);--pList)
	{
		count += pList->GetNbElements();
	}

	return count;
}

//-----------------------------------------------------------------------------
//	Name:		GetError
//	Object:		Access to an error.
//
//	@param		nIndex	the index of the error [0..nIndex..GetNbrErrors]
//	@return				a pointer on the specified error if it exists, NULL otherwise
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	ERROR_MODULE_ABC::GetError(const int &	nIndex) const
{
	QDT_ASSERT(nIndex>=0 && 
			   nIndex<GetNbrErrors());	// index must be valid

	int localIndex(nIndex);
	const QDT_LIST * pList = (&_aErrors[0]),
				   * pEndList = (&_aErrors[0]) + ICE_ERROR_ABC::EL_MAX - 1;
	QDT_ASSERT(pList);
	
	for(;pList!=pEndList;++pList)
	{
		if(localIndex < static_cast<int>(pList->GetNbElements()))
		{
			return static_cast<ICE_ERROR_ABC*>(pList->GetIndex(localIndex));
		}
		localIndex -= pList->GetNbElements();
	}

	return NULL;
}

//-----------------------------------------------------------------------------
//	Name:		HasError
//	Object:		Check if the module has at least one error.
//
//	@return		true if the module has errors, false otherwise.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ERROR_MODULE_ABC::HasError() const
{
	const QDT_LIST * pList = (&_aErrors[0]) + ICE_ERROR_ABC::EL_MAX - 1;
	QDT_ASSERT(pList);
	
	for(;pList>=(&_aErrors[0]);--pList)
	{
		if(pList->GetNbElements()!=0)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		HasError
//	Object:		Check if the module has the error.
//
//	@param		pError	the pointer on the error to find.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
bool	ERROR_MODULE_ABC::HasError(const ICE_ERROR_ABC *	pError) const
{
	QDT_ASSERT(pError);	// pError must exist
	QDT_ASSERT(pError->GetErrorLevel() > ICE_ERROR_ABC::EL_UNKNOW
			&& pError->GetErrorLevel() < ICE_ERROR_ABC::EL_MAX);	// pError Level must be valid

	const QDT_LIST * pList = &_aErrors[pError->GetErrorLevel()];
	QDT_ASSERT(pList);
	const QDT_LIST_ELEMENT * pErrIt = pList->GetHead();

	while(pErrIt)
	{
		if(pErrIt == pError)
			return true;
		pErrIt = static_cast<ICE_ERROR_ABC*>(pErrIt->GetNext());
	}

	return false;
}

//-----------------------------------------------------------------------------
//	Name:		AddError
//	Object:		Add an error in the lists.
//
//	@param		pError	the pointer on the error to add.
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::AddError(ICE_ERROR_ABC *	pError)
{
	QDT_ASSERT(pError);	// pError must exist

	QDT_ASSERT(pError->GetErrorLevel() > ICE_ERROR_ABC::EL_UNKNOW
			&& pError->GetErrorLevel() < ICE_ERROR_ABC::EL_MAX);	// pError Level must be valid

	QDT_ASSERT(!HasError(pError)); // pError mustn't be already in a list

	QDT_LIST * pList = &_aErrors[pError->GetErrorLevel()];

	QDT_ASSERT(pList);	// must exist

	pList->InsertQueue(pError);
}



//-----------------------------------------------------------------------------
//	Name:		RemoveError
//	Object:		Remove an error from the lists.
//
//	@param		pError	the pointer on the error to remove
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::RemoveError(ICE_ERROR_ABC *	pError)
{
	QDT_ASSERT(pError);	// pError must exist

	QDT_ASSERT(pError->GetErrorLevel() > ICE_ERROR_ABC::EL_UNKNOW
			&& pError->GetErrorLevel() < ICE_ERROR_ABC::EL_MAX);	// pError Level must be valid

	QDT_ASSERT(HasError(pError)); // pError must be in a list

	QDT_LIST * pList = &_aErrors[pError->GetErrorLevel()];

	QDT_ASSERT(pList); // must exist
	QDT_ASSERT(pList->Find(pError)!=-1); // error must be in the list

	pList->Sup(pError);
}

//-----------------------------------------------------------------------------
//	Name:		DeleteAllErrors
//	Object:		Delete all errors in lists and clear the lists.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::DeleteAllErrors()
{
	QDT_LIST * pList = (&_aErrors[0]) + ICE_ERROR_ABC::EL_MAX - 1;
	for(;pList >= (&_aErrors[0]);--pList)
		pList->DeleteAllElements();
}

//-----------------------------------------------------------------------------
//	Name:		Dump
//	Object:		This method dumps the module content
//
//	@param		pTextDataFile	the text stream to write into
//	06-02-20:	CAU - Created
//-----------------------------------------------------------------------------
void	ERROR_MODULE_ABC::Dump(TEXT_DATA_FILE_ABC *	pTextDataFile) const
{
	QDT_ASSERT(pTextDataFile);

	// Module' Header
	QDT_VERIFY(pTextDataFile->PushSection("HEADER"));
		// Module' id
		QDT_VERIFY(pTextDataFile->PushElement("ID",QDT_STRING::ToString(GetModuleID())));
		// Module' name
		QDT_VERIFY(pTextDataFile->PushElement("NAME",GetName()));
		// error count
		QDT_VERIFY(pTextDataFile->PushElement("NBR_ERRORS", QDT_STRING::ToString(GetNbrErrors())));
	QDT_VERIFY(pTextDataFile->PopSection());

	// Dump errors by error level
	const QDT_LIST * pList = (&_aErrors[0]),
					* pEndList = (&_aErrors[0]) + ICE_ERROR_ABC::EL_MAX - 1;
	QDT_ASSERT(pList);
	
	int i(0);
	for(;pList!=pEndList;++pList,i++)
	{
		if(!pList->IsEmpty())
		{
			QDT_VERIFY(pTextDataFile->PushSection("ERROR_LEVEL"));
				// Error Level' Header
				QDT_VERIFY(pTextDataFile->PushSection("HEADER"));
					// error level id
					QDT_VERIFY(pTextDataFile->PushElement("ID",QDT_STRING::ToString(i)));
					// error level name
					QDT_VERIFY(pTextDataFile->PushElement("NAME",ICE_ERROR_ABC::GetErrorLevel(static_cast<ICE_ERROR_ABC::ERROR_LEVEL>(i))));
					// error count
					QDT_VERIFY(pTextDataFile->PushElement("NBR_ERRORS", QDT_STRING::ToString(GetNbrErrors())));
				QDT_VERIFY(pTextDataFile->PopSection());
					
				// errors
				ICE_ERROR_ABC * pError = static_cast<ICE_ERROR_ABC*>(pList->GetHead());
				while(pError)
				{
					QDT_VERIFY(pTextDataFile->PushSection("ERROR"));
						// error time stamp
					QDT_VERIFY(pTextDataFile->PushElement("DATE",QDT_STRING::ToString(pError->GetErrorTime())));
						// error code
						QDT_VERIFY(pTextDataFile->PushElement("CODE",QDT_STRING::ToString(pError->GetErrorCode())));
						// error message
						QDT_VERIFY(pTextDataFile->PushElement("MESSAGE",pError->GetErrorMessage()));
						// error additionnal info
						pError->Dump(pTextDataFile);

					QDT_VERIFY(pTextDataFile->PopSection());

					pError = static_cast<ICE_ERROR_ABC*>(pError->GetNext());
				}
			QDT_VERIFY(pTextDataFile->PopSection());
		}
	}

	// Dump additionnal info about the module
	DumpAdditionalInfo(pTextDataFile);
}

}; // KCORE

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
