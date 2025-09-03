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
//
//	06-02-14:	CAU - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KCORE(IceError/IceTextError)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

namespace	QDT
{

namespace	KCORE
{

#ifdef _DEBUG
	#include "IceTextError.inl"
#endif //_DEBUG

//-----------------------------------------------------------------------------
//	Name:		ICE_TEXT_ERROR constructor
//	Object:		Class Constructor.
//
//	@param		iErrorCode	the code of the error
//	@param		eErrorLevel	the ERROR_LEVEL of the error
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_TEXT_ERROR::ICE_TEXT_ERROR(const int			iErrorCode,
							   const ERROR_LEVEL &	eErrorLevel)
:
ICE_ERROR_ABC(iErrorCode, eErrorLevel)
{

}

//-----------------------------------------------------------------------------
//	Name:		~ICE_TEXT_ERROR destructor
//	Object:		Class Destructor.
//
//	06-02-15:	CAU - Created
//-----------------------------------------------------------------------------
ICE_TEXT_ERROR::~ICE_TEXT_ERROR()
{

}

}; // KCore

}; // QDT

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
