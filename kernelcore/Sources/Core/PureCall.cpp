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
//	Message.cpp
//
//	05-09-21:	RMA - Created
//*****************************************************************************


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include "Root.h"

#if defined (PLATFORM_PC)

extern "C" int __cdecl _purecall(void);

//-----------------------------------------------------------------------------
//	Name:		_purecall
//	Object:		used to catch pure virtual function call, display call stack instead
//				of MS standard dialog
//	01-12-03:	DAC - Created
//-----------------------------------------------------------------------------
int __cdecl	_purecall(void	)
{

	QDT_Error("PURE_VIRTUAL_FUNCTION_CALL");
	return	(0);
}

#endif

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
