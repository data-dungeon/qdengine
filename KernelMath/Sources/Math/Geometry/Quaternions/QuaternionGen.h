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
// CLASS: QUATERNION
//
//
//00-11-09 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: QUATERNION
//00-11-09 JVA Created: 
//=============================================================================

public:
				void			SetRotationGen(const VECTOR & V1, const VECTOR & V2);
				void			SetFromMatrixGen(const MATRIX & Matrix);
				void			SetVectRollGen(const VECTOR & V, const Float32 rRoll);
	_INLINE_	QUATERNION&		AssignGen(const QUATERNION &	Q);
	_INLINE_	QUATERNION		MulConstGen(const QUATERNION & Q) const;
	_INLINE_	QUATERNION&		MulAssignGen(const QUATERNION & Q);
	_INLINE_	QUATERNION&		AddAssignGen(const QUATERNION & Q);
	_INLINE_	QUATERNION&		DivAssignGen(const Float32 rVal);
	_INLINE_	QUATERNION		MulGen(const Float32 rVal ) const;
	_INLINE_	void			MulGen(const QUATERNION & Q1, const QUATERNION & Q2);
	_INLINE_	void			MulGen(const QUATERNION &	Q);
	_INLINE_	void			InterLinearGen(const Float32 rLerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2, bool bNormalize);
	_INLINE_	void			InterSLERPGen(const Float32 rSlerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2);
	_INLINE_	void			BasicInterLinearGen(const Float32 rLerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2, bool bNormalize);
	_INLINE_	void			BasicInterSLERPGen(const Float32 rSlerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2);
	_INLINE_	void			NormalizeGen();
	_INLINE_	Float32			Norm2Gen() const;
	_INLINE_	void			InverseGen(const QUATERNION &Q);
	_INLINE_	QUATERNION&		InverseGen();
	_INLINE_	QUATERNION&		MulAssignGen(const Float32 rVal);
	_INLINE_	QUATERNION		AddGen(const QUATERNION& Q) const;
	_INLINE_	QUATERNION		SubGen(const QUATERNION& Q) const;
				VECTOR			MulGen(const VECTOR& V) const;
	_INLINE_	Float32			DotGen(const QUATERNION& Q) const;
	_INLINE_	void			MulGen(const VECTOR &	V, const Float32	rValue );

	static		QUATERNION		SlerpGen(const Float32 rCoeff, const QUATERNION& Q1, const QUATERNION& Q2);
	static		QUATERNION		SquadGen(const Float32 rCoeff, const QUATERNION& Q1, const QUATERNION& Q2, const QUATERNION&, const QUATERNION&);

//=============================================================================
// CODE ENDS HERE
//=============================================================================
