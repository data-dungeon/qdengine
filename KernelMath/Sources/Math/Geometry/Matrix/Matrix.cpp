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
// CLASS: MATRIX
//
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include 	"Root.h"
#include 	INCL_KMATH(Math/Geometry/Matrix/Matrix)

#include 	INCL_KCORE(Stream/iostream)
#include 	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include 	INCL_KMATH(Math/Geometry/More/Angle3D)
#include 	INCL_KMATH(Math/Geometry/Shapes/Shapes0D/Point4D/Point4D)

#ifdef _DEBUG
#include	"Matrix.inl"
#endif

//=============================================================================
// CODE STARTS HERE
//=============================================================================

const MATRIX MATRIX::IDENTITY		( 1.0f,  0.0f,  0.0f,  0.0f,
									  0.0f,  1.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  1.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  1.0f);

const MATRIX MATRIX::ZERO			( 0.0f,  0.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  0.0f);

const MATRIX MATRIX::ROTATION_X_180	( 1.0f,  0.0f,  0.0f,  0.0f,
							  		  0.0f, -1.0f,  0.0f,  0.0f,
							  		  0.0f,  0.0f, -1.0f,  0.0f,
							  		  0.0f,  0.0f,  0.0f,  1.0f);

const MATRIX MATRIX::ROTATION_Y_180	(-1.0f,  0.0f,  0.0f,  0.0f,
							   		  0.0f,  1.0f,  0.0f,  0.0f,
							   		  0.0f,  0.0f, -1.0f,  0.0f,
							   		  0.0f,  0.0f,  0.0f,  1.0f);

const MATRIX MATRIX::ROTATION_Z_180	(-1.0f,  0.0f,  0.0f,  0.0f,
									  0.0f, -1.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  1.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  1.0f);

const MATRIX MATRIX::MIRROR_X		(-1.0f,  0.0f,  0.0f,  0.0f,
									  0.0f,  1.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  1.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  1.0f);

const MATRIX MATRIX::MIRROR_Y		( 1.0f,  0.0f,  0.0f,  0.0f,
									  0.0f, -1.0f,  0.0f,  0.0f,
									  0.0f,  0.0f,  1.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  1.0f);

const MATRIX MATRIX::MIRROR_Z		( 1.0f,  0.0f,  0.0f,  0.0f,
									  0.0f,  1.0f,  0.0f,  0.0f,
									  0.0f,  0.0f, -1.0f,  0.0f,
									  0.0f,  0.0f,  0.0f,  1.0f);

MATRIX::MATRIX(Float32 r00, Float32 r01, Float32 r02, Float32 r03,
			   Float32 r10, Float32 r11, Float32 r12, Float32 r13,
			   Float32 r20, Float32 r21, Float32 r22, Float32 r23,
			   Float32 r30, Float32 r31, Float32 r32, Float32 r33)
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] = r00;
	_r[0][1] = r01;
	_r[0][2] = r02;
	_r[0][3] = r03;

	_r[1][0] = r10;
	_r[1][1] = r11;
	_r[1][2] = r12;
	_r[1][3] = r13;

	_r[2][0] = r20;
	_r[2][1] = r21;
	_r[2][2] = r22;
	_r[2][3] = r23;

	_r[3][0] = r30;
	_r[3][1] = r31;
	_r[3][2] = r32;
	_r[3][3] = r33;
}

//-----------------------------------------------------------------------------
//	Name: SlowCopy
//	Object: 
//	03-05-19:	SBE - Created
//-----------------------------------------------------------------------------
void	MATRIX::SlowCopy(const MATRIX &	Mat)
{
	QDT_MEMCOPY(ToPointer(), Mat.ToPointer(), 16 * sizeof(Float32));
}

//-----------------------------------------------------------------------------
//	Name:		SetPerspectiveProjection
//	Object:		
//	04-01-21:	FDE - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPerspectiveProjection(Float32	rFOV,
										 Float32	rNearPlane,
										 Float32	rFarPlane, 
										 Float32	rViewportAspect)
{
	MATRIX_GET_GENERIC_DATA();

	Float32	rQ, rCoTan, rInvProfundis;

	Zero();

    rInvProfundis	= 1.0f / (rFarPlane - rNearPlane);
    rQ				= rFarPlane * rInvProfundis;
	rCoTan			= 1.0f / MATH::Tan(rFOV * 0.5f);

    _r[0][0] 	= rCoTan;
	_r[1][1] 	= rCoTan * rViewportAspect;
	
	_r[2][2] 	= rQ;
    _r[3][2] 	= -rQ * rNearPlane;
    _r[2][3] 	= 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		MATRIX::Read
//	Object:		Read a matrix from a stream
//	01-03-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::Read(IO_STREAM &Stream)
{
	MATRIX_GET_GENERIC_DATA();

	Stream >> _r[3][3];
	Stream >> _r[3][2];
	Stream >> _r[3][1];
	Stream >> _r[3][0];

	Stream >> _r[2][3];
	Stream >> _r[2][2];
	Stream >> _r[2][1];
	Stream >> _r[2][0];

	Stream >> _r[1][3];
	Stream >> _r[1][2];
	Stream >> _r[1][1];
	Stream >> _r[1][0];

	Stream >> _r[0][3];
	Stream >> _r[0][2];
	Stream >> _r[0][1];
	Stream >> _r[0][0];
}

//-----------------------------------------------------------------------------
//	Name:		MATRIX::Write
//	Object:		Write a matrix in a stream
//	01-03-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::Write(IO_STREAM &Stream) const
{
	MATRIX_GET_GENERIC_DATA();

	Stream << _r[3][3];
	Stream.DebugOutput(" ");
	Stream << _r[3][2];
	Stream.DebugOutput(" ");
	Stream << _r[3][1];
	Stream.DebugOutput(" ");
	Stream << _r[3][0];
	Stream.DebugOutput("\r\n");

	Stream << _r[2][3];
	Stream.DebugOutput(" ");
	Stream << _r[2][2];
	Stream.DebugOutput(" ");
	Stream << _r[2][1];
	Stream.DebugOutput(" ");
	Stream << _r[2][0];
	Stream.DebugOutput("\r\n");

	Stream << _r[1][3];
	Stream.DebugOutput(" ");
	Stream << _r[1][2];
	Stream.DebugOutput(" ");
	Stream << _r[1][1];
	Stream.DebugOutput(" ");
	Stream << _r[1][0];
	Stream.DebugOutput("\r\n");

	Stream << _r[0][3];
	Stream.DebugOutput(" ");
	Stream << _r[0][2];
	Stream.DebugOutput(" ");
	Stream << _r[0][1];
	Stream.DebugOutput(" ");
	Stream << _r[0][0];
	Stream.DebugOutput("\r\n");
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::IsIdentity
//	Object: Return true if the matrix is identity, ie if the matrix looks like that:
//			|1 0 0 0|
//			|0 1 0 0|
//			|0 0 1 0|
//			|0 0 0 1|
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
bool MATRIX::IsIdentity() const
{
	MATRIX_GET_GENERIC_DATA();

	if ( MATH::Abs( _r[0][0] - 1.0f ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[0][1] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[0][2] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[0][3] ) > 0.00001f )
	{
		return ( false );
	}


	if ( MATH::Abs( _r[1][0] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[1][1] - 1.0f ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[1][2] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[1][3] ) > 0.00001f )
	{
		return ( false );
	}


	if ( MATH::Abs( _r[2][0] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[2][1] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[2][2] - 1.0f ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[2][3] ) > 0.00001f )
	{
		return ( false );
	}

	if ( MATH::Abs( _r[3][0] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[3][1] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[3][2] ) > 0.00001f )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[3][3] - 1.0f ) > 0.00001f )
	{
		return ( false );
	}

	return ( true );
}


