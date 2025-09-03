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
//	This class is only usefull for the construction of an Oriented Bounding
//	Box (OBB).
//
//	01-02-27:	GGO - Created
//*****************************************************************************


//=============================================================================
// Name: COVARIANCE_MATRIX
//	01-02-27:	GGO - Created
//=============================================================================

#ifndef __KMATH_MATH_LINEARALGEBRA_COVARIANCEMATRIX_H__
#define __KMATH_MATH_LINEARALGEBRA_COVARIANCEMATRIX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include	INCL_KMATH(Math/Geometry/Shapes/Shapes1D/Vector/Vector)

namespace	QDT
{
	namespace KMATH
	{
		class COVARIANCE_MATRIX
		{
		public:

			/** Create a covariance matrix of a set of points. */
			COVARIANCE_MATRIX(int nNbrPoints, VECTOR *pPoints);

			/** Return the normalized eigenvectors of a covariance matrix. */
			void	GetEigenVectors(VECTOR& EigenVector1, VECTOR& EigenVector2, VECTOR& EigenVector3);

			bool	GetMainDirection(float rEpsilon, VECTOR &DirectionOut);

			bool	IsIdentity(float rEpsilon = 0.0001f) const;
			bool	IsDiagonal(float rEpsilon = 0.0001f) const;
			bool	IsDegenerated(float rEpsilon = 0.0001f) const;
			float	GetDet() const;
			void	Normalize();
			bool	IsNull(float rEpsilon = 0.0001f) const;

			/** Return the eigenvalues of the covariance matrix and their multiplicity. */
			void	GetEigenValues(float& rEigenValue1, float& rEigenValue2, float& rEigenValue3);


		private:

			float	_m00;
			float	_m01;
			float	_m02;
			float	_m11;
			float	_m12;
			float	_m22;

			/** The constructor is private so that a covariance matrix can only be build from a set of points. */
			COVARIANCE_MATRIX();

			/** Used to find the eigenvalues of the covariance matrix. */
			void	FindRootsDegree3Polynom( float   a,  float   b,  float   c,  float   d, 
											float & r1, float & r2, float & r3 );

			/** Used to find an eigenvector. */
			void	Solve2x3LinearSystem(float pCoefs[5], VECTOR &SolutionVector);

			/** Used to find the eigenvectors of an empty matrix. */
			void	FindEigenVectors2x2(float pCoefs[3], float &rEigenVectorX, float &rEigenVectorY);

			/** Used to find the eignevectors when there is a double eigenvalue. */
			void	FindOrthogonalVector(const VECTOR &Vector, VECTOR &OrthoVector);

			/** Used to find the initialization vector of the GetMainDirection algorithm. */
			VECTOR	GetInitVector() const;
		};
	}
}

#endif //__KMATH_MATH_LINEARALGEBRA_COVARIANCEMATRIX_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
