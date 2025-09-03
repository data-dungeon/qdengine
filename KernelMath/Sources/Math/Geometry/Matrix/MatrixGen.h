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
// CLASS: MATRIX_SLOW
//
//
//00-11-08 GHP Created: 
//*****************************************************************************

//=============================================================================
// Name: MATRIX
// 00-11-08 GHP Created: 
//=============================================================================

public:
	_INLINE_	Float32		GetElementGen(UInt8 nLin, UInt8 nCol) const;
	_INLINE_	Float32&	GetElementGen(UInt8 nLin, UInt8 nCol);

	_INLINE_	MATRIX &	AssignGen(const MATRIX & Mat);
	_INLINE_	void		CopyGen(const MATRIX & Mat);	
	_INLINE_	void		TransposeGen();	

				void		TransformPositionGen(const VECTOR& Src, VECTOR& Dst) const;
				void		TransformDirectionGen(const VECTOR& Src, VECTOR& Dst) const;
	_INLINE_	void		TransformPositionGen(VECTOR & V) const;
	_INLINE_	void		TransformDirectionGen(VECTOR & V) const;
				void		BatchTransformPositionGen(const VECTOR *pSrc, VECTOR *pDst, unsigned int *pIndices, int nNbrVertices) const;
				void		BatchTransformDirectionGen(const VECTOR *pSrc, VECTOR *pDst, unsigned int *pIndices, int nNbrVertices) const;

				void		MulGen(const MATRIX & M1, const MATRIX & M2);
				void		MulGen(const MATRIX & M);
				void		MulGen(Float32 r);
				void		MulAffineGen(const MATRIX & M1, const MATRIX & M2);
				void		MulAffineGen(const MATRIX & M);
				void		MulAffineGen(Float32 r);

				void		AddGen(const MATRIX & M1, const MATRIX & M2);
				void		AddGen(const MATRIX & M);

				void		AddAffineGen(const MATRIX & M1, const MATRIX & M2);
				void		AddAffineGen(const MATRIX & M);
				void		SquareGen();
				void		SquareAffineGen();
				void		InverseGen(const MATRIX& MatrixSource);
				void		InverseAffineGen(const MATRIX & Mat);
				void		InverseAffineNoScaleGen(const MATRIX & Mat);
				void		GetTransposeGen(MATRIX & Transpose);

				void		SetRotXYZGen(Float32 rX, Float32 rY, Float32 rZ);
				void		Mul3x3Gen(const MATRIX & Mat1, const MATRIX & Mat2);
				void		GetQuaternionGen(QUATERNION& Quat) const;
	_INLINE_	void		SetQuaternionGen(const QUATERNION & Quat);
				void		ApplyGen(const VECTOR & Src, VECTOR & Res) const;
				void		Transpose3x3Gen(const MATRIX & Mat);

//=============================================================================
// CODE ENDS HERE
//=============================================================================
