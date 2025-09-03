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
// CLASS: MATRIX
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include 	"Root.h"
#include 	INCL_KMATH(Math/Geometry/Matrix/Matrix)

#ifdef _DEBUG
#include	"MatrixGen.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		
//	03-02-06:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformPositionGen(const VECTOR &	Src,
								     VECTOR &			Dst) const
{
	MATRIX_GET_GENERIC_DATA();

	Dst.SetX(Src.GetX() * _r[0][0] + Src.GetY() * _r[1][0] + Src.GetZ() * _r[2][0] + _r[3][0]);
	Dst.SetY(Src.GetX() * _r[0][1] + Src.GetY() * _r[1][1] + Src.GetZ() * _r[2][1] + _r[3][1]);
	Dst.SetZ(Src.GetX() * _r[0][2] + Src.GetY() * _r[1][2] + Src.GetZ() * _r[2][2] + _r[3][2]);
}

//-----------------------------------------------------------------------------
//	Name:		Transform
//	Object:		Same as transform position but does not apply the translation
//				This is useful for normals!!
//	03-02-06:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformDirectionGen(const VECTOR &	Src,
								      VECTOR &			Dst) const
{
	MATRIX_GET_GENERIC_DATA();

	Dst.SetX(Src.GetX() * _r[0][0] + Src.GetY() * _r[1][0] + Src.GetZ() * _r[2][0]);
	Dst.SetY(Src.GetX() * _r[0][1] + Src.GetY() * _r[1][1] + Src.GetZ() * _r[2][1]);
	Dst.SetZ(Src.GetX() * _r[0][2] + Src.GetY() * _r[1][2] + Src.GetZ() * _r[2][2]);
} 

//-----------------------------------------------------------------------------
//	Name:		BatchTransformDirection
//	Object:		
//	03-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformDirectionGen(const VECTOR *		pSrc,
										   VECTOR *			pDst,
										   unsigned int *		pIndices,
										   int					nNbrVertices) const
{
	MATRIX_GET_GENERIC_DATA();

	int i = 0;
	int nIndice;

	while (i < nNbrVertices)
	{
		nIndice = *pIndices;

		pDst[nIndice].SetX(pSrc[nIndice].GetX() * _r[0][0] + pSrc[nIndice].GetY() * _r[1][0] + pSrc[nIndice].GetZ() * _r[2][0]);
		pDst[nIndice].SetY(pSrc[nIndice].GetX() * _r[0][1] + pSrc[nIndice].GetY() * _r[1][1] + pSrc[nIndice].GetZ() * _r[2][1]);
		pDst[nIndice].SetZ(pSrc[nIndice].GetX() * _r[0][2] + pSrc[nIndice].GetY() * _r[1][2] + pSrc[nIndice].GetZ() * _r[2][2]);

		++pIndices;
		++i;
	}
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformPosition
//	Object:		
//	03-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformPositionGen(const VECTOR *	pSrc,
									      VECTOR *			pDst,
									      unsigned int *	pIndices,
									      int				nNbrVertices) const
{
	MATRIX_GET_GENERIC_DATA();

	int i = 0;
	int nIndice;

	while (i < nNbrVertices)
	{
		nIndice = *pIndices;

		pDst[nIndice].SetX(pSrc[nIndice].GetX() * _r[0][0] + pSrc[nIndice].GetY() * _r[1][0] + pSrc[nIndice].GetZ() * _r[2][0] + _r[3][0]);
		pDst[nIndice].SetY(pSrc[nIndice].GetX() * _r[0][1] + pSrc[nIndice].GetY() * _r[1][1] + pSrc[nIndice].GetZ() * _r[2][1] + _r[3][1]);
		pDst[nIndice].SetZ(pSrc[nIndice].GetX() * _r[0][2] + pSrc[nIndice].GetY() * _r[1][2] + pSrc[nIndice].GetZ() * _r[2][2] + _r[3][2]);

		++pIndices;
		++i;
	}
}

