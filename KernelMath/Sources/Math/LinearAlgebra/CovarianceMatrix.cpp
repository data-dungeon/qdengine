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
// CLASS: COVARIANCE_MATRIX
//
//
//	01-02-27:	GGO - Created
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include 	INCL_KMATH(Math/LinearAlgebra/CovarianceMatrix)
#include	INCL_KMATH(Math/Math)
//#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

//-----------------------------------------------------------------------------
//	Name: COVARIANCE_MATRIX constructor
//	Object: 
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
COVARIANCE_MATRIX::COVARIANCE_MATRIX()
{
}


//-----------------------------------------------------------------------------
//	Name: COVARIANCE_MATRIX constructor
//	Object:	Create a covariance matrix of a set of points.
//	01-02-27:	GGO - Created
//-----------------------------------------------------------------------------
COVARIANCE_MATRIX::COVARIANCE_MATRIX(int nNbrPoints, VECTOR *pPoints)
{
	QDT_ASSERT( nNbrPoints > 0 );

	//locals
	int i;
	float rX_Mean = 0.0f;
	float rY_Mean = 0.0f;
	float rZ_Mean = 0.0f;


	//Set the coefs of the matrix to zero.
	_m00 = _m01 = _m02 = _m11 = _m12 = _m22 = 0.0f;


	//First we compute the mean of the X, Y and Z coordinates of the set of points.
	for (i = 0; i < nNbrPoints; i++)
	{
		rX_Mean += pPoints[i].GetX();
		rY_Mean += pPoints[i].GetY();
		rZ_Mean += pPoints[i].GetZ();
	}
	rX_Mean /= (float)nNbrPoints;
	rY_Mean /= (float)nNbrPoints;
	rZ_Mean /= (float)nNbrPoints;


	//And then we compute the coefs of the matrix.
	for (i = 0; i < nNbrPoints; i++)
	{
		_m00 += MATH::Square( pPoints[i].GetX() );
		_m11 += MATH::Square( pPoints[i].GetY() );
		_m22 += MATH::Square( pPoints[i].GetZ() );

		_m01 += pPoints[i].GetX() * pPoints[i].GetY();
		_m02 += pPoints[i].GetX() * pPoints[i].GetZ();
		_m12 += pPoints[i].GetY() * pPoints[i].GetZ();
	}

	_m00 /= (float)nNbrPoints;
	_m11 /= (float)nNbrPoints;
	_m22 /= (float)nNbrPoints;
	_m01 /= (float)nNbrPoints;
	_m02 /= (float)nNbrPoints;
	_m12 /= (float)nNbrPoints;

	_m00 -= MATH::Square( rX_Mean );
	_m11 -= MATH::Square( rY_Mean );
	_m22 -= MATH::Square( rZ_Mean );

	_m01 -= rX_Mean * rY_Mean;
	_m02 -= rX_Mean * rZ_Mean;
	_m12 -= rY_Mean * rZ_Mean;
}


//-----------------------------------------------------------------------------
//	Name: COVARIANCE_MATRIX::GetEigenValues
//	Object: 
//	01-02-28:	GGO - Created
//-----------------------------------------------------------------------------
void	COVARIANCE_MATRIX::GetEigenValues( float &	rEigenValue1,
										   float &	rEigenValue2,
										   float &	rEigenValue3 )
{
	float a = 1.0f;
	float b = -_m00 - _m11 - _m22;
	float c =  _m00 * _m11 + _m00 * _m22 + _m11 * _m22 - _m01 * _m01 - _m12 * _m12 - _m02 * _m02;
	float d = -_m00 * _m11 * _m22 - 2.0f * _m01 * _m02 * _m12 + _m00 * _m12 * _m12 + _m01 * _m01 * _m22  + _m02 * _m02 * _m11;

	FindRootsDegree3Polynom( a, b, c, d, rEigenValue1, rEigenValue2, rEigenValue3 );
}


