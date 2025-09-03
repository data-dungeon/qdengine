//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MATRIX::Zero
// Object: Set the matrix to zero
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::Zero()
{
	*this = ZERO;
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Identity
// Object: Set the matrix to identity
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::Identity()
{
	*this = IDENTITY;
}

//-----------------------------------------------------------------------------
//	Name:		TransformPosition
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
/*void	MATRIX::TransformPosition(POINT3D &	P) const
{
	VECTOR V1(P);
	VECTOR V2;
	TransformPosition(V1, V2);
	P = V2;
}*/

//-----------------------------------------------------------------------------
// Name: MATRIX::ResetTranslation
// Object: Set the translation component of the matrix to (0 0 0)
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void MATRIX::ResetTranslation()
{
	MATRIX_GET_GENERIC_DATA();

	_r[3][0] = 0.0f;
	_r[3][1] = 0.0f;
	_r[3][2] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		ResetRotation
//	Object:		
//	06-04-20:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::ResetRotation()
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] = 1.0f;
	_r[0][1] = 0.0f;
	_r[0][2] = 0.0f;
	_r[1][0] = 0.0f;
	_r[1][1] = 1.0f;
	_r[1][2] = 0.0f;
	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetRight
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
void MATRIX::SetRight(const VECTOR & vAxisRight)
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] = vAxisRight.GetX();
	_r[0][1] = vAxisRight.GetY();
	_r[0][2] = vAxisRight.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		SetUp
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
void MATRIX::SetUp(const VECTOR & vAxisUp)
{
	MATRIX_GET_GENERIC_DATA();

	_r[1][0] = vAxisUp.GetX();
	_r[1][1] = vAxisUp.GetY();
	_r[1][2] = vAxisUp.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		SetAt
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
void MATRIX::SetAt(const VECTOR & vAxisAt)
{
	MATRIX_GET_GENERIC_DATA();

	_r[2][0] = vAxisAt.GetX();
	_r[2][1] = vAxisAt.GetY();
	_r[2][2] = vAxisAt.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		SetPosition
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPosition(const VECTOR &	Vector)
{
	MATRIX_GET_GENERIC_DATA();

	_r[3][0] = Vector.GetX();
	_r[3][1] = Vector.GetY();
	_r[3][2] = Vector.GetZ();
}

//-----------------------------------------------------------------------------
//	Name:		GetRight
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
const VECTOR& MATRIX::GetRight(void) const
{
	MATRIX_GET_GENERIC_DATA();

	const VECTOR* pVec=reinterpret_cast<const VECTOR*>(&_r[0]);
	return static_cast<const VECTOR&>(*pVec);
}

//-----------------------------------------------------------------------------
//	Name:		GetUp
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
const VECTOR& MATRIX::GetUp(void) const
{
	MATRIX_GET_GENERIC_DATA();

	const VECTOR* pVec=reinterpret_cast<const VECTOR*>(&_r[1]);
	return static_cast<const VECTOR&>(*pVec);
}

//-----------------------------------------------------------------------------
//	Name:		GetAt
//	Object:		
//	05-12-15:	BFA - Created
//-----------------------------------------------------------------------------
const VECTOR& MATRIX::GetAt(void) const
{
	MATRIX_GET_GENERIC_DATA();

	const VECTOR* pVec=reinterpret_cast<const VECTOR*>(&_r[2]);
	return static_cast<const VECTOR&>(*pVec);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::GetTranslation
// Object: Get the translation of the matrix
//01-02-02 RMA Created: 
//-----------------------------------------------------------------------------
VECTOR MATRIX::GetTranslation() const
{
	MATRIX_GET_GENERIC_DATA();

	return (VECTOR(_r[3][0], _r[3][1], _r[3][2])) ;
}

//-----------------------------------------------------------------------------
//	Name:		GetPosition
//	Object:		
//	05-12-05:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::GetPosition(VECTOR &	Vector) const
{
	MATRIX_GET_GENERIC_DATA();

	Vector.SetXYZ(_r[3][0], _r[3][1], _r[3][2]);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::SetTranslation
// Object: Set the translation of the matrix
//01-02-02 RMA Created: 
//-----------------------------------------------------------------------------
void MATRIX::SetTranslation(const VECTOR & Vector)
{
	MATRIX_GET_GENERIC_DATA();

	_r[3][0] = Vector.GetX() ;
	_r[3][1] = Vector.GetY() ;
	_r[3][2] = Vector.GetZ() ;
}

//-----------------------------------------------------------------------------
// Name: MATRIX::SetTranslation
// Object: Set the translation of the matrix
//01-04-26 MMO Created: 
//-----------------------------------------------------------------------------
void MATRIX::SetTranslation(Float32 DX, Float32 DY, Float32 DZ)
{
	MATRIX_GET_GENERIC_DATA();

	_r[3][0] = DX;
	_r[3][1] = DY;
	_r[3][2] = DZ;
}

//-----------------------------------------------------------------------------
// Name: MATRIX::GetPosition
// Object: Get the position of the matrix
//01-02-02 RMA Created: 
//-----------------------------------------------------------------------------
/*void MATRIX::GetPosition(POINT3D &outPoint) const
{
	outPoint.SetXYZ(_r[3][0], _r[3][1], _r[3][2]) ;
}*/


//-----------------------------------------------------------------------------
// Name: MATRIX::SetPosition
// Object: Set the position of the matrix
//01-02-02 RMA Created:  
//-----------------------------------------------------------------------------
/*void MATRIX::SetPosition(const POINT3D & Point)
{
	SlowSetPosition(Point);
}*/

//-----------------------------------------------------------------------------
// Name: MATRIX::SetPosition
// Object: Set the position of the matrix (to manage alignment problem (only called by VM))
//	03-05-19:	SBE - Created
//-----------------------------------------------------------------------------
/*void MATRIX::SlowSetPosition(const POINT3D & Point)
{
	_r[3][0] = Point.GetX() ;
	_r[3][1] = Point.GetY() ;
	_r[3][2] = Point.GetZ() ;
}*/

//-----------------------------------------------------------------------------
//	Name: MATRIX::SetRotXYZ
//	Object: Same as above, except that the method argument is an Angle3D.
//	01-03-02:	GGO - Created
//-----------------------------------------------------------------------------
void MATRIX::SetRotXYZ(const ANGLE3D &Angle)
{
	SetRotXYZ( Angle.GetAlpha(), Angle.GetBeta(), Angle.GetGamma() );
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::SetRotXYZ
//	Object: Same as above, except that the method argument is an VECTOR.
//	01-03-02:	GGO - Created
//-----------------------------------------------------------------------------
void MATRIX::SetRotXYZ(const VECTOR &vAngle)
{
	SetRotXYZ( vAngle.GetX(), vAngle.GetY(), vAngle.GetZ() );
}

//-----------------------------------------------------------------------------
//	Name:		SetRotation
//	Object:		Set a rotation with a custom axe
//	01-05-15:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetRotation(const VECTOR &	Axe,
							const Float32		rAngle)
{
	// Build quaternion which represent the rotation
	QUATERNION Q(Axe, rAngle);
	SetQuaternion(Q);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::SetScale
// Object: Scale the matrix.
//		   In this case, previous rotation or scale in the 3x3 component is not
//		   erased.
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::SetScale(const VECTOR & Scale3D)
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] *= Scale3D.GetX() ;
	_r[0][1] *= Scale3D.GetY() ;
	_r[0][2] *= Scale3D.GetZ() ;

	_r[1][0] *= Scale3D.GetX() ;
	_r[1][1] *= Scale3D.GetY() ;
	_r[1][2] *= Scale3D.GetZ() ;

	_r[2][0] *= Scale3D.GetX() ;
	_r[2][1] *= Scale3D.GetY() ;
	_r[2][2] *= Scale3D.GetZ() ;
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Apply
// Object: Multiply the POINT3D Point by the matrix
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
/*void	MATRIX::Apply(POINT3D & Point) const
{
	POINT3D	Copy(Point);	
	Apply(Copy, Point);
}*/

//-----------------------------------------------------------------------------
//	Name:		Apply
//	Object:		
//	01-09-26:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::Apply(VECTOR &	Point) const
{
	VECTOR	Copy(Point);
	Apply(Copy, Point);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Apply
// Object: Multiply an array of POINT3D of size nNbrPoints by the matrix
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
/*void	MATRIX::Apply(int		nNbrPoints, 
					  POINT3D  *pResTab) const
{	
	Apply(nNbrPoints, pResTab, pResTab);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Apply
// Object: Multiply the VECTOR by the matrix
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::Apply(VECTOR & Vector) const
{
	VECTOR	Copy(Vector);
	
	Apply(Copy, Vector);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Apply
// Object: Multiply the array of VECTOR of size nNbrVectors by the matrix
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::Apply(int		nNbrVectors, 
					  VECTOR   *pVectorTab) const
{
	Apply(nNbrVectors, pVectorTab, pVectorTab);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::Apply
// Object: Multiply the array of VECTOR_PACKED of size nNbrVectors pSrc by a
//		  matrix, the result is put in the array of vector pRes
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void	MATRIX::Apply(int				   nNbrVectors, 
					  const	VECTOR_PACKED *pSrcTab, 
					  VECTOR			  *pResTab) const
{
	VECTOR	v;
	while(nNbrVectors--)
	{		
		v.SetFromVectorPacked(*pSrcTab);
		Apply(v, *pResTab);		
		++pSrcTab;
		++pResTab;
	}
}*/

//-----------------------------------------------------------------------------
//	Name:		FilterRotXYZ
//	Object:		Filter rotation
//	01-09-05:	cvi - Created
//-----------------------------------------------------------------------------
void	MATRIX::FilterRotXYZ(bool	bX,
							 bool	bY,
							 bool	bZ)
{
	VECTOR		vAngle;

	vAngle = GetRotXYZ();
	if ( !bX )
	{
		vAngle.SetX(0.0f);
	}

	if ( !bY )
	{
		vAngle.SetY(0.0f);
	}

	if ( !bZ )
	{
		vAngle.SetZ(0.0f);
	}

	SetRotXYZ(vAngle);
}

//-----------------------------------------------------------------------------
//	Name:		FilterTrans
//	Object:		Filter translation
//	01-09-05:	cvi - Created
//-----------------------------------------------------------------------------
void	MATRIX::FilterTrans(bool	bX,
							bool	bY,
							bool	bZ)
{
	MATRIX_GET_GENERIC_DATA();

	if ( !bX )
	{
		_r[3][0] = 0.0f;
	}

	if ( !bY )
	{
		_r[3][1] = 0.0f;
	}

	if ( !bZ )
	{
		_r[3][2] = 0.0f;
	}
}

//-----------------------------------------------------------------------------
//	Name:		FilterPitchHeadRoll
//	Object:		
//	01-09-08:	cvi - Created
//-----------------------------------------------------------------------------
void	MATRIX::FilterPitchHeadRoll(bool	bPitch,
									bool	bHead,
									bool	bRoll)
{
	QUATERNION	QP(false), QH(false), QR(false);
	QUATERNION	Q1(false), Q2(false);

	GetPitchHeadRoll(QP, QH, QR);

	if ( !bPitch)
	{
		QP.Id();
	}

	if ( !bHead)
	{
		QH.Id();
	}

	if ( !bRoll)
	{
		QR.Id();
	}

	Q1.Mul( QH, QP );
	Q2.Mul( QR, Q1 );
	SetQuaternion(Q2);
}

//-----------------------------------------------------------------------------
//	Name:		QPitch
//	Object:		Get the unit quaternion representing only the rotation around X
//	01-09-07:	GHP - Created
//-----------------------------------------------------------------------------
bool	MATRIX::QPitch(QUATERNION &	Qx)const
{
	VECTOR	V(0.0f, 0.0f, 1.0f ),
			Vz(0.0f, 0.0f, 1.0f ), 
			VDst; 

	//	Apply matrix 
	TransformDirection(V, VDst);
	// project on YZ plane
	VDst.SetX(0.0);
	//
	VDst.Normalize();
	//
	if ( VDst.IsOneD() )
	{
		return ( false ) ;
	}
	Qx.SetRotation(Vz, VDst);

	return (true);
}

//-----------------------------------------------------------------------------
//	Name:		QHeading
//	Object:		Get the unit quaternion representing only the rotation around Y
//	01-09-07:	GHP - Created
//-----------------------------------------------------------------------------
bool	MATRIX::QHeading(QUATERNION &	Qy)const
{
	QUATERNION dQ(false), dQ2(false); 
	GetPitchHeadRoll(dQ2,Qy,dQ) ;
	return ( true ) ; 
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	01-09-10:	JLT - Created
//-----------------------------------------------------------------------------
const Float32* MATRIX::ToPointer() const
{
	MATRIX_GET_GENERIC_DATA();

	return((Float32*)&_r[0]);
}

//-----------------------------------------------------------------------------
//	Name:		GetBasis
//	Object:		
//	02-05-30:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR MATRIX::GetBasis(const UInt32 Index) const
{
	MATRIX_GET_GENERIC_DATA();

	QDT_ASSERT(Index<3);
	return VECTOR(_r[Index][_X_], _r[Index][_Y_], _r[Index][_Z_]);
}

//-----------------------------------------------------------------------------
//	Name:		GetInverseBasis
//	Object:		
//	02-05-30:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR MATRIX::GetImageBasis(const UInt32 Index) const
{
	MATRIX_GET_GENERIC_DATA();

	QDT_ASSERT(Index<3);
	return VECTOR(_r[_X_][Index], _r[_Y_][Index], _r[_Z_][Index]);
}

//-----------------------------------------------------------------------------
//	Name:		ApplyRotation
//	Object:		
//	02-06-07:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ VECTOR MATRIX::ApplyRotation(const VECTOR &V) const
{
	VECTOR W1, W2;

	W2 = GetImageBasis(_X_);
	W1[_X_] = V * W2;

	W2 = GetImageBasis(_Y_);
	W1[_Y_] = V * W2;

	W2 = GetImageBasis(_Z_);
	W1[_Z_] = V * W2;
	
	return W1;
}

//-----------------------------------------------------------------------------
//	Name:		HasNoTranslation
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ bool MATRIX::HasNoTranslation() const
{
	MATRIX_GET_GENERIC_DATA();

	bool bResult;

	if ( ((*(int*)&(_r[3][0])) == 0) && 
		 ((*(int*)&(_r[3][1])) == 0) &&
		 ((*(int*)&(_r[3][2])) == 0))
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	return bResult;
}

//-----------------------------------------------------------------------------
//	Name:		MakeAffine
//	Object:		
//	04-08-05:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MakeAffine()
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][3] = 0.0f;
	_r[1][3] = 0.0f;
	_r[2][3] = 0.0f;
	_r[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		ToPointer
//	Object:		
//	02-06-10:	JLT - Created
//-----------------------------------------------------------------------------
Float32* MATRIX::ToPointer()
{
	MATRIX_GET_GENERIC_DATA();

	return((Float32*)&_r[0][0]);
}

//-----------------------------------------------------------------------------
//	Name:		SetFromPtr
//	Object:		
//	02-10-30:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void MATRIX::SetFromPtr(const Float32 Ptr[16])
{
	QDT_ASSERT(Ptr != NULL);
	QDT_MEMCOPY(ToPointer(), Ptr, 16 * sizeof(Float32)); 
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	02-12-02:	JLT - Created
//-----------------------------------------------------------------------------
_INLINE_ void MATRIX::Set(const MATRIX &M)
{
	if (this != &M)
	{
		Copy(M);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SlowSet
//	Object:		
//	03-05-19:	SBE - Created
//-----------------------------------------------------------------------------
_INLINE_ void MATRIX::SlowSet(const MATRIX &M)
{
	if (this != &M)
	{
		SlowCopy(M);
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================