//-----------------------------------------------------------------------------
//	Name:		MulAffine
//	Object:		
//	04-08-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulAffineGen(const MATRIX &	M1,
							 const MATRIX &	M2)
{
	MATRIX_GET_GENERIC_DATA();

	QDT_ASSERT(this != &M1);
	QDT_ASSERT(this != &M2);
	QDT_ASSERT_MSG(M1.IsAffine(), ("Use should not use this method on a non-affine MATRIX"));
	QDT_ASSERT_MSG(M2.IsAffine(), ("Use should not use this method on a non-affine MATRIX"));

	_r[0][0] = M1(0, 0) * M2(0, 0) + M1(0, 1) * M2(1, 0) + M1(0, 2) * M2(2, 0);
	_r[0][1] = M1(0, 0) * M2(0, 1) + M1(0, 1) * M2(1, 1) + M1(0, 2) * M2(2, 1);
	_r[0][2] = M1(0, 0) * M2(0, 2) + M1(0, 1) * M2(1, 2) + M1(0, 2) * M2(2, 2);

	_r[1][0] = M1(1, 0) * M2(0, 0) + M1(1, 1) * M2(1, 0) + M1(1, 2) * M2(2, 0);
	_r[1][1] = M1(1, 0) * M2(0, 1) + M1(1, 1) * M2(1, 1) + M1(1, 2) * M2(2, 1);
	_r[1][2] = M1(1, 0) * M2(0, 2) + M1(1, 1) * M2(1, 2) + M1(1, 2) * M2(2, 2);

	_r[2][0] = M1(2, 0) * M2(0, 0) + M1(2, 1) * M2(1, 0) + M1(2, 2) * M2(2, 0);
	_r[2][1] = M1(2, 0) * M2(0, 1) + M1(2, 1) * M2(1, 1) + M1(2, 2) * M2(2, 1);
	_r[2][2] = M1(2, 0) * M2(0, 2) + M1(2, 1) * M2(1, 2) + M1(2, 2) * M2(2, 2);

	_r[3][0] = M1(3, 0) * M2(0, 0) + M1(3, 1) * M2(1, 0) + M1(3, 2) * M2(2, 0) + M2(3, 0);
	_r[3][1] = M1(3, 0) * M2(0, 1) + M1(3, 1) * M2(1, 1) + M1(3, 2) * M2(2, 1) + M2(3, 1);
	_r[3][2] = M1(3, 0) * M2(0, 2) + M1(3, 1) * M2(1, 2) + M1(3, 2) * M2(2, 2) + M2(3, 2);
}

//-----------------------------------------------------------------------------
//	Name:		MulAffine
//	Object:		
//	04-08-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulAffineGen(const MATRIX & M)
{
	QDT_ASSERT(this != &M);

	MATRIX Temp(*this);

	Mul(Temp, M);
}

//-----------------------------------------------------------------------------
//	Name:		MulAffine
//	Object:		
//	04-08-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulAffineGen(Float32	r)
{
	MATRIX_GET_GENERIC_DATA();

	QDT_ASSERT(IsAffine() && "Use should not use this method on a non-affine MATRIX");

	_r[0][0] *= r;
	_r[0][1] *= r;
	_r[0][2] *= r;

	_r[1][0] *= r;
	_r[1][1] *= r;
	_r[1][2] *= r;

	_r[2][0] *= r;
	_r[2][1] *= r;
	_r[2][2] *= r;

	_r[3][0] *= r;
	_r[3][1] *= r;
	_r[3][2] *= r;
}

//-----------------------------------------------------------------------------
//	Name:		Mul1Optimized
//	Object:		
//	03-02-10:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulGen(const MATRIX &	ParamM1,
					   const MATRIX &	ParamM2)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(M1, ParamM1);
	MATRIX_GET_GENERIC_DATA_PARAM(M2, ParamM2);

	QDT_ASSERT(this != &ParamM1);
	QDT_ASSERT(this != &ParamM2);

	QDT_ASSERT(ToPointer() != ParamM1.ToPointer());
	QDT_ASSERT(ToPointer() != ParamM2.ToPointer());

	_r[0][0] = M1._r[0][0] * M2._r[0][0] + M1._r[0][1] * M2._r[1][0] + M1._r[0][2] * M2._r[2][0] + M1._r[0][3] * M2._r[3][0];
	_r[0][1] = M1._r[0][0] * M2._r[0][1] + M1._r[0][1] * M2._r[1][1] + M1._r[0][2] * M2._r[2][1] + M1._r[0][3] * M2._r[3][1];
	_r[0][2] = M1._r[0][0] * M2._r[0][2] + M1._r[0][1] * M2._r[1][2] + M1._r[0][2] * M2._r[2][2] + M1._r[0][3] * M2._r[3][2];
	_r[0][3] = M1._r[0][0] * M2._r[0][3] + M1._r[0][1] * M2._r[1][3] + M1._r[0][2] * M2._r[2][3] + M1._r[0][3] * M2._r[3][3];
	
	_r[1][0] = M1._r[1][0] * M2._r[0][0] + M1._r[1][1] * M2._r[1][0] + M1._r[1][2] * M2._r[2][0] + M1._r[1][3] * M2._r[3][0];
	_r[1][1] = M1._r[1][0] * M2._r[0][1] + M1._r[1][1] * M2._r[1][1] + M1._r[1][2] * M2._r[2][1] + M1._r[1][3] * M2._r[3][1];
	_r[1][2] = M1._r[1][0] * M2._r[0][2] + M1._r[1][1] * M2._r[1][2] + M1._r[1][2] * M2._r[2][2] + M1._r[1][3] * M2._r[3][2];
	_r[1][3] = M1._r[1][0] * M2._r[0][3] + M1._r[1][1] * M2._r[1][3] + M1._r[1][2] * M2._r[2][3] + M1._r[1][3] * M2._r[3][3];
	
	_r[2][0] = M1._r[2][0] * M2._r[0][0] + M1._r[2][1] * M2._r[1][0] + M1._r[2][2] * M2._r[2][0] + M1._r[2][3] * M2._r[3][0];
	_r[2][1] = M1._r[2][0] * M2._r[0][1] + M1._r[2][1] * M2._r[1][1] + M1._r[2][2] * M2._r[2][1] + M1._r[2][3] * M2._r[3][1];
	_r[2][2] = M1._r[2][0] * M2._r[0][2] + M1._r[2][1] * M2._r[1][2] + M1._r[2][2] * M2._r[2][2] + M1._r[2][3] * M2._r[3][2];
	_r[2][3] = M1._r[2][0] * M2._r[0][3] + M1._r[2][1] * M2._r[1][3] + M1._r[2][2] * M2._r[2][3] + M1._r[2][3] * M2._r[3][3];
	
	_r[3][0] = M1._r[3][0] * M2._r[0][0] + M1._r[3][1] * M2._r[1][0] + M1._r[3][2] * M2._r[2][0] + M1._r[3][3] * M2._r[3][0];
	_r[3][1] = M1._r[3][0] * M2._r[0][1] + M1._r[3][1] * M2._r[1][1] + M1._r[3][2] * M2._r[2][1] + M1._r[3][3] * M2._r[3][1];
	_r[3][2] = M1._r[3][0] * M2._r[0][2] + M1._r[3][1] * M2._r[1][2] + M1._r[3][2] * M2._r[2][2] + M1._r[3][3] * M2._r[3][2];
	_r[3][3] = M1._r[3][0] * M2._r[0][3] + M1._r[3][1] * M2._r[1][3] + M1._r[3][2] * M2._r[2][3] + M1._r[3][3] * M2._r[3][3];
}

