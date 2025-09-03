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
//	CLASS:	ICE_ERROR_ABC
//	The ICE_ERROR_ABC class implements the basic error concept.
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	ICE_ERROR_ABC_H
#define	ICE_ERROR_ABC_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include	INCL_KCORE(Tools/QDT_List)
#include	INCL_KCORE(Tools/QdtString)
#include	INCL_KCORE(TextDataFile/TextDataFileABC)

namespace	QDT
{
	namespace	KCORE
	{
		class	ICE_ERROR_ABC : public QDT_LIST_ELEMENT
		{
		public :
			/** Error levels enumeration. */
			enum	ERROR_LEVEL
			{
				EL_UNKNOW = -1,
				EL_WARNING1,
				EL_WARNING2,
				EL_WARNING3,
				EL_WARNING4,
				EL_ERROR,
				EL_FATAL_ERROR,
				EL_MAX
			};

			/** Access to the name of an Error Level as a string
			* @param	eErrorLevel	the error level
			* @return				the name of the error level
			*/
			static QDT_STRING GetErrorLevel(const ERROR_LEVEL& eErrorLevel);

		public :
			/** Class Constructor.
			* @param	iErrorCode	the code of the error
			* @param	eErrorLevel	the level of the error
			*/
					ICE_ERROR_ABC(const int iErrorCode, const ERROR_LEVEL eErrorLevel);

			/** Class Destructor.
			*/
			virtual	~ICE_ERROR_ABC();

		public :
			/** Error Level accessor.
			* @return	the error level
			*/
			_INLINE_	const ERROR_LEVEL&	GetErrorLevel()	const;

			/** Error Code Accessor.
			* @return	the error code
			*/
			_INLINE_	const int&			GetErrorCode()	const;

			/** Checks error' level
			* @return	true if the level is set to a warning, false otherwise.
			*/
			_INLINE_	bool				IsWarning()		const;
			/** Checks error' level
			* @return	true if the level is set to an error, false otherwise.
			*/
			_INLINE_	bool				IsError()		const;
			/** Checks error' level
			* @return	true if the level is set to a fatal error, false otherwise.
			*/
			_INLINE_	bool				IsFatalError()	const;

			/** Access error creation time
			* @return	the time stamp of the error
			*/
			_INLINE_	const UInt64&		GetErrorTime()		const;

		public :	// to overload
			/** This method must be overloaded to return the error as a {@link QD_STRING}.
			* @return	the error as a string
			*/
			virtual				QDT_STRING	GetErrorMessage() const = 0;

			/** This method can be overloaded to dump additionnal information about the error
			* @param	pTextDataFile	the text stream to write into
			*/
			virtual	_INLINE_	void		Dump(TEXT_DATA_FILE_ABC * pTextDataFile) const;

		private :
			/** the error code */
			int			_nCode;
			/** the error level */
			ERROR_LEVEL	_eLevel;
			/** the error creation time */
			UInt64		_nTime;
		}; // ICE_ERROR_ABC

		#ifndef _DEBUG
			#include	"IceErrorABC.inl"
		#endif // _DEBUG

	}; // KCORE

}; // QDT

#endif //ICE_ERROR_ABC_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
