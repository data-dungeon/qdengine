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
// 00-11-09 JVA Created: 
//
//*****************************************************************************

//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		If bFlagInit is true, initialize the quaternion with the Identity
//					Else don't initialize any members
//	00-11-13:	JVA - Created
//	01-01-08:	ONA	- Added the boolean flag to increase the performance with a better robustness
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION()
{
	Id();		//TODO : Remove this ... please ! :-/
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		If bFlagInit is true, initialize the quaternion with the Identity
//					Else don't initialize any members
//	00-11-13:	JVA - Created
//	01-01-08:	ONA	- Added the boolean flag to increase the performance with a better robustness
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(bool	bSetIdentity)
{
	QDT_ASSERT( bSetIdentity == false );
	if (bSetIdentity == true)
		Id();
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		If bFlagInit is true, initialize the quaternion with the Identity
//					Else don't initialize any members
//	00-11-13:	JVA - Created
//	01-01-08:	ONA	- Added the boolean flag to increase the performance with a better robustness
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(OBJECT_TYPE eObjectType)
{
	QDT_ASSERT( eObjectType == VM_OBJECT );

	Id();
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		Copy Contructor
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Changed the call to Set unstead of Build
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const QUATERNION& Q)
{
	Set(Q);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		Copy Contructor
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Changed the call to Set unstead of Build
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const QUATERNION& Q, OBJECT_TYPE eObjectType)
{
	QDT_ASSERT( eObjectType == VM_OBJECT );

	Set(Q);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		Construct with the menbers initialization
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Changed the call to Set unstead of Build
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const Float32 rW, const Float32 rX, const Float32 rY, const Float32 rZ)
{
	SetWXYZ(rW, rX, rY, rZ);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		Construct from an 3D Angle
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Changed the call to Set unstead of Build
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const ANGLE3D& Angle3D)
{
	Set(Angle3D); 
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION constructor
//	Object:		Construct from a vector and an angle
//	00-11-13:	JVA - Created: 
//	01-01-08:	ONA - Changed the call to Set unstead of Build
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const VECTOR& vDir, const Float32 rRoll)
{
	SetVectRoll(vDir, rRoll);
}

//-----------------------------------------------------------------------------
// Name: QUATERNION constructor
// Object: COnstructor from a rotation defined by 2 vectors
//01-01-16 RMA Created: 
//-----------------------------------------------------------------------------
QUATERNION::QUATERNION(const VECTOR& V1, const VECTOR& V2)
{
	SetRotation(V1, V2);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::Id
//	Object:		Initialize with Identity(1.0f, 0.0f, 0.0f, 0.0f) the quaternion
//	01-01-08:	ONA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::Id()
{
	SlowId();
}

//-----------------------------------------------------------------------------
//	Name:		SlowId
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowId()
{
	SetWXYZ(1.0f, 0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::Set
//	Object:		Initialize by a quaternion
//	00-11-13:	JVA - Created
//	01-01-08;	ONA - Name changed (was Build)
//-----------------------------------------------------------------------------
void	QUATERNION::Set(const QUATERNION& Q)
{
	SlowSet(Q);
}

//-----------------------------------------------------------------------------
//	Name:		SlowSet
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowSet(const QUATERNION &	Q)
{
	QUAT_GET_GENERIC_DATA();

	Q.GetWXYZ(_r[_W_], _r[_X_], _r[_Y_], _r[_Z_]);
}


//-----------------------------------------------------------------------------
//	Name:		QUATERNION::SetWXYZ
//	Object:		Initialize by members
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Name changed (was Build)
//-----------------------------------------------------------------------------
void	QUATERNION::SetWXYZ(const Float32	rW, const Float32 rX, const Float32 rY, const Float32 rZ)
{
	SlowSetWXYZ(rW, rX, rY, rZ);
}

//-----------------------------------------------------------------------------
//	Name:		SlowSetWXYZ
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowSetWXYZ(const Float32	rW, const Float32	rX, const Float32	rY, const Float32	rZ)
{
	QUAT_GET_GENERIC_DATA();

	_r[_W_] = rW;
	_r[_X_] = rX;
	_r[_Y_] = rY;
	_r[_Z_] = rZ;
}


//-----------------------------------------------------------------------------
//	Name: QUATERNION::SetX
//	Object: Set the value of X
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetX(Float32 rValue)
{
	QUAT_GET_GENERIC_DATA();

	_r[_X_] = rValue ;
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::SetY
//	Object: Set the value of 
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetY(Float32 rValue)
{
	QUAT_GET_GENERIC_DATA();

	_r[_Y_] = rValue ;
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::SetZ
//	Object: Set the value of X
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetZ(Float32 rValue)
{
	QUAT_GET_GENERIC_DATA();

	_r[_Z_] = rValue ;
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::SetW
//	Object: Set the value of W
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetW(Float32 rValue)
{
	QUAT_GET_GENERIC_DATA();

	_r[_W_] = rValue ;
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::GetWXYZ
//	Object:		Get the members of the quaternion
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::GetWXYZ(Float32& rW, Float32& rX, Float32& rY, Float32& rZ) const
{
	QUAT_GET_GENERIC_DATA();

	rW = _r[_W_];
	rX = _r[_X_];
	rY = _r[_Y_];
	rZ = _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::GetX
//	Object: Get the X value
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::GetX() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_X_]) ;
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::GetY
//	Object: Get the Y value
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::GetY() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_Y_]) ;
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::GetZ
//	Object: Get the Z value
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::GetZ() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_Z_]) ;
}

//-----------------------------------------------------------------------------
//	Name: QUATERNION::GetW
//	Object: Get the W value
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::GetW() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_W_]) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetXRef
//	Object:		Get the X value
//
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32& QUATERNION::GetXRef() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_X_]) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetYRef
//	Object:		Get the Y value
//
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32& QUATERNION::GetYRef() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_Y_]) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetZRef
//	Object:		Get the Z value
//
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32& QUATERNION::GetZRef() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_Z_]) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetWRef
//	Object:		Get the W value
//
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32& QUATERNION::GetWRef() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_W_]) ;
}

