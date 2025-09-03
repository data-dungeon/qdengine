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
//	CLASS:	OBB
//
//	02-04-02:	JLT - Created
//*****************************************************************************

#include	"Root.h"
#include 	INCL_KMATH(Math/Math)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/OBB/OBB)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/AABB/AABB)
#include 	INCL_KMATH(Math/Geometry/BoundingVolume/Sphere/Sphere)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Segment/Segment)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes2D/Plane/Plane)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Hexahedron/Hexahedron)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Frustrum/Frustrum)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes3D/Cone/Cone)
#include 	INCL_KMATH(Math/Geometry/Matrix/Matrix)
#include 	INCL_KCORE(Profiler/Profiler)
#include 	INCL_KMATH(Math/LinearAlgebra/CovarianceMatrix)
#include 	INCL_KCORE(Stream/iostream)

//=============================================================================
//	CODE STARTS HERE
//=============================================================================

void*		OBB::_pVTable = NULL;

//-----------------------------------------------------------------------------
//	Name:		OBB constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
OBB::OBB()
:
BOUNDING_VOLUME(TYPE_OBB)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		OBB constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
OBB::OBB(const OBB &C)
:
BOUNDING_VOLUME(TYPE_OBB),
_Center(C._Center),
_Direction1(C._Direction1),
_Direction2(C._Direction2),
_Direction3(C._Direction3),
_rDistance1(C._rDistance1),
_rDistance2(C._rDistance2),
_rDistance3(C._rDistance3),
_Axe1(C._Axe1),
_Axe2(C._Axe2),
_Axe3(C._Axe3)
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		OBB constructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
OBB::OBB(const int nNbrPts, VECTOR *pPoints)
:
BOUNDING_VOLUME(TYPE_OBB)
{
	Compute(nNbrPts, pPoints);
}