//-----------------------------------------------------------------------------
//	Name:		Mul2Optimized
//	Object:		
//	03-02-10:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulGen(const MATRIX & M)
{
	QDT_ASSERT(this != &M);

	MATRIX Temp(*this);

	Mul(Temp, M);
}

//-----------------------------------------------------------------------------
//	Name:		Mul3Optimized
//	Object:		
//	03-02-10:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulGen(Float32 r)
{	
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] *= r;
	_r[0][1] *= r;
	_r[0][2] *= r;
	_r[0][3] *= r;

	_r[1][0] *= r;
	_r[1][1] *= r;
	_r[1][2] *= r;
	_r[1][3] *= r;

	_r[2][0] *= r;
	_r[2][1] *= r;
	_r[2][2] *= r;
	_r[2][3] *= r;

	_r[3][0] *= r;
	_r[3][1] *= r;
	_r[3][2] *= r;
	_r[3][3] *= r;
}

//-----------------------------------------------------------------------------
//	Name:		Square
//	Object:		
//	03-02-10:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SquareGen()
{
	MATRIX Temp(*this);
	Mul(Temp, Temp);
}

//-----------------------------------------------------------------------------
//	Name:		SquareAffine
//	Object:		
//	04-08-04:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SquareAffineGen()
{
	QDT_ASSERT(IsAffine() && "Use should not use this method on a non-affine MATRIX");

	MATRIX Temp(*this);
	MulAffine(Temp, Temp);
}