//-----------------------------------------------------------------------------
//	Name:		SlowAssign
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::SlowAssign(const QUATERNION& Q)
{
	if ( this != & Q )
	{
		SlowSet( Q );
	}

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::operator ==
//	Object:		Equality operator
//	00-12-04:	GHP - Created
//-----------------------------------------------------------------------------
bool	QUATERNION::operator ==(const QUATERNION& Q) const
{
	QUAT_GET_GENERIC_DATA();

	if	(( MATH::EqualsEpsilon( _r[_W_], Q[_W_] ) == true ) &&
		 ( MATH::EqualsEpsilon( _r[_X_], Q[_X_] ) == true ) &&
		 ( MATH::EqualsEpsilon( _r[_Y_], Q[_Y_] ) == true ) &&
		 ( MATH::EqualsEpsilon( _r[_Z_], Q[_Z_] ) == true ))
	{
		return ( true );
	}

	if	(( MATH::EqualsEpsilon( -_r[_W_], Q[_W_] ) == true ) &&
		 ( MATH::EqualsEpsilon( -_r[_X_], Q[_X_] ) == true ) &&
		 ( MATH::EqualsEpsilon( -_r[_Y_], Q[_Y_] ) == true ) &&
		 ( MATH::EqualsEpsilon( -_r[_Z_], Q[_Z_] ) == true ))
	{
		return ( true );
	}

	return ( false );
}

//-----------------------------------------------------------------------------
//	Name:		EqualWithError
//	Object:		
//	01-07-27:	GHP - Created
//-----------------------------------------------------------------------------
bool	QUATERNION::EqualWithError(const QUATERNION& Q, const Float32	rError) const
{
	QUAT_GET_GENERIC_DATA();

	return (	( MATH::EqualsEpsilon( _r[_W_], Q[_W_], rError ) == true ) &&
				( MATH::EqualsEpsilon( _r[_X_], Q[_X_], rError ) == true ) &&
				( MATH::EqualsEpsilon( _r[_Y_], Q[_Y_], rError ) == true ) &&
				( MATH::EqualsEpsilon( _r[_Z_], Q[_Z_], rError ) == true ) );
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::operator !=
//	Object:		Difference operator
//	01-01-08:	ONA - Created
//-----------------------------------------------------------------------------
bool	QUATERNION::operator !=(const QUATERNION& Q) const
{
	QUAT_GET_GENERIC_DATA();

	return ( ( MATH::EqualsEpsilon( _r[_W_], Q[_W_] ) == false ) ||
			 ( MATH::EqualsEpsilon( _r[_X_], Q[_X_] ) == false ) ||
			 ( MATH::EqualsEpsilon( _r[_Y_], Q[_Y_] ) == false ) ||
			 ( MATH::EqualsEpsilon( _r[_Z_], Q[_Z_] ) == false ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowMulConst
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SlowMulConst(const QUATERNION& Q) const
{
	QUATERNION	QResult(false);
	QResult.SlowMul(*this, Q);
	return	(QResult);
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowMul(const VECTOR& V, const Float32 rValue)
{
	QUAT_GET_GENERIC_DATA();

	_r[_X_] = V[_X_] * rValue;
	_r[_Y_] = V[_Y_] * rValue;
	_r[_Z_] = V[_Z_] * rValue;
}

//-----------------------------------------------------------------------------
//	Name:		Inverse
//	Object:		
//	01-03-15:	ONA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::Inverse(const QUATERNION& Q)
{
	QUATERNION tempQ( Q ) ; 
	tempQ.Inverse() ;
	Set( tempQ ) ;
}

//-----------------------------------------------------------------------------
// Name: QUATERNION::Inverse
// Object: 
//01-05-04 GHP Created: 
//-----------------------------------------------------------------------------
QUATERNION&	QUATERNION::Inverse() 
{
	QUAT_GET_GENERIC_DATA();

	Float32	rInvNorm2 = 1.f / Norm2() ;

	_r[_W_] *= rInvNorm2 ; 
	_r[_X_] *= - rInvNorm2 ; 
	_r[_Y_] *= - rInvNorm2 ; 
	_r[_Z_] *= - rInvNorm2 ; 

	return ( *this ) ;
}

//-----------------------------------------------------------------------------
//	Name:		UnitInverse
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
void QUATERNION::UnitInverse(const QUATERNION& Q)
{
	QDT_ASSERT(Q.IsUnit());

	SlowUnitInverse(Q);
}

//-----------------------------------------------------------------------------
//	Name:		SlowUnitInverse
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowUnitInverse(const QUATERNION& Q)
{
	QUAT_GET_GENERIC_DATA();

	_r[_X_] = - Q[_X_];
	_r[_Y_] = - Q[_Y_];
	_r[_Z_] = - Q[_Z_]; 
}

//-----------------------------------------------------------------------------
//	Name:		UnitInverse
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::UnitInverse() 
{
	QDT_ASSERT(IsUnit());

	return	(SlowUnitInverse());
}

//-----------------------------------------------------------------------------
//	Name:		SlowUnitInverse
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION &	QUATERNION::SlowUnitInverse()
{
	QUAT_GET_GENERIC_DATA();

	_r[_X_] = - _r[_X_]; 
	_r[_Y_] = - _r[_Y_]; 
	_r[_Z_] = - _r[_Z_]; 

	return (*this);
}

//-----------------------------------------------------------------------------
// Name: QUATERNION::IsNormalized
// Object: 
//01-05-03 GHP Created: 
//-----------------------------------------------------------------------------
bool QUATERNION::IsNormalized() const
{ 
	Float32  rNorm =  Norm2();

	return ( MATH::Abs(rNorm - 1.0f) < 0.01f ) ;
}

//-----------------------------------------------------------------------------
//	Name:		IsPure
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
bool QUATERNION::IsPure() const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_W_] == 0.f);
}

//-----------------------------------------------------------------------------
//	Name:		IsUnit
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
bool QUATERNION::IsUnit() const 
{
	Float32  rNorm =  Norm2();
	
	return ( MATH::Abs(rNorm - 1.0f) < MATH::EPSILON() ) ;
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SlowMul(const Float32	rVal) const
{
	QUAT_GET_GENERIC_DATA();

	return ( QUATERNION(  _r[_W_] * rVal, _r[_X_] * rVal , _r[_Y_] * rVal, _r[_Z_] * rVal) ) ; 
}

QUATERNION	QUATERNION::SlowMulNoConst(const Float32	rVal)
{
	QUAT_GET_GENERIC_DATA();

	return ( QUATERNION(  _r[_W_] * rVal, _r[_X_] * rVal , _r[_Y_] * rVal, _r[_Z_] * rVal) ) ; 
}

//-----------------------------------------------------------------------------
//	Name:		SlowMulAssign
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::SlowMulAssign(const QUATERNION& Q)
{
	SlowMul(Q);

	return	(*this);
}

//-----------------------------------------------------------------------------
//	Name:		SlowAddAssign
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::SlowAddAssign(const QUATERNION& Q)
{
	QUAT_GET_GENERIC_DATA();

	_r[_X_] += Q[_X_];
	_r[_Y_] += Q[_Y_];
	_r[_Z_] += Q[_Z_];
	_r[_W_] += Q[_W_];

	return  ( *this ) ;
}

//-----------------------------------------------------------------------------
//	Name:		SlowDivAssign
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::SlowDivAssign(const Float32 rVal)
{
	QUAT_GET_GENERIC_DATA();

	Float32 rInvVal = 1.f / rVal;

	_r[_X_] *= rInvVal;
	_r[_Y_] *= rInvVal;
	_r[_Z_] *= rInvVal;
	_r[_W_] *= rInvVal;

	return  ( *this ) ;
}


//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-11-30:	JLT - Created
//-----------------------------------------------------------------------------
Float32 QUATERNION::operator[](const unsigned int i) const
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT(i<4);
	return (_r[i]);
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-11-30:	JLT - Created
//-----------------------------------------------------------------------------
Float32& QUATERNION::operator[](const unsigned int i)
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT(i<4);
	return (_r[i]);
}

//-----------------------------------------------------------------------------
//	Name:		Norm
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
Float32 QUATERNION::Norm() const
{
	return (MATH::Sqrt(Norm2()));
}

//-----------------------------------------------------------------------------
//	Name:		SlowMulAssign
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION&	QUATERNION::SlowMulAssign(const Float32 K)
{
	*this = SlowMulNoConst( K );

	return *this;
}

//-----------------------------------------------------------------------------
//	Name:		SlowDot
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
Float32	QUATERNION::SlowDot(const QUATERNION& Q) const
{
	QUAT_GET_GENERIC_DATA();

	return (_r[_X_]*Q[_X_] + _r[_Y_]*Q[_Y_] + _r[_Z_]*Q[_Z_] + _r[_W_]*Q[_W_]);
}


//-----------------------------------------------------------------------------
//	Name:		GetAngleWith
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
Float32 QUATERNION::GetAngleWith(const QUATERNION& Q) const
{
	return (MATH::ACos(this->Dot(Q)));
}

//-----------------------------------------------------------------------------
//	Name:		SlowAdd
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SlowAdd(const QUATERNION& Q) const
{
	QUAT_GET_GENERIC_DATA();

	return ( QUATERNION( _r[_W_] + Q[_W_], _r[_X_] + Q[_X_], _r[_Y_] + Q[_Y_], _r[_Z_] + Q[_Z_] ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowSub
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SlowSub(const QUATERNION& Q) const
{
	QUAT_GET_GENERIC_DATA();

	return ( QUATERNION( _r[_W_] - Q[_W_], _r[_X_] - Q[_X_], _r[_Y_] - Q[_Y_], _r[_Z_] - Q[_Z_] ) );
}

//-----------------------------------------------------------------------------
//	Name:		GetAxis
//	Object:		
//	01-12-19:	JLT - Created
//-----------------------------------------------------------------------------
VECTOR QUATERNION::GetAxis() const
{
	return	(SlowGetAxis());
}

//-----------------------------------------------------------------------------
//	Name:		SlowGetAxis
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR	QUATERNION::SlowGetAxis() const
{
	QUAT_GET_GENERIC_DATA();

	VECTOR V;

	V[_X_] = _r[_X_];
	V[_Y_] = _r[_Y_];
	V[_Z_] = _r[_Z_];

	V.Normalize();

	return V;
}

//-----------------------------------------------------------------------------
//	Name:		IsIdentity
//	Object:		
//	01-12-20:	JLT - Created
//-----------------------------------------------------------------------------
bool QUATERNION::IsIdentity() const
{
	QUAT_GET_GENERIC_DATA();

	return (IsUnit() && MATH::Abs(_r[_W_] - 1.f) < MATH::EPSILON());
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	02-06-14:	JLT - Created
//-----------------------------------------------------------------------------
const Float32* QUATERNION::ToPointer() const
{
	QUAT_GET_GENERIC_DATA();

	return (&_r[_X_]);
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	02-06-14:	JLT - Created
//-----------------------------------------------------------------------------
Float32* QUATERNION::ToPointer()
{
	QUAT_GET_GENERIC_DATA();

	return (&_r[_X_]);
}

//-----------------------------------------------------------------------------
//	Name:		GetAngle3D
//	Object:		
//	03-10-23:	CVI - Created
//-----------------------------------------------------------------------------
ANGLE3D	QUATERNION::GetAngle3D() const
{
	//	TO DO : Implement optimized algo 
	MATRIX		M;
	VECTOR		V;
	ANGLE3D		A;

	M.Identity();

	M.SetQuaternion( *this );
	V = M.GetRotXYZ();

	A.Set( V.GetX(), V.GetY(), V.GetZ() );
	return ( A );
}


//=============================================================================
// CODE ENDS HERE
//=============================================================================
