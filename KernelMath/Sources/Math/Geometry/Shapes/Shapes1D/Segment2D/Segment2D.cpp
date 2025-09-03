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
//	CLASS:	SEGMENT_2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include "Root.h"
#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment2D/Segment2D)

#include INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include "Segment2D.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT2D::Read(IO_STREAM &	Stream)
{
	Stream >> _Point1 >> _Point2;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
void	SEGMENT2D::Write(IO_STREAM &	Stream) const
{
	Stream << _Point1 << _Point2;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
