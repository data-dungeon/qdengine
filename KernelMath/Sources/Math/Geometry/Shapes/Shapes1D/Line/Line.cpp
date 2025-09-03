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
// CLASS: LINE
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include "Root.h"
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Line/Line)
#include INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include "line.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	LINE::Read(IO_STREAM &	s)
{
	s >> _Point >> _Direction;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	LINE::Write(IO_STREAM &	s) const
{
	s << _Point << _Direction;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