//-----------------------------------------------------------------------------
//	Name: MATRIX::IsAffine
//	Object: Return true if the matrix is affine, ie if the matrix looks like that:
//			|X X X X|
//			|X X X X|
//			|X X X X|
//			|0 0 0 1|
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
bool MATRIX::IsAffine() const
{
	MATRIX_GET_GENERIC_DATA();

	const Float32 EPSILON = 0.00001f;

	if ( MATH::Abs( _r[0][3] ) > EPSILON )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[1][3] ) > EPSILON )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[2][3] ) > EPSILON )
	{
		return ( false );
	}
	if ( MATH::Abs( _r[3][3] - 1.0f ) > EPSILON )
	{
		return ( false );
	}

	return (true);
}


//-----------------------------------------------------------------------------
//	Name: MATRIX::HasNoScale
//	Object: Return true if the matrix is affine and if the 3x3 submatrix is orthogonal.
//	01-02-13:	GGO - Created
//-----------------------------------------------------------------------------
bool MATRIX::HasNoScale() const
{
	MATRIX_GET_GENERIC_DATA();

	QDT_ASSERT( IsAffine() == true );

	const Float32 rEpsilon = 0.05f;

	//Check if the column vector are normalized (their norm is 1).
	VECTOR v0( _r[0][0], _r[1][0], _r[2][0] );
	VECTOR v1( _r[0][1], _r[1][1], _r[2][1] );
	VECTOR v2( _r[0][2], _r[1][2], _r[2][2] );
	if ( MATH::Abs(v0.SquareLength() - 1.0f ) > rEpsilon)
	{
		return ( false );
	}
	if ( MATH::Abs(v1.SquareLength() - 1.0f ) > rEpsilon)
	{
		return ( false );
	}
	if ( MATH::Abs(v2.SquareLength() - 1.0f ) > rEpsilon)
	{
		return ( false );
	}

	//Check if the line vector are normalized (their norm is 1).
	v0.SetXYZ( _r[0][0], _r[0][1], _r[0][2] );
	v1.SetXYZ( _r[1][0], _r[1][1], _r[1][2] );
	v2.SetXYZ( _r[2][0], _r[2][1], _r[2][2] );

	if ( MATH::Abs(v0.SquareLength() - 1.0f ) > rEpsilon)
	{
		return ( false );
	}
	if ( MATH::Abs(v1.SquareLength() - 1.0f ) > rEpsilon)
	{
		return ( false );
	}
	if ( MATH::Abs(v2.SquareLength() - 1.0f ) > rEpsilon)
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		InverseProjection
//	Object:		USE THIS ONLY FOR PERSPECTIVE PROJECTION MATRIX LIKE :
//				| X 0 0 0 |
//				| 0 X 0 0 |
//				| 0 0 X 1 |
//				| 0 0 X 0 |
//	04-10-25:	ELE - Created
//-----------------------------------------------------------------------------
void	MATRIX::InversePerspective(const MATRIX &	ParamMat)
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Mat, ParamMat);

	QDT_ASSERT(this != (&ParamMat) && " Can compute inverse matrix on itself, please use a temp matrix");

	QDT_ASSERT(Mat._r[1][0] == 0.0f); // Projection matrix
	QDT_ASSERT(Mat._r[2][0] == 0.0f);
	QDT_ASSERT(Mat._r[3][0] == 0.0f);
	QDT_ASSERT(Mat._r[0][1] == 0.0f);
	QDT_ASSERT(Mat._r[2][1] == 0.0f);
	QDT_ASSERT(Mat._r[3][1] == 0.0f);
	QDT_ASSERT(Mat._r[0][2] == 0.0f);
	QDT_ASSERT(Mat._r[1][2] == 0.0f);
	QDT_ASSERT(Mat._r[0][3] == 0.0f);
	QDT_ASSERT(Mat._r[1][3] == 0.0f);
	QDT_ASSERT(Mat._r[2][3] == 1.0f);
	QDT_ASSERT(Mat._r[3][3] == 0.0f);

