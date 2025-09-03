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
//	CLASS:	BOUNDING_VOLUME
//
//	02-04-02:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingVolume)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/Cylinder/Cylinder)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/BoundingBox/BoundingBox)
#include 	INCL_KCORE(stream/iostream)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		BOUNDING_VOLUME constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME::BOUNDING_VOLUME(TYPE eType)
:
_eType	(eType)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		BOUNDING_VOLUME destructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME::~BOUNDING_VOLUME()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		GetRadius
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
float	BOUNDING_VOLUME::GetRadius() const
{
	QDT_FAIL();
	return (0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	03-02-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_VOLUME::IsInside(const FRUSTRUM &	Frustrum) const
{
	bool	bSide;

	if (Intersect(Frustrum.GetClippingPlane(FRUSTRUM::CP_NEAR), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Frustrum.GetClippingPlane(FRUSTRUM::CP_RIGHT), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Frustrum.GetClippingPlane(FRUSTRUM::CP_LEFT), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Frustrum.GetClippingPlane(FRUSTRUM::CP_BOTTOM), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Frustrum.GetClippingPlane(FRUSTRUM::CP_TOP), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Frustrum.GetClippingPlane(FRUSTRUM::CP_FAR), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		IsInside
//	Object:		
//	03-02-20:	ELE - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_VOLUME::IsInside(const HEXAHEDRON &	Hexahedron) const
{
	bool	bSide;

	if (Intersect(Hexahedron.GetPlaneNear(), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Hexahedron.GetPlaneRight(), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Hexahedron.GetPlaneLeft(), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Hexahedron.GetPlaneTop(), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	if (Intersect(Hexahedron.GetPlaneBottom(), bSide) == false)
	{
		if (bSide) return (false);
	}
	else
	{
		return	(false);
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		ExtractPlanes
//	Object:		
//	04-08-24:	ELE - Created
//-----------------------------------------------------------------------------
void	BOUNDING_VOLUME::ExtractPlanes() const
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-10-24:	ELE - Created
//-----------------------------------------------------------------------------
bool	BOUNDING_VOLUME::Intersect(const VECTOR &PIn,
								  VECTOR		&POut,
								  const VECTOR  &vMotion,
								  const float	rInertia,
								  const float	rE) const
{
	return	(false);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