//-----------------------------------------------------------------------------
//	Name:		InverseProjection
//	Object:		MATRIX inversion
//	06-01-05:	BFA - Created
//-----------------------------------------------------------------------------
void MATRIX::InverseGen(const MATRIX& ParamMATRIXSource)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(MATRIXSource, ParamMATRIXSource);

	const Float32 fEPSILON = 0.0000001f;

	Float32 fA0 = MATRIXSource._r[0][0]*MATRIXSource._r[1][1] - MATRIXSource._r[0][1]*MATRIXSource._r[1][0];
	Float32 fA1 = MATRIXSource._r[0][0]*MATRIXSource._r[1][2] - MATRIXSource._r[0][2]*MATRIXSource._r[1][0];
	Float32 fA2 = MATRIXSource._r[0][0]*MATRIXSource._r[1][3] - MATRIXSource._r[0][3]*MATRIXSource._r[1][0];
	Float32 fA3 = MATRIXSource._r[0][1]*MATRIXSource._r[1][2] - MATRIXSource._r[0][2]*MATRIXSource._r[1][1];
	Float32 fA4 = MATRIXSource._r[0][1]*MATRIXSource._r[1][3] - MATRIXSource._r[0][3]*MATRIXSource._r[1][1];
	Float32 fA5 = MATRIXSource._r[0][2]*MATRIXSource._r[1][3] - MATRIXSource._r[0][3]*MATRIXSource._r[1][2];
	Float32 fB0 = MATRIXSource._r[2][0]*MATRIXSource._r[3][1] - MATRIXSource._r[2][1]*MATRIXSource._r[3][0];
	Float32 fB1 = MATRIXSource._r[2][0]*MATRIXSource._r[3][2] - MATRIXSource._r[2][2]*MATRIXSource._r[3][0];
	Float32 fB2 = MATRIXSource._r[2][0]*MATRIXSource._r[3][3] - MATRIXSource._r[2][3]*MATRIXSource._r[3][0];
	Float32 fB3 = MATRIXSource._r[2][1]*MATRIXSource._r[3][2] - MATRIXSource._r[2][2]*MATRIXSource._r[3][1];
	Float32 fB4 = MATRIXSource._r[2][1]*MATRIXSource._r[3][3] - MATRIXSource._r[2][3]*MATRIXSource._r[3][1];
	Float32 fB5 = MATRIXSource._r[2][2]*MATRIXSource._r[3][3] - MATRIXSource._r[2][3]*MATRIXSource._r[3][2];

	Float32 fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
	if (MATH::Abs(fDet) <= fEPSILON)
	{
		Zero();
		return;
	}

	_r[0][0] = + MATRIXSource._r[1][1]*fB5 - MATRIXSource._r[1][2]*fB4 + MATRIXSource._r[1][3]*fB3;
	_r[1][0] = - MATRIXSource._r[1][0]*fB5 + MATRIXSource._r[1][2]*fB2 - MATRIXSource._r[1][3]*fB1;
	_r[2][0] = + MATRIXSource._r[1][0]*fB4 - MATRIXSource._r[1][1]*fB2 + MATRIXSource._r[1][3]*fB0;
	_r[3][0] = - MATRIXSource._r[1][0]*fB3 + MATRIXSource._r[1][1]*fB1 - MATRIXSource._r[1][2]*fB0;
	_r[0][1] = - MATRIXSource._r[0][1]*fB5 + MATRIXSource._r[0][2]*fB4 - MATRIXSource._r[0][3]*fB3;
	_r[1][1] = + MATRIXSource._r[0][0]*fB5 - MATRIXSource._r[0][2]*fB2 + MATRIXSource._r[0][3]*fB1;
	_r[2][1] = - MATRIXSource._r[0][0]*fB4 + MATRIXSource._r[0][1]*fB2 - MATRIXSource._r[0][3]*fB0;
	_r[3][1] = + MATRIXSource._r[0][0]*fB3 - MATRIXSource._r[0][1]*fB1 + MATRIXSource._r[0][2]*fB0;
	_r[0][2] = + MATRIXSource._r[3][1]*fA5 - MATRIXSource._r[3][2]*fA4 + MATRIXSource._r[3][3]*fA3;
	_r[1][2] = - MATRIXSource._r[3][0]*fA5 + MATRIXSource._r[3][2]*fA2 - MATRIXSource._r[3][3]*fA1;
	_r[2][2] = + MATRIXSource._r[3][0]*fA4 - MATRIXSource._r[3][1]*fA2 + MATRIXSource._r[3][3]*fA0;
	_r[3][2] = - MATRIXSource._r[3][0]*fA3 + MATRIXSource._r[3][1]*fA1 - MATRIXSource._r[3][2]*fA0;
	_r[0][3] = - MATRIXSource._r[2][1]*fA5 + MATRIXSource._r[2][2]*fA4 - MATRIXSource._r[2][3]*fA3;
	_r[1][3] = + MATRIXSource._r[2][0]*fA5 - MATRIXSource._r[2][2]*fA2 + MATRIXSource._r[2][3]*fA1;
	_r[2][3] = - MATRIXSource._r[2][0]*fA4 + MATRIXSource._r[2][1]*fA2 - MATRIXSource._r[2][3]*fA0;
	_r[3][3] = + MATRIXSource._r[2][0]*fA3 - MATRIXSource._r[2][1]*fA1 + MATRIXSource._r[2][2]*fA0;

	Float32 fInvDet = 1.0f/fDet;
	for (Int32 iRow = 0; iRow < 4; iRow++)
	{
		for (Int32 iCol = 0; iCol < 4; iCol++)
		{
			_r[iRow][iCol] *= fInvDet;
		}
	}
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::Inverse
//	Object: This method computes the inverse of a general affine MATRIX:
//					|X X X 0|
//					|X X X 0|
//					|X X X 0|
//					|X X X 1|			
//			The computation is **REALLY** heavy and FastInverse should be prefered
//			if possible.
//	01-02-08:	GGO - Created
//-----------------------------------------------------------------------------
void	MATRIX::InverseAffineGen(const MATRIX & ParamMat)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Mat, ParamMat);

	QDT_ASSERT(this != (&ParamMat) && " Can compute inverse MATRIX on itself, please use a temp MATRIX");
	QDT_ASSERT(ParamMat.IsAffine() && "Use should not use this method on a non-affine MATRIX");

	Float32 rTemp = Mat._r[0][0] * Mat._r[1][1] * Mat._r[2][2]
				- Mat._r[0][0] * Mat._r[2][1] * Mat._r[1][2]
				- Mat._r[1][0] * Mat._r[0][1] * Mat._r[2][2]
				+ Mat._r[1][0] * Mat._r[2][1] * Mat._r[0][2]
				+ Mat._r[2][0] * Mat._r[0][1] * Mat._r[1][2]
				- Mat._r[2][0] * Mat._r[1][1] * Mat._r[0][2];

	QDT_ASSERT(rTemp != 0.0f);

	rTemp = 1.0f / rTemp;

	Float32 M00xM11 = Mat._r[0][0] * Mat._r[1][1];
	Float32 M00xM12 = Mat._r[0][0] * Mat._r[1][2];
	Float32 M00xM21 = Mat._r[0][0] * Mat._r[2][1];
	Float32 M00xM22 = Mat._r[0][0] * Mat._r[2][2];

	Float32 M01xM10 = Mat._r[0][1] * Mat._r[1][0];
	Float32 M01xM12 = Mat._r[0][1] * Mat._r[1][2];
	Float32 M01xM20 = Mat._r[0][1] * Mat._r[2][0];
	Float32 M01xM22 = Mat._r[0][1] * Mat._r[2][2];

	Float32 M02xM10 = Mat._r[0][2] * Mat._r[1][0];
	Float32 M02xM11 = Mat._r[0][2] * Mat._r[1][1];
	Float32 M02xM20 = Mat._r[0][2] * Mat._r[2][0];
	Float32 M02xM21 = Mat._r[0][2] * Mat._r[2][1];

	Float32 M10xM21 = Mat._r[1][0] * Mat._r[2][1];
	Float32 M10xM22 = Mat._r[1][0] * Mat._r[2][2];

	Float32 M11xM20 = Mat._r[1][1] * Mat._r[2][0];
	Float32 M11xM22 = Mat._r[1][1] * Mat._r[2][2];

	Float32 M12xM20 = Mat._r[1][2] * Mat._r[2][0];
	Float32 M12xM21 = Mat._r[1][2] * Mat._r[2][1];


	_r[0][0] = ( M11xM22 - M12xM21) * rTemp;
	_r[0][1] =	(-M01xM22 + M02xM21) * rTemp;
	_r[0][2] = ( M01xM12 - M02xM11) * rTemp;
	_r[0][3] = 0.0f;
	
	_r[1][0] = (-M10xM22 + M12xM20) * rTemp;
	_r[1][1] = ( M00xM22 - M02xM20) * rTemp;
	_r[1][2] = (-M00xM12 + M02xM10) * rTemp;
	_r[1][3] = 0.0f;
	
	_r[2][0] = ( M10xM21 - M11xM20) * rTemp;
	_r[2][1] = (-M00xM21 + M01xM20) * rTemp;
	_r[2][2] = ( M00xM11 - M01xM10) * rTemp;
	_r[2][3] = 0.0f;

	_r[3][0] = (-M10xM21 * Mat._r[3][2]
				    + M10xM22 * Mat._r[3][1]
				    + M11xM20 * Mat._r[3][2]
				    - M12xM20 * Mat._r[3][1]
				    - M11xM22 * Mat._r[3][0]
				    + M12xM21 * Mat._r[3][0]) * rTemp;

	_r[3][1] = ( M00xM21 * Mat._r[3][2]
					- M00xM22 * Mat._r[3][1]
					- M01xM20 * Mat._r[3][2]
					+ M02xM20 * Mat._r[3][1]
					+ M01xM22 * Mat._r[3][0]
					- M02xM21 * Mat._r[3][0]) * rTemp;

	_r[3][2] = (-M00xM11 * Mat._r[3][2]
					+ M00xM12 * Mat._r[3][1]
					+ M01xM10 * Mat._r[3][2]
					- M02xM10 * Mat._r[3][1]
					- M01xM12 * Mat._r[3][0]
					+ M02xM11 * Mat._r[3][0]) * rTemp;
	_r[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		Transpose
//	Object:		
//	04-03-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::GetTransposeGen(MATRIX & ParamTranspose)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Transpose, ParamTranspose);

	QDT_ASSERT(&ParamTranspose != this);
	Transpose._r[0][0] = _r[0][0];
	Transpose._r[0][1] = _r[1][0];
	Transpose._r[0][2] = _r[2][0];
	Transpose._r[0][3] = _r[3][0];

	Transpose._r[1][0] = _r[0][1];
	Transpose._r[1][1] = _r[1][1];
	Transpose._r[1][2] = _r[2][1];
	Transpose._r[1][3] = _r[3][1];

	Transpose._r[2][0] = _r[0][2];
	Transpose._r[2][1] = _r[1][2];
	Transpose._r[2][2] = _r[2][2];
	Transpose._r[2][3] = _r[3][2];

	Transpose._r[3][0] = _r[0][3];
	Transpose._r[3][1] = _r[1][3];
	Transpose._r[3][2] = _r[2][3];
	Transpose._r[3][3] = _r[3][3];
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::SetXYZ
//	Object: Set the rotation by X, Y and Z (same order as Maya).
//			This matrix correspond to M(X) * M(Y) * M(Z) where
//			- M(X) =
//				1			 0			 0
//				0			 CosAlpha	 SinAlpha
//				0			-SinAlpha	 CosAlpha
//			- M(Y) =
//				CosBeta		 0			-SinBeta
//				0			 1			 0
//			   -SinBeta		 0			 CosBeta
//			- M(Z) = 
//				CosGamma	 SinGamma	 0
//			   -SinGamma	 CosGamma	 0
//				0			 0			 1
//
//			Alpha correspond to a rotation by the X axe
//			Beta  correspond to a rotation by the Y axe
//			Gamma correspond to a rotation by the Z axe
//
//			This erase the 3x3 component of the matrix. 
//		
//	01-01-23:	RMA - Created
//-----------------------------------------------------------------------------
void MATRIX::SetRotXYZGen( Float32 rAlpha, Float32 rBeta, Float32 rGamma )
{
	MATRIX_GET_GENERIC_DATA();

	Float32	rCosAlpha,
			rSinAlpha,
			rCosBeta,
			rSinBeta,
			rCosGamma,
			rSinGamma;

	MATH::GetCosinusAndSinus(rAlpha, rCosAlpha, rSinAlpha);
	MATH::GetCosinusAndSinus(rBeta, rCosBeta, rSinBeta);
	MATH::GetCosinusAndSinus(rGamma, rCosGamma, rSinGamma);

	// For optimizations
	Float32 rSinAlphaXSinBeta = rSinAlpha * rSinBeta ;
	Float32 rCosAlphaXSinBeta = rCosAlpha * rSinBeta ;

	_r[0][0] =   (rCosBeta * rCosGamma) ;
	_r[0][1] =	(rCosBeta * rSinGamma) ;
	_r[0][2] = -	(rSinBeta) ;

	_r[1][0] =	(rSinAlphaXSinBeta * rCosGamma) - (rCosAlpha * rSinGamma) ;
	_r[1][1] =	(rSinAlphaXSinBeta * rSinGamma) + (rCosAlpha * rCosGamma) ;
	_r[1][2] =	(rSinAlpha * rCosBeta) ;

	_r[2][0] =	(rCosAlphaXSinBeta * rCosGamma) + (rSinAlpha * rSinGamma) ;
	_r[2][1] =	(rCosAlphaXSinBeta * rSinGamma) - (rSinAlpha * rCosGamma) ;
	_r[2][2] =	(rCosAlpha * rCosBeta) ;
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::InverseAffineNoScale
//	Object: This method computes the inverse of a MATRIX placed in parameter.
//			This method is faster than InverseAffine but it works only with 
//			MATRIX used to represent 3D orientation & position. If there is a .
//			scale, it can't works. Use this method for the commun case.
//	01-02-07:	RMA - Created
//	01-02-08:	GGO - Revisited
//-----------------------------------------------------------------------------
void	MATRIX::InverseAffineNoScaleGen(const MATRIX & ParamMat)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Mat, ParamMat);

	QDT_ASSERT(this != (&ParamMat) && " Can compute inverse MATRIX on itself, please use a temp MATRIX");
	QDT_ASSERT(ParamMat.HasNoScale() && "Use should not use this method on a scaled MATRIX");
	QDT_ASSERT(ParamMat.IsAffine() && "Use should not use this method on a non-affine MATRIX");
	
	// Translation inversion.
	_r[3][0] = - Mat._r[3][0] * Mat._r[0][0]
					- Mat._r[3][1] * Mat._r[0][1]
					- Mat._r[3][2] * Mat._r[0][2];

	_r[3][1] = - Mat._r[3][0] * Mat._r[1][0]
					- Mat._r[3][1] * Mat._r[1][1]
					- Mat._r[3][2] * Mat._r[1][2]; 

	_r[3][2] = - Mat._r[3][0] * Mat._r[2][0]
					- Mat._r[3][1] * Mat._r[2][1]
					- Mat._r[3][2] * Mat._r[2][2]; 

	// Rotation inversion (transposition of the 3X3 sub-MATRIX).
	_r[1][0] = Mat._r[0][1];
	_r[2][0] = Mat._r[0][2];
	_r[1][2] = Mat._r[2][1];

	_r[0][1] = Mat._r[1][0];
	_r[0][2] = Mat._r[2][0];
	_r[2][1] = Mat._r[1][2];

	// The diagonal coefficient remain unchanged.
	_r[0][0] = Mat._r[0][0];
	_r[1][1] = Mat._r[1][1];
	_r[2][2] = Mat._r[2][2];

	_r[0][3] = 0.0f;
	_r[1][3] = 0.0f;
	_r[2][3] = 0.0f;
	_r[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Add
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::AddAffineGen(const MATRIX & ParamM1, 
							 const MATRIX & ParamM2)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(M1, ParamM1);
	MATRIX_GET_GENERIC_DATA_PARAM(M2, ParamM2);

	QDT_ASSERT(ToPointer() != ParamM1.ToPointer());
	QDT_ASSERT(ToPointer() != ParamM2.ToPointer());
	QDT_ASSERT(ParamM1.IsAffine());
	QDT_ASSERT(ParamM2.IsAffine());

	_r[0][0] = M1._r[0][0] + M2._r[0][0];
	_r[0][1] = M1._r[0][1] + M2._r[0][1];
	_r[0][2] = M1._r[0][2] + M2._r[0][2];

	_r[1][0] = M1._r[1][0] + M2._r[1][0];
	_r[1][1] = M1._r[1][1] + M2._r[1][1];
	_r[1][2] = M1._r[1][2] + M2._r[1][2];

	_r[2][0] = M1._r[2][0] + M2._r[2][0];
	_r[2][1] = M1._r[2][1] + M2._r[2][1];
	_r[2][2] = M1._r[2][2] + M2._r[2][2];

	_r[3][0] = M1._r[3][0] + M2._r[3][0];
	_r[3][1] = M1._r[3][1] + M2._r[3][1];
	_r[3][2] = M1._r[3][2] + M2._r[3][2];
}

//-----------------------------------------------------------------------------
//	Name:		Add
//	Object:		
//	03-12-22:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::AddAffineGen(const MATRIX &	ParamM)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(M, ParamM);

	QDT_ASSERT(ToPointer() != ParamM.ToPointer());
	QDT_ASSERT(ParamM.IsAffine());

	_r[0][0] += M._r[0][0];
	_r[0][1] += M._r[0][1];
	_r[0][2] += M._r[0][2];

	_r[1][0] += M._r[1][0];
	_r[1][1] += M._r[1][1];
	_r[1][2] += M._r[1][2];

	_r[2][0] += M._r[2][0];
	_r[2][1] += M._r[2][1];
	_r[2][2] += M._r[2][2];

	_r[3][0] += M._r[3][0];
	_r[3][1] += M._r[3][1];
	_r[3][2] += M._r[3][2];
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Add
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::AddGen( const MATRIX & ParamM1, 
						const MATRIX & ParamM2)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(M1, ParamM1);
	MATRIX_GET_GENERIC_DATA_PARAM(M2, ParamM2);

	QDT_ASSERT( ToPointer() != ParamM1.ToPointer() );
	QDT_ASSERT( ToPointer() != ParamM2.ToPointer() );

	_r[0][0] = M1._r[0][0] + M2._r[0][0];
	_r[0][1] = M1._r[0][1] + M2._r[0][1];
	_r[0][2] = M1._r[0][2] + M2._r[0][2];
	_r[0][3] = M1._r[0][3] + M2._r[0][3];

	_r[1][0] = M1._r[1][0] + M2._r[1][0];
	_r[1][1] = M1._r[1][1] + M2._r[1][1];
	_r[1][2] = M1._r[1][2] + M2._r[1][2];
	_r[1][3] = M1._r[1][3] + M2._r[1][3];

	_r[2][0] = M1._r[2][0] + M2._r[2][0];
	_r[2][1] = M1._r[2][1] + M2._r[2][1];
	_r[2][2] = M1._r[2][2] + M2._r[2][2];
	_r[2][3] = M1._r[2][3] + M2._r[2][3];

	_r[3][0] = M1._r[3][0] + M2._r[3][0];
	_r[3][1] = M1._r[3][1] + M2._r[3][1];
	_r[3][2] = M1._r[3][2] + M2._r[3][2];
	_r[3][3] = M1._r[3][3] + M2._r[3][3];
}