/*	Float32	rDet = -Mat._r[0][0] * Mat._r[1][1] * Mat._r[3][2];

	QDT_ASSERT(rTemp != 0.0f);
	Float32	rInvDet = 1.0f / rDet;

	Zero();

	_r[0][0] = -Mat._r[1][1] * Mat._r[3][2]					   * rInvDet;
	_r[1][1] = -Mat._r[0][0] * Mat._r[3][2]					   * rInvDet;
	_r[3][2] = -Mat._r[0][0] * Mat._r[1][1]					   * rInvDet;
	_r[2][3] = -Mat._r[0][0] * Mat._r[1][1] * Mat._r[3][2] * rInvDet;
	_r[3][3] =  Mat._r[0][0] * Mat._r[1][1] * Mat._r[2][2] * rInvDet;*/

	Zero();

	_r[0][0] = 1.0f / Mat._r[0][0];
	_r[1][1] = 1.0f / Mat._r[1][1];
	_r[3][2] = 1.0f / Mat._r[3][2];
	_r[2][3] = 1.0f;
	_r[3][3] = -Mat._r[2][2] * _r[3][2];
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::Inverse
//	Object: This method computes the inverse of a general affine matrix:
//					|X X X 0|
//					|X X X 0|
//					|X X X 0|
//					|X X X 1|			
//			The computation is **REALLY** heavy and FastInverse should be prefered
//			if possible.
//	01-02-08:	GGO - Created
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//	Name: MATRIX::operator ==
//	Object: Test if two matrix are identical or not.
//	01-02-19:	GGO - Created
//-----------------------------------------------------------------------------
bool	MATRIX::operator == ( const MATRIX & ParamMat ) const
{
	MATRIX_GET_GENERIC_DATA();
	MATRIX_GET_GENERIC_DATA_PARAM(Mat, ParamMat);

	if ( ( MATH::Abs( _r[0][0] - Mat._r[0][0] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[0][1] - Mat._r[0][1] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[0][2] - Mat._r[0][2] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[0][3] - Mat._r[0][3] ) > 0.00001f ) ||
         ( MATH::Abs( _r[1][0] - Mat._r[1][0] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[1][1] - Mat._r[1][1] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[1][2] - Mat._r[1][2] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[1][3] - Mat._r[1][3] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[2][0] - Mat._r[2][0] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[2][1] - Mat._r[2][1] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[2][2] - Mat._r[2][2] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[2][3] - Mat._r[2][3] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[3][0] - Mat._r[3][0] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[3][1] - Mat._r[3][1] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[3][2] - Mat._r[3][2] ) > 0.00001f ) ||
	     ( MATH::Abs( _r[3][3] - Mat._r[3][3] ) > 0.00001f ) )
	{
		return ( false );
	}

	return ( true );
}

//-----------------------------------------------------------------------------
//	Name:		SlowSetQuaternion
//	Object:		
//	03-05-19:	SBE - Created
//-----------------------------------------------------------------------------
void	MATRIX::SlowSetQuaternion(const QUATERNION &	Quat)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 r2X = Quat.GetX() * 2 ;
	Float32 r2Y = Quat.GetY() * 2 ;
	Float32 r2Z = Quat.GetZ() * 2 ;
	Float32 rWx2X = Quat.GetW() * r2X ;
	Float32 rWx2Y = Quat.GetW() * r2Y ;
	Float32 rWx2Z = Quat.GetW() * r2Z ;
	Float32 rXx2X = Quat.GetX() * r2X ;
	Float32 rXx2Y = Quat.GetX() * r2Y ;
	Float32 rXx2Z = Quat.GetX() * r2Z ;
	Float32 rYx2Y = Quat.GetY() * r2Y ;
	Float32 rYx2Z = Quat.GetY() * r2Z ;
	Float32 rZx2Z = Quat.GetZ() * r2Z ;
	
	_r[0][0] = 1.0f - (rYx2Y + rZx2Z) ;
	_r[1][0] = rXx2Y - rWx2Z ;
	_r[2][0] = rXx2Z + rWx2Y ;
	_r[0][1] = rXx2Y + rWx2Z ;
	_r[1][1] = 1.0f - (rXx2X + rZx2Z) ;
	_r[2][1] = rYx2Z - rWx2X ;
	_r[0][2] = rXx2Z - rWx2Y ;
	_r[1][2] = rYx2Z + rWx2X ;
	_r[2][2] = 1.0f - (rXx2X + rYx2Y) ;	
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::SetRotation
//	Object: This give the rotation which transform the vector1 to the vector 2
//			with a roll
//	01-02-08:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetRotation(const VECTOR & Vector1,
							const VECTOR & Vector2,
							Float32 rRoll)
{
	VECTOR	V1, V2;
	ANGLE3D	Angle3D;
	Float32	rAlpha;
	Float32	rBeta;
	Float32	rGamma;
	Float32	rAlpha1, rAlpha2;
	Float32	rBeta1, rBeta2;
	Float32	rCosAlpha1, rCosAlpha2;
	Float32	rBuffer;
	
	V1.Normalize(Vector1);
	V2.Normalize(Vector2);
	
	rAlpha1 = MATH::ASin(Vector1.GetY());
	
	rCosAlpha1 = MATH::Cos(rAlpha1);
	
	if(MATH::Abs(rCosAlpha1) <= MATH::EPSILON())
	{
		rBeta1 = 0.0f;
	}
	else
	{
		rBuffer = Vector1.GetZ() / rCosAlpha1;
		
		if(rBuffer < -1.0f)
		{
			rBuffer = -1.0f;
		}
		else if(rBuffer > 1.0f)
		{
			rBuffer = 1.0f;
		}
		
		
		if(Vector1.GetX() >= 0.0f)
		{
			rBeta1 = MATH::ACos(rBuffer);
		}
		else
		{
			rBeta1 = - MATH::ACos(rBuffer);
		}
	}
	
	
	rAlpha2 = MATH::ASin(Vector2.GetY());
	
	rCosAlpha2 = MATH::Cos(rAlpha2);
	
	if(MATH::Abs(rCosAlpha2) <= MATH::EPSILON())
	{
		rBeta2 = 0.0f;
	}
	else
	{
		rBuffer = Vector2.GetZ() / rCosAlpha2;
		
		if(rBuffer < -1.0f)
		{
			rBuffer = -1.0f;
		}
		else if(rBuffer > 1.0f)
		{
			rBuffer = 1.0f;
		}
		
		
		if(Vector2.GetX() >= 0.0f)
		{
			rBeta2 = MATH::ACos(rBuffer);
		}
		else
		{
			rBeta2 = - MATH::ACos(rBuffer);
		}
	}
	
	rAlpha = rAlpha2 - rAlpha1;
	rBeta = rBeta2 - rBeta1;
	rGamma = rRoll;
	
	Angle3D.Set(rAlpha, rBeta, rGamma);
	
	Identity() ;
	
	SetRotXYZ(Angle3D) ;
}

