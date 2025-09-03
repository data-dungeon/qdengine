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
// CLASS: INTERSECTION_FRUSTRUM
//
//
//	01-06-19:	GGO - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include "Root.h"
#include INCL_KMATH(Math/Geometry/Intersection/IntersectionFrustrum)

//#include INCL_KMATH(Math/Geometry/Matrix/Matrix)
//#include INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)
#include INCL_KMATH(Math/Math)
#include INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)

//-----------------------------------------------------------------------------
//	Name: INTERSECTION::TestIntersectionFrustrumSphere
//	Object: 
//	01-06-08:	GGO - Created
//-----------------------------------------------------------------------------

bool	INTERSECTION_FRUSTRUM::TestIntersectionFrustrumSphere(const FRUSTRUM &	Frustrum,
															  const SPHERE &	Sphere,
															  int				nSpeed)
{
	QDT_ASSERT( Frustrum.GetFrontDirection().IsNormalized() == true );
	QDT_ASSERT( Frustrum.GetLeftDirection().IsNormalized() == true );
	QDT_ASSERT( Frustrum.GetTopDirection().IsNormalized() == true );
	QDT_ASSERT( ( nSpeed == 0 ) || ( nSpeed == 1 ) );


	VECTOR vDiff = Sphere.GetCenter() - Frustrum.GetOrigin();
	VECTOR vTemp;
	float rX_TransformedCenter; // Coordinate of the sphere center
	float rY_TransformedCenter; // in the frustrum coordinate
	float rZ_TransformedCenter; // system.
	float rSqrRadius;
	float rSqrNear;
	float rSqrDistance;
	float rRatio;
	float rA, rB, rSqrDist1, rSqrDist2;
	float rDotProduct;

	float rRadius = Sphere.GetRadius();

	float rFar  = Frustrum.GetFar();
	float rNear = Frustrum.GetNear();
	float rLeft = Frustrum.GetLeft();
	float rTop  = Frustrum.GetTop();
	float rLeftByRatio; // = left * ratio
	float rTopByRatio;  // = top  * ratio




	// Test along the main direction of the frustrum
	rX_TransformedCenter = vDiff.Dot( Frustrum.GetFrontDirection() );

	if ( ( rX_TransformedCenter >= rFar  + rRadius ) ||
		 ( rX_TransformedCenter <= rNear - rRadius ) )
	{
		return ( false );
	}


	// Optis
	rSqrRadius = rRadius * rRadius;
	rSqrNear   = rNear   * rNear;



	// Test along the left direction of the frustrum.
	rY_TransformedCenter = vDiff.Dot( Frustrum.GetLeftDirection() );
	rY_TransformedCenter = MATH::Abs( rY_TransformedCenter );
	rDotProduct			 = rY_TransformedCenter * rNear - rX_TransformedCenter * rLeft;

	if ( ( rDotProduct > 0.0f ) &&
		 ( rDotProduct * rDotProduct >= rSqrRadius * ( rSqrNear + rLeft * rLeft ) ) )
	{
		return ( false );
	}


	// Test along the top direction of the frustrum.
	rZ_TransformedCenter = vDiff.Dot( Frustrum.GetTopDirection() );
	rZ_TransformedCenter = MATH::Abs( rZ_TransformedCenter );
	rDotProduct          = rZ_TransformedCenter * rNear - rX_TransformedCenter * rTop;

	if ( ( rDotProduct > 0.0f ) &&
		 ( rDotProduct * rDotProduct >= rSqrRadius * ( rSqrNear + rTop * rTop ) ) )
	{
		return ( false );
	}


	if ( nSpeed == 1 )
	{
		return ( true );
	}




	// But we need to perform more accurate tests to know if the sphere really intersects
	// or not the frustrum.

	// Optis.
	rRatio =	   Frustrum.GetFar() / Frustrum.GetNear();
	rLeftByRatio = rLeft * rRatio;
	rTopByRatio  = rTop  * rRatio;


	// Distance between sphere center and far top edge.
	bool bZ_LargerTopByRatio  = rZ_TransformedCenter > rTopByRatio;
	bool bY_LargerLeftByRatio = rY_TransformedCenter > rLeftByRatio;

	if ( ( bZ_LargerTopByRatio  == true  ) &&
		 ( bY_LargerLeftByRatio == false ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rFar ) +
			          rTop  * ( rZ_TransformedCenter - rTopByRatio );

		if ( rDotProduct >= 0.0f  )
		{
			rSqrDistance = MATH::Square( rZ_TransformedCenter - rTopByRatio ) +
						   MATH::Square( rX_TransformedCenter - rFar );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and near top edge.
	bool bZ_LargerTop  = rZ_TransformedCenter > rTop;
	bool bY_LargerLeft = rY_TransformedCenter > rLeft;

	if ( ( bZ_LargerTop  == true  ) &&
		 ( bY_LargerLeft == false ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rNear ) +
			          rTop  * ( rZ_TransformedCenter - rTop );

		if ( rDotProduct <= 0.0f )
		{
			rSqrDistance = MATH::Square( rZ_TransformedCenter - rTop ) +
						   MATH::Square( rX_TransformedCenter - rNear );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and far left edge.
	if ( ( bZ_LargerTopByRatio  == false ) &&
		 ( bY_LargerLeftByRatio == true  ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rFar ) +
			          rLeft * ( rY_TransformedCenter - rLeftByRatio );

		if ( rDotProduct >= 0.0f )
		{
			rSqrDistance = MATH::Square( rY_TransformedCenter - rLeftByRatio ) +
						   MATH::Square( rX_TransformedCenter - rFar );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and near left edge.
	if ( ( bZ_LargerTop  == false ) &&
		 ( bY_LargerLeft == true  ) ) 
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rNear ) + 
			          rLeft * ( rY_TransformedCenter - rLeft );

		if ( rDotProduct <= 0.0f )
		{
			rSqrDistance = MATH::Square( rY_TransformedCenter - rLeft ) +
						   MATH::Square( rX_TransformedCenter - rNear );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and top left far vertex.
	if ( ( bZ_LargerTopByRatio  == true ) &&
		 ( bY_LargerLeftByRatio == true ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rFar ) +
			          rLeft * ( rY_TransformedCenter - rLeftByRatio ) +
				      rTop  * ( rZ_TransformedCenter - rTopByRatio );

		if ( rDotProduct >= 0.0f )
		{
			rSqrDistance = MATH::Square( rX_TransformedCenter - rFar ) + 
						   MATH::Square( rY_TransformedCenter - rLeftByRatio ) +
						   MATH::Square( rZ_TransformedCenter - rTopByRatio );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and top left near vertex.
	if ( ( bZ_LargerTop  == true ) &&
		 ( bY_LargerLeft == true ) )
	{
		rDotProduct = rNear * ( rX_TransformedCenter - rNear ) +
					  rLeft * ( rY_TransformedCenter - rLeft ) +
					  rTop  * ( rZ_TransformedCenter - rTop );

		if ( rDotProduct <= 0.0f )
		{
			rSqrDistance = MATH::Square( rX_TransformedCenter - rNear ) + 
						   MATH::Square( rY_TransformedCenter - rLeft ) +
						   MATH::Square( rZ_TransformedCenter - rTop );

			if ( rSqrDistance > rSqrRadius )
			{
				return ( false );
			}
		}
	}


	// Distance between sphere center and top left edge (the worst case...).
	if ( ( rNear * rY_TransformedCenter - rLeft * rX_TransformedCenter >= 0.0f ) &&
		 ( rNear * rZ_TransformedCenter - rTop  * rX_TransformedCenter >= 0.0f ) )
	{
		rA        = rY_TransformedCenter * rNear / rLeft - rX_TransformedCenter;
		rB        = rY_TransformedCenter - rX_TransformedCenter * rLeft / rNear;
		rSqrDist1 = MATH::Square( rA * rB ) / ( rA * rA + rB * rB ); // Heigth h of a rectangular triangle whose sides are a and b: h = ab/(sqrt(aa+bb))

		rA        = rZ_TransformedCenter * rNear / rTop - rX_TransformedCenter;
		rB        = rZ_TransformedCenter - rX_TransformedCenter * rTop / rNear;
		rSqrDist2 = MATH::Square( rA * rB ) / ( rA * rA + rB * rB ); // Heigth h of a rectangular triangle whose sides are a and b: h = ab/(sqrt(aa+bb))

		rSqrDistance = rSqrDist1 + rSqrDist2;


		if ( rSqrDistance > rSqrRadius )
		{
			return ( false );
		}
	}


	QDT_ASSERT( nSpeed == 0 );

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: INTERSECTION::TestIntersectionFrustrumAABB
//	Object: 
//	01-06-14:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION_FRUSTRUM::TestIntersectionFrustrumAABB(const FRUSTRUM &	Frustrum,
															const AABB &		aabb,
															int					nSpeed)
{
	QDT_ASSERT( ( nSpeed == 0 ) || ( nSpeed == 1 ) || ( nSpeed == 2 ) );

	float rRatio = Frustrum.GetFar() / Frustrum.GetNear();
	float rProjNear, rProjFar;

	// We test if the normal of the AABB (ie. the canonic axis) are separating axis.

	// Projection along the X-axis.
	rProjNear = Frustrum.GetOrigin().GetX() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetX() - 
				Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetX() ) -
				Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetX() );
	if ( rProjNear >= aabb.GetXMax() )
	{
		rProjFar =	Frustrum.GetOrigin().GetX() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetX() -
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetX() ) * rRatio -
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetX() )  * rRatio;

		if ( rProjFar >= aabb.GetXMax() )
		{
			return ( false );
		}
	}

	rProjNear =	Frustrum.GetOrigin().GetX() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetX() + 
				Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetX() ) +
				Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetX() );

	if ( rProjNear <= aabb.GetXMin() )
	{
		rProjFar =	Frustrum.GetOrigin().GetX() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetX() +
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetX() ) * rRatio +
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetX() )  * rRatio;

		if ( rProjFar <= aabb.GetXMin() )
		{
			return ( false );
		}
	}


	// Projection along the Y-axis.
	rProjNear = Frustrum.GetOrigin().GetY() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetY() - 
				Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetY() ) -
				Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetY() );

	if ( rProjNear >= aabb.GetYMax() )
	{
		rProjFar =	Frustrum.GetOrigin().GetY() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetY() -
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetY() ) * rRatio -
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetY() )  * rRatio;

		if ( rProjFar >= aabb.GetYMax() )
		{
			return ( false );
		}
	}

	rProjNear = Frustrum.GetOrigin().GetY() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetY() + 
				Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetY() ) +
				Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetY() );

	if ( rProjNear <= aabb.GetYMin() )
	{
		rProjFar =	Frustrum.GetOrigin().GetY() +
					Frustrum.GetFar()  * Frustrum.GetFrontDirection().GetY() +
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetY() ) * rRatio +
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetY() )  * rRatio;

		if ( rProjFar <= aabb.GetYMin() )
		{
			return ( false );
		}
	}


	// Projection along the Z-axis.
	rProjNear = Frustrum.GetOrigin().GetZ() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetZ() - 
				Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetZ() ) -
				Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetZ() );

	if ( rProjNear >= aabb.GetZMax() )
	{
		rProjFar =	Frustrum.GetOrigin().GetZ() +
					Frustrum.GetFar() * Frustrum.GetFrontDirection().GetZ() -
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetZ() ) * rRatio -
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetZ() )  * rRatio;

		if ( rProjFar >= aabb.GetZMax() )
		{
			return ( false );
		}
	}

	rProjNear = Frustrum.GetOrigin().GetZ() + 
				Frustrum.GetNear() * Frustrum.GetFrontDirection().GetZ() + 
				Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetZ() ) +
				Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetZ() );

	if ( rProjNear <= aabb.GetZMin() )
	{
		rProjFar =	Frustrum.GetOrigin().GetZ() +
					Frustrum.GetFar() * Frustrum.GetFrontDirection().GetZ() +
					Frustrum.GetLeft() * MATH::Abs( Frustrum.GetLeftDirection().GetZ() ) * rRatio +
					Frustrum.GetTop()  * MATH::Abs( Frustrum.GetTopDirection().GetZ() )  * rRatio;

		if ( rProjFar <= aabb.GetZMin() )
		{
			return ( false );
		}
	}


	if ( nSpeed == 2 )
	{
		return ( true );
	}




	// We test if the normal of the frustrum are separating axis.
	float rProjAABB;
	float rDotProduct;

	float rAABB_DistanceX = 0.5f * ( aabb.GetXMax() - aabb.GetXMin() );
	float rAABB_DistanceY = 0.5f * ( aabb.GetYMax() - aabb.GetYMin() );
	float rAABB_DistanceZ = 0.5f * ( aabb.GetZMax() - aabb.GetZMin() );

	VECTOR CenterAABB( 0.5f * ( aabb.GetXMin() + aabb.GetXMax() ),
			 		    0.5f * ( aabb.GetYMin() + aabb.GetYMax() ),
					    0.5f * ( aabb.GetZMin() + aabb.GetZMax() ) );

	VECTOR NewFrustrumOrigin = Frustrum.GetOrigin() - CenterAABB;


	// Near and far
	rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetFrontDirection().GetX() ) +
				rAABB_DistanceY * MATH::Abs( Frustrum.GetFrontDirection().GetY() ) +
				rAABB_DistanceZ * MATH::Abs( Frustrum.GetFrontDirection().GetZ() );
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetFrontDirection() );

	if ( rDotProduct <= 0.0f )
	{
		if ( Frustrum.GetFar() + rProjAABB + rDotProduct <= 0.0f ) // Remember here rDotProduct is negative.
		{
			return ( false );
		}
	}
	else
	{
		if ( rDotProduct + Frustrum.GetNear() >= rProjAABB )
		{
			return ( false );
		}
	}


	// Left
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalLeft() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalLeft().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalLeft().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalLeft().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

	// Right
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalRight() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalRight().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalRight().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalRight().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

	// Top
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalTop() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalTop().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalTop().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalTop().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}

	// Bottom
	rDotProduct = NewFrustrumOrigin.Dot( Frustrum.GetNormalBottom() );
	if ( rDotProduct < 0.0f )
	{
		rProjAABB = rAABB_DistanceX * MATH::Abs( Frustrum.GetNormalBottom().GetX() ) +
					rAABB_DistanceY * MATH::Abs( Frustrum.GetNormalBottom().GetY() ) +
					rAABB_DistanceZ * MATH::Abs( Frustrum.GetNormalBottom().GetZ() );

		if ( MATH::Abs( rDotProduct ) > rProjAABB )
		{
			return ( false );
		}
	}


	if ( nSpeed == 1 )
	{
		return ( true );
	}




	// Finally we test if the cross products are separating axis.
	int   i;
	float rProjLeftTop;

	VECTOR NewFrustrumFar  = NewFrustrumOrigin + Frustrum.GetFar()  * Frustrum.GetFrontDirection();
	VECTOR NewFrustrumNear = NewFrustrumOrigin + Frustrum.GetNear() * Frustrum.GetFrontDirection();

	
	// Near/far edges.
	VECTOR vFrustrumEdge;
	for (i = 0; i < 6; i++)
	{
		switch(i)
		{
		case 0:
			vFrustrumEdge = Frustrum.GetLeftDirection();
			break;
		case 1:
			vFrustrumEdge = Frustrum.GetTopDirection();
			break;
		case 2:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() +
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() +
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		case 3:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() +
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() -
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		case 4:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() -
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() +
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		case 5:
			vFrustrumEdge = Frustrum.GetNear() * Frustrum.GetFrontDirection() -
							Frustrum.GetLeft() * Frustrum.GetLeftDirection() -
							Frustrum.GetTop()  * Frustrum.GetTopDirection();
			break;
		default:
			QDT_FAIL();
		}


		/* Cross vector: 0.0f
						 vFrustrumEdge.GetZ()
						-vFrustrumEdge.GetY() */

		rProjAABB    = rAABB_DistanceY * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   rAABB_DistanceZ * MATH::Abs( vFrustrumEdge.GetY() );

		rProjNear    = NewFrustrumNear.GetY() * vFrustrumEdge.GetZ() -
					   NewFrustrumNear.GetZ() * vFrustrumEdge.GetY();

		rProjFar     = NewFrustrumFar.GetY() * vFrustrumEdge.GetZ() -
					   NewFrustrumFar.GetZ() * vFrustrumEdge.GetY();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{	
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( Frustrum.GetLeftDirection().GetY() * vFrustrumEdge.GetZ() -
						     		  Frustrum.GetLeftDirection().GetZ() * vFrustrumEdge.GetY() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( Frustrum.GetTopDirection().GetY()  * vFrustrumEdge.GetZ() -
					     			  Frustrum.GetTopDirection().GetZ()  * vFrustrumEdge.GetY() );

			if ( ( rProjAABB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
			     ( rProjAABB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}


		/* Cross vector: vFrustrumEdge.GetZ()
						 0.0f
						-vFrustrumEdge.GetX() */

		rProjAABB    = rAABB_DistanceX * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   rAABB_DistanceZ * MATH::Abs( vFrustrumEdge.GetX() );

		rProjNear    = NewFrustrumNear.GetX() * vFrustrumEdge.GetZ() -
					   NewFrustrumNear.GetZ() * vFrustrumEdge.GetX();

		rProjFar     = NewFrustrumFar.GetX() * vFrustrumEdge.GetZ() -
					   NewFrustrumFar.GetZ() * vFrustrumEdge.GetX();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{			
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( Frustrum.GetLeftDirection().GetX() * vFrustrumEdge.GetZ() -
					     			  Frustrum.GetLeftDirection().GetZ() * vFrustrumEdge.GetX() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( Frustrum.GetTopDirection().GetX()  * vFrustrumEdge.GetZ() -
					     			  Frustrum.GetTopDirection().GetZ()  * vFrustrumEdge.GetX() );

			if ( ( rProjAABB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjAABB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}


		/* Cross vector: vFrustrumEdge.GetY()
						-vFrustrumEdge.GetX()
						 0.0f */

		rProjAABB    = rAABB_DistanceX * MATH::Abs( vFrustrumEdge.GetY() ) +
					   rAABB_DistanceY * MATH::Abs( vFrustrumEdge.GetX() );

		rProjNear    = NewFrustrumNear.GetX() * vFrustrumEdge.GetY() -
					   NewFrustrumNear.GetY() * vFrustrumEdge.GetX();

		rProjFar     = NewFrustrumFar.GetX() * vFrustrumEdge.GetY() -
					   NewFrustrumFar.GetY() * vFrustrumEdge.GetX();

		if ( rProjNear * rProjFar > 0.0f )   // 'rProjNear' and 'rProjFar' have the same sign.
		{
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( Frustrum.GetLeftDirection().GetX() * vFrustrumEdge.GetY() -
					     			  Frustrum.GetLeftDirection().GetY() * vFrustrumEdge.GetX() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( Frustrum.GetTopDirection().GetX()  * vFrustrumEdge.GetY() -
					     			  Frustrum.GetTopDirection().GetY()  * vFrustrumEdge.GetX() );

			if ( ( rProjAABB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjAABB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}
	}



	QDT_ASSERT( nSpeed == 0 );

	return ( true );
}



//-----------------------------------------------------------------------------
//	Name:		TestIntersectionFrustrumOBB
//	Object:		
//	01-07-16:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION_FRUSTRUM::TestIntersectionFrustrumOBB(const FRUSTRUM &	Frustrum,
														   const OBB &		obb,
														   int				nSpeed)
{
	QDT_ASSERT( ( nSpeed == 0 ) || ( nSpeed == 1 ) || ( nSpeed == 2 ) );


	float	rRatio = Frustrum.GetFar() / Frustrum.GetNear();
	float	rProjNear, rProjFar, rProjLeftTop;
	VECTOR	vDiff = Frustrum.GetOrigin() - obb.GetCenter();


	// Test if the 1st axis of the OBB is a separating axis.
	float rNewFrustrumOriginX   = vDiff.Dot( obb.GetDirection1() );
	float rNewFrustrumFrontDirX = Frustrum.GetFrontDirection().Dot( obb.GetDirection1() );
	float rNewFrustrumLeftDirX  = Frustrum.GetLeftDirection().Dot(  obb.GetDirection1() );
	float rNewFrustrumTopDirX   = Frustrum.GetTopDirection().Dot(   obb.GetDirection1() );

	rProjNear = rNewFrustrumOriginX + 
		        Frustrum.GetNear() * rNewFrustrumFrontDirX;

	rProjFar  = rNewFrustrumOriginX + 
			    Frustrum.GetFar()  * rNewFrustrumFrontDirX;

	if ( rProjNear * rProjFar > 0.0f )
	{
		rProjLeftTop = Frustrum.GetLeft() * MATH::Abs( rNewFrustrumLeftDirX ) +
					   Frustrum.GetTop()  * MATH::Abs( rNewFrustrumTopDirX );

		if ( ( obb.GetDistance1() < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
			 ( obb.GetDistance1() < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
		{
			return ( false );
		}
	}


	// Test if the 2nd axis of the OBB is a separating axis.
	float rNewFrustrumOriginY   = vDiff.Dot( obb.GetDirection2() );
	float rNewFrustrumFrontDirY = Frustrum.GetFrontDirection().Dot( obb.GetDirection2() );
	float rNewFrustrumLeftDirY  = Frustrum.GetLeftDirection().Dot(  obb.GetDirection2() );
	float rNewFrustrumTopDirY   = Frustrum.GetTopDirection().Dot(   obb.GetDirection2() );

	rProjNear = rNewFrustrumOriginY + 
		        Frustrum.GetNear() * rNewFrustrumFrontDirY;

	rProjFar  = rNewFrustrumOriginY + 
			    Frustrum.GetFar()  * rNewFrustrumFrontDirY;

	if ( rProjNear * rProjFar > 0.0f )
	{
		rProjLeftTop = Frustrum.GetLeft() * MATH::Abs( rNewFrustrumLeftDirY ) +
					   Frustrum.GetTop()  * MATH::Abs( rNewFrustrumTopDirY );

		if ( ( obb.GetDistance2() < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
			 ( obb.GetDistance2() < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
		{
			return ( false );
		}
	}


	// Test if the 3rd axis of the OBB is a separating axis.
	float rNewFrustrumOriginZ   = vDiff.Dot( obb.GetDirection3() );
	float rNewFrustrumFrontDirZ = Frustrum.GetFrontDirection().Dot( obb.GetDirection3() );
	float rNewFrustrumLeftDirZ  = Frustrum.GetLeftDirection().Dot(  obb.GetDirection3() );
	float rNewFrustrumTopDirZ   = Frustrum.GetTopDirection().Dot(   obb.GetDirection3() );

	rProjNear = rNewFrustrumOriginZ + 
		        Frustrum.GetNear() * rNewFrustrumFrontDirZ;

	rProjFar  = rNewFrustrumOriginZ + 
			    Frustrum.GetFar()  * rNewFrustrumFrontDirZ;

	if ( rProjNear * rProjFar > 0.0f )
	{
		rProjLeftTop = Frustrum.GetLeft() * MATH::Abs( rNewFrustrumLeftDirZ ) +
					   Frustrum.GetTop()  * MATH::Abs( rNewFrustrumTopDirZ );

		if ( ( obb.GetDistance3() < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
			 ( obb.GetDistance3() < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
		{
			return ( false );
		}
	}


	if ( nSpeed == 2 )
	{
		return ( true );
	}



	// We test if the normal of the frustrum are separating axis.
	float rProjOBB;
	float rDotProduct;
	VECTOR vFrustrumNewFrontDir( rNewFrustrumFrontDirX,
		                         rNewFrustrumFrontDirY,
								 rNewFrustrumFrontDirZ );
	VECTOR vFrustrumNewOrigin( rNewFrustrumOriginX,
		                       rNewFrustrumOriginY,
							   rNewFrustrumOriginZ );
	VECTOR vNewNormal;

	// Near and far
	rProjOBB = obb.GetDistance1() * MATH::Abs( vFrustrumNewFrontDir.GetX() ) +
			   obb.GetDistance2() * MATH::Abs( vFrustrumNewFrontDir.GetY() ) +
			   obb.GetDistance3() * MATH::Abs( vFrustrumNewFrontDir.GetZ() );
	rDotProduct = vFrustrumNewOrigin.Dot( vFrustrumNewFrontDir );

	if (rDotProduct <= 0.0f)
	{
		if (Frustrum.GetFar() + rProjOBB + rDotProduct <= 0.0f) // Remember here rDotProduct is negative.
		{
			return ( false );
		}
	}
	else
	{
		if (rDotProduct + Frustrum.GetNear() >= rProjOBB)
		{
			return ( false );
		}
	}


	// Left
	vNewNormal.SetX( obb.GetDirection1().Dot( Frustrum.GetNormalLeft() ) );
	vNewNormal.SetY( obb.GetDirection2().Dot( Frustrum.GetNormalLeft() ) );
	vNewNormal.SetZ( obb.GetDirection3().Dot( Frustrum.GetNormalLeft() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = obb.GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   obb.GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   obb.GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

	// Right
	vNewNormal.SetX( obb.GetDirection1().Dot( Frustrum.GetNormalRight() ) );
	vNewNormal.SetY( obb.GetDirection2().Dot( Frustrum.GetNormalRight() ) );
	vNewNormal.SetZ( obb.GetDirection3().Dot( Frustrum.GetNormalRight() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = obb.GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   obb.GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   obb.GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

	// Top
	vNewNormal.SetX( obb.GetDirection1().Dot( Frustrum.GetNormalTop() ) );
	vNewNormal.SetY( obb.GetDirection2().Dot( Frustrum.GetNormalTop() ) );
	vNewNormal.SetZ( obb.GetDirection3().Dot( Frustrum.GetNormalTop() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = obb.GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   obb.GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   obb.GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

	// Bottom
	vNewNormal.SetX( obb.GetDirection1().Dot( Frustrum.GetNormalBottom() ) );
	vNewNormal.SetY( obb.GetDirection2().Dot( Frustrum.GetNormalBottom() ) );
	vNewNormal.SetZ( obb.GetDirection3().Dot( Frustrum.GetNormalBottom() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = obb.GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   obb.GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   obb.GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}


	if ( nSpeed == 1 )
	{
		return ( true );
	}



	// Finally we test if the cross products are separating axis.
	int   i;

	VECTOR vNewFrustrumFar  = vFrustrumNewOrigin + Frustrum.GetFar()  * vFrustrumNewFrontDir;
	VECTOR vNewFrustrumNear = vFrustrumNewOrigin + Frustrum.GetNear() * vFrustrumNewFrontDir;
	VECTOR vFrustrumNewLeftDir( rNewFrustrumLeftDirX, rNewFrustrumLeftDirY, rNewFrustrumLeftDirZ );
	VECTOR vFrustrumNewTopDir(  rNewFrustrumTopDirX,  rNewFrustrumTopDirY,  rNewFrustrumTopDirZ );

	VECTOR vFrustrumEdge;
	for ( i = 0; i < 6; i++ )
	{
		switch( i )
		{
		case 0:
			vFrustrumEdge.SetX( rNewFrustrumLeftDirX );
			vFrustrumEdge.SetY( rNewFrustrumLeftDirY );
			vFrustrumEdge.SetZ( rNewFrustrumLeftDirZ );
			break;
		case 1:
			vFrustrumEdge.SetX( rNewFrustrumTopDirX );
			vFrustrumEdge.SetY( rNewFrustrumTopDirY );
			vFrustrumEdge.SetZ( rNewFrustrumTopDirZ );
			break;
		case 2:
			vFrustrumEdge = Frustrum.GetNear() * vFrustrumNewFrontDir +
							Frustrum.GetLeft() * vFrustrumNewLeftDir +
							Frustrum.GetTop()  * vFrustrumNewTopDir;
			break;
		case 3:
			vFrustrumEdge = Frustrum.GetNear() * vFrustrumNewFrontDir +
							Frustrum.GetLeft() * vFrustrumNewLeftDir -
							Frustrum.GetTop()  * vFrustrumNewTopDir;
			break;
		case 4:
			vFrustrumEdge = Frustrum.GetNear() * vFrustrumNewFrontDir -
							Frustrum.GetLeft() * vFrustrumNewLeftDir +
							Frustrum.GetTop()  * vFrustrumNewTopDir;
			break;
		case 5:
			vFrustrumEdge = Frustrum.GetNear() * vFrustrumNewFrontDir -
							Frustrum.GetLeft() * vFrustrumNewLeftDir -
							Frustrum.GetTop()  * vFrustrumNewTopDir;
			break;
		default:
			QDT_FAIL();
		}

		/* Cross vector: 0.0f
						 vFrustrumEdge.GetZ()
						-vFrustrumEdge.GetY() */

		rProjOBB     = obb.GetDistance2() * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   obb.GetDistance3() * MATH::Abs( vFrustrumEdge.GetY() );

		rProjNear    = vNewFrustrumNear.GetY() * vFrustrumEdge.GetZ() -
					   vNewFrustrumNear.GetZ() * vFrustrumEdge.GetY();

		rProjFar     = vNewFrustrumFar.GetY() * vFrustrumEdge.GetZ() -
					   vNewFrustrumFar.GetZ() * vFrustrumEdge.GetY();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( rNewFrustrumLeftDirY * vFrustrumEdge.GetZ() -
					     			  rNewFrustrumLeftDirZ * vFrustrumEdge.GetY() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( rNewFrustrumTopDirY  * vFrustrumEdge.GetZ() -
					     			  rNewFrustrumTopDirZ  * vFrustrumEdge.GetY() );

			if ( ( rProjOBB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjOBB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}


		/* Cross vector: vFrustrumEdge.GetZ()
						 0.0f
						-vFrustrumEdge.GetX() */

		rProjOBB     = obb.GetDistance1() * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   obb.GetDistance3() * MATH::Abs( vFrustrumEdge.GetX() );

		rProjNear    = vNewFrustrumNear.GetX() * vFrustrumEdge.GetZ() -
					   vNewFrustrumNear.GetZ() * vFrustrumEdge.GetX();

		rProjFar     = vNewFrustrumFar.GetX() * vFrustrumEdge.GetZ() -
					   vNewFrustrumFar.GetZ() * vFrustrumEdge.GetX();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( rNewFrustrumLeftDirX * vFrustrumEdge.GetZ() -
					     			  rNewFrustrumLeftDirZ * vFrustrumEdge.GetX() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( rNewFrustrumTopDirX  * vFrustrumEdge.GetZ() -
					     			  rNewFrustrumTopDirZ  * vFrustrumEdge.GetX() );

			if ( ( rProjOBB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjOBB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}


		/* Cross vector: vFrustrumEdge.GetY()
						-vFrustrumEdge.GetX()
						 0.0f */

		rProjOBB     = obb.GetDistance1() * MATH::Abs( vFrustrumEdge.GetY() ) +
					   obb.GetDistance2() * MATH::Abs( vFrustrumEdge.GetX() );

		rProjNear    = vNewFrustrumNear.GetX() * vFrustrumEdge.GetY() -
					   vNewFrustrumNear.GetY() * vFrustrumEdge.GetX();

		rProjFar     = vNewFrustrumFar.GetX() * vFrustrumEdge.GetY() -
					   vNewFrustrumFar.GetY() * vFrustrumEdge.GetX();

		if ( rProjNear * rProjFar > 0.0f )	// 'rProjNear' and 'rProjFar' have the same sign.
		{
			rProjLeftTop = Frustrum.GetLeft() *
						   MATH::Abs( rNewFrustrumLeftDirX * vFrustrumEdge.GetY() -
					     			  rNewFrustrumLeftDirY * vFrustrumEdge.GetX() ) +
						   Frustrum.GetTop() *
						   MATH::Abs( rNewFrustrumTopDirX  * vFrustrumEdge.GetY() -
					     			  rNewFrustrumTopDirY  * vFrustrumEdge.GetX() );

			if ( ( rProjOBB < MATH::Abs( rProjNear ) - rProjLeftTop ) &&
				 ( rProjOBB < MATH::Abs( rProjFar  ) - rProjLeftTop * rRatio ) )
			{
				return ( false );
			}
		}
	}



	QDT_ASSERT( nSpeed == 0 );

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		TestIntersectionFrustrumPoint
//	Object:		
//	02-04-29:	GGO - Created
//-----------------------------------------------------------------------------
bool	INTERSECTION_FRUSTRUM::TestIntersectionFrustrumPoint(const FRUSTRUM &	Frustrum,
															 const VECTOR &	Point)
{
	if ( Frustrum.GetClippingPlane( FRUSTRUM::CP_RIGHT ).GetSignedDistance( Point ) > 0.0f )
	{
		return ( false );
	}
	if ( Frustrum.GetClippingPlane( FRUSTRUM::CP_LEFT ).GetSignedDistance( Point ) > 0.0f )
	{
		return ( false );
	}
	if ( Frustrum.GetClippingPlane( FRUSTRUM::CP_BOTTOM ).GetSignedDistance( Point ) > 0.0f )
	{
		return ( false );
	}
	if ( Frustrum.GetClippingPlane( FRUSTRUM::CP_TOP ).GetSignedDistance( Point ) > 0.0f )
	{
		return ( false );
	}
	if ( Frustrum.GetClippingPlane( FRUSTRUM::CP_FAR ).GetSignedDistance( Point ) > 0.0f )
	{
		return ( false );
	}
	if ( Frustrum.GetClippingPlane( FRUSTRUM::CP_NEAR ).GetSignedDistance( Point ) > 0.0f )
	{
		return ( false );
	}

	return ( true );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
