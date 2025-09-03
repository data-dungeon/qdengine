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
// CLASS: CIRCLE
//
//
//00-12-18 JVA Created: 
//*****************************************************************************

#include "Root.h"
#include INCL_KMATH(Math/Math)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes1D\Vector\Vector)
#include INCL_KMATH(Math\Geometry\Shapes\Shapes2D\Circle\Circle)
#include INCL_KCORE(stream/iostream)

#ifdef _DEBUG
	#include "Circle.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	CIRCLE::Read(IO_STREAM &	s)
{
	s >> _Center >> _rRadius;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	CIRCLE::Write(IO_STREAM &	s) const
{
	s << _Center << _rRadius;
}

//-----------------------------------------------------------------------------
// Name: CIRCLE::GetRandomPoint
// Object: 
//00-12-07 JVA Created: 
//-----------------------------------------------------------------------------
void	CIRCLE::GetRandomPoint(VECTOR &Point) const
{
	// We assume that _Axis = (0.0 1.0 0.0)

	float	x, z;
	float	rRandAngle;
	VECTOR	Vector;

	rRandAngle = MATH::Rand(MATH::PI() * 2.0f);

	MATH::GetCosinusAndSinus(rRandAngle, x, z);
	
	Vector.SetXYZ(x, 0.0f, z);

	Vector.SetLength(_rRadius);

	Point.SetX(Vector.GetX());
	Point.SetY(Vector.GetY());
	Point.SetZ(Vector.GetZ());
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
