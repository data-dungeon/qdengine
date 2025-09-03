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
//	02-12-16:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math\Geometry\VerticalRay\VerticalRay)

#include	INCL_KMATH(Math\Geometry\Shapes\Shapes2D\Triangle\Triangle)

#ifdef	_DEBUG
#include	"VerticalRay.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		VERTICAL_RAY constructor
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
VERTICAL_RAY::VERTICAL_RAY()
:
_Origin(0.0f, 0.0f, 0.0f),
_rPosDistance( QDT_FLOAT32_MAX),
_rNegDistance(-QDT_FLOAT32_MAX)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VERTICAL_RAY constructor
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
VERTICAL_RAY::VERTICAL_RAY(const VECTOR &	Origin)
:
_Origin(Origin),
_rPosDistance( QDT_FLOAT32_MAX),
_rNegDistance(-QDT_FLOAT32_MAX)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VERTICAL_RAY constructor
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
VERTICAL_RAY::VERTICAL_RAY(const VERTICAL_RAY & C)
:
_Origin(C._Origin),
_rPosDistance(C._rPosDistance),
_rNegDistance(C._rNegDistance)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VERTICAL_RAY destructor
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
VERTICAL_RAY::~VERTICAL_RAY()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
VERTICAL_RAY &	VERTICAL_RAY::operator=(const VERTICAL_RAY & C)
{
	_Origin			= C._Origin;
	_rPosDistance	= C._rPosDistance;
	_rNegDistance	= C._rNegDistance;

	return	(*this);
} 

//-----------------------------------------------------------------------------
//	Name:		TestIntersection
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
void	VERTICAL_RAY::TestIntersection(const TRIANGLE &	Triangle,
									   bool &			bAbove,
									   bool &			bBelow)
{
	if (TestIntersectionAbove(Triangle)) bAbove = true;
	if (TestIntersectionBelow(Triangle)) bBelow = true;
}
/*
//-----------------------------------------------------------------------------
//	Name:		TestIntersection
//	Object:		
//	03-08-27:	ELE - Created
//-----------------------------------------------------------------------------
void	VERTICAL_RAY::TestIntersection(const VECTOR &	P1,
									   const VECTOR &	P2,
									   const VECTOR &	P3,
									   bool &			bAbove,
									   bool &			bBelow)
{
	if (TestIntersectionAbove(P1, P2, P3)) bAbove = true;
	if (TestIntersectionBelow(P1, P2, P3)) bBelow = true;
}
*/
//-----------------------------------------------------------------------------
//	Name:		TestIntersectionAbove
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	VERTICAL_RAY::TestIntersectionAbove(const TRIANGLE &	Triangle)
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
	VECTOR vEdge1 = Triangle.GetPoint1() - Triangle.GetPoint0();
	VECTOR vEdge2 = Triangle.GetPoint2() - Triangle.GetPoint0();

	// Begin to calculate determinant, also used to calculate u parameter
	vP.SetXYZ(vEdge2.GetZ(), 0.0f, -vEdge2.GetX());					// vP.Cross(Direction , vEdge2);
	rDet = vEdge1.GetX() * vP.GetX() + vEdge1.GetZ() * vP.GetZ();	// rDet = vEdge1.Dot(vP);

	if (rDet < 0.0f)
	{
		return (false);
	}
	
	// Create a vector from vertex 0 to the origin of the ray
	vT = GetOrigin() - Triangle.GetPoint0();
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

/*	float   ru, rv, rw;
	VECTOR  vT(false);
	VECTOR	vQ(false);
	VECTOR	vP(false);
	float	rDet;
	float	rInvDet ;

	static VECTOR Direction(0.0f, 1.0f, 0.0f);

	// Find vectors for two edge sharing vertex 0.
	VECTOR vEdge1 = Triangle.GetPoint1() - Triangle.GetPoint0();
	VECTOR vEdge2 = Triangle.GetPoint2() - Triangle.GetPoint0();

	// Begin to calculate determinant, also used to calculate u parameter
	vP.Cross(Direction , vEdge2);
	rDet = vEdge1.Dot(vP);

	if (rDet < 0.0f)
	{
		return (false);
	}

	// Create a vector from vertex 0 to the origin of the ray
	vT = GetOrigin() - Triangle.GetPoint0();
	vT[_Y_] -= 1.0f;

	// The projection of the vector vT along the first axis
	ru = vT.Dot(vP);
	if ((ru < 0.0f) || ( ru > rDet))
	{
		return (false);
	}

	vQ.Cross( vT , vEdge1 );

    // calculate rV parameter and test bounds
	rv = Direction.Dot(vQ);
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
	
	return (false);*/
}
/*
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
	VECTOR  vT(false);
	VECTOR	vQ(false);
	VECTOR	vP(false);
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
*/
//-----------------------------------------------------------------------------
//	Name:		TestIntersectionBelow
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	VERTICAL_RAY::TestIntersectionBelow(const TRIANGLE &	Triangle)
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
	VECTOR vEdge1 = Triangle.GetPoint1() - Triangle.GetPoint0();
	VECTOR vEdge2 = Triangle.GetPoint2() - Triangle.GetPoint0();

	// Begin to calculate determinant, also used to calculate u parameter
	vP.SetXYZ(-vEdge2.GetZ(), 0.0f, vEdge2.GetX());					// vP.Cross(Direction , vEdge2);
	rDet = vEdge1.GetX() * vP.GetX() + vEdge1.GetZ() * vP.GetZ();	// rDet = vEdge1.Dot(vP);

	if (rDet < 0.0f)
	{
		return (false);
	}
	
	// Create a vector from vertex 0 to the origin of the ray
	vT = GetOrigin() - Triangle.GetPoint0();
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

/*	float   ru, rv, rw;
	VECTOR  vT(false);
	VECTOR	vQ(false);
	VECTOR	vP(false);
	float	rDet;
	float	rInvDet ;

	static VECTOR Direction(0.0f, -1.0f, 0.0f);

	// Find vectors for two edge sharing vertex 0.
	VECTOR vEdge1 = Triangle.GetPoint1() - Triangle.GetPoint0();
	VECTOR vEdge2 = Triangle.GetPoint2() - Triangle.GetPoint0();

	// Begin to calculate determinant, also used to calculate u parameter
	vP.Cross(Direction , vEdge2);
	rDet = vEdge1.Dot(vP);

	if (rDet < 0.0f)
	{
		return (false);
	}

	// Create a vector from vertex 0 to the origin of the ray
	vT = GetOrigin() - Triangle.GetPoint0();
	vT[_Y_] += 1.0f;

	// The projection of the vector vT along the first axis
	ru = vT.Dot(vP);
	if ((ru < 0.0f) || ( ru > rDet))
	{
		return (false);
	}

	vQ.Cross( vT , vEdge1 );

    // calculate rV parameter and test bounds
	rv = Direction.Dot(vQ);
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
	
	return (false);*/
}
/*
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
	VECTOR  vT(false);
	VECTOR	vQ(false);
	VECTOR	vP(false);
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
*/
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
