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

#include	"Root.h"
#include	INCL_KMATH(Math/Geometry/Quaternions/Quaternion)
#include	INCL_KMATH(Math/Geometry/Quaternions/QuaternionCompressed)

#include	INCL_KCORE(Stream/iostream)

#ifdef _DEBUG
#include	"Quaternion.inl"
#endif

const QUATERNION QUATERNION::IDENTITY	(1.0f, 0.0f, 0.0f, 0.0f);
const QUATERNION QUATERNION::ZERO		(0.0f, 0.0f, 0.0f, 0.0f);

//-----------------------------------------------------------------------------
//	Name:		ReadPacked
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::Read(IO_STREAM& Stream)
{
	QUAT_GET_GENERIC_DATA();

	Stream >> _r[_W_] >> _r[_X_] >> _r[_Y_] >> _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		ReadPacked
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::Write(IO_STREAM& Stream) const
{
	QUAT_GET_GENERIC_DATA();

	Stream << _r[_W_] << _r[_X_] << _r[_Y_] << _r[_Z_];
}

//-----------------------------------------------------------------------------
//	Name:		ReadPacked
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::ReadPacked(IO_STREAM& Stream)
{
	char x, y, z, w;
	VECTOR V;
	Float32 rAngle;

	Stream >> x;
	Stream >> y;
	Stream >> z;
	Stream >> w;

	V.SetX( static_cast<Float32>(x / 127.0f)); 
	V.SetY( static_cast<Float32>(y / 127.0f)); 
	V.SetZ( static_cast<Float32>(z / 127.0f));
	rAngle = static_cast<Float32>(w / 127.0f); 

	if (V.IsNull() && rAngle == 0.0f)
	{
		SetWXYZ(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		SetVectRoll(V, rAngle);
	}
}

//-----------------------------------------------------------------------------
//	Name:		WritePacked
//	Object:		
//	03-09-23:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::WritePacked(IO_STREAM& Stream) const
{
	Float32 rAngle;
	VECTOR V;
	
	GetVectRoll(V, rAngle);

	Stream << static_cast<char>(V.GetX() * 127.0f);
	Stream << static_cast<char>(V.GetY() * 127.0f);
	Stream << static_cast<char>(V.GetZ() * 127.0f);
	Stream << static_cast<char>(rAngle	* 127.0f);
}

//-----------------------------------------------------------------------------
//	Name:		Exp
//	Object:		For Q  = [0,theta V], V unit vector
//				Exp(Q) = [cos(theta),sin(theta)V]
//				Exp(Q) is in H1   !!!!
//
//	01-12-11:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION QUATERNION::Exp(const QUATERNION& Q) 
{
	QUATERNION QResult;

	//QDT_ASSERT(Q.IsPure());

	Float32 Theta = Q.Norm(), CosTheta, SinTheta;
	Float32 K;

	MATH::GetCosinusAndSinus(Theta, CosTheta, SinTheta);

	if ( MATH::Abs(SinTheta) < 0.000001f )
	{
		QResult = QUATERNION::IDENTITY;
	}
	else
	{
		K = SinTheta / Theta;

		QResult[_W_] = CosTheta;
		QResult[_X_] = Q[_X_] * K;
		QResult[_Y_] = Q[_Y_] * K;
		QResult[_Z_] = Q[_Z_] * K;
	}

	return QResult;
}

//-----------------------------------------------------------------------------
//	Name:		Log
//	Object:		For Q from H1 : Q = [cos(theta),sin(theta)V], V unit vector
//				Log(Q) = [0,theta V]
//	Rem:		Log(Q) is not in H1   !!!!
//
//	01-12-11:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION QUATERNION::Log(const QUATERNION& Q) 
{
	QUATERNION QResult(false);

	//QDT_ASSERT(Q.IsUnit());
	
	Float32 Theta = MATH::ACos(Q[_W_]), SinTheta = MATH::Sin(Theta);
	Float32 K;

	if ( MATH::Abs(SinTheta) >= 0.000001f )
	{
		K = Theta / SinTheta;
		QResult[_X_] = Q[_X_] * K;
		QResult[_Y_] = Q[_Y_] * K;
		QResult[_Z_] = Q[_Z_] * K;
		QResult[_W_] = 0.0f;
	}
	else
	{
		QResult = QUATERNION::ZERO;
	}
	
	return QResult;
}

//-----------------------------------------------------------------------------
//	Name:		Power
//	Object:		
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
QUATERNION	QUATERNION::Power(const QUATERNION& Q, const Float32 t)
{
//	QDT_ASSERT(Q.IsUnit());

	QUATERNION QResult = Log(Q);

	QResult *= t;

	return Exp(QResult);
}

//-----------------------------------------------------------------------------
//	Name:		BuildControlQuaternions
//	Object:		Build intermediate Quaternions to allow control of the spline 
//				We must have
//					Squadn-1(1)  = Squadn(0) = Qn
//					Squadn-1'(1) = Squadn'(0)
//				and instead of Shoemacker who assumes to have 
//					Squadn'(0) = QnTn with Tn average tangent
//				we can have a C2-curve with (An) = (Bn)
//				and then 
//					An = QnExp[-(Log(InvQn*Qn-1)+Log(InvQn*Qn+1))/4]
//
//	01-12-12:	JLT - Created
//-----------------------------------------------------------------------------
void QUATERNION::BuildControlQuaternions(const QUATERNION& P0, const QUATERNION& P1, const QUATERNION& P2, QUATERNION& A)
{
//	QDT_ASSERT(P0.IsUnit() && P1.IsUnit()  && P2.IsUnit());

	QUATERNION InvP1( P1 );
	InvP1.Inverse();

	QUATERNION LogQ0 = Log(InvP1 * P0);
	QUATERNION LogQ2 = Log(InvP1 * P2);

	A = P1 * Exp((LogQ0 + LogQ2)*(-0.25f));
	A.Normalize();
}

//-----------------------------------------------------------------------------
//	Name:	GetXYEulerAngle
//	Object:	Retrieve euler angles around x & y axis
//			Works only if there 's no rotation on z !!!
//
//	01-12-18:	JLT - Created
//-----------------------------------------------------------------------------
void QUATERNION::GetXYEulerAngle(Float32& Alpha, Float32& Beta) const
{
	QUAT_GET_GENERIC_DATA();

	//QDT_ASSERT(IsUnit() || IsIdentity());

	if (IsIdentity())
	{
		Alpha = Beta = 0.f;
	}
	else
	{
		Alpha = - 2.f * MATH::ATan(_r[_Z_], _r[_Y_]);
		Beta  = - 2.f * MATH::ATan(_r[_Z_], _r[_X_]);
	}
}

//-----------------------------------------------------------------------------
//	Name:		GetPitchHeadRoll
//	Object:		
//	03-06-18:	JJA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::GetPitchHeadRoll(QUATERNION& qPitch, QUATERNION& qHead, QUATERNION& qRoll)
{
	MATRIX	M;
	M.Identity();
	M.SetQuaternion( *this );
	M.GetPitchHeadRoll( qPitch, qHead, qRoll );
}

//-----------------------------------------------------------------------------
//	Name:		GetAngle3D
//	Object:		
//	04-01-19:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::GetPitchHeadRoll(Float32& rX, Float32& rY, Float32& rZ) const
{
	VECTOR				V;
	QUATERNION			QP, QH, QR;
	QUATERNION			InvQ;
	QUATERNION			QTemp;
	VECTOR				Vy;
	VECTOR				Vz(VECTOR::UNIT_Z);
	VECTOR				Vx;

	//
	Vy = (*this) * Vz;
	Vx = Vy;
	Vy.SetY( 0.0f );
	if ( Vy.Length() <= MATH::EPSILON() )
	{
		QH.Id();
	}
	else
	{
		Vy.Normalize();
		QH.SetRotation( Vz, Vy );
	}

	//
	InvQ.Inverse( QH );
	V = InvQ * Vx;
	QP.SetRotation( Vz, V );

	//
	QP.GetVectRoll( V, rX );
	if ( V.GetX() < 0.0f )	rX = -rX;

	//
	QH.GetVectRoll( V, rY );
	if ( V.GetY() < 0.0f )
	{
		rY = -rY;
	}

	//
	QTemp = QH * QP;
	QTemp.Inverse(QTemp);
	QR = QTemp * (*this);
	QR.GetVectRoll( V, rZ );
	if ( V.GetZ() < 0.0f )
	{
		rZ = -rZ;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SetPitchHeadRoll
//	Object:		
//	04-11-29:	VPI - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SetPitchHeadRoll(const Float32 rX, const Float32 rY, const Float32 rZ)
{
	QUATERNION		QP;
	QUATERNION		QH;
	QUATERNION		QR;

	QP.SetVectRoll( VECTOR::UNIT_X, rX );
	QH.SetVectRoll( VECTOR::UNIT_Y, rY );
	QR.SetVectRoll( VECTOR::UNIT_Z, rZ );

	*this = QH * QP * QR;

}

//-----------------------------------------------------------------------------
//	Name:		operator=
//	Object:		
//	04-01-23:	RMA - Created
//-----------------------------------------------------------------------------
QUATERNION& QUATERNION::operator=(const QUATERNION_COMPRESSED& Q)
{
	Q.Uncompress(*this);
	return (*this);
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::Set
//	Object:		Initialize by a ANGLE3D
//	Below is the extended version of what is done inside this function
//	in a form which is easier to read 
//	EulerAngle.Set( V[ 0 ],  V[ 1 ] , V [ 2 ] ) ;
//	Qa.SetWXYZ( MATH::Cos( V[0] / 2.0f )	, MATH::Sin( V[0] / 2.0f )	, 0.0f						, 0.0f ) ;
//	Qb.SetWXYZ( MATH::Cos( V[1] / 2.0f )	, 0.0f						, MATH::Sin( V[1] / 2.0f )	, 0.0f ) ;
//	Qc.SetWXYZ( MATH::Cos( V[2] / 2.0f )	, 0.0f						, 0.0f						, MATH::Sin( V[2] / 2.0f ) ) ;
//	
//	Q0 = Qc* Qb * Qa ; 
//	00-11-13:	JVA Created
//	01-01-08:	ONA - Name changed (was Build)
//	01-07-26	GHP	- Add the comment after testing renamed variables 
//  01-12-12:	JLT - remove 4 multiplications
//-----------------------------------------------------------------------------
void	QUATERNION::Set(const ANGLE3D& Angle3D)
{
	QUAT_GET_GENERIC_DATA();

    Float32 rHalfCosAlpha, rHalfSinAlpha, rHalfCosBeta, rHalfSinBeta, rHalfCosGamma, rHalfSinGamma;
	Float32 rAlpha, rBeta, rGamma;

	Float32 SinAlphaCosBeta, SinBetaSinGamma, SinBetaCosGamma, CosAlphaCosBeta;

	Angle3D.GetAngles(rAlpha, rBeta, rGamma);

	MATH::GetCosinusAndSinus(rGamma * 0.5f, rHalfCosGamma, rHalfSinGamma);
	MATH::GetCosinusAndSinus(rBeta * 0.5f, rHalfCosBeta, rHalfSinBeta);
	MATH::GetCosinusAndSinus(rAlpha * 0.5f, rHalfCosAlpha, rHalfSinAlpha);

	SinAlphaCosBeta = rHalfSinAlpha * rHalfCosBeta;
	SinBetaSinGamma = rHalfSinBeta  * rHalfSinGamma;
	SinBetaCosGamma = rHalfSinBeta  * rHalfCosGamma;
	CosAlphaCosBeta = rHalfCosAlpha * rHalfCosBeta;

	_r[_X_] = SinAlphaCosBeta * rHalfCosGamma	 - rHalfCosAlpha   * SinBetaSinGamma;
	_r[_Y_] = rHalfCosAlpha   * SinBetaCosGamma + SinAlphaCosBeta * rHalfSinGamma;
	_r[_Z_] = CosAlphaCosBeta * rHalfSinGamma   - rHalfSinAlpha   * SinBetaCosGamma;
	_r[_W_] = CosAlphaCosBeta * rHalfCosGamma   + rHalfSinAlpha   * SinBetaSinGamma;
}

//-----------------------------------------------------------------------------
//	Name:		Set
//	Object:		
//	03-04-02:	RMA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::Set(const VECTOR& vAngle3D)
{
	QUAT_GET_GENERIC_DATA();

	Float32 rHalfCosAlpha, rHalfSinAlpha, rHalfCosBeta, rHalfSinBeta, rHalfCosGamma, rHalfSinGamma;
	Float32 rAlpha, rBeta, rGamma;

	Float32 SinAlphaCosBeta, SinBetaSinGamma, SinBetaCosGamma, CosAlphaCosBeta;

	vAngle3D.GetXYZ(rAlpha, rBeta, rGamma);

	MATH::GetCosinusAndSinus(rGamma * 0.5f, rHalfCosGamma, rHalfSinGamma);
	MATH::GetCosinusAndSinus(rBeta * 0.5f, rHalfCosBeta, rHalfSinBeta);
	MATH::GetCosinusAndSinus(rAlpha * 0.5f, rHalfCosAlpha, rHalfSinAlpha);

	SinAlphaCosBeta = rHalfSinAlpha * rHalfCosBeta;
	SinBetaSinGamma = rHalfSinBeta  * rHalfSinGamma;
	SinBetaCosGamma = rHalfSinBeta  * rHalfCosGamma;
	CosAlphaCosBeta = rHalfCosAlpha * rHalfCosBeta;

	_r[_X_] = SinAlphaCosBeta * rHalfCosGamma	 - rHalfCosAlpha   * SinBetaSinGamma;
	_r[_Y_] = rHalfCosAlpha   * SinBetaCosGamma + SinAlphaCosBeta * rHalfSinGamma;
	_r[_Z_] = CosAlphaCosBeta * rHalfSinGamma   - rHalfSinAlpha   * SinBetaCosGamma;
	_r[_W_] = CosAlphaCosBeta * rHalfCosGamma   + rHalfSinAlpha   * SinBetaSinGamma;
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::GetVectRoll
//	Object:		Get the vector and the roll calculated from the quaternion
//	00-11-13:	JVA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::GetVectRoll(VECTOR& V, Float32& rRoll) const
{
	QUAT_GET_GENERIC_DATA();

	Float32			f0, f1;
	VECTOR			VD( 1.0f, 1.0f, 1.0f );
	Float32			rVal;

	f0 = MATH::ACos(_r[_W_]);

	if	( f0 != 0.0f)
	{
		f1 = 1.f / MATH::Sin( f0 );

		V.SetXYZ(_r[_X_] * f1, _r[_Y_] * f1, _r[_Z_] * f1);
	}
	else
	{
		V.SetXYZ(_r[_X_], _r[_Y_], _r[_Z_]);
	}

	//	Check rotation way
	rVal = VD.Dot( V );
	if ( rVal < 0.0f )
	{
		f0 = - f0;
		V.Mul( -1.0f );
	}
	rRoll = 2 * f0;


	//	Desactivation because we don't have to touch to the angle now
	/*if	(rRoll > MATH::PI())
	{
		rRoll = rRoll - 2 * MATH::PI() ;
	}*/
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowMul(const QUATERNION& Q1, const QUATERNION& Q2)
{
	QUAT_GET_GENERIC_DATA();

	_r[_W_] = (Q1[_W_] * Q2[_W_]) - (Q1[_X_] * Q2[_X_]) - (Q1[_Y_] * Q2[_Y_]) - (Q1[_Z_] * Q2[_Z_]);
	_r[_X_] = (Q1[_X_] * Q2[_W_]) + (Q1[_W_] * Q2[_X_]) + (Q1[_Y_] * Q2[_Z_]) - (Q1[_Z_] * Q2[_Y_]);
	_r[_Y_] = (Q1[_Y_] * Q2[_W_]) + (Q1[_W_] * Q2[_Y_]) - (Q1[_X_] * Q2[_Z_]) + (Q1[_Z_] * Q2[_X_]);
	_r[_Z_] = (Q1[_Z_] * Q2[_W_]) + (Q1[_W_] * Q2[_Z_]) + (Q1[_X_] * Q2[_Y_]) - (Q1[_Y_] * Q2[_X_]);
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowMul(const QUATERNION& Q)
{
	QUAT_GET_GENERIC_DATA();

	Float32	w = _r[_W_],
			x = _r[_X_],
			y = _r[_Y_],
			z = _r[_Z_];

	_r[_W_] = (w * Q[_W_]) - (x * Q[_X_]) - (y * Q[_Y_]) - (z * Q[_Z_]);
	_r[_X_] = (x * Q[_W_]) + (w * Q[_X_]) + (y * Q[_Z_]) - (z * Q[_Y_]);
	_r[_Y_] = (y * Q[_W_]) + (w * Q[_Y_]) - (x * Q[_Z_]) + (z * Q[_X_]);
	_r[_Z_] = (z * Q[_W_]) + (w * Q[_Z_]) + (x * Q[_Y_]) - (y * Q[_X_]);
}

//-----------------------------------------------------------------------------
//	Name:		SlowInterLinear
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowInterLinear(const Float32 rLerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2, bool bNormalize)
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT((rLerpCoeff >= 0.0f) && (rLerpCoeff <= 1.0f));
	//MATH::Clip( rLerpCoeff, 0.0f, 1.0f) ; 	//can't clip because rLerpCoeff is 'const'

	if	(Q1 == Q2)
	{
		Set(Q1);
		return;
	}

	Float32	rScale = 1.0f - rLerpCoeff;

	_r[_X_] = (rScale * Q1[_X_]) + (rLerpCoeff * Q2[_X_]);
	_r[_Y_] = (rScale * Q1[_Y_]) + (rLerpCoeff * Q2[_Y_]);
	_r[_Z_] = (rScale * Q1[_Z_]) + (rLerpCoeff * Q2[_Z_]);
	_r[_W_] = (rScale * Q1[_W_]) + (rLerpCoeff * Q2[_W_]);

	if ( bNormalize )
	{
		SlowNormalize();
	}
}

//-----------------------------------------------------------------------------
//	Name:		SlowInterSLERP
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowInterSLERP(const Float32 rSlerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2)
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT((rSlerpCoeff >= 0.0f) && (rSlerpCoeff <= 1.0f));
	//MATH::Clip( rSlerpCoeff, 0.0f, 1.0f) ;  	//can't clip because rLerpCoeff is 'const'

	if	(Q1 == Q2)
	{
		Set(Q1);
		return;
	}

	Float32	rTempo[4];
	Float32	rOmega, rCosOmega, rSinOmega, rScale0, rScale1;
	
	rScale0 = 1.0f - rSlerpCoeff;
	rScale1 = rSlerpCoeff;

	rCosOmega = (Q1[_X_] * Q2[_X_] ) + (Q1[_Y_] * Q2[_Y_]) + (Q1[_Z_] * Q2[_Z_] ) + (Q1[_W_] * Q2[_W_] );
	
	if	(rCosOmega < 0.0f)
	{
		rCosOmega	= - rCosOmega;
		rTempo[0]	= - Q2[_X_];
		rTempo[1]	= - Q2[_Y_];
		rTempo[2]	= - Q2[_Z_];
		rTempo[3]	= - Q2[_W_];
	}
	else
	{
		rTempo[0] = Q2[_X_];
		rTempo[1] = Q2[_Y_];
		rTempo[2] = Q2[_Z_];
		rTempo[3] = Q2[_W_];
	}
	
	if	( MATH::Abs( ( 1.0f - rCosOmega ) ) > MATH::EPSILON() ) 
	{
		MATH::Clip( rCosOmega, -1.0f, 1.0f); 
		rOmega = MATH::ACos( rCosOmega );
		
		if ( MATH::Abs( MATH::Sin ( rOmega ) ) > MATH::EPSILON())
		{
			QDT_ASSERT( rOmega != 0.0f); //if raised ...Divide by 0 exception below
			rSinOmega	= 1.0f / MATH::Sin ( rOmega );
			rScale0		= MATH::Sin ( ( 1.0f - rSlerpCoeff ) * rOmega ) * rSinOmega;
			rScale1		= MATH::Sin ( rSlerpCoeff * rOmega ) * rSinOmega;
		}
	}

	_r[_X_] = ( rScale0 * Q1[_X_] ) + ( rScale1 * rTempo[0] ) ;
	_r[_Y_] = ( rScale0 * Q1[_Y_] ) + ( rScale1 * rTempo[1] ) ;
	_r[_Z_] = ( rScale0 * Q1[_Z_] ) + ( rScale1 * rTempo[2] ) ;
	_r[_W_] = ( rScale0 * Q1[_W_] ) + ( rScale1 * rTempo[3] ) ;
}

//-----------------------------------------------------------------------------
//	Name:		QUATERNION::InterSLERPId
//	Object:		Make a spherical linear interpolation between Q and Id with a coefficient from 0.0f to 1.0f
//				If (Q._w < 0) we interpolate to the negative Id
//	01-01-04:	ONA - Created
//-----------------------------------------------------------------------------
void	QUATERNION::InterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q)
{
	SlowInterSLERPId(rSlerpCoeff, Q);
}

//-----------------------------------------------------------------------------
//	Name:		SlowInterSLERPId
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowInterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q)
{
	QUAT_GET_GENERIC_DATA();

	Float32	rOmega, rCosOmega, rSinOmega, rScale0, rScale1 ;
	bool	bFlagPos;

	QDT_ASSERT((rSlerpCoeff >= 0.0f) && (rSlerpCoeff <= 1.0f));
	//MATH::Clip( rSlerpCoeff, 0.0f, 1.0f) ;  	//can't clip because rLerpCoeff is 'const'

	if	(Q[_W_] >= 0)
	{
		rCosOmega = Q[_W_];
		bFlagPos = true;
	}
	else
	{
		rCosOmega = -Q[_W_];
		bFlagPos = false;
	}

	if ( ( 1.0f - rCosOmega) > MATH::EPSILON() ) 
	{
		rOmega		= MATH::ACos( rCosOmega ) ;
		rSinOmega	= 1.0f / MATH::Sin ( rOmega );
		rScale0		= MATH::Sin ( ( 1.0f - rSlerpCoeff ) * rOmega ) * rSinOmega ;
		rScale1		= MATH::Sin ( rSlerpCoeff * rOmega ) * rSinOmega ;
	} 
	else
	{
		rScale0 = 1.0f - rSlerpCoeff;
		rScale1 = rSlerpCoeff;
	}

	_r[_X_] = rScale1 * Q[_X_];
	_r[_Y_] = rScale1 * Q[_Y_];
	_r[_Z_] = rScale1 * Q[_Z_];
	_r[_W_] = rScale1 * Q[_W_];

	if	(bFlagPos)
	{
		_r[_W_] += rScale0;
	}
	else
	{
		_r[_W_] -= rScale0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SlowBasicInterLinear
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowBasicInterLinear(const Float32 rLerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2, bool bNormalize)
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT((rLerpCoeff >= 0.0f) && (rLerpCoeff <= 1.0f));
	//MATH::Clip( rLerpCoeff, 0.0f, 1.0f) ;  	//can't clip because rLerpCoeff is 'const'

	if	(Q1 == Q2)
	{
		Set(Q1);
		return;
	}

	Float32	rScale = 1.0f - rLerpCoeff ;

	_r[_X_] = (rScale * Q1[_X_]) + (rLerpCoeff * Q2[_X_]);
	_r[_Y_] = (rScale * Q1[_Y_]) + (rLerpCoeff * Q2[_Y_]);
	_r[_Z_] = (rScale * Q1[_Z_]) + (rLerpCoeff * Q2[_Z_]);
	_r[_W_] = (rScale * Q1[_W_]) + (rLerpCoeff * Q2[_W_]);

	if ( bNormalize )
	{
		SlowNormalize() ;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SlowBasicInterSLERP
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowBasicInterSLERP(const Float32 rSlerpCoeff, const QUATERNION& Q1, const QUATERNION& Q2)
{
	QUAT_GET_GENERIC_DATA();

//	QDT_ASSERT((rSlerpCoeff >= 0.0f) && (rSlerpCoeff <= 1.0f));
	//MATH::Clip( rSlerpCoeff, 0.0f, 1.0f) ;  	//can't clip because rLerpCoeff is 'const'

	Float32	rTempo[4];
	Float32	rOmega, rCosOmega, rSinOmega, rScale0, rScale1;
	
	rScale0 = 1.0f - rSlerpCoeff;
	rScale1 = rSlerpCoeff;
	
	rCosOmega = (Q1[_X_] * Q2[_X_] ) + (Q1[_Y_] * Q2[_Y_]) + (Q1[_Z_] * Q2[_Z_] ) + (Q1[_W_] * Q2[_W_] );
	
	rTempo[0] = Q2[_X_];
	rTempo[1] = Q2[_Y_];
	rTempo[2] = Q2[_Z_];
	rTempo[3] = Q2[_W_];
	
	if	( MATH::Abs( ( 1.0f - rCosOmega ) ) > MATH::EPSILON() ) 
	{
		MATH::Clip(rCosOmega,-1.0f,1.0f);
		rOmega = MATH::ACos( rCosOmega );
		
		if ( MATH::Abs( MATH::Sin ( rOmega ) ) > MATH::EPSILON())
		{
			QDT_ASSERT( rOmega != 0.0f); //if raised ...Divide by 0 exception below
			rSinOmega	= 1.0f / MATH::Sin ( rOmega );
			rScale0		= MATH::Sin ( ( 1.0f - rSlerpCoeff ) * rOmega ) * rSinOmega;
			rScale1		= MATH::Sin ( rSlerpCoeff * rOmega ) * rSinOmega;
		}
	}
	
	_r[_X_] = ( rScale0 * Q1[_X_] ) + ( rScale1 * rTempo[0] ) ;
	_r[_Y_] = ( rScale0 * Q1[_Y_] ) + ( rScale1 * rTempo[1] ) ;
	_r[_Z_] = ( rScale0 * Q1[_Z_] ) + ( rScale1 * rTempo[2] ) ;
	_r[_W_] = ( rScale0 * Q1[_W_] ) + ( rScale1 * rTempo[3] ) ;
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterLinearId
//	Object:		
//	02-09-04:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION::BasicInterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize)
{
	SlowBasicInterLinearId(rLerpCoeff, Q, bNormalize);
}

//-----------------------------------------------------------------------------
//	Name:		SlowBasicInterLinearId
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowBasicInterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize)
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT((rLerpCoeff >= 0.0f) && (rLerpCoeff <= 1.0f));
	//MATH::Clip( rLerpCoeff, 0.0f, 1.0f) ; 	 	//can't clip because rLerpCoeff is 'const'

	_r[_X_] = rLerpCoeff * Q[_X_];
	_r[_Y_] = rLerpCoeff * Q[_Y_];
	_r[_Z_] = rLerpCoeff * Q[_Z_];
	_r[_W_] = rLerpCoeff * Q[_W_];

	if	(Q[_W_] >= 0)
	{
		_r[_W_] += 1.0f - rLerpCoeff;
	}
	else
	{
		_r[_W_] -= 1.0f - rLerpCoeff;
	}
	if ( bNormalize )
	{
		SlowNormalize() ;
	}
}

//-----------------------------------------------------------------------------
//	Name:		BasicInterSLERPId
//	Object:		
//	02-09-04:	CVI - Created
//-----------------------------------------------------------------------------
void	QUATERNION::BasicInterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q)
{
	SlowBasicInterSLERPId(rSlerpCoeff, Q);
}

//-----------------------------------------------------------------------------
//	Name:		SlowBasicInterSLERPId
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowBasicInterSLERPId(const Float32 rSlerpCoeff, const QUATERNION& Q)
{
	QUAT_GET_GENERIC_DATA();

	Float32	rOmega, rCosOmega, rSinOmega, rScale0, rScale1 ;
	bool	bFlagPos;

	QDT_ASSERT((rSlerpCoeff >= 0.0f) && (rSlerpCoeff <= 1.0f));
	//MATH::Clip( rSlerpCoeff, 0.0f, 1.0f) ; 	 	//can't clip because rLerpCoeff is 'const'


	if	(Q[_W_] >= 0)
	{
		bFlagPos = true;
	}
	else
	{
		bFlagPos = false;
	}
	rCosOmega = Q[_W_];

	if ( ( 1.0f - rCosOmega) > MATH::EPSILON() ) 
	{
		rOmega		= MATH::ACos( rCosOmega ) ;
		rSinOmega	= 1.0f / MATH::Sin ( rOmega );
		rScale0		= MATH::Sin ( ( 1.0f - rSlerpCoeff ) * rOmega ) * rSinOmega ;
		rScale1		= MATH::Sin ( rSlerpCoeff * rOmega ) * rSinOmega ;
	} 
	else
	{
		rScale0 = 1.0f - rSlerpCoeff;
		rScale1 = rSlerpCoeff;
	}

	_r[_X_] = rScale1 * Q[_X_];
	_r[_Y_] = rScale1 * Q[_Y_];
	_r[_Z_] = rScale1 * Q[_Z_];
	_r[_W_] = rScale1 * Q[_W_];

	if	(bFlagPos)
	{
		_r[_W_] += rScale0;
	}
	else
	{
		_r[_W_] -= rScale0;
	}
}

//-----------------------------------------------------------------------------
//	Name:		SlowNormalize
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowNormalize()
{
	QUAT_GET_GENERIC_DATA();

	Float32 rInvNorm = 1.f / MATH::Sqrt( Norm2() ) ;

	_r[_W_] *= rInvNorm ; 
	_r[_X_] *= rInvNorm ; 
	_r[_Y_] *= rInvNorm ; 
	_r[_Z_] *= rInvNorm ; 
}

//-----------------------------------------------------------------------------
//	Name:		SlowMul
//	Object:		unaligned and slow version used but VM operators
//	03-07-21:	NHO - Created
//-----------------------------------------------------------------------------
VECTOR	QUATERNION::SlowMul(const VECTOR& V) const
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
//	Name:		QUATERNION::InterLinearId
//	Object:		Make a linear interpolation between Q and Id with a coefficient from 0.0f to 1.0f
//				If (Q._w < 0) we interpolate to the negative Id
//	01-01-08:	ONA - Created
//	01-03-29:	GHP Modified  Added the normalize option linear interpolation does not conserve magnitude
//-----------------------------------------------------------------------------
void	QUATERNION::InterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize)
{
	SlowInterLinearId(rLerpCoeff, Q, bNormalize);
}

//-----------------------------------------------------------------------------
//	Name:		SlowInterLinearId
//	Object:		
//	03-03-19:	DAC - Created
//-----------------------------------------------------------------------------
void	QUATERNION::SlowInterLinearId(const Float32 rLerpCoeff, const QUATERNION& Q, bool bNormalize)
{
	QUAT_GET_GENERIC_DATA();

	QDT_ASSERT((rLerpCoeff >= 0.0f) && (rLerpCoeff <= 1.0f));
	//MATH::Clip( rLerpCoeff, 0.0f, 1.0f) ; 	 	//can't clip because rLerpCoeff is 'const'

	_r[_X_] = rLerpCoeff * Q[_X_];
	_r[_Y_] = rLerpCoeff * Q[_Y_];
	_r[_Z_] = rLerpCoeff * Q[_Z_];
	_r[_W_] = rLerpCoeff * Q[_W_];

	if	(Q[_W_] >= 0)
	{
		_r[_W_] += 1.0f - rLerpCoeff;
	}
	else
	{
		_r[_W_] -= 1.0f - rLerpCoeff;
	}
	if (bNormalize)
	{
		SlowNormalize() ;
	}
}

//=============================================================================
// CODE ENDS HERE
//=============================================================================
