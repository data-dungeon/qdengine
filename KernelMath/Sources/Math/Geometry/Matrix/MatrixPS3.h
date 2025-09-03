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
//00-11-08 GHP Created: 
//*****************************************************************************

//=============================================================================
// Name: MATRIX
// 00-11-08 GHP Created: 
//=============================================================================

// Macros that casts the platform data structure into the generic data structure (for generic code compatibility)
// Call this macro at the beginning of every methods that manipulates _r directly.
#define MATRIX_GET_GENERIC_DATA() Float32 (&_r) [4][4] = (Float32 (&)[4][4]) _M;

struct TM
{ 
	Float32 (&_r) [4][4];
	
	TM(const MATRIX &	M):_r((Float32 (&)[4][4]) M){}
};

#define MATRIX_GET_GENERIC_DATA_PARAM(M, PARAM) TM M(PARAM);

private:
	_INLINE_	Float32			GetElementPS3(UInt8 nLin, UInt8 nCol) const;

	_INLINE_	MATRIX &		AssignPS3(const MATRIX & Mat);
	_INLINE_	void			CopyPS3(const MATRIX & Mat);	
	_INLINE_	void			TransposePS3();

	_INLINE_	void			TransformPositionPS3(const VECTOR& Src, VECTOR& Dst) const;
	_INLINE_	void			TransformDirectionPS3(const VECTOR& Src, VECTOR& Dst) const;
	_INLINE_	void			TransformPositionPS3(VECTOR & V) const;
	_INLINE_	void			TransformDirectionPS3(VECTOR & V) const;
	_INLINE_	void			BatchTransformPositionPS3(const VECTOR *pSrc, VECTOR *pDst, unsigned int *pIndices, int nNbrVertices) const;
	_INLINE_	void			BatchTransformDirectionPS3(const VECTOR *pSrc, VECTOR *pDst, unsigned int *pIndices, int nNbrVertices) const;

	_INLINE_	void			MulPS3(const MATRIX & M1, const MATRIX & M2);
	_INLINE_	void			MulPS3(const MATRIX & M);
	_INLINE_	void			MulPS3(Float32 r);
	_INLINE_	void			MulAffinePS3(const MATRIX & M1, const MATRIX & M2);
	_INLINE_	void			MulAffinePS3(const MATRIX & M);
	_INLINE_	void			MulAffinePS3(Float32 r);

	_INLINE_	void			AddPS3(const MATRIX & M1, const MATRIX & M2);
	_INLINE_	void			AddPS3(const MATRIX & M);

	_INLINE_	void			AddAffinePS3(const MATRIX & M1, const MATRIX & M2);
	_INLINE_	void			AddAffinePS3(const MATRIX & M);
	_INLINE_	void			SquarePS3();
	_INLINE_	void			SquareAffinePS3();
	_INLINE_	void			InversePS3(const MATRIX& MatrixSource);
	_INLINE_	void			InverseAffinePS3(const MATRIX & Mat);
	_INLINE_	void			InverseAffineNoScalePS3(const MATRIX & Mat);
	_INLINE_	void			GetTransposePS3(MATRIX & Transpose);

	_INLINE_	void			SetRotXYZPS3(Float32 rX, Float32 rY, Float32 rZ);
	_INLINE_	void			Mul3x3PS3(const MATRIX & Mat1, const MATRIX & Mat2);
	_INLINE_	void			GetQuaternionPS3(QUATERNION& Quat) const;
	_INLINE_	void			SetQuaternionPS3(const QUATERNION & Quat);
	_INLINE_	void			ApplyPS3(const VECTOR & Src, VECTOR & Res) const;
	_INLINE_	void			Transpose3x3PS3(const MATRIX & Mat);

private:
	// Put platform specific Data structure here
	Matrix4		_M;

//=============================================================================
// CODE ENDS HERE
//=============================================================================
