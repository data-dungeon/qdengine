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
// CLASS: COLOR
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include "Root.h"
#include INCL_KMATH(Math/Color/Color)
#include INCL_KMATH(Math/Color/ColorInt)
#include INCL_KCORE(Stream/iostream)

const COLOR	COLOR::WHITE		( 255,	255,	255	);
const COLOR	COLOR::GREY			( 127,	127,	127	);
const COLOR	COLOR::BLACK		( 0,	0,		0	);
const COLOR	COLOR::RED			( 255,	0,		0	);
const COLOR	COLOR::GREEN		( 0,	255,	0	);
const COLOR	COLOR::BLUE			( 0,	0,		255	);
const COLOR	COLOR::YELLOW		( 255,	255,	0	);
const COLOR	COLOR::MAGNETA		( 255,	0,		255	);
const COLOR	COLOR::CYAN			( 0,	255,	255	);
const COLOR	COLOR::ORANGE		( 255,	127,	0	);
const COLOR	COLOR::VIOLET		( 127,	0,		127	);
const COLOR	COLOR::DARK_RED		( 127,	0,		0	);
const COLOR	COLOR::DARK_GREEN	( 0,	127,	0	);
const COLOR	COLOR::DARK_BLUE	( 0,	0,		127	);
const COLOR	COLOR::DARK_GREY	( 64,	64,		64	);

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		Default constructor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const Bool	bFlagInit)
{
	if	(bFlagInit)
	{
		_nInt32 = 0xFF000000;
	}
}

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		Copy constructor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const COLOR & Color)
:
_nInt32(Color._nInt32)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		Unsigned long member constructor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const unsigned int nARGB)
:
_nInt32(nARGB)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		Four UChar members constructor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const UChar	cA, 
			 const UChar	cR, 
			 const UChar	cG, 
			 const UChar	cB)
{
	_cB = cB;
	_cG = cG;
	_cR = cR;
	_cA = cA;
}

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		Three UChar members constructor
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const UChar	cR, 
			 const UChar	cG, 
			 const UChar	cB)
{
	_cB = cB;
	_cG = cG;
	_cR = cR;
	_cA = 0xFF;
}

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		
//	02-07-09:	RMA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const COLOR_INT &	C)
{
	COLOR_INT Borned = C;
	Borned.Born();

	_cB = static_cast<UChar>(Borned.GetBlue());
	_cG = static_cast<UChar>(Borned.GetGreen());
	_cR = static_cast<UChar>(Borned.GetRed());
	_cA = static_cast<UChar>(Borned.GetAlpha());
}

//-----------------------------------------------------------------------------
//	Name:		COLOR constructor
//	Object:		Constructor from a COLOR_FLOAT
//	00-11-10:	JVA - Created
//-----------------------------------------------------------------------------
COLOR::COLOR(const COLOR_FLOAT & Color)
{
	COLOR_FLOAT ClampedColor = Color;
	ClampedColor.Clamp();

	_cA = static_cast<UChar>(ClampedColor.GetAlpha() * 255);
	_cR = static_cast<UChar>(ClampedColor.GetRed() * 255);
	_cG = static_cast<UChar>(ClampedColor.GetGreen() * 255);
	_cB = static_cast<UChar>(ClampedColor.GetBlue() * 255);
}

//-----------------------------------------------------------------------------
//	Name:		COLOR destructor
//	Object:		Destructor
//	01-03-19:	RMA - Created
//-----------------------------------------------------------------------------
COLOR::~COLOR()
{
	//	Do nothing...
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	02-02-05:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR::Read(IO_STREAM &	s)
{
	s >> _nInt32;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	02-02-05:	RMA - Created
//-----------------------------------------------------------------------------
void	COLOR::Write(IO_STREAM &	s) const
{
	s << _nInt32;
	s.DebugOutput(QDT_COMMENT("\t"));

	s.DebugOutput(QDT_COMMENT("R = "));
	s.DebugOutput((int)_cR);
	s.DebugOutput(QDT_COMMENT("\t"));

	s.DebugOutput(QDT_COMMENT("G = "));
	s.DebugOutput((int)_cG);
	s.DebugOutput(QDT_COMMENT("\t"));

	s.DebugOutput(QDT_COMMENT("B = "));
	s.DebugOutput((int)_cB);
	s.DebugOutput(QDT_COMMENT("\t"));

	s.DebugOutput(QDT_COMMENT("A = "));
	s.DebugOutput((int)_cA);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
