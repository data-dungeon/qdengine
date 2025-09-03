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

// Macros that casts the platform data structure into the generic data structure (for generic code compatibility)
// Call this macro at the beginning of every methods that manipulates _r directly.
#define QUAT_GET_GENERIC_DATA() Float32 (&_r) [4] = (Float32 (&)[4]) _Q;

struct TQ
{ 
	Float32 (&_r) [4];
	
	TQ(const QUATERNION &	Q):_r((Float32 (&)[4]) Q){}
};

#define QUAT_GET_GENERIC_DATA_PARAM(Q, PARAM) TQ Q(PARAM);

	private:
		_INLINE_					QUATERNION(const Quat	&Q);
		_INLINE_	void			SetRotationPS3(const VECTOR & V1, const VECTOR & V2);
		_INLINE_	void			SetFromMatrixPS3(const MATRIX & Matrix);
		_INLINE_	void			SetVectRollPS3(const VECTOR & V, const Float32 rRoll);
		_INLINE_	QUATERNION&		AssignPS3(const QUATERNION &	Q);
		_INLINE_	QUATERNION		MulConstPS3(const QUATERNION & Q) const;
		_INLINE_	QUATERNION&		MulAssignPS3(const QUATERNION & Q);
		_INLINE_	QUATERNION&		AddAssignPS3(const QUATERNION & Q);
		_INLINE_	QUATERNION&		DivAssignPS3(const Float32 rVal);
		_INLINE_	QUATERNION		MulPS3(const Float32 rVal ) const;
		_INLINE_	void			MulPS3(const QUATERNION & Q1, const QUATERNION & Q2);
		_INLINE_	void			MulPS3(const QUATERNION &	Q);
		_INLINE_	void			InterLinearPS3(const Float32 rLerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2, bool bNormalize);
		_INLINE_	void			InterSLERPPS3(const Float32 rSlerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2);
		_INLINE_	void			BasicInterLinearPS3(const Float32 rLerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2, bool bNormalize);
		_INLINE_	void			BasicInterSLERPPS3(const Float32 rSlerpCoeff, const QUATERNION & Q1, const QUATERNION & Q2);
		_INLINE_	void			NormalizePS3();
		_INLINE_	Float32			Norm2PS3() const;
		_INLINE_	QUATERNION&		MulAssignPS3(const Float32 rVal);
		_INLINE_	QUATERNION		AddPS3(const QUATERNION& Q) const;
		_INLINE_	QUATERNION		SubPS3(const QUATERNION& Q) const;
		_INLINE_	VECTOR			MulPS3(const VECTOR& V) const;
		_INLINE_	Float32			DotPS3(const QUATERNION& Q) const;
		_INLINE_	void			MulPS3(const VECTOR &	V, const Float32	rValue );

static  _INLINE_	QUATERNION		SlerpPS3(const Float32 rCoeff, const QUATERNION& Q1, const QUATERNION& Q2);
static  _INLINE_	QUATERNION		SquadPS3(const Float32 rCoeff, const QUATERNION& Q1, const QUATERNION& Q2, const QUATERNION&, const QUATERNION&);

	private:
	// Put platform specific Data structure here
		Quat	_Q;

//=============================================================================
// CODE ENDS HERE
//=============================================================================
