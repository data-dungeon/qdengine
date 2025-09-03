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
// CLASS: COLOR_FLOAT
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Color/ColorFloat)
#include	INCL_KMATH(Math/Color/Color)
#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include	"ColorFloat.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT constructor
//	Object:		Constructor from a color
//	01-03-19:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT::COLOR_FLOAT(const COLOR &	Color)
{
	_r[3] = static_cast<float>(Color.GetAlpha())	* (1 / 255.0f);
	_r[0] = static_cast<float>(Color.GetRed())		* (1 / 255.0f);
	_r[1] = static_cast<float>(Color.GetGreen())	* (1 / 255.0f);
	_r[2] = static_cast<float>(Color.GetBlue())		* (1 / 255.0f);
}

//-----------------------------------------------------------------------------
//	Name: COLOR_FLOAT::Read
//	Object: 
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::Read(IO_STREAM &s)
{
	s >> _r[3] >> _r[0] >> _r[1] >> _r[2];
}

//-----------------------------------------------------------------------------
//	Name: COLOR_FLOAT::Write
//	Object: 
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT::Write(IO_STREAM &s) const
{
	s.DebugOutput("A = ");
	s << _r[3];
	s.DebugOutput("\t");

	s.DebugOutput("R = ");
	s << _r[0];
	s.DebugOutput("\t");

	s.DebugOutput("G = ");
	s << _r[1];
	s.DebugOutput("\t");
	
	s.DebugOutput("B = ");
	s << _r[2];
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

