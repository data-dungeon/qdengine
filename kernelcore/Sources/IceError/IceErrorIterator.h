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
//	The ICE_ERROR_ITERATOR provides error browsing from the ERROR_MANAGER
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	ICE_ERROR_ITERATOR_H
#define	ICE_ERROR_ITERATOR_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include INCL_KCORE(IceError/ErrorModuleABC)

namespace	QDT
{

	namespace	KCORE
	{

		class ICE_ERROR_ITERATOR
		{
		public:
			/** Class constructor
			* @param	bModuleContinuity		the iterator can browse through modules if true, iterator is limited to one module otherwise
			* @param	bErrorLevelContinuity	the iterator can browse through error levels if true, iterator is limited to one error level otherwise
			*/
			ICE_ERROR_ITERATOR(const bool bModuleContinuity = true, const bool bErrorLevelContinuity = true);

			/** Class Destructor.
			*/
			~ICE_ERROR_ITERATOR();
		
		public :
			/** Get the first error
			* @param	eModuleID	the id of the module to access to get the first error
			* @param	eErrorLevel	the error level to access in the module to get the error
			* @return				returns the first error of the specified error level of the specified module if it exists, a not valid iterator otherwise
			*/
			static	ICE_ERROR_ITERATOR	Begin(const ERROR_MANAGER::MODULE_ID& eModuleID = ERROR_MANAGER::MI_UNKNOW, const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel = ICE_ERROR_ABC::EL_UNKNOW);

			/** Get the Last error
			* @param	eModuleID	the id of the module to access to get the last error
			* @param	eErrorLevel	the error level to access in the module to get the last error
			* @return				returns the last error of the specified error level of the specified module if it exists, a not valid iterator otherwise
			*/
			static	ICE_ERROR_ITERATOR	Last(const ERROR_MANAGER::MODULE_ID& eModuleID = ERROR_MANAGER::MI_UNKNOW, const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel = ICE_ERROR_ABC::EL_UNKNOW);

			/** Get an invalid iterator
			* @return an invalid iterator
			*/
			static	ICE_ERROR_ITERATOR	End();

		public :
			/** Equality operator 
			* @param	it	the iterator to test with
			* @return		true if iterators are on the same error, false otherwise
			*/
			_INLINE_	bool						operator == (const ICE_ERROR_ITERATOR& it) const;

			/** Difference operator 
			* @param	it	the iterator to test with
			* @return		false if iterators are on the same error, true otherwise
			*/
			_INLINE_	bool						operator != (const ICE_ERROR_ITERATOR& it) const;

			/** Get the current error
			* @return		the current error.
			*/
			_INLINE_	ICE_ERROR_ABC *				operator *	()	const;

			/** Get the current error
			* @return		the current error.
			*/
			_INLINE_	ICE_ERROR_ABC *				GetError() const;

		public :
			/** Set the module continuity to allow browsing through modules
			/* @param bModuleContinuity	allows or not module continuity
			*/
			_INLINE_	void						SetModuleContinuity(const bool bModuleContinuity);

			/** Get the module continuity
			* @return	the module continuity attribute
			*/
			_INLINE_	bool						GetModuleContinuity() const;

			/** Set the error level continuity to allow browsing through error levels
			/* @param bErrorLevelContinuity	allows or not error levels continuity
			*/
			_INLINE_	void						SetErrorLevelContinuity(const bool bErrorLevelContinuity);

			/** Get the error level continuity
			* @return	the error level continuity attribute
			*/
			_INLINE_	bool						GetErrorLevelContinuity() const;

		public :

			/** Allows to know if the current module has errors 
			* @return	true if the current module has errors, false otherwise
			*/
			_INLINE_	bool						ModuleHasError() const;
		
			/** Allows to know if the current error level has errors 
			* @return	true if the current error level has errors, false otherwise
			*/
			_INLINE_	bool						ErrorLevelHasError() const;

			/** Get the current module ID
			* @return	the current module ID if exists, MI_MAX otherwise
			*/
			_INLINE_	ERROR_MANAGER::MODULE_ID	GetModuleID() const;

			/** Get the current error level
			* @return	the current error level if it exists, EL_MAX otherwise
			*/
			_INLINE_	ICE_ERROR_ABC::ERROR_LEVEL	GetErrorLevel() const;
			
		
		public :
			/** Defines the current module
			*	Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_UNKNOW + 1 otherwise
			* @param	eModuleId	the module id to set as the current one
			* @return				true if the specified module exists, false otherwise
			*/
						bool				GoToModule(const ERROR_MANAGER::MODULE_ID& eModuleId);

