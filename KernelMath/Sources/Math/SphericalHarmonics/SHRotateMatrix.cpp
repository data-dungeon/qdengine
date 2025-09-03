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
//	CLASS:	VECTOR_2
//
//	03-03-10:	RMA - Created
//*****************************************************************************

//=============================================================================
//	CODE STARTS HERE
//=============================================================================
#include	"Root.h"

#include	"SHRotateMatrix.h"

#ifdef _DEBUG
#	include	"SHRotateMatrix.inl"
#endif

#include	INCL_KMATH(Math/Geometry/Matrix/Matrix)

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
void	SHROTATE_MATRIX::Transform(Float32*	source,
								   Float32* dest) const
{

		// Band 0 is always multiplied by 1 so it stays untouched
		dest[0] = source[0];

		// Loop through each band
		for (int l = 1; l < N_BANDS; l++)
		{
			const SUB_MATRIX& M = m_Matrices[l];

			// Calculate band offset into coeff list
			int band_offset = l * (l + 1);

			// Now through each argument of the destination coeff-list
			for (int mo = -l; mo <= l; mo++)
			{
				// Clear destination
				Float32& d = dest[band_offset + mo];
				d = 0;

				// Pre-calculate of mo's lookup in the sub-matrix, plus mi's shift
				int p_mo = (mo + M.GetShift()) * M.GetWidth() + M.GetShift();

				// Multiply-add with each argument of the source coeff-list
				for (int mi = -l; mi <= l; mi++)
					d += source[band_offset + mi] * M(p_mo + mi);
			}
		}
}



//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	06-02-23:	PDE - Created
//-----------------------------------------------------------------------------
void	SHROTATE_MATRIX::Transform(SPHERICAL_HARMONICS *	source,
								   SPHERICAL_HARMONICS *	dest) const
{
		Float32* destRed = dest->GetRedComponents();
		Float32* sourceRed = source->GetRedComponents();
		Float32* destGreen = dest->GetGreenComponents();
		Float32* sourceGreen = source->GetGreenComponents();
		Float32* destBlue = dest->GetBlueComponents();
		Float32* sourceBlue = source->GetBlueComponents();

		// Band 0 is always multiplied by 1 so it stays untouched
		destRed[0]   = sourceRed[0];
		destGreen[0] = sourceGreen[0];
		destBlue[0]  = sourceBlue[0];

		// Loop through each band
		for (int l = 1; l < N_BANDS; l++)
		{
			const SUB_MATRIX& M = m_Matrices[l];

			// Calculate band offset into coeff list
			int band_offset = l * (l + 1);

			// Now through each argument of the destination coeff-list
			for (int mo = -l; mo <= l; mo++)
			{
				// Clear destination
				UInt32  offsetDest = band_offset + mo;
				Float32& dRed   = destRed[offsetDest];
				Float32& dGreen = destGreen[offsetDest];
				Float32& dBlue  = destBlue[offsetDest];

				dRed = dGreen = dBlue = 0;

				// Pre-calculate of mo's lookup in the sub-matrix, plus mi's shift
				int p_mo = (mo + M.GetShift()) * M.GetWidth() + M.GetShift();

				// Multiply-add with each argument of the source coeff-list
				for (int mi = -l; mi <= l; mi++)
				{
					Float32 coeff = M(p_mo + mi);
					UInt32  offset = band_offset + mi;
					dRed += sourceRed[offset] * coeff;
					dGreen += sourceGreen[offset] * coeff;
					dBlue += sourceBlue[offset] * coeff;
				}
			}
		}
}



//-----------------------------------------------------------------------------
//	Name:		Ivanic_SHRotate
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
void	SHROTATE_MATRIX::Ivanic_SHRotate(MATRIX &	rotation)
{
	// To order required in paper
	//MATRIX r = cMatrix::Transpose(rotation);

	// The first band is rotated by a permutation of the original matrix
	(*this)(1, -1, -1) =  rotation(1, 1);
	(*this)(1, -1,  0) = -rotation(2, 1);
	(*this)(1, -1, +1) =  rotation(0, 1);
	(*this)(1,  0, -1) = -rotation(1, 2);
	(*this)(1,  0,  0) =  rotation(2, 2);
	(*this)(1,  0, +1) = -rotation(0, 2);
	(*this)(1, +1, -1) =  rotation(1, 0);
	(*this)(1, +1,  0) = -rotation(2, 0);
	(*this)(1, +1, +1) =  rotation(0, 0);

	// Calculate each block of the rotation matrix for each subsequent band
	for (int band = 2; band < N_BANDS; band++)
	{
		for (int m = -band; m <= band; m++)
			for (int n = -band; n <= band; n++)
				(*this)(band, m, n) = Ivanic_M(band, m, n, (*this));
	}
}