//-----------------------------------------------------------------------------
//	Name: COVARIANCE_MATRIX::GetEigenVectors
//	Object: 
//	01-02-28:	GGO - Created
//-----------------------------------------------------------------------------
void	COVARIANCE_MATRIX::GetEigenVectors( VECTOR &	EigenVector1,
										    VECTOR &	EigenVector2,
										    VECTOR &	EigenVector3 )
{
	// "Normalize" the matrix in order to prevent numerical error due to large coefs.
	float rCoefMax = MATH::Max( MATH::Max( MATH::Max( _m00, _m01 ), MATH::Max( _m02, _m11 ) ), MATH::Max( _m12, _m22 ));
	_m00 /= rCoefMax;
	_m01 /= rCoefMax;
	_m02 /= rCoefMax;
	_m11 /= rCoefMax;
	_m12 /= rCoefMax;
	_m22 /= rCoefMax;

	// Compute epsilon depending of the coefs.
	const float rEpsilon = 0.01f;
	

	//If the matrix is diagonal, the eigenvectors are the canonic ones.
	if ( ( MATH::Abs( _m01 ) < rEpsilon ) &&
		 ( MATH::Abs( _m02 ) < rEpsilon ) &&
		 ( MATH::Abs( _m12 ) < rEpsilon ) )
	{
		EigenVector1.SetXYZ( 1.0f, 0.0f, 0.0f );
		EigenVector2.SetXYZ( 0.0f, 1.0f, 0.0f );
		EigenVector3.SetXYZ( 0.0f, 0.0f, 1.0f );
		return;
	}



	// _m01 = _m02 = 0
	if ( ( MATH::Abs( _m01 ) < rEpsilon ) && ( MATH::Abs( _m02 ) < rEpsilon ) )
	{
		EigenVector1.SetXYZ( 1.0f, 0.0f, 0.0f );
		
		float pCoefs[3];
		pCoefs[0] = _m11;
		pCoefs[1] = _m12;
		pCoefs[2] = _m22;

		float y, z;
		FindEigenVectors2x2( pCoefs, y, z );

		EigenVector2.SetXYZ( 0.0f, y, z );

		EigenVector3.Cross( EigenVector1, EigenVector2 );

		EigenVector2.Normalize();
		EigenVector3.Normalize();

		return;
	}
	
	// _m01 = _m12 = 0
	if ( ( MATH::Abs( _m01 ) < rEpsilon ) && ( MATH::Abs( _m12 ) < rEpsilon ) )
	{
		EigenVector1.SetXYZ( 0.0f, 1.0f, 0.0f );
		
		float pCoefs[3];
		pCoefs[0] = _m00;
		pCoefs[1] = _m02;
		pCoefs[2] = _m22;

		float x, z;
		FindEigenVectors2x2( pCoefs, x, z );

		EigenVector2.SetXYZ( x, 0.0f, z );

		EigenVector3.Cross( EigenVector1, EigenVector2 );

		EigenVector2.Normalize();
		EigenVector3.Normalize();

		return;
	}
	
	// _m02 = _m12 = 0
	if ( ( MATH::Abs( _m02 ) < rEpsilon ) && ( MATH::Abs( _m12 ) < rEpsilon ) )
	{
		EigenVector1.SetXYZ( 0.0f, 0.0f, 1.0f );
		
		float pCoefs[3];
		pCoefs[0] = _m00;
		pCoefs[1] = _m01;
		pCoefs[2] = _m11;

		float x, y;
		FindEigenVectors2x2( pCoefs, x, y );

		EigenVector2.SetXYZ( x, y, 0.0f );

		EigenVector3.Cross( EigenVector1, EigenVector2 );

		EigenVector2.Normalize();
		EigenVector3.Normalize();

		return;
	}


	// The eigenvalues of the matrix.
	float rEigenValue1, rEigenValue2, rEigenValue3;
	this->GetEigenValues( rEigenValue1, rEigenValue2, rEigenValue3 );

 
	//== The three eigenvalues are the same, so we choose the eigenvectors as the canonic ones
	//   (but any set of three orthonormal vectors would be right too). ==
	if ( ( MATH::Abs( rEigenValue1 - rEigenValue2 ) < rEpsilon ) &&
		 ( MATH::Abs( rEigenValue1 - rEigenValue3 ) < rEpsilon ) &&
		 ( MATH::Abs( rEigenValue2 - rEigenValue3 ) < rEpsilon ) )
	{
		EigenVector1.SetXYZ( 1.0f, 0.0f, 0.0f );
		EigenVector2.SetXYZ( 0.0f, 1.0f, 0.0f );
		EigenVector3.SetXYZ( 0.0f, 0.0f, 1.0f );
		return;
	}
	//==


	//== There is a double eigenvalue, so we look for the eigenvector corresponding to the single
	//   eigenvalue, and the two others eigenvectors are orthogonal to the first. ==

	// e1 = e2
	if ( MATH::Abs( rEigenValue1 - rEigenValue2 ) < rEpsilon )
	{
		float pCoefs[5];
		pCoefs[0] = _m00 - rEigenValue3;
		pCoefs[1] = _m01;
		pCoefs[2] = _m02;
		pCoefs[3] = _m11 - rEigenValue3;
		pCoefs[4] = _m12;

		Solve2x3LinearSystem( pCoefs, EigenVector1 );

		FindOrthogonalVector( EigenVector1, EigenVector2 );

		EigenVector3.Cross( EigenVector1, EigenVector2 );

		EigenVector1.Normalize();
		EigenVector2.Normalize();
		EigenVector3.Normalize();

		return;
	}

	// e2 = e3
	if ( MATH::Abs( rEigenValue2 - rEigenValue3 ) < rEpsilon )
	{
		float pCoefs[5];
		pCoefs[0] = _m00 - rEigenValue1;
		pCoefs[1] = _m01;
		pCoefs[2] = _m02;
		pCoefs[3] = _m11 - rEigenValue1;
		pCoefs[4] = _m12;

		Solve2x3LinearSystem( pCoefs, EigenVector1 );

		FindOrthogonalVector( EigenVector1, EigenVector2 );

		EigenVector3.Cross( EigenVector1, EigenVector2 );

		EigenVector1.Normalize();
		EigenVector2.Normalize();
		EigenVector3.Normalize();

		return;
	}

	// e1 = e3
	if ( MATH::Abs( rEigenValue1 - rEigenValue3 ) < rEpsilon )
	{
		float pCoefs[5];
		pCoefs[0] = _m00 - rEigenValue2;
		pCoefs[1] = _m01;
		pCoefs[2] = _m02;
		pCoefs[3] = _m11 - rEigenValue2;
		pCoefs[4] = _m12;

		Solve2x3LinearSystem( pCoefs, EigenVector1 );

		FindOrthogonalVector( EigenVector1, EigenVector2 );

		EigenVector3.Cross( EigenVector1, EigenVector2 );

		EigenVector1.Normalize();
		EigenVector2.Normalize();
		EigenVector3.Normalize();

		return;
	}
	//==


	//== And finally the general case ==
	float rEigenValueMin, rEigenValueMax;

	rEigenValueMin = MATH::Min( rEigenValue1,   rEigenValue2 );
	rEigenValueMin = MATH::Min( rEigenValueMin, rEigenValue3 );

	rEigenValueMax = MATH::Max( rEigenValue1,   rEigenValue2 );
	rEigenValueMax = MATH::Max( rEigenValueMax, rEigenValue3 );


	//== The eigenvector corresponding to the min eigenvalue. ==
	float pCoefs[5];
	pCoefs[0] = _m00 - rEigenValueMin;
	pCoefs[1] = _m01;
	pCoefs[2] = _m02;
	pCoefs[3] = _m11 - rEigenValueMin;
	pCoefs[4] = _m12;

	Solve2x3LinearSystem( pCoefs, EigenVector1 );
	//==


	//== The eigenvector corresponding to the max eigenvalue. ==
	pCoefs[0] = _m00 - rEigenValueMax;
	pCoefs[1] = _m01;
	pCoefs[2] = _m02;
	pCoefs[3] = _m11 - rEigenValueMax;
	pCoefs[4] = _m12;

	Solve2x3LinearSystem( pCoefs, EigenVector2 );
	//==


	EigenVector3.Cross( EigenVector1, EigenVector2 );

	EigenVector1.Normalize();
	EigenVector2.Normalize();
	EigenVector3.Normalize();
	//====
}



