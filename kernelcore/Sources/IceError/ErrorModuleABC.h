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
//	The ERROR_MODULE_ABC manages a list of {@link ICE_ERROR_ABC}
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	ERROR_MODULE_ABC_H
#define	ERROR_MODULE_ABC_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include	INCL_KCORE(IceError/IceErrorABC)
#include	INCL_KCORE(IceError/ErrorManager)
#include	INCL_KCORE(TextDataFile/TextDataFileABC)

DECL_KCORE(ICE_ERROR_ITERATOR)

namespace	QDT
{
	namespace	KCORE
	{
		class ERROR_MODULE_ABC : public QDT_LIST_ELEMENT
		{
		protected :
			friend class ERROR_MANAGER;	// force user to interact with the ERROR_MANAGER
			friend class ICE_ERROR_ITERATOR;

			typedef	int	(*NEW_ERROR_CALLBACK)	(ERROR_MODULE_ABC * pModule, ICE_ERROR_ABC * pError);
			typedef	int	(*DELETE_ERROR_CALLBACK)(ERROR_MODULE_ABC * pModule, ICE_ERROR_ABC * pError);

		public :
			/** Class Constructor.
			* @param	eModuleID	the module id
			*/
					ERROR_MODULE_ABC(const ERROR_MANAGER::MODULE_ID& eModuleID);

			/** Class Destructor.
			*/
			virtual	~ERROR_MODULE_ABC();

		public :
			/** Access to the Module Name
			* @return	the name of the module
			*/
			virtual		QDT_STRING						GetName() const = 0;

			/** Module ID Accessor.
			* @return	the id of the module
			*/
			_INLINE_	const ERROR_MANAGER::MODULE_ID&	GetModuleID()	const;

			/** Registers the callback of the NewError method.
			* @param	fCallBack	the callback to register
			*/
			_INLINE_	void							RegisterNewErrorCallback(NEW_ERROR_CALLBACK fCallBack);

			/** Unregister the callback of the NewError method.
			*/
			_INLINE_	void							UnregisterNewErrorCallback();

			/** Registers the callback of the DeleteError method.
			* @param	fCallBack	the callback to register
			*/
			_INLINE_	void							RegisterDeleteErrorCallback(DELETE_ERROR_CALLBACK fCallBack);

			/** Unregister the callback of the NewError method.
			*/
			_INLINE_	void							UnregisterDeleteErrorCallback();

		protected: 
			/** Creates a new error.
			* THIS METHOD MUST DO THE ALLOCATION OF THE ERROR 
			* AND PUSH THE NEW ERROR IN THE ERRORS LISTS (see AddError() method)
			* @param	iErrorCode	the error code
			* @return				a pointer on the created error
			*/
			virtual				ICE_ERROR_ABC *	NewError(const int iErrorCode) = 0;

			/** Delete an existing error
			* THIS METHOD MUST DO THE RELEASE OF THE ERROR
			* AND REMOVE THE DELETED ERROR FROM THE ERRORS LISTS (see RemoveError() method)
			* @param	pError	the error to remove
			*/
			virtual				void			DeleteError(ICE_ERROR_ABC * pError) = 0;

			/** Access to the Module's Error count.
			* @return	the number of error in this module
			*/
								int				GetNbrErrors()	const;

			/**	Access to an error.
			* @param	nIndex	the index of the error [0..nIndex..GetNbrErrors]
			* @return			a pointer on the specified error if it exists, NULL otherwise
			*/
								ICE_ERROR_ABC *	GetError(const int& nIndex) const;

			/** Get the module's error count per error level
			* @param	eErrorLevel	the error level
			* @return				the number of error in the specified error level
			*/
					_INLINE_	int				GetNbrErrors(const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel) const;

			/** Get an error of a specified error level
			* @param	eErrorLevel	the error level
			* @param	nErrorIndex	the error index in the error level
			* @return				the specified error
			*/
					_INLINE_	ICE_ERROR_ABC *	GetError(const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel, const int& nErrorIndex) const;

			/** Get the first error of a specified error level
			* @param	eErrorLevel	the error level
			* @return				the first error in the specified level
			*/
					_INLINE_	ICE_ERROR_ABC *	GetFirstError(const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel) const;

			/** Get the last error of a specified error level
			* @param	eErrorLevel	the error level
			* @return				the last error in the specified level
			*/
					_INLINE_	ICE_ERROR_ABC *	GetLastError(const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel) const;

			/** Get the next error from the specified one 
			* @param	pError		the current error
			* @return				the next error after the current one in the same error level if it exists, NULL otherwise
			*/
					_INLINE_	ICE_ERROR_ABC *	GetNextError(const ICE_ERROR_ABC * pError) const;

			/** Get the previous error from the specified one 
			* @param	pError		the current error
			* @return				the previous error before the current one  in the same error level if it exists, NULL otherwise
			*/
					_INLINE_	ICE_ERROR_ABC *	GetPreviousError(const ICE_ERROR_ABC * pError) const;


			/** Call the NewError method callback.
			* @param	pError	the error to use in the callback
			*/
					_INLINE_	void			CallNewErrorCallback(ICE_ERROR_ABC * pError);

			/** Call the DeleteError method callback.
			* @param	pError	the error to use in the callback
			*/
					_INLINE_	void			CallDeleteErrorCallback(ICE_ERROR_ABC * pError);

			/** Check if the module has at least one error
			* @return	true if the module has errors, false otherwise
			*/
								bool			HasError() const;

			/** Check if the error level has at least one error
			* @param	eErrorLevel	specifies the error level
			* @return				true if the error level has errors, false otherwise
			*/
					_INLINE_	bool			HasError(const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel) const;

			/** Check if the module has the specified error
			* @param	pError	the pointer on the error to find
			* @return			true if the module has the error, false otherwise
			*/
								bool			HasError(const ICE_ERROR_ABC * pError) const;

			/** Add an error in the lists
			* @param	pError	the pointer on the error to add
			*/
								void			AddError(ICE_ERROR_ABC * pError);

			/** Remove an error from the lists
			* @param	pError	the pointer on the error to remove
			*/
								void			RemoveError(ICE_ERROR_ABC * pError);

			/** Release all errors and clear the lists
			*/
								void			DeleteAllErrors();

			/** Loop method to provide frame per frame process
			*/
			virtual	_INLINE_	void			Loop();

			/** This method dumps the module content
			* @param	pTextDataFile	the text stream to write into
			*/
								void			Dump(TEXT_DATA_FILE_ABC * pTextDataFile) const;

			/** This method can be overloaded to dump additionnal information in the module
			* @param	pTextDataFile	the text stream to write into
			*/
			virtual	_INLINE_	void			DumpAdditionalInfo(TEXT_DATA_FILE_ABC * pTextDataFile) const;

		private :
			/** the module ID */
			ERROR_MANAGER::MODULE_ID	_eID;
			/** Callback on the NewError method */
			NEW_ERROR_CALLBACK			_fNewErrorCallback;
			/** Callback on the DeleteError method */
			DELETE_ERROR_CALLBACK		_fDeleteErrorCallback;
			/** the error list */
			QDT_LIST					_aErrors[ICE_ERROR_ABC::EL_MAX];
		}; // ERROR_MODULE_ABC

		#ifndef _DEBUG
			#include "ErrorModuleABC.inl"
		#endif // _DEBUG

	}; // KCORE

}; // QDT

#endif //ERROR_MODULE_ABC_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
