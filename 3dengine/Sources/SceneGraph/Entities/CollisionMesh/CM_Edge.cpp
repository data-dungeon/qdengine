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
//	CLASS:	CM_EDGE
//
//	03-08-07:	JPE - Created
//*****************************************************************************

#include	"Root.h"
#include	INCL_3DENGINE(SceneGraph/Entities/CollisionMesh/CM_Edge)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point2D/Point2D)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

#define PARALLEL_PRECISION 0.1f


//=============================================================================
//	CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetIntersection
//	Object:		
//	04-03-25:	JPE - Created
//-----------------------------------------------------------------------------
CM_EDGE::EDGE_INTERSECTION_RC CM_EDGE::GetIntersection(	VECTOR &	v1,
														VECTOR &	v2,
														POINT2D	*	Vertices,
														POINT2D	&	Intersect)

{
//#ifdef USE_LINEAR
//	static int PUTAINCKIKICOMANDEICI = false;
	POINT2D	&v3			= Vertices[_VtxIdx[0]];
	POINT2D	&v4			= Vertices[_VtxIdx[1]];
	POINT2D	v34			= v4 - v3;
	POINT2D	v34Norm		= v4 - v3;

	v34Norm.Normalize();
/*
if(PUTAINCKIKICOMANDEICI)
{
	int		bParallel	= false;
	float	XDiff12		= v1.GetX() - v2.GetX();
	POINT2D	v12(v2.GetX() - v1.GetX(), v2.GetZ() - v1.GetZ());

	v12.Normalize();

	float	Cross1234	= v34Norm.Cross(v12);

	if (Cross1234 * Cross1234 <= 0.0001f)
	{
		bParallel = true;
	}
	else if(MATH::Abs(XDiff12) < 0.0001f)
	{
		if(MATH::Abs( v34.GetX() ) < 0.0001f)
		{
			bParallel = true;
		}
		else
		{
			float c = v34.GetY() / v34.GetX();
			float d = v3.GetY() - c * v3.GetX();

			Intersect.SetX( v1.GetX() );
			Intersect.SetY( c * Intersect.GetX() + d );
		}
	}
	else if(MATH::Abs(v34.GetX()) < 0.0001f)
	{
		float a = (v1.GetZ() - v2.GetZ()) / XDiff12;
		float b = v2.GetZ() - a * v2.GetX();

		Intersect.SetX( v3.GetX() );
		Intersect.SetY( a * Intersect.GetX() + b );
	}
	else
	{
		float a = (v1.GetZ() - v2.GetZ()) / XDiff12;
		float b = v2.GetZ() - a * v2.GetX();
		float c = v34.GetY() / v34.GetX();
		float d = v3.GetY() - c * v3.GetX();

		if( MATH::Abs(a-c) < 0.0001f)
		{
			bParallel = true;
		}
		else
		{
			Intersect.SetX( (d-b) / (a-c) );
			Intersect.SetY( a * Intersect.GetX() + b );
		}
	}

	if(!bParallel)
	{
		if(IsPointInside(Intersect, Vertices) == SIDE_INSIDE)
		{
			POINT2D	Pt1( v1.GetX(), v1.GetZ() );
			POINT2D Pt2( v2.GetX(), v2.GetZ() );
			if(IsPointInside(Intersect, Vertices, &Pt1, &Pt2) == SIDE_INSIDE)
				return EIRC_FULLINTERSECT;
		}

		return EIRC_NO_INTERSECTION;
	}
}
else
*/
{	
	POINT2D	v12(v2.GetX() - v1.GetX(), v2.GetZ() - v1.GetZ());
	const float	Epsilon = 0.0001f;

	float	fDet		= v34.Cross(v12);
    POINT2D Diff(v3.GetX() - v1.GetX(), v3.GetY() - v1.GetZ());
    float	rfD0SqrLen	= v12.GetSquareLength();

    if ( fDet*fDet > Epsilon*rfD0SqrLen*v34.GetSquareLength() )
    {
        // Lines intersect in a single point.  Return both s and t values for
        // use by calling functions.
        float fInvDet	= 1.0f/fDet;
        float afT0		= v34.Cross(Diff)*fInvDet;
        float afT1		= v12.Cross(Diff)*fInvDet;

		if ((afT0 < -0.001f) || (afT0 > 1.001f))
			return EIRC_NO_INTERSECTION;
		if ((afT1 < -0.001f) || (afT1 > 1.001f))
			return EIRC_NO_INTERSECTION;

		Intersect.SetXY(v3.GetX() + v34.GetX() * afT1,
						v3.GetY() + v34.GetY() * afT1);

		return EIRC_FULLINTERSECT;
    }
    else
    {
        // lines are parallel
        fDet = v12.Cross(Diff);
        float fRHS = Epsilon*rfD0SqrLen*Diff.GetSquareLength();
        if ( fDet*fDet > fRHS )
        {
            return EIRC_NO_INTERSECTION;
        }
    }
}

	POINT2D	v31(v1.GetX() - v3.GetX(), v1.GetZ() - v3.GetY());

	v31.Normalize();

	if (MATH::Abs(v34Norm.Cross(v31)) > 0.001f)
	{
		return EIRC_NO_INTERSECTION;
	}

	static const float	rEpsilonPara	= 0.41f;

	if (MATH::Abs(v34.GetX()) > MATH::Abs(v34.GetY()))
	{
		if (v1.GetX() < v2.GetX())
		{
			if (v3.GetX() < v4.GetX())
			{
				if (v1.GetX() < v3.GetX())
				{
					if (v2.GetX() > v3.GetX() - rEpsilonPara)	
					{
						Intersect.Set(v3);
						return EIRC_PARALLEL;
					}
				}
				else if (v4.GetX() > v1.GetX() - rEpsilonPara)
				{
					Intersect.SetXY(v1.GetX(), v1.GetZ());
					return EIRC_PARALLEL;
				}
			}
			else
			{
				if (v1.GetX() < v4.GetX())
				{
					if (v2.GetX() > v4.GetX() - rEpsilonPara)
					{
						Intersect.Set(v4);
						return EIRC_PARALLEL;
					}
				}
				else if (v3.GetX() > v1.GetX() - rEpsilonPara)
				{
					Intersect.SetXY(v1.GetX(), v1.GetZ());
					return EIRC_PARALLEL;
				}
			}
		}
		else
		{
			if (v3.GetX() < v4.GetX())
			{
				if (v2.GetX() < v3.GetX())
				{
					if (v1.GetX() > v3.GetX() - rEpsilonPara)		
						Intersect.SetXY(v1.GetX(), v1.GetZ());
						return EIRC_PARALLEL;
				}
				else if (v4.GetX() > v2.GetX() - rEpsilonPara)
				{
					Intersect.Set(v4);
					return EIRC_PARALLEL;
				}
			}
			else
			{
				if (v2.GetX() < v4.GetX())
				{
					if (v1.GetX() > v4.GetX() - rEpsilonPara)
					{
						Intersect.SetXY(v1.GetX(), v1.GetZ());
						return EIRC_PARALLEL;
					}
				}
				else if (v3.GetX() > v2.GetX() - rEpsilonPara)
				{
					Intersect.Set(v3);
					return EIRC_PARALLEL;
				}
			}
		}
	}
	else
	{
		if (v1.GetZ() < v2.GetZ())
		{
			if (v3.GetY() < v4.GetY())
			{
				if (v1.GetZ() < v3.GetY())
				{
					if (v2.GetZ() > v3.GetY() - rEpsilonPara)	
					{
						Intersect.Set(v3);
						return EIRC_PARALLEL;
					}
				}
				else if (v4.GetY() > v1.GetZ() - rEpsilonPara)
				{
					Intersect.SetXY(v1.GetX(), v1.GetZ());
					return EIRC_PARALLEL;
				}
			}
			else
			{
				if (v1.GetZ() < v4.GetY())
				{
					if (v2.GetZ() > v4.GetY() - rEpsilonPara)
					{
						Intersect.Set(v4);
						return EIRC_PARALLEL;
					}
				}
				else if (v3.GetY() > v1.GetZ() - rEpsilonPara)
				{
					Intersect.SetXY(v1.GetX(), v1.GetZ());
					return EIRC_PARALLEL;
				}
			}
		}
		else
		{
			if (v3.GetY() < v4.GetY())
			{
				if (v2.GetZ() < v3.GetY())
				{
					if (v1.GetZ() > v3.GetY() - rEpsilonPara)		
						Intersect.SetXY(v1.GetX(), v1.GetZ());
						return EIRC_PARALLEL;
				}
				else if (v4.GetY() > v2.GetZ() - rEpsilonPara)
				{
					Intersect.Set(v4);
					return EIRC_PARALLEL;
				}
			}
			else
			{
				if (v2.GetZ() < v4.GetY())
				{
					if (v1.GetZ() > v4.GetY() - rEpsilonPara)
					{
						Intersect.SetXY(v1.GetX(), v1.GetZ());
						return EIRC_PARALLEL;
					}
				}
				else if (v3.GetY() > v2.GetZ() - rEpsilonPara)
				{
					Intersect.Set(v3);
					return EIRC_PARALLEL;
				}
			}
		}
	}

	return EIRC_NO_INTERSECTION;
}