//-----------------------------------------------------------------------------
//	Name: COVARIANCE_MATRIX::FindEigenVector2x2
//	Object: Find the eignevectors of a 2x2 symetric matrix.
//			We need only to find a single vector, since the orther one is orthogonal.
//	01-03-01:	GGO - Created
//-----------------------------------------------------------------------------
void COVARIANCE_MATRIX::FindEigenVectors2x2( float pCoefs[3],
										 	 float & rEigenVectorX,
											 float & rEigenVectorY )
{
	float a = pCoefs[0];
	float b = pCoefs[1];
	float c = pCoefs[2];

	//the eigenvalues are the roots of: x² +x(-a-c) + ac-b² =0
	float det = ( a + c ) * ( a + c ) - 4.0f * ( a * c - b * b );

	//Theoretically, the determinant should not be null, because this would mean
	//that the covariance matrix is diagonal, and this case is already handled.
	//But for safety, we handle this case anyway.
	if ( MATH::Abs( det ) < 0.0001f )
	{
		rEigenVectorX = 1.0f;
		rEigenVectorY = 0.0f;

		return;
	}

	float rEigenValue = 0.5f * ( a + c + MATH::Sqrt( det ) );

	rEigenVectorX = 1.0f;
	rEigenVectorY = - ( a - rEigenValue ) / b;
}



