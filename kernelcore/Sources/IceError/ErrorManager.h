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
//	The ERROR_MANAGER handles a list of error modules.
//	This class is a Singleton.
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	ERROR_MANAGER_H
#define	ERROR_MANAGER_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include	INCL_KCORE(Tools/QDT_List)
#include	INCL_KCORE(TextDataFile/TextDataFileABC)

DECL_KCORE(ERROR_MODULE_ABC)
DECL_KCORE(ICE_ERROR_ABC)
DECL_KCORE(ICE_ERROR_ITERATOR)

namespace QDT
{
	namespace KCORE
	{

		class ERROR_MANAGER
		{
		public :
			/** Error modules enumeration. */
			enum MODULE_ID
			{
				MI_UNKNOW = -1,
				MI_KCORE,
				MI_KPHYSICS,
				MI_IAMSC,
				MI_KCOM,
				MI_IAMENGINE,
				MI_IAMAPP,
				MI_IAMPROJ,
				MI_SINT,
				MI_MAX
			};

		private :
			/** Class Constructor. */
			ERROR_MANAGER();

		public :
			/** Class Destructor. */
			~ERROR_MANAGER();

			/** Creates the Singleton. */
			static	_INLINE_	void			Create();
			
			/** Releases the Singleton. */
			static	_INLINE_	void			Release();

			/** Access the Singleton.
			* @return the error manager instance
			*/
			static	_INLINE_	ERROR_MANAGER *	Instance();
		
			// Return if have been instancied
			static _INLINE_		bool			IsInstancied();

			static	_INLINE_	void			SetInstance(ERROR_MANAGER * pErrorManager);

		public : // Module Management
			/** Add a new module to manage 
			* @param	pModule	the module to add
			*/
			void				AddModule(ERROR_MODULE_ABC * pModule);

			/** Remove a module from managed list
			* @param	pModule	the module to remove
			*/
			void				RemoveModule(ERROR_MODULE_ABC * pModule);

		public : // Errors in all modules
			/** Check if one of the module has errors.
			* @return	returns true if any error exists, false otherwise
			*/
			bool				HasErrors() const;

			/** Access to Error count.
			* @return	the number of error in all the modules
			*/
			int					GetNbrErrors() const;

			/** Creates a new error
			* @param	eModuleID	the id of the module
			* @param	iErrorCode	the error code
			* @return				a pointer on the created error
			*/
			ICE_ERROR_ABC *		NewError(const MODULE_ID& eModuleID, const int iErrorCode);

			/** Delete an existing error
			* @param	pError	the error to remove
			*/
			void				DeleteError(ICE_ERROR_ABC * pError);

			/** Call the loop methods on each modules
			*/
			void				Loop();

			/** Provides error outputing 
			* @param	pTextDataFile	the text stream to write into
			*/
			void				DumpCrashLog(TEXT_DATA_FILE_ABC * pTextDataFile) const;

		protected :	// prefer using ICE_ERROR_ITERATOR
			/**	Access to an error.
			* @param	nIndex	the index of the error ]0..nIndex..GetNbrErrors]
			* @return			a pointer on the specified error if it exists, NULL otherwise
			*/
			ICE_ERROR_ABC *		GetError(const int& nIndex) const;

			// Errors per module
			/** Check if the specified module has errors.
			* @param	pModule	the module to check
			* @return			returns true if any error exists in the module, false otherwise
			*/
			bool				HasErrors(ERROR_MODULE_ABC * pModule) const;

			/** Access to Error count of a specified module.
			* @param	pModule	the specified module
			* @return			the number of error in the module
			*/
			int					GetNbrErrors(ERROR_MODULE_ABC * pModule) const;

			/**	Access to an error.
			* @param	pModule	the specified module
			* @param	nIndex	the index of the error [0..nIndex..pModule->GetNbrErrors]
			* @return			a pointer on the specified error if it exists, NULL otherwise
			*/
			ICE_ERROR_ABC *		GetError(ERROR_MODULE_ABC * pModule, const int nIndex)	const;		

		protected:
			friend class ICE_ERROR_ITERATOR;

			/** Get the number of managed modules
			* @return	the number of managed modules
			*/
			int					GetNbrModules() const;

			/** Get a module
			* @param	nModuleIndex	the index of the module to get
			* @return					the specified module
			*/
			ERROR_MODULE_ABC *	GetModule(const int& nModuleIndex) const;

			/** Get the first module
			* @return	the first managed module, NULL if none
			*/
			ERROR_MODULE_ABC *	GetFirstModule() const;

			/** Get the last module
			* @return	the last managed module, NULL if none
			*/
			ERROR_MODULE_ABC *	GetLastModule() const;

			/** Get the next module
			* @param	pModule	the current module
			* @return			the next managed module after the current one, NULL if none
			*/
			ERROR_MODULE_ABC *	GetNextModule(const ERROR_MODULE_ABC * pModule) const;

			/** Get the next module
			* @param	pModule	the current module
			* @return			the previous managed module before the current one, NULL if none
			*/
			ERROR_MODULE_ABC *	GetPreviousModule(const ERROR_MODULE_ABC * pModule) const;

			/** Find a module in the list from its ID
			* @param	eModuleID	the id of the module to find
			* @return				a pointer of the module if it exists in the list, NULL otherwise
			*/
			ERROR_MODULE_ABC *	FindModule(const MODULE_ID& eModuleID) const;

			/** Delete an existing error specifiying the module
			*	Allows to skip the FindModule method if module is known
			* @param	pModule	the module that owns the error
			* @param	pError	the error to remove
			*/
			void				DeleteError(ERROR_MODULE_ABC * pModule, ICE_ERROR_ABC * pError);
		private :
			/** Singleton instance */
			static	ERROR_MANAGER *	_pSingleton;
			/** List of Managed modules */
					QDT_LIST		_lModules;
		}; // ERROR_MANAGER

		#ifndef _DEBUG
			#include "ErrorManager.inl"
		#endif //_DEBUG

	}; // KCORE	

}; // QDT

#endif //ERROR_MANAGER_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