//-----------------------------------------------------------------------------
//	Name:		IsPointInside
//	Object:		
//	04-03-29:	JPE - Created
//-----------------------------------------------------------------------------
int	CM_EDGE::IsPointInside(POINT2D &	pt,
						   POINT2D *	Vertices,
						   POINT2D *	pV1,
						   POINT2D *	pV2 )
{
	//		Epsilon :
	//      |--|
	//		x--x-----------------------------x--x
	static const	float	Epsilon = 0.1f;
	if( !pV1)
	{
		pV1 = &Vertices[_VtxIdx[0]];
		pV2 = &Vertices[_VtxIdx[1]];
	}
	float	DiffX = pV1->GetX() - pV2->GetX();
	float	DiffY = pV1->GetY() - pV2->GetY();

	if( MATH::Abs(DiffX) > MATH::Abs(DiffY) )
	{
		if(DiffX > 0)
		{
			if( pt.GetX() >= (pV1->GetX()+Epsilon) )	return SIDE_P1;
			if( pt.GetX() <= (pV2->GetX()-Epsilon) )	return SIDE_P2;
		}
		else
		{
			if( pt.GetX() <= (pV1->GetX()-Epsilon) )	return SIDE_P1;
			if( pt.GetX() >= (pV2->GetX()+Epsilon) )	return SIDE_P2;
		}
	}
	else
	{
		if(DiffY > 0)
		{
			if( pt.GetY() >= (pV1->GetY()+Epsilon) )	return SIDE_P1;
			if( pt.GetY() <= (pV2->GetY()-Epsilon) )	return SIDE_P2;
		}
		else
		{
			if( pt.GetY() <= (pV1->GetY()-Epsilon) )	return SIDE_P1;
			if( pt.GetY() >= (pV2->GetY()+Epsilon) )	return SIDE_P2;
		}
	}

	return SIDE_INSIDE;
}

