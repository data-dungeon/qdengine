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
// CLASS: RAY_CAST_PARAMETERS
//
//
//	01-02-27:	RMA - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Reset
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY_CAST_PARAMETERS::Reset()
{
	_nFlags					= 0;
	_ImpactNodeID			= 0;
	_PortalInNodeID			= 0;
	_CurrentNodeID			= 0;
	_rImpactDistance		= QDT_FLOAT32_MAX;
	_nNumGeomHeaderHitted	= -1;
	_nNumTriangleHitted		= -1;
	_ImpactNormal.SetXYZ(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::Copy
//	Object: Copy method to avoid code duplication in operator = & copy constructor
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::Copy(const RAY_CAST_PARAMETERS & RayCastParameters)
{
	_nFlags				= RayCastParameters._nFlags;
	_Ray				= RayCastParameters._Ray;
	_ImpactNodeID		= RayCastParameters._ImpactNodeID;
	_rImpactDistance	= RayCastParameters._rImpactDistance;
	_ImpactNormal		= RayCastParameters._ImpactNormal;
	_PortalInNodeID		= RayCastParameters._PortalInNodeID;
	_CurrentNodeID		= RayCastParameters._CurrentNodeID;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetFlag
//	Object: Get the flags mask
//	01-02-05:	RMA - Created
//-----------------------------------------------------------------------------
int RAY_CAST_PARAMETERS::GetFlags(int nFlagsMask) const
{
	return (_nFlags & nFlagsMask) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetFlag
//	Object: Set the flags mask of the node to the flags set 
//	01-02-05:	RMA - Created
//-----------------------------------------------------------------------------	
void RAY_CAST_PARAMETERS::SetFlags(int nFlagsMask, int nFlagsSet)
{
	_nFlags &= ~nFlagsMask ;
	_nFlags |=  nFlagsSet ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetRay
//	Object: Get the ray
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
const RAY & RAY_CAST_PARAMETERS::GetRay() const
{
	return (_Ray) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetRay
//	Object: Set the ray
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::SetRay(const RAY & Ray)
{
	_Ray = Ray ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetImpactNode
//	Object: Get the impact node
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
const int RAY_CAST_PARAMETERS::GetImpactNodeID() const
{
	return (_ImpactNodeID) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetImpactNode
//	Object: Set the impact node
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::SetImpactNodeID(const int ImpactNodeID)
{
	_ImpactNodeID = ImpactNodeID;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetGlobalMatrix
//	Object: Get the global matrix of the current node
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
const MATRIX & RAY_CAST_PARAMETERS::GetGlobalMatrix() const
{
	return (_GlobalMatrix) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetGlobalMatrix
//	Object: Set the global matrix of the current node
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::SetGlobalMatrix(const MATRIX & Matrix)
{
	_GlobalMatrix = Matrix ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetImpactDistance
//	Object: Get the impact distance
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
float RAY_CAST_PARAMETERS::GetImpactDistance() const
{
	return (_rImpactDistance) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetImpactDistance
//	Object: Set the impact distance
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::SetImpactDistance(float rValue)
{
	_rImpactDistance = rValue ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetNormalImpact
//	Object: Get the normal impact
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR & RAY_CAST_PARAMETERS::GetImpactNormal() const
{
	return (_ImpactNormal) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetNormalImpact
//	Object: Set the normal impact
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::SetImpactNormal(const VECTOR & Normal)
{
	_ImpactNormal = Normal ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::GetImpactTriangle
//	Object: Get the triangle of the impact
//	01-02-28:	RMA - Created
//-----------------------------------------------------------------------------
const TRIANGLE & RAY_CAST_PARAMETERS::GetImpactTriangle() const
{
	return (_ImpactTriangle) ;
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::SetImpactTriangle
//	Object: Set the triangle of the impact
//	01-02-28:	RMA - Created
//-----------------------------------------------------------------------------
void RAY_CAST_PARAMETERS::SetImpactTriangle(const TRIANGLE & Triangle)
{
	_ImpactTriangle = Triangle ;
}

//-----------------------------------------------------------------------------
//	Name:		GetNumTriangleHitted
//	Object:		Get the numero of the triangle hitted
//	01-05-25:	RMA - Created
//-----------------------------------------------------------------------------
int	RAY_CAST_PARAMETERS::GetNumTriangleHitted() const
{
	return (_nNumTriangleHitted);
}

//-----------------------------------------------------------------------------
//	Name:		SetNumTriangleHitted
//	Object:		Set the numof the triangle hitted
//	01-05-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY_CAST_PARAMETERS::SetNumTriangleHitted(int	nNum)
{
	_nNumTriangleHitted = nNum;
}

//-----------------------------------------------------------------------------
//	Name:		GetNumGeomHeaderHitted
//	Object:		Get the numero of the geometry header hitted
//	01-05-25:	RMA - Created
//-----------------------------------------------------------------------------
int	RAY_CAST_PARAMETERS::GetNumGeomHeaderHitted() const
{
	return (_nNumGeomHeaderHitted);
}

//-----------------------------------------------------------------------------
//	Name:		SetNumGeomHeaderHitted
//	Object:		Set the numero of the geometry header hitted
//	01-05-25:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY_CAST_PARAMETERS::SetNumGeomHeaderHitted(int nNum)
{
	_nNumGeomHeaderHitted = nNum;
}

//-----------------------------------------------------------------------------
//	Name:		GetImpactPoint
//	Object:		
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR	RAY_CAST_PARAMETERS::GetImpactPoint() const
{
	return (VECTOR(_Ray.GetOrigin() + _Ray.GetDirection() * _rImpactDistance));
}

//-----------------------------------------------------------------------------
//	Name:		SetPortalInNodeIndex
//	Object:		
//	01-09-11:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CAST_PARAMETERS::SetPortalInNodeID(const int NodeID)
{
	_PortalInNodeID = NodeID;
}

//-----------------------------------------------------------------------------
//	Name:		GetPortalInNodeIndex
//	Object:		
//	01-09-11:	ONA - Created
//-----------------------------------------------------------------------------
const int	RAY_CAST_PARAMETERS::GetPortalInNodeID()
{
	return (_PortalInNodeID);
}

//-----------------------------------------------------------------------------
//	Name:		SetCurrentNodeIndex
//	Object:		
//	01-09-11:	ONA - Created
//-----------------------------------------------------------------------------
void	RAY_CAST_PARAMETERS::SetCurrentNodeID(const int NodeID)
{
	_CurrentNodeID = NodeID;
}

//-----------------------------------------------------------------------------
//	Name:		GetCurrentNodeIndex
//	Object:		
//	01-09-11:	ONA - Created
//-----------------------------------------------------------------------------
const int RAY_CAST_PARAMETERS::GetCurrentNodeID()
{
	return (_CurrentNodeID);
}

//-----------------------------------------------------------------------------
//	Name:		CatchPrimitiveSet
//	Object:		
//	03-06-11:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_CAST_PARAMETERS::CatchPrimitiveSet() const
{
	return (_bCatchPrimitiveSet);
}

//-----------------------------------------------------------------------------
//	Name:		SetCatchPrimitiveSet
//	Object:		
//	03-06-11:	RMA - Created
//-----------------------------------------------------------------------------
void	RAY_CAST_PARAMETERS::SetCatchPrimitiveSet(bool	b)
{
	_bCatchPrimitiveSet = b;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
