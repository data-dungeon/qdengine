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
//	CLASS:	HEXAHEDRON
//
//	01-10-09:	GGO - Created
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Name:		HEXAHEDRON constructor
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
HEXAHEDRON::HEXAHEDRON(const HEXAHEDRON & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneNear
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlaneNear() const
{
	return ( _PlaneNear );
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneFar
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlaneFar() const
{
	return ( _PlaneFar );
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneLeft
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlaneLeft() const
{
	return ( _PlaneLeft );
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneRight
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlaneRight() const
{
	return ( _PlaneRight );
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneTop
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlaneTop() const
{
	return ( _PlaneTop );
}

//-----------------------------------------------------------------------------
//	Name:		GetPlaneBottom
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
const PLANE &	HEXAHEDRON::GetPlaneBottom() const
{
	
	return ( _PlaneBottom );
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================
