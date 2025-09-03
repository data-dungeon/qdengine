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
//	CLASS:	COLOR_INT
//
//	02-07-09:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Color/ColorInt)
#include	INCL_KCORE(Stream/iostream)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	03-11-14:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_INT::Read(IO_STREAM &	s)
{
	s >> _nAlpha >> _nRed >> _nGreen >> _nBlue;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	03-11-14:	FPR - Created
//-----------------------------------------------------------------------------
void	COLOR_INT::Write(IO_STREAM &	s) const
{
	s.DebugOutput("A = ");
	s << _nAlpha;
	s.DebugOutput("\t");

	s.DebugOutput("R = ");
	s << _nRed;
	s.DebugOutput("\t");

	s.DebugOutput("G = ");
	s << _nGreen;
	s.DebugOutput("\t");
	
	s.DebugOutput("B = ");
	s << _nBlue;
}



//=============================================================================
//	CODE ENDS HERE
//=============================================================================
