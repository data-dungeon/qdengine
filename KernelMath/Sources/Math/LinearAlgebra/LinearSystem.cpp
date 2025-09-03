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
// CLASS: LINEAR_SYSTEM
//
//
//	01-03-20:	GGO - Created
//*****************************************************************************

//*****************************************************************************
//
// CLASS DESCRIPTION (GAUSS_ELIM)
//
//	The LINEAR_SYSTEM class is used to solve linear system, such as M.x = b,
//	where M is a matrix, x is a vector whose coeffs are solutions of the system,
//	and b another vector.
//	The system is supposed to be solvable (ie. it has a unique solution).
//	If it's not the case, unpredictible results may happen (especially division
//	by zero).
//
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

#include	"Root.h"
#include	INCL_KMATH(Math/LinearAlgebra/LinearSystem)
#include 	INCL_KMATH(Math/Math)

//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM constructor
//	Object: Create a default linear system set to identity (the the matrix is 
//	identity and the vector's coeffs are all set to 1).
//	01-03-21:	GGO - Created
//-----------------------------------------------------------------------------
LINEAR_SYSTEM::LINEAR_SYSTEM( int	nSystemOrder )
{
	int i, j;


	// Matrix coeffs memory allocation.
	_pprMatrix = new float*[nSystemOrder];
	for ( i = 0; i < nSystemOrder; i++)
	{
		_pprMatrix[i] = new float[nSystemOrder];
	}


	// Vector coeffs memory allocation.
	_prVector = new float[nSystemOrder];


	// The coeffs of the matrix are first set to 0.
	for ( i = 0; i < nSystemOrder; i++ )
	{
		for ( j = 0; j < nSystemOrder; j++ )
		{
			_pprMatrix[ i ][ j ] = 0.0f;
		}
	}

	
	// Then the diagonal coeffs are set to 1.
	for ( i = 0; i < nSystemOrder; i++ )
	{
		_pprMatrix[ i ][ i ] = 1.0f;
	}


	// The vector coeffs are set to 1.
	for ( i = 0; i < nSystemOrder; i++ )
	{
		_prVector[ i ] = 1.0f;
	}


	_nSystemOrder = nSystemOrder;
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM constructor
//	Object: 
//	01-03-20:	GGO - Created
//-----------------------------------------------------------------------------
LINEAR_SYSTEM::LINEAR_SYSTEM( float **	pprMatrix,
						      float *	prVector,
						      int		nSystemOrder )
{
	int i, j;


	// Matrix coeffs memory allocation.
	_pprMatrix = new float*[nSystemOrder];
	for ( i = 0; i < nSystemOrder; i++)
	{
		_pprMatrix[i] = new float[nSystemOrder];
	}


	// Vector coeffs memory allocation.
	_prVector = new float[nSystemOrder];


	// Copy of the matrix coeffs.
	for ( i = 0; i < nSystemOrder; i++ )
	{
		for ( j = 0; j < nSystemOrder; j++ )
		{
			_pprMatrix[i][j] = pprMatrix[i][j];
		}
	}


	// Copy of the vector coeffs.
	for (i = 0; i < nSystemOrder; i++)
	{
		_prVector[i] = prVector[i];
	}


	// The order of the system
	_nSystemOrder = nSystemOrder;
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM destructor
//	Object:	Destructor.
//	01-03-20:	GGO - Created
//-----------------------------------------------------------------------------
LINEAR_SYSTEM::~LINEAR_SYSTEM()
{
	int i;


	// Free the matrix.
	for( i = 0; i < _nSystemOrder; i++)
	{
		delete[] _pprMatrix[i];
	}
	delete[] _pprMatrix;


	// Free the vector.
	delete[] _prVector;
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM constructor
//	Object:	Unused constructor because private.
//	01-03-20:	GGO - Created
//-----------------------------------------------------------------------------
LINEAR_SYSTEM::LINEAR_SYSTEM()
{
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::SetMatrix
//	Object: Set the coeffs a the system matrix.
//	01-03-21:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::SetMatrix( float **	pprMatrix )
{
	// Copy of the matrix coeffs.
	for ( int i = 0; i < _nSystemOrder; i++ )
	{
		for ( int j = 0; j < _nSystemOrder; j++ )
		{
			_pprMatrix[i][j] = pprMatrix[i][j];
		}
	}
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::SetVecTor
//	Object: 
//	01-03-21:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::SetVecTor( float *	prVector )
{
	// Copy of the vector coeffs.
	for ( int i = 0; i < _nSystemOrder; i++ )
	{
		_prVector[i] = prVector[i];
	}
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::SetCoeffAt
//	Object: 
//	01-03-21:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::SetMatrixCoeff( int		nLin,
									   int		nCol,
									   float	rCoeff )
{
	QDT_ASSERT( ( nLin >= 0 ) && ( nLin < _nSystemOrder ) );
	QDT_ASSERT( ( nCol >= 0 ) && ( nCol < _nSystemOrder ) );

	_pprMatrix[ nLin ][ nCol ] = rCoeff;
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::SetVectorCoeff
//	Object: 
//	01-03-21:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::SetVectorCoeff( int		nLin,
									   float	rCoeff )
{
	QDT_ASSERT( ( nLin >= 0 ) && ( nLin < _nSystemOrder ) );

	_prVector[ nLin ] = rCoeff;
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::Solve
//	Object: Solve the system and put the solution into 'prSolution'.
//	01-03-21:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::Solve(	float *	prSolution, 
								bool	bNoDet )
{
	if	( bNoDet == false )
	{
		QDT_ASSERT( MATH::Abs( Det() ) > 0.001f );
	}


	// First we transform the system to an upper triangle matrix.
	Factorization();


	// And we back substitute to solve the system.
	BackSubstitution( prSolution );
}


//-----------------------------------------------------------------------------
//	Name: GAUSS_ELIM::PermuteLines
//	Object: 
//	01-03-20:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::PermuteLines( int	nIndex1stLine,
								     int	nIndex2ndLine )
{
	// Return if both index are the same.
	if ( nIndex1stLine == nIndex2ndLine )
	{
		return;
	}


	// Locals.
	int		i;
	float*	pLineTemp;


	// Copy of the first line into a temp buffer.
	pLineTemp = new float[ _nSystemOrder ];
	for ( i = 0; i < _nSystemOrder; i++)
	{
		pLineTemp[ i ] = _pprMatrix[ nIndex1stLine ][ i ];
	}


	// Copy of the second line into the first one.
	for ( i = 0; i < _nSystemOrder; i++)
	{
		_pprMatrix[ nIndex1stLine ][ i ] = _pprMatrix[ nIndex2ndLine ][ i ];
	}


	// Copy of the temp buffer into the second line.
	for ( i = 0; i < _nSystemOrder; i++)
	{
		_pprMatrix[ nIndex2ndLine ][ i ] = pLineTemp[ i ];
	}


	delete[] pLineTemp;


	// And we swap the vector coeffs too.
	MATH::Swap( _prVector[ nIndex1stLine ], _prVector[ nIndex2ndLine ] );
}


//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::Factorization
//	Object: Reduction of the system to Upper Triangular.
//	01-03-20:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::Factorization()
{
	int		i, j, k;
	int		nIndexPivotLine = 0;
	float	rMax, rInvMii;


	for( i = 0; i < _nSystemOrder; i++)
	{
		// We first look for the pivot.
		rMax = 0.0f;
		for ( j = i; j < _nSystemOrder; j++)
		{
			if ( MATH::Abs( _pprMatrix[ j ][ i ] ) > rMax )
			{
				rMax = MATH::Abs( _pprMatrix[ j ][ i ] );
				nIndexPivotLine = j;
			}
		}


		// Then we swap the lines
		PermuteLines( i, nIndexPivotLine );


		// Opti.
		rInvMii = 1.0f / _pprMatrix[ i ][ i ];


		// Reduce the lower lines.
		for ( j = i + 1; j < _nSystemOrder; j++ )
		{
			// Is '_pprMatrix[j][i]' null? If so, there no need to reduce the reduce line
			// since a zero already is at the right place.
			if ( _pprMatrix[ j ][ i ] != 0.0f )
			{
				// The matrix coefs.
				for ( k = i + 1; k < _nSystemOrder; k++ )
				{		
					if ( _pprMatrix[ j ][ k ] == 0.0f )
					{
						_pprMatrix[ j ][ k ] =  -_pprMatrix[ i ][ k ] * _pprMatrix[ j ][ i ];
					}
					else
					{
						_pprMatrix[ j ][ k ]  = _pprMatrix[ i ][ i ] * _pprMatrix[ j ][ k ]
											  - _pprMatrix[ i ][ k ] * _pprMatrix[ j ][ i ];
					}
					_pprMatrix[ j ][ k ] *=  rInvMii;
				}

				// The vector coef.
				_prVector[ j ]  = _pprMatrix[ i ][ i ] * _prVector[ j ]
							    - _pprMatrix[ j ][ i ] * _prVector[ i ];
				_prVector[ j ] *=  rInvMii;


				// Zeros under the diagonal.
				_pprMatrix[ j ][ i ] = 0.0f;
			}
		}
	}
}



//-----------------------------------------------------------------------------
//	Name: LINEAR_SYSTEM::BackSubstitution
//	Object: Solving backwards through the Upper Triangular Form.
//	01-03-20:	GGO - Created
//-----------------------------------------------------------------------------
void	LINEAR_SYSTEM::BackSubstitution(float *	prSolution)
{
	int i, j;


	for ( i = _nSystemOrder - 1; i >= 0; i-- )
	{
		prSolution[ i ] = 0.0f;
		for ( j = i + 1; j < _nSystemOrder; j++ )
		{
			prSolution[ i ] -= _pprMatrix[ i ][ j ] * prSolution[ j ];
		}
		prSolution[ i ] += _prVector[ i ];
		prSolution[ i ] /= _pprMatrix[ i ][ i ];
	}
}


//-----------------------------------------------------------------------------
//	Name:		DetAux
//	Object:		
//	01-07-19:	GGO - Created
//-----------------------------------------------------------------------------
float	LINEAR_SYSTEM::DetAux(int		nSize,
							  float * *	pprCoefs)
{
	int i, j, y;
	QDT_ASSERT( nSize >= 2 );


	// If the matrix size is 2, return the det.
	if ( nSize == 2 )
	{
		return ( pprCoefs[ 0 ][ 0 ] * pprCoefs[ 1 ][ 1 ] - pprCoefs[ 1 ][ 0 ] * pprCoefs[ 0 ][ 1 ] );
	}
	else
	// Else recurse.
	{
		float rReturnDet = 0.0f;
		float rDetTmp;

		// Memory allocation.
		float **pprSubCoeffs = new float*[ nSize - 1 ];
		for ( i = 0; i < nSize - 1; i++ )
		{
			pprSubCoeffs[ i ] = new float[ nSize - 1 ];
		}

		// We compute the det on the 1st line.
		for ( y = 0; y < nSize; ++y )
		{
			// Fill the coefs.
			for ( i = 0; i < nSize - 1; ++i )
			{
				for ( j = 0; j < nSize - 1; ++j )
				{
					pprSubCoeffs[ i ][ j ] = pprCoefs[ i + 1 ][ ( j < y ? j : j + 1 ) ];
				}
			}


			rDetTmp  = DetAux( nSize - 1, pprSubCoeffs );
			rDetTmp *= pprCoefs[ 0 ][ y ];
			if ( y % 2 == 1 )
			{
				rDetTmp *= -1.0f;
			}
			rReturnDet += rDetTmp;
		}

		// Free memory.
		for ( i = 0; i < nSize - 1; i++ )
		{
			delete[] pprSubCoeffs[ i ];
		}
		delete[] pprSubCoeffs;


		return ( rReturnDet );
	}
}


//-----------------------------------------------------------------------------
//	Name:		Det
//	Object:		
//	01-07-19:	GGO - Created
//-----------------------------------------------------------------------------
float	LINEAR_SYSTEM::Det()
{
	return ( DetAux( _nSystemOrder, _pprMatrix ) );
}




//=============================================================================
// CODE ENDS HERE
//=============================================================================
