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
//	CLASS:	KCORE_ERROR_MODULE
//
//	06-02-14:	CAU - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IceError/KCoreErrorModule)
#include	INCL_KCORE(IceError/KCoreError)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace QDT
{

namespace KCORE
{

#ifdef _DEBUG
	#include "KCoreErrorModule.inl"
#endif // _DEBUG

#define MAX_WARNINGS 50

//-----------------------------------------------------------------------------
//	Name:		KCORE_ERROR_MODULE constructor
//	Object:		Class Constructor.
//
//	@param		eModuleID	the error module id
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
KCORE_ERROR_MODULE::KCORE_ERROR_MODULE()
:
ERROR_MODULE_ABC(ERROR_MANAGER::MI_KCORE)
{

}

//-----------------------------------------------------------------------------
//	Name:		~KCORE_ERROR_MODULE destructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
KCORE_ERROR_MODULE::~KCORE_ERROR_MODULE()
{

}

//-----------------------------------------------------------------------------
//	Name:		NewError
//	Object:		Creates a new error.
//				THIS METHOD MUST DO THE ALLOCATION OF THE ERROR.
//				AND PUSH THE NEW ERROR IN THE ERRORS LISTS (see AddError() method).
//
//	@param		iErrorCode	the error code
//	@return					a pointer on the created error
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_ERROR_ABC *	KCORE_ERROR_MODULE::NewError(const int	iErrorCode)
{
	static bool bIsAlreadyThrowingError = false;

    if(bIsAlreadyThrowingError)
    {
        return NULL;
    }

	KCORE_ERROR * pNewError = NULL;
	
    bIsAlreadyThrowingError = true;

	switch(iErrorCode)
	{
		default:
		{
			QDT_FAIL();
		}	
		case KE_QDT_WARNING:
		{
			int nNbrWarnings = GetNbrErrors(ICE_ERROR_ABC::EL_WARNING1);
			pNewError = new KCORE_ERROR(iErrorCode, ICE_ERROR_ABC::EL_WARNING1);
			if(nNbrWarnings >= MAX_WARNINGS)
			{
				ERROR_MANAGER * pErrManager = ERROR_MANAGER::Instance();
				QDT_ASSERT(pErrManager); // must exist
				ICE_ERROR_ABC * pErr = GetFirstError(ICE_ERROR_ABC::EL_WARNING1);
				QDT_ASSERT(pErr); // must exist
				pErrManager->DeleteError(pErr);
			}
			break;
		}
		case KE_QDT_ERROR:
		{
			pNewError = new KCORE_ERROR(iErrorCode, ICE_ERROR_ABC::EL_ERROR);
			break;
		}
		case KE_QDT_ERROR_CRITICAL:
		{
			pNewError = new KCORE_ERROR(iErrorCode, ICE_ERROR_ABC::EL_FATAL_ERROR);
			break;
		}
	}

	AddError(pNewError);

    bIsAlreadyThrowingError = false;

	return pNewError;
}

//-----------------------------------------------------------------------------
//	Name:		DeleteError
//	Object:		Delete an existing error.
//				THIS METHOD MUST DO THE RELEASE OF THE ERROR.
//				AND REMOVE THE DELETED ERROR FROM THE ERRORS LISTS (see RemoveError() method).
//
//	@param		pError	the error to remove
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
void	KCORE_ERROR_MODULE::DeleteError(ICE_ERROR_ABC *	pError)
{
	QDT_ASSERT(pError);	// must exist
	RemoveError(pError);
	
	delete pError;
}

}; // KCORE

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
