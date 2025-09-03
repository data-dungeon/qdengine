//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetElement
//	Object:		
//	06-03-06:	VMA - Created
//-----------------------------------------------------------------------------
Float32	MATRIX::GetElementPS3(UInt8	nLin,
							  UInt8	nCol) const
{
	return _M.getElem(nLin, nCol);
}

//-----------------------------------------------------------------------------
//	Name:		AssignPS3
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
MATRIX &	MATRIX::AssignPS3(const MATRIX &	Mat)
{
	CopyPS3(Mat);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		CopyPS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::CopyPS3(const MATRIX & Mat)
{
	_M = Mat._M;
}

//-----------------------------------------------------------------------------
//	Name:		TransposePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransposePS3()
{
	GetTransposePS3(*this);
}

//-----------------------------------------------------------------------------
//	Name:		TransformPositionPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformPositionPS3(const VECTOR &	Src,
									 VECTOR &		Dst) const
{
	TransformDirectionPS3(Src, Dst);
	
	Float32 rPad = Dst.GetPad();
	Dst += _M.getTranslation();
	Dst.SetPad(rPad);
}

//-----------------------------------------------------------------------------
//	Name:		TransformDirectionPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformDirectionPS3(const VECTOR &	Src,
									  VECTOR &			Dst) const
{
	Dst._V = _M.getUpper3x3() * Src._V;
}

//-----------------------------------------------------------------------------
//	Name:		TransformPositionPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformPositionPS3(VECTOR &	V) const
{
	Float32 rPad = V.GetPad();
	TransformPositionPS3(V, V);
	V.SetPad(rPad);
}

//-----------------------------------------------------------------------------
//	Name:		TransformDirectionPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformDirectionPS3(VECTOR &	V) const
{
	TransformDirectionPS3(V, V);
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformPositionPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformPositionPS3(const VECTOR *	pSrc,
										  VECTOR *			pDst,
										  unsigned int *	pIndices,
										  int				nNbrVertices) const
{
	int i = 0;
	int nIndice;

	while (i < nNbrVertices)
	{
		nIndice = *pIndices;

		TransformPositionPS3( pSrc[nIndice], pDst[nIndice]);

		++pIndices;
		++i;
	}
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformDirectionPS3
//	Object:		
//	06-03-13:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformDirectionPS3(const VECTOR *	pSrc,
										   VECTOR *			pDst,
										   unsigned int *	pIndices,
										   int				nNbrVertices) const
{
	int i = 0;
	int nIndice;

	while (i < nNbrVertices)
	{
		nIndice = *pIndices;

		TransformDirectionPS3( pSrc[nIndice], pDst[nIndice]);

		++pIndices;
		++i;
	}
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulPS3(const MATRIX &	M1,
					   const MATRIX &	M2)
{
	_M = M2._M * M1._M;
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulPS3(const MATRIX &	M)
{
	MulPS3(*this, M);
}

//-----------------------------------------------------------------------------
//	Name:		MulPS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulPS3(Float32	r)
{
	_M *= r;
}

//-----------------------------------------------------------------------------
//	Name:		MulAffinePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulAffinePS3(const MATRIX &	M1,
							 const MATRIX &	M2)
{
	MulPS3(M1, M2);
}

//-----------------------------------------------------------------------------
//	Name:		MulAffinePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulAffinePS3(const MATRIX &	M)
{
	MulPS3(M);
}

//-----------------------------------------------------------------------------
//	Name:		MulAffinePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::MulAffinePS3(Float32	r)
{
	MulPS3(r);
	MakeAffine();
}

//-----------------------------------------------------------------------------
//	Name:		AddPS3
//	Object:		
//	06-03-06:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::AddPS3(	const MATRIX &	M1,
						const MATRIX &	M2)
{
	_M = M1._M + M2._M;
}

//-----------------------------------------------------------------------------
//	Name:		AddPS3
//	Object:		
//	06-03-06:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::AddPS3(const MATRIX &	M)
{
	_M += M._M;
}

//-----------------------------------------------------------------------------
//	Name:		AddAffinePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::AddAffinePS3(const MATRIX &	M1,
							 const MATRIX &	M2)
{
	AddPS3(M1, M2);
	MakeAffine();
}

//-----------------------------------------------------------------------------
//	Name:		AddAffinePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::AddAffinePS3(const MATRIX &	M)
{
	AddPS3(M);
	MakeAffine();
}

//-----------------------------------------------------------------------------
//	Name:		SquarePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SquarePS3()
{
	MulPS3(*this);
}

//-----------------------------------------------------------------------------
//	Name:		SquareAffinePS3
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SquareAffinePS3()
{
	MulAffinePS3(*this);
}

//-----------------------------------------------------------------------------
//	Name:		Inverse
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::InversePS3(const MATRIX &	MatrixSource)
{
	_M = inverse(MatrixSource._M);
}

//-----------------------------------------------------------------------------
//	Name:		InverseAffine
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::InverseAffinePS3(const MATRIX &	Mat)
{
	_M = affineInverse(Mat._M);
}

//-----------------------------------------------------------------------------
//	Name:		InverseAffineNoScale
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::InverseAffineNoScalePS3(const MATRIX &	Mat)
{
	InverseAffine(Mat);
}

//-----------------------------------------------------------------------------
//	Name:		GetTranspose
//	Object:		
//	06-03-08:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::GetTransposePS3(MATRIX &	Transpose)
{
	Transpose._M = transpose(_M);
}

//-----------------------------------------------------------------------------
//	Name:		SetRotXYZPS3
//	Object:		
//	06-03-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetRotXYZPS3(Float32	rX,
							 Float32	rY,
							 Float32	rZ)
{
	QDT_ASSERT((rX >= -MATH::PI()) && (rX <= MATH::PI()));
	QDT_ASSERT((rY >= -MATH::PI()) && (rY <= MATH::PI()));
	QDT_ASSERT((rZ >= -MATH::PI()) && (rZ <= MATH::PI()));

	_M.setUpper3x3(Matrix3::rotationZYX(Vector3(rX, rY, rZ)));
}

//-----------------------------------------------------------------------------
//	Name:		Mul3x3PS3
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::Mul3x3PS3(const MATRIX &	Mat1,
						  const MATRIX &	Mat2)
{
	_M.setUpper3x3(Mat2._M.getUpper3x3() * Mat1._M.getUpper3x3());
}

//-----------------------------------------------------------------------------
//	Name:		GetQuaternionPS3
//	Object:		
//	06-03-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::GetQuaternionPS3(QUATERNION &	Q) const
{
	Q._Q = Quat(_M.getUpper3x3());
}

//-----------------------------------------------------------------------------
//	Name:		SetQuaternionPS3
//	Object:		
//	06-03-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetQuaternionPS3(const QUATERNION &	Q)
{
	_M.setUpper3x3(Matrix3(Q._Q));
}

//-----------------------------------------------------------------------------
//	Name:		ApplyPS3
//	Object:		
//	06-03-14:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::ApplyPS3(const VECTOR &	Src,
						 VECTOR &			Res) const
{
	Vector4 VSrc = (Vector4&) Src;
	Vector4 VRes = (Vector4&) Res;

	VRes = _M * VSrc;

	Res = (VECTOR&) VRes;
}

//-----------------------------------------------------------------------------
//	Name:		Transpose3x3PS3
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::Transpose3x3PS3(const MATRIX &	Mat)
{
	_M = Mat._M;
	_M.setUpper3x3( transpose(Mat._M.getUpper3x3()) ); 
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================


