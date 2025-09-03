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
//	CLASS:	INTERSECTION_HEXAHEDRON
//
//	01-10-09:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include 	INCL_KMATH(Math/Geometry/Intersection/IntersectionHexahedron)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)

//-----------------------------------------------------------------------------
//	Name:		INTERSECTION_HEXAHEDRON constructor
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
INTERSECTION_HEXAHEDRON::INTERSECTION_HEXAHEDRON()
{
	QDT_FAIL();
}

//-----------------------------------------------------------------------------
//	Name:		INTERSECTION_HEXAHEDRON destructor
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
INTERSECTION_HEXAHEDRON::~INTERSECTION_HEXAHEDRON()
{
	QDT_FAIL();
}

//*****************************************************************************
//
//	POINT
//
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionHexahedronPoint
//	Object:		
//	02-04-29:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION_HEXAHEDRON::TestIntersectionHexahedronPoint(const HEXAHEDRON &	Hexahedron,
																 const VECTOR &		Point)
{
	if ( Hexahedron.GetPlaneNear().GetSide( Point ) == true )
	{
		return ( false );
	}
	if ( Hexahedron.GetPlaneFar().GetSide( Point ) == true )
	{
		return ( false );
	}
	if ( Hexahedron.GetPlaneLeft().GetSide( Point ) == true )
	{
		return ( false );
	}
	if ( Hexahedron.GetPlaneRight().GetSide( Point ) == true )
	{
		return ( false );
	}
	if ( Hexahedron.GetPlaneTop().GetSide( Point ) == true )
	{
		return ( false );
	}
	if ( Hexahedron.GetPlaneBottom().GetSide( Point ) == true )
	{
		return ( false );
	}

	return ( true );
}



//*****************************************************************************


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