//-----------------------------------------------------------------------------
//	Name:		Target
//	Object:		Use just Thales...
//	01-07-02:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::Target(const VECTOR & TargetPos, bool bReverse )
{
#if 0
/*	VECTOR		Dir = TargetPos - GetPosition();	
	Float32		rX = MATH::Sqrt(MATH::Square(Dir.GetX()) + MATH::Square(Dir.GetZ())) ;	// Thales
	ANGLE3D		Angle(-MATH::ATan(-Dir.GetY(), rX) , MATH::ATan(-Dir.GetX(), -Dir.GetZ()), 0.0f);

	SetRotXYZ(Angle);	
	*/
	VECTOR		Dir1 = TargetPos - GetPosition();	
	VECTOR		Dir2 = TargetPos - GetPosition();	
	VECTOR		V
	if ( bReverse ) 
	{
		V.SetXYZ( 0.0f, 0.0f, 1.0f );
	}
	else
	{
		V.SetXYZ( 0.0f, 0.0f, -1.0f );
	}
	VECTOR		Vr, Vr2;
//	Float32		rDot;
//	Float32		rAlpha;
	MATRIX		Temp;
	MATRIX		Temp2;
	MATRIX		Temp3;
	MATRIX		Result;
	QUATERNION	Q;

	//	Normalize direction
	Dir1.Normalize();
	Dir2.SetY( 0 );
	Dir2.Normalize();


	//	Rotation axis
	Q.SetRotation( V, Dir2 );

	//	Set rotation for XZ part
	Temp.Identity();
	Temp.SetQuaternion(Q);

	//	Temp2 matrix = rotation around X-axis
	Temp2.Identity();

	//	To make alignement with z axis
	Temp3.Inverse( Temp );
	//	Cross product for axis
	Q.SetRotation( Dir2, Dir1 );
	Temp2.SetQuaternion(Q);

	//	Result
	Result.Mul( Temp2, Temp );
	Result.SetPosition( GetPosition() );
	Copy( Result );

#else
	VECTOR	Pos;
	Pos = GetTranslation();
	VECTOR		Dir1 = TargetPos - Pos;
	VECTOR		Dir2 = TargetPos - Pos;
	VECTOR		V ; 

	if ( bReverse ) 
	{
		V.SetXYZ( 0.0f, 0.0f, 1.0f );
	}
	else
	{
		V.SetXYZ( 0.0f, 0.0f, -1.0f );
	}
	VECTOR		Vr, Vr2;
	Float32		rDot;
	Float32		rAlpha;
	MATRIX		Temp;
	MATRIX		Temp2;
	MATRIX		Temp3;
	MATRIX		Result;
	
	//	Normalize direction
	Dir1.Normalize();
	Dir2.SetY( 0 );
	Dir2.Normalize();

	//	Rotation axis
	Vr.Cross( V, Dir2 );
	if ( !Vr.IsNull( 0.00001f ) )
	{
		Vr.Normalize();
		rDot	= V.Dot(Dir2);
		rAlpha	= MATH::ACos( rDot );
	}
	else
	{
		rDot = V.Dot(Dir2);
		if ( rDot >= 0.0f )
		{
			rAlpha = 0.0f;
		}
		else
		{
			rAlpha = MATH::PI();
		}
		Vr.SetXYZ(0.0f, 1.0f, 0.0f);
	}

	//	Set rotation for XZ part
	Temp.Identity();
	Temp.SetRotation( Vr, rAlpha );

	//	Temp2 matrix = rotation around X-axis
	Temp2.Identity();
	
	//	To make alignement with z axis
	Temp3.InverseAffine( Temp );

	//	Cross product for axis
	Vr2.Cross( Dir2, Dir1 );

	if ( !Vr2.IsNull( 0.00001f ) )
	{
		Temp3.TransformDirection( Vr2, Vr );
		//	Keep only x value
		Vr.SetY( 0 );
		Vr.SetZ(0);
		//	Normalize vector
		Vr.Normalize();
		//	Get angle value
		rDot = Dir1.Dot( Dir2 );
		rAlpha	= MATH::ACos( rDot );
	}
	else
	{
		rDot = Dir1.Dot(Dir2);
		if ( rDot >= 0.0f )
		{
			rAlpha = 0.0f;
		}
		else
		{
			rAlpha = MATH::PI();
		}

		Vr.SetXYZ(1.0f, 0.0f, 0.0f);
	}
	
	Temp2.SetRotation( Vr, rAlpha );
	
	//	Result
	Result.Mul( Temp2, Temp );

	VECTOR	TmpPos;
	TmpPos = GetTranslation();
	Result.SetTranslation( TmpPos );

	Copy( Result );
#endif	
}

//-----------------------------------------------------------------------------
//	Name:		GetRotXYZ
//	Object:		Solve the system of the matrix
//				Please check http://sunsite.dk/nl3dp/matrixfaq.php3#Q37 for
//				more informations
//	01-07-03:	RMA - Created
//-----------------------------------------------------------------------------
#if 0
VECTOR	MATRIX::GetRotXYZ() const
{
	Float32 rAngleX, rAngleY, rAngleZ;
	Float32 rD, rC;
	Float32 rX, rY;

	// Calculate Y-axis angle
	rAngleY = rD = -MATH::ASin(_r[0][2]);        
    rC =  MATH::Cos(rAngleY);	
	
    if (MATH::Abs(rC) > 0.002 )
    {
		// Gimball lock?

		// No, so get X-axis angle
		rX      =   _r[2][0] / rC;           
		rY      =  -_r[2][1] / rC;

		rAngleX  = MATH::ATan(rY, rX);

		// Get Z-axis angle
		rX      =  _r[0][0] / rC;           
		rY      = -_r[1][0] / rC;

		rAngleZ  = MATH::ATan(rY, rX);
    }
    else                                 
    {
		// Gimball lock has occurred

		// Set X-axis angle to zero 
		rAngleX  = 0;                      

		// And calculate Z-axis angle
		rX      = _r[1][1];                 
		rY      = _r[0][1];

		rAngleZ  = MATH::ATan(rY, rX);
	}

	// Clamp all angles to range
    rAngleX = MATH::Clamp(rAngleX, -2.0f * MATH::PI(), 2.0f * MATH::PI());  
    rAngleY = MATH::Clamp(rAngleY, -2.0f * MATH::PI(), 2.0f * MATH::PI());
    rAngleZ = MATH::Clamp(rAngleZ, -2.0f * MATH::PI(), 2.0f * MATH::PI());

	return (VECTOR(rAngleX, rAngleY, rAngleZ));
}
#else
VECTOR	MATRIX::GetRotXYZ() const
{
	MATRIX_GET_GENERIC_DATA();

	Float32	rAngleX, rAngleY, rAngleZ;
	Float32	rD, rC;
	Float32	sina, cosa, sing, cosg;

	
	// Calculate Y-axis angle
	rAngleY = rD = -MATH::ASin(_r[0][2]);        
    rC =  MATH::Cos(rAngleY);	
	
    if (MATH::Abs(rC) > 0.001) 
    {
		sina	= _r[1][2] / rC;
		cosa	= _r[2][2] / rC;
		rAngleX = MATH::ASin(sina);
		if ( sina < 0.0f )
		{
			rAngleX = -MATH::Abs(rAngleX);
		}
		else
		{
			rAngleX = MATH::Abs(rAngleX);
		}
		if ( cosa < 0.0f )
		{
			if ( rAngleX < 0.0f )
			{
				rAngleX = -MATH::PI() - rAngleX;
			}
			else
			{
				rAngleX = MATH::PI() - rAngleX;
			}
		}
		
		sing	= _r[0][1] / rC;
		cosg	= _r[0][0] / rC;
		rAngleZ = MATH::ASin(sing);
		
		if ( sing < 0.0f )
		{
			rAngleZ = -MATH::Abs(rAngleZ);
		}
		else
		{
			rAngleZ = MATH::Abs(rAngleZ);
		}
		
		if ( cosg < 0.0f )
		{
			if ( rAngleZ < 0.0f )
			{
				rAngleZ = -MATH::PI() - rAngleZ;
			}
			else
			{
				rAngleZ = MATH::PI() - rAngleZ;
			}
		}
    }
    else                                 
    {
		rAngleX  = 0;                      
		sing	= -_r[1][0];
		cosg	= _r[1][1];
		rAngleZ = MATH::ASin(sing);
		
		if ( sing < 0.0f )
		{
			rAngleZ = -MATH::Abs(rAngleZ);
		}
		else
		{
			rAngleZ = MATH::Abs(rAngleZ);
		}
		
		if ( cosg < 0.0f )
		{
			if ( rAngleZ < 0.0f )
			{
				rAngleZ = -MATH::PI() - rAngleZ;
			}
			else
			{
				rAngleZ = MATH::PI() - rAngleZ;
			}
		}
		
	}
	
	
	// Clamp all angles to range
    rAngleX = MATH::Clamp(rAngleX, -2.0f * MATH::PI(), 2.0f * MATH::PI());  
    rAngleY = MATH::Clamp(rAngleY, -2.0f * MATH::PI(), 2.0f * MATH::PI());
    rAngleZ = MATH::Clamp(rAngleZ, -2.0f * MATH::PI(), 2.0f * MATH::PI());
	
	return (VECTOR(rAngleX, rAngleY, rAngleZ));
}
#endif

