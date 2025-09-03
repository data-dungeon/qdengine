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
// CLASS: COLOR_FLOAT_ACCUMULATOR
//
//
//	01-04-04:	RMA - Created
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (COLOR_FLOAT_ACCUMULATOR)
//
//	The COLOR_FLOAT_ACCUMULATOR is like a color float, but the internal value
//	can be superior to one, so this is useful for computing average of colors.
//
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include "Root.h"
#include INCL_KMATH(Math/Color/ColorFloatAccumulator)
#include INCL_KCORE(Stream/iostream)

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT_ACCUMULATOR constructor
//	Object:		Constructor
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT_ACCUMULATOR::COLOR_FLOAT_ACCUMULATOR()
:
_rA(0.0f),
_rR(0.0f),
_rG(0.0f),
_rB(0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT_ACCUMULATOR constructor
//	Object:		Constructor
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT_ACCUMULATOR::COLOR_FLOAT_ACCUMULATOR(const float rA,
												 const float rR, 
												 const float rG, 
												 const float rB)
:
_rA(rA),
_rR(rR),
_rG(rG),
_rB(rB)
{
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT_ACCUMULATOR destructor
//	Object:		Destructor
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT_ACCUMULATOR::~COLOR_FLOAT_ACCUMULATOR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT_ACCUMULATOR::COLOR_FLOAT_ACCUMULATOR 
//	Object:		Get color
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT COLOR_FLOAT_ACCUMULATOR::GetDiffuseColor() const
{
	float rA = _rA;
	float rR = _rR;
	float rG = _rG;
	float rB = _rB;

	if (rR >= 1.0f) rR = 1.0f;
	if (rG >= 1.0f) rG = 1.0f;
	if (rB >= 1.0f) rB = 1.0f;

	return (COLOR_FLOAT(rA, rR, rG, rB));
}

//-----------------------------------------------------------------------------
//	Name:		COLOR_FLOAT_ACCUMULATOR::COLOR_FLOAT_ACCUMULATOR 
//	Object:		Get color
//	01-04-04:	RMA - Created
//-----------------------------------------------------------------------------
COLOR_FLOAT COLOR_FLOAT_ACCUMULATOR::GetSpecularColor() const
{
	float rA, rR, rG, rB;

	rA = rR = rG = rB = 0.0f;

	if (_rR <= 1.0f) rR = 0.0f;
	if (_rG <= 1.0f) rG = 0.0f;
	if (_rB <= 1.0f) rB = 0.0f;

	if (_rR >= 2.0f) rR = 1.0f;
	if (_rG >= 2.0f) rG = 1.0f;
	if (_rB >= 2.0f) rB = 1.0f;

	if (1.0 <= _rR && _rR <= 2.0) rR = _rR - 1.0f;  
	if (1.0 <= _rG && _rG <= 2.0) rG = _rG - 1.0f;  
	if (1.0 <= _rB && _rB <= 2.0) rB = _rB - 1.0f;  

	return (COLOR_FLOAT(rA, rR, rG, rB));
}
	
//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-02-05:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT_ACCUMULATOR::Read(IO_STREAM &	s)
{
	s >> _rA >> _rR >> _rG >> _rB;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-02-05:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR_FLOAT_ACCUMULATOR::Write(IO_STREAM &	s) const
{
	s << _rA << _rR << _rG << _rB;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