//-----------------------------------------------------------------------------
//	Name:		Ivanic_uvw
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
void	SHROTATE_MATRIX::Ivanic_uvw(const Int32	l,
										const Int32	m,
										const Int32	n,
										Float32 &		u,
										Float32 &		v,
										Float32 &		w)
{
			// Pre-calculate simple reusable terms
		Float32 d = Kronecker_delta(m, 0);
		Int32 abs_m = MATH::Abs(m);

		// Only calculate the required denominator once
		Float32 denom;
		if (MATH::Abs(n) == l)
			denom = (Float32)((2 * l) * (2 * l - 1));
		else
			denom = (Float32)((l + n) * (l - n));

		// Now just calculate the scalars
		u = MATH::Sqrt((l + m) * (l - m) / denom);
		v = 0.5f * MATH::Sqrt((1 + d) * (l + abs_m - 1) * (l + abs_m) / denom) * (1 - 2 * d);
		w = -0.5f * MATH::Sqrt((l - abs_m - 1) * (l - abs_m) / denom) * (1 - d);
}

//-----------------------------------------------------------------------------
//	Name:		Ivanic_P
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SHROTATE_MATRIX::Ivanic_P(const Int32				i,
								  const Int32				l,
								  const Int32				a,
								  const int					b,
								  const SHROTATE_MATRIX &	M)
{
		// Rather than passing the permuted rotation matrix around grab it directly from the first
		// rotation band which is never modified
		Float32 ri1 = M(1, i, 1);
		Float32 rim1 = M(1, i, -1);
		Float32 ri0 = M(1, i, 0);

		if (b == -l)
			return (ri1 * M(l - 1, a, -l + 1) + rim1 * M(l - 1, a, l - 1));

		else if (b == l)
			return (ri1 * M(l - 1, a, l - 1) - rim1 * M(l - 1, a, -l + 1));

		else // |b|<l
			return (ri0 * M(l - 1, a, b));
}

//-----------------------------------------------------------------------------
//	Name:		Ivanic_U
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SHROTATE_MATRIX::Ivanic_U(const Int32				l,
								  const Int32				m,
								  const Int32				n,
								  const SHROTATE_MATRIX &	M)
{
	// All cases fall correctly through here
	return (Ivanic_P(0, l, m, n, M));
}

//-----------------------------------------------------------------------------
//	Name:		Ivanic_V
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SHROTATE_MATRIX::Ivanic_V(const Int32				l,
								  const Int32				m,
								  const Int32				n,
								  const SHROTATE_MATRIX &	M)
{
		if (m == 0)
		{
			Float32 p0 = Ivanic_P(1, l, 1, n, M);
			Float32 p1 = Ivanic_P(-1, l, -1, n, M);
			return (p0 + p1);
		}

		else if (m > 0)
		{
			Float32 d = Kronecker_delta(m, 1);
			Float32 p0 = Ivanic_P(1, l, m - 1, n, M);
			Float32 p1 = Ivanic_P(-1, l, -m + 1, n, M);
			return (p0 * MATH::Sqrt(1 + d) - p1 * (1 - d));
		}

		else // m < 0
		{
			Float32 d = Kronecker_delta(m, -1);
			Float32 p0 = Ivanic_P(1, l, m + 1, n, M);
			Float32 p1 = Ivanic_P(-1, l, -m - 1, n, M);
			return (p0 * (1 - d) + p1 * MATH::Sqrt(1 + d));
		}
}

//-----------------------------------------------------------------------------
//	Name:		Ivanic_W
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SHROTATE_MATRIX::Ivanic_W(const Int32				l,
								  const Int32				m,
								  const Int32				n,
								  const SHROTATE_MATRIX &	M)
{
		if (m == 0)
		{
			// Never gets called as kd=0
			QDT_ASSERT(false);
			return (0);
		}

		else if (m > 0)
		{
			Float32 p0 = Ivanic_P(1, l, m + 1, n, M);
			Float32 p1 = Ivanic_P(-1, l, -m - 1, n, M);
			return (p0 + p1);
		}

		else // m < 0
		{
			Float32 p0 = Ivanic_P(1, l, m - 1, n, M);
			Float32 p1 = Ivanic_P(-1, l, -m + 1, n, M);
			return (p0 - p1);
		}
}

//-----------------------------------------------------------------------------
//	Name:		Ivanic_M
//	Object:		
//	06-02-22:	PDE - Created
//-----------------------------------------------------------------------------
Float32	SHROTATE_MATRIX::Ivanic_M(const Int32				l,
								  const Int32				m,
								  const Int32				n,
								  const SHROTATE_MATRIX &	M)
{
		// First get the scalars
		Float32 u, v, w;
		Ivanic_uvw(l, m, n, u, v, w);

		// Scale by their functions
		if (u)
			u *= Ivanic_U(l, m, n, M);
		if (v)
			v *= Ivanic_V(l, m, n, M);
		if (w)
			w *= Ivanic_W(l, m, n, M);

		return (u + v + w);
}




//=============================================================================
//	CODE ENDS HERE
//=============================================================================
