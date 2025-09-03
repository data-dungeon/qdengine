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
//	CLASS:	ZONE_DISPLAY_GEOMETRY
//
//	03-09-01:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetGeometry
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
GEOMETRY_ABC *	ZONE_DISPLAY_GEOMETRY::GetGeometry() const
{
	return (_pGeometry);
}

//-----------------------------------------------------------------------------
//	Name:		GetMaterial
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
const COM_INSTANCE &	ZONE_DISPLAY_GEOMETRY::GetMaterial() const
{
	return (_ciMaterial);
}

//-----------------------------------------------------------------------------
//	Name:		GetBBox
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
const AABB &	ZONE_DISPLAY_GEOMETRY::GetBBox() const
{
	return (_BBox);
}

//-----------------------------------------------------------------------------
//	Name:		GetBSphere
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
const SPHERE	ZONE_DISPLAY_GEOMETRY::GetBSphere() const
{
	return (_BSphere);
}

//-----------------------------------------------------------------------------
//	Name:		SetGeometry
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE_DISPLAY_GEOMETRY::SetGeometry(GEOMETRY_ABC *	pGeometry)
{
	_pGeometry = pGeometry;
}

//-----------------------------------------------------------------------------
//	Name:		SetMaterial
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE_DISPLAY_GEOMETRY::SetMaterial(const COM_INSTANCE &	ciMaterial)
{
	_ciMaterial = ciMaterial;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingBox
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE_DISPLAY_GEOMETRY::SetBoundingBox(const AABB &	BBox)
{
	_BBox = BBox;
}

//-----------------------------------------------------------------------------
//	Name:		SetBoundingSphere
//	Object:		
//	03-09-01:	RMA - Created
//-----------------------------------------------------------------------------
void	ZONE_DISPLAY_GEOMETRY::SetBoundingSphere(const SPHERE &	BSphere)
{
	_BSphere = BSphere;
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
