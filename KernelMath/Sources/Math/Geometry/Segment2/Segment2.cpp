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
//	CLASS:	SEGMENT_2
//
//	03-03-12:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Segment2/Segment2)

//-----------------------------------------------------------------------------
//	Name:		SEGMENT_2 constructor
//	Object:		
//	03-03-12:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT_2::SEGMENT_2()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SEGMENT_2 constructor
//	Object:		
//	03-03-12:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT_2::SEGMENT_2(const VECTOR_2 &	Point0,
					 const VECTOR_2 &	Point1)
:
_Point0(Point0),
_Point1(Point1)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SEGMENT_2 constructor
//	Object:		
//	03-03-12:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT_2::SEGMENT_2(const SEGMENT_2 & C)
:
_Point0(C._Point0),
_Point1(C._Point1)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		SEGMENT_2 destructor
//	Object:		
//	03-03-12:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT_2::~SEGMENT_2()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-03-12:	RMA - Created
//-----------------------------------------------------------------------------
SEGMENT_2 &	SEGMENT_2::operator=(const SEGMENT_2 & C)
{
	_Point0 = C._Point0;
	_Point1 = C._Point1;

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersection
//	Object:		
//	03-03-18:	RMA - Created
//-----------------------------------------------------------------------------
bool SEGMENT_2::Find(const VECTOR_2& vPos, 
					 const VECTOR_2& vDir, 
					 VECTOR_2&		 vDiff, 
					 float&			 rDirSqrLen, 
					 int&			 nQuantity, 
					 float			 rT[2]) const

{
	// Intersection is a solution to P0+s*D0 = P1+t*D1.  Rewrite as
    // s*D0 - t*D1 = P1 - P0, a 2x2 system of equations.  If D0 = (x0,y0)
    // and D1 = (x1,y1) and P1 - P0 = (c0,c1), then the system is
    // x0*s - x1*t = c0 and y0*s - y1*t = c1.  The error tests are relative
    // to the size of the direction vectors, |Cross(D0,D1)| >= e*|D0|*|D1|
    // rather than absolute tests |Cross(D0,D1)| >= e.  The quantities
    // P1-P0, |D0|^2, and |D1|^2 are returned for use by calling functions.

    float rDet = vDir.GetX() * GetDirection().GetY() - vDir.GetY() * GetDirection().GetX();
    vDiff.Sub(vPos, GetOrigin());
    rDirSqrLen = GetDirection().SquareLength();

    const float rEpsilon = 1e-06f;
    if (rDet * rDet > rEpsilon * rDirSqrLen * vDir.SquareLength())
    {
        // Lines intersect in a single point.  Return both s and t values for
        // use by calling functions.
        float rInvDet = 1.0f / rDet;
        nQuantity = 1;
        rT[0] = (vDir.GetX() * vDiff.GetY() - vDir.GetY() * vDiff.GetX()) * rInvDet;
        rT[1] = (GetDirection().GetX() * vDiff.GetY() - GetDirection().GetY() * vDiff.GetX()) * rInvDet;
    }
    else
    {
        // lines are parallel
        rDet = GetDirection().GetX() * vDiff.GetY() - GetDirection().GetY() * vDiff.GetX();
        if (rDet * rDet > rEpsilon * rDirSqrLen * vDiff.SquareLength())
        {
            // lines are disjoint
            nQuantity = 0;
        }
        else
        {
            // lines are the same
            nQuantity = 2;
        }
    }

    return (nQuantity != 0);
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersection
//	Object:		
//	03-03-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	SEGMENT_2::FindIntersection(const SEGMENT_2 &S, int &nQuantity, float rT[2]) const
{
	VECTOR_2 vDiff;
    float	 rD0SqrLen;

    bool bIntersects = Find(S.GetOrigin(), S.GetDirection(), vDiff, rD0SqrLen, nQuantity, rT);

    if (bIntersects)
    {
        if (nQuantity == 1)
        {
            if (rT[0] < 0.0f || rT[0] > 1.0f || rT[1] < 0.0f || rT[1] > 1.0f)
            {
                // lines intersect, but segments do not
                nQuantity = 0;
            }
        }
        else
        {
            // segments are on the same line
            float rDotRS = GetDirection().Dot(S.GetDirection());
            float rDot0, rDot1;
            if (rDotRS > 0.0f)
            {
                rDot0 = vDiff.Dot(GetDirection());
                rDot1 = rDot0 + rDotRS;
            }
            else
            {
                rDot1 = vDiff.Dot(GetDirection());
                rDot0 = rDot1 + rDotRS;
            }

            // compute intersection of [t0,t1] and [0,1]
            if (rDot1 < 0.0f || rDot0 > rD0SqrLen)
            {
                nQuantity = 0;
            }
            else if (rDot1 > 0.0f)
            {
                if (rDot0 < rD0SqrLen)
                {
                    float rInvLen = 1.0f / rD0SqrLen;
                    nQuantity = 2;
                    rT[0] = ( rDot0 < 0.0f ? 0.0f : rDot0 * rInvLen );
                    rT[1] = ( rDot1 > rD0SqrLen ? 1.0f : rDot1 * rInvLen);
                }
                else  // fT0 == 1
                {
                    nQuantity = 1;
                    rT[0] = 1.0f;
                }
            }
            else  // fT1 == 0
            {
                nQuantity = 1;
                rT[0] = 0.0f;
            }
        }
    }

    return (nQuantity != 0);

}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
