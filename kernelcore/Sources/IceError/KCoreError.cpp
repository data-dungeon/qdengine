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
//
//	06-02-14:	CAU - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IceError/KCoreError)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace	QDT
{

namespace	KCORE
{

#ifdef _DEBUG
	#include "KCoreError.inl"
#endif //_DEBUG

//-----------------------------------------------------------------------------
//	Name:		KCORE_ERROR constructor
//	Object:		Class Constructor.
//
//	@param		iErrorCode	the code of the error
//	@param		eErrorLevel	the ERROR_LEVEL of the error
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
KCORE_ERROR::KCORE_ERROR(const int				iErrorCode,
						 const ERROR_LEVEL &	eErrorLevel)
:
ICE_TEXT_ERROR(iErrorCode, eErrorLevel)
{

}

//-----------------------------------------------------------------------------
//	Name:		~KCORE_ERROR destructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
KCORE_ERROR::~KCORE_ERROR()
{

}

}; // KCore

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
