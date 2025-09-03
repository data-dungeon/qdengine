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

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	KCORE_ERROR_MODULE_H
#define	KCORE_ERROR_MODULE_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include	INCL_KCORE(IceError/ErrorModuleABC)

namespace	QDT
{
	namespace	KCORE
	{
		class KCORE_ERROR_MODULE : public ERROR_MODULE_ABC
		{
		public :

			enum KCORE_ERRORS
			{
				KE_UNKNOW,
				KE_QDT_WARNING,
				KE_QDT_ERROR,
				KE_QDT_ERROR_CRITICAL,
				KE_MAX
			};

			/** Class Constructor.
			*/
			KCORE_ERROR_MODULE();

			/** Class Destructor.
			*/
			~KCORE_ERROR_MODULE();

			/** Access to the Module Name
			* @return	the name of the module
			*/
			_INLINE_	virtual		QDT_STRING	GetName() const;
		protected : 
			/** Creates a new error.
			* THIS METHOD MUST DO THE ALLOCATION OF THE ERROR 
			* AND PUSH THE NEW ERROR IN THE ERRORS LISTS (see AddError() method)
			* @param	iErrorCode	the error code
			* @return				a pointer on the created error
			*/
			virtual	ICE_ERROR_ABC *	NewError(const int iErrorCode);

			/** Delete an existing error
			* THIS METHOD MUST DO THE RELEASE OF THE ERROR
			* AND REMOVE THE DELETED ERROR FROM THE ERRORS LISTS (see RemoveError() method)
			* @param	pError	the error to remove
			*/
			virtual	void			DeleteError(ICE_ERROR_ABC * pError);
		}; // ERROR_MODULE_ABC

		#ifndef _DEBUG
			#include "KCoreErrorModule.inl"
		#endif // _DEBUG

	}; // KCORE

}; // QDT

#endif //KCORE_ERROR_MODULE_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
