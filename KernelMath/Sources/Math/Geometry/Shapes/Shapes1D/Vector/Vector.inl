//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR()
{
	// Do nothing
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR( OBJECT_TYPE eObjectType )
{
	QDT_ASSERT( eObjectType == VM_OBJECT );

	SlowZero();
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR( const VECTOR & V, OBJECT_TYPE eObjectType )
{
	QDT_ASSERT( eObjectType == VM_OBJECT );

	SlowCopy(V);
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	02-11-17:	SBE - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR( const POINT4D & Pt )
{
	SlowCopy(Pt);
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR( const POINT4D & Pt, OBJECT_TYPE eObjectType )
{
	QDT_ASSERT( eObjectType == VM_OBJECT );

	SlowCopy(Pt);
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR( const VECTOR_PACKED & V )
{
	SetFromVectorPacked( V );
}

//-----------------------------------------------------------------------------
//	Name:		VECTOR constructor
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
VECTOR::VECTOR(const VECTOR &	V,
			   const float	rNorm)
{
	SetFromVectorAndNorm( V , rNorm );
}

//-----------------------------------------------------------------------------
//	Name:		IsInRange
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
bool	VECTOR::IsInRange(const VECTOR &	Pos,
						  const float	R) const
{
	VECTOR V = (*this) - Pos;

	return (V*V < R*R);
}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-09-02:	SBE - Created
//-----------------------------------------------------------------------------
VECTOR &	VECTOR::operator=(const POINT4D &	Point)
{
	Copy(Point);

	return ( *this );
}

//-----------------------------------------------------------------------------
//	Name:		SlowAssign
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR &	VECTOR::SlowAssign(const VECTOR &	ParamVector)
{
	VECTOR_GET_GENERIC_DATA();
	VECTOR_GET_GENERIC_DATA_PARAM(Vector, ParamVector);

	_r[_X_] = Vector._r[_X_];
	_r[_Y_] = Vector._r[_Y_];
	_r[_Z_] = Vector._r[_Z_];	

	return ( *this );
}

//-----------------------------------------------------------------------------
//	Name:		operator == 
//	Object:		Equality operator
//	01-01-17:	RMA - Created
//	01-07-27:	GHP - Modified uses EqualsEpsilon
//-----------------------------------------------------------------------------
bool VECTOR::operator == ( const VECTOR & Vector ) const
{
	VECTOR_GET_GENERIC_DATA();

	return (	( MATH::EqualsEpsilon( _r[_X_], Vector[_X_], 0.001f ) == true ) &&
				( MATH::EqualsEpsilon( _r[_Y_], Vector[_Y_], 0.001f ) == true ) &&
				( MATH::EqualsEpsilon( _r[_Z_], Vector[_Z_], 0.001f ) == true ) );
}
//-----------------------------------------------------------------------------
//	Name:		EqualWithError
//	Object:		
//	01-07-27:	GHP - Created
//-----------------------------------------------------------------------------
bool	VECTOR::EqualWithError(const VECTOR &	Vector,
							   const float		rError) const
{
	VECTOR_GET_GENERIC_DATA();

	return (	( MATH::EqualsEpsilon( _r[_X_], Vector[_X_], rError ) == true ) &&
				( MATH::EqualsEpsilon( _r[_Y_], Vector[_Y_], rError ) == true ) &&
				( MATH::EqualsEpsilon( _r[_Z_], Vector[_Z_], rError ) == true ) );
}
//-----------------------------------------------------------------------------
//	Name:		operator != 
//	Object:		Difference operator
//	01-01-17:	RMA - Created
//-----------------------------------------------------------------------------
bool VECTOR::operator != ( const VECTOR & Vector ) const
{
	VECTOR_GET_GENERIC_DATA();

	if ( MATH::Abs( _r[_X_] - Vector[_X_] ) > 0.001f )
	{
		return ( true );
	}

	if ( MATH::Abs( _r[_Y_] - Vector[_Y_] ) > 0.001f )
	{
		return ( true );
	}

	if ( MATH::Abs( _r[_Z_] - Vector[_Z_] ) > 0.001f )
	{
		return ( true );
	}

	return ( false );	
}

//-----------------------------------------------------------------------------
//	Name:		SlowZero
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowZero()
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = 0.0f;
	_r[_Y_] = 0.0f;
	_r[_Z_] = 0.0f;
}


//-----------------------------------------------------------------------------
//	Name: VECTOR::SetXYZ
//	Object:
//	01-02-14:	GGO - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetXYZ( const float x, const float y, const float z )
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = x;
	_r[_Y_] = y;
	_r[_Z_] = z;
}

//-----------------------------------------------------------------------------
//	Name: VECTOR::SetXYZ
//	Object:
//	01-02-14:	GGO - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowSetXYZ( const float x, const float y, const float z )
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = x;
	_r[_Y_] = y;
	_r[_Z_] = z;
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	02-01-10:	ELE - Created
//-----------------------------------------------------------------------------
void	VECTOR::Set(IO_STREAM &	Stream)
{
	VECTOR_GET_GENERIC_DATA();

	Stream >> _r[_X_] >> _r[_Y_] >> _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowCopy
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowCopy(const VECTOR &	V)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V[_X_];
	_r[_Y_] = V[_Y_];
	_r[_Z_] = V[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		IsZero
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
bool	VECTOR::IsZero() const
{
	VECTOR_GET_GENERIC_DATA();

	return ((_r[_X_] == 0.0f) && (_r[_Y_] == 0.0f) && (_r[_Z_] == 0.0f));
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-01-10:	SBE - Created
//-----------------------------------------------------------------------------
void	VECTOR::Copy(const POINT4D &	P)
{
	SlowCopy(P);
}

//-----------------------------------------------------------------------------
//	Name:		Copy
//	Object:		
//	03-01-10:	SBE - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowCopy(const POINT4D &	P)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = P[_X_];
	_r[_Y_] = P[_Y_];
	_r[_Z_] = P[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SetFromPoints
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetFromPoints( const VECTOR & Pt1,
							   const VECTOR & Pt2 )
{
	SlowSetFromPoints(Pt1, Pt2);
}

//-----------------------------------------------------------------------------
//	Name:		SlowSetFromPoints
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowSetFromPoints(const VECTOR &	Pt1,
								  const VECTOR &	Pt2)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = Pt2.GetX() - Pt1.GetX();
	_r[_Y_] = Pt2.GetY() - Pt1.GetY();
	_r[_Z_] = Pt2.GetZ() - Pt1.GetZ();
}


//-----------------------------------------------------------------------------
//	Name:		SetFromVectorPacked
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetFromVectorPacked( const VECTOR_PACKED & V )
{
	VECTOR_GET_GENERIC_DATA();

	int nX , nY , nZ;
	V.GetXYZ( nX , nY , nZ );
	float rInv = 1.0f / 127.0f;

	_r[_X_] = static_cast< float > ( nX ) * rInv;
	_r[_Y_] = static_cast< float > ( nY ) * rInv;
	_r[_Z_] = static_cast< float > ( nZ ) * rInv;
}


//-----------------------------------------------------------------------------
//	Name:		SetFromVectorAndNorm
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetFromVectorAndNorm( const VECTOR &	V,
									  const float		rNorm )
{
	VECTOR_GET_GENERIC_DATA();

	QDT_ASSERT( rNorm > 0.0f );

	_r[_X_] = V[_X_];
	_r[_Y_] = V[_Y_];
	_r[_Z_] = V[_Z_];

	SetLength( rNorm );
}



//-----------------------------------------------------------------------------
//	Name:		IsNull
//	Object:		
//	01-01-17:	JVA - Created
//	01-02-14:	GGO Revisited
//  02-07-17:   JLT only one return when inlining a function (much better for the compiler)
//-----------------------------------------------------------------------------
_INLINE_ bool VECTOR::IsNull( float repsilon ) const
{
	VECTOR_GET_GENERIC_DATA();

	QDT_ASSERT( repsilon >= 0.0f );

	float X = MATH::Abs( _r[_X_] );
	float Y = MATH::Abs( _r[_Y_] );
	float Z = MATH::Abs( _r[_Z_] );

	return ( X <= repsilon && Y <= repsilon && Z <= repsilon);
}

//-----------------------------------------------------------------------------
//	Name:		SlowLength
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
float	VECTOR::SlowLength() const
{
	VECTOR_GET_GENERIC_DATA();

	return ( MATH::Sqrt( MATH::Square( _r[_X_] ) + MATH::Square( _r[_Y_] ) + MATH::Square( _r[_Z_] ) ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowSquareLength
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
float	VECTOR::SlowSquareLength() const
{
	VECTOR_GET_GENERIC_DATA();

	return ( MATH::Square( _r[_X_] ) + MATH::Square( _r[_Y_] ) + MATH::Square( _r[_Z_] ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowAdd
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowAdd(const VECTOR &	V1,
						const VECTOR &	V2)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V1[_X_] + V2[_X_];
	_r[_Y_] = V1[_Y_] + V2[_Y_];
	_r[_Z_] = V1[_Z_] + V2[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowAdd
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowAdd(const VECTOR &	V)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] += V[_X_];
	_r[_Y_] += V[_Y_];
	_r[_Z_] += V[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowAdd
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowAdd(const VECTOR &	p,
						const float		s)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] += p[_X_] * s;
	_r[_Y_] += p[_Y_] * s;
	_r[_Z_] += p[_Z_] * s;
}

//-----------------------------------------------------------------------------
//	Name:		SlowAddAssign
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR &	VECTOR::SlowAddAssign(const VECTOR &	V)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] += V[_X_];
	_r[_Y_] += V[_Y_];
	_r[_Z_] += V[_Z_];

	return ( *this );
}

//-----------------------------------------------------------------------------
//	Name:		SlowAdd
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR	VECTOR::SlowAdd(const VECTOR &	V) const
{
	VECTOR_GET_GENERIC_DATA();

	return ( VECTOR( _r[_X_] + V[_X_], _r[_Y_] + V[_Y_], _r[_Z_] + V[_Z_] ) );
}

//-----------------------------------------------------------------------------
//	Name:		Add
//	Object:		
//	04-04-15:	RMA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowAdd(float r)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] += r;
	_r[_Y_] += r;
	_r[_Z_] += r;
}

//-----------------------------------------------------------------------------
//	Name:		SlowSub
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowSub(const VECTOR &	V1,
						const VECTOR &	V2)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V1[_X_] - V2[_X_];
	_r[_Y_] = V1[_Y_] - V2[_Y_];
	_r[_Z_] = V1[_Z_] - V2[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowSub
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowSub(const VECTOR &	V)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] -= V[_X_];
	_r[_Y_] -= V[_Y_];
	_r[_Z_] -= V[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowSub
//	Object:		
//	04-04-15:	RMA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowSub(float r)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] -= r;
	_r[_Y_] -= r;
	_r[_Z_] -= r;
}

//-----------------------------------------------------------------------------
//	Name:		SlowSubAssign
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR &	VECTOR::SlowSubAssign(const VECTOR &	V)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] -= V[_X_];
	_r[_Y_] -= V[_Y_];
	_r[_Z_] -= V[_Z_];

	return ( *this );
}

//-----------------------------------------------------------------------------
//	Name:		SlowSub
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR	VECTOR::SlowSub(const VECTOR &	V) const
{
	VECTOR_GET_GENERIC_DATA();

	return ( VECTOR( _r[_X_] - V[_X_], _r[_Y_] - V[_Y_], _r[_Z_] - V[_Z_] ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowNeg
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowNeg(const VECTOR &	V)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = - V[_X_];
	_r[_Y_] = - V[_Y_];
	_r[_Z_] = - V[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowNeg
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowNeg()
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = - _r[_X_];
	_r[_Y_] = - _r[_Y_];
	_r[_Z_] = - _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowMinusUnaryOp
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR	VECTOR::SlowMinusUnaryOp() const
{
	VECTOR_GET_GENERIC_DATA();

	return ( VECTOR( -_r[_X_], -_r[_Y_], -_r[_Z_] ) );
}


//-----------------------------------------------------------------------------
//	Name: VECTOR::operator -
//	Object: 
//	01-02-26:	GGO - Created
//-----------------------------------------------------------------------------
VECTOR	VECTOR::operator + () const
{
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowMul(const VECTOR &	V,
						const float		rValue)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V[_X_] * rValue;
	_r[_Y_] = V[_Y_] * rValue;
	_r[_Z_] = V[_Z_] * rValue;
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowMul(const float	rValue)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] *= rValue;
	_r[_Y_] *= rValue;
	_r[_Z_] *= rValue;
}



//-----------------------------------------------------------------------------
//	Name:		Mul
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::Mul(const	VECTOR & V,
					const	VECTOR & Scale3D)
{
	SlowMul(V, Scale3D);
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowMul(const VECTOR &	V,
						const VECTOR &	Scale3D)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V[_X_] * Scale3D[_X_];
	_r[_Y_] = V[_Y_] * Scale3D[_Y_];
	_r[_Z_] = V[_Z_] * Scale3D[_Z_];
}



//-----------------------------------------------------------------------------
//	Name:		Mul
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::Mul(const VECTOR & Scale3D)
{
	SlowMul(Scale3D);
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowMul(const VECTOR &	Scale3D)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] *= Scale3D.GetX();
	_r[_Y_] *= Scale3D.GetY();
	_r[_Z_] *= Scale3D.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		SlowMulAssign
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR &	VECTOR::SlowMulAssign(float	rValue)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] *= rValue;
	_r[_Y_] *= rValue;
	_r[_Z_] *= rValue;

	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
VECTOR	VECTOR::SlowMul(float	rValue) const
{
	VECTOR_GET_GENERIC_DATA();

	return ( VECTOR( _r[_X_] * rValue, _r[_Y_] * rValue, _r[_Z_] * rValue ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowCross
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowCross(const VECTOR &	V1,
						  const VECTOR &	V2)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V1[_Y_] * V2[_Z_] - V1[_Z_] * V2[_Y_];
	_r[_Y_] = V1[_Z_] * V2[_X_] - V1[_X_] * V2[_Z_];
	_r[_Z_] = V1[_X_] * V2[_Y_] - V1[_Y_] * V2[_X_];
}

//-----------------------------------------------------------------------------
//	Name:		SlowDot
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
float	VECTOR::SlowDot(const VECTOR &	V) const
{
	VECTOR_GET_GENERIC_DATA();

	return ( _r[_X_] * V[_X_] + _r[_Y_] * V[_Y_] + _r[_Z_] * V[_Z_] );
}

//-----------------------------------------------------------------------------
//	Name:		SetLength
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
float	VECTOR::SetLength(float	rNorm)
{
	float d = Length();

	if (d < MATH::EPSILON())
	{
		return (0.0f);
	}

	Mul( rNorm / d );

	return ( d );
}

//-----------------------------------------------------------------------------
//	Name:		SlowSetLength
//	Object:		Set the length of a vector. Returns the previous length.
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float	VECTOR::SlowSetLength( float rNorm )
{
	QDT_ASSERT(rNorm > 0.0f);

	float d = SlowLength();

	if (d < MATH::EPSILON())
	{
		return (0.0f);
	}

	SlowMul( rNorm / d );

	return ( d );
}

//-----------------------------------------------------------------------------
//	Name:		Normalize
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
float	VECTOR::Normalize()
{
	return ( SetLength( 1.0f ) );
}

//-----------------------------------------------------------------------------
//	Name:		SlowNormalize
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float	VECTOR::SlowNormalize()
{
	return ( SlowSetLength( 1.0f ) );
}

//-----------------------------------------------------------------------------
//	Name:		Normalize
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
float	VECTOR::Normalize(const VECTOR &	V)
{
	Copy( V );

	return (SetLength(1.0f));
}

//-----------------------------------------------------------------------------
//	Name:		SlowNormalize
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
float	VECTOR::SlowNormalize(const VECTOR &	V)
{
	SlowCopy( V );

	return (SlowSetLength(1.0f));
}

//-----------------------------------------------------------------------------
//	Name:		IsNormalized
//	Object:		
//	01-01-17:	JVA - Created
//	01-02-09:	GGO Revisited
//-----------------------------------------------------------------------------
bool	VECTOR::IsNormalized() const
{
	return ( MATH::Abs( SquareLength() - 1.0f ) < 0.05f );
}


//-----------------------------------------------------------------------------
//	Name:		GetXYZ
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::GetXYZ( float & rX,
						float & rY,
						float & rZ ) const
{
	VECTOR_GET_GENERIC_DATA();

	rX = _r[_X_];
	rY = _r[_Y_];
	rZ = _r[_Z_];
}


//-----------------------------------------------------------------------------
//	Name:		GetX
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float VECTOR::GetX() const
{
	VECTOR_GET_GENERIC_DATA();

	return	( _r[_X_] );
}


//-----------------------------------------------------------------------------
//	Name:		GetY
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float VECTOR::GetY() const
{
	VECTOR_GET_GENERIC_DATA();

	return	( _r[_Y_] );
}


//-----------------------------------------------------------------------------
//	Name:		GetZ
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float VECTOR::GetZ() const
{
	VECTOR_GET_GENERIC_DATA();

	return	( _r[_Z_] );
}

//-----------------------------------------------------------------------------
//	Name:		GetXRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	VECTOR::GetXRef() const
{
	VECTOR_GET_GENERIC_DATA();

	return	( _r[_X_] );
}

//-----------------------------------------------------------------------------
//	Name:		GetYRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	VECTOR::GetYRef() const
{
	VECTOR_GET_GENERIC_DATA();

	return	( _r[_Y_] );
}

//-----------------------------------------------------------------------------
//	Name:		GetZRef
//	Object:		
//	06-03-16:	CAU - Created
//-----------------------------------------------------------------------------
const Float32 &	VECTOR::GetZRef() const
{
	VECTOR_GET_GENERIC_DATA();

	return	( _r[_Z_] );
}


//-----------------------------------------------------------------------------
//	Name:		SetX
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetX( const float rX )
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = rX;
}


//-----------------------------------------------------------------------------
//	Name:		SetY
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetY( const float rY )
{
	VECTOR_GET_GENERIC_DATA();

	_r[_Y_] = rY;
}


//-----------------------------------------------------------------------------
//	Name:		SetZ
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetZ( const float rZ )
{
	VECTOR_GET_GENERIC_DATA();

	_r[_Z_] = rZ;
}


//-----------------------------------------------------------------------------
//	Name:		ZDot
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float	VECTOR::ZDot( VECTOR & V )
{
	VECTOR_GET_GENERIC_DATA();

	return ( _r[_X_] * V[_Y_] - V[_X_] * _r[_Y_] );
}


//-----------------------------------------------------------------------------
//	Name:		InterLinear
//	Object:		
//	01-01-17:	JVA - Created
//	01-03-29:	GHP revisited ==> renamed ( coherence with Quaternion class )  and ASSERT added
//-----------------------------------------------------------------------------
void	VECTOR::InterLinear(const float rLerpCoeff,
							const VECTOR &	V1,
							const VECTOR &	V2)
{
//	$$$ Eric, there is a problem when rLerpCoeff = 1.00000
//	QDT_ASSERT((rLerpCoeff >= 0.0f) && (rLerpCoeff <= 1.0f));

	SlowInterLinear(rLerpCoeff, V1, V2);
}

//-----------------------------------------------------------------------------
//	Name:		SlowInterLinear
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowInterLinear(const float	rLerpCoeff,
								const VECTOR &	V1,
								const VECTOR &	V2)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = V1[_X_] + rLerpCoeff * ( V2[_X_] - V1[_X_] );
	_r[_Y_] = V1[_Y_] + rLerpCoeff * ( V2[_Y_] - V1[_Y_] );
	_r[_Z_] = V1[_Z_] + rLerpCoeff * ( V2[_Z_] - V1[_Z_] );
}

//-----------------------------------------------------------------------------
//	Name:		InterLinearId
//	Object:		Linear interpolation with id vector with a coefficient from 0.0f to 1.0f
//	01-03-29:	GHP - Created
//-----------------------------------------------------------------------------
void VECTOR::InterLinearId(const float rLerpCoeff, VECTOR& v) 
{
//	$$$ Eric, there is a problem when rLerpCoeff = 1.00000
//	QDT_ASSERT((rLerpCoeff >= 0.0f) && (rLerpCoeff <= 1.0f));

	SlowInterLinearId(rLerpCoeff, v);
}

//-----------------------------------------------------------------------------
//	Name:		SlowInterLinearId
//	Object:		
//	03-03-18:	DAC - Created
//-----------------------------------------------------------------------------
void	VECTOR::SlowInterLinearId(const float	rLerpCoeff,
								  VECTOR &		v)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_X_] = rLerpCoeff *  v[_X_] ;
	_r[_Y_] = rLerpCoeff *  v[_Y_] ;
	_r[_Z_] = rLerpCoeff *  v[_Z_] ;
}


//-----------------------------------------------------------------------------
//	Name:		Det
//	Object:		
//	01-01-17:	JVA - Created
//-----------------------------------------------------------------------------
float	VECTOR::Det( VECTOR & V1 , VECTOR & V2 ) const
{	
	VECTOR_GET_GENERIC_DATA();

	return ( _r[_X_] * ( V1[_Y_] * V2[_Z_] - V1[_Z_] * V2[_Y_] ) -
			 _r[_Y_] * ( V1[_X_] * V2[_Z_] - V1[_Z_] * V2[_X_] ) +
			 _r[_Z_] * ( V1[_X_] * V2[_Y_] - V1[_Y_] * V2[_X_] ) );
}

//-----------------------------------------------------------------------------
//	Name:		IsOneD
//	Object:		If 2 components are almost zero the vector lies in one dimension only
//	01-09-07:	GHP - Created
//-----------------------------------------------------------------------------
bool	VECTOR::IsOneD()
{
	VECTOR_GET_GENERIC_DATA();

	float rEpsilon = MATH::EPSILON();

	return ( 
			MATH::Abs( _r[_X_] ) < rEpsilon && MATH::Abs( _r[_Y_] ) < rEpsilon
		||	MATH::Abs( _r[_X_] ) < rEpsilon && MATH::Abs( _r[_Z_] ) < rEpsilon
		||	MATH::Abs( _r[_Y_] ) < rEpsilon && MATH::Abs( _r[_Z_] ) < rEpsilon
		) ; 
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-11-30:	JLT - Created
//-----------------------------------------------------------------------------
float VECTOR::operator[](const unsigned int i) const
{
	VECTOR_GET_GENERIC_DATA();

	QDT_ASSERT(i<3);
	return _r[i];
}

//-----------------------------------------------------------------------------
//	Name:		operator[]
//	Object:		
//	01-11-30:	JLT - Created
//-----------------------------------------------------------------------------
float& VECTOR::operator[](const unsigned int i)
{
	VECTOR_GET_GENERIC_DATA();

	QDT_ASSERT(i<3);
	return _r[i];
}

//-----------------------------------------------------------------------------
//	Name: scalar product	
//	Object:		
//	02-01-14:	JLT - Created
//-----------------------------------------------------------------------------
float VECTOR::operator *(const VECTOR &V) const
{
	VECTOR_GET_GENERIC_DATA();

	return (_r[_X_] * V[_X_] + _r[_Y_] * V[_Y_] + _r[_Z_] * V[_Z_]);
}

//-----------------------------------------------------------------------------
//	Name:		GetClosestAxis
//	Object:		
//	02-05-29:	JLT - Created
//-----------------------------------------------------------------------------
int VECTOR::GetClosestAxis() const
{
	VECTOR_GET_GENERIC_DATA();

	float a[_Z_];
	int axis;

	a[_X_] = MATH::Abs(_r[_X_]);
	a[_Y_] = MATH::Abs(_r[_Y_]);
	
	axis = ((a[_X_] < a[_Y_]) ? _Y_ : _X_);

	return ((a[axis] < MATH::Abs(_r[_Z_])) ? _Z_ : axis);
}

//-----------------------------------------------------------------------------
//	Name:		operator^
//	Object:		
//	02-06-14:	JLT - Created
//-----------------------------------------------------------------------------
VECTOR VECTOR::operator^(const VECTOR &V) const
{
	VECTOR_GET_GENERIC_DATA();

	VECTOR W;

	W[_X_] = _r[_Y_] * V[_Z_] - _r[_Z_] * V[_Y_];
	W[_Y_] = _r[_Z_] * V[_X_] - _r[_X_] * V[_Z_];
	W[_Z_] = _r[_X_] * V[_Y_] - _r[_Y_] * V[_X_];
		
	return W;
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	02-06-14:	JLT - Created
//-----------------------------------------------------------------------------
const float* VECTOR::ToPointer() const
{
	VECTOR_GET_GENERIC_DATA();

	return (&_r[_X_]);
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	02-06-14:	JLT - Created
//-----------------------------------------------------------------------------
float* VECTOR::ToPointer()
{
	VECTOR_GET_GENERIC_DATA();

	return (&_r[_X_]);
}

//-----------------------------------------------------------------------------
//	Name:		GetNormeInf
//	Object:		
//	02-07-15:	JLT - Created
//-----------------------------------------------------------------------------
float VECTOR::GetNormeInf() const
{
	VECTOR_GET_GENERIC_DATA();

	float X = MATH::Abs(_r[_X_]);
	float Y = MATH::Abs(_r[_Y_]);
	float Z = MATH::Abs(_r[_Z_]);

	return ((X < Y) ? ((Y < Z) ? Z : Y) : ((X < Z) ? Z : X));
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	02-10-18:	JLT - Created
//-----------------------------------------------------------------------------
void VECTOR::Set(const VECTOR &V)
{
	Copy(V);
}

//-----------------------------------------------------------------------------
//	Name:		GetPad
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
float	VECTOR::GetPad() const
{
	VECTOR_GET_GENERIC_DATA();

	return (_r[_W_]);
}

//-----------------------------------------------------------------------------
//	Name:		SetPad
//	Object:		
//	04-01-22:	RMA - Created
//-----------------------------------------------------------------------------
void	VECTOR::SetPad(const float	rPad)
{
	VECTOR_GET_GENERIC_DATA();

	_r[_W_] = rPad;
}

//-----------------------------------------------------------------------------
//	Name:		MinimizeWith
//	Object:		
//	04-03-23:	SBE - Created
//-----------------------------------------------------------------------------
void	VECTOR::MinimizeWith(const VECTOR &	ParamV)
{
	VECTOR_GET_GENERIC_DATA();
	VECTOR_GET_GENERIC_DATA_PARAM(V, ParamV);

	_r[_X_] = MATH::Min(_r[_X_], V._r[_X_]);
	_r[_Y_] = MATH::Min(_r[_Y_], V._r[_Y_]);
	_r[_Z_] = MATH::Min(_r[_Z_], V._r[_Z_]);
}

//-----------------------------------------------------------------------------
//	Name:		MaximizeWith
//	Object:		
//	04-03-23:	SBE - Created
//-----------------------------------------------------------------------------
void	VECTOR::MaximizeWith(const VECTOR & ParamV)
{
	VECTOR_GET_GENERIC_DATA();
	VECTOR_GET_GENERIC_DATA_PARAM(V, ParamV);

	_r[_X_] = MATH::Max(_r[_X_], V._r[_X_]);
	_r[_Y_] = MATH::Max(_r[_Y_], V._r[_Y_]);
	_r[_Z_] = MATH::Max(_r[_Z_], V._r[_Z_]);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