//-----------------------------------------------------------------------------
//	Name: FindRootsDegree3Polynom
//	Object: Find the roots a polynom of degree 3 such as: ax^3 + bx² + cx + d = 0
//			This method works only if the polynom has real solutions.
//			Else an ASSERT will occur.
//	01-02-28:	GGO - Created
//-----------------------------------------------------------------------------
void	COVARIANCE_MATRIX::FindRootsDegree3Polynom( float   a,  float   b,  float   c,  float   d, 
													float & r1, float & r2, float & r3 )
{
	QDT_ASSERT( MATH::Abs( a ) > MATH::EPSILON() );

	float A = b / a;
	float B = c / a;
	float C = d / a;

	float Q = ( A * A - 3.0f * B ) / 9.0f;
	float R = ( 2.0f * A * A * A - 9.0f * A * B + 27 * C ) / 54.0f;
	float rTemp =  R / MATH::Sqrt( Q * Q * Q );
	float Theta = MATH::ACos( rTemp );

	r1 = - 2.0f * MATH::Sqrt( Q ) * MATH::Cos( Theta / 3.0f ) - A / 3.0f;
	r2 = - 2.0f * MATH::Sqrt( Q ) * MATH::Cos( ( Theta + 2.0f * MATH::PI() ) / 3.0f ) - A / 3.0f;
	r3 = - 2.0f * MATH::Sqrt( Q ) * MATH::Cos( ( Theta - 2.0f * MATH::PI() ) / 3.0f ) - A / 3.0f;
}


