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
// CLASS: CONE
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include	"Root.h"
#include INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Cone/Cone)

#include INCL_KCORE(Stream/iostream)
#include INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)

#ifdef _DEBUG
#include "cone.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::Read(IO_STREAM &	s)
{
	s >> _Top >> _Axe >> _rRadius >> _rAngle >> _rCosAngle >> _rSinAngle;
}

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-06:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::Write(IO_STREAM &	s) const
{
	s << _Top << _Axe << _rRadius << _rAngle << _rCosAngle << _rSinAngle;
}

//-----------------------------------------------------------------------------
//	Name: CONE::Display
//	Object: 
//	01-07-04:	GGO - Created
//-----------------------------------------------------------------------------
void	CONE::Display( COLOR_FLOAT &	Color,
					   float			rLength,
					   int				nStep)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		DisplayFilled
//	Object:		
//	03-05-23:	FDE - Created
//-----------------------------------------------------------------------------
void	CONE::DisplayFilled(const COLOR_FLOAT &	Color,
							float				rLength,
							int					nStep)
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
const CONE &	CONE::operator=(const CONE & Cone)
{
	if (this != & Cone)
	{
		_Top		= Cone._Top;
		_Axe		= Cone._Axe;
		_rRadius	= Cone._rRadius;
		_rAngle		= Cone._rAngle;
		_rCosAngle	= Cone._rCosAngle;
		_rSinAngle	= Cone._rSinAngle;
	}

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-06-22:	RMA - Created
//-----------------------------------------------------------------------------
void	CONE::Transform(const MATRIX	&Matrix)
{
	Matrix.TransformPosition(_Top);
	Matrix.TransformDirection(_Axe);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	CONE::Intersect(const HEXAHEDRON &	Hexahedron) const
{
	// Transform cone to OBB
	OBB	ConeOBB;

	float	rDistance1, rDistance2, rDistance3;
	VECTOR	Direction1, Direction2, Direction3, Center;
	VECTOR	Tmp;
	PLANE	P;

	rDistance1 = _rRadius / 2;
	rDistance2 = rDistance3 = MATH::Sin(_rAngle) * _rRadius;

	Tmp.Mul(_Axe, rDistance1);
	Center.Add(_Top, Tmp);

	Direction1 = _Axe;
	P.SetFromPointAndNormal(_Top, _Axe);
	P.GetVectors(Direction2, Direction3);

	ConeOBB.SetCenter(Center);
	ConeOBB.SetDistance1(rDistance1);
	ConeOBB.SetDistance2(rDistance2);
	ConeOBB.SetDistance3(rDistance3);
	ConeOBB.SetDirection1(Direction1);
	ConeOBB.SetDirection2(Direction2);
	ConeOBB.SetDirection3(Direction3);

	// Intersect
	return	(ConeOBB.Intersect(Hexahedron));
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

