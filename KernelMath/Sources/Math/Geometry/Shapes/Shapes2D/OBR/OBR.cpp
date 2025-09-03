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
//	CLASS:	OBR
//
//	01-10-08:	GGO - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/Math)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/OBR/OBR)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include	INCL_KMATH(Math/LinearAlgebra/CovarianceMatrix)
#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include 	INCL_KCORE(stream/iostream)

#ifdef _DEBUG
	#include "OBR.inl"
#endif

//-----------------------------------------------------------------------------
//	Name:		OBR constructor
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
OBR::OBR()
:
_Center(VECTOR::ZERO),
_vDirection1(VECTOR::ZERO),
_vDirection2(VECTOR::ZERO),
_rDistance1(0.0f),
_rDistance2(0.0f)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		OBR constructor
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
OBR::OBR(const OBR & C)
{
	Copy(C);
}

//-----------------------------------------------------------------------------
//	Name:		OBR destructor
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
OBR::~OBR()
{
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
OBR &	OBR::operator=(const OBR & C)
{
	if	(this != &C)
	{
		Copy(C);
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
void	OBR::Copy(const OBR & C)
{
	_Center			= C._Center;
	_vDirection1	= C._vDirection1;
	_vDirection2	= C._vDirection2;
	_rDistance1		= C._rDistance1;
	_rDistance2		= C._rDistance2;
}


//-----------------------------------------------------------------------------
//	Name:		OBR constructor
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
OBR::OBR(int		nNbrPoints,
		 VECTOR *	pPoints,
		 float		rEpsilon)
{
	switch ( nNbrPoints )
	{
	case 0:
		_Center = VECTOR::ZERO;
		_vDirection1 = VECTOR::UNIT_X;
		_vDirection2 = VECTOR::UNIT_Y;
		_rDistance1 = 0.0f;
		_rDistance2 = 0.0f;

	case 1:
		_Center = pPoints[ 0 ];
		_vDirection1 = VECTOR::UNIT_X;
		_vDirection2 = VECTOR::UNIT_Y;
		_rDistance1 = 0.0f;
		_rDistance2 = 0.0f;

	case 2:
		_Center = ( pPoints[ 0 ] + pPoints[ 1 ] ) * 0.5f;

		_vDirection1 = pPoints[ 1 ] - pPoints[ 0 ];
		_rDistance1 = 0.5f * _vDirection1.Length();
		_vDirection1.Normalize();
		
		_vDirection2.SetXYZ( 0.0f, 0.0f, 0.0f );
		_rDistance2 = 0.0f;

	default:

		QDT_ASSERT( nNbrPoints > 0 );

		Compute(nNbrPoints, pPoints, rEpsilon);
	}
}


//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
void	OBR::Compute(int		nNbrPoints,
					 VECTOR *	pPoints,
					 float		rEpsilon)
{
	// Locals.
	int		i;
	bool	bRes;

	// The direction of the OBR.

	// The main direction of the points set.
	COVARIANCE_MATRIX CovarianceMatrix1( nNbrPoints, pPoints );
	if ( CovarianceMatrix1.IsNull() == true )
	{
		_vDirection1.SetXYZ( 1.0f, 0.0f, 0.0f );
		_vDirection2.SetXYZ( 0.0f, 1.0f, 0.0f );
		_rDistance1 = 0.0f;
		_rDistance2 = 0.0f;

		// QDT_Warning(QDT_COMMENT("An OBR has been computed from an empty covariance matrix!1"));
		return;
	}
	bRes = CovarianceMatrix1.GetMainDirection(rEpsilon, _vDirection1);
	QDT_ASSERT(bRes);

	// The second direction.
	VECTOR *pPointTemp = new VECTOR[ nNbrPoints ];
	VECTOR	vPoint, vHyperplan;
	for ( i = 0; i < nNbrPoints; ++i )
	{
		vPoint.SetXYZ( pPoints[ i ].GetX(), pPoints[ i ].GetY(), pPoints[ i ].GetZ() );

		vHyperplan = vPoint - vPoint.Dot( _vDirection1 ) * _vDirection1;

		pPointTemp[ i ].SetXYZ( vHyperplan.GetX(), vHyperplan.GetY(), vHyperplan.GetZ() );
	}
	delete[] pPointTemp;

	COVARIANCE_MATRIX CovarianceMatrix2( nNbrPoints, pPointTemp );
	bRes = CovarianceMatrix2.GetMainDirection(rEpsilon, _vDirection2);
	QDT_ASSERT(bRes);

	QDT_ASSERT( MATH::EqualsEpsilon( _vDirection2.Dot( _vDirection1 ), 0.0f ) == true );

	// The half distances and the center .
	VECTOR vOrtho;
	vOrtho.Cross( _vDirection1, _vDirection2 );

	float rMin1, rMax1, rMin2, rMax2, rMin3, rMax3;
	rMin1 = rMax1 = _vDirection1.Dot( pPoints[0] );
	rMin2 = rMax2 = _vDirection2.Dot( pPoints[0] );
	rMin3 = rMax3 =  vOrtho.Dot( pPoints[0] );

	// We parse the set of points and we look if their projections
	// on the direction vectors are the min or the max.
	float rProj1, rProj2, rProj3;
	for ( i = 1; i < nNbrPoints; ++i )
	{
		rProj1 = _vDirection1.Dot( pPoints[i] );
		rProj2 = _vDirection2.Dot( pPoints[i] );
		rProj3 = vOrtho.Dot( pPoints[i] );

		// Is the projection larger than the current max?
		if ( rProj1 > rMax1 )
		{
			rMax1 = rProj1;
		}
		if ( rProj2 > rMax2 )
		{
			rMax2 = rProj2;
		}
		if ( rProj3 > rMax3 )
		{
			rMax3 = rProj3;
		}

		// Is the projection smaller than the current min?
		if ( rProj1 < rMin1 )
		{
			rMin1 = rProj1;
		}
		if ( rProj2 < rMin2 )
		{
			rMin2 = rProj2;
		}
		if ( rProj3 < rMin3 )
		{
			rMin3 = rProj3;
		}
	}

	// Once we have both the min and the max along each direction, we can deduce
	// the center and the half firections.
	_rDistance1 = 0.5f * ( rMax1 - rMin1 );
	_rDistance2 = 0.5f * ( rMax2 - rMin2 );
//	QDT_ASSERT( MATH::EqualsEpsilon( rMin3, rMax3, rEpsilon ) == true );

	if ( MATH::Abs( _rDistance1 ) < 0.00001f )
	{
		_rDistance1 = 0.0f;
	}
	if ( MATH::Abs( _rDistance2 ) < 0.00001f )
	{
		_rDistance2 = 0.0f;
	}

	_Center = 0.5f * ( rMin1 + rMax1 ) * _vDirection1 +
			  0.5f * ( rMin2 + rMax2 ) * _vDirection2;
}


//-----------------------------------------------------------------------------
//	Name:		GeneratePlanes
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
bool	OBR::GeneratePlanes(const VECTOR&	Point,
							PLANE	(& pPlanes)[4]) const
{
	// The point is in the plane of the OBR, so we return false.
	VECTOR vNormal;
	vNormal.Cross( _vDirection1, _vDirection2 );
	VECTOR	vDiff( _Center - Point );
	float	rDot = vNormal.Dot( vDiff );

	
	// The point is in the plane of the OBR.
	if ( MATH::EqualsEpsilon( rDot, 0.0f, 0.0001f ) == true )
	{
		return ( false );
	}
	else
	{
		VECTOR vTemp;
		

		if ( rDot <= 0.0f )
		{
			vTemp = vDiff + _rDistance1 * _vDirection1 - _rDistance2 * _vDirection2;

			vNormal.Cross( vTemp, _vDirection2 );
			pPlanes[ 0 ].SetFromPointAndNormal( Point, vNormal );

			vNormal.Cross( vTemp, _vDirection1 );
			pPlanes[ 1 ].SetFromPointAndNormal( Point, vNormal );


			vTemp = vDiff - _rDistance1 * _vDirection1 + _rDistance2 * _vDirection2;

			vNormal.Cross( _vDirection2, vTemp );
			pPlanes[ 2 ].SetFromPointAndNormal( Point, vNormal );

			vNormal.Cross( _vDirection1, vTemp );
			pPlanes[ 3 ].SetFromPointAndNormal( Point, vNormal );
		}
		else
		{
			vTemp = vDiff + _rDistance1 * _vDirection1 - _rDistance2 * _vDirection2;

			vNormal.Cross( _vDirection2, vTemp );
			pPlanes[ 0 ].SetFromPointAndNormal( Point, vNormal );

			vNormal.Cross( _vDirection1, vTemp );
			pPlanes[ 1 ].SetFromPointAndNormal( Point, vNormal );


			vTemp = vDiff - _rDistance1 * _vDirection1 + _rDistance2 * _vDirection2;

			vNormal.Cross( vTemp, _vDirection2 );
			pPlanes[ 2 ].SetFromPointAndNormal( Point, vNormal );

			vNormal.Cross( vTemp, _vDirection1 );
			pPlanes[ 3 ].SetFromPointAndNormal( Point, vNormal );
		}


		return ( true );
	}
}


//-----------------------------------------------------------------------------
//	Name:		Display
//	Object:		
//	01-10-08:	GGO - Created
//-----------------------------------------------------------------------------
void	OBR::Display(const COLOR_FLOAT &	Color) const
{
// RMA -> On ne doit pas afficher les maths comme ça!!!
	QDT_FAIL();
}


//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
void	OBR::Transform(const MATRIX &	Matrix)
{
	Matrix.TransformPosition( _Center );
	Matrix.TransformDirection( _vDirection1 );
	Matrix.TransformDirection( _vDirection2 );
}


//-----------------------------------------------------------------------------
//	Name:		GetPlane
//	Object:		
//	01-10-09:	GGO - Created
//-----------------------------------------------------------------------------
PLANE	OBR::GetPlane(const VECTOR &	vNormal)
{
	// We check if the normal really is perpendicular to the OBR.
	QDT_ASSERT( MATH::EqualsEpsilon( vNormal.Dot( _vDirection1 ), 0.0f, 0.001f ) == true );
	QDT_ASSERT( MATH::EqualsEpsilon( vNormal.Dot( _vDirection2 ), 0.0f, 0.001f ) == true );

	return(PLANE(_Center, vNormal));
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	02-12-11:	RMA - Created
//-----------------------------------------------------------------------------
float	OBR::GetArea() const
{
	VECTOR CrossProduct;
	CrossProduct.Cross(_vDirection1 * _rDistance1, _vDirection2 * _rDistance2);
	return (CrossProduct.Length() * 4.0f);
}

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-24:	VMA - Created
//-----------------------------------------------------------------------------
void	OBR::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(OBR) );
}


//=============================================================================
//	CODE ENDS HERE
//=============================================================================