//-----------------------------------------------------------------------------
//	Name: Solve2x3LinearSystem
//	Object: Solve a 2x3 symetric linear system in order to find
//	the eigenvectors of the covariance matrix.
//	The system is like that:	pCoefs[0].x + pCoefs[1].y + pCoefs[2].z = 0
//								pCoefs[1].x + pCoefs[4].y + pCoefs[5].z = 0
//	01-02-28:	GGO - Created
//-----------------------------------------------------------------------------
void COVARIANCE_MATRIX::Solve2x3LinearSystem( float pCoefs[5], VECTOR & SolutionVector )
{
	float a = pCoefs[0];
	float b = pCoefs[1];
	float c = pCoefs[2];
	float d = pCoefs[3];
	float e = pCoefs[4];

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	if ( ( MATH::Abs( a ) + MATH::Abs( b ) >= MATH::Abs( b ) + MATH::Abs( d ) ) &&
		 ( MATH::Abs( a ) + MATH::Abs( b ) >= MATH::Abs( c ) + MATH::Abs( e ) ) )
	{
		if ( MATH::Abs( b * b - a * d ) > MATH::Abs( b * c - a * e ) )
		{
			y = -( b * c - a * e ) / ( b * b - a * d );
			z = 1.0f;
		}
		else
		{
			z = -( b * b - a * d ) / ( b * c - a * e );
			y = 1.0f;
		}

		if ( MATH::Abs( a ) > MATH::Abs( b ) )
		{
			x = -( b * y + c * z ) / a;
		}
		else
		{
			x = -( d * y + e * z ) / b;
		}
	}
	else
	if ( ( MATH::Abs( b ) + MATH::Abs( d ) >= MATH::Abs( a ) + MATH::Abs( b ) ) &&
		 ( MATH::Abs( b ) + MATH::Abs( d ) >= MATH::Abs( c ) + MATH::Abs( e ) ) )
	{
		if ( MATH::Abs( b * b - a * d ) > MATH::Abs( c * d - b * e ) )
		{
			x = -( c * d - b * e ) / ( a * d - b * b );
			z = 1.0f;
		}
		else
		{
			z = -( a * d - b * b ) / ( c * d - b * e );
			x = 1.0f;
		}

		if ( MATH::Abs( b ) > MATH::Abs( d ) )
		{
			y = -( a * x + c * z ) / b;
		}
		else
		{
			y = -( b * x + e * z ) / d;
		}
	}
	else
	if ( ( MATH::Abs( c ) + MATH::Abs( e ) >= MATH::Abs( a ) + MATH::Abs( b ) ) &&
		 ( MATH::Abs( c ) + MATH::Abs( e ) >= MATH::Abs( b ) + MATH::Abs( d ) ) )
	{
		if ( MATH::Abs( a * e - b * c ) > MATH::Abs( b * e - c * d ) )
		{
			x = -( b * d - c * d ) / ( a * e - b * c );
			y = 1.0f;
		}
		else
		{
			y = -( a * e - b * c ) / ( b * e - c * d );
			x = 1.0f;
		}

		if ( MATH::Abs( c ) > MATH::Abs( a ) )
		{
			z = -( a * x + b * y ) / c;
		}
		else
		{
			z = -( b * x - d * y ) / e;
		}
	}
	else
	{
		QDT_FAIL();
	}

	SolutionVector.SetX( x );
	SolutionVector.SetY( y );
	SolutionVector.SetZ( z );
}


//-----------------------------------------------------------------------------
//	Name: FindOrthogonalVectors
//	Object: Find a vector orthogonal to another vector.
//	01-02-28:	GGO - Created
//-----------------------------------------------------------------------------
void COVARIANCE_MATRIX::FindOrthogonalVector( const VECTOR &	Vector,
											  VECTOR &			OrthoVector )
{
	if ( MATH::Abs( Vector.GetY() ) >= MATH::Abs( Vector.GetX() ) &&
	     MATH::Abs( Vector.GetZ() ) >= MATH::Abs( Vector.GetX() ) )
	{
		OrthoVector.SetX( 0.0f );
		OrthoVector.SetY(  Vector.GetZ() );
		OrthoVector.SetZ( -Vector.GetY() );

		return;
	}


	if ( MATH::Abs( Vector.GetX() ) >= MATH::Abs( Vector.GetY() ) &&
	     MATH::Abs( Vector.GetZ() ) >= MATH::Abs( Vector.GetY() ) )
	{
		OrthoVector.SetX( -Vector.GetZ() );
		OrthoVector.SetY(  0.0f );
		OrthoVector.SetZ(  Vector.GetX() );

		return;
	}


	if ( MATH::Abs( Vector.GetX() ) >= MATH::Abs( Vector.GetZ() ) &&
	     MATH::Abs( Vector.GetY() ) >= MATH::Abs( Vector.GetZ() ) )
	{
		OrthoVector.SetX(  Vector.GetY() );
		OrthoVector.SetY( -Vector.GetX() );
		OrthoVector.SetZ(  0.0f );

		return;
	}

	QDT_FAIL();
}



