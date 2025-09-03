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
//	CLASS:	DELTA_FRAME
//
//	02-06-10:	JLT - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Frame/DeltaFrame)

#ifdef _DEBUG
#include	"DeltaFrame.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		DELTA_FRAME constructor
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
DELTA_FRAME::DELTA_FRAME()
{
	_v[0].SetXYZ(0,0,0);
	_v[1].SetXYZ(0,0,0);
}

//-----------------------------------------------------------------------------
//	Name:		DELTA_FRAME destructor
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
DELTA_FRAME::~DELTA_FRAME()
{

}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