#if 0
//-----------------------------------------------------------------------------
//	Name:		GetPitchHeadRoll
//	Object:		
//	01-09-08:	cvi - Created
//-----------------------------------------------------------------------------
void	MATRIX::GetPitchHeadRoll(QUATERNION &	QPitch,
								 QUATERNION &	QHead,
								 QUATERNION &	QRoll)
{
	VECTOR			VSrc, VDst1, VDst2;
	MATRIX			InvMat, Temp;
	QUATERNION		Q;
	Float32			rDot;
	VECTOR			VTemp;


	//	First: we find the vector witch transformation give the vector (0, 0, 1)
	//	so that we be influenced by roll

	VSrc.SetXYZ(0.0f, 0.0f, 1.0f);
//	InvMat.Inverse(*this);
	InvMat = *this;
	InvMat.Apply(VSrc, VDst1);

	//	VDst1 is so that with or without roll , the result is V(0, 0, 1)
	// So now let's find the pitch

	//	Projection of VDst1 on YZ plane
	VDst2 = VDst1;
	VDst2.SetY(0.0f);	
	VDst2.Normalize();

	//	Set vectors in the same way ( front or back ) to compute pitch
	rDot = VDst2.Dot( VSrc );
	if ( rDot < 0.0f )
	{
		VTemp = -VSrc;
		//	Pitch equal rotation of VTemp to VDst2
		QPitch.SetRotation( VDst2, VTemp );
	}
	else
	{
		VTemp = VSrc;
		//	Pitch equal rotation of VDst2 to VTemp
		QPitch.SetRotation( VTemp, VDst2 );
	}

	//	Now, let's find Heading

	//	Build matrix with pitch rotation
	Temp.SetQuaternion(QPitch);
	//	Transform VDst1
	//	CVI : 11 / 04 / 02
//	Temp.Apply(VDst1, VDst2);
	VDst2 = VDst1;
	//	VDst2 should now be in XZ plane ( in theorie ), but put it in to avoid precision error
	VDst2.SetY( 0.0f );
	VDst2.Normalize();
	//	Heading equal to rotation from VDst2 ( witch is inside XZ plane )
	//	To VSrc=(0, 0, 1)
	QHead.SetRotation( VSrc, VDst2 );

	//	Now let's find the roll

	//	Compose the 2 first rotations
	Q = QHead * QPitch;
	Temp.SetQuaternion(Q);

	//	Try have a matrix with only roll in it
	InvMat.Inverse(Temp);
	Temp.Mul(InvMat, *this);
	Temp.GetQuaternion( QRoll );
	return;
	//	Now temp has just the roll

	//	Rotate Y-axis around Z-axis
	VSrc.SetXYZ(0.0f, 1.0f, 0.0f );
	Temp.Apply(VSrc, VDst1);
	//	And get quaternion
	QRoll.SetRotation(VSrc, VDst1);

}
#else
//-----------------------------------------------------------------------------
//	Name:		GetPitchHeadRoll
//	Object:		
//	01-09-08:	cvi - Created
//-----------------------------------------------------------------------------
void	MATRIX::GetPitchHeadRoll(QUATERNION &	QPitch,
								 QUATERNION &	QHead,
								 QUATERNION &	QRoll)const
{
	VECTOR			VSrc, VDst1, VDst2;
	MATRIX			InvMat, Temp;
	QUATERNION		Q(false);
	Float32			rDot;
	Float32			rVal;
	VECTOR			VTemp;

	//	First: we find the vector witch transformation give the vector (0, 0, 1)
	//	so that we be influenced by roll

	VSrc.SetXYZ(0.0f, 0.0f, 1.0f);
	InvMat = *this;
	InvMat.TransformDirection(VSrc, VDst1);

	VDst2 = VDst1;
	VDst2.SetY( 0.0f );
	VDst2.Normalize();

	QHead.SetRotation( VSrc, VDst2 );

	rDot = VDst2.Dot( VDst1 );
	rVal = MATH::ACos( MATH::Clamp(rDot, -1.0f, 1.0f) );
	if ( VDst1.GetY() > 0.0f )	rVal = -rVal;

	VDst2.SetXYZ( 1.0f, 0.0f, 0.0f );
	QPitch.SetVectRoll( VDst2, rVal );
	
	Q = QHead * QPitch;
	InvMat.SetQuaternion( Q );
	Temp.InverseAffine( InvMat );
	InvMat.Mul( Temp, *this );
	InvMat.GetQuaternion( QRoll );
}
#endif