//-----------------------------------------------------------------------------
//	Name:		GetMainDirection
//	Object:		
//	01-10-08:	GGO - Created
//	02-09-04:	RMA - Found special case whith infinite boucle
//					  I patch before to redo everything later... JLT check the
//					  & it seems to be wrong because special cases.
//-----------------------------------------------------------------------------
bool	COVARIANCE_MATRIX::GetMainDirection(float	rEpsilon,
											VECTOR &DirectionOut)
{
	VECTOR	vMainDirection;
	VECTOR	vMainDirectionTmp(VECTOR::ZERO);
	VECTOR	vSum;
	float	rScal;
	
	vMainDirection = GetInitVector();

	int nNbrIterations = 0;

	while ( ( MATH::EqualsEpsilon( vMainDirection.GetX(), vMainDirectionTmp.GetX(), rEpsilon ) == false ) ||
			( MATH::EqualsEpsilon( vMainDirection.GetY(), vMainDirectionTmp.GetY(), rEpsilon ) == false ) ||
			( MATH::EqualsEpsilon( vMainDirection.GetZ(), vMainDirectionTmp.GetZ(), rEpsilon ) == false ) )
	{
		++nNbrIterations;

		if (nNbrIterations > 1000)
		{
			return (false);
		}

		vMainDirectionTmp = vMainDirection;

		vMainDirection.SetX( _m00 * vMainDirectionTmp.GetX() +
						     _m01 * vMainDirectionTmp.GetY() +
						     _m02 * vMainDirectionTmp.GetZ() );
						     
		vMainDirection.SetY( _m01 * vMainDirectionTmp.GetX() +
						     _m11 * vMainDirectionTmp.GetY() +
						     _m12 * vMainDirectionTmp.GetZ() );

		vMainDirection.SetZ( _m02 * vMainDirectionTmp.GetX() +
						     _m12 * vMainDirectionTmp.GetY() +
						     _m22 * vMainDirectionTmp.GetZ() );

		vMainDirection.Normalize();
		
		// Opposite vectors.
		vSum	= vMainDirection + vMainDirectionTmp;
		rScal	= vSum.Dot( vSum );
		if ( MATH::Abs( rScal ) <= MATH::EPSILON() )
		{
			vMainDirection.Neg();
		}
	}

	QDT_ASSERT( vMainDirection.IsNormalized() == true );

	DirectionOut = vMainDirection;

	return (true);
}


//-----------------------------------------------------------------------------
//	Name:		GetInitVector
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
VECTOR	COVARIANCE_MATRIX::GetInitVector() const
{
	float	rNorm0, rNorm1, rNorm2;
	VECTOR	InitVector;

	rNorm0 = MATH::Abs( _m00 ) + MATH::Abs( _m01 ) + MATH::Abs( _m02 );
	rNorm1 = MATH::Abs( _m01 ) + MATH::Abs( _m11 ) + MATH::Abs( _m12 );
	rNorm2 = MATH::Abs( _m02 ) + MATH::Abs( _m12 ) + MATH::Abs( _m22 );

	if ( ( rNorm0 >= rNorm1 ) && ( rNorm0 >= rNorm2 ) )
	{
		InitVector.SetXYZ( _m00, _m01, _m02 );
	}
	else
	if ( ( rNorm1 >= rNorm0 ) && ( rNorm1 >= rNorm2 ) )
	{
		InitVector.SetXYZ( _m01, _m11, _m12 );
	}
	else
	{
		QDT_ASSERT( rNorm2 >= rNorm0 );
		QDT_ASSERT( rNorm2 >= rNorm1 );

		InitVector.SetXYZ( _m02, _m12, _m22 );
	}

	return ( InitVector );
}