//-----------------------------------------------------------------------------
//	Name:		Add
//	Object:		
//	03-12-22:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::AddGen(const MATRIX &	ParamM)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(M, ParamM);

	QDT_ASSERT( ToPointer() != ParamM.ToPointer() );

	_r[0][0] += M._r[0][0];
	_r[0][1] += M._r[0][1];
	_r[0][2] += M._r[0][2];
	_r[0][3] += M._r[0][3];

	_r[1][0] += M._r[1][0];
	_r[1][1] += M._r[1][1];
	_r[1][2] += M._r[1][2];
	_r[1][3] += M._r[1][3];

	_r[2][0] += M._r[2][0];
	_r[2][1] += M._r[2][1];
	_r[2][2] += M._r[2][2];
	_r[2][3] += M._r[2][3];

	_r[3][0] += M._r[3][0];
	_r[3][1] += M._r[3][1];
	_r[3][2] += M._r[3][2];
	_r[3][3] += M._r[3][3];
}

//-----------------------------------------------------------------------------
//	Name:		Mul3x3
//	Object:		
//	02-10-11:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::Mul3x3Gen(const MATRIX &	ParamMat1,
						  const MATRIX &	ParamMat2)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Mat1, ParamMat1);
	MATRIX_GET_GENERIC_DATA_PARAM(Mat2, ParamMat2);

	QDT_ASSERT( ToPointer() != ParamMat1.ToPointer() );
	QDT_ASSERT( ToPointer() != ParamMat2.ToPointer() );

	_r[0][0] = Mat1._r[0][0] * Mat2._r[0][0] + Mat1._r[0][1] * Mat2._r[1][0] + Mat1._r[0][2] * Mat2._r[2][0];
	_r[0][1] = Mat1._r[0][0] * Mat2._r[0][1] + Mat1._r[0][1] * Mat2._r[1][1] + Mat1._r[0][2] * Mat2._r[2][1];
	_r[0][2] = Mat1._r[0][0] * Mat2._r[0][2] + Mat1._r[0][1] * Mat2._r[1][2] + Mat1._r[0][2] * Mat2._r[2][2];
	
	_r[1][0] = Mat1._r[1][0] * Mat2._r[0][0] + Mat1._r[1][1] * Mat2._r[1][0] + Mat1._r[1][2] * Mat2._r[2][0];
	_r[1][1] = Mat1._r[1][0] * Mat2._r[0][1] + Mat1._r[1][1] * Mat2._r[1][1] + Mat1._r[1][2] * Mat2._r[2][1];
	_r[1][2] = Mat1._r[1][0] * Mat2._r[0][2] + Mat1._r[1][1] * Mat2._r[1][2] + Mat1._r[1][2] * Mat2._r[2][2];
	
	_r[2][0] = Mat1._r[2][0] * Mat2._r[0][0] + Mat1._r[2][1] * Mat2._r[1][0] + Mat1._r[2][2] * Mat2._r[2][0];
	_r[2][1] = Mat1._r[2][0] * Mat2._r[0][1] + Mat1._r[2][1] * Mat2._r[1][1] + Mat1._r[2][2] * Mat2._r[2][1];
	_r[2][2] = Mat1._r[2][0] * Mat2._r[0][2] + Mat1._r[2][1] * Mat2._r[1][2] + Mat1._r[2][2] * Mat2._r[2][2];	
}

