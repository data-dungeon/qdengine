
//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::AssignGen
//	Object:		Assignation operator
//	01-01-08:	ONA - Created
//-----------------------------------------------------------------------------
QUATERNION&	QUATERNION::AssignGen(const QUATERNION &	Q)
{
	return	(SlowAssign(Q));
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::MulConstGen
//	Object:		Multiply operator
//	01-01-08:	ONA - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::MulConstGen(const QUATERNION &	Q) const
{
	QUATERNION	QResult(false);
	QResult.Mul(*this, Q);
	return	(QResult);
}

//-----------------------------------------------------------------------------
//	Name:		MulAssignGen
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::MulAssignGen(const Float32 K)
{
	return	(SlowMulAssign(K));
}

//-----------------------------------------------------------------------------
//	Name:		AddAssignGen
//	Object:		
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
QUATERNION&	 QUATERNION::AddAssignGen(const QUATERNION & Q) 
{
	return	(SlowAddAssign(Q));
}

//-----------------------------------------------------------------------------
// Name: QUATERNION::DivAssignGen
// Object: 
// 01-05-04 GHP Created: 
//-----------------------------------------------------------------------------
QUATERNION&	QUATERNION::DivAssignGen(const Float32 rVal) 
{
	return	(SlowDivAssign(rVal));
}

//-----------------------------------------------------------------------------
//	Name:		MulGen
//	Object:		
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
QUATERNION  QUATERNION::MulGen(const Float32 rVal ) const
{
	return	(SlowMul(rVal));
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::MulGen
//	Object:		Multiply two quaternion and put the result on this instance
//	00-11-13:	JVA - Created
//	[t1; u] * [t2; v] = [t1t2 - uv; t1v + t2u + u^v]
//-----------------------------------------------------------------------------
void	QUATERNION::MulGen(const QUATERNION &	Q1,
						   const QUATERNION &	Q2)
{
	SlowMul(Q1, Q2);
}

//-----------------------------------------------------------------------------
//	Name:		MulGen
//	Object:		
//	03-01-10:	SBE - Created
//-----------------------------------------------------------------------------
void	QUATERNION::MulGen(const QUATERNION &	Q)
{
	SlowMul(Q);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::InterLinearGen
//	Object:		Linear interpolation between two quaternions
//					The interpolation coefficient must be between 0.0f and 1.0f
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Several modifications, name (was InterLin), QDT_ASSERT added
//	01-03-29:	GHP Modified  Added the normalize option linear interpolation does not conserve magnitude
//-----------------------------------------------------------------------------
void	QUATERNION::InterLinearGen(const Float32			rLerpCoeff,
								   const QUATERNION &	Q1,
								   const QUATERNION &	Q2,
								   bool				    bNormalize )
{
	SlowInterLinear(rLerpCoeff, Q1, Q2, bNormalize);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::InterSLERPGen
//	Object:		Spherical Linear Interpolation between two quaternions from GGemIII p461 modified from GameDev Apr1998
//					The interpolation coefficient must be between 0.0f and 1.0f
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Several modifications, QDT_ASSERT added
//-----------------------------------------------------------------------------
void	QUATERNION::InterSLERPGen(const Float32			rSlerpCoeff,
							      const QUATERNION &	Q1,
							      const QUATERNION &	Q2)
{
	SlowInterSLERP(rSlerpCoeff, Q1, Q2);
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterLinear
//	Object:		
//	02-09-04:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION::BasicInterLinearGen(const Float32		rLerpCoeff,
									    const QUATERNION &	Q1,
									    const QUATERNION &	Q2,
									    bool				bNormalize)
{
	SlowBasicInterLinear(rLerpCoeff, Q1, Q2, bNormalize);
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterSLERP
//	Object:		
//	02-09-04:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION::BasicInterSLERPGen(const Float32			rSlerpCoeff,
									   const QUATERNION &	Q1,
									   const QUATERNION &	Q2)
{
	SlowBasicInterSLERP(rSlerpCoeff, Q1, Q2);
}

//-----------------------------------------------------------------------------
//	Name:		NormalizeGen
//	Object:		
//	01-03-28:	GHP - Created
//-----------------------------------------------------------------------------
void QUATERNION::NormalizeGen() 
{
	SlowNormalize();
}

//-----------------------------------------------------------------------------
// Name: QUATERNION::Norm2Gen
// Object: 
//01-05-04 GHP Created: 
//-----------------------------------------------------------------------------
Float32  QUATERNION::Norm2Gen() const 
{
	QUAT_GET_GENERIC_DATA();

	return ( MATH::Square( _r[_X_] ) + MATH::Square( _r[_Y_] ) + MATH::Square( _r[_Z_] ) + MATH::Square( _r[_W_] ) ) ;
}

//-----------------------------------------------------------------------------
//	Name:		MulAssignGen
//	Object:		
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
QUATERNION&	 QUATERNION::MulAssignGen(const QUATERNION & Q) 
{
	Mul(Q);

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		AddGen
//	Object:		
//	03-01-10:	SBE - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::AddGen(const QUATERNION &	Q) const
{
	return (SlowAdd(Q));
}

//-----------------------------------------------------------------------------
//	Name:		SubGen
//	Object:		
//	03-01-10:	SBE - Created
//-----------------------------------------------------------------------------
QUATERNION QUATERNION::SubGen(const QUATERNION &Q) const
{
	return	(SlowSub(Q));
}

//-----------------------------------------------------------------------------
//	Name:		DotGen
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
Float32 QUATERNION::DotGen(const QUATERNION &Q) const
{
	return	(SlowDot(Q));
}

//-----------------------------------------------------------------------------
//	Name:		MulGen
//	Object:		
//	03-01-10:	SBE - Created
//-----------------------------------------------------------------------------
void	QUATERNION::MulGen(const VECTOR &	V,
						   const Float32	rValue )
{
	SlowMul(V, rValue);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