//-----------------------------------------------------------------------------
//	Name:		IsDiagonal
//	Object:		
//	01-11-28:	AMA - Created
//-----------------------------------------------------------------------------
bool	COVARIANCE_MATRIX::IsDiagonal(float	rEpsilon) const
{
	return ( ( MATH::EqualsEpsilon( _m01, 0.0f, rEpsilon ) == true ) &&
			 ( MATH::EqualsEpsilon( _m02, 0.0f, rEpsilon ) == true ) &&
			 ( MATH::EqualsEpsilon( _m12, 0.0f, rEpsilon ) == true ) );
}


//-----------------------------------------------------------------------------
//	Name:		Normalize
//	Object:		
//	01-11-28:	AMA - Created
//-----------------------------------------------------------------------------
void	COVARIANCE_MATRIX::Normalize()
{
	float	rMax = MATH::Max( _m00, MATH::Max( _m01, MATH::Max( _m02, MATH::Max( _m11, MATH::Max( _m12, _m22 ) ) ) ) );
	float	rInvMax;

	if ( MATH::EqualsEpsilon( rMax, 0.0f ) == true )
	{
		return;
	}

	rInvMax = 1.0f / rMax;


	_m00 *= rInvMax;
	_m01 *= rInvMax;
	_m02 *= rInvMax;
	_m11 *= rInvMax;
	_m12 *= rInvMax;
	_m22 *= rInvMax;
}

//-----------------------------------------------------------------------------
//	Name:		GetDet
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
float	COVARIANCE_MATRIX::GetDet() const
{
	float	rDet;

	rDet = _m00 * _m11 * _m22 + 2 * _m01 * _m02 * _m12
		  - _m00 * _m12 * _m12
		  - _m11 * _m02 * _m02
		  - _m22 * _m01 * _m01;

	return ( rDet );
}

//-----------------------------------------------------------------------------
//	Name:		IsDegenerated
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
bool	COVARIANCE_MATRIX::IsDegenerated(float	rEpsilon) const
{
	return ( MATH::Abs( GetDet() ) < rEpsilon );
}

//-----------------------------------------------------------------------------
//	Name:		IsIdentity
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
bool	COVARIANCE_MATRIX::IsIdentity(float	rEpsilon) const
{
	return ( ( MATH::Abs( _m01 ) < rEpsilon ) &&
			 ( MATH::Abs( _m02 ) < rEpsilon ) &&
			 ( MATH::Abs( _m12 ) < rEpsilon ) &&
			 ( MATH::Abs( _m00 - 1.0f ) < rEpsilon ) &&
			 ( MATH::Abs( _m11 - 1.0f ) < rEpsilon ) &&
			 ( MATH::Abs( _m22 - 1.0f ) < rEpsilon ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsNull
//	Object:		
//	02-04-18:	GGO - Created
//-----------------------------------------------------------------------------
bool	COVARIANCE_MATRIX::IsNull(float	rEpsilon) const
{
	return ( ( MATH::Abs( _m01 ) < rEpsilon ) &&
			 ( MATH::Abs( _m02 ) < rEpsilon ) &&
			 ( MATH::Abs( _m12 ) < rEpsilon ) &&
			 ( MATH::Abs( _m00 ) < rEpsilon ) &&
			 ( MATH::Abs( _m11 ) < rEpsilon ) &&
			 ( MATH::Abs( _m22 ) < rEpsilon ) );
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================

