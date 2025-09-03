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
//	CLASS:	ICE_TEXT_ERROR
//	The ICE_TEXT_ERROR inherits ICE_ERROR_ABC and implements additionnal methods 
//  on error message management
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	ICE_TEXT_ERROR_H
#define	ICE_TEXT_ERROR_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include	INCL_KCORE(IceError/IceErrorABC)
#include	INCL_KCORE(Tools/QdtString)

namespace	QDT
{
	namespace	KCORE
	{
		class ICE_TEXT_ERROR : public ICE_ERROR_ABC
		{
		public :
			/** Class Constructor.
			* @param	iErrorCode	the code of the error
			* @param	eErrorLevel	the ERROR_LEVEL of the error
			*/
					ICE_TEXT_ERROR(const int iErrorCode, const ERROR_LEVEL& eErrorLevel);

			/** Class Destructor.
			*/
			virtual	~ICE_TEXT_ERROR();

		public :
			/** Adds additionnal text to the error
			* @param	sText	the additionnal text
			*/
			_INLINE_ void		SetAdditionnalErrorText(const QDT_STRING& sText);

			/** This method must be overloaded to return the error as a QDT_STRING.
			* @return	the error as a string
			*/
			virtual _INLINE_ QDT_STRING	GetErrorMessage() const;

		private :
			/** Additionnal error text */
			QDT_STRING _sText;
		}; // ICE_TEXT_ERROR

		#ifndef _DEBUG
			#include "IceTextError.inl"
		#endif //_DEBUG

	}; // KCORE

}; // QDT

#endif //ICE_TEXT_ERROR_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