			/** Go to the first module
			*	Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_UNKNOW + 1 otherwise
			*	Set the current error to the first error of the specified error level
			* @return	true if any module exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToFirstModule();

			/** Go to the first module that owns any errors
			*	Only in the current error level if ErrorLevelContinuity is not true, starts from EL_UNKNOW + 1 otherwise
			*	Set the current error to the first error of the specified error level that owns any errors
			* @return	true if any module with errors exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToFirstModuleWithErrors();

			/** Go to the last module
			*	Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_MAX-1 otherwise
			*	Set the current error to the last error of the specified error level
			* @return	true if any module exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToLastModule();

			/** Go to the last module that owns any errors
			*	Only in the current error level if ErrorLevelContinuity is not true, starts from EL_UNKNOW + 1 otherwise
			*	Set the current error to the last error of the specified error level that owns any errors
			* @return	true if any module with errors exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToLastModuleWithErrors();

			/** Go to the next module 
			*	Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_UNKNOW + 1 otherwise
			*	Set the current error to the first error of the specified error level
			* @return	true if the next module exists, false otherwise and the iterator is invalidate
			*/
						bool				GoToNextModule();

			/** Go to the next module that owns any errors 
			*	Only in the current error level if ErrorLevelContinuity is not true, starts from EL_UNKNOW + 1 otherwise
			*	Set the current error to the first error of the specified error level that owns any errors
			* @return	true if any next module with errors exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToNextModuleWithErrors();

			/** Go to the previous module 
			*	Keeps the current error level if ErrorLevelContinuity is not true, sets error level to EL_MAX-1 otherwise
			*	Set the current error to the last error of the last of the specified error level
			* @return	true if the previous module exists, false otherwise and the iterator is invalidate
			*/
						bool				GoToPreviousModule();

			/** Go to the previous module that owns any errors
			*	Only in the current error level if ErrorLevelContinuity is not true, starts from EL_MAX-1 otherwise
			*	Set the current error to the last error of the specified error level that owns any errors
			* @return	true if any previous module with errors exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToPreviousModuleWithErrors();

			/** Defines the current error level
			*	Sets the current error as the first error of the specified error level of the current module
			*	@param	eErrorLevel	the error level to set as the current
			*	@return				true if any module exists, false otherwise
			*/
						bool				GoToErrorLevel(const ICE_ERROR_ABC::ERROR_LEVEL& eErrorLevel);

			/** Go to the first error level
			*	Only in the current module if moduleContinuity is not true, else it sets the current module to the first managed module.
			*	Set the current error to the first error of the found error level and the iterator is invalidate
			* @return	true if any module exist, false otherwise
			*/
						bool				GoToFirstErrorLevel();

			/** Go to the first error level that owns any errors
			*	Only in the current module if moduleContinuity is not true, else it starts from the first managed module to look for the first error level with errors
			*	Set the current error to the first error of the found error level
			* @return	true if a error levels with error has been found, false otherwise and the iterator is invalidate
			*/
						bool				GoToFirstErrorLevelWithErrors();

			/** Go to the last error level
			*	Only in the current module if moduleContinuity is not true, else it sets the current module to the last managed module.
			*	Set the current error to the last error of the found error level and the iterator is invalidate
			* @return	true if any module exist, false otherwise
			*/
						bool				GoToLastErrorLevel();

			/** Go to the last error level that owns any errors
			*	Only in the current module if moduleContinuity is not true, else it starts from the last managed module to look for the first error level with errors
			*	Set the current error to the lax error of the found error level
			* @return	true if a error levels with error has been found, false otherwise and the iterator is invalidate
			*/
						bool				GoToLastErrorLevelWithErrors();

			/** Go to the next error level
			*	Only in the current module if moduleContinuity is not true, else it continues from the current module.
			*	Set the current error to the first error of the next error level
			* @return	true if the next error level exists, false otherwise and the iterator is invalidate
			*/
						bool				GoToNextErrorLevel();

			/** Go to the next error level that owns any errors
			*	Only in the current module if moduleContinuity is not true, else it continues from the current module to find the next error level that owns errors.
			*	Set the current error to the first error of the founded error level
			* @return	true if any next error levels with errors exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToNextErrorLevelWithErrors();

			/** Go to the previous error level
			*	Only in the current module if moduleContinuity is not true, else it continues from the current module.
			*	Set the current error to the last error of the previous error level
			* @return	true if the previous error level exists, false otherwise and the iterator is invalidate
			*/
						bool				GoToPreviousErrorLevel();