//-----------------------------------------------------------------------------
//	Name:		ClearRotation
//	Object:		
//	02-11-04:	JLT - Created
//-----------------------------------------------------------------------------
void MATRIX::ClearRotation()
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] = 1.f;	_r[0][1] = 0.f;	_r[0][2] = 0.f;
	_r[1][0] = 0.f;	_r[1][1] = 1.f;	_r[1][2] = 0.f;
	_r[2][0] = 0.f;	_r[2][1] = 0.f;	_r[2][2] = 1.f;
}

//-----------------------------------------------------------------------------
//	Name:		QRoll
//	Object:		Get the unit quaternion representing only the rotation around Z
//	01-09-07:	GHP - Created
//-----------------------------------------------------------------------------
bool	MATRIX::QRoll(QUATERNION &	Qz)const
{
	VECTOR	V(0.0f, 1.0f, 0.0f ), 
			Vz(0.0f, 1.0f, 0.0f ), 
			VDst; 

	//	Apply matrix 
	TransformDirection(V, VDst);
	// project to XY plane
	VDst.SetZ(0.0);
	VDst.Normalize();
	//
	if ( VDst.IsOneD() )
	{
		return ( false ) ;
	}
	Qz.SetRotation(Vz, VDst);
	
	return (true) ;
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::SetRotX
//	Object: Set a rotation with the X axe.
//			This erase the 3x3 component of the matrix. 
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetRotX(Float32 rAlpha)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCosAlpha, rSinAlpha;

	MATH::GetCosinusAndSinus(rAlpha, rCosAlpha, rSinAlpha);
	
	_r[0][0] = 1.0f ;
	_r[0][1] = 0.0f ;
	_r[0][2] = 0.0f ;
	_r[1][0] = 0.0f ;
	_r[1][1] = rCosAlpha ;
	_r[1][2] = rSinAlpha ;
	_r[2][0] = 0.0f ;
	_r[2][1] = -rSinAlpha ;
	_r[2][2] = rCosAlpha ;
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::SetRotY
//	Object: Set a rotation with the Y axe.
//			This erase the 3x3 component of the matrix. 
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetRotY(Float32 rBeta)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCosBeta, rSinBeta;

	MATH::GetCosinusAndSinus(rBeta, rCosBeta, rSinBeta);
	
	_r[0][0] = rCosBeta ;
	_r[0][1] = 0.0f;
	_r[0][2] = -rSinBeta ;
	_r[1][0] = 0.0f;
	_r[1][1] = 1.0f;
	_r[1][2] = 0.0f;
	_r[2][0] = rSinBeta ;
	_r[2][1] = 0.0f;
	_r[2][2] = rCosBeta;
}

