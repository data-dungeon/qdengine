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
//	01-09-03	ONA - Updated categories includes
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (RAY_CAST_PARAMETERS)
//
//	The RAY_CAST_PARAMETERS is a container for all the parameters need to make
//	a ray cast. To cast a ray, you have first to create a object of the class
//	RAY_CAST_PARAMETERS with the ray you want to cast. This object is the 
//	parameter of the methode NODE::RayCast(RAY_CAST_PARAMETERS &) which test
//	the ray with the node & all this children.
//	There are different parameters in this class :
//		- Some flags
//		- The ray we cast (of course)
//		- The ray in the local repere of a node
//		- The node of the impact
//		- The distance of the impact 
//		- The position of the impact 
//		- The normal of the impact
//		- The triangle of the impact
//		- The trigger we hit (used when flag TEST_TRIGGER is active)
//		- The distance of the trigger we hit (used when flag TEST_TRIGGER is active)
//
//	NOTE :  If the flags TEST_TRIGGER is activated :
//			the Ray can hit two nodes : the first node contains the impact
//			like in classical raycasting. The second node is the first
//			trigger hit by the node. The triggers don't stop the rays, but
//			if the ray hit a trigger, it can be useful to send a message
//			for game reasons.
//
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/RayCaster/RayCastParameters)

#ifdef _DEBUG
	#include "RayCastParameters.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RAY_CAST_PARAMETERS constructor
//	Object:		
//	01-07-26:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CAST_PARAMETERS::RAY_CAST_PARAMETERS()
:
_nFlags					(0),
_ImpactNodeID			(0),
_PortalInNodeID			(0),
_CurrentNodeID			(0),
_rImpactDistance		(QDT_FLOAT32_MAX),
_ImpactNormal			(0.0f, 0.0f, 0.0f),
_nNumGeomHeaderHitted	(-1),
_nNumTriangleHitted		(-1),
_bCatchPrimitiveSet		(false)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS constructor
//	Object: Constructor with a ray
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CAST_PARAMETERS::RAY_CAST_PARAMETERS(const RAY & Ray)
:
_nFlags					(0),
_Ray					(Ray),
_ImpactNodeID			(0),
_PortalInNodeID			(0),
_CurrentNodeID			(0),
_rImpactDistance		(QDT_FLOAT32_MAX),
_ImpactNormal			(VECTOR::ZERO),
_nNumGeomHeaderHitted	(-1),
_nNumTriangleHitted		(-1),
_bCatchPrimitiveSet		(false)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS constructor
//	Object: Copy constructor
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CAST_PARAMETERS::RAY_CAST_PARAMETERS(const RAY_CAST_PARAMETERS & RayCastParameters)
{
	Copy(RayCastParameters);
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS destructor
//	Object: Destructor
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CAST_PARAMETERS::~RAY_CAST_PARAMETERS()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name: RAY_CAST_PARAMETERS::operator 
//	Object: Operator =
//	01-02-27:	RMA - Created
//-----------------------------------------------------------------------------
RAY_CAST_PARAMETERS & RAY_CAST_PARAMETERS::operator = (const RAY_CAST_PARAMETERS & RayCastParameters)
{
	if (this != &RayCastParameters)
	{
		Copy(RayCastParameters) ;
	}

	return (*this) ;
}

//-----------------------------------------------------------------------------
//	Name:		UpdateRayCastParameter
//	Object:		Call this method when the ray hit a triangle
//	01-10-04:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_CAST_PARAMETERS::TestTriangleHit(const TRIANGLE	&TriangleHit,
										     float			 rDistance)
{
	VECTOR Normal;	// No need to initialize

	if (rDistance < _rImpactDistance)
	{
		_rImpactDistance	= rDistance;
		_ImpactTriangle		= TriangleHit;

		Normal.Cross(TriangleHit.GetPoint1() - TriangleHit.GetPoint0(), TriangleHit.GetPoint2() - TriangleHit.GetPoint1()); 
							
		Normal.Normalize();
		GetGlobalMatrix().TransformDirection(Normal);
		_ImpactNormal = Normal;
		
		return (true);
	}

	return (false);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
