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
// CLASS: VECTOR
//
//
//00-11-10 JVA Created: 
//*****************************************************************************


//=============================================================================
// Name: VECTOR
//00-11-10 JVA Created: 
//=============================================================================

// Macros that casts the platform data structure into the generic data structure (for generic code compatibility)
// Call this macro at the beginning of every methods that manipulates _r directly.
#define VECTOR_GET_GENERIC_DATA() Float32 (&_r) [4] = (Float32 (&)[4]) _V;

struct TV
{ 
	Float32 (&_r) [4];
	
	TV(const VECTOR &	V):_r((Float32 (&)[4]) V){}
};

#define VECTOR_GET_GENERIC_DATA_PARAM(V, PARAM) TV V(PARAM);

private:
	_INLINE_					VECTOR(const Vector3	&V);
	_INLINE_	void			ConstructPS3(const VECTOR& V);
	_INLINE_	void			ConstructPS3(const float rVx, const float rVy, const float rVz);
	_INLINE_	void			InterpolatePS3(float rT, const VECTOR& V1, const VECTOR& V2);

	_INLINE_	VECTOR&			AssignPS3(const VECTOR& Vector);
	_INLINE_	void			CopyPS3(const VECTOR & V);

	_INLINE_	float			LengthPS3() const;
	_INLINE_	float			SquareLengthPS3() const;

	_INLINE_	void			AbsolutePS3();
	_INLINE_	void			AddMulPS3(const VECTOR &V, float r);
	_INLINE_	void			AddPS3(const VECTOR &p, const float s);
	_INLINE_	void			AddPS3(const VECTOR &V1, const VECTOR &V2);
	_INLINE_	void			AddPS3(const VECTOR &V);
	_INLINE_	void			AddPS3(float r);
	_INLINE_	void			SubPS3(const VECTOR &V1, const VECTOR &V2);
	_INLINE_	void			SubPS3(const VECTOR &V);
	_INLINE_	void			SubPS3(float r);
	_INLINE_	void			NegPS3(const VECTOR &V);
	_INLINE_	void			NegPS3();
	_INLINE_	void			MulPS3(const VECTOR &V, const float rValue);
	_INLINE_	void			MulPS3(const float rValue);

	_INLINE_	VECTOR &		OpAddAssignPS3(const VECTOR &V);
	_INLINE_	VECTOR 			OpAddPS3(const VECTOR &V) const;
	_INLINE_	VECTOR &		OpSubAssignPS3(const VECTOR &V);
	_INLINE_	VECTOR 			OpSubPS3(const VECTOR &V) const;
	_INLINE_	VECTOR			OpMinusUnaryPS3() const;
	_INLINE_	VECTOR &		OpMulAssignPS3(float rValue);
	_INLINE_	VECTOR			OpMulPS3(float rValue) const;
	_INLINE_	VECTOR			OpDivPS3(const float rValue) const;
	_INLINE_	VECTOR	&		OpDivAssignPS3(const float rValue);
	_INLINE_	void			CrossPS3(const VECTOR &V1, const VECTOR &V2);
	_INLINE_	float			DotPS3(const VECTOR &V) const;
	_INLINE_	void			ZeroPS3();

private:
	// Put platform specific Data structure here
	Vector3	_V;

//=============================================================================
// CODE ENDS HERE
//=============================================================================