//-----------------------------------------------------------------------------
//	Name: MATRIX::BuildZ
//	Object: Set a rotation with the Z axe.
//			This erase the 3x3 component of the matrix. 
//	01-02-07:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetRotZ(Float32 rGamma)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCosGamma, rSinGamma;

	MATH::GetCosinusAndSinus(rGamma, rCosGamma, rSinGamma);
	
	_r[0][0] = rCosGamma ;
	_r[0][1] = rSinGamma ;
	_r[0][2] = 0.0f;
	_r[1][0] = -rSinGamma ;
	_r[1][1] = rCosGamma ;
	_r[1][2] = 0.0f;
	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformPosition
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformPosition(const VECTOR *	pSrc,
									   VECTOR *			pDst,
									   unsigned int		nNbrVertices) const
{
	QDT_ASSERT(pSrc != pDst);

	unsigned int i;
	for (i = 0 ; i < nNbrVertices ; ++i)
	{
		TransformPosition(pSrc[i], pDst[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformDirection
//	Object:		
//	04-08-09:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformDirection(const VECTOR *	pSrc,
										VECTOR *		pDst,
										unsigned int	nNbrVertices) const
{
	QDT_ASSERT(pSrc != pDst);

	unsigned int i;
	for (i = 0 ; i < nNbrVertices ; ++i)
	{
		TransformDirection(pSrc[i], pDst[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformPosition
//	Object:		
//	05-11-24:	SBE - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformPosition(VECTOR *		pDstSrc,
									   unsigned int	nNbrVertices) const
{
	unsigned int i;
	for (i = 0 ; i < nNbrVertices ; ++i)
	{
		VECTOR V(pDstSrc[i]);

		TransformPosition(V, pDstSrc[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformPoint
//	Object:		
//	05-11-28:	SBE - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformPoint(VECTOR *		pDstSrc,
									unsigned int	nNbrVertices) const
{
	unsigned int i;
	for (i = 0 ; i < nNbrVertices ; ++i)
	{
		VECTOR P(pDstSrc[i]);

		Apply(P);
		P /= P.GetPad();

		pDstSrc[i].SetXYZ(P.GetX(), P.GetY(), P.GetZ());
	}
}

//-----------------------------------------------------------------------------
//	Name:		BatchTransformDirection
//	Object:		
//	05-11-24:	SBE - Created
//-----------------------------------------------------------------------------
void	MATRIX::BatchTransformDirection(VECTOR *		pDstSrc,
										unsigned int	nNbrVertices) const
{
	unsigned int i;
	for (i = 0 ; i < nNbrVertices ; ++i)
	{
		VECTOR V(pDstSrc[i]);

		TransformDirection(V, pDstSrc[i]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPitchHeadRoll
//	Object:		
//	04-11-29:	CVI - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPitchHeadRoll(Float32	rPitch,
								 Float32	rHead,
								 Float32	rRoll)
{
	QUATERNION	Q(false);
	QUATERNION	QP(false);
	QUATERNION	QH(false);
	QUATERNION	QR(false);
	VECTOR		Vx(1.0f, 0.0f, 0.0f);
	VECTOR		Vy(0.0f, 1.0f, 0.0f);
	VECTOR		Vz(0.0f, 0.0f, 1.0f);


	QP.SetVectRoll( Vx, rPitch );
	QH.SetVectRoll( Vy, rHead );
	QR.SetVectRoll( Vz, rRoll );

	Q = QP * QH * QR;
	
	SetQuaternion( Q );
}

//-----------------------------------------------------------------------------
//	Name:		SetHeadPitchRoll
//	Object:		
//	06-05-04:	NHO- Created
//-----------------------------------------------------------------------------
void	MATRIX::SetHeadPitchRoll(Float32	rHead,
								 Float32	rPitch,
								 Float32	rRoll)
{
	QUATERNION	Q(false);
	QUATERNION	QP(false);
	QUATERNION	QH(false);
	QUATERNION	QR(false);

	QP.SetVectRoll( VECTOR::UNIT_X, rPitch );
	QH.SetVectRoll( VECTOR::UNIT_Y, rHead );
	QR.SetVectRoll( VECTOR::UNIT_Z, rRoll );

	Q = QH * QP * QR;
	
	SetQuaternion( Q );
}

//-----------------------------------------------------------------------------
//	Name:		SetPerspectiveFov
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void    MATRIX::SetPerspectiveFov_D3D(Float32 rHorizontalFieldOfVision, 
									  Float32 rAspectRatio, 
									  Float32 rZNearPlane, 
									  Float32 rZFarPlane)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCos = static_cast<float>(MATH::Cos(0.5f * rHorizontalFieldOfVision));
	Float32 rSin = static_cast<float>(MATH::Sin(0.5f * rHorizontalFieldOfVision));
	Float32 rH = rCos / rSin;
	Float32 rW = rH / rAspectRatio;

	_r[0][0] = rW;
	_r[0][1] = 0.0f;
	_r[0][2] = 0.0f;
	_r[0][3] = 0.0f;

	_r[1][0] = 0.0f;
	_r[1][1] = rH;
	_r[1][2] = 0.0f;
	_r[1][3] = 0.0f;

	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = rZFarPlane / (rZFarPlane - rZNearPlane);
	_r[2][3] = 1.0f;

	_r[3][0] = 0.0f;
	_r[3][1] = 0.0f;
	_r[3][2] = -(rZFarPlane * rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[3][3] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetPerspectiveFov
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPerspectiveFov_OGL(Float32 rHorizontalFieldOfVision, 
									  Float32 rAspectRatio, 
									  Float32 rZNearPlane, 
									  Float32 rZFarPlane)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCos = static_cast<float>(MATH::Cos(0.5f * rHorizontalFieldOfVision));
	Float32 rSin = static_cast<float>(MATH::Sin(0.5f * rHorizontalFieldOfVision));
	Float32 rH = rCos / rSin;
	Float32 rW = rH / rAspectRatio;

	_r[0][0] = rW;
	_r[0][1] = 0.0f;
	_r[0][2] = 0.0f;
	_r[0][3] = 0.0f;

	_r[1][0] = 0.0f;
	_r[1][1] = rH;
	_r[1][2] = 0.0f;
	_r[1][3] = 0.0f;

	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = (rZFarPlane + rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[2][3] = 1.0f;

	_r[3][0] = 0.0f;
	_r[3][1] = 0.0f;
	_r[3][2] = -(2 * rZFarPlane * rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[3][3] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetPerspectiveFov
//	Object:		
//	05-09-25:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPerspectiveFov(Float32 rHorizontalFieldOfVision, 
								  Float32 rAspectRatio, 
								  Float32 rZNearPlane, 
								  Float32 rZFarPlane)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCos = static_cast<float>(MATH::Cos(0.5f * rHorizontalFieldOfVision));
	Float32 rSin = static_cast<float>(MATH::Sin(0.5f * rHorizontalFieldOfVision));
	Float32 rH = rCos / rSin;
	Float32 rW = rH / rAspectRatio;

	_r[0][0] = rW;
	_r[0][1] = 0.0f;
	_r[0][2] = 0.0f;
	_r[0][3] = 0.0f;

	_r[1][0] = 0.0f;
	_r[1][1] = rH;
	_r[1][2] = 0.0f;
	_r[1][3] = 0.0f;

	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = rZFarPlane / (rZFarPlane - rZNearPlane);
	_r[2][3] = 1.0f;

	_r[3][0] = 0.0f;
	_r[3][1] = 0.0f;
	_r[3][2] = -(rZFarPlane * rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[3][3] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetPerspectiveFovMayaOGL
//	Object:		
//	06-02-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPerspectiveFovMayaOGL(Float32	rHorizontalFieldOfVision,
										 Float32	rViewportAspectRatio,
										 Float32	rZNearPlane,
										 Float32	rZFarPlane,
										 Float32	rScale)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCos = static_cast<float>(MATH::Cos(0.5f * rHorizontalFieldOfVision));
	Float32 rSin = static_cast<float>(MATH::Sin(0.5f * rHorizontalFieldOfVision));
	Float32 rH = rCos / rSin;

	_r[0][0] = rH / rScale;
	_r[0][1] = 0.0f;
	_r[0][2] = 0.0f;
	_r[0][3] = 0.0f;

	_r[1][0] = 0.0f;
	_r[1][1] = (rH * rViewportAspectRatio) / rScale;
	_r[1][2] = 0.0f;
	_r[1][3] = 0.0f;

	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = -(rZFarPlane + rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[2][3] = -1.0f;

	_r[3][0] = 0.0f;
	_r[3][1] = 0.0f;
	_r[3][2] = -(2 * rZFarPlane * rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[3][3] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetPerspectiveFovMayaD3D
//	Object:		
//	06-02-01:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetPerspectiveFovMayaD3D(Float32	rHorizontalFieldOfVision,
										 Float32	rViewportAspectRatio,
										 Float32	rZNearPlane,
										 Float32	rZFarPlane,
										 Float32	rScale)
{
	MATRIX_GET_GENERIC_DATA();

	Float32 rCos = static_cast<float>(MATH::Cos(0.5f * rHorizontalFieldOfVision));
	Float32 rSin = static_cast<float>(MATH::Sin(0.5f * rHorizontalFieldOfVision));
	Float32 rH = rCos / rSin;

	_r[0][0] = rH / (rScale);
	_r[0][1] = 0.0f;
	_r[0][2] = 0.0f;
	_r[0][3] = 0.0f;

	_r[1][0] = 0.0f;
	_r[1][1] = (rH * rViewportAspectRatio) / (rScale);
	_r[1][2] = 0.0f;
	_r[1][3] = 0.0f;

	_r[2][0] = 0.0f;
	_r[2][1] = 0.0f;
	_r[2][2] = -rZFarPlane / (rZFarPlane - rZNearPlane);
	_r[2][3] = -1.0f;

	_r[3][0] = 0.0f;
	_r[3][1] = 0.0f;
	_r[3][2] = -(rZFarPlane * rZNearPlane) / (rZFarPlane - rZNearPlane);
	_r[3][3] = 0.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetOrthogonalProjLeftHanded
//	Object:		
//	05-11-24:	SBE - Created
//-----------------------------------------------------------------------------
void	MATRIX::SetOrthogonalProjLeftHanded(Float32	l,
											Float32	r,
											Float32	b,
											Float32	t,
											Float32	zn,
											Float32	zf)
{
	MATRIX_GET_GENERIC_DATA();

	_r[0][0] = 2/(r-l);			_r[0][1] = 0.0f;			_r[0][2] = 0.0f;			_r[0][3] = 0.0f;
	_r[1][0] = 0.0f;			_r[1][1] = 2/(t-b);			_r[1][2] = 0.0f;			_r[1][3] = 0.0f;
	_r[2][0] = 0.0f;			_r[2][1] = 0.0f;			_r[2][2] = 1/(zf-zn);		_r[2][3] = 0.0f;
	_r[3][0] = (l+r)/(l-r);		_r[3][1] = (t+b)/(b-t);		_r[3][2] = zn/(zn-zf);		_r[3][3] = 1.0f;	
}

//-----------------------------------------------------------------------------
//	Name:		LookAtLH
//	Object:		
//	05-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::LookAt(const VECTOR & vEyePoint, 
					   const VECTOR & vLookAtPoint, 
					   const VECTOR & vUp)
{
	VECTOR vZAxis;
	vZAxis.Sub(vLookAtPoint, vEyePoint);

	LookAtLH(vEyePoint, vZAxis, vUp);
}

//-----------------------------------------------------------------------------
//	Name:		LookAtLH
//	Object:		
//	05-09-28:	RMA - Created
//-----------------------------------------------------------------------------
void	MATRIX::LookAtLH(const VECTOR & vEyePoint, 
					   const VECTOR & vLookAt, 
					   const VECTOR & vUp)
{
	MATRIX_GET_GENERIC_DATA();

	VECTOR vXAxis, vYAxis, vZAxis;

	vZAxis = vLookAt;
	vZAxis.Normalize();

	vXAxis.Cross(vUp, vZAxis);
	vXAxis.Normalize();

	vYAxis.Cross(vZAxis, vXAxis);

	//m_L0 = vXAxis.XYZ();
	_r[0][0] = vXAxis.GetX();
	_r[1][0] = vXAxis.GetY();
	_r[2][0] = vXAxis.GetZ();
	_r[3][0] = -vXAxis.Dot(vEyePoint);

	// m_L1 = vYAxis.XYZ();
	_r[0][1] = vYAxis.GetX();
	_r[1][1] = vYAxis.GetY();
	_r[2][1] = vYAxis.GetZ();
	_r[3][1] = -vYAxis.Dot(vEyePoint);

	// m_L2 = vZAxis.XYZ();
	_r[0][2] = vZAxis.GetX();
	_r[1][2] = vZAxis.GetY();
	_r[2][2] = vZAxis.GetZ();
	_r[3][2] = -vZAxis.Dot(vEyePoint);

	_r[0][3] = 0.0f;
	_r[1][3] = 0.0f;
	_r[2][3] = 0.0f;
	_r[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
//	Name:		SetEulerAnglesYXZ
//	Object:		
//	06-06-20:	BFA - Created (adapted from "GeometricTools" \WildMagic3\Foundation\Math\Wm3Matrix3.inl - FromEulerAnglesYXZ)
//-----------------------------------------------------------------------------
void MATRIX::SetEulerAnglesYXZ(const Float32 rYAngle, const Float32 rXAngle, const Float32 rZAngle)
{
    Float32 fCos, fSin;

	//build Y Rotation mtx - aka Head
    fCos = MATH::Cos(rYAngle);
    fSin = MATH::Sin(rYAngle);
    MATRIX mtxRotY;
	mtxRotY.SetRight	(VECTOR( fCos, 0.0f, fSin));
	mtxRotY.SetUp		(VECTOR::UNIT_Y);
	mtxRotY.SetAt		(VECTOR(-fSin, 0.0f, fCos));

	//build X Rotation mtx - aka Pitch
    fCos = MATH::Cos(rXAngle);
    fSin = MATH::Sin(rXAngle);
    MATRIX mtxRotX;
	mtxRotX.SetRight	(VECTOR::UNIT_X);
	mtxRotX.SetUp		(VECTOR(0.0f, fCos, -fSin));
	mtxRotX.SetAt		(VECTOR(0.0f, fSin,  fCos));

    //build Z Rotation mtx - aka Roll
	fCos = MATH::Cos(rZAngle);
    fSin = MATH::Sin(rZAngle);
    MATRIX mtxRotZ;
	mtxRotZ.SetRight	(VECTOR(fCos, -fSin, 0.0f));
	mtxRotZ.SetUp		(VECTOR(fSin,  fCos, 0.0f));
	mtxRotZ.SetAt		(VECTOR::UNIT_Z);

	//aply matrices mul (only apply 3x3)
	MATRIX mtxRotFinal;
	mtxRotFinal.Mul3x3(mtxRotZ, mtxRotX);
	mtxRotFinal.Mul3x3(MATRIX(mtxRotFinal),mtxRotY);

	//finaly only apply 3x3 part of matrix (do not alter position)
	SetRight(mtxRotFinal.GetRight());
	SetUp	(mtxRotFinal.GetUp());
	SetAt	(mtxRotFinal.GetAt());
}

//-----------------------------------------------------------------------------
//	Name:		GetEulerAnglesYXZ
//	Object:		
//	06-06-20:	BFA - Created
//-----------------------------------------------------------------------------
Bool MATRIX::GetEulerAnglesYXZ(Float32& rYAngle, Float32& rXAngle, Float32& rZAngle) const
{
	//extract head (Y)
	VECTOR	vAtProjXZ(GetAt().GetX(), 0.0f, GetAt().GetZ());
	vAtProjXZ.Normalize();
	rYAngle = -MATH::ATan(vAtProjXZ.GetX(),vAtProjXZ.GetZ());

	//extract pitch (X)
	rXAngle =MATH::ASin(GetAt().GetY());

	//extract roll (Z)
	VECTOR vNewRight;
	vNewRight.Cross(VECTOR::UNIT_Y, GetAt());
	vNewRight.Normalize();
	Float32	rDot=MATH::Clamp(vNewRight.Dot(GetRight()),-1.0f,1.0f);

	if (GetRight().GetY()>0.0f)
	{
		rZAngle =-MATH::ACos(rDot);
	}
	else
	{
		rZAngle =MATH::ACos(rDot);
	}

	return true;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