			/** Go to the previous error level that owns any errors
			*	Only in the current module if moduleContinuity is not true, else it continues from the current module to find the previous error level that owns errors.
			*	Set the current error to the last error of the founded error level
			* @return	true if any previous error levels with errors exist, false otherwise and the iterator is invalidate
			*/
						bool				GoToPreviousErrorLevelWithErrors();

			/** Go to the first error
			*	Only in the current errorLevel if errorLevelContinuity is not true, else it starts from the EL_UNKNOW + 1 to look for the first error
			*	Only in the current module if moduleContinuity  is not true, else it starts from the first managed module to look for the first error
			* @return	true if an error has been found, false otherwise and the iterator is invalidate
			*/
						bool				GoToFirst();

			/** Go to the last error
			*	Only in the current errorLevel if errorLevelContinuity is not true, else it starts from the EL_MAX-1 to look for the last error
			*	Only in the current module if moduleContinuity  is not true, else it starts from the last managed module to look for the last error
			* @return	true if an error has been found, false otherwise and the iterator is invalidate
			*/
						bool				GoToLast();

			/** Go to the next error
			*	Only in the current errorLevel if errorLevelContinuity is not true, else it continues from the current error level to look for the next error
			*	Only in the current module if moduleContinuity  is not true, else it continues from the current module to look for the next error
			* @return	true if an error has been found, false otherwise and the iterator is invalidate
			*/
						bool				GoToNext();

			/** Go the previous error
			*	Only in the current errorLevel if errorLevelContinuity is not true, else it continues from the current error level to look for the previous error
			*	Only in the current module if moduleContinuity  is not true, else it continues from the current module to look for the previous error
			* @return	true if an error has been found, false otherwise and the iterator is invalidate
			*/
						bool				GoToPrevious();

			/** Checks if the iterator is valid
			* @return	true if the module, error and level are corrects, false otherwise
			*/
			_INLINE_	bool				IsValid() const;

		public :
			/** Pre Increment operator
			* @return this
			*/
			_INLINE_	const ICE_ERROR_ITERATOR&	operator ++();

			/** Post Increment operator
			* @return this
			*/
			_INLINE_	ICE_ERROR_ITERATOR			operator ++ (int);

			/** Sum operator
			* @param	nInc	the step of the incrementation
			* @return			a new iterator constructed with this incremented of nInc
			*/
			_INLINE_	ICE_ERROR_ITERATOR			operator + (int nInc) const;

			/** Sum affectation operator
			* @param	nInc	the step of the incrementation
			* @return			this
			*/
			_INLINE_	const ICE_ERROR_ITERATOR&	operator +=(int nInc);

			/** Pre Decrement operator
			* @return this
			*/
			_INLINE_	const ICE_ERROR_ITERATOR&	operator --();

			/** Post Decrement operator
			* @return	a decremented copy of this
			*/
			_INLINE_	ICE_ERROR_ITERATOR			operator -- (int);

			/** Sub operator
			* @param	nDec	the step of the decrementation
			* @return			a new iterator constructed with this decremented of nDec
			*/
			_INLINE_	ICE_ERROR_ITERATOR			operator	- (int nDec) const;

			/** Sub affectation operator
			* @param	nDec	the step of the decrementation
			* @return			this
			*/
			_INLINE_	const ICE_ERROR_ITERATOR&	operator -=(int nDec);

		protected :
			/** Invalidate the iterator
			*/
			void	Invalidate();

		private :
			// browsing informations
			/** If the iterator can browse through modules */
			bool						_bModuleContinuity;
			/** If the iterator can browse through error levels */
			bool						_bErrorLevelContinuity;
			/** The current module in ERROR_MANAGER modules list */
			ERROR_MODULE_ABC *			_pModule;
			/** The current error level in the current module */
			ICE_ERROR_ABC::ERROR_LEVEL	_eErrorLevel;
			/** The current error */
			ICE_ERROR_ABC *				_pError;

		}; // ICE_ERROR_ITERATOR

		#ifndef _DEBUG
			#include "IceErrorIterator.inl"
		#endif //_DEBUG

	}; // KCORE

}; // QDT

#endif //ICE_ERROR_ITERATOR_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