//-----------------------------------------------------------------------------
// Name: MATRIX::GetQuaternionGen
// Object: 
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void MATRIX::GetQuaternionGen(QUATERNION& Quat) const
{	
	MATRIX_GET_GENERIC_DATA();

	Float32	rTrace, s, q[4], x, y, z, w;
	int		i, j, k;
	
	int	nNxt[3] = {1, 2, 0};
	
	rTrace = _r[0][0] + _r[1][1] + _r[2][2];
	
	if(rTrace > 0.0f)
	{
		s = MATH::Sqrt(rTrace + 1.0f);
		w = s / 2.0f;
		s = 0.5f / s;
		x = (_r[1][2] - _r[2][1]) * s;
		y = (_r[2][0] - _r[0][2]) * s;
		z = (_r[0][1] - _r[1][0]) * s;
		Quat.SetWXYZ(w, x, y, z);
	}
	else
	{
		i = 0;
		
		if(_r[1][1] > _r[0][0])
		{
			i = 1;
		}
		if(_r[2][2] > _r[i][i])
		{
			i = 2;
		}
		
		j = nNxt[i];
		k = nNxt[j];
		
		s = MATH::Sqrt((_r[i][i] - (_r[j][j] + _r[k][k])) + 1.0f);
		
		q[i] = s * 0.5f;
		
		if(s != 0.0f)
		{
			s = 0.5f / s;
		}
		
		q[3] = (_r[j][k] - _r[k][j]) * s;
		q[j] = (_r[i][j] + _r[j][i]) * s;
		q[k] = (_r[i][k] + _r[k][i]) * s;
		Quat.SetWXYZ(q[3], q[0], q[1], q[2]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		Apply
//	Object:		
//	01-09-26:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::ApplyGen(const VECTOR &	Src,
						VECTOR &			Res) const
{
	MATRIX_GET_GENERIC_DATA();

	Res.SetX(_r[0][0] * Src.GetX() + _r[1][0] * Src.GetY() + _r[2][0] * Src.GetZ() + _r[3][0] * Src.GetPad());
	Res.SetY(_r[0][1] * Src.GetX() + _r[1][1] * Src.GetY() + _r[2][1] * Src.GetZ() + _r[3][1] * Src.GetPad());
	Res.SetZ(_r[0][2] * Src.GetX() + _r[1][2] * Src.GetY() + _r[2][2] * Src.GetZ() + _r[3][2] * Src.GetPad());
	Res.SetPad(_r[0][3] * Src.GetX() + _r[1][3] * Src.GetY() + _r[2][3] * Src.GetZ() + _r[3][3] * Src.GetPad());
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Transpose3x3
// Object: Transposition of the 3x3 part of the 4x4 MATRIX
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::Transpose3x3Gen(const MATRIX & ParamMat)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Mat, ParamMat);

	_r[0][0] = Mat._r[0][0];
	_r[0][1] = Mat._r[1][0];
	_r[0][2] = Mat._r[2][0];
	_r[1][0] = Mat._r[0][1];
	_r[1][1] = Mat._r[1][1];
	_r[1][2] = Mat._r[2][1];	
	_r[2][0] = Mat._r[0][2];
	_r[2][1] = Mat._r[1][2];
	_r[2][2] = Mat._r[2][2];
	
	_r[0][3] = Mat._r[0][3];
	_r[1][3] = Mat._r[1][3];
	_r[2][3] = Mat._r[2][3];
	
	_r[3][0] = Mat._r[3][0];
	_r[3][1] = Mat._r[3][1];
	_r[3][2] = Mat._r[3][2];
	_r[3][3] = Mat._r[3][3];
}

			
//-----------------------------------------------------------------------------
//	Name:		MulProject
//	Object:		
//	03-02-07:	RMA - Created
//-----------------------------------------------------------------------------

void	MATRIX::MulProject(const VECTOR& In, VECTOR& Out) const
{
	MATRIX_GET_GENERIC_DATA();


	Out[0] = (In[0] * _r[0][0] + In[1] * _r[1][0] + In[2] * _r[2][0] + _r[3][0]);
	Out[1] = (In[0] * _r[0][1] + In[1] * _r[1][1] + In[2] * _r[2][1] + _r[3][1]);
	Out[2] = (In[0] * _r[0][2] + In[1] * _r[1][2] + In[2] * _r[2][2] + _r[3][2]);
	Float32 w = (In[0] * _r[0][3] + In[1] * _r[1][3] + In[2] * _r[2][3] + _r[3][3]);

	if (w==0) 
	{
		w = MATH::EPSILON();
	}

	Out /= w;
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
