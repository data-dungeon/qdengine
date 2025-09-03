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
//	CLASS:	RAY_2D
//
//	03-03-14:	RMA - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math\Geometry\Ray2D\Ray2D)

#include	INCL_KMATH(Math\Math)
#include	INCL_KMATH(Math\Geometry\Segment2\Segment2)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		RAY_2D constructor
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
RAY_2D::RAY_2D()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAY_2D constructor
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
RAY_2D::RAY_2D(const VECTOR_2 &	vOrigin,
			   const VECTOR_2 &	vDirection)
:
_vOrigin	(vOrigin),
_vDirection	(vDirection)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAY_2D constructor
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
RAY_2D::RAY_2D(const RAY_2D & C)
:
_vOrigin	(C._vOrigin),
_vDirection	(C._vDirection)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		RAY_2D destructor
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
RAY_2D::~RAY_2D()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
RAY_2D &	RAY_2D::operator=(const RAY_2D & C)
{
	_vOrigin	= C._vOrigin;
	_vDirection = C._vDirection;

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersection
//	Object:		
//	03-03-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_2D::FindIntersection(const VECTOR_2	&	vCenter,
								 float				rRadius,
								 VECTOR_2 &			vIntersection) const
{
	float	rT[2];
	int		nQ;

    bool bIntersects = Find(vCenter, rRadius, nQ, rT);
	
	if (bIntersects)
	{
		// reduce root count if line-circle intersections are not on ray
        if (nQ == 1)
        {
            if (rT[0] < 0.0f)
			{
                nQ = 0;
			}
        }
        else
        {
            if (rT[1] < 0.0f)
            {
                nQ = 0;
            }
            else if (rT[0] < 0.0f)
            {
                nQ = 1;
                rT[0] = rT[1];
            }
        }

		if (nQ == 1)
		{
			vIntersection = GetDirection();
			vIntersection.Mul(rT[0]);
			vIntersection.Add(GetOrigin());
		}
		else if (nQ == 2)
		{
			if (rT[0] < rT[1])
			{
				vIntersection = GetDirection();
				vIntersection.Mul(rT[0]);
				vIntersection.Add(GetOrigin());
			}
			else
			{
				vIntersection = GetDirection();
				vIntersection.Mul(rT[1]);
				vIntersection.Add(GetOrigin());
			}
		}
	}

	return (nQ != 0);
}

//-----------------------------------------------------------------------------
//	Name:		FindIntersection
//	Object:		
//	03-03-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_2D::FindIntersection(const VECTOR_2&	vCenter,
								 float				rRadius,
								 int &				nQuantity,
								 VECTOR_2			vT[2]) const
{
	float rT[2];
    bool bIntersects = Find(vCenter, rRadius, nQuantity, rT);

    if (bIntersects)
    {
        // reduce root count if line-circle intersections are not on ray
        if (nQuantity == 1)
        {
            if (rT[0] < 0.0f)
			{
                nQuantity = 0;
			}
        }
        else
        {
            if (rT[1] < 0.0f)
            {
                nQuantity = 0;
            }
            else if (rT[0] < 0.0f )
            {
                nQuantity = 1;
                rT[0] = rT[1];
            }
        }

        // construct ray-circle points of intersection
        for (int i = 0; i < nQuantity; i++)
		{
            vT[i] = GetDirection();
			vT[i].Mul(rT[i]);
			vT[i].Add(GetOrigin());
		}
    }

    return (nQuantity != 0);

}

//-----------------------------------------------------------------------------
//	Name:		FindIntersection
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_2D::FindIntersection(const SEGMENT_2 &Segment, 
								 int			 &nQuantity, 
								 float			  rT[2]) const
{
	VECTOR_2	vDiff;
    float		rD0SqrLen;
    bool		bIntersects;
	
	QDT_ASSERT(GetDirection().SquareLength() > 0.0f);

	bIntersects = Find(Segment.GetOrigin(), Segment.GetDirection(), vDiff, rD0SqrLen, nQuantity, rT);

    if (bIntersects)
    {
        if (nQuantity == 1)
        {
			// Epsilon = 0.2
			// It can seems to be very diry but it's necessary for collisions

            if (rT[0] < 0.0f || rT[1] < 0.0f || rT[1] > 1.0f)
            {
                // lines intersect, but ray and segment do not
                nQuantity = 0;
            }
        }
        else
        {
            // ray and segment are on the same line
            float rDotRS = GetDirection().Dot(Segment.GetDirection());
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

            // compute intersection of [t0,t1] and [0,+infinity]
            if (rDot0 >= 0.0f)
            {
                // complete overlap
                float rInvLen = 1.0f / rD0SqrLen;
                rT[0] = rDot0 * rInvLen;
                rT[1] = rDot1 * rInvLen;
            }
            else if (rDot1 > 0.0f)
            {
                // partial overlap
                rT[0] = 0.0f;
                rT[1] = rDot1 / rD0SqrLen;
            }
            else if (rDot1 < 0.0f)
            {
                // no overlap
                nQuantity = 0;
            }
            else
            {
                // overlap at a single end point
                nQuantity = 1;
                rT[0] = 0.0f;
            }
        }
    }

    return (nQuantity != 0);
}

//-----------------------------------------------------------------------------
//	Name:		Find
//	Object:		
//	03-03-18:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_2D::Find(const VECTOR_2 &	vCenter,
					 float				rRadius,
					 int &				nQuantity,
					 float				rT[2]) const
{
	// Intersection of a the line P+t*D and the circle |X-C| = R.  The t
    // value is a root to the quadratic equation:
    //   0 = |t*D+P-C|^2 - R^2
    //     = |D|^2*t^2 + 2*D.Dot(P-C)*t + |P-C|^2-R^2
    //     = a2*t^2 + 2*a1*t + a0
    // If two roots are returned, the order is T[0] < T[1].  Hopefully the
    // application will be kind and provide line directions D that are not so
    // small that a2 is nearly zero and potentially creates numerical
    // problems.
 
    VECTOR_2 vDiff;
	vDiff.Sub(GetOrigin(), vCenter);

    float rA0 = vDiff.SquareLength() - (rRadius * rRadius);
    float rA1 = GetDirection().Dot(vDiff);
    float rA2 = GetDirection().SquareLength();
    float rDiscr = (rA1 * rA1) - (rA0 * rA2);
    if (rDiscr > 0.0f)
    {
        nQuantity = 2;
        float rInvA2 = 1.0f / rA2;
        rDiscr = MATH::Sqrt(rDiscr);
        rT[0] = (-rA1 - rDiscr) * rInvA2;
        rT[1] = (-rA1 + rDiscr) * rInvA2;
    }
    else if (rDiscr < 0.0f )
    {
        nQuantity = 0;
    }
    else  // rDiscr == 0
    {
        nQuantity = 1;
        rT[0] = -rA1 / rA2;
    }

    return (nQuantity != 0);
}

//-----------------------------------------------------------------------------
//	Name:		Find
//	Object:		
//	03-03-14:	RMA - Created
//-----------------------------------------------------------------------------
bool	RAY_2D::Find(const VECTOR_2& vPos, 
					 const VECTOR_2& vDir, 
					 VECTOR_2&		 vDiff,
					 float&			 rRayDirSqrLen, 
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
    rRayDirSqrLen = GetDirection().SquareLength();

    const float rEpsilon = 1e-06f;
    if (rDet * rDet > rEpsilon * rRayDirSqrLen * vDir.SquareLength())
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
        if (rDet * rDet > rEpsilon * rRayDirSqrLen * vDiff.SquareLength())
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

//=============================================================================
//	CODE ENDS HERE
//=============================================================================
