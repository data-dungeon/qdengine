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


//=============================================================================
// Name: LINEAR_SYSTEM
//	01-03-20:	GGO - Created
//=============================================================================

#ifndef __KMATH_MATH_LINEARALGEBRA_LINEARSYSTEM_H__
#define __KMATH_MATH_LINEARALGEBRA_LINEARSYSTEM_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace	QDT
{
	namespace KMATH
	{
		class LINEAR_SYSTEM
		{
		public:
			/** Create a default linear system. */
			LINEAR_SYSTEM(int nSystemOrder);
			/** Create a linear system with its coeffs. */
			LINEAR_SYSTEM(float **pprMatrix, float *prVector, int nSystemOrder);
			/** Desctructor. */
			~LINEAR_SYSTEM();


			/** Set the matrix coeffs of the system. */
			void	SetMatrix(float **pprMatrix);
			/** Set the vector coeffs of the system. */
			void	SetVecTor(float *prVector);
			/** Set a coeff of the matrix of the linear system. */
			void	SetMatrixCoeff(int nLin, int nCol, float rCoeff);
			/** Set a coeff of the vector of the linear system. */
			void	SetVectorCoeff(int nLin, float rCoeff);
			/** Get the determinant of the linear system. */
			float	Det();


			/** Find the solution of the system. */
			void	Solve(float* prSolution, bool bNoDet = false);


		private:
			float**	_pprMatrix;
			float*	_prVector;
			int		_nSystemOrder;


			/** Default constructor, private because user should not use it. */
			LINEAR_SYSTEM();

			
			/** Permute two lines of the system. */
			void	PermuteLines(int nIndex1stLine, int nIndex2ndLine);
			/** Reduction of the system to Upper Triangular. */
			void	Factorization();
			/** Solving backwards through the Upper Triangular Form. */
			void	BackSubstitution(float *prSolution);
			/** Used to compute Det(). */
			float	DetAux(int nSize, float **pprCoefs);
			
		};
	}
}

#endif //__KMATH_MATH_LINEARALGEBRA_LINEARSYSTEM_H__

//=============================================================================
// CODE ENDS HERE
//=============================================================================
