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
//	CLASS:	KCORE_ERROR
//	The KCORE_ERROR inherits ICE_ERROR_ABC and implements additionnal methods 
//  on error message management
//
//	06-02-14:	CAU - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#ifndef	KCORE_ERROR_H
#define	KCORE_ERROR_H

#ifdef PRAGMA_ONCE
#pragma once
#endif //PRAGMA_ONCE

#include	INCL_KCORE(IceError/IceTextError)

namespace	QDT
{
	namespace	KCORE
	{
		class KCORE_ERROR : public ICE_TEXT_ERROR
		{
		public :
			/** Class Constructor.
			* @param	iErrorCode	the code of the error
			* @param	eErrorLevel	the ERROR_LEVEL of the error
			*/
					KCORE_ERROR(const int iErrorCode, const ERROR_LEVEL& eErrorLevel);

			/** Class Destructor.
			*/
			virtual	~KCORE_ERROR();

		}; // KCORE_ERROR

		#ifndef _DEBUG
			#include "KCoreError.inl"
		#endif //_DEBUG

	}; // KCORE

}; // QDT

#endif //KCORE_ERROR_H

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
