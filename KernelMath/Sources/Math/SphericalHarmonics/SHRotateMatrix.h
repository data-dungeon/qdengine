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
//	CLASS:	SHROTATE_MATRIX
//	The SHROTATE_MATRIX class implements ...
//
//	06-02-22:	PDE - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================


#ifndef __KMATH_MATH_SPHERICAL_ROTATE_MATRIX_H__
#define __KMATH_MATH_SPHERICAL_ROTATE_MATRIX_H__

#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

DECL_KMATH(MATRIX)
DECL_KMATH(SPHERICAL_HARMONICS)

#include	INCL_KMATH(Math/Math)

#define N_BANDS 3
#define MAT_SIZE 35    // 1^2 + 3^2 + 5^2 = 1 + 9 + 25 = 35

namespace	QDT
{
	namespace KMATH
	{

		class	SHROTATE_MATRIX
		{
		public:
			_INLINE_ SHROTATE_MATRIX();


			_INLINE_ ~SHROTATE_MATRIX();

			// Element accessors which take the band index and (+,-) index relative to the centre
			// of the sub-matrix
			_INLINE_ Float32&	operator () (const int l, const int m, const int n);
			_INLINE_ Float32    operator () (const int l, const int m, const int n) const;

			void Transform(Float32* source, Float32* dest) const;
			void Transform(SPHERICAL_HARMONICS* source, SPHERICAL_HARMONICS* dest) const;
			void   Ivanic_SHRotate(MATRIX& rotation);

			private:

			_INLINE_ Float32 Kronecker_delta(const Int32 m, const Int32 n);
			void Ivanic_uvw(const Int32 l, const Int32 m, const Int32 n, Float32& u, Float32& v, Float32& w);
			Float32 Ivanic_P(const Int32 i, const Int32 l, const Int32 a, const int b, const SHROTATE_MATRIX& M);
			Float32 Ivanic_U(const Int32 l, const Int32 m, const Int32 n, const SHROTATE_MATRIX& M);
			Float32 Ivanic_V(const Int32 l, const Int32 m, const Int32 n, const SHROTATE_MATRIX& M);
			Float32 Ivanic_W(const Int32 l, const Int32 m, const Int32 n, const SHROTATE_MATRIX& M);
			Float32 Ivanic_M(const Int32 l, const Int32 m, const Int32 n, const SHROTATE_MATRIX& M);


			class SUB_MATRIX
			{
				public:

					SUB_MATRIX() {};


				// Construct that needs previously allocated element array
				SUB_MATRIX(Float32* elements, const Int32 width)  :
					m_Elements(elements),
					m_Width(width),
					m_Size(width * width),
					m_Shift((width - 1) / 2)
				{
				}

				// Element accessors with (+,-) lookup
				Float32& operator () (const Int32 m, const Int32 n){return (m_Elements[Index(m, n)]);}
				Float32 operator () (const Int32 m, const Int32 n) const{return (m_Elements[Index(m, n)]);}

				// Linear element accessors
				Float32& operator () (const Int32 i) {return (m_Elements[Index(i)]);}
				Float32 operator () (const Int32 i) const {return (m_Elements[Index(i)]);}

				int	GetWidth(void) const{return (m_Width);}
				int	GetShift(void) const{return (m_Shift);}

			private:
				int Index(const Int32 m, const Int32 n) const
				{
					QDT_ASSERT(m >= -m_Shift && m <= m_Shift);
					QDT_ASSERT(n >= -m_Shift && n <= m_Shift);
					return ((m + m_Shift) * m_Width + (n + m_Shift));
				}

				int Index(const Int32 i) const
				{
					QDT_ASSERT(i >= 0 && i < m_Size);
					return (i);
				}

				Float32*	m_Elements; // Element array
				Int32	m_Width;    // Width of the sub-matrix
				Int32	m_Size;		// Total size of the sub-matrix in multiples of doubles
				Int32	m_Shift;	// Value to shift incoming matrix indices by
			};

			// Sub-matrix for each band
			SUB_MATRIX	m_Matrices[N_BANDS];

			// Element array for each sub-matrix
			Float32  	m_Elements[MAT_SIZE];

		};
	}
}

#ifndef _DEBUG
#include	"SHRotateMatrix.inl"
#endif

#endif
//=============================================================================
//	CODE ENDS HERE
//=============================================================================
