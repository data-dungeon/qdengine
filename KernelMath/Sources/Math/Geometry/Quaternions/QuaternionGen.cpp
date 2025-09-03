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
//
//00-11-09 JVA Created: 
//*****************************************************************************

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include	INCL_KMATH(Math/Geometry/Quaternions/QuaternionCompressed)

#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include	"QuaternionGen.inl"
#endif

//-----------------------------------------------------------------------------
// Name:		QUATERNION::SetRotationGen
// Object:		Initialize by a rotation defined by two vectors
//01-01-16 RMA Created: 
//-----------------------------------------------------------------------------
void	QUATERNION::SetRotationGen(const VECTOR & V1, const VECTOR & V2)
{
	QUAT_GET_GENERIC_DATA();

	Float32 rCosAlpha ;
	Float32 rAlpha ;

	VECTOR vAxe;	// Axe of the rotation

	vAxe.Cross(V1, V2) ;

	if ( vAxe.IsNull() == false )
	{
		vAxe.Normalize() ;
		rCosAlpha	= V1.Dot(V2) ;
	}
	else
	{
		//	We insure that V1 and V2 are normalized for dot product
		VECTOR	VV1(V1), 
				VV2(V2);

		VV1.Normalize();
		VV2.Normalize();
		rCosAlpha = VV1.Dot( VV2 );
	}

	if ( rCosAlpha >= 1.0f)
	{
		rAlpha = 0.0f;
	}
	else if ( rCosAlpha <= -1.0f )
	{
		rAlpha = MATH::PI();
	}
	else
	{
		rAlpha		= MATH::ACos(rCosAlpha) ;
	}
	
	Float32 rHalfAlpha	= rAlpha / 2 ;
	Float32 rCosHalfAlpha, rSinHalfAlpha;
	
	MATH::GetCosinusAndSinus(rHalfAlpha, rCosHalfAlpha, rSinHalfAlpha);
	
	_r[_W_] = rCosHalfAlpha;
	/*
	if	(vAxe.Length() < MATH::EPSILON())
	{
		_r[_X_] = vAxe.GetX() * rSinHalfAlpha ;
		_r[_Y_] = rSinHalfAlpha ;
		_r[_Z_] = vAxe.GetZ() * rSinHalfAlpha ;
	}
	*/
	if	( vAxe.IsNull() )
	{ 
		_r[_X_] = 0.0f ;
		_r[_Y_] = rSinHalfAlpha ;
		_r[_Z_] = 0.0f ; 
	}
	else
	{
		 Mul(vAxe, rSinHalfAlpha);
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetFromMatrixGen
//	Object:		
//	01-03-12:	ELE - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetFromMatrixGen(const MATRIX & Matrix)
{
	QUAT_GET_GENERIC_DATA();

	Float32	rDiagSum, s;
	int		i, j , k;
	int		nxt[3] = {1, 2, 0};
	Float32	q[4];

	rDiagSum = Matrix(0, 0) + Matrix(1, 1) + Matrix (2, 2);

	if	(rDiagSum > 0.0f)
	{
		s = MATH::Sqrt(rDiagSum + 1.0f);
		_r[_W_] = s / 2.0f;
		s = 0.5f / s;
		_r[_X_] = (Matrix(1, 2) - Matrix(2, 1)) * s;
		_r[_Y_] = (Matrix(2, 0) - Matrix(0, 2)) * s;
		_r[_Z_] = (Matrix(0, 1) - Matrix(1, 0)) * s;
	}
	else
	{
		i = 0; 
		if	(Matrix(1, 1) > Matrix(0, 0))
		{
			i = 1;
		}
		if	(Matrix(2, 2) > Matrix(i, i))
		{
			i = 2;
		}
		j = nxt[i];
		k = nxt[j];
		
		s = MATH::Sqrt((Matrix(i, i) - (Matrix(j, j) + Matrix(k, k))) + 1.0f);

		q[i] = s * 0.5f;

		if	(s != 0.0f)
		{
			s = 0.5f / s;
		}

		q[3] = (Matrix(j, k) - Matrix(k, j)) * s;
		q[j] = (Matrix(i, j) - Matrix(j, i)) * s;
		q[k] = (Matrix(i, k) - Matrix(k, i)) * s;

		_r[_X_] = q[0];
		_r[_Y_] = q[1];
		_r[_Z_] = q[2];
		_r[_W_] = q[3];
	}
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::SetVectRollGen
//	Object:		Initialize by a vector and an angle
//	00-11-13:	JVA - Created
//	01-01-08:	ONA - Name changed (was Build)
//  01-12-04:   JLT - FIX TO HAVE BIJECTION WITH GETVECTROLL & OPTIMS
//-----------------------------------------------------------------------------
void	QUATERNION::SetVectRollGen(const VECTOR &	Dir,
								   const Float32		rRoll)
{
	QUAT_GET_GENERIC_DATA();

	Float32	rNorm, ca, sa, roll, K;

	rNorm = Dir.Length();

	if	(rNorm <= MATH::EPSILON())
    {
		Id();
		return;
    }
	
	roll = rRoll;
	if	(roll < 0.f)
	{
		//	Deactivation made by CVI : 25/09/02
		//	This is to keep the way of the rotation wanted by the user
//		roll = roll + 2 * MATH::PI() ;
	}
	roll *= .5f;
	
	MATH::GetCosinusAndSinus(roll, ca, sa);

	_r[_W_] = ca;
	Dir.GetXYZ(_r[_X_], _r[_Y_], _r[_Z_]);
	K = sa / rNorm;

	_r[_X_] *= K;
	_r[_Y_] *= K;
	_r[_Z_] *= K;
}

//-----------------------------------------------------------------------------
//	Name:		MulGen
//	Object:		apply rotation with a vector
//	01-12-20:	JLT - Created
//				VResult = QVQ'  with Q' = conjugate of Q
//-----------------------------------------------------------------------------
VECTOR QUATERNION::MulGen(const VECTOR &V) const
{
	QUAT_GET_GENERIC_DATA();

    Float32 T, U, K, W, t, u, v, w, x, y, z;
    
    t = _r[_W_];
    u = _r[_X_];
    v = _r[_Y_];
    w = _r[_Z_];
    
    x = V[_X_];
    y = V[_Y_];
    z = V[_Z_];
    
    T =  u*x + v*y + w*z;
    U =  t*x - w*y + v*z;
    K =  w*x + t*y - u*z;
    W = -v*x + u*y + t*z;
    
    VECTOR  VResult;

    VResult[_X_] = T*u + U*t - K*w + W*v;
    VResult[_Y_] = T*v + U*w + K*t - W*u;
    VResult[_Z_] = T*w - U*v + K*u + W*t;
    
    return VResult;
	
}

//-----------------------------------------------------------------------------
//	Name:		SlerpGen
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SlerpGen(const	Float32 rT,
							     const QUATERNION &P,
							     const QUATERNION &Q)
{
    /*Float32 Theta = P.GetAngleWith(Q);
	
    if ( MATH::Abs(Theta) < 0.001f)
        return P;
	
    Float32 InvSin = 1.0f / MATH::Sin(Theta);
    Float32 Coeff0 = MATH::Sin((1.0f - rT) * Theta) * InvSin;
    Float32 Coeff1 = MATH::Sin(rT * Theta) * InvSin;
	
    return (Coeff0 * P + Coeff1 * Q);*/

	QUATERNION	InvP( P );
	InvP.Inverse();

	return	( Power( Q * InvP, rT ) * P );
}

//-----------------------------------------------------------------------------
//	Name:		SquadGen
//	Object:		Cubic interpolation, A & B are control quaternions (see below)
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::SquadGen(const	Float32 rT,
							     const QUATERNION &P,
							     const QUATERNION &A,
							     const QUATERNION &B,
							     const QUATERNION &Q)
{
//	QDT_ASSERT(P.IsUnit() && A.IsUnit() && B.IsUnit() && Q.IsUnit() && MATH::Abs(rT) <= 1.f);
	
	QUATERNION QResult, SlerpPQ, SlerpAB;
	Float32 K = 2.f * rT * (1.f - rT);
	
	SlerpPQ = Slerp(rT, P, Q);
	SlerpAB = Slerp(rT, A, B);

	SlerpPQ.Normalize();
	SlerpAB.Normalize();

	QResult = Slerp(K, SlerpPQ, SlerpAB);

	return QResult;
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
