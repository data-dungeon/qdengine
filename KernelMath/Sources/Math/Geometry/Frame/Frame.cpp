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
//	CLASS:	FRAME
//
//	02-06-10:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Frame/Frame)

#ifdef _DEBUG
#include "Frame.inl"
#endif

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		FRAME constructor
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
FRAME::FRAME(const VECTOR &Trans, FRAME *pParent)
:
_pParent(pParent),
_Trans(Trans)
{
	_pTf.Identity();
	_fTp.Identity();
}


//-----------------------------------------------------------------------------
//	Name:		FRAME destructor
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
FRAME::~FRAME()
{

}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
