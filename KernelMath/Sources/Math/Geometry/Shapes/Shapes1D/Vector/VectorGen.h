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

public:
	_INLINE_	void		ConstructGen(const VECTOR& V);
	_INLINE_	void		ConstructGen(const float rVx, const float rVy, const float rVz);
	_INLINE_	void		InterpolateGen(float rT, const VECTOR& V1, const VECTOR& V2);

	_INLINE_	VECTOR&		AssignGen(const VECTOR& Vector);
	_INLINE_	void		CopyGen(const VECTOR & V);

	_INLINE_	float		LengthGen() const;
	_INLINE_	float		SquareLengthGen() const;

	_INLINE_	void		AbsoluteGen();
	_INLINE_	void		AddMulGen(const VECTOR &V, float r);
	_INLINE_	void		AddGen(const VECTOR &p, const float s);
	_INLINE_	void		AddGen(const VECTOR &V1, const VECTOR &V2);
	_INLINE_	void		AddGen(const VECTOR &V);
	_INLINE_	void		AddGen(float r);
	_INLINE_	void		SubGen(const VECTOR &V1, const VECTOR &V2);
	_INLINE_	void		SubGen(const VECTOR &V);
	_INLINE_	void		SubGen(float r);
	_INLINE_	void		NegGen(const VECTOR &V);
	_INLINE_	void		NegGen();
	_INLINE_	void		MulGen(const VECTOR &V, const float rValue);
	_INLINE_	void		MulGen(const float rValue);

	_INLINE_	VECTOR &	OpAddAssignGen(const VECTOR &V);
	_INLINE_	VECTOR 		OpAddGen(const VECTOR &V) const;
	_INLINE_	VECTOR &	OpSubAssignGen(const VECTOR &V);
	_INLINE_	VECTOR 		OpSubGen(const VECTOR &V) const;
	_INLINE_	VECTOR		OpMinusUnaryGen() const;
	_INLINE_	VECTOR &	OpMulAssignGen(float rValue);
	_INLINE_	VECTOR		OpMulGen(float rValue) const;
	_INLINE_	VECTOR		OpDivGen(const float rValue) const;
	_INLINE_	VECTOR	&	OpDivAssignGen(const float rValue);
	_INLINE_	void		CrossGen(const VECTOR &V1, const VECTOR &V2);
	_INLINE_	float		DotGen(const VECTOR &V) const;
	_INLINE_	void		ZeroGen();

//=============================================================================
// CODE ENDS HERE
//=============================================================================