//-----------------------------------------------------------------------------
//	Name:		OBB destructor
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
OBB::~OBB()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-04-02:	JLT - Created
//-----------------------------------------------------------------------------
OBB& OBB::operator=(const OBB & C)
{
	if	(this != &C)
	{
		_Center = C._Center;
		_Direction1 = C._Direction1;
		_Direction2 = C._Direction2;
		_Direction3 = C._Direction3;
		_rDistance1 = C._rDistance1;
		_rDistance2 = C._rDistance2;
		_rDistance3 = C._rDistance3;
		_Axe1		= C._Axe1;
		_Axe2		= C._Axe2;
		_Axe3		= C._Axe3;
	}
	
	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Clear
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
void	OBB::Clear()
{
	_Center.SetXYZ(0.0f, 0.0f, 0.0f);
	_Direction1.SetXYZ(0.0f, 0.0f, 0.0f);
	_Direction2.SetXYZ(0.0f, 0.0f, 0.0f);
	_Direction3.SetXYZ(0.0f, 0.0f, 0.0f);
	_rDistance1 = 0.0f;
	_rDistance2 = 0.0f;
	_rDistance3 = 0.0f;
	_Axe1.SetXYZ(0.0f, 0.0f, 0.0f);
	_Axe2.SetXYZ(0.0f, 0.0f, 0.0f);
	_Axe3.SetXYZ(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		Read
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER_ONLY_DATABANK
void	OBB::Read(IO_STREAM &	Stream)
{
	Stream	>> _Center 
			>> _Direction1 >> _Direction2 >> _Direction3
			>> _rDistance1 >> _rDistance2 >> _rDistance3;

	_Axe1 = _rDistance1 * _Direction1;
	_Axe2 = _rDistance2 * _Direction2;
	_Axe3 = _rDistance3 * _Direction3;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		Write
//	Object:		
//	01-07-10:	RMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	OBB::Write(IO_STREAM &	Stream) const
{
	Stream.DebugOutput("Center : ");
	Stream	<< _Center;
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("Direction 1 : ");
	Stream	<< _Direction1;
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("Direction 2 : ");
	Stream	<< _Direction2;
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("Direction 3 : ");
	Stream	<< _Direction3;
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("Distance 1 : ");
	Stream	<< _rDistance1;
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("Distance 2 : ");
	Stream	<< _rDistance2;
	Stream.DebugOutput("\t");
	
	Stream.DebugOutput("Distance 3 : ");
	Stream	<< _rDistance3;
}
#endif

//-----------------------------------------------------------------------------
//	Name:		WriteBinary
//	Object:		
//	03-11-13:	VMA - Created
//-----------------------------------------------------------------------------
#ifndef _MASTER
void	OBB::WriteBinary(IO_STREAM &	Stream)
{
	Stream.PutBufferFast( this, sizeof(OBB) );
}
#endif

//-----------------------------------------------------------------------------
//	Name: OBB::Display
//	Object: 
//	01-03-01:	GGO - Created
//-----------------------------------------------------------------------------
void	OBB::Display(const COLOR_FLOAT & Color) const
{
	// RMA -> On ne doit pas afficher les maths comme ça!!!
}

//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	05-03-24:	GGO - Created
//-----------------------------------------------------------------------------
void	OBB::Compute(Float32	rY,
					 Float32	rX1,
					 Float32	rZ1,
					 Float32	rX2,
					 Float32	rZ2,
					 Float32	rX3,
					 Float32	rZ3,
					 Float32	rX4,
					 Float32	rZ4)
{
	static	const Float32	EPSILON = 0.0001f;
			Float32		CovarianceMatrix[2][2];
			Float32		rTrace, rDet, rDisc, rEigen, rK1, rK2;
			Float32		rX_Mean, rZ_Mean, rMax, rMax1, rMax2, rMin1, rMin2, rProj1, rProj2;

	_Direction1	= VECTOR( 1.0f, 0.0f, 0.0f );
	_Direction2	= VECTOR( 0.0f, 0.0f, 1.0f );
	_Direction3	= VECTOR( 0.0f, 1.0f, 0.0f );

	// Build the covariance matrix.
	rX_Mean = 0.25f * ( rX1 + rX2 + rX3 + rX4 );
	rZ_Mean = 0.25f * ( rZ1 + rZ2 + rZ3 + rZ4 );

	CovarianceMatrix[0][0] = 0.25f * ( rX1 * rX1 + rX2 * rX2 + rX3 * rX3 + rX4 * rX4 ) - rX_Mean * rX_Mean;
	CovarianceMatrix[0][1] = 0.25f * ( rX1 * rZ1 + rX2 * rZ2 + rX3 * rZ3 + rX4 * rZ4 ) - rX_Mean * rZ_Mean;
	CovarianceMatrix[1][0] = CovarianceMatrix[0][1];
	CovarianceMatrix[1][1] = 0.25f * ( rZ1 * rZ1 + rZ2 * rZ2 + rZ3 * rZ3 + rZ4 * rZ4 ) - rZ_Mean * rZ_Mean;

	// Normalize the matrix to avoid numerical issues.
	rMax = MATH::Abs( CovarianceMatrix[0][0] );
	rMax = MATH::Max( rMax, MATH::Abs( CovarianceMatrix[0][1] ) );
	rMax = MATH::Max( rMax, MATH::Abs( CovarianceMatrix[1][0] ) );
	rMax = MATH::Max( rMax, MATH::Abs( CovarianceMatrix[1][1] ) );

	if ( rMax > EPSILON )
	{
		CovarianceMatrix[0][0] /= rMax;
		CovarianceMatrix[0][1] /= rMax;
		CovarianceMatrix[1][0] /= rMax;
		CovarianceMatrix[1][1] /= rMax;

		// Find the first of the 2 eigen vectors.
		rTrace	= CovarianceMatrix[0][0] + CovarianceMatrix[1][1];
		rDet	= CovarianceMatrix[0][0] * CovarianceMatrix[1][1] - CovarianceMatrix[0][1] * CovarianceMatrix[1][0];

		rDisc	= 0.25f * rTrace * rTrace - rDet;
		if ( rDisc >= 0.0 )
		{
			rEigen	= 0.5f * rTrace + MATH::Sqrt( rDisc );

			if ( MATH::Abs( rEigen ) > EPSILON )
			{
				rK1 = CovarianceMatrix[0][0] + CovarianceMatrix[1][0] - rEigen;
				rK2 = CovarianceMatrix[0][1] + CovarianceMatrix[1][1] - rEigen;

				if ( MATH::Abs( rK1 ) > MATH::Abs( rK2 ) )
				{
					if ( MATH::Abs( rK1 ) > EPSILON )
					{
						_Direction1.SetXYZ( -rK2 / rK1, 0.0f, 1.0f );
						_Direction1.Normalize();
					}	
				}
				else
				{
					if ( MATH::Abs( rK2 ) > EPSILON )
					{
						_Direction1.SetXYZ( 1.0f, 0.0f, -rK1 / rK2 );
						_Direction1.Normalize();
					}
				}
			}
		}
	}

	_Direction2.Cross( _Direction1, _Direction3 );
	QDT_ASSERT( _Direction2.IsNormalized() == true );

	// Find the half-distances.
	rMax1 	= _Direction1.GetX() * rX1 + _Direction1.GetZ() * rZ1;
	rMax2 	= _Direction2.GetX() * rX1 + _Direction2.GetZ() * rZ1;
	rMin1 	= rMax1;
	rMin2 	= rMax2;

	rProj1	= _Direction1.GetX() * rX2 + _Direction1.GetZ() * rZ2;
	rProj2	= _Direction2.GetX() * rX2 + _Direction2.GetZ() * rZ2;
	rMax1	= MATH::Max( rMax1, rProj1 );
	rMax2	= MATH::Max( rMax2, rProj2 );
	rMin1	= MATH::Min( rMin1, rProj1 );
	rMin2	= MATH::Min( rMin2, rProj2 );

	rProj1	= _Direction1.GetX() * rX3 + _Direction1.GetZ() * rZ3;
	rProj2	= _Direction2.GetX() * rX3 + _Direction2.GetZ() * rZ3;
	rMax1	= MATH::Max( rMax1, rProj1 );
	rMax2	= MATH::Max( rMax2, rProj2 );
	rMin1	= MATH::Min( rMin1, rProj1 );
	rMin2	= MATH::Min( rMin2, rProj2 );

	rProj1	= _Direction1.GetX() * rX4 + _Direction1.GetZ() * rZ4;
	rProj2	= _Direction2.GetX() * rX4 + _Direction2.GetZ() * rZ4;
	rMax1	= MATH::Max( rMax1, rProj1 );
	rMax2	= MATH::Max( rMax2, rProj2 );
	rMin1	= MATH::Min( rMin1, rProj1 );
	rMin2	= MATH::Min( rMin2, rProj2 );

	_rDistance1 = 0.5f * ( rMax1 - rMin1 );
	_rDistance2 = 0.5f * ( rMax2 - rMin2 );
	_rDistance3 = 0.0f;
	
	// Compute center and axis.
	_Center = 0.5f * ( rMin1 + rMax1 ) * _Direction1 +
			  0.5f * ( rMin2 + rMax2 ) * _Direction2;
	_Center.SetY( rY );

	_Axe1 = _rDistance1 * _Direction1;
	_Axe2 = _rDistance2 * _Direction2;
	_Axe3 = _rDistance3 * _Direction3;
}

//-----------------------------------------------------------------------------
//	Name:		Compute
//	Object:		
//	01-07-16:	RMA - Created
//  04-10-77:   RMA - Modified : I've change the coordinate system of the entry
//				points with the barycenter of the points cloud to avoid big 
//				precision artefacts.
//-----------------------------------------------------------------------------
void	OBB::Compute(int		nNbrPoints,
					 VECTOR *	pPoints)
{
	QDT_ASSERT( nNbrPoints > 0 );

	const Float32 EPSILON = 0.1f;
	double		dx = 0.0, dy = 0.0, dz = 0.0;
	int			i;

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		dx += pPoints[i].GetX();
		dy += pPoints[i].GetY();
		dz += pPoints[i].GetZ();
	}

	VECTOR BaryCenter( (Float32)(dx / nNbrPoints), (Float32)(dy / nNbrPoints), (Float32)(dz / nNbrPoints));

	VECTOR * pNewPoints = new VECTOR[nNbrPoints];

	for (i = 0 ; i < nNbrPoints ; ++i)
	{
		pNewPoints[i] = pPoints[i] - BaryCenter;
	}

	//== The direction vectors of the OBB ==
	FindDirections( nNbrPoints, pNewPoints );

	if ( ( MATH::EqualsEpsilon( _Direction1.Dot( _Direction2 ), 0.0f, EPSILON ) == false ) ||
		 ( MATH::EqualsEpsilon( _Direction1.Dot( _Direction3 ), 0.0f, EPSILON ) == false ) ||
		 ( MATH::EqualsEpsilon( _Direction2.Dot( _Direction3 ), 0.0f, EPSILON ) == false ) ||
		 ( MATH::EqualsEpsilon( _Direction1.Length(), 1.0, EPSILON ) == false ) ||
		 ( MATH::EqualsEpsilon( _Direction2.Length(), 1.0, EPSILON ) == false ) ||
		 ( MATH::EqualsEpsilon( _Direction3.Length(), 1.0, EPSILON ) == false ) )
	{
		_Direction1.SetXYZ( 1.0f, 0.0f, 0.0f );
		_Direction2.SetXYZ( 0.0f, 1.0f, 0.0f );
		_Direction3.SetXYZ( 0.0f, 0.0f, 1.0f );
	}
	//==

	//== The half distances and the center ==
	Float32 rMin1 = QDT_FLOAT32_MAX;
	Float32 rMin2 = QDT_FLOAT32_MAX;
	Float32 rMin3 = QDT_FLOAT32_MAX;
	Float32 rMax1 = -QDT_FLOAT32_MAX;
	Float32 rMax2 = -QDT_FLOAT32_MAX;
	Float32 rMax3 = -QDT_FLOAT32_MAX;

	// We parse the set of points and we look if their projections
	// on the direction vectors are the min or the max.
	Float32 rProj1, rProj2, rProj3;
	for ( i = 0; i < nNbrPoints; i++)
	{
		rProj1 = _Direction1.Dot( pNewPoints[i] );
		rProj2 = _Direction2.Dot( pNewPoints[i] );
		rProj3 = _Direction3.Dot( pNewPoints[i] );

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
	// the center and the half directions.
	_rDistance1 = 0.5f * ( rMax1 - rMin1 );
	_rDistance2 = 0.5f * ( rMax2 - rMin2 );
	_rDistance3 = 0.5f * ( rMax3 - rMin3 );

	_Center = 0.5f * ( rMin1 + rMax1 ) * _Direction1 +
			  0.5f * ( rMin2 + rMax2 ) * _Direction2 +
		      0.5f * ( rMin3 + rMax3 ) * _Direction3;

	_Axe1 = _rDistance1 * _Direction1;
	_Axe2 = _rDistance2 * _Direction2;
	_Axe3 = _rDistance3 * _Direction3;

	_Center += BaryCenter;

	delete [] pNewPoints;
}

//-----------------------------------------------------------------------------
//	Name:		Grow
//	Object:		
//	04-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	OBB::Grow(const VECTOR &	P)
{
	VECTOR V;
	V.Sub(P, _Center);

	Float32 rProj1 = MATH::Abs(V.Dot(_Direction1));
	Float32 rProj2 = MATH::Abs(V.Dot(_Direction2));
	Float32 rProj3 = MATH::Abs(V.Dot(_Direction3));

	if (rProj1 > _rDistance1)
	{
		_rDistance1 = rProj1; 
		_Axe1 = _rDistance1 * _Direction1;
	}

	if (rProj2 > _rDistance2)
	{
		_rDistance2 = rProj2; 
		_Axe1 = _rDistance2 * _Direction2;
	}

	if (rProj3 > _rDistance3)
	{
		_rDistance3 = rProj3; 
		_Axe1 = _rDistance3 * _Direction3;
	}
}

//-----------------------------------------------------------------------------
//	Name:		Extend
//	Object:		
//	04-10-04:	RMA - Created
//-----------------------------------------------------------------------------
void	OBB::Extend(Float32	r)
{
	_rDistance1 += r;
	_rDistance2 += r;
	_rDistance3 += r;
}

//-----------------------------------------------------------------------------
//	Name:		Extend
//	Object:		
//	04-10-04:	RMA - Created
//-----------------------------------------------------------------------------
void	OBB::Extend(Float32	rx, Float32	ry, Float32	rz)
{
	_rDistance1 += rx;
	_rDistance2 += ry;
	_rDistance3 += rz;
}

//-----------------------------------------------------------------------------
//	Name:		GetDirections
//	Object:		
//	01-11-14:	GGO - Created
//-----------------------------------------------------------------------------
void	OBB::FindDirections(int			nNbrPoints,
							VECTOR *	pPoints)
{
	bool bRes;

	QDT_ASSERT( nNbrPoints >= 0 );

	// First direction.
	COVARIANCE_MATRIX CovarianceMatrix1( nNbrPoints, pPoints );
	if ( CovarianceMatrix1.IsNull( 0.001f ) == true )
	{
		_Direction1.SetXYZ( 1.0f, 0.0f, 0.0f );
		_Direction2.SetXYZ( 0.0f, 1.0f, 0.0f );
		_Direction3.SetXYZ( 0.0f, 0.0f, 1.0f );

		return;
	}

	CovarianceMatrix1.Normalize();
	bRes = CovarianceMatrix1.GetMainDirection(0.001f, _Direction1);
	QDT_ASSERT(bRes);
	_Direction1.Normalize();

	// Second direction.
	VECTOR   vTemp;
	VECTOR *pPointsTemp = new VECTOR[nNbrPoints];
	for ( int i = 0; i < nNbrPoints; ++i )
	{
		vTemp.SetXYZ(pPoints[ i ].GetX(), pPoints[i].GetY(), pPoints[i].GetZ());
		vTemp -= _Direction1 * vTemp.Dot(_Direction1);
		pPointsTemp[i].SetXYZ(vTemp.GetX(), vTemp.GetY(), vTemp.GetZ());
	}
	COVARIANCE_MATRIX CovarianceMatrix2( nNbrPoints, pPointsTemp );
	if ( CovarianceMatrix2.IsNull( 0.001f ) == true )
	{
		_Direction2 = _Direction1.GetOrthoVector();
	}
	else
	{
		CovarianceMatrix2.Normalize();
		if (!CovarianceMatrix2.GetMainDirection(0.00001f, _Direction2))
		{
			_Direction2 = _Direction1.GetOrthoVector();
		}
	}

	_Direction2.Normalize();

	// Third direction.
	_Direction3.Cross(_Direction1, _Direction2);

	delete[] pPointsTemp;
}

//-----------------------------------------------------------------------------
//	Name:		GetPoints
//	Object:		
//	01-09-13:	GGO - Created
//-----------------------------------------------------------------------------
void	OBB::GetPoints(VECTOR	(& Points)[8]) const
{
	VECTOR PointLeft(  GetCenter() + _rDistance1 * _Direction1 );
	VECTOR PointRight( GetCenter() - _rDistance1 * _Direction1 );

	VECTOR v2	= _rDistance2 * _Direction2;
	VECTOR v3	= _rDistance3 * _Direction3;

	Points[0] =  PointLeft  + v2 + v3;
	Points[1] =  PointLeft  + v2 - v3;
	Points[2] =  PointLeft  - v2 + v3;
	Points[3] =  PointLeft  - v2 - v3;

	Points[4] =  PointRight + v2 + v3;
	Points[5] =  PointRight + v2 - v3;
	Points[6] =  PointRight - v2 + v3;
	Points[7] =  PointRight - v2 - v3;
}

//-----------------------------------------------------------------------------
//	Name:		GetSize
//	Object:		
//	02-03-11:	ELE - Created
//-----------------------------------------------------------------------------
Float32	OBB::GetSize() const
{
	Float32	rH[3];

	rH[0] = _rDistance1 * 2;
	rH[1] = _rDistance2 * 2;
	rH[2] = _rDistance3 * 2;

	return	(MATH::Max(3, rH));
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	06-01-30:	ELE - Created
//-----------------------------------------------------------------------------
void	OBB::Set(const CONE &	Cone)
{
	VECTOR	Tmp;
	PLANE	P;

	const VECTOR &	ConeAxe = Cone.GetAxe();
	const VECTOR &	ConeTop = Cone.GetTop();
	Float32			rConeRadius = Cone.GetRadius();

	_rDistance1 =  rConeRadius / 2;
	_rDistance2 = _rDistance3 = Cone.GetSinAngle() * rConeRadius;

	Tmp.Mul(ConeAxe, _rDistance1);
	_Center.Add(ConeTop, Tmp);

	_Direction1 = ConeAxe;
	P.SetFromPointAndNormal(ConeTop, ConeAxe);
	P.GetVectors(_Direction2, _Direction3);
}

//-----------------------------------------------------------------------------
//	Name: OBB::Transform
//	Object: Transform an OBB, but I'm sure u've already guessed...
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
void	OBB::Transform( const MATRIX & Matrix )
{
	// DAC/ELE: removed because if we need to scale a mesh for ex, we need to have scale in
	// OBB Transform too
	//QDT_ASSERT(Matrix.HasNoScale() == true);

	Matrix.TransformDirection(_Direction1);
	Matrix.TransformDirection(_Direction2);
	Matrix.TransformDirection(_Direction3);

	Matrix.TransformPosition(VECTOR(_Center), _Center);

	_Axe1 = _rDistance1 * _Direction1;
	_Axe2 = _rDistance2 * _Direction2;
	_Axe3 = _rDistance3 * _Direction3;
}

//-----------------------------------------------------------------------------
//	Name:		GetArea
//	Object:		
//	03-02-20:	ELE - Created
//-----------------------------------------------------------------------------
Float32	OBB::GetArea() const
{
	if	(_rDistance1 < _rDistance2)
	{
		if	(_rDistance3 < _rDistance1)
		{
			return	(4 * _rDistance1 * _rDistance2);
		}
		else
		{
			return	(4 * _rDistance2 * _rDistance3);
		}
	}
	else
	{
		if	(_rDistance3 < _rDistance2)
		{
			return	(4 * _rDistance1 * _rDistance2);
		}
		else
		{
			return	(4 * _rDistance1 * _rDistance3);
		}
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetRandomPoint
//	Object:		
//	01-11-29:	ELE - Created
//-----------------------------------------------------------------------------
void	OBB::GetRandomPoint(VECTOR &	Point) const
{
	// We assume that _Direction1 is X, _Direction2 is Y and _Direction3 is Z

	Float32	rPosX = MATH::Rand(2.0f * _rDistance1) - _rDistance1;
	Float32	rPosY = MATH::Rand(2.0f * _rDistance2) - _rDistance2;
	Float32	rPosZ = MATH::Rand(2.0f * _rDistance3) - _rDistance3;

	Point.SetXYZ(rPosX, rPosY, rPosZ);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const PLANE &	Plane,
					   bool &			bSide) const
{
	if	(Plane.GetNormal().IsNull())
	{
		bSide = false;
		return (false);
	}

	QDT_ASSERT( Plane.GetNormal().IsNormalized() == true );

	// We project the obb and the plane on the plane normal.
	Float32 rProjPlane = Plane.GetNormal().Dot(_Center) + Plane.GetD();
	Float32 rEff	     = MATH::Abs(Plane.GetNormal().Dot(_Axe1)) +
					   MATH::Abs(Plane.GetNormal().Dot(_Axe2)) +
					   MATH::Abs(Plane.GetNormal().Dot(_Axe3));

	// Intersection.
	if ( MATH::Abs(rProjPlane) <= rEff)
	{
		return (true);
	}

	// No intersection, so we look if the obb in the half space of the plane normal.
	bSide = (rProjPlane >= 0.0f);
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const RAY &	Ray,
					   VECTOR &	HitPoint) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const RAY &	Ray,
					   Float32 &		rDistance) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		IsSeparatingAxis
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::IsSeparatingAxis(const PLANE &	Plane) const
{
//	QDT_ASSERT(!Plane.GetNormal().IsNull());
//	QDT_ASSERT(Plane.GetNormal().IsNormalized());

	Float32 rProjPlane = Plane.GetSignedDistance(_Center);
	Float32 rEff	     = MATH::Abs(Plane.GetNormal().Dot(_Axe1)) +
					   MATH::Abs(Plane.GetNormal().Dot(_Axe2)) +
					   MATH::Abs(Plane.GetNormal().Dot(_Axe3));

	return (rProjPlane - rEff >= 0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	06-01-27:	ELE - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const HEXAHEDRON &	Hexahedron) const
{
	if (IsSeparatingAxis(Hexahedron.GetPlaneNear())		||
		IsSeparatingAxis(Hexahedron.GetPlaneLeft())		||
		IsSeparatingAxis(Hexahedron.GetPlaneRight())	||
		IsSeparatingAxis(Hexahedron.GetPlaneTop())		||
		IsSeparatingAxis(Hexahedron.GetPlaneBottom())	||
		IsSeparatingAxis(Hexahedron.GetPlaneFar()))
	{
		return (false);
	}
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-07-02:	ELE - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const HEXAHEDRON &	Hexahedron,
					   bool &				bIntersectFront) const
{
	bool	bSide;

	// Test if the normals of the planes of the hexahedron are separating axis.
	if (IsSeparatingAxis(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		return	(false);
	}
	else
	{
		bIntersectFront = Intersect(Hexahedron.GetPlaneNear(), bSide);
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneLeft())		||
		IsSeparatingAxis(Hexahedron.GetPlaneRight())	||
		IsSeparatingAxis(Hexahedron.GetPlaneTop())		||
		IsSeparatingAxis(Hexahedron.GetPlaneBottom())	||
		IsSeparatingAxis(Hexahedron.GetPlaneFar()))
	{
		return (false);
	}
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const HEXAHEDRON &	Hexahedron,
					   bool &				bInside,
					   bool &				bIntersectFront) const
{
/*	bool	bIntersect, bSide;

	if (IsSeparatingAxis(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		bInside = false;
		return	(false);
	}
	else
	{
		bIntersect = Intersect(Hexahedron.GetPlaneNear(), bSide);
		bIntersectFront = bIntersect;
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneLeft()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneLeft(), bSide);
		}
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneRight()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneRight(), bSide);
		}
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneTop()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneTop(), bSide);
		}
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneBottom()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneBottom(), bSide);
		}
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneFar()))
	{
		bInside = false;
		return	(false);
	}
	else
	{
		if	(bIntersect == false)
		{
			bIntersect = Intersect(Hexahedron.GetPlaneFar(), bSide);
		}
	}

	bInside = !bIntersect;
*/
	bool	bSide;

	bInside = true;

	if	(!Intersect(Hexahedron.GetPlaneNear(), bSide))
	{
		bIntersectFront = false;

		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bIntersectFront = true;
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneLeft(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneRight(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneBottom(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneTop(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneFar(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	OBB::IntersectFront(const HEXAHEDRON &	Hexahedron,
							bool &				bIntersectFront) const
{
	bool	bSide;

	// Test if the normals of the planes of the hexahedron are separating axis.
	if (IsSeparatingAxis(Hexahedron.GetPlaneNear()))
	{
		bIntersectFront = false;
		return	(false);
	}
	else
	{
		int	nSide;

		Hexahedron.GetPlaneNear().GetSide(_Center, nSide, 1.0f);
		if	(nSide <= 0)
		{
			bIntersectFront = Intersect(Hexahedron.GetPlaneNear(), bSide);
		}
		else
		{
			bIntersectFront = false;
			return	(false);
		}
	}

	if (IsSeparatingAxis(Hexahedron.GetPlaneLeft())		||
		IsSeparatingAxis(Hexahedron.GetPlaneRight())	||
		IsSeparatingAxis(Hexahedron.GetPlaneTop())		||
		IsSeparatingAxis(Hexahedron.GetPlaneBottom())	||
		IsSeparatingAxis(Hexahedron.GetPlaneFar()))
	{
		return (false);
	}
	
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		IntersectFront
//	Object:		
//	03-08-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	OBB::IntersectFront(const HEXAHEDRON &	Hexahedron,
							bool &				bInside,
							bool &				bIntersectFront) const
{
	bool	bSide;

	bInside = true;

	int	nSide;

	Hexahedron.GetPlaneNear().GetSide(_Center, nSide, 1.0f);
	if	(nSide <= 0)
	{
		if	(!Intersect(Hexahedron.GetPlaneNear(), bSide))
		{
			bIntersectFront = false;

			if	(bSide)
			{
				bInside = false;
				return	(false);
			}
		}
		else
		{
			bIntersectFront = true;
			bInside = false;
		}
	}
	else
	{
		bIntersectFront = false;
		return	(false);
	}

	if	(!Intersect(Hexahedron.GetPlaneLeft(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneRight(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneBottom(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneTop(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	if	(!Intersect(Hexahedron.GetPlaneFar(), bSide))
	{
		if	(bSide)
		{
			bInside = false;
			return	(false);
		}
	}
	else
	{
		bInside = false;
	}

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-27:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const FRUSTRUM &	Frustrum) const
{
	Float32	rProjNear, rProjFar, rProjLeftTop;
	Float32	rRatio = Frustrum.GetFar() / Frustrum.GetNear();
	VECTOR	vDiff  = Frustrum.GetOrigin() - GetCenter();

	// Test if the 1st axis of the OBB is a separating axis.
	Float32 rNewFrustrumOriginX   = vDiff.Dot(GetDirection1());
	Float32 rNewFrustrumFrontDirX = Frustrum.GetFrontDirection().Dot(GetDirection1());
	Float32 rNewFrustrumLeftDirX  = Frustrum.GetLeftDirection().Dot(GetDirection1());
	Float32 rNewFrustrumTopDirX   = Frustrum.GetTopDirection().Dot(GetDirection1());

	rProjNear = rNewFrustrumOriginX + 
		        Frustrum.GetNear() * rNewFrustrumFrontDirX;

	rProjFar  = rNewFrustrumOriginX + 
			    Frustrum.GetFar()  * rNewFrustrumFrontDirX;

	if (rProjNear * rProjFar > 0.0f)
	{
		rProjLeftTop = Frustrum.GetLeft() * MATH::Abs(rNewFrustrumLeftDirX) +
					   Frustrum.GetTop()  * MATH::Abs(rNewFrustrumTopDirX);

		if ((GetDistance1() < MATH::Abs(rProjNear) - rProjLeftTop) &&
			(GetDistance1() < MATH::Abs(rProjFar ) - rProjLeftTop * rRatio))
		{
			return (false);
		}
	}

	// Test if the 2nd axis of the OBB is a separating axis.
	Float32 rNewFrustrumOriginY   = vDiff.Dot(GetDirection2());
	Float32 rNewFrustrumFrontDirY = Frustrum.GetFrontDirection().Dot(GetDirection2());
	Float32 rNewFrustrumLeftDirY  = Frustrum.GetLeftDirection().Dot(GetDirection2());
	Float32 rNewFrustrumTopDirY   = Frustrum.GetTopDirection().Dot(GetDirection2());

	rProjNear = rNewFrustrumOriginY + 
		        Frustrum.GetNear() * rNewFrustrumFrontDirY;

	rProjFar  = rNewFrustrumOriginY + 
			    Frustrum.GetFar()  * rNewFrustrumFrontDirY;

	if ( rProjNear * rProjFar > 0.0f )
	{
		rProjLeftTop = Frustrum.GetLeft() * MATH::Abs(rNewFrustrumLeftDirY) +
					   Frustrum.GetTop()  * MATH::Abs(rNewFrustrumTopDirY);

		if ((GetDistance2() < MATH::Abs(rProjNear) - rProjLeftTop) &&
			(GetDistance2() < MATH::Abs(rProjFar ) - rProjLeftTop * rRatio))
		{
			return ( false );
		}
	}

	// Test if the 3rd axis of the OBB is a separating axis.
	Float32 rNewFrustrumOriginZ   = vDiff.Dot(GetDirection3());
	Float32 rNewFrustrumFrontDirZ = Frustrum.GetFrontDirection().Dot(GetDirection3());
	Float32 rNewFrustrumLeftDirZ  = Frustrum.GetLeftDirection().Dot(GetDirection3());
	Float32 rNewFrustrumTopDirZ   = Frustrum.GetTopDirection().Dot(GetDirection3());

	rProjNear = rNewFrustrumOriginZ + 
		        Frustrum.GetNear() * rNewFrustrumFrontDirZ;

	rProjFar  = rNewFrustrumOriginZ + 
			    Frustrum.GetFar()  * rNewFrustrumFrontDirZ;

	if (rProjNear * rProjFar > 0.0f)
	{
		rProjLeftTop = Frustrum.GetLeft() * MATH::Abs(rNewFrustrumLeftDirZ) +
					   Frustrum.GetTop()  * MATH::Abs(rNewFrustrumTopDirZ);

		if ((GetDistance3() < MATH::Abs(rProjNear) - rProjLeftTop ) &&
			(GetDistance3() < MATH::Abs(rProjFar ) - rProjLeftTop * rRatio))
		{
			return (false);
		}
	}

	// We test if the normal of the frustrum are separating axis.
	Float32 rProjOBB;
	Float32 rDotProduct;
	VECTOR vFrustrumNewFrontDir( rNewFrustrumFrontDirX,
		                         rNewFrustrumFrontDirY,
								 rNewFrustrumFrontDirZ );
	VECTOR vFrustrumNewOrigin( rNewFrustrumOriginX,
		                       rNewFrustrumOriginY,
							   rNewFrustrumOriginZ );
	VECTOR vNewNormal;

	// Near and far
	rProjOBB = GetDistance1() * MATH::Abs( vFrustrumNewFrontDir.GetX() ) +
			   GetDistance2() * MATH::Abs( vFrustrumNewFrontDir.GetY() ) +
			   GetDistance3() * MATH::Abs( vFrustrumNewFrontDir.GetZ() );
	rDotProduct = vFrustrumNewOrigin.Dot( vFrustrumNewFrontDir );

	if ( rDotProduct <= 0.0f )
	{
		if ( Frustrum.GetFar() + rProjOBB + rDotProduct <= 0.0f ) // Remember here rDotProduct is negative.
		{
			return ( false );
		}
	}
	else
	{
		if ( rDotProduct + Frustrum.GetNear() >= rProjOBB )
		{
			return ( false );
		}
	}

	// Left
	vNewNormal.SetX( GetDirection1().Dot( Frustrum.GetNormalLeft() ) );
	vNewNormal.SetY( GetDirection2().Dot( Frustrum.GetNormalLeft() ) );
	vNewNormal.SetZ( GetDirection3().Dot( Frustrum.GetNormalLeft() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

	// Right
	vNewNormal.SetX( GetDirection1().Dot( Frustrum.GetNormalRight() ) );
	vNewNormal.SetY( GetDirection2().Dot( Frustrum.GetNormalRight() ) );
	vNewNormal.SetZ( GetDirection3().Dot( Frustrum.GetNormalRight() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

	// Top
	vNewNormal.SetX( GetDirection1().Dot( Frustrum.GetNormalTop() ) );
	vNewNormal.SetY( GetDirection2().Dot( Frustrum.GetNormalTop() ) );
	vNewNormal.SetZ( GetDirection3().Dot( Frustrum.GetNormalTop() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

	// Bottom
	vNewNormal.SetX( GetDirection1().Dot( Frustrum.GetNormalBottom() ) );
	vNewNormal.SetY( GetDirection2().Dot( Frustrum.GetNormalBottom() ) );
	vNewNormal.SetZ( GetDirection3().Dot( Frustrum.GetNormalBottom() ) );

	rDotProduct = vFrustrumNewOrigin.Dot( vNewNormal );
	if ( rDotProduct < 0.0f )
	{
		rProjOBB = GetDistance1() * MATH::Abs( vNewNormal.GetX() ) +
				   GetDistance2() * MATH::Abs( vNewNormal.GetY() ) +
				   GetDistance3() * MATH::Abs( vNewNormal.GetZ() );

		if ( -rDotProduct > rProjOBB )
		{
			return ( false );
		}
	}

//	if ( nSpeed == 1 )
//	{
//		return ( true );
//	}

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

		// Cross vector: 0.0f
		//				 vFrustrumEdge.GetZ()
		//				-vFrustrumEdge.GetY()

		rProjOBB     = GetDistance2() * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   GetDistance3() * MATH::Abs( vFrustrumEdge.GetY() );

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


		// Cross vector: vFrustrumEdge.GetZ()
		//				 0.0f
		//				-vFrustrumEdge.GetX()

		rProjOBB     = GetDistance1() * MATH::Abs( vFrustrumEdge.GetZ() ) +
					   GetDistance3() * MATH::Abs( vFrustrumEdge.GetX() );

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


		// Cross vector: vFrustrumEdge.GetY()
		//				-vFrustrumEdge.GetX()
		//				 0.0f

		rProjOBB     = GetDistance1() * MATH::Abs( vFrustrumEdge.GetY() ) +
					   GetDistance2() * MATH::Abs( vFrustrumEdge.GetX() );

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

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		GetCenter
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
const VECTOR &	OBB::GetCenter() const
{
	return (_Center);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
BOUNDING_VOLUME &	OBB::operator=(const BOUNDING_VOLUME &	BoundingVolume)
{
	QDT_ASSERT(BoundingVolume.GetType() == GetType());
	return (*this = static_cast<const OBB &>(BoundingVolume));
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const CONE &	Cone) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-11-28:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const SPHERE & Sphere) const
{
	VECTOR TransformedSphere = Sphere.GetCenter() - _Center;
	Float32 fRadius = Sphere.GetRadius();
	Float32 fX = MATH::Abs(TransformedSphere.Dot(_Direction1));
	Float32 fY = MATH::Abs(TransformedSphere.Dot(_Direction2));
	Float32 fZ = MATH::Abs(TransformedSphere.Dot(_Direction3));
	
	if (fX > _rDistance1 + fRadius) return (false);
	if (fY > _rDistance2 + fRadius) return (false);
	if (fZ > _rDistance3 + fRadius) return (false);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-10-29:	ELE - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const AABB &	Aabb) const
{
	QDT_NOT_IMPLEMENTED();
	return (false);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-12:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const VECTOR &	Point) const
{
	VECTOR vDiff( Point - GetCenter() );

	return ( ( MATH::Abs( vDiff.Dot( GetDirection1() ) ) < GetDistance1() ) &&
			 ( MATH::Abs( vDiff.Dot( GetDirection2() ) ) < GetDistance2() ) &&
			 ( MATH::Abs( vDiff.Dot( GetDirection3() ) ) < GetDistance3() ) );

}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	04-01-13:	ELE - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const VECTOR &	Point,
					   Float32			rEpsilon) const
{
	VECTOR vDiff( Point - GetCenter() );

	return ( ( MATH::Abs( vDiff.Dot( GetDirection1() ) ) < GetDistance1() + rEpsilon) &&
			 ( MATH::Abs( vDiff.Dot( GetDirection2() ) ) < GetDistance2() + rEpsilon) &&
			 ( MATH::Abs( vDiff.Dot( GetDirection3() ) ) < GetDistance3() + rEpsilon) );
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	02-12-16:	RMA - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(VERTICAL_RAY &	VerticalRay) const
{
//	QDT_NOT_IMPLEMENTED();
	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		Intersect
//	Object:		
//	03-01-15:	GGO - Created
//-----------------------------------------------------------------------------
bool	OBB::Intersect(const CYLINDER &	Cylinder) const
{
	QDT_NOT_IMPLEMENTED();
	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		SetFromAabb
//	Object:		
//	03-01-15:	GGO - Created
//-----------------------------------------------------------------------------
void	OBB::SetFromAabb(const AABB &	Aabb)
{
	_Direction1.SetXYZ( 1.0f, 0.0f, 0.0f );
	_Direction2.SetXYZ( 0.0f, 1.0f, 0.0f );
	_Direction3.SetXYZ( 0.0f, 0.0f, 1.0f );

	_Center = Aabb.GetCenter();

	_rDistance1 = 0.5f * ( Aabb.GetXMax() - Aabb.GetXMin() );
	_rDistance2 = 0.5f * ( Aabb.GetYMax() - Aabb.GetYMin() );
	_rDistance3 = 0.5f * ( Aabb.GetZMax() - Aabb.GetZMin() );

	QDT_ASSERT( _rDistance1 >= 0.0f );
	QDT_ASSERT( _rDistance2 >= 0.0f );
	QDT_ASSERT( _rDistance3 >= 0.0f );

	_Axe1 = _rDistance1 * _Direction1;
	_Axe2 = _rDistance2 * _Direction2;
	_Axe3 = _rDistance3 * _Direction3;
}

//-----------------------------------------------------------------------------
//	Name:		GetOptimalFocalAngle
//	Object:		
//	03-04-16:	ELE - Created
//-----------------------------------------------------------------------------
Float32	OBB::GetOptimalFocalAngle(const VECTOR &	Pos,
								  Float32				rAngle,
								  Float32 &			rNear,
								  Float32 &			rIntersectNear,
								  const VECTOR &	FrontDir) const
{
	int		i, nMinPoint;
	Float32	rDistance, rMinDistance, rDot, rOpeningAngle, rDiffAngle, rCos, rMinCos = 1.0f;
	VECTOR	Points[8];
	VECTOR	V[8];
	VECTOR	Vc;

	Vc.SetFromPoints(Pos, _Center);
	rIntersectNear = Vc.Normalize();

	GetPoints(Points);

	nMinPoint = 0;
	V[nMinPoint].SetFromPoints(Pos, Points[nMinPoint]);
	rMinDistance = V[nMinPoint].Normalize();

	for	(i = 1 ; i < 8 ; ++i)
	{
		V[i].SetFromPoints(Pos, Points[i]);
		rDistance = V[i].Normalize();

		if	(rDistance < rMinDistance)
		{
			rMinDistance = rDistance;
			nMinPoint = i;
		}
	}

	if	(Vc.Dot(V[nMinPoint]) < 0.0f)
	{
		return	(0.0f);
	}

	rNear = rMinDistance * FrontDir.Dot(V[nMinPoint]);
	rDot = FrontDir.Dot(Vc);

	for	(i = 0 ; i < 8 ; ++i)
	{
		if	(i != nMinPoint)
		{
			rCos = V[i].Dot(FrontDir);

			if	(rCos < rMinCos)
			{
				rMinCos = rCos;
			}
		}
	}

	rOpeningAngle = MATH::ACos(rMinCos);
	rDiffAngle = MATH::ACos(rDot);

	if	(rAngle == 0.0f)
	{
		return	((rOpeningAngle + rDiffAngle) * 2.0f);
	}
	else
	{
		if	(rAngle + rOpeningAngle > rDiffAngle)
		{
			return	((rOpeningAngle + rDiffAngle) * 2.0f);
		}
		else
		{
			return	(0.0f);
		}
	}
}

//=============================================================================
//	CODE ENDS HERE
//=============================================================================

