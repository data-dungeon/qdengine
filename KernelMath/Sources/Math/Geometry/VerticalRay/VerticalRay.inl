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
//	CLASS:	VERTICAL_RAY
//
//	03-08-27:	ELE - Created
//*****************************************************************************

#include	INCL_KMATH(Math/Math)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TestIntersection
//	Object:		
//	03-08-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	VERTICAL_RAY::TestIntersection(const VECTOR &	P1,
									   const VECTOR &	P2,
									   const VECTOR &	P3,
									   bool &			bAbove,
									   bool &			bBelow)
{
	bool	bReturn = false;

	if (TestIntersectionAbove(P1, P2, P3)) { bAbove = true; bReturn = true; }
	if (TestIntersectionBelow(P1, P2, P3)) { bBelow = true; bReturn = true; }

	return	(bReturn);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionAbove
//	Object:		
//	03-08-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	VERTICAL_RAY::TestIntersectionAbove(const VECTOR &	P1,
										    const VECTOR &	P2,
										    const VECTOR &	P3)
{
	// Optimized version which take in account the fact that the
	// projection is vertical

	float   ru, rv, rw;
	VECTOR  vT;
	VECTOR	vQ;
	VECTOR	vP;
	float	rDet;
	float	rInvDet ;

	// Find vectors for two edge sharing vertex 0.
	VECTOR vEdge1 = P2 - P1;
	VECTOR vEdge2 = P3 - P1;

	// Begin to calculate determinant, also used to calculate u parameter
	vP.SetXYZ(vEdge2.GetZ(), 0.0f, -vEdge2.GetX());					// vP.Cross(Direction , vEdge2);
	rDet = vEdge1.GetX() * vP.GetX() + vEdge1.GetZ() * vP.GetZ();	// rDet = vEdge1.Dot(vP);

	if (rDet < 0.0f)
	{
		return (false);
	}
	
	// Create a vector from vertex 0 to the origin of the ray
	vT = GetOrigin() - P1;
	vT[_Y_] -= 1.0f;

	// The projection of the vector vT along the first axis
	ru = vT.GetX() * vP.GetX() + vT.GetZ() * vP.GetZ();  // vT.Dot(vP);
	if ((ru < 0.0f) || ( ru > rDet))
	{
		return (false);
	}

	vQ.Cross(vT , vEdge1);

    // calculate rV parameter and test bounds
	rv = vQ.GetY(); // rv = Direction.Dot(vQ);
	if (rv < 0.0 || ru + rv > rDet)
	{
		return (false);
	}

	// And finally, the projection on the last axis should be negative.
	rw = vEdge2.Dot(vQ);
	rInvDet = 1.0f / rDet;
	rw *= rInvDet;
	if (rw > 0.0f && rw < _rPosDistance)
	{
		_rPosDistance = rw;	
		return (true);
	}
	
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionBelow
//	Object:		
//	03-08-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	VERTICAL_RAY::TestIntersectionBelow(const VECTOR &	P1,
											const VECTOR &	P2,
											const VECTOR &	P3)
{
	// Optimized version which take in account the fact that the
	// projection is vertical

	float   ru, rv, rw;
	VECTOR  vT;
	VECTOR	vQ;
	VECTOR	vP;
	float	rDet;
	float	rInvDet;

	// Find vectors for two edge sharing vertex 0.
	VECTOR vEdge1 = P2 - P1;
	VECTOR vEdge2 = P3 - P1;

	// Begin to calculate determinant, also used to calculate u parameter
	vP.SetXYZ(-vEdge2.GetZ(), 0.0f, vEdge2.GetX());					// vP.Cross(Direction , vEdge2);
	rDet = vEdge1.GetX() * vP.GetX() + vEdge1.GetZ() * vP.GetZ();	// rDet = vEdge1.Dot(vP);

	if (rDet < 0.0f)
	{
		return (false);
	}
	
	// Create a vector from vertex 0 to the origin of the ray
	vT = GetOrigin() - P1;
	vT[_Y_] += 1.0f;

	// The projection of the vector vT along the first axis
	ru = vT.GetX() * vP.GetX() + vT.GetZ() * vP.GetZ();  // vT.Dot(vP);
	if ((ru < 0.0f) || ( ru > rDet))
	{
		return (false);
	}

	vQ.Cross(vT , vEdge1);

    // calculate rV parameter and test bounds
	rv = -vQ.GetY(); // rv = Direction.Dot(vQ);
	if (rv < 0.0 || ru + rv > rDet)
	{
		return (false);
	}

	// And finally, the projection on the last axis should be negative.
	rw = vEdge2.Dot(vQ);
	rInvDet = 1.0f / rDet;
	rw *= rInvDet;
	if (rw > 0.0f && -rw > _rNegDistance)
	{
		_rNegDistance = -rw;	
		return (true);
	}
	
	return (false);
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
