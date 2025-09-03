
//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QUATERNION
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const Quat & Q)
{
	_Q = Q;
}

//-----------------------------------------------------------------------------
//	Name:		SetRotationPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetRotationPS3(const VECTOR &	V1,
								   const VECTOR &	V2)
{
	_Q = Quat::rotation(V1._V, V2._V);
}

//-----------------------------------------------------------------------------
//	Name:		SetFromMatrixPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetFromMatrixPS3(const MATRIX &	Matrix)
{
	_Q = Quat(Matrix._M.getUpper3x3());
}

//-----------------------------------------------------------------------------
//	Name:		SetVectRollPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetVectRollPS3(const VECTOR &	V,
								   const Float32		rRoll)
{
	_Q = Quat::rotation(rRoll, V._V);
}

//-----------------------------------------------------------------------------
//	Name:		AssignPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION &	QUATERNION::AssignPS3(const QUATERNION &	Q)
{
	_Q = Q._Q;
}

//-----------------------------------------------------------------------------
//	Name:		MulConstPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::MulConstPS3(const QUATERNION &	Q) const
{
	return QUATERNION(_Q * Q._Q);
}

//-----------------------------------------------------------------------------
//	Name:		MulAssignPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION &	QUATERNION::MulAssignPS3(const QUATERNION &	Q)
{
	_Q *= Q._Q;
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		AddAssignPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION &	QUATERNION::AddAssignPS3(const QUATERNION &	Q)
{
	_Q += Q._Q;
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		DivAssignPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION &	QUATERNION::DivAssignPS3(const Float32	rVal)
{
	_Q /= rVal;
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::MulPS3(const Float32	rVal) const
{
	return QUATERNION(_Q * rVal);
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::MulPS3(const QUATERNION &	Q1,
						   const QUATERNION &	Q2)
{
	_Q = Q1._Q * Q2._Q;
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::MulPS3(const QUATERNION &	Q)
{
	_Q *= Q._Q;
}

//-----------------------------------------------------------------------------
//	Name:		InterLinearPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::InterLinearPS3(const Float32			rLerpCoeff,
								   const QUATERNION &	Q1,
								   const QUATERNION &	Q2,
								   bool					bNormalize)
{
	_Q = lerp( rLerpCoeff, Q1._Q, Q2._Q);

	if (bNormalize)
	{
		NormalizePS3();
	}
}

//-----------------------------------------------------------------------------
//	Name:		InterSLERPPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::InterSLERPPS3(const Float32			rSlerpCoeff,
								  const QUATERNION &	Q1,
								  const QUATERNION &	Q2)
{
	_Q = slerp( rSlerpCoeff, Q1._Q, Q2._Q);
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterLinearPS3
//	Object:		
//	06-03-14:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::BasicInterLinearPS3(const Float32			rLerpCoeff,
										const QUATERNION &	Q1,
										const QUATERNION &	Q2,
										bool				bNormalize)
{
	_Q = lerp( rLerpCoeff, Q1._Q, Q2._Q);

	if (bNormalize)
	{
		NormalizePS3();
	}
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterSLERPPS3
//	Object:		
//	06-03-14:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::BasicInterSLERPPS3(const Float32			rSlerpCoeff,
									   const QUATERNION &	Q1,
									   const QUATERNION &	Q2)
{
	_Q = slerp( rSlerpCoeff, Q1._Q, Q2._Q);
}

//-----------------------------------------------------------------------------
//	Name:		NormalizePS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::NormalizePS3()
{
	_Q = normalize(_Q);
}

//-----------------------------------------------------------------------------
//	Name:		Norm2PS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::Norm2PS3() const
{
	return norm(_Q);
}

//-----------------------------------------------------------------------------
//	Name:		MulAssignPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION &	QUATERNION::MulAssignPS3(const Float32	rVal)
{
	_Q *= rVal;
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		AddPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::AddPS3(const QUATERNION &	Q) const
{
	return QUATERNION(_Q + Q._Q);
}

//-----------------------------------------------------------------------------
//	Name:		SubPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SubPS3(const QUATERNION &	Q) const
{
	return QUATERNION(_Q - Q._Q);
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
VECTOR	QUATERNION::MulPS3(const VECTOR &	V) const
{
	return VECTOR(rotate(_Q, V._V));
}

//-----------------------------------------------------------------------------
//	Name:		DotPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::DotPS3(const QUATERNION &	Q) const
{
	return (dot(_Q, Q._Q));
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::MulPS3(const VECTOR &	V,
						   const Float32		rValue)
{
	_Q = Quat(Vector4(V._V * rValue));
}


//-----------------------------------------------------------------------------
//	Name:		SlerpPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SlerpPS3(const Float32		rCoeff,
								 const QUATERNION &	Q1,
								 const QUATERNION &	Q2)
{
	return QUATERNION(slerp(rCoeff, Q1._Q, Q2._Q));
}

//-----------------------------------------------------------------------------
//	Name:		SquadPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SquadPS3(const Float32		rCoeff,
								 const QUATERNION &	Q1,
								 const QUATERNION &	Q2,
								 const QUATERNION & Q3,
								 const QUATERNION & Q4)
{
	return QUATERNION(squad(rCoeff, Q1._Q, Q2._Q, Q3._Q, Q4._Q));
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
