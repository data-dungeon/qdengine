//=============================================================================
// CODE STARTS HERE
//=============================================================================

//-----------------------------------------------------------------------------
//	Name:		GetElementGen
//	Object:		
//	06-03-06:	VMA - Created
//-----------------------------------------------------------------------------
Float32	MATRIX::GetElementGen(UInt8	nLin,
							  UInt8	nCol) const
{
	MATRIX_GET_GENERIC_DATA();
	
	return (_r[nLin][nCol]);
}

//-----------------------------------------------------------------------------
//	Name:		GetElementGen
//	Object:		
//	06-03-07:	VMA - Created
//-----------------------------------------------------------------------------
Float32 &	MATRIX::GetElementGen(UInt8	nLin,
								  UInt8	nCol)
{
	MATRIX_GET_GENERIC_DATA();
	
	return (_r[nLin][nCol]);
}

//-----------------------------------------------------------------------------
//	Name:		OpCopyGen
//	Object:		
//	06-03-09:	VMA - Created
//-----------------------------------------------------------------------------
MATRIX &	MATRIX::AssignGen(const MATRIX &	Mat)
{
	CopyGen(Mat);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::Copy
//	Object: Copy method to avoid a code duplication in copy constructor & = 
//			operator
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void MATRIX::CopyGen(const MATRIX & Mat)
{
	SlowCopy(Mat);
}

//-----------------------------------------------------------------------------
//	Name:		Transpose
//	Object:		
//	04-08-05:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransposeGen()
{
	MATRIX Temp;
	GetTranspose(Temp);
	*this = Temp;
}

//-----------------------------------------------------------------------------
//	Name:		TransformPositionGen
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformPositionGen(VECTOR &	V) const
{
	VECTOR Temp(V);
	TransformPosition(Temp, V);
}

//-----------------------------------------------------------------------------
//	Name:		TransformDirectionGen
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::TransformDirectionGen(VECTOR &	V) const
{
	VECTOR Temp(V);
	TransformDirection(Temp, V);
}

//-----------------------------------------------------------------------------
// Name: MATRIX::SetQuaternion
// Object: Set a quaternion. This erase the 3x3 component of the matrix.
//00-11-10 JVA Created: 
//-----------------------------------------------------------------------------
void MATRIX::SetQuaternionGen(const QUATERNION & Quat)
{
	SlowSetQuaternion(Quat);
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================


