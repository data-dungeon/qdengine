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
// CLASS: SORTED_NODE_INFOS
//
//
//	01-05-14:	ELE - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		SORTED_NODE_INFOS constructor
//	Object:		
//	01-05-14:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_NODE_INFOS::SORTED_NODE_INFOS()
:
_pNode					(NULL),
_nNbrClippingPlanes		(0),
_rDistanceFromCamera	(0),
_nNumObject				(-1)
{
}

//-----------------------------------------------------------------------------
//	Name:		SORTED_NODE_INFOS destructor
//	Object:		
//	01-05-14:	ELE - Created
//-----------------------------------------------------------------------------
SORTED_NODE_INFOS::~SORTED_NODE_INFOS()
{
}

//-----------------------------------------------------------------------------
//	Name:		ResetClippingPlanes
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_NODE_INFOS::ResetClippingPlanes()
{
	_nNbrClippingPlanes = 0; 
}

//-----------------------------------------------------------------------------
//	Name:		AddClippingPlane
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_NODE_INFOS::AddClippingPlane(const PLANE & ClippingPlane) 
{
	_ClippingPlanesArray[_nNbrClippingPlanes++] = ClippingPlane;
}

//-----------------------------------------------------------------------------
//	Name:		GetNode
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
NODE*	SORTED_NODE_INFOS::GetNode() const
{
	return	(_pNode);
}

//-----------------------------------------------------------------------------
//	Name:		GetNbrClippingPlanes
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
int	SORTED_NODE_INFOS::GetNbrClippingPlanes() const
{
	return	(_nNbrClippingPlanes);
}

//-----------------------------------------------------------------------------
//	Name:		GetClippingPlane
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
const PLANE &	SORTED_NODE_INFOS::GetClippingPlane(int nNumClippingPlane) const
{
	return	(_ClippingPlanesArray[nNumClippingPlane]);
}

//-----------------------------------------------------------------------------
//	Name:		GetDistance
//	Object:		
//	01-05-14:	ELE - Created
//-----------------------------------------------------------------------------
float	SORTED_NODE_INFOS::GetDistance() const
{
	return	(_rDistanceFromCamera);
}

//-----------------------------------------------------------------------------
//	Name:		GetNumGeoHeader
//	Object:		
//	01-05-14:	ELE - Created
//-----------------------------------------------------------------------------
int	SORTED_NODE_INFOS::GetNumObject() const
{
	return	(_nNumObject);
}

//-----------------------------------------------------------------------------
//	Name:		SetNode
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_NODE_INFOS::SetNode(NODE *pNode)
{
	_pNode = pNode;
}

//-----------------------------------------------------------------------------
//	Name:		SetDistance
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_NODE_INFOS::SetDistance(float rDistance)
{
	_rDistanceFromCamera = rDistance;
}

//-----------------------------------------------------------------------------
//	Name:		SetNumObject
//	Object:		
//	01-05-15:	ELE - Created
//-----------------------------------------------------------------------------
void	SORTED_NODE_INFOS::SetNumObject(int nNumObject)
{
	_nNumObject = nNumObject;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
