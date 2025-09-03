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
//	CLASS:	INTERSECTION_2D
//
//	01-07-24:	RMA - Created
//*****************************************************************************

#include	"Root.h"

#define		QDT_INCLUDE_MEMORY
#include	INCL_KCORE(Includes/Includes)
#include 	INCL_KMATH(Math/Math)
#include 	INCL_KMATH(Math/Geometry/Intersection/Intersection2D)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment2D/Segment2D)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Triangle2D/Triangle2D)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/AABR2D/AABR2D)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionSegment2DSegment2D
//	Object:		Argh!!!
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION2D::TestIntersectionSegment2DSegment2D(const SEGMENT2D &	Sg1,
														   const SEGMENT2D &	Sg2)
{
	POINT2D Vector = Sg1.GetPoint2() - Sg1.GetPoint1();
	POINT2D Vector1 = Sg2.GetPoint1() - Sg1.GetPoint2();
	POINT2D Vector2 = Sg2.GetPoint2() - Sg1.GetPoint2();
	float rAngle1 = (Vector.Cross(Vector1));
	float rAngle2 = (Vector.Cross(Vector2));
	if ((rAngle1 > 0.0f) && (rAngle2 > 0.0f))	return (false);
	if ((rAngle1 < 0.0f) && (rAngle2 < 0.0f))	return (false);
	
	Vector = Sg2.GetPoint2() - Sg2.GetPoint1();
	Vector1 = Sg1.GetPoint2() - Sg2.GetPoint2();
	Vector2 = Sg1.GetPoint1() - Sg2.GetPoint2();
	rAngle1 = (Vector.Cross(Vector1));
	rAngle2 = (Vector.Cross(Vector2));
	if ((rAngle1 > 0.0f) && (rAngle2 > 0.0f))	return (false);
	if ((rAngle1 < 0.0f) && (rAngle2 < 0.0f))	return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionPoint2DAABR2D
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION2D::TestIntersectionPoint2DAABR2D(const POINT2D &	Pt,
													  const AABR2D &	AABR2D)
{
	if (Pt.GetX() > AABR2D.GetXMax()) return (false);
	if (Pt.GetY() > AABR2D.GetYMax()) return (false);
	if (Pt.GetX() < AABR2D.GetXMin()) return (false);
	if (Pt.GetY() < AABR2D.GetYMin()) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionPoint2DAABR2D
//	Object:		
//	01-07-24:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION2D::TestIntersectionPoint2DAABR2D(const POINT2D &	Pt,
													  const AABR2D &	AABR2D,
													  float				rEpsilon)
{
	if (Pt.GetX() > AABR2D.GetXMax() + rEpsilon) return (false);
	if (Pt.GetY() > AABR2D.GetYMax() + rEpsilon) return (false);
	if (Pt.GetX() < AABR2D.GetXMin() - rEpsilon) return (false);
	if (Pt.GetY() < AABR2D.GetYMin() - rEpsilon) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionPoint2DTriangle2D
//	Object:		
//	01-07-25:	RMA - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION2D::TestIntersectionPoint2DTriangle2D(const POINT2D &		Pt,
														  const TRIANGLE2D &	Triangle)
{
	POINT2D Vector1 = Triangle.GetPoint2() - Triangle.GetPoint1();
	POINT2D Vector2 = Triangle.GetPoint3() - Triangle.GetPoint2();
	POINT2D Vector3 = Triangle.GetPoint1() - Triangle.GetPoint3();

	POINT2D VectorA = Triangle.GetPoint1() - Pt;
	POINT2D VectorB = Triangle.GetPoint2() - Pt;
	POINT2D VectorC = Triangle.GetPoint3() - Pt;

	float rCross1 = Vector1.Cross(VectorA);
	float rCross2 = Vector2.Cross(VectorB);
	float rCross3 = Vector3.Cross(VectorC);

	if ((rCross1 + MATH::EPSILON() > 0) && (rCross2 + MATH::EPSILON() > 0) && (rCross3 + MATH::EPSILON() > 0))
	{
		return (true);
	}

	if ((rCross1 - MATH::EPSILON() < 0) && (rCross2 - MATH::EPSILON() < 0) && (rCross3 - MATH::EPSILON() < 0))
	{
		return (true);
	}

	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionSegment2DAABR2D
//	Object:		
//	01-09-25:	RMA - Created
//-----------------------------------------------------------------------------
/*bool	INTERSECTION2D::TestIntersectionSegment2DAABR2D(const SEGMENT2D &	Sg,
													    const AABR2D &		AABR2D)
{
	if (INTERSECTION2D::TestIntersectionPoint2DAABR2D(Sg.GetPoint1(), AABR2D))	return (true);
	if (INTERSECTION2D::TestIntersectionPoint2DAABR2D(Sg.GetPoint2(), AABR2D))	return (true);
	if (INTERSECTION2D::TestIntersectionSegment2DSegment2D(Sg, AABR2D.GetSidedSegment(AABR2D::SIDE_NORTH)))	return (true);
	if (INTERSECTION2D::TestIntersectionSegment2DSegment2D(Sg, AABR2D.GetSidedSegment(AABR2D::SIDE_SOUTH)))	return (true);
	if (INTERSECTION2D::TestIntersectionSegment2DSegment2D(Sg, AABR2D.GetSidedSegment(AABR2D::SIDE_WEST)))	return (true);
	if (INTERSECTION2D::TestIntersectionSegment2DSegment2D(Sg, AABR2D.GetSidedSegment(AABR2D::SIDE_EAST)))	return (true);
	
	return (false);
}*/

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionAABR2DAABR2D
//	Object:		
//	01-09-26:	RMA - Created
//-----------------------------------------------------------------------------
/*bool	INTERSECTION2D::TestIntersectionAABR2DAABR2D(const AABR2D &	AABR2D1,
												     const AABR2D &	AABR2D2)
{
	if (AABR2D1.GetXMax() < AABR2D2.GetXMin())	return (false);
	if (AABR2D1.GetXMin() > AABR2D2.GetXMax())	return (false);
	if (AABR2D1.GetYMax() < AABR2D2.GetYMin())	return (false);
	if (AABR2D1.GetYMin() > AABR2D2.GetYMax())	return (false);
	
	return (true);
}*/

//-----------------------------------------------------------------------------
//	Name:		LineLine
//	Object:		
//	01-08-30:	JLT - Created
//-----------------------------------------------------------------------------
bool INTERSECTION2D::LineLine(const POINT2D &	P0,
							  const POINT2D &	P1,
							  const POINT2D &	P2,
							  const POINT2D &	P3,
							  POINT2D &			P,
							  int &				nNbrInt,
							  float				T[2])
{
	float	D0X = P1[_X_] - P0[_X_];
	float	D0Y = P1[_Y_] - P0[_Y_];

	float	D1X = P3[_X_] - P2[_X_];
	float	D1Y = P3[_Y_] - P2[_Y_];

	float	DiffX = P2[_X_] - P0[_X_];
	float	DiffY = P2[_Y_] - P0[_Y_];

	float	D0Len2 = D0X * D0X + D0Y * D0Y;
	float	D1Len2 = D1X * D1X + D1Y * D1Y;
	float	DiffLen2 = DiffX * DiffX + DiffY * DiffY;

	float	Cross = D0Y * D1X - D0X * D1Y;
	float	Area0 = DiffY * D0X - DiffX * D0Y;
	float	Area1 = DiffY * D1X - DiffX * D1Y;

	if (Cross * Cross < 0.000001f * D0Len2 * D1Len2)
	{
		if (Area0 * Area0 < 0.000001f * DiffLen2 * D1Len2)
		{
			nNbrInt = 2;
			return	(false);
		}
		else
		{
			nNbrInt = 0;
			return	(false);
		}
	}
	else
	{
		float	InvCross = 1.f / Cross;
		float	t = Area1 * InvCross;
		float	s = Area0 * InvCross;

		T[0] = t;
		T[1] = s;

		P = (P0 * (1 - t)) + (P1 * t);
		nNbrInt = 1;

		return	(true);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SegmentSegment
//	Object:		
//	01-08-27:	JLT - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION2D::SegmentSegment(const POINT2D &	P0,
									   const POINT2D &	P1,
									   const POINT2D &	P2,
									   const POINT2D &	P3,
									   POINT2D &		PInt,
									   int &			nNbrInt)
{
	float	T[2];
	float	rEpsilon	= MATH::EPSILON();

	LineLine(P0, P1, P2, P3, PInt, nNbrInt, T);

	if	(nNbrInt == 0)
	{
		return	(false);
	}

	float	t = T[0];
	float	s = T[1];

	if	((t <= 0.f + rEpsilon) || (t >= 1.f - rEpsilon) || (s <= 0.f + rEpsilon) || (s >= 1.f - rEpsilon))
	{
		nNbrInt = 0;
		return	(false);
	}

	return	(true);
}

//-----------------------------------------------------------------------------
//	Name:		SegmentSegmentNoColinearity
//	Object:		
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
INTERSECTION2D::INTERSECTION	INTERSECTION2D::SegmentSegmentNoColinearity(SEGMENT2D &	S1,
																			SEGMENT2D &	S2,
																			POINT2D &	IntPoint)
{
	int	nNbrPoint;

	//	No colinearity possible, handle the standard segment intersection
	if	(SegmentSegment(S1.GetPoint1(), S1.GetPoint2(), S2.GetPoint1(), S2.GetPoint2(), IntPoint, nNbrPoint))
	{
		QDT_ASSERT(nNbrPoint != 2);		//	This case is handled with colinearity !
		return	(I_ONE_INTERSECTION);
	}
	else
	{
		return	(I_NO_INTERSECTION_NO_COLINEAR_SEGMENT);
	}
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersection
//	Object:		
//	05-02-08:	JPE - Created
//-----------------------------------------------------------------------------
INTERSECTION2D::INTERSECTION	INTERSECTION2D::FindIntersection(	const POINT2D &	Pt11,
																	const POINT2D &	Pt12,
																	const POINT2D &	Pt21,
																	const POINT2D &	Pt22,
																	POINT2D &		Intersection)
{
	POINT2D	rkD0(Pt12.GetX()-Pt11.GetX(), Pt12.GetY()-Pt11.GetY());
	POINT2D	rkD1(Pt22.GetX()-Pt21.GetX(), Pt22.GetY()-Pt21.GetY());
    float	fDet = rkD1.Cross(rkD0);
	POINT2D	rkDiff(Pt21.GetX() - Pt11.GetX(), Pt21.GetY() - Pt11.GetY());
    float	rfD0SqrLen = rkD0.GetSquareLength();

    if ( fDet*fDet > MATH::EPSILON()*rfD0SqrLen*rkD1.GetSquareLength() )
    {
        float fInvDet = 1.0f/fDet;
        float afT0 = rkD1.Cross(rkDiff)*fInvDet;
		Intersection.SetX( Pt11.GetX() + afT0 * rkD0.GetX() );
		Intersection.SetY( Pt11.GetY() + afT0 * rkD0.GetY() );

		return I_ONE_INTERSECTION;
    }

	// Should be completed in case you need info about full line intersect
	return I_NO_INTERSECTION_PARRALEL_LINE;
}


//-----------------------------------------------------------------------------
//	Name:		SegmentSegment
//	Object:		
//	01-09-02:	ONA - Created
//-----------------------------------------------------------------------------
INTERSECTION2D::INTERSECTION	INTERSECTION2D::SegmentSegment(SEGMENT2D &	S1,
															   SEGMENT2D &	S2,
															   SEGMENT2D &	IntSegment,
															   POINT2D &	IntPoint)
{
	float	DeltaX, DeltaY;
	float	v0, v1;

	DeltaX = S2.GetPoint2().GetX() - S1.GetPoint1().GetX();
	DeltaY = S2.GetPoint2().GetY() - S1.GetPoint1().GetY();

	if	( MATH::Abs(DeltaX) > MATH::Abs(DeltaY))
	{
		float	y;

		v0 = (S2.GetPoint1().GetX() - S1.GetPoint1().GetX()) / DeltaX;
		y = (v0 * DeltaY) + S1.GetPoint1().GetY();
		if	(MATH::Abs(y - S2.GetPoint1().GetX()) > MATH::EPSILON())
		{
			return	(SegmentSegmentNoColinearity(S1, S2, IntPoint));
		}

		v1 = (S2.GetPoint2().GetX() - S1.GetPoint1().GetX()) / DeltaX;
		y = (v1 * DeltaY) + S1.GetPoint1().GetY();
		if	(MATH::Abs(y - S2.GetPoint2().GetX()) > MATH::EPSILON())
		{
			return	(SegmentSegmentNoColinearity(S1, S2, IntPoint));
		}
	}
	else
	{
		float	x;

		v0 = (S2.GetPoint1().GetY() - S1.GetPoint1().GetY()) / DeltaY;
		x = (v0 * DeltaX) + S1.GetPoint1().GetX();
		if	(MATH::Abs(x - S2.GetPoint1().GetY()) > MATH::EPSILON())
		{
			return	(SegmentSegmentNoColinearity(S1, S2, IntPoint));
		}

		v1 = (S2.GetPoint2().GetY() - S1.GetPoint1().GetY()) / DeltaY;
		x = (v1 * DeltaX) + S1.GetPoint1().GetX();
		if	(MATH::Abs(x - S2.GetPoint2().GetY()) > MATH::EPSILON())
		{
			return	(SegmentSegmentNoColinearity(S1, S2, IntPoint));
		}
	}

	//	So colinearity is possible...
	float	rUMin, rUMax;
	float	rVMin, rVMax;
	bool	bInvert;

	rUMin = 0.0f;
	rUMax = 1.0f;
	if	(v0 > v1)
	{
		bInvert = true;
		rVMin = v1;
		rVMax = v0;
	}
	else
	{
		bInvert = false;
		rVMin = v0;
		rVMax = v1;
	}

	if	(rVMax < rUMin)
	{
		return	(I_NO_INTERSECTION_COLINEAR_SEGMENT);
	}
	else if	(rVMax == rUMin)
	{
		IntPoint.Set(S1.GetPoint1());
		return	(I_ONE_INTERSECTION);
	}
	if	(rVMin > rUMax)
	{
		return	(I_NO_INTERSECTION_COLINEAR_SEGMENT);
	}
	else if	(rVMin == rUMax)
	{
		IntPoint.Set(S1.GetPoint2());
		return	(I_ONE_INTERSECTION);
	}

	//	Okay segment intersection !
	//	Determine the three cases
	if	(rUMin < rVMin)
	{
		if	(bInvert)
		{
			IntSegment.SetPoint1(S2.GetPoint2());
		}
		else
		{
			IntSegment.SetPoint1(S2.GetPoint1());
		}
	}
	else
	{
		IntSegment.SetPoint1(S1.GetPoint1());
	}

	if	(rUMax < rVMax)
	{
		IntSegment.SetPoint1(S1.GetPoint2());
	}
	else
	{
		if	(bInvert)
		{
			IntSegment.SetPoint1(S2.GetPoint2());
		}
		else
		{
			IntSegment.SetPoint1(S2.GetPoint1());
		}
	}

	return	(I_SEGMENT_INTERSECTION);
}

//***************************************************************************************
//Clipping Sutherland-Hodgman
//
//01-05-22: MMO Created 
//
//JLT : can be optimized with a vertex pool
//
bool	INTERSECTION2D::ClipPolygon(const AABR2D &	bbox,
									const POINT2D *	InPoly,
									POINT2D *		OutPoly,
									int				InCount,
									int &			OutCount)
{
	static float T[2];
	register POINT2D *S, *P;
	register int j;

	POINT2D Pk;
	int PkCount;
	
	POINT2D *InTable = new POINT2D[3*InCount];
	POINT2D *OutTable = new POINT2D[3*InCount];
	QDT_MEMCOPY(InTable, InPoly, InCount*sizeof(POINT2D));

	int MaxOut = OutCount;

	static int counter; ++counter;
	
	int i;
	for (i = 0; i<4; ++i)
	{
		int iplus1 = (i+1)%4;

		POINT2D Pi = bbox[i];
		POINT2D Piplus1 = bbox[iplus1];
		POINT2D Middle = (Pi + Piplus1) * 0.5f;
		POINT2D Center(bbox.GetXCenter(),bbox.GetYCenter());
		POINT2D Normal = Center - Middle;

		OutCount = 0;
		S = &(InTable[InCount-1]);

		for (j=0; j<InCount; ++j)
		{
			P = &(InTable[j]);
			
			POINT2D MiddleS = (*S) - Middle;
			POINT2D MiddleP = (*P) - Middle;

			float psP = Normal[_X_]*MiddleP[_X_] + Normal[_Y_]*MiddleP[_Y_];
			float psS = Normal[_X_]*MiddleS[_X_] + Normal[_Y_]*MiddleS[_Y_];

			if (psP >= -0.001f)
			{
				if (psS >= -0.001f)
				{				
					if (OutCount >= MaxOut)
					{
						delete [] InTable;
						delete [] OutTable;
						return false;
					}
					
					OutTable[OutCount++] = *P;
				}
				else
				{
					if (OutCount >= MaxOut-1)
					{
						delete [] InTable;
						delete [] OutTable;
						return false;
					}
					
					if (LineLine(Pi, Piplus1, *S, *P, Pk, PkCount,T))
					{
						if (PkCount == 1)
							OutTable[OutCount++] = Pk;
						else
						{
							QDT_FAIL();
						}
					}
						
					OutTable[OutCount++] = *P;
				}
			}
			else
			{
				if (psS >= -0.001f)
				{
					if (OutCount >= MaxOut)
					{
						delete [] OutTable;
						delete [] InTable;
						return false;
					}

					LineLine(Pi, Piplus1, *S, *P, Pk, PkCount,T);
					if (PkCount != 0)
					{
						OutTable[OutCount++] = Pk;
					}
				}
					
			}
			
			S = P;
		}

		POINT2D *pTemp = InTable;
		InTable = OutTable;
		OutTable = pTemp;
		InCount = OutCount;
	}

	QDT_MEMCOPY(OutPoly,InTable,sizeof(POINT2D)*OutCount);

	delete [] InTable;
	delete [] OutTable;

	return (OutCount != 0);
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionCircleAABR2D
//	Object:		Arvo's algorithm
//				See http://www.gamasutra.com/features/19991018/Gomez_4.htm
//				for details	
//	02-03-11:	RMA - Created
//-----------------------------------------------------------------------------
/*bool	INTERSECTION2D::TestIntersectionCircleAABR2D(const CIRCLE &	Circle,
													  const AABR2D &	Box)
{
	float rS, rD = 0.0f;

	// Find the square of the distance
	// from the sphere to the box
	if (Circle.GetCenter().GetX() < Box.GetXMin())
	{
		rS = Circle.GetCenter().GetX() - Box.GetXMin();
		rD += rS * rS;
	}
	else if (Circle.GetCenter().GetX() > Box.GetXMax())
	{
		rS = Circle.GetCenter().GetX() - Box.GetXMax();
		rD += rS * rS;
	}

	if (Circle.GetCenter().GetY() < Box.GetYMin())
	{
		rS = Circle.GetCenter().GetY() - Box.GetYMin();
		rD += rS * rS;
	}
	else if (Circle.GetCenter().GetY() > Box.GetYMax())
	{
		rS = Circle.GetCenter().GetY() - Box.GetYMax();
		rD += rS * rS;
	}

	return (rD <= Circle.GetRadius() * Circle.GetRadius());
}*/

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
