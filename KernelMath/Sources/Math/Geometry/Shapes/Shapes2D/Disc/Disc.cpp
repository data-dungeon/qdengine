//*****************************************************************************
//
// Copyright ( C ) 2000 - 2005 Quantic Dream SA
//
// These coded instructions , statements and computer programs contain
// unpublished information proprietary to Quantic Dream SA and are
// protected by French and EEC copyright laws. They may not be
// disclosed to third parties or copied or duplicated , in whole or in part,
// without prior written consent of Quantic Dream SA
//
// Unpublished - rights reserved under the Copyright Laws of the EEC.
//
//*****************************************************************************
//
// CLASS: DISC
//
//
//00 - 12 - 18 JVA Created:
//*****************************************************************************

#include 	"Root.h"
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Disc/Disc)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include 	INCL_KCORE(stream/iostream)

#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

#ifdef _DEBUG
#include "disc.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	DISC::Read(IO_STREAM &	s)
{
	s >> _Center >> _rRadius >> _Axis;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	DISC::Write(IO_STREAM &	s) const
{
	s << _Center << _rRadius << _Axis;
}

//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	DISC::Display(const COLOR_FLOAT &	Color,
					  int					nStep) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
// Name: DISC::Transform
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::Transform( const DISC &Disc , const MATRIX &Matrix )
{
	Matrix.TransformPosition( Disc._Center , _Center );
	Matrix.TransformDirection( Disc._Axis , _Axis );
}

//-----------------------------------------------------------------------------
// Name: DISC::Transform
// Object:
//00 - 11 - 13 JVA Created:
//-----------------------------------------------------------------------------
void	DISC::Transform( const MATRIX & Matrix )
{
	Matrix.TransformPosition( _Center );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
