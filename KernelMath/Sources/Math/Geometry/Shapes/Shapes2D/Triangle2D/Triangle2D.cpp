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
//	CLASS:	TRIANGLE_2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle2D/Triangle2D)

#ifdef _DEBUG
#include	"Triangle2D.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		Contain
//	Object:		
//	01-08-28:	JLT - Created
//-----------------------------------------------------------------------------
/*
	triangle T(P0,P1,P2) M belongs T if (u,v)

	P0M = uP0P1 + vP0P2 so easy now
*/
bool TRIANGLE2D::Contain(const POINT2D &P) const
{
	POINT2D e1 = _Point2  - _Point1;
	POINT2D e2 = _Point3  - _Point1;

	float u,v;
	
	float DetM = e1[_X_] * e2[_Y_] - e1[_Y_] * e2[_X_];

	if (MATH::Abs(DetM) < 0.001f)
		return false;
	else
	{
		float InvDetM = 1.f / DetM;

		POINT2D e3 = P - _Point1;

		u = e3[_X_] * e2[_Y_] - e3[_Y_] * e2[_X_];
		u *= InvDetM;

		if (u<0.f || u>1.f)
			return false;

		v = e1[_X_] * e3[_Y_] - e1[_Y_] * e3[_X_];
		v *= InvDetM;

		if (v<0.f || (u+v)>1.f)
			return false;
		else
			return true;
	}
}



	
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
