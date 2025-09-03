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
// CLASS: POINT_2D
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include "Root.h"
#include INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include INCL_KMATH(Math/Math)
#include INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include "Point2D.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT2D::Read(IO_STREAM &	s)
{
	s >> _rX >> _rY;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	POINT2D::Write(IO_STREAM &	s) const
{
	s << _rX;
	s.DebugOutput("\t");

	s << _rY;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
